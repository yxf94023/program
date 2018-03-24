/**
 *\file c11_thread.cc
 *\brief 练习线程
 *\date 2018/03/19
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

/**
 *\brief 测试pthread_self函数
 *
 *pthread_t pthread_self(void);
 *
 *返回值：调用线程的线程ID
 *
 *<ol>
 *<li>每个线程具有唯一的线程ID，线程ID只在所属进程环境中有效</li>
 
*<li>线程ID用结构pthread_t数据类型表示，实现的时候可以用一个结构体来代表pthread_t数据类型， 所以可移植的操作系统实现不能把它作为整数处理。因此必须使用函数来对比两个线程ID进行比较：<br/>
 *
 *int pthread equal(pthread_t tid1, pthread_t tid2);
 *
 *返回值：若相等则返回非0值， 否则返回0
 *</li>
 *</ol>
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_pthread_self()
{
	pid_t pid;
	pthread_t tid;
	
	pid = getpid();
	tid = pthread_self();
	
	printf("%s:%d pid[%d] tid[%u]\n", __FILE__, __LINE__, pid, (unsigned int)tid);
	
	return 0;
}
 
static void *tpc_fun(void *arg)
{
	pthread_t tid = pthread_self();
	printf("%s:%d hello tid[%u]\n", __FILE__, __LINE__, (unsigned int)tid);
	
	return ((void*)0);
}
	
/**
 *\brief 测试pthread_create函数
 *
 *<code>
 *int pthread_create(pthread_t *restrict tidp,
 *					 const pthread_attr_t *restrict attr,
 *					 void *(*start_rtn)(void), void *restrict arg);
 *
 *返回值：若成功则返回0， 否则返回错误编号
 *</code>
 *
 *<ul>
 *<li>由tidp指向的内存单元被设置为新创建的线程ID</li>
 *<li>attr参数用于定制各种不同的线程属性，如果只为NULL，则为默认属性的线程</li>
 *<li>新创建的线程从start_rtn函数的地址开始执行， arg为函数的参数</li>
 *<li>线程创建时并不能保证哪个线程会先运行（创建线程与调用线程）。新建线程可以访问进程的地址空间，并且集成调用线程的浮点环境和信号屏蔽字，但是该线程的未决信号集被清楚</li>
 *<li>每个线程都提供errno的副本</li>
 *</ul>
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_pthread_create()
{
	int ret = 0;
	pthread_t tid;
	
	ret = pthread_create(&tid, NULL, tpc_fun, NULL);
	if (ret != 0){
		
		printf("%s:%d create thread failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
	printf("%s:%d world\n", __FILE__, __LINE__);
	sleep(1);
	
	return 0;
}

static void *exit_1(void *arg)
{
	printf("%s:%d thread1 .\n", __FILE__, __LINE__);
	sleep(10);
	return ((void*)1);
}

static void *exit_2(void *arg)
{
	printf("%s:%d thread2 .\n", __FILE__, __LINE__);
	return ((void*)2);
}

/**
 *\brief 测试pthread_exit函数
 *
 *<code>
 *void pthread_exit(void *rval_ptr);
 *
 *int pthread_join(pthread_t thread, void **rval_ptr);
 *
 *返回值：若成功则返回0，否则返回错误编号
 *</code>
 *
 *rval_ptr是一个无类型指针，与传给启动例程的单个参数类似。进程的其它线程可以通过调用pthread_join函数访问到这个指针。
 *
 *pthread_join调用线程将一直阻塞，直到指定的线程调用pthread_exit、从启动例程中返回或者被取消。<br/>
 *如果线程只是从它的启动例程返回，rval_ptr将包含返回码。<br/>
 *如果线程被取消，由rval_ptr指定的内存单元就置为PTHREAD_CANCELED。<br/>
 *如果对线程的返回值并不感兴趣， 可以把rval_ptr置为NULL。在这种情况下，调用pthread_join函数将等待指定的线程终止，但并不获取线程的终止状态。
 *
 *如果进程中的任一线程调用exit，_Exit或者_exit，那么整个进程就会终止。与此类似，如果信号的默认动作时终止进程，那么，把该信号发送到线程会终止整个进程。单个线程可以通过下列三种方式退出，在不中止整个进程的情况下停止它的控制流：
 *<ol>
 *<li>线程只是从启动例程中返回，返回值是线程的退出码</li>
 *<li>线程可以被同一进程中的其他线程取消</li>
 *<li>线程调用pthread_exit</li>
 *</ol>
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_pthread_exit()
{
	int ret = 0;
	pthread_t tid1, tid2;
	void *tret;
	
	ret = pthread_create(&tid1, NULL, exit_1, NULL);
	if (ret != 0){
		
		printf("%s:%d pthread_create failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
	ret = pthread_create(&tid2, NULL, exit_2, NULL);
	if (ret != 0){
		
		printf("%s:%d pthread_create failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
	
	ret = pthread_join(tid1, &tret);
	if (ret != 0){
		
		printf("%s:%d pthread_join failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
	printf("%s:%d thread 1 exit_code(%p)\n", __FILE__, __LINE__, tret);
	ret = pthread_join(tid2, &tret);
	if (ret != 0){
		
		printf("%s:%d pthread_join failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
	printf("%s:%d thread 2 exit_code(%p)\n", __FILE__, __LINE__, tret);
	
	ret = pthread_join(tid2, &tret);
	if (ret != 0){
		
		printf("%s:%d pthread_join failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
	printf("%s:%d thread 2 exit_code(%p)\n", __FILE__, __LINE__, tret);	
	return 0;
/*
# ./c11_thread pthread_exit
c11_thread.cc:97 thread1 .
c11_thread.cc:104 thread2 .
c11_thread.cc:158 thread 1 exit_code(0x1)
c11_thread.cc:165 thread 2 exit_code(0x2)
c11_thread.cc:173 pthread_join failed(No such process)
*/	
}

/**
 *\brief 测试pthread_cancel函数
 *
 *<code>
 *int pthread_cancel(pthread_t tid)
 *
 *返回值：若成功则返回0， 否则返回错误编号
 *</code>
 *
 *线程可以通过调用pthread_cancel函数来请求取消同一进程中的其它线程
 *默认的情况下，pthread_cancel函数会使得由tid标识的线程的行为如同调用了参数为PTHREAD_CANCELED的pthread_exit函数， 但是线程可以选择忽略取消方式或是控制取消方法。
 *
 *\warning 注意pthread_cancel并不等待线程终止， 它仅仅提出请求。所以， 系统并不会立即关闭被取消线程，仅仅有在被取消线程下次系统调用时，才会真正结束线程。或调用pthread_testcancel，让内核去检測是否须要取消当前线程。
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_pthread_cancel()
{
	int ret = 0;
	pthread_t tid1;
	void *tret;
	
	ret = pthread_create(&tid1, NULL, exit_1, NULL);
	if (ret != 0){
		
		printf("%s:%d pthread_create failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
	
	ret = pthread_cancel(tid1);
	if (ret != 0){
		
		printf("%s:%d pthread_cancel failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;		
	}
	ret = pthread_join(tid1, &tret);
	if (ret != 0){
		
		printf("%s:%d pthread_join failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
	printf("%s:%d thread 1 exit_code(%p)\n", __FILE__, __LINE__, tret);

	return 0;
/*
# ./c11_thread pthread_cancel
c11_thread.cc:97 thread1 .
c11_thread.cc:230 thread 1 exit_code(0xffffffffffffffff)

定义在Linux的pthread库中常数PTHREAD_CANCELED的值是-1
*/
}

static void show_help()
{
	printf("pthread_self, 测试pthread_self函数\n\n"
			"pthread_create, 测试pthread_create函数\n\n"
			"pthread_exit, 测试pthread_exit函数\n\n"
			"pthread_cancel, 测试pthread_cancel函数\n\n");
}

int main(int argc, char **argv)
{
	if (argc == 2 and strcmp("pthread_self", argv[1]) == 0){
		
		test_pthread_self();
	}else if (argc == 2 and strcmp("pthread_create", argv[1]) == 0){
		
		test_pthread_create();
	}else if (argc == 2 and strcmp("pthread_exit", argv[1]) == 0){
		
		test_pthread_exit();
	}else if (argc == 2 and strcmp("pthread_cancel", argv[1]) == 0){
		
		test_pthread_cancel();
	}else{
		
		show_help();
	}
	
	return 0;
}