/**
 *\file c10_signal.cc
 *\brief 练习信号
 *\date 2017/10/07
 */

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

static void deal_signal(int signo)
{
#ifndef SIGRTMIN
#define	SIGRTMIN (34)
#endif
	printf("access signo is [%d]\n", signo);
	switch (signo)
	{
		case SIGINT:
		printf("SIGINT\n");
		break;
		case SIGTSTP:
		printf("SIGTSTP\n");
		break;
		case SIGQUIT:
		printf("SIGQUIT\n");
		break;
		case SIGTERM:
		printf("SIGTERM\n");
		break;
		case SIGALRM:
			struct passwd *pw;
			
			printf("alarm signal handler\n");
			if ((pw = getpwnam("root")) == NULL){
				
				printf("getpwnam(root) error\n");
			}else{
				    
				printf("pw->pw_name = %s\n", pw->pw_name);  
				printf("pw->pw_passwd = %s\n", pw->pw_passwd);  
				printf("pw->pw_uid = %d\n", pw->pw_uid);  
				printf("pw->pw_gid = %d\n", pw->pw_gid);  
				printf("pw->pw_gecos = %s\n", pw->pw_gecos);  
				printf("pw->pw_dir = %s\n", pw->pw_dir);  
				printf("pw->pw_shell = %s\n", pw->pw_shell);  
			}
			alarm(1);
			break;
		default:
		if (signo == SIGRTMIN){
		
			printf("SIGRTMIN\n");
		}
		break;
	}
	sleep(2);
}

/**
 *\brief 测试signal函数
 *
 *<code>
 *void (*signal)(int signo, void (*func)(int)))(int);
 *
 *返回值：若成功则返回信号以前的处理配置， 若出错则返回SIG_ERR
 *</code>
 *
 *signo是下面列出的信号列表名。 
 *
 *<table>
 *<caption>signal中func的分类</caption>
 *<tr><th width="40">序号</th><th width="60">分类</th><th>说明</th></tr>
 *<tr><td>1</td><td>SIG_IGN</td><td>则向内核表示忽略此信号（记住有两个信号SIGKILL和SIGSTOP不能忽略）</td></tr>
 *<tr><td>2</td><td>SIG_DFL</td><td>则表示接到此信号后的动作时系统默认动作</td></tr>
 *<tr><td>3</td><td>函数地址</td><td>则在信号发生时，调用该函数。我们称这种处理为“捕捉”该信号。称此函数为信号处理程序或信号扑捉函数</td></tr>
 *</table>
 *
 *<ul>
 *<li>信号是软件中断</li>
 *<li>信号提供了一种处理异步事件的方法</li>
 *<li>每个信号都有一个名字，这些名字都以三个字符SIG开头</li>
 *<li>信号都被定义为正整数（信号编号）</li>
 *<li>不存在编号为0的信号，kill函数对信号编号0有特殊应用，称这种信号为空信号</li>
 *</ul>
 *
 *<table>
 *<caption>UNIX系统信号列表</caption>
 *<tr><th width="40">序号</th><th width="60">名字</th><th>描述</th><th width="100">默认动作</th></tr>
 *<tr><td>1</td><td>SIGABRT</td><td>异常终止(abort)<br/>调用abort函数时产生此信号，进程异常终止</td><td>终止+core</td></tr>
 *<tr><td>2</td><td>SIGALRM</td><td>超时（alarm）<br/>在调用alarm函数设置的计时器超时，产生此信号，若由setitimer函数设置的间隔时间超时时，也会产生此信号</td><td>终止</td></tr>
 *<tr><td>3</td><td>SIGBUS</td><td>硬件故障<br/>指示一个实现定义的硬件故障，当出现某些类型的内存故障时，实现常常产生此种信号</td><td>终止+core</td></tr>
 *<tr><td>4</td><td>SIGCANCEL</td><td>线程库内部使用<br/>这是Solaris线程库内部使用的信号，它不供一般应用</td><td>忽略</td></tr>
 *<tr><td>5</td><td>SIGCHLD</td><td>子进程状态改变<br/>在一个进程终止或停止时，将SIGCHLD信号发送给其父进程，系统默认是忽略。如果父进程希望被告知其子进程的这种改变，则应捕捉此信号，捕捉函数中通常调用一种wait函数以取得子进程ID和其终止状态</td><td>忽略</td></tr>
 *<tr><td>6</td><td>SIGCONT</td><td>使暂停进程继续<br/>此作业控制信号被发送给需要继续运行，但当前处于停滞状态的进程。如果接收到此信号的进程处于停止状态，则系统默认动作是使该进程继续运行，否则默认动作是忽略此信号。</td><td>继续/忽略</td></tr>
 *<tr><td>7</td><td>SIGEMT</td><td>硬件故障</td><td>终止+core</td></tr>
 *<tr><td>8</td><td>SIGFPE</td><td>算术异常<br/>例如除以0，浮点溢出等</td><td>终止+core</td></tr>
 *<tr><td>9</td><td>SIGFREEZE</td><td>检查点冻结<br/>此信号仅由Solaris定义，它用于通知进程在冻结系统状态之前需要采取特定动作</td><td>忽略</td></tr>
 *<tr><td>10</td><td>SIGHUP</td><td>连接断开<br/>如果中断接口检测到一个连接断开，则将此信号发送给与该终端相关的控制进程（会话首进程）</td><td>终止</td></tr>
 *<tr><td>11</td><td>SIGILL</td><td>非法硬件指令<br/>此信号指示进程已执行一条非法硬件指令</td><td>终止+core</td></tr>
 *<tr><td>12</td><td>SIGINFO</td><td>键盘状态请求<br/>这是一种BSD信号，当用户按状态键（一般采用Ctrl+T）时，终端驱动程序产生此信号并送至前台进程组中的每一个进程， 此信号通常导致在终端上显示前台进程组中各进程的状态信息</td><td>终止</td></tr>
 *<tr><td>13</td><td>SIGINT</td><td>终端中断符<br/>当用户按中断键（一般采用DELETE或Ctrl+C）时，终端驱动程序产生此信号并送至前台进程组中的每一个进程， 当一个进程在运行时失控， 特别是它正在屏幕上产生大量不需要的输出时，常用此信号终止它</td><td>终止</td></tr>
 *<tr><td>14</td><td>SIGIO</td><td>异步I/O<br/>此信号指示一个异步I/O事件</td><td>忽略/终止</td></tr>
 *<tr><td>15</td><td>SIGIOT</td><td>硬件故障</td><td>终止+core</td></tr>
 *<tr><td>16</td><td>SIGKILL</td><td>终止<br/>这是两个不能被捕捉或忽略的信号之一， 它向系统管理员提供了一种可以杀死任一进程的可靠方法</td><td>终止</td></tr>
 *<tr><td>17</td><td>SIGLWP</td><td>线程库内部使用</td><td>忽略</td></tr>
 *<tr><td>18</td><td>SIGPIPE</td><td>写至无读进程的管道<br/>如果在写到管道时读进程已终止，则产生此信号。当类型为SOCK_STREAM的套接字已不再连接时， 进程写到该套接字也产生此信号</td><td>终止</td></tr>
 *<tr><td>19</td><td>SIGPOLL</td><td>可轮询事件（poll）</td><td>终止</td></tr>
 *<tr><td>20</td><td>SIGPROF</td><td>梗概时间超时（setitimer）<br/>当setitimer(2)函数设置的梗概统计计时器（profiling interval timer）已到期时产生次信号</td><td>终止</td></tr>
 *<tr><td>21</td><td>SIGPWR</td><td>电源失效/重启动<br/>这是一种依赖于系统信号。它主要用于有不间断电源(UPS)的系统。如果电源失效，则UPS起作用，而且通常软件会接到通知。在这种情况下，系统依靠蓄电池电源继续运行，所以无须任何处理。但是如果蓄电池也将不能支持工作，则软件通常会再次接到通知，此时，系统必须在15~30秒内使其各部分都停止运行。此时应当发送SIGPWR信号。在大多数系统中，接到蓄电池电压过低信息的进程将信号SIGPWR发送给init进程，然后由init处理停机操作。</td><td>终止/忽略</td></tr>
 *<tr><td>22</td><td>SIGQUIT</td><td>终端退出符<br/>当用户在终端上按退出键（一般采用Ctrl+\）时，产生此信号，并发送至前台进程组中的所有进程， 此信号不仅会终止前台进程组， 同时还会产生一个core文件</td><td>终止+core</td></tr>
 *<tr><td>23</td><td>SIGSEGV</td><td>无效内存引用<br/>该信号指示进程进行了一次无效内存引用</td><td>终止+core</td></tr>
 *<tr><td>24</td><td>SIGSKFLT</td><td>协处理器栈故障</td><td>终止</td></tr>
 *<tr><td>25</td><td>SIGSTOP</td><td>停止<br/>这是一个作业控制信号，用于停止一个进程它类似于交互停止信号（SIGTSTP），但是SIGSTOP不能被捕捉或忽略</td><td>暂停进程</td></tr>
 *<tr><td>26</td><td>SIGSYS</td><td>无效系统调用<br/>该信号指示一个无效的系统调用。由于某种未知的原因，进程执行了一条机器指令，内核认为这是一个系统调用，但该指令指示系统调用类型的参数却是无效的。这种情况是可能发生的，例如，用户编写了一道使用新	系统调用的程序，然后尝试运行该程序的二进制可执行代码，而所用的操作系统却是不支持该系统调用的较早版本，于是就会出现上述情况。</td><td>终止+core</td></tr>
 *<tr><td>27</td><td>SIGTERM</td><td>终止<br/>这是由kill（1）命令发送的系统默认终止信号。</td><td>终止</td></tr>
 *<tr><td>28</td><td>SIGTHAW</td><td>检查点解冻</td><td>忽略</td></tr>
 *<tr><td>29</td><td>SIGTRAP</td><td>硬件故障在</td><td>终止+core</td></tr>
 *<tr><td>30</td><td>SIGTSTP</td><td>终端停止符<br/>交互式停止信号，当用户在终端上按挂起键（一般采用Ctrl+Z）时，终端驱动程序产生次信号。该信号送至前台进程组中的所有进程</td><td>暂停进程</td></tr>
 *<tr><td>31</td><td>SIGTTIN</td><td>后台读控制tty<br/>当一个后台进程组中的进程试图读其控制终端时，终端驱动程序产生此信号。在下列特殊情形下不产生次信号：（a）读进程忽略或阻塞次信号，（b）读进程所属的进程u是孤儿进程组，此时操作返回出错，并将errno设置为EIO</td><td>暂停进程</td></tr>
 *<tr><td>32</td><td>SIGTTOU</td><td>后台写至控制tty<br/>当一个后台进程组中的进程试图写到其控制终端时产生次信号。与上面所述的SIGTTIN信号不同，一个进程可以选择允许后台进程写到控制终端。<br/> 如果不允许后台进程写，则与SIGTTIN相似，也有两种特殊情况：（a）写进程忽略或阻塞此信号，（b）写进程所属进程组是孤儿进程组。在这种情况下不产生次信号，写操作返回出错， 并将errno设置为EIO。</td><td>暂停进程</td></tr>
 *<tr><td>33</td><td>SIGURG</td><td>紧急情况（套接字）<br/>此信号通知进程已经发生一次紧急情况。在网络连接上接受到带外的数据时， 可选择产生次信号。</td><td>忽略</td></tr>
 *<tr><td>34</td><td>SIGUSR1</td><td>用户定义的信号</td><td>终止</td></tr>
 *<tr><td>35</td><td>SIGUSR2</td><td>用户定义的信号</td><td>终止</td></tr>
 *<tr><td>36</td><td>SIGVTALRM</td><td>虚拟时间闹钟（setitimer）<br/>当一个由setitimer(2)函数设置的虚拟间隔时间到期时产生此信号。</td><td>终止</td></tr>
 *<tr><td>37</td><td>SIGWAITING</td><td>线程库内部使用</td><td>忽略</td></tr>
 *<tr><td>38</td><td>SIGWINCH</td><td>终端窗口大小改变<br/>内核维持与每个终端或为终端相关联的窗口大小。进程可以用ioctl函数得到或设置窗口大小。如果进程用ioctl的设置窗口大小命令更改窗口大小，则内核产生SIGWINCH信号并将其送至前台进程组。</td><td>忽略</td></tr>
 *<tr><td>39</td><td>SIGXCPU</td><td>超过CPU限制（setrlimit）</td><td>终止+core</td></tr> 
 *<tr><td>40</td><td>SIGXFSZ</td><td>超过文件长度限制</td><td>终止+core</td></tr> 
 *<tr><td>41</td><td>SIGXRES</td><td>超过资源控制</td><td>忽略</td></tr> 
 *</table>
 *
 *<ul>
 *<li><b>程序启动：</b>当执行一个程序时，所有信号的状态都是系统默认或忽略。通常所有信号都被设置为他们的默认动作，除非调用exec的进程忽略该信号。确切地讲， exec函数将原先设置为要捕捉的信号都更改为它们的默认动作，其他信号的状态则不变（对于一个进程原先要捕捉的信号，当其执行一个新程序后，就自然不能再捕捉它了，因为信号捕捉函数的地址很可能在所执行的新程序中已无意义）</li>
 *<li><b>进程创建：</b>当一个进程调用fork时，其子进程继承父进程的信号处理方式。因为子进程在开始时复制了父进程的存储印象，所以信号捕捉函数的地址在子进程中是有意义的</li>
 *</ul>
 *
 *
 *在早期的UNIX中信号是不可靠的，<b>不可靠</b>是指信号可能丢失（在执行自定义函数期间会丢失同类信号），现在Linux在SIGRTMIN实时信号之前的都叫不可靠信号（可以通过<b>kill -l</b>命令显示所有信号），这里的不可靠主要是不支持信号队列，就是当多个信号发生在进程中的时候（收到信号的速度超过进程处理的速度的时候），这些没来的及处理的信号就会被丢掉，仅仅留下一个信号。<br/>
 *<b>可靠信号</b>是多个信号发送到进程的时候（收到信号的速度超过进程处理信号的速度的时候），这些没来得及处理的信号就会排入进程的队列，等待进程有机会处理的时候，依次再处理，信号不丢失<br/>
 *\retval 0 成功
 *\retval !0 失败
 */
int test_signal()
{
	signal(SIGINT, deal_signal); // 2 ctrl + c
	signal(SIGKILL, deal_signal);// 
	signal(SIGSTOP, deal_signal);//
	signal(SIGTSTP, deal_signal);// 20 ctrl + z
	signal(SIGQUIT, deal_signal);// 3 'ctrl + \'
	signal(SIGTERM, deal_signal);// 15 kill pid
	signal(SIGALRM, deal_signal);// 测试可重入性质
	signal(SIGRTMIN, deal_signal);// 可靠信号
	for (;;){
		
		struct passwd *pwd;
		if ((pwd = getpwnam("test")) == NULL){
			
			printf("getpwnam error\n");
		}
		if ((strcmp(pwd->pw_name, "test")) != 0){
			
			printf("return value corrupted!\n");
		}
	}
	
	/*i=0;while [ $i -lt 10 ];do pid=$(pidof c10_signal); kill -2 $pid; kill -34 $pid; ((++i)) done */
/*

access signo is [2]
SIGINT
access signo is [2]
SIGINT











access signo is [34]
SIGRTMIN
access signo is [34]
SIGRTMIN
access signo is [34]
SIGRTMIN
access signo is [34]
SIGRTMIN
access signo is [34]
SIGRTMIN
access signo is [34]
SIGRTMIN
access signo is [34]
SIGRTMIN
access signo is [34]
SIGRTMIN
access signo is [34]
SIGRTMIN
access signo is [34]
SIGRTMIN
*/	
	return 0;
}

/**
 *\brief 测试kill函数
 *
 *int kill(pid_t pid, int signo);<br/>
 *int raise(int signo);<br/>
 *返回值：成功返回0， 出错返回-1<br/>
 *raise只能将命令发送给进程自身，等价于kill(getpid(), signo)<br/>
 *<table>
 *<tr><th width="40">序号</th><th width="90">pid参数</th><th>描述</th></tr>
 *<tr><td>1</td><td>pid > 0</td><td>将该信号发送给进程ID为pid的进程</td></tr>
 *<tr><td>2</td><td>pid == 0</td><td>将该信号发送给与发送进程属于同一进程组的所有进程（这些进程的进程组ID等于发送进程的进程组ID），而且发送进程具有向这些进程发送信号的权限。</td></tr>
 *<tr><td>3</td><td>pid < 0</td><td>将该信号发送给其他进程组ID等于pid的绝对值，而且发送进程具有向其发送信号的权限。</td></tr>
 *<tr><td>4</td><td>pid == -1</td><td>将该信号发送给发送进程有权限向它们发送信号的系统上的所有进程。</td></tr>
 *</table>
 *\retval 0 成功
 *\retval !0 失败
 */
int test_kill()
{
	pid_t child_pid = -1;
	int status = -1, ret = -1;
	
	child_pid = fork();
	if (child_pid == -1){
		
		printf("fork failed!, %s\n", strerror(errno));
		return 1;
	}else if (0 == child_pid){
		
		printf("chilid process %d\n", getpid());
		sleep(100);
		return 0;
	}
	
	sleep(5);
	// 父进程
	if (0 == (ret = waitpid(child_pid, &status, WNOHANG))){ // 非阻塞
	
		printf("return pid %d\n", ret);
		ret = kill(child_pid, SIGKILL);
		if (ret){
			
			printf("kill failed\n");
			waitpid(child_pid, &status, 0);
		}else{
			
			printf("%d killed\n", child_pid);
		}
	}
	
	return 0;
}

static void deal_alarm(int signo)
{
	unsigned int ret = alarm(10);
	
	printf("%s:%d resave %d sencods\n", __FILE__, __LINE__, ret);
}

/**
 *\brief 测试alarm函数
 *
 *unsigned int alarm(unsigned int seconds);<br/>
 *返回值：0或以前设置的闹钟事件的余留秒数<br/>
 *<br/>
 *使用alarm函数可以设置一个计时器，在将来某个指定的时间该计时器会超时。当计时器超时时，产生SIGALRM信号。如果不忽略或不捕捉此信号，则其默认动作时终止调用该alarm函数的进程。
 *
 *\warning 经过指定的seconds秒后会产生信号SIGALRM，由于信号来自内核产生，存在进程调度的延迟，所以最终到信号的处理还需一些时间
 *\warning 每个进程只能有一个闹钟时钟
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_alarm()
{
	signal(SIGALRM, deal_alarm);
	raise(SIGALRM); // 向自己发送信号
	
	sleep(3);
	
	unsigned int ret = alarm(10);
	printf("%s:%d resave %u seconds\n", __FILE__, __LINE__, ret);
	
	return 0;
}

/**
 *\brief 测试sigset信号集
 *
 *<ul>
 *<li>信号集是一种数据类型（sigset_t），能表示多个信号</li>
 *<li>根据前面介绍，系统中的信号种类数量可能超过一个整型量所包含的位数，所以一般不能用整型中的一位代表一种信号</li>
 *</ul>
 *<code>
 *int sigemptyset(sigset_t *set);
 *
 *int sigfillset(sigset_t *set);
 *
 *int sigaddset(sigset_t *set, int signo);
 *
 *int sigdelset(sigset_t *set, int signo);
 *
 *返回值：若成功则返回0，若出错则返回-1
 *
 *int sigismember(const sigset_t *set, int signo);
 *
 *返回值：若真则返回1，若假则返回0，若出错则返回-1<br/>
 *</code>
 *<ol>
 *<li>sigemptyset初始化由set指向的信号集，清除其中所有信号</li>
 *<li>sigfillset初始化由set指向的信号集，使其包括所有信号</li>
 *<li>sigaddset将一个信号添加到现有集中</li>
 *<li>sigdelset则从信号集中删除一个信号</li>
 *</ol>
 *
 *\retval 0 成功
 *\retval !0 失败 
 */
int test_sigset()
{
	//信号集的增删
	sigset_t sigset;
	
	sigfillset(&sigset);// 填充所有信号
	
	if (sigismember(&sigset, SIGALRM)){
		
		printf("%s:%d SIGARLM exsit!\n", __FILE__, __LINE__);
	}else{
		
		printf("%s:%d SIGARLM not exsit!\n", __FILE__, __LINE__);
	}
	
	sigdelset(&sigset, SIGALRM);
	
	if (sigismember(&sigset, SIGALRM)){
		
		printf("%s:%d SIGARLM exsit!\n", __FILE__, __LINE__);
	}else{
		
		printf("%s:%d SIGARLM not exsit!\n", __FILE__, __LINE__);
	}
		
	return 0;
}

static void sig_procmask(int signo)
{
	if (signo == SIGINT){
		
		printf("%s:%d catch SIGINT \n", __FILE__, __LINE__);
	}else if (signo == SIGRTMIN){
		
		printf("%s:%d catch SIGRTMIN \n", __FILE__, __LINE__);
	}
}

/**
 *\brief 测试sigprocmask函数
 *
 *在linux中，进程可以接收到各种信号，如果不对信号进行处理，系统会采用默认的处理方法。 但是，我们也可以在进程中屏蔽掉某些信号，使得进程不去处理这些信号（SIGKILL和SIGSTOP例外），这就是信号屏蔽字。一个进程的信号屏蔽字规定了当前阻塞而不能递送给该进程的信号集。<br/>
 *调用函数sigprocmask可以检测或更改其信号屏蔽字，或者在一个步骤中同时执行这两个操作。<br/>
 * 
 *int sigprocmask(int how, const sigset_t *restrict set, sigset_t *restrict oset);<br/>
 *返回值：若成功则返回0， 若出错则返回-1<br/>
 *
 *<ul>
 *<li>若oset != NULL，那么进程的当前信号屏蔽字通过oset返回</li>
 *<li>若 set != NULL，则参数how指示如何修改当前信号屏蔽字
 *<table>
 *<tr><th width="60">序号</th><th width="60">how取值</th><th>说明</th></tr>
 *<tr><td>1</td><td>SIG_BLOCK</td><td>该进程新的信号屏蔽字是其当前信号屏蔽字和set指向信号集的并集。set包含了我们希望阻塞的附加信号</td></tr>
 *<tr><td>2</td><td>SIG_UNBLOCK</td><td>该进程新的信号屏蔽字是其当前信号屏蔽字和set指向信号集的补集的交集。set包含了我们希望解除阻塞的信号</td></tr>
 *<tr><td>3</td><td>SIG_SETMASK</td><td>该进程新的信号屏蔽字将被set指向的信号集的值代替</td></tr>
 *</table>
 
 </li>
 *<li>若 set == NULL，则不能改变进程的信号屏蔽字，how的值也无意义</li>
 *</ul>
 *\retval 0 成功
 *\retval !0 失败  
 */
int test_sigprocmask()
{
	// 这里分别对一个可靠信号 和 不可靠信号进行测试
	sigset_t new_mask, old_mask;
	
	signal(SIGINT, sig_procmask);// 不可靠信号 
	signal(SIGRTMIN, sig_procmask);// 可靠信号
	
	// 添加两个信号到信号屏蔽字中
	sigemptyset(&new_mask);
	sigaddset(&new_mask, SIGINT);
	sigaddset(&new_mask, SIGRTMIN);
	
	if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask)){
		
		printf("%s:%d sigprocmask failed, %s\n", __FILE__, __LINE__, strerror(errno));
		return 1;
	}
	printf("%s:%d Enter pengding\n", __FILE__, __LINE__);
	sleep(5);  // 在睡眠的过程中执行下面的命令
	/*
i=0;pid=$(pidof c10_signal);while [ $i -lt 10 ]; do ((++i)); kill -2 $pid; kill -34 $pid; done;	
	*/
	
	// 恢复老的屏蔽字， 这里需要注意， 老的屏蔽字中不能阻塞上面两个信号， 不然看不到效果了
	if (sigprocmask(SIG_SETMASK, &old_mask, NULL)){

		printf("%s:%d sigprocmask failed, %s\n", __FILE__, __LINE__, strerror(errno));
		return 1;	
	}
	printf("%s:%d Exit pending\n", __FILE__, __LINE__);
	sleep(5);

/*
# ./c10_signal sigprocmask
c10_signal.cc:415 Enter pengding
c10_signal.cc:427 Exit pending
c10_signal.cc:368 catch SIGRTMIN 
c10_signal.cc:368 catch SIGRTMIN 
c10_signal.cc:368 catch SIGRTMIN 
c10_signal.cc:368 catch SIGRTMIN 
c10_signal.cc:368 catch SIGRTMIN 
c10_signal.cc:368 catch SIGRTMIN 
c10_signal.cc:368 catch SIGRTMIN 
c10_signal.cc:368 catch SIGRTMIN 
c10_signal.cc:368 catch SIGRTMIN 
c10_signal.cc:368 catch SIGRTMIN 
c10_signal.cc:365 catch SIGINT 
*/	
	return 0;
}

/**
 *\brief 测试sigpending函数
 *
 *int sigpending(sigset_t *set);<br/>
 *返回值：若成功则返回0， 若出错则返回-1<br/>
 *sigpending函数返回信号集，其中的各个信号对于调用进程是阻塞的而不能递送，因而也一定是当前未决的，信号集通过set参数返回。<br/>
 *\retval 0 成功
 *\retval !0 失败  
 */
int test_sigpengding()
{
	return 0;
}

/**
 *\brief 测试sigaction函数
 *
 *int sigaction(int signo, const struct sigaction *restrict act, struct sigaction *restrict oact);<br/>
 *返回值：若成功则返回0， 若出错则返回-1<br/>
 *<br/>
 *
 *struct sigaction{
 *
 *	 void	(*sa_handler)(int);	// 信号处理函数	 
 *	 sigset_t sa_mask;	// 阻塞信号集
 *	 int	sa_flags;	// 
 *	 void	(*sa_sigaction)(int, siginfo_t *, void *);	//
 *}
 *
 *<ul>
 *<li>sigacton函数的功能是检查或修改与指定信号相关联的处理动作</li>
 *<li></li>
 *</ul>
 *
 *<table>
 *<caption> 中断的系统调用 </caption>
 *<tr><th width=60>序号</th><th width=400>描述</th><th width=400>说明</th><th>实例</th></tr>
 *<tr><td colspan=4>早期UNIX系统的一个特性是：如果进程在执行一个低速系统调用而阻塞期间捕捉到一个信号，则该系统调用就被中断不再继续执行。该系统调用返回出错，其errno被设置为EINTR。这样处理的理由是：因为一个信号发生了，进程捕捉到了它，这意味着已经发生了某种事情，所以是个应当唤醒阻塞的系统调用的好机会。
 *
 在这里，我们必须区分系统调用和函数。当捕捉到某个信号时，被中断的是内核中的执行的系统调用。</td></tr>
 *<tr><td></td><td>为了支持这种特性，将系统调用分成两类：低速系统调用和其他系统调用。低速系统调用是可能会使进程永远阻塞的一类系统调用，它们包括：
 *<ol>
 *<li>在读某些类型的文件（管道，终端设备以及网络设备）时，如果数据并不存在则可能会使调用者永远阻塞</li>
 *<li>在写这些类型的文件时，如果不能立即接受这些数据， 则也可能会使调用者永远阻塞。</li>
 *<li>打开某些类型文件，在某种条件发生之前也可能会使调用者阻塞（例如，打开终端设备）</li>
 *<li>pause（按照定义，它使调用进程休眠直至捕捉到一个信号）和wait函数。</li>
 *<li>某些ioctl操作</li>
 *<li>某些进程间通信函数</li>
 *</ol></td><td>在这些低速系统调用中，一个值得注意的例外是与磁盘I/O有关的系统调用。虽然读、写一个磁盘文件可能暂时阻塞调用者（在磁盘驱动器请求排入队列，然后再适当时间执行请求期间），但是除非发生硬件错误，I/O操作总会很快返回，并使调用者不再处于阻塞状态。</td><td></td></tr>
 *<tr><td colspan=4>对于中断了的系统调用，有三种处理方式：</td></tr>
 *<tr><td>1</td><td>人为重启被中断的系统调用</td><td>人为当碰到EINTR错误的时候，有一些可以重启的系统调用要进行重启，而对于有一些系统调用时不能够重启的。例如：accpept、read、write、select和open之类的函数，是可以进行重启的。不过对于套接字编程中的connect函数我们是不能重启的，若connect函数返回一个EINTR错误的时候，我们不能再次调用它，否则将立即返回一个错误。针对connect不能重启的处理方法是，必须调用select来等待连接完成。
 *
 *像一些IO系统调用执行时，如read等待输入期间，如果收到一个信号，系统将中断read，转而执行信号处理函数，当信号处理函数返回后，系统调用会遇到一个必须要考虑的问题：是重新开始这个系统调用，还是让系统调用失败？ 早期UNIX系统做法是，中断系统调用，并让系统调用失败，比如read返回-1，同时设置errno为EINTR，中断了的系统调用是没有完成的调用，它的失败是临时性的，如果再次调用则可能成功，这并不是真正的失败，所以要对这种情况进行处理。</td><td>
 *<code>
 *
 *again:
 *
 *	if ((n = read(fd, buf, BUF_SIZE)) < 0){
 *
 *		if (errno == EINTR){ goto again;}
 *
 *		.....
 *
 *	}
 *</code></td></tr>
 *<tr><td>2</td><td>安装信号时设置SA_RESTART属性（该方法对有的系统调用无效）</td><td>我们还可以从信号的角度来解决这个问题，安装信号的时候， 设置SA_RESTART属性， 那么当信号处理函数返回后， 不会让系统调用返回失败，而是将让被该信号中断的系统调用自动恢复。
 *
 *但注意， 并不是所有的系统调用都可以自动恢复。 如msgsnd和msgrcv就是典型的例子，msgsnd/msgrcv以block方式发送/接受消息时，会因为进程收到了信号而中断。此时msgsnd/msgrcv将返回-1， errno被设置为EINTR。且即使在插入信号时设置了SA_RESTART，也无效。 所以在使用的时候为了保险起见还是需要查看下系统的man手册 和 测试覆盖。</td><td>
 *struct sigaction action;
 *
 *action.sa_handler = handler_func;<br/>
 *sigemptyset(&action.sa_mask);<br/>
 *action.sa_flags = 0;<br/>
 *action.sa_flags |= SA_RESTART;<br/>
 *
 *sigaction(SIGALRM, &action, NULL);
 *</td></tr>
 *<tr><td>3</td><td>忽略信号（让系统不产生信号中断）</td><td>在安装信号时，明确告诉系统不会产生该信号的中断</td><td>
 *struct sigaction action;
 *
 *action.sa_handler = SIG_IGN;<br/>
 *sigemptyset(&action.sa_mask);<br/>
 *
 *sigaction(SIGALRM, &action, NULL);
 *</td></tr>
 *</table>
 *
 * 
 *<table>
 *<caption> 可重入函数 </caption>
 *<tr><th></th><th></th><th></th><th></th><th></th><th></th></tr>
 *<tr><td colspan=6>可重入函数是可以被中断的函数，可以在函数执行的任何时刻中断它，而一段时间之后又可以恢复运行，而相应的数据不会破坏或者丢失。
 *
 *可重入函数使用的变量有两种情况：
 *<ol>
 *<li>使用局部变量，变量保存在CPU寄存器中或者堆栈中</li>
 *<li>使用全局变量，但是这时候要注意保护全局变量（防止任务中断后被其它任务改变变量）</li>
 *</ol>
 *基本上下面的函数都是不可重入的：
 *<ol>
 *<li>函数内使用了静态的数据</li>
 *<li>函数内使用了malloc（）或者free（）函数的</li>
 *<li>函数内调用了标准的I/O函数的</li>
 *</ol>
 *常用的可重入函数的方法有：
 *<ol>
 *<li>不要使用全局变量，防止别的代码覆盖这些变量的值</li>
 *<li>调用这类函数之前先关掉中断，调用完之后马上打开中断。防止函数执行期间被中断进入别的任务执行</li>
 *<li>使用信号量（互斥条件）</li>
 *</ol>
 *总之：要保证中断是安全的
 *</td></tr> 
 *<tr><td>1</td><td>accept</td><td>fchmod</td><td>lseek</td><td>sendto</td><td>stat</td></tr>
 *<tr><td>2</td><td>access</td><td>fchown</td><td>lstat</td><td>setgid</td><td>symlink</td></tr>
 *<tr><td>3</td><td>aio_error</td><td>fcntl</td><td>mkdir</td><td>setpgid</td><td>sysconf</td></tr> 
 *<tr><td>4</td><td>aio_return</td><td>fdatasync</td><td>mkfifo</td><td>setsid</td><td>tcdrain</td></tr>
 *<tr><td>5</td><td>aio_suspend</td><td>fork</td><td>open</td><td>setsockopt</td><td>tcflow</td></tr>  
 *<tr><td>6</td><td>alarm</td><td>fpathconf</td><td>pathconf</td><td>setuid</td><td>tcflush</td></tr>
 *<tr><td>7</td><td>bind</td><td>fstat</td><td>pause</td><td>shutdown</td><td>tcgetattr</td></tr>
 *<tr><td>8</td><td>cfgetispeed</td><td>fsync</td><td>pipe</td><td>sigaction</td><td>tcgetpgrp</td></tr>
 *<tr><td>9</td><td>cfgetospeed</td><td>ftruncate</td><td>poll</td><td>sigaddset</td><td>tcsendbreak</td></tr>
 *<tr><td>10</td><td>cfsetispeed</td><td>getegid</td><td>posix_trace_event</td><td>sigdelset</td><td>tcsetattr</td></tr>
 *<tr><td>11</td><td>cfsetospeed</td><td>geteuid</td><td>pselect</td><td>sigemptyset</td><td>tcsetpgrp</td></tr> 
 *<tr><td>12</td><td>chdir</td><td>getgid</td><td>raise</td><td>sigfillset</td><td>time</td></tr>
 *<tr><td>13</td><td>chmod</td><td>getgroups</td><td>read</td><td>sigismember</td><td>timer_getoverrun</td></tr>  
 *<tr><td>14</td><td>chown</td><td>getpeername</td><td>readlink</td><td>signal</td><td>timer_gettime</td></tr>
 *<tr><td>15</td><td>clock_gettime</td><td>getpgrp</td><td>recv</td><td>sigpause</td><td>timer_settime</td></tr>  
 *<tr><td>16</td><td>close</td><td>getpid</td><td>recvfrom</td><td>sigpending</td><td>times</td></tr>
 *<tr><td>17</td><td>connect</td><td>getppid</td><td>recvmsg</td><td>sigprocmask</td><td>umask</td></tr>   
 *<tr><td>18</td><td>creat</td><td>getsockname</td><td>rename</td><td>sigqueue</td><td>uname</td></tr> 
 *<tr><td>19</td><td>dup</td><td>getsockopt</td><td>rmdir</td><td>sigset</td><td>unlink</td></tr> 
 *<tr><td>20</td><td>dup2</td><td>getuid</td><td>select</td><td>sigsuspend</td><td>utime</td></tr> 
 *<tr><td>21</td><td>execle</td><td>kill</td><td>sem_post</td><td>sleep</td><td>wait</td></tr> 
 *<tr><td>22</td><td>execve</td><td>link</td><td>send</td><td>socket</td><td>waitpid</td></tr> 
 *<tr><td>23</td><td>_Exit&_exit</td><td>listen</td><td>sendmsg</td><td>socketpair</td><td>write</td></tr>  
 *</table>
 *
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_sigaction()
{
	
	return 0;
}

static void show_help()
{
	printf("signal, 测试signal函数\n\n"
			"kill, 测试kill函数\n\n"
			"alarm, 测试alarm函数\n\n"
			"sigset, 测试sigset函数\n\n"
			"sigprocmask, 测试sigprocmask函数\n\n"
			"times system_cmd, 测试times函数\n\n");
}

int main(int argc, char **argv)
{
	if (argc == 2 and strcmp("signal", argv[1]) == 0){
		
		test_signal();
		while (1){}
	}else if (argc == 2 and strcmp("kill", argv[1]) == 0){
		
		test_kill();
	}else if (argc == 2 and strcmp("alarm", argv[1]) == 0){
		
		test_alarm();
	}else if (argc == 2 and strcmp("sigset", argv[1]) == 0){
		
		test_sigset();
	}else if (argc == 2 and strcmp("sigprocmask", argv[1]) == 0){
		
		test_sigprocmask();
	}else{
		
		show_help();
	}
	
	return 0;
}