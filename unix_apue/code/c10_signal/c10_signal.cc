/**
 *\file c10_signal.cc
 *\brief 练习信号
 *\date 2017/10/07
 */

#include <unistd.h>

/**
 *\brief 测试signal函数
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
 *<tr><th width="40">序号</th><th width="60">名字</th><th>描述</th><th>默认动作</th></tr>
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
 *<tr><td>20</td><td>SIGPROF</td><td>梗概时间超时（setitimer）</td><td>终止</td></tr>
 *<tr><td>21</td><td>SIGPWR</td><td>电源失效/重启动</td><td>终止/忽略</td></tr>
 *<tr><td>22</td><td>SIGQUIT</td><td>终端退出符<br/>当用户在终端上按退出键（一般采用Ctrl+\）时，产生此信号，并发送至前台进程组中的所有进程， 此信号不仅会终止前台进程组， 同时还会产生一个core文件</td><td>终止+core</td></tr>
 *<tr><td>23</td><td>SIGSEGV</td><td>无效内存引用</td><td>终止+core</td></tr>
 *<tr><td>24</td><td>SIGSKFLT</td><td>协处理器栈故障</td><td>终止</td></tr>
 *<tr><td>25</td><td>SIGSTOP</td><td>停止</td><td>暂停进程</td></tr>
 *<tr><td>26</td><td>SIGSYS</td><td>无效系统调用</td><td>终止+core</td></tr>
 *<tr><td>27</td><td>SIGTERM</td><td>终止</td><td>终止</td></tr>
 *<tr><td>28</td><td>SIGTHAW</td><td>检查点解冻</td><td>忽略</td></tr>
 *<tr><td>29</td><td>SIGTRAP</td><td>硬件故障</td><td>终止+core</td></tr>
 *<tr><td>30</td><td>SIGTSTP</td><td>终端停止符</td><td>暂停进程</td></tr>
 *<tr><td>31</td><td>SIGTTIN</td><td>后台读控制tty</td><td>暂停进程</td></tr>
 *<tr><td>32</td><td>SIGTTOU</td><td>后台写至控制tty</td><td>暂停进程</td></tr>
 *<tr><td>33</td><td>SIGURG</td><td>紧急情况（套接字）</td><td>忽略</td></tr>
 *<tr><td>34</td><td>SIGUSR1</td><td>用户定义的信号</td><td>终止</td></tr>
 *<tr><td>35</td><td>SIGUSR2</td><td>用户定义的信号</td><td>终止</td></tr>
 *<tr><td>36</td><td>SIGVTALRM</td><td>虚拟时间闹钟（setitimer）</td><td>终止</td></tr>
 *<tr><td>37</td><td>SIGWAITING</td><td>线程库内部使用</td><td>忽略</td></tr>
 *<tr><td>38</td><td>SIGWINCH</td><td>终端窗口大小改变</td><td>忽略</td></tr>
 *<tr><td>39</td><td>SIGXCPU</td><td>超过CPU限制（setrlimit）</td><td>终止+core</td></tr> 
 *<tr><td>40</td><td>SIGXFSZ</td><td>超过文件长度限制</td><td>终止+core</td></tr> 
 *<tr><td>41</td><td>SIGXRES</td><td>超过资源控制</td><td>忽略</td></tr> 
 *</table>
 *\retval 0 成功
 *\retval !0 失败
 */
int test_signal()
{
	return 0;
}