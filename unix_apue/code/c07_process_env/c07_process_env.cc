/**
 *\file c07_process_env.cc
 *\brief 练习进程环境
 *\date 2017/10/01
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/**
 *\brief 测试exit函数
 *
 *有8种方式使进程终止，前5种为正常终止，后3种为异常终止<br/>
 *1、从main返回<br/>
 *2、调用exit<br/>
 *3、调用_exit或_Exit<br/>
 *4、最后一个线程从其启动例程返回<br/>
 *5、最后一个线程调用pthread_exit<br/>
 *
 *6、调用abort<br/>
 *7、接到一个信号并终止<br/>
 *8、最后一个线程对取消请求做出响应<br/>
 *
 *void exit(int status);<br/>
 *void _Exit(int status);<br/>
 *void _exit(int status);<br/>
 *<br/>
 *_exit和_Exit立即进入内核，exit则先执行一些清理处理（包括调用执行各终止处理程序，关闭所有标准I/O流等），然后进入内核。由于历史原因， exit函数总是执行一个标准I/O库的清理关闭操作，为所有打开调用fclose函数，这会使得所有缓冲的输出数据都被冲洗写到文件上。<br/>
 *三个exit函数都带一个整型参数，称之为终止状态。<br/>
 *
 *环境表:
 *每个程序都会接收到一张环境表，与参数表一样，环境表也是一个字符指针数组，其中每个指针包含一个以null结束的C字符串的地址。全局变量environ则包含了该指针数组的地址： extern char **environ;<br/>
 *<br/>
 *程序的存储空间布局：
 *正文段，这是由CPU执行的机器指令部分，通常，正文段是可共享的，所以即使是频繁执行的程序在存储器中也只需有一个副本，另外，正文段常常是只读的，以防止程序由于意外而修改其自身的指令<br/>
 *初始化数据段，通常将此段称为数据段， 它包含了程序中需明确地赋值的变量<br/>
 *非初始化数据段，通常将此段成为bss段， 在程序开始执行之前，内核将此段中的数据初始化为0或空指针<br/>
 *栈，自动变量以及每次函数调用时所需保存的信息都存放在此段中，每次调用函数时， 其返回地址以及调用者的环境信息都存放在栈中<br/>
 *堆，通常在堆中进行动态存储分配，由于历史上形成的惯例，堆位于非初始化数据段和栈之间<br/>
 *<br/>
 *共享库：
 *共享库使得可执行文件中不再需要包含公用的库例程，而只需在所有进程都可引用的存储区中维护这种库例程的一个副本。程序第一次执行或者第一次调用某个库函数时，用动态链接方法将程序与共享库函数相链接，这减少了每个可执行文件的长度，但增加了一些运行时间开销，这种时间开销发生在该程序第一次被执行时，或者每个共享库函数第一次被调用时。共享库的另一个优点是可以用库函数的心版本代替老版本，而无需对使用该库的程序重新连接编辑。
 * 
 *\retval 0 	成功
 *\retval !0	失败
 */
int test_exit()
{
	return 0;
}

/**
 *\brief 测试atexit函数
 *
 *int atexit(void (*func)(void));<br/>
 *返回值: 若成功则返回0，若出错则返回非0值
 *<br/><br/>
 *按照规定，一个进程可以登记多大32个函数，这些函数将由exit自动调用，称这些函数为终止处理程序（exit handler），并调用atexit函数来登记这些函数。 其中，atexit的参数是一个函数地址，当调用此函数时无需向它传送任何参数，也不期望它返回一个值。exit调用这些函数的顺序与它们登记时候的顺序相反，同一个函数如若登记多次，则也会调用多次。<br/>
 *内核使程序执行的唯一方法是调用一个exec函数，进程自愿终止的唯一方法是显示或隐式（通过调用exit）调用_exit或_Exit，进程也可非自愿地由一个信号使其终止
 *
 *\retval 0 	成功
 *\retval !0	失败
 */ 
int test_atexit()
{
	return 0;
}

/**
 *\brief 测试环境变量
 *
 *char *getenv(const char *name);<br/>
 *返回值：指向与name关联的value的指针，若未找到则返回NULL<br/>
 *<br/>
 *函数返回一个指针，它指向name=value字符串中的value，我们应当使用getenv从环境中取一个指定环境变量的值， 而不是直接访问environ<br/>
 *
 *\retval 0 	成功
 *\retval !0	失败
 */
int test_getenv()
{
	return 0;
}

/**
 *\brief 测试设置环境变量
 *
 *int putenv(char *str);<br/>
 *int setenv(const char *name, const char *value, int rewrite);<br/>
 *int unsetenv(const char *name);<br/>
 *返回值：若成功则返回0， 若出错则返回非0值<br/>
 *除了取环境变量的值，有时也需要设置环境变量，我们可能希望改变现有变量的值，或者增加新的环境变量
 *<ul>
 *<li>putenv取形式为name=value的字符串，将其放到环境表中，如果name已经存在，则先删除其原来的定义</li>
 *<li>setenv将name设置为value，如果在环境中name已经存在， 那么，rewrite非0， 则首先删除其现有的定义；若rewrite为0，则不删除其现有定义</li>
 *<li>unsetenv删除name的定义，即使不存在这种定义也不算出错</li>
 *</ul>
 *环境表和环境字符串通常存放在进程存储空间的顶部（栈之上），删除一个字符串很简单，只要先在环境表中找到该指针，然后将所有后续指针都向环境表首部顺次移动一个位置，但是增加一个字符串或修改一个现有的字符串就困难得多。环境表和环境字符串通常占用的是进程地址空间的顶部，所以它不能再向高地址方向（向上）扩展，同时也不能移动在它之下的各栈帧，所以它也不能向低地址向（向下）扩展，两者组合使得该空间的长度不能再增加。
 *<ol>
 *<li>如果修改一个现有的name<br/>
 (a)如果新value的长度少于或等于现有value的长度，则只要在原字符串所用空间中写入新字符串<br/>
 (b)如果新value的长度大于原长度，则必须调用malloc为新字符串分配空间，然后将新字符串复制到该空间在红，接着使环境表中针对name的指针指向新分配区<br/>
 </li>
 *<li>如果要增加一个新的name，则操作就更加复杂，首先调用malloc为name=value字符串分配空间，然后将该字符串复制到此空间中<br/>
 (a)如果这是第一次增加 一个新的name，则必须调用malloc为新的指针表分配空间，接着，将原来的环境表复制到新分配区，并将指向新name=value字符串的指针存放在该指针表的表尾，然后又将一个空指针存放在其后。最后使environ指向新指针表。<br/>
 (b)如果这不是第一次增加一个新name，则可知以前调用malloc在堆中为环境表分配了空间，所以只要调用realloc，以分配比原空间多存放一个指针的空间，然后，将指向新name=value字符串的指针存放在该表表尾，后面跟一个空指针</li>
 *</ol>
 *
 *\retval 0 	成功
 *\retval !0	失败 
 */
int test_setenv()
{
	return 0;
}

/**
 *\brief 设置资源
 *
 *int getrlimit(int resource, struct rlimit *rlptr);<br/>
 *int setrlimit(int resource, const struct rlimit *rlptr);<br/>
 *返回值：若成功则返回0， 若出错则返回非0值<br/>
 *每个进程都有一组资源限制，其中一些可以用getrlimit和setrlimit函数查询和更改。进程的资源限制通常是在系统初始化时由进程0建立的，然后由每个后续进程继承。每种实现都可以用自己的方法对各种限制做出调整。<br/>
 *struct rlimit{<br/>
	rlim_t	rlim_cur;	///< 软限制<br/>
	rlim_t	rlim_max;	///< 硬限制， maxinum value for rlim_cur<br/>
 };<br/>
 *在更改资源限制时， 须遵循下列三个规则：<br/>
 *1、任何一个进程都可将一个软限制值更改为小于或等于其硬限制值<br/>
 *2、任何一个进程都可降低其硬限制值，但它必须大于或等于其软限制值，这种降低对普通用户而言是不可逆的<br/>
 *3、只有超级用户进程可以提高硬限制值<br/>
 *<table>
 *<caption>resource类型</caption>
 *<tr><th width="40">序号</th><th width="80">名称</th><th>描述</th></tr>
 *<tr><td>1</td><td>RLIMIT_AS</td><td>进程可用存储区的最大总长度（字节），这回影响sbrk函数和mmap函数</td></tr>
 *<tr><td>2</td><td>RLIMIT_CORE</td><td>core文件的最大字节数，若其值为0则阻止创建core文件</td></tr>
 *<tr><td>3</td><td>RLIMIT_CPU</td><td>CPU时间的最大量值（秒），当超过此软限制时，向该进程发送SIGXCPU信号</td></tr>
 *<tr><td>4</td><td>RLIMIT_DATA</td><td>数据段的最大字节长度，初始化数据、非初始以及堆的总和</td></tr>
 *<tr><td>5</td><td>RLIMIT_FSIZE</td><td>可以创建的文件的最大字节长度，当超过此软限制时， 则向该进程发送SIGXFSZ</td></tr>
 *<tr><td>6</td><td>RLIMIT_LOCKS</td><td>一个进程可持有的文件锁的最大数</td></tr>
 *<tr><td>7</td><td>RLIMIT_MEMLOCK</td><td>一个进程使用mlock能够锁定在存储器中的最大字节长度</td></tr>
 *<tr><td>8</td><td>RLIMIT_NOFILE</td><td>每个进程能打开的最大文件数，更改此限制将影响到sysconf函数在参数_SC_OPEN_MAX中返回的值</td></tr>
 *<tr><td>9</td><td>RLIMIT_NPROC</td><td>每个实际用户ID可拥有的最大进程数，更改此限制将影响到sysconf函数在参数_SC_CHILD_MAX中返回的值</td></tr>
 *<tr><td>10</td><td>RLIMIT_RSS</td><td>最大驻内存集的字节长度，如果物理存储器供不应求，则内核将从进程处取回超过RSS的部分</td></tr>
 *<tr><td>11</td><td>RLIMIT_SBSIZE</td><td>用户在任一给定时刻可以占用的套接字缓冲区的最大长度（字节）</td></tr>
 *<tr><td>12</td><td>RLIMIT_STACK</td><td>栈的最大字节长度</td></tr>
 *<tr><td>13</td><td>RLIMIT_VMEM</td><td>这是RLIMIT_AS的同义词</td></tr>
 *</table>
 *资源限制影响到调用进程并由其子进程继承，这就意味着为了影响一个用户的所有后续进程，需将资源限制的设置构造在shell之中。
 *
 *\retval 0 	成功
 *\retval !0	失败 
 */
int test_getrlimit()
{
	return 0;
} 