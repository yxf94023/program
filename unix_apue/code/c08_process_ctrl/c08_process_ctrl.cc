/**
 *\file c08_process_ctrl.cc
 *\brief 练习进程控制
 *\date 2017/10/02
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/times.h>

/**
 *\brief 测试进程id函数
 *
 *1. 每个进程都有一个非负整型表示的唯一进程ID<br/>
 *2. 进程ID是可以重用，当一个进程终止后，其进程ID就可以再次使用了，大多数UNIX系统实现延迟重用算法，使得赋予新进程的ID不同于最近终止进程所使用的ID。<br/>
 *3. ID为0的进程通常是调度进程，常常被称为交换进程（swapper），该进程是内核的一部分，它并不执行任何磁盘上的程序，因此也被称为系统进程。<br/>
 *4. ID为1的通常是init进程，在自举过程结束时由内核调用，init进程不会终止，它是一个普通的用户进程，但是它以超级用户特权运行<br/>
 *5. 每个UNIX系统实现都有它自己的一套提供操作系统服务的内核进程，例如，在某些UNIX的虚拟存储器实现中，进程ID2是页守护进程（pagedaemon），它负责支持虚拟存储系统的分页操作<br/>
 *
 *pid_t getpid(void)<br/>
 *	返回值：调用进程的进程ID<br/>
 *pid_t getppid(void)<br/>
 *	返回值：调用进程的父进程ID<br/>
 *pid_t getuid(void);<br/>
 *	返回值：调用进程的实际用户ID<br/>
 *pid_t geteuid(void);<br/>
 *	返回值：调用进程的有效用户ID<br/>
 *pid_t getgid(void);<br/>
 *	返回值：调用进程的实际组ID<br/>
 *pid_t getegid(void);<br/>
 *	返回值：调用进程的有效组ID<br/>
 *
 *\warning 这些函数都没有出错返回
 *
 *\retval 0	成功
 *\retval !0 失败
 */
int test_id()
{
	return 0;
}

/**
 *\brief 测试fork函数
 *
 *pid_t fork(void);<br/>
 *返回值：子进程中返回0， 父进程中返回子进程ID， 出错返回-1<br/>
 *<br/>
 *fork函数被调用一次，但返回两次，两次返回的唯一区别是子进程的返回值是0，而父进程的返回值则是新子进程的进程ID。将子进程ID返回给父进程的理由是，一个进程的子进程可以有多个，并且没有一个函数使一个进程可以获得其所有子进程的进程ID。子进程得到返回值0的理由是，一个进程只会有一个父进程，所以子进程总是可以调用getppid以获得其父进程的进程ID<br/>
 *子进程和父进程继续执行fork调用之后的指令，子进程是父进程的副本（子进程获得父进程数据空间、堆和栈的副本），这是子进程所拥有的副本，父、子进程并不共享这些存储空间部分，父、子进程共享正文段<br/>
 *由于在fork之后经常跟随者exec，所以现在的很多实现并不执行一个父进程数据段、栈和堆的完全复制，作为替代，使用了写时复制（Copy-On-Write, COW)技术，这些区域由父、子进程共享，而且内核将它们的访问权限改变为只读的，如果父、子进程中的任一个试图修改这些区域，则内核只为修改区域的那块内存制作一个副本，通常是虚拟存储器系统中的一“页”<br/>
 *文件共享，在重定向父进程的标准输出时，子进程的标准输出也被重定向，实际上，fork的一个特性是父进程的所有打开文件描述符都被复制到子进程中，父、子进程的每个相同的打开描述符共享一个文件表项。这种共享文件的方式使父、子进程对同一文件使用了一个文件偏移量。在fork之后处理文件描述符有两种常见的情况：<br/>
 *(1)、父进程等待子进程完成，在这种情况下，父进程无需对其描述符做任何处理，当子进程终止后，它曾进行过读、写操作的任一共享描述符的文件偏移量已执行了相应更新<br/>
 *(2)、父、子进程各自执行不同的程序段，在这种情况下，在fork之后，父、子进程各自关闭它们不需使用的文件描述符，这样就不会干扰对方使用的文件描述符，这种方法是网络服务进程中经常使用的<br/>
 *(3)、除了打开文件之外，父进程的很多其他属性也由子进程继承，包括
 *a. 实际用户ID、实际组ID、 有效用户ID、 有效组ID<br/>
 *b. 附件组ID<br/>
 *c. 进程组ID<br/>
 *d. 会话ID<br/>
 *e. 控制终端<br/>
 *f. 设置用户ID标志和设置组ID标志<br/>
 *g. 当前工作目录<br/>
 *h. 根目录<br/>
 *i. 文件模式创建屏蔽字<br/>
 *j. 信号屏蔽和安排<br/>
 *k. 针对任一打开文件描述符的在执行时关闭(close-on-exec)标志<br/>
 *l. 环境<br/>
 *m. 连接的共享存储段<br/>
 *n. 存储映射<br/>
 *o. 资源限制<br/><br/>
 *父子之间的区别<br/>
 *a. fork的返回值<br/>
 *b. 进程ID不同<br/>
 *c. 两个进程具有不同的父进程ID<br/>
 *d. 子进程的tms_utime、tms_stime、tms_cutime以及tms_ustime均被设置为0<br/>
 *e. 父进程设置的文件锁不会被子进程继承<br/>
 *f. 子进程的未处理的闹钟(alarm)被清除<br/>
 *g. 子进程的未处理信号集设置为空集<br/>
 *
 *\retval 0	成功
 *\retval !0 失败 
 */
int test_fork()
{
	return 0;
}

/**
 *\brief 测试exit函数
 *
 *"退出状态"（它是转向exit或_exit的参数，或main的返回值）和"终止状态"两个术语的区别，在最后调用_exit时，内核将退出状态转换成终止状态，如果子进程正常终止，则父进程可以获得子进程的退出状态<br/>
 *对于父进程已经终止的所有进程，它们的父进程都改变为init进程，我们称这些进程由init进程领养，操作过程大致如下，在一个进程终止时，内核逐个检查所有活动进程，以判断它是否是正要终止进程的子进程，如果是，则将该进程的父进程ID更改为1，这种处理方法保证了每个进程都有一个父进程<br/>
 *子进程在父进程之前终止，那么父进程又如何能在做相应检查时得到子进程的终止状态？内核为每个终止进程保存了一定量的信息， 所以当终止进程的父进程调用wait或waitpid时，可以得到这些信息，这些信息至少包括进程ID、该进程的终止状态、以及该进程使用的CPU时间总量，内核可以释放终止进程所使用的所有存储区，关闭其所有打开文件。一个已经终止、但是其父进程尚未对其进行善后处理的进程被称为僵死进程（zombie）。如果编写一个长期运行的程序，它调用fork产生了很多子进程，那么除非父进程等待取得子进程的终止状态，否则这些子进程终止后就会变成僵死进程<br/>
 *
 *\retval 0	成功
 *\retval !0 失败 
 */
int test_exit()
{
	return 0;
}

/**
 *\brief 测试wait函数
 *
 *pid_t wait(int *statloc);<br/>
 *pid_t waitpid(pid_t pid, int *statloc, int options);<br/>
 *返回值：若成功则返回进程ID，出错则返回-1<br/>
 *这两个函数的参数statloc是一个整型指针，如果statloc不是一个空指针，则终止进程的终止状态存放在它所指向的单元内，如果不关心终止状态， 则可将该参数指针定为空指针<br/>
 *<br/>
 *当一个进程正常或异常终止时，内核就向其父进程发送SIGCHLD信号，因为进程终止是个异步事件，所以这种信号也是内核向父进程发的异步通知。父进程可以选择忽略该信号， 或者提供一个信号处理函数， 对于这种信号系统的默认动作是忽略它。调用wait或waitpid的进程可能会发生的情况：<br/>
 *1. 如果其所有子进程都还在运行，则阻塞<br/>
 *2. 如果一个子进程已终止，正等待父进程获取其终止状态，则取得该子进程的终止状态立即返回<br/>
 *3. 如果它没有任何子进程，则立即出错返回<br/><br/>
 *
 *这两个函数的区别如下：<br/>
 *1. 在一个子进程终止前， wait使其调用者阻塞，而waitpid有一个选项，可使调用者不阻塞<br/>
 *2. waitpid并不等待在其调用之后的第一个终止子进程，它有若干个选项，可以控它所等待的进程<br/>
 *<table>
 *<caption>检查wait和waitpid所返回的终止状态宏</caption>
 *<tr><th width="40">序号</th><th width="100">宏</th><th>描述</th></tr>
 *<tr><td>1</td><td>WIFEXITED(status)</td><td>若为正常终止子进程返回的状态，则为真，对于这种情况可执行WEXITSTATUS(status)，取子进程传送给exit, _exit或_Exit参数的低8位</td></tr>
 *<tr><td>2</td><td>WIFSIGNALED(status)</td><td>若为异常终止子进程返回的状态，则为真（接到一个不捕捉的信号），对于这种情况，可以执行WTERMSIG(status)，取得子进程终止的信号编号，另外，有些实现定义宏WCOREDUMP(status)，若已产生终止进程的core文件， 则它返回真</td></tr>
 *<tr><td>3</td><td>WIFSTOPPED(status)</td><td>若为当前暂停子进程的返回的状态，则为真， 对于这种情况， 可执行WSTOPSIG(status)， 取得子进程暂停的型号编号</td></tr>
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
 *<caption>waitpid的options常亮</caption>
 *<tr><th width="40">序号</th><th width="100">常量</th><th>描述</th></tr> 
 *<tr><td>1</td><td>WCONTINUED</td><td>若实现支持作业控制，那么由pid指定的任一子进程在暂停后已经继续，但其状态尚未报告，则返回其状态</td></tr>
 *<tr><td>2</td><td>WNOHANG</td><td>若由pid指定的子进程并不是立即可用的，则waitpid不阻塞，此时其返回值为0</td></tr>
 *<tr><td>3</td><td>WUNTRACED</td><td>若某实现支持作业控制，而由pid指定的任一子进程已处于暂停状态，并且其状态自暂停以来还未报告过，则返回其状态，WIFSTOPPED宏确定返回值是否对应于一个暂停子进程</td></tr>
 *</table>
 *
 *waitpid函数返回终止子进程的进程ID，并将该子进程的终止状态存放在由statloc指向的存储单元中，对于wait，其唯一的出错是调用进程没有子进程（函数调用被一个信号中断时，也可能返回另一种出错），但是对于waitpid，如果指定的进程或进程组不存在，或者参数pid指定的进程不是调用进程的子进程则都将出错， waitpid函数提供了wait函数没有的三个功能：
 *1. waitpid可等待一个特定的进程，而wait则返回任一终止子进程的状态<br/>
 *2. waitpid提供了一个wait的非阻塞版本，有时用户希望取得一个子进程的状态，但不想阻塞<br/>
 *3. waitpid支持作业控制（利用WUNTRACED和WCONTINUED选项） 
 *\retval 0	成功
 *\retval !0 失败 
 */
int test_wait()
{
	return 0;
}

/**
 *\brief 测试waitid函数
 *
 *int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);<br/>
 *返回值：若成功则返回0， 若出错则返回-1<br/>
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
 *int execl(const char *pathname, const char *arg0, ..., (char*)0);<br/>
 *int execv(const char *pathname, char *const argv[]);<br/>
 *int execle(const char *pathname, const char *arg0, ..., (char*)0, char *const envp[]);<br/>
 *int execve(const char *pathname, char *const argv[], char *const envp[]);<br/>
 *int execlp(const char *filename, const char *arg0, ..., (char*)0);<br/>
 *int execvp(const char *filename, char *const argv[]);<br/>
 *返回值：若出错返回-1， 若成功则不返回值<br/><br/>
 *当进程调用一种exec函数时，该进程执行的程序完全替换为新程序，而新程序则从其main函数开始执行，因为调用exec并不创建新进程，所以前后的进程ID并未改变，exec指示用一个全新的程序替换了当前进程的正文、数据、堆和栈段<br/>
 *1. 在这一系列的exec函数中， 前4个取路径名作为参数，后两个则取文件名作为参数<br/>
 *	. 如果filename中包含/，则将其视为路径名<br/>
 *	. 否则就按PATH环境变量，在它所指定的各个目录中搜寻可执行文件<br/>
 *2. 在这一系列的exec函数中， 传递参数的方式是不同的，函数名中含有l的要求每个命令行参数都说明为一个单独的参数。函数名中含有v的在传递参数时，需要先构造一个指向各参数的指针数组，然后将该数组地址作为这位函数的参数<br/>
 *3. 在这一系列的exec函数中，向新程序传递环境表存在差异，以e结尾的两个函数可以传递一个指向环境字符串指针数组的指针， 其他四个函数则使用调用进程中的environ变量为新程序复制现有的环境<br/>
 *
 *\retval 0	成功
 *\retval !0 失败   
 */
int test_exec()
{
	return 0;
}

/**
 *\brief 测试times函数
 *
 *clock_t times(struct tms *buf);<br/>
 *返回值：若成功则返回流逝的墙上时钟时间（单位，时钟滴答数），若出错则返回-1<br/>
 *<br/>
 *struct tms{<br/>
	 clock_t	tms_utime;	// user CPU time<br/>
	 clock_t	tms_stime;	// system CPU time<br/>
	 clock_t	tms_cutime;	// user CPU time, terminated children<br/>
	 clock_t	tms_cstime;	// system CPU time, terminated children<br/>
 };<br/>
 *1. 任一进程都可调用times函数已获得它自己及已终止子进程的 墙上时钟时间、用户CPU时间和系统CPU时间<br/>
 *2. struct tms结构体中没有包含墙上时钟时间的任何测量值，作为替代， times函数返回墙上时钟时间作为其函数值。此值是相对于过去的某一时刻测量的， 所以不能用其绝对值， 而必须使用其相对值<br/>
 *
 *\retval 0	成功
 *\retval !0 失败   
 */
int test_times()
{
	return 0;
}