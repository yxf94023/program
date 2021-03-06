/**
 *\file c09_process_relt.cc
 *\brief 练习进程关系
 *\date 2017/10/07
 */

#include <unistd.h>

/**
 *\brief 测试getpgid函数
 *
 *pid_t getpgid(pid_t pid);<br/>
 *返回值: 若成功则返回进程组ID， 若出错则返回-1<br/>
 *<br/>
 *<ul>
 *<li>进程组是一个或多个进程的集合</li>
 *<li>每个进程组有一个唯一的进程组ID，类似于进程ID</li>
 *<li>每个进程除了有一个进程ID外，还属于一个进程组</li>
 *<li>每个进程组都可以有一个组长进程，组长进程的标志是，其进程组ID等于进程ID</li>
 *<li>只要在某个进程组中有一个进程存在，则该进程组就存在，这与其组长进程是否终止无关</li> 
 *<li>从进程组创建开始到其中最后一个进程离开为止的时间区间称为进程组的生存期。进程组中的最后一个进程可以终止，或者转移到另一个进程组</li>
 *</ul>
 *\retval 0 成功
 *\retval !0 失败
 */
int test_getpgid()
{
	return 0;
}

/**
 *\brief 测试setpgid函数
 *
 *int setpgid(pid_t pid, pid_t pgid);<br/>
 *返回值：若成功则返回0， 若出错则返回-1<br/><br/>
 *函数将pid进程的进程组ID设置为pgid，如果这两个参数相等，则由pid指定的进程变成进程组组长。 如果pid是0， 则使用调用者的进程ID。另外，如果pgid是0，则由pid指定的进程ID将作用进程组ID<br/>
 *<br/>
 *<ul>
 *<li>进程通过调用setpgid来加入一个现有的组或者创建一个新进程组</li>
 *<li>一个进程只能为它自己或它的子进程设置进程组ID。但，如果子进程调用了exec函数后，父进程不能再改变子进程的进程组ID</li>
 *</ul>
 *\retval 0 成功
 *\retval !0 失败 
 */
int test_setpgid()
{
	return 0;
}

/**
 *\brief 测试setsid函数
 *
 *pid_t setsid(void);<br/>
 *返回值：若成功则返回进程组ID， 若出错则返回-1<br/><br/>
 *会话是一个或多个进程组的集合， 进程调用setsid函数建立一个新会话， 如果调用此函数的进程不是一个进程组的组长，则此函数就会创建一个新会话，结果将发生以下情况：<br/>
 *1. 该进程变成新会话首进程（session leader），此时， 该进程是新会话中唯一的进程<br/>
 *2. 该进程称为一个新进程组的组长进程，新进程组ID是该调用进程的进程组ID<br/>
 *3. 该进程没有控制终端， 如果在调用setsid之前该进程有一个控制终端， 那么这种联系也会被终端<br/>
 *4. 如果调用进程已经是一个进程组的组长， 则此函数返回出错。为了保证不会发生这种情况， 通常先调用fork，然后使其父进程终止， 而子进程则继续。 因为子进程继承了父进程的进程组ID，而其进程ID则是新分配的， 两者不可能相等， 所以这就保证子进程不会是一个进程组的组长<br/>
 *5. 会话进程是具有唯一进程ID的单个进程，所以可以将会话首进程ID视为会话ID。getsid函数返回会话首进程的进程组ID<br/>
 *
 *\retval 0 成功
 *\retval !0 失败 
 */
int test_setsid()
{
	return 0;
}

/**
 *\brief 测试getsid函数
 *
 *pid_t getsid(pid_t pid);<br/>
 *返回值：若成功则返回会话首进程的进程组ID， 若出错则返回-1<br/><br/>
 *
 *\warning 如果pid是0， getsid返回调用进程的会话首进程的进程组ID， 处于安全方面的考虑， 某些实现会有如下限制， 如若pid并不属于调用者所在的会话， 那么调用者就不能得到该会话首进程的进程组ID<br/>
 *
 *\retval 0 成功
 *\retval !0 失败 
 */
int test_getsid()
{
	return 0;
}

