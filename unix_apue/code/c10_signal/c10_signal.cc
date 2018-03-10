/**
 *\file c10_signal.cc
 *\brief 练习信号
 *\date 2017/10/07
 */

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

void deal_signal(int signo)
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
 *在早期的UNIX中信号是不可靠的，不可靠是指信号可能丢失（在执行自定义函数期间会丢失同类信号），现在Linux在SIGRTMIN实时信号之前的都叫不可靠信号（可以通过kill -l命令显示所有信号），这里的不可靠主要是不支持信号队列，就是当多个信号发生在进程中的时候（收到信号的速度超过进程处理的速度的时候），这些没来的及处理的信号就会被丢掉，仅仅留下一个信号。<br/>
 *可靠信号是多个信号发送到进程的时候（收到信号的速度超过进程处理信号的速度的时候），这些没来得及处理的信号就会排入进程的队列，等待进程有机会处理的时候，依次再处理，信号不丢失<br/>
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
	signal(SIGRTMIN, deal_signal);// 可靠信号
	
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
	return 0;
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
	return 0;
}

/**
 *\brief 测试sigset信号集
 *
 *<ul>
 *<li>信号集是一种数据类型（sigset_t），能表示多个信号</li>
 *<li>根据前面介绍，系统中的信号种类数量可能超过一个整型量所包含的位数，所以一般不能用整型中的一位代表一种信号</li>
 *</ul>
 * 
 *int sigemptyset(sigset_t *set);<br/>
 *int sigfillset(sigset_t *set);<br/>
 *int sigaddset(sigset_t *set, int signo);<br/>
 *int sigdelset(sigset_t *set, int signo);<br/>
 *返回值：若成功则返回0，若出错则返回-1<br/><br/>
 *int sigismember(const sigset_t *set, int signo);<br/>
 *返回值：若真则返回1，若假则返回0，若出错则返回-1<br/>
 *
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
	return 0;
}
 
/**
 *\brief 测试sigprocmask函数
 *
 *一个进程的信号屏蔽字规定了当前阻塞而不能递送给该进程的信号集。<br/>
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
 *struct sigaction{<br/>
	 void	(*sa_handler)(int);	// 信号处理函数
	 sigset_t sa_mask;	// 阻塞信号集
	 int	sa_flags;	// 
	 void	(*sa_sigaction)(int, siginfo_t *, void *);	//
 }
 *<ul>
 *<li>sigacton函数的功能是检查或修改与指定信号相关联的处理动作</li>
 *<li></li>
 *</ul>
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
			"fork, 测试fork函数\n\n"
			"wait, 测试wait函数\n\n"
			"exec, 测试exec函数\n\n"
			"times system_cmd, 测试times函数\n\n");
}

int main(int argc, char **argv)
{
	if (argc == 2 and strcmp("signal", argv[1]) == 0){
		
		test_signal();
		while (1){}
	}else{
		
		show_help();
	}
	
	return 0;
}