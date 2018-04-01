/**
 *\file c13_daemon.cc
 *\brief 练习守护进程
 *\date 2018/04/01
 */

//#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <signal.h>

/**
 *\brief 测试守护进程
 *
 *守护进程也称作精灵进程是生存期较长的一种进程。它们常常在系统自举时启动，仅在系统关闭时才终止。
 *因为它们没有控制终端， 所以说它们是在后台运行的。
 *
 *系统进程依赖于操作系统实现。父进程ID为0的各进程通常是内核进程，它们作为系统自举过程的一部分而启动。内核进程是特殊的，通常存在于系统的整个生命期中。它们以超级用户特权运行，无控制终端，无命令行。
 *
 *大多数守护进程都以超级用户（用户ID为0）特权运行。
 *没有一个守护进程具有控制终端，其终端名设置为问号（？），终端前台进程组ID设置为-1。
 *内核守护进程以无控制终端方式启动。
 *用户层守护进程缺少控制终端可能是守护进程调用了setsid的结构。
 *所有用户层守护进程都是进程组的组长进程以及会话的首进程，而且是这些进程组和会话中的唯一进程。最后，应当引起注意的是大多数守护进程的父进程是init进程。
 *
 *<ol>
 *<li>init，通常经常ID为1，它是一个系统守护进程，负责启动各运行层次特定的系统服务。这些服务通常是在它们自己拥有的守护进程的帮助下实现的。</li>
 *<li>keventd，守护进程为在内核中运行计划执行的函数提供进程上下文。</li>
 *<li>kapmd，守护进程对很多计算机系统中具有的高级电源管理提供支持。</li>
 *<li>kswapd，守护进程也称为页面调出守护进程，它通过将脏页面以低速写到磁盘上从而使这些页面在需要时仍可回收使用，这种方式支持虚存子系统。</li>
 *<li>bdflush，守护进程将高速缓存中的数据冲洗到磁盘上，当可用内存达到下线时，守护进程将脏缓冲区从缓冲池（buffer cache）中冲洗到磁盘上。</li>
 *<li>kupdated，守护进程将高速缓存中的数据冲洗到磁盘上， 每隔一定时间间隔，守护进程将脏页面冲洗到磁盘上，以便在系统失效时减少丢失的数据。</li>
 *<li>portmap，守护进程为端口映射守护进程，它提供将RPC（Remote Procedure Call，远程过程调用）系统号映射为网络端口号的服务。</li>
 *<li>syslogd，守护进程可由操作人员把系统消息记入日志的任何程序使用。</li>
 *<li>inetd，守护进程侦听系统网络接口，以便取得来自网络的对各种网络服务进程的请求。</li>
 *<li>nfsd,lockd和rpciod，守护进程提供对网络文件系统（Network File System, NFS）的支持。</li>
 *<li>crond，守护进程在指定的日期和时间执行指定的命令。</li>
 *<li>cupsd，守护进程是打印假脱机进程，它处理对系统提出的所有打印请求。</li>
 *</ol>
 *
 *<table>
 *<caption> 编程规则 </caption>
 *<tr><th width="40">序号</th><th width="300">描述</th><th>说明</th></tr>
 *<tr><td>1</td><td>重新设置文件模式创建屏蔽字</td><td>调用umask将文件模式创建屏蔽字设置为0。由继承得来的文件模式创建屏蔽字可能会拒绝设置某些权限。例如，若守护进程要创建一个组可读、写的文件，而继承的文件模式创建屏蔽字可能屏蔽了这两种权限，于是所要求的组可读、写就不能起作用。</td></tr>
 *<tr><td>2</td><td>创建子进程并终止父进程</td><td>调用fork，然后使父进程退出（exit）。这样做实现了下面几点：
 <ol>
 <li>如果该守护进程是作为一条简单shell命令启动的，那么父进程终止使得shell认为这条命令已经执行完毕</li>
 <li>子进程继承了父进程的进程组ID，但具有一个新的进程ID，这就保证了子进程不是一个进程组的组长进程。</li>
 </ol></td></tr>
 *<tr><td>3</td><td>创建一个新会话</td><td>调用setsid以创建一个新会话。使调用进程称为新会话的首进程，成为一个新进程组的组长进程，没有控制终端</td></tr>
 *<tr><td>4</td><td>更改工作目录</td><td>将当前工作目录更改为根目录。从父进程继承过来的当前工作目录可能在一个装配文件系统中。因为守护进程通常在系统再引导之前是一直存在的，所以如果守护进程的当前工作目录在一个装配文件系统中，那么该文件系统就不能被卸载。这与装配文件系统的原意不符。</td></tr>
 *<tr><td>5</td><td>关闭不需要的文件描述符</td><td>关闭不再需要的文件描述符，这使守护进程不再持有从父进程继承来的某些文件描述符。可以使用getrlimit函数来获取最大支持的描述符，然后统一关掉这些描述符。</td></tr>
 *<tr><td>6</td><td>标准输入、输出和出错关联/dev/null设备</td><td>某些守护进程打开/dev/null使其具有文件描述符0、1和2，这样，任何一个试图读标准输入、写标准输出或标准出错的库例程都不会产生任何效果。因为守护进程并不与终端设备关联，所以不能在终端设备上显示其输出，也无处从交互式用户那里接收输入。即使守护进程是从交互式会话启动的，但因为守护进程是在后台运行的，所以登录会话的终止并不影响守护进程。如果其他用户在同一个终端设备上登录，我们也不会在该终端上见到守护进程的输出，用户也不可期望他们在终端上的输入会由守护进程读取。</td></tr>
 *</table>
 *\retval 0 成功
 *\retval !0 失败
 */
int test_daemon()
{
	mode_t old_mask;
	struct rlimit rl;
	struct sigaction sa;
	pid_t pid;
	
	// 设置文件创建屏蔽字
	old_mask = umask(0);
	old_mask = old_mask;
	
	// 或的最大支持的文件描述符句柄数量
	if (getrlimit(RLIMIT_NOFILE, &rl)){
		
		printf("%s:%d getrlimit max file failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	
	// 成为一个新的会话， 将失去绑定的终端
	if ((pid = fork()) < 0){
		
		printf("%s:%d fork failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -2;
	}else if (pid > 0){
		
		exit(0);
	}
	setsid();
	
	// 忽略会话首进程退出发送的SIGHUP信号
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL)){
		
		printf("%s:%d sigaction failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -3;
	}
	if ((pid = fork()) < 0){
		
		printf("%s:%d fork failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -4;		
	}else if (pid > 0){
		
		exit(0);
	}
	
	// 修改当前的工作目录
	if (chdir("/")){
		
		printf("%s:%d chdir failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -5;
	}
	
	// 关掉不再使用的文件句柄
	if (rl.rlim_max == RLIM_INFINITY){
		
		rl.rlim_max = 1024;
	}
	for (unsigned int i = 0; i < rl.rlim_max; ++i){
		
		close(i);
	}
	
	// 关联/dev/null 到标注输入 输出 出错
	int fd0, fd1, fd2;
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);
	fd0 = fd0;
	fd1 = fd1;
	fd2 = fd2;
	
	while (true){
		
		printf("hello\n");
			
		sleep(1);
	}
	
	return 0;
/*
# ps  --pid=26420 -o ppid,pid,pgid,sid,tty,tpgid,uid,command
 PPID   PID  PGID   SID TT       TPGID   UID COMMAND
# ps  --pid=26421 -o ppid,pid,pgid,sid,tty,tpgid,uid,command
 PPID   PID  PGID   SID TT       TPGID   UID COMMAND
    1 26421 26420 26420 ?           -1     0 ./c13_daemon daemon

通过ps命令可以看到， 没有一个活动进程的ID是26420。这意味着，我们的守护进程在一个孤儿进程组中，它不是会话首进程，于是不会有机会分配到一个控制终端。这是程序代码中执行第二个fork造成的。由此可以见到，次守护进程已经被正确地初始化了。	
*/
}

/**
 *\brief 测试单实例守护进程
 *
 *为了正常运作， 某些守护进程实现为单实例的，也就是在任一时刻只运行该守护进程的一个副本。
 *
 *文件锁和记录锁机制是一种方法的基础，该方法用来保证一个守护进程只有一个副本在运行。如果每一个守护进程创建一个文件，并且在整个文件上加上一把写锁，那么只允许创建一把这样的写锁，所以在此之后如试图再创建一把这样的写锁就将失败，以此想后续守护进程副本知名已有一个副本正在运行。
 *
 *文件和记录锁提供了一种方便的互斥机制，如果守护进程在整个文件上得到一把写锁，那么在该守护进程终止时， 这把锁将被自动删除。这就简化了复原所需的处理，去除了对以前的守护进程实例需要进行清理的有关操作。
 *
 *<table>
 *<caption>守护进程的惯例</caption>
 *<tr><th width="40">序号</th><th width="100">描述</th><th>说明</th></tr>
 *<tr><td>1</td><td>单实例文件锁</td><td>若守护进程使用锁文件，那么该文件通常存放在/var/run目录中。守护进程可能需要具有超级用户权限才能在此目录下创建文件。锁文件的名字通常是name.pid，其中name是该守护进程或服务的名字。例如，cron守护进程锁文件的名字是/var/run/crond.pid</td></tr>
 *<tr><td>2</td><td>配置文件路径</td><td>若守护进程支持配置选项，那么配置文件通常存放在/etc目录中。配置文件的名字通常是name.conf，其中，name是该守护进程或服务的名字。例如syslogd守护进程的配置文件是/etc/syslog.conf。</td></tr>
 *<tr><td>3</td><td>启动方式</td><td>守护进程可用命令行启动，但通常它们是由系统初始化脚本启动的。如果在守护进程终止时， 应当自动地重新启动它，我们可在/etc/inittab中为该守护进程包括_respawn记录项，这样，init就将重启动该守护进程。</td></tr>
 *<tr><td>4</td><td>配置文件更新</td><td>若守护进程有配置文件， 那么当该守护进程启动时， 它加载配置文件， 但在此后一般就不会再查看它。若管理员更改了配置文件， 那么该守护进程可能需要被停止， 然后再启动， 以使配置文件的更改生效。为避免这种麻烦，某些守护进程将捕捉SIGHUP信号，当它们接受到该信号时，重读配置文件。因为守护进程并不与终端相结合，它们或者是无控制终端的会话首进程，或者是孤儿进程组的成员， 所以守护进程并不期望接受SIGHUP。于是，它们可以安全地重复使用它。</td></tr> 
 *</table>
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_single_daemon()
{
	const char *LOCKFILE = "/var/run/daemon.pid";
	const mode_t LOCKMODE = (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	
	int fd;
	char buf[16];
	
	fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
	if (fd < 0){
		
		printf("%s:%d open failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	
	if (flock(fd, LOCK_EX) < 0){
		
		if (errno == EACCES || errno == EAGAIN){
			
			close(fd);
			return -2;
		}
		
		printf("%s:%d can't lock file[%s], %s\n", __FILE__, __LINE__,  LOCKFILE, strerror(errno));
		exit(1);
	}
	
	ftruncate(fd, 0);
	sprintf(buf, "%ld", (long)getpid());
	write(fd, buf, strlen(buf) + 1);
	
	sleep(20);
	
	return 0;
}

static void show_help()
{
	printf("daemon, 测试daemon进程\n\n"
		"single, 测试单实例\n\n");
}

int main(int argc, char **argv)
{
	if (argc == 2 and strcmp(argv[1], "daemon") == 0){
		
		test_daemon();
	}else if (argc == 2 and strcmp(argv[1], "single") == 0){
		
		test_single_daemon();
	}else{
		
		show_help();
	}
	
	return 0;
}