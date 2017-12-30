/**
 *\file c08_process_ctrl.cc
 *\brief 练习进程控制
 *\date 2017/10/02
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/**
 *\brief 测试进程id函数
 *
 *1. 每个进程都有一个非负整型表示的唯一进程ID<br/>
 *2. 进程ID是可以重用，当一个进程终止后，其进程ID就可以再次使用了，大多数UNIX系统实现延迟重用算法，使得赋予新进程的ID不同于最近终止进程所使用的ID。<br/>
 *3. ID为0的进程通常是调度进程，常常被称为交换进程（swapper），该进程是内核的一部分，它并不执行任何磁盘上的程序，因此也被称为系统进程。<br/>
 *4. ID为1的通常是init进程，在自举过程结束时由内核调用，init进程不会终止，它是一个普通的用户进程，但是它以超级用户特权运行<br/>
 *5. 每个UNIX系统实现都有它自己的一套提供操作系统服务的内核进程，例如，在某些UNIX的虚拟存储器实现中，进程ID2是页守护进程（pagedaemon），它负责支持虚拟存储系统的分页操作<br/>
 *
 *<code>
 *
 *	pid_t getpid(void)
 *	返回值：调用进程的进程ID
 *
 *	pid_t getppid(void)
 *	返回值：调用进程的父进程ID
 *
 *	pid_t getuid(void)
 *	返回值：调用进程的实际用户ID
 *
 *	pid_t geteuid(void)
 *	返回值：调用进程的有效用户ID
 *
 *	pid_t getgid(void)
 *	返回值：调用进程的实际组ID
 *
 *	pid_t getegid(void)
 *	返回值：调用进程的有效组ID
 *
 *</code>
 *
 *\warning 这些函数都没有出错返回
 *
 *\retval 0	成功
 *\retval !0 失败
 */
int test_id()
{
	pid_t id = 0;
	
	id = getpid();
	printf("%s:%d 进程id为[%d]\n", __FILE__, __LINE__, id);
	id = getppid();
	printf("%s:%d 父进程id为[%d]\n", __FILE__, __LINE__, id);
	id = getuid();
	printf("%s:%d 进程的实际用户id[%d]\n", __FILE__, __LINE__, id);
	id = geteuid();
	printf("%s:%d 进程的有效用户id[%d]\n", __FILE__, __LINE__, id);
	id = getgid();
	printf("%s:%d 进程的实际组id[%d]\n", __FILE__, __LINE__, id);	
	id = getegid();
	printf("%s:%d 进程的有效组id[%d]\n", __FILE__, __LINE__, id);
	
	return 0;
}

/**
 *\brief 测试fork函数
 *
 *<code>
 *
 *	pid_t fork(void)
 *
 *	返回值：子进程中返回0， 父进程中返回子进程ID， 出错返回-1
 *
 *</code>
 *<table>
 *<caption>父子进程关联性分析</caption>
 *<tr><th width="60">序号</th><th width="100">关联性操作</th><th>说明</th></tr>
 *<tr><td>1</td><td>fork返回值</td><td><ul><li>将子进程ID返回给父进程的理由是，一个进程的子进程可以有多个，并且没有一个函数使一个进程可以获得其所有子进程的进程ID</li><li>子进程得到返回值0的理由是，一个进程只会有一个父进程，所以子进程总是可以调用getppid以获得其父进程的进程ID</li></ul></td></tr>
 *<tr><td>2</td><td>写时复制技术</td><td>子进程和父进程继续执行fork调用之后的指令，子进程是父进程的副本（子进程获得父进程数据空间、堆和栈的副本），这是子进程所拥有的副本。<br/>父、子进程并不共享这些存储空间部分。父、子进程共享正文段。<br/>由于在fork之后经常跟随者exec，所以现在的很多实现并不执行一个父进程数据段、栈和堆的完全复制，作为替代，使用了写时复制（Copy-On-Write, COW)技术。<br/>这些区域由父、子进程共享，而且内核将它们的访问权限改变为只读的，如果父、子进程中的任一个试图修改这些区域，则内核只为修改区域的那块内存制作一个副本，通常是虚拟存储器系统中的一“页”</td></tr>
 *<tr><td>3</td><td>文件共享</td><td>在重定向父进程的标准输出时，子进程的标准输出也被重定向。<br/>实际上，fork的一个特性是父进程的所有打开文件描述符都被复制到子进程中，父、子进程的每个相同的打开描述符共享一个文件表项。<br/>这种共享文件的方式使父、子进程对同一文件使用了一个文件偏移量。在fork之后处理文件描述符有两种常见的情况：<ul><li>父进程等待子进程完成，在这种情况下，父进程无需对其描述符做任何处理，当子进程终止后，它曾进行过读、写操作的任一共享描述符的文件偏移量已执行了相应更新</li><li>父、子进程各自执行不同的程序段，在这种情况下，在fork之后，父、子进程各自关闭它们不需使用的文件描述符，这样就不会干扰对方使用的文件描述符，这种方法是网络服务进程中经常使用的</li></ul></td></tr>
 *<tr><td>4</td><td>继承属性</td><td><ol><li>实际用户ID、实际组ID、 有效用户ID、 有效组ID</li><li>附加组ID</li><li>进程组ID</li><li>会话ID</li><li>控制终端</li><li>设置用户ID标志和设置组ID标志</li><li>当前工作目录</li><li>根目录</li><li>文件模式创建屏蔽字</li><li>信号屏蔽和安排</li><li>针对任一打开文件描述符的在执行时关闭(close-on-exec)标志</li><li>环境</li><li>连接的共享存储段</li><li>存储映射</li><li>资源限制</li></ol></td></tr>
 *<tr><td>5</td><td>差异属性</td><td><ol><li>fork的返回值</li><li>进程ID不同</li><li>两个进程具有不同的父进程ID</li><li>子进程的tms_utime、tms_stime、tms_cutime以及tms_ustime均被设置为0</li><li>父进程设置的文件锁不会被子进程继承</li><li>子进程的未处理的闹钟(alarm)被清除</li><li>子进程的未处理信号集设置为空集</li></ol></td></tr> 
 *</table>
 *
 *\retval 0	成功
 *\retval !0 失败 
 */
int test_fork()
{
	int var = 88;
	static int g = 20;
	pid_t pid = 0;
	int ret = 0;
	const char *buf = "a write to stdout\n";
	
	ret = write(STDOUT_FILENO, buf, strlen(buf));
	if (ret < 0){
		
		printf("%s:%d write failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	printf("%s:%d before fork\n", __FILE__, __LINE__);
	
	if ((pid = fork()) < 0){
		
		// 失败
		printf("%s:%d fork failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		exit(1);
	}else if (pid == 0){
		
		// 子进程
		var++;
		g++;
	}else{
		
		// 父进程
		sleep(12);
	}

/*
这里父进程没有处理子进程的终止状态， 会导致子进程进入僵死状态
root     17330  0.0  0.0  12488   808 pts/2    S+   12:36   0:00  |   |   \_ ./c08_process_ctrl fork
root     17331  0.0  0.0      0     0 pts/2    Z+   12:36   0:00  |   |       \_ [c08_process_ctr] <defunct>	
*/
	printf("%s:%d pid[%d] ppid[%d] g[%d] var[%d]\n", 
			__FILE__, __LINE__, getpid(), getppid(), g, var);
	
	return 0;
}

static void print_status(int status);

/**
 *\brief 测试wait函数
 *
 *<table>
 *<caption>进程终止 父子进程变化</caption>
 *<tr><th width="40">序号</th><th width="100">行为</th><th width="400">描述</th></tr>
 *<tr><td>1</td><td>产生终止状态</td><td><ul><li>不管进程如何终止， 最后都会执行内核中的同一段代码。这段代码为相应进程关闭所有打开描述符，释放它所使用的存储器等</li><li>对于任意一种终止情况(正常或异常)，我们都希望终止进程能够通知其父进程它是如何终止的。对于三个终止函数（exit、_exit和_Exit)，实现这一点的方法是， 将其推出状态作为参数传递给函数。在异常终止情况下，内核（不是进程本身）产生一个指示其异常终止原因的终止状态。在任意一种情况下，该终止进程的父进程都能用wait或waitpid函数取得其终止状态</li></ul></td></tr>
 *<tr><td>2</td><td>init领养子进程</td><td>对于父进程已经终止的所有进程，它们的父进程都改变为init进程，我们称这些进程由init进程领养，操作过程大致如下，在一个进程终止时，内核逐个检查所有活动进程，以判断它是否是正要终止进程的子进程，如果是，则将该进程的父进程ID更改为1，这种处理方法保证了每个进程都有一个父进程</td></tr>
 *<tr><td>3</td><td>产生僵死进程</td><td>子进程在父进程之前终止，那么父进程又如何能在做相应检查时得到子进程的终止状态？<br/>内核为每个终止进程保存了一定量的信息， 所以当终止进程的父进程调用wait或waitpid时，可以得到这些信息，这些信息至少包括进程ID、该进程的终止状态、以及该进程使用的CPU时间总量，内核可以释放终止进程所使用的所有存储区，关闭其所有打开文件。<br/>一个已经终止、但是其父进程尚未对其进行善后处理的进程被称为僵死进程（zombie）。如果编写一个长期运行的程序，它调用fork产生了很多子进程，那么除非父进程等待取得子进程的终止状态，否则这些子进程终止后就会变成僵死进程</td></tr>
 *<tr><td>4</td><td>发送SIGCHLD信号</td><td>当一个进程正常或异常终止时，内核就向其父进程发送SIGCHLD信号，因为进程终止是个异步事件，所以这种信号也是内核向父进程发的异步通知。父进程可以选择忽略该信号， 或者提供一个信号处理函数， 对于这种信号系统的默认动作是忽略它</td></tr>
 *</table>
 *
 *<code>
 *
 *	pid_t wait(int *statloc);
 *	pid_t waitpid(pid_t pid, int *statloc, int options);
 *
 *	返回值：若成功则返回进程ID，出错则返回-1
 *	这两个函数的参数statloc是一个整型指针，
 *		如果statloc不是一个空指针，则终止进程的终止状态存放在它所指向的单元内，
 *		如果不关心终止状态， 则可将该参数指针定为空指针
 *</code>
 * 
 *调用wait或waitpid的进程可能会发生的情况：
 *<ol>
 *<li>如果其所有子进程都还在运行，则阻塞</li>
 *<li>如果一个子进程已终止，正等待父进程获取其终止状态，则取得该子进程的终止状态立即返回</li>
 *<li>如果它没有任何子进程，则立即出错返回</li>
 *</ol>
 *
 *这两个函数的区别如下：
 *<ol>
 *<li>在一个子进程终止前， wait使其调用者阻塞，而waitpid有一个选项，可使调用者不阻塞</li>
 *<li>waitpid并不等待在其调用之后的第一个终止子进程，它有若干个选项，可以控它所等待的进程</li>
 *</ol>
 *<table>
 *<caption>检查wait和waitpid所返回的终止状态宏</caption>
 *<tr><th width="40">序号</th><th width="100">宏</th><th>描述</th></tr>
 *<tr><td>1</td><td>WIFEXITED(status)</td><td>若为正常终止子进程返回的状态，则为真<br/>对于这种情况可执行WEXITSTATUS(status)，取子进程传送给exit, _exit或_Exit参数的低8位</td></tr>
 *<tr><td>2</td><td>WIFSIGNALED(status)</td><td>若为异常终止子进程返回的状态，则为真（接到一个不捕捉的信号）<br/>对于这种情况，可以执行WTERMSIG(status)，取得子进程终止的信号编号，另外，有些实现定义宏WCOREDUMP(status)，若已产生终止进程的core文件， 则它返回真</td></tr>
 *<tr><td>3</td><td>WIFSTOPPED(status)</td><td>若为当前暂停子进程的返回的状态，则为真<br/>对于这种情况， 可执行WSTOPSIG(status)， 取得子进程暂停的型号编号</td></tr>
 *<tr><td>4</td><td>WIFCONTINUED(status)</td><td>若在作业控制暂停后已经继续的子进程返回了状态， 则为真</td></tr>
 *</table>
 *<br/>
 *<table>
 *<caption>waitpid函数中pid参数的作用</caption>
 *<tr><th width="40">序号</th><th width="100">值</th><th>说明</th></tr>
 *<tr><td>1</td><td>pid == -1</td><td>等待任一子进程，就这一方面而言，waitpid与wait等效</td></tr>
 *<tr><td>2</td><td>pid > 0</td><td>等待其进程ID与pid相等的子进程</td></tr>
 *<tr><td>3</td><td>pid == 0</td><td>等待其组ID等于调用进程组ID的任一子进程</td></tr>
 *<tr><td>4</td><td>pid < -1</td><td>等待其组ID等于pid绝对值的任一子进程</td></tr>
 *</table>
 *<br/>
 *<table>
 *<caption>waitpid的options常量</caption>
 *<tr><th width="40">序号</th><th width="100">常量</th><th>描述</th></tr> 
 *<tr><td>1</td><td>WCONTINUED</td><td>若实现支持作业控制，那么由pid指定的任一子进程在暂停后已经继续，但其状态尚未报告，则返回其状态</td></tr>
 *<tr><td>2</td><td>WNOHANG</td><td>若由pid指定的子进程并不是立即可用的，则waitpid不阻塞，此时其返回值为0</td></tr>
 *<tr><td>3</td><td>WUNTRACED</td><td>若某实现支持作业控制，而由pid指定的任一子进程已处于暂停状态，并且其状态自暂停以来还未报告过，则返回其状态，WIFSTOPPED宏确定返回值是否对应于一个暂停子进程</td></tr>
 *</table>
 *
 *waitpid函数返回终止子进程的进程ID，并将该子进程的终止状态存放在由statloc指向的存储单元中，对于wait，其唯一的出错是调用进程没有子进程（函数调用被一个信号中断时，也可能返回另一种出错），但是对于waitpid，如果指定的进程或进程组不存在，或者参数pid指定的进程不是调用进程的子进程则都将出错， waitpid函数提供了wait函数没有的三个功能：
 *<ol>
 *<li>waitpid可等待一个特定的进程，而wait则返回任一终止子进程的状态</li>
 *<li>waitpid提供了一个wait的非阻塞版本，有时用户希望取得一个子进程的状态，但不想阻塞</li>
 *<li>waitpid支持作业控制（利用WUNTRACED和WCONTINUED选项）</li>
 *</ol>
 *
 *\retval 0	成功
 *\retval !0 失败 
 */
int test_wait()
{
	pid_t pid;
	int status;
	int div = 0;
	
	if ((pid = fork()) < 0){
		
		printf("%s:%d fork failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}else if (pid == 0){
		
		exit(7);	// 子进程退出状态
	}
	
	if (wait(&status) != pid){ // 等待子进程退出
	
		printf("%s:%d wait failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -2;
	}
	print_status(status);
	
	if ((pid = fork()) < 0){
		
		printf("%s:%d fork failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}else if (pid == 0){
		
		abort();	// 子进程退出状态
	}
	
	if (wait(&status) != pid){ // 等待子进程退出
	
		printf("%s:%d wait failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -2;
	}
	print_status(status);	
	
	if ((pid = fork()) < 0){
		
		printf("%s:%d fork failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}else if (pid == 0){
		
		status /= div;	// 子进程退出状态
	}
	
	if (wait(&status) != pid){ // 等待子进程退出
	
		printf("%s:%d wait failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -2;
	}
	print_status(status);
	
	
	// 调用fork两次避免僵死进程
	if ((pid = fork()) < 0){
		
		printf("%s:%d fork failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;		
	}else if (pid == 0){
		
		if ((pid = fork()) < 0){
			
			printf("%s:%d fork failed(%s)\n", __FILE__, __LINE__, strerror(errno));
			return -1;				
		}else if (pid > 0){
			
			exit(0);
		}
		
		// 这是第二个子进程  父进程退出后会被init领养
		sleep(5);
		printf("%s:%d 第二个子进程， ppid[%d]\n", __FILE__, __LINE__, getppid());
		exit(0);
	}
	
	if (waitpid(pid, NULL, 0) != pid){
		
		printf("%s:%d waitpid failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -2;
	}
	
	return 0;
}

static void print_status(int status)
{
	if (WIFEXITED(status)){
		
		printf("正常终止， 退出状态码 %d\n", WEXITSTATUS(status));
	}else if (WIFSIGNALED(status)){
		
		printf("异常终止， 信号值为 %d%s\n", WTERMSIG(status), 
#ifdef	WCOREDUMP
				WCOREDUMP(status) ? " (产生core文件)" : "");
#else
				"");
#endif
	}else if (WIFSTOPPED(status)){
		
		printf("子进程停止，信号值为 %d\n", WSTOPSIG(status));
	}
}

/**
 *\brief 测试waitid函数
 *
 *<code>
 *
 *	int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
 *
 *	返回值：若成功则返回0， 若出错则返回-1
 *
 *</code>
 *
 *与waitpid相似， waitid允许一个进程指定要等待的子进程，但它使用单独的参数表示要等待的子进程的类型，而不是将此与进程ID或进程组ID组合成一个参数。
 *<table>
 *<caption>waitid的idtype常量</caption>
 *<tr><th width="40">序号</th><th width="100">常量</th><th>描述</th></tr>
 *<tr><td>1</td><td>P_PID</td><td>等待一个特定的进程，id包含要等待子进程的进程ID</td></tr>
 *<tr><td>2</td><td>P_PGID</td><td>等待一个特定进程组中的任一子进程，id包含要等待子进程的进程组ID</td></tr>
 *<tr><td>3</td><td>P_ALL</td><td>等待任一子进程，忽略id</td></tr> 
 *</table>
 *options参数是下表中各标志的按位“或”，这些标志指示调用者关注那些状态变化
 *<table>
 *<caption>waitid的options常量</caption>
 *<tr><th width="40">序号</th><th width="100">常量</th><th>描述</th></tr>
 *<tr><td>1</td><td>WCONTINUED</td><td>等待一个进程，它以前曾被暂停，此后又已继续，但其状态尚未报告</td></tr>
 *<tr><td>2</td><td>WEXITED</td><td>等待已退出的进程</td></tr>
 *<tr><td>3</td><td>WNOHANG</td><td>如无可用的子进程退出状态，立即返回而非阻塞</td></tr>
 *<tr><td>4</td><td>WNOWAIT</td><td>不破坏子进程退出状态，该子进程退出状态可由后续的wait、waitid或waitpid调用取得</td></tr>
 *<tr><td>5</td><td>WSTOPPED</td><td>等待一个进程，它已经暂停，但其状态尚未报告</td></tr> 
 *</table>
 *\retval 0	成功
 *\retval !0 失败  
 */
int test_waitid()
{
	return 0;
}

/**
 *\brief 测试exec函数
 *
 *<code>
 *
 *	int execl(const char *pathname, const char *arg0, ..., (char*)0);
 *	int execv(const char *pathname, char *const argv[]);
 *	int execle(const char *pathname, const char *arg0, ..., (char*)0, char *const envp[]);
 *	int execve(const char *pathname, char *const argv[], char *const envp[]);
 *	int execlp(const char *filename, const char *arg0, ..., (char*)0);
 *	int execvp(const char *filename, char *const argv[]);
 *
 *	返回值：若出错返回-1， 若成功则不返回值
 *</code>
 *当进程调用一种exec函数时，该进程执行的程序完全替换为新程序，而新程序则从其main函数开始执行，因为调用exec并不创建新进程，所以前后的进程ID并未改变，exec指示用一个全新的程序替换了当前进程的正文、数据、堆和栈段<br/>
 *<ol>
 *<li>在这一系列的exec函数中， 前4个取路径名作为参数，后两个则取文件名作为参数
 *<ul><li>如果filename中包含/，则将其视为路径名</li><li>否则就按PATH环境变量，在它所指定的各个目录中搜寻可执行文件</li></ul>
 *</li>
 *<li>在这一系列的exec函数中， 传递参数的方式是不同的，函数名中含有l的要求每个命令行参数都说明为一个单独的参数。函数名中含有v的在传递参数时，需要先构造一个指向各参数的指针数组，然后将该数组地址作为这位函数的参数</li>
 *<li>在这一系列的exec函数中，向新程序传递环境表存在差异，以e结尾的两个函数可以传递一个指向环境字符串指针数组的指针， 其他四个函数则使用调用进程中的environ变量为新程序复制现有的环境</li>
 *</ol>
 *
 *\retval 0	成功
 *\retval !0 失败   
 */
int test_exec()
{
	pid_t pid;
	extern char **environ;
	
	if ((pid = fork()) < 0){
		
		printf("%s:%d fork failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}else if (pid == 0){
		
		// 指定路径 和 环境变量
		if (execle("/home/admin/c08_process_ctrl", "c08_process_ctrl", "id", NULL, environ) < 0){
			
			printf("%s:%d excle failed(%s)\n", __FILE__, __LINE__, strerror(errno));
			return -2;
		}
	}
	
	if (waitpid(pid, NULL, 0) < 0){
		
		printf("%s:%d waitpid failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	
	if ((pid = fork()) < 0){
		
		printf("%s:%d fork failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;		
	}else if (pid == 0){
		
		// 这个地方需要自己 在外边设置下PATH路径
		if (execlp("c08_process_ctrl", "c08_process_ctrl", "wait", NULL) < 0){
			
			printf("%s:%d excle failed(%s)\n", __FILE__, __LINE__, strerror(errno));
			return -2;			
		}
	}
	
	return 0;
}

static void print_times(clock_t real, struct tms *tmsstart, struct tms *tmsend);
/**
 *\brief 测试times函数
 *
 *<code>
 *
 *	clock_t times(struct tms *buf);
 *
 *	返回值：若成功则返回流逝的墙上时钟时间（单位，时钟滴答数），若出错则返回-1
 *
 *	struct tms{
 *		 clock_t	tms_utime;	// user CPU time
 *		 clock_t	tms_stime;	// system CPU time
 *		 clock_t	tms_cutime;	// user CPU time, terminated children
 *		 clock_t	tms_cstime;	// system CPU time, terminated children
 *	};
 *</code>
 *
 *<ol>
 *<li>
 *任一进程都可调用times函数已获得它自己及已终止子进程的 墙上时钟时间、用户CPU时间和系统CPU时间
 *</li>
 *<li>
 *struct tms结构体中没有包含墙上时钟时间的任何测量值，作为替代， times函数返回墙上时钟时间作为其函数值。此值是相对于过去的某一时刻测量的， 所以不能用其绝对值， 而必须使用其相对值
 *</li>
 *<li>所有由此函数返回的clock_t值都用_SC_CLK_TCK(由sysconf函数返回的每秒时钟滴答数)变换成秒数</li></ol>
 *
 *\param[in] cmd 执行的命令用于system调用
 *
 *\retval 0	成功
 *\retval !0 失败   
 */
int test_times(const char *cmd)
{
	struct tms tmsstart, tmsend;
	clock_t start, end;
	int status;
	
	printf("%s:%d cmd[%s]\n", __FILE__, __LINE__, cmd);
	if ((start = times(&tmsstart)) == -1){
		
		printf("%s:%d times failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	
	if ((status = system(cmd)) < 0){
		
		printf("%s:%d system failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	
	if ((end = times(&tmsend)) == -1){
		
		printf("%s:%d times failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;		
	}
	
	print_times(end - start, &tmsstart, &tmsend);
	print_status(status);
	
	return 0;
}

static void print_times(clock_t real, struct tms *tmsstart, struct tms *tmsend)
{
	static long clktck = 0;
	
	if (clktck == 0){
		
		if ((clktck = sysconf(_SC_CLK_TCK)) < 0){
			
			printf("%s:%d sysconf failed(%s)\n", __FILE__, __LINE__, strerror(errno));
			return;
		}
	}
	
	printf("    real:    %7.2f\n", real / (double) clktck);
	printf("    user:    %7.2f\n", (tmsend->tms_utime - tmsstart->tms_utime) / (double) clktck);
	printf("    sys:    %7.2f\n", (tmsend->tms_stime - tmsstart->tms_stime) / (double) clktck);
	printf(" child user:    %7.2f\n", (tmsend->tms_cutime - tmsstart->tms_cutime) / (double) clktck);
	printf(" child sys:    %7.2f\n", (tmsend->tms_cstime - tmsstart->tms_cstime) / (double) clktck);	
}

static void show_help()
{
	printf("id, 测试进程相关的id信息\n\n"
			"fork, 测试fork函数\n\n"
			"wait, 测试wait函数\n\n"
			"exec, 测试exec函数\n\n"
			"times system_cmd, 测试times函数\n\n");
}

int main(int argc, char **argv)
{
	if (argc == 2 && strcmp(argv[1], "id") == 0){
		
		test_id();
	}else if (argc == 2 && strcmp(argv[1], "fork") == 0){
		
		test_fork();
	}else if (argc == 2 && strcmp(argv[1], "wait") == 0){
		
		test_wait();
	}else if (argc == 2 && strcmp(argv[1], "exec") == 0){
		
		test_exec();
	}else if (argc == 3 && strcmp(argv[1], "times") == 0){
		
		test_times(argv[2]);
	}else{
		
		show_help();
	}
	
	return 0;
}