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

static void clean(void *arg)
{
	printf("%s:%d clean(%s)\n", __FILE__, __LINE__, (char*)arg);
	return;
}

static void *clean_1(void *arg)
{
	printf("%s:%d clean_1 start\n", __FILE__, __LINE__);
	pthread_cleanup_push(clean, (void*)("thread clean_1 1"));
	pthread_cleanup_push(clean, (void*)("thread clean_1 2"));
	printf("%s:%d clean_1 end\n", __FILE__, __LINE__);
	
	//return ((void *)4);
	
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(0);
	return ((void *)8);
}

static void *clean_2(void *arg)
{
	printf("%s:%d clean_2 start\n", __FILE__, __LINE__);
	pthread_cleanup_push(clean, (void*)("thread clean_2 1"));
	pthread_cleanup_push(clean, (void*)("thread clean_2 2"));
	printf("%s:%d clean_2 end\n", __FILE__, __LINE__);
	
	pthread_exit((void*)5);
	
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(0);
	return ((void *)10);
}

/**
 *\brief 测试pthread_cleanup类函数
 *
 *<code>
 *void pthread_cleanup_push(void (*rtn)(void*), void *arg);
 *
 *void pthread_cleanup_pop(int execute);
 *
 *</code>
 *
 *线程清理处理程序：类似于进程调用atexit函数，线程也可以安排在自身退出时需要调用的函数。 调用的程序与注册时的顺序相反。
 *
 *一般来说，线程终止有两种场景：正常终止和异常终止。线程主动调用pthread_exit()或者从线程函数中return都将使线程正常退出，这是可预见的退出方法；线程异常终止是在其他线程的干预下（pthread_cancel)， 或者由于自身运行出错（访问非法地址）而退出，这种退出是不可预见的。
 *
 *不论是何种终止形式，都会存在资源释放的问题， 在不考虑因运行出错而退出的前提下，如何保证线程终止时能顺利的释放掉自己所占用的资源，特别是锁资源，就是一个必须考虑解决的问题。
 *
 *pthread_cleanup_push()/pthread_cleanup_pop()函数对用于自动释放资源从pthread_cleanup_push()的调用点到pthread_cleanup_pop()之间的程序段中的终止动作，都将执行pthread_cleanup_push所指定的清理函数。<b color="red">如果execute参数置为0，清理函数将不被调用。无论那种情况，pthread_cleanup_pop都将删除上次pthread_cleanup_push调用建立的清理处理程序</b>
 *
 *pthread_cleanup_push()/pthread_cleanup_pop()是以宏方式实现的，
 *<code>
 *
 *#define pthread_cleanup_push(routine,arg)   \ <br/>
  { struct _pthread_cleanup_buffer _buffer; \ <br/>
    _pthread_cleanup_push (&_buffer, (routine), (arg));<br/>
 *#define pthread_cleanup_pop(execute)        \ <br/>                                 
    _pthread_cleanup_pop (&_buffer, (execute)); }
 *</code>
 *
 *可见,pthread_cleanup_push()带有一个"{"，而pthread_cleanup_pop()带有一个"}"，因此这两个函数必须成对出现，且必须位于程序的同一级别的代码段中才能通过编译。
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_pthread_cleanup()
{
	int ret = 0;
	pthread_t tid1, tid2;
	void *tret;
	
	ret = pthread_create(&tid1, NULL, clean_1, (void *)1);
	if (ret != 0){
		
		printf("%s:%d pthread_create failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
	
	ret = pthread_create(&tid2, NULL, clean_2, (void *)1);
	if (ret != 0){
		
		printf("%s:%d pthread_create failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
	sleep(1);

	ret = pthread_join(tid1, &tret);
	if (ret != 0){
		
		printf("%s:%d pthread_join failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;		
	}
	printf("%s:%d pthread1 exit(%p)\n", __FILE__, __LINE__, tret);
	
	ret = pthread_join(tid2, &tret);
	if (ret != 0){
		
		printf("%s:%d pthread_join failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;		
	}
	printf("%s:%d pthread2 exit(%p)\n", __FILE__, __LINE__, tret);

	return 0;
/*
./c11_thread pthread_cleanup
c11_thread.cc:250 clean_1 start
c11_thread.cc:253 clean_1 end
c11_thread.cc:244 clean(thread clean_1 2)
c11_thread.cc:264 clean_2 start
c11_thread.cc:267 clean_2 end
c11_thread.cc:244 clean(thread clean_2 2)
c11_thread.cc:244 clean(thread clean_2 1)
c11_thread.cc:336 pthread1 exit(0x8)
c11_thread.cc:344 pthread2 exit(0x5)
*/	
}

static void *detach_1(void *arg)
{
	printf("%s:%d enter detach_1 thread\n", __FILE__, __LINE__);
	sleep(3);
	printf("%s:%d out detach_1 thread\n", __FILE__, __LINE__);
	
	return ((void*)38);
}

/**
 *\brief 测试pthread_detach函数
 *
 *<code>
 *
 *int pthread_detach(pthread_t tid);
 *
 *返回值： 若成功返回0， 否则返回错误编号
 *</code>
 *
 *pthread_detach调用可以用于使线程进入分离状态<br/>
 *默认情况下， 线程的终止状态会保存到对该线程调用pthread_join<br/>
 *如果线程已经处于分离状态，线程的底层存储资源可以在线程终止时立即被回收。<br/>
 *当线程被分离时，并不能用pthread_join函数等待它的终止状态。对分离状态的线程进行pthread_join的调用会产生实效，返回EINVAL。<br/>
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_pthread_detach()
{
	int ret = 0;
	pthread_t tid1;
	void *tret;
	
	ret = pthread_create(&tid1, NULL, detach_1, NULL);
	if (ret != 0){
		
		printf("%s:%d pthread_create failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
	
	ret = pthread_detach(tid1);
	if (ret != 0){
		
		printf("%s:%d pthread_detach failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
		
	sleep(1);
	
	ret = pthread_join(tid1, &tret);
	if (ret != 0){
		
		printf("%s:%d pthread_join failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
	
	return 0;
/*
# ./c11_thread pthread_detach
c11_thread.cc:363 enter detach_1 thread
c11_thread.cc:413 pthread_join failed(Invalid argument)
*/	
}

// 执行次数计数器
typedef struct counter_st{
	
	pthread_mutex_t mutex;	// 互斥量
	unsigned int id1;	// 计数器1
	unsigned int id2;	// 计数器2
	unsigned int total;	// 汇总
}counter_st;

static unsigned int s_mutex_run = 0;	// 初始化未完成的情况下是不允许工作的

static void *mutex_thr1(void *arg)
{
	counter_st *counter = (counter_st *)arg;
	
	while (!s_mutex_run){
		
		sleep(1);
	}
	
	unsigned int i = 0;
	int ret = 0;
	
	while (i < 10){
		
		// 先获取信号量
		ret = pthread_mutex_trylock(&counter->mutex);
		if (!ret){
			
			counter->id1++;
			counter->total++;
			++i;
			sleep(3);
		}else{
			
			printf("%s:%d trylock failed(%s)\n", __FILE__, __LINE__, strerror(ret));
			continue;
		}
		ret = pthread_mutex_unlock(&counter->mutex);
		if (ret){
			
			printf("%s:%d unlock failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		}
	}
	
	return (void *)1;
}


static void *mutex_thr2(void *arg)
{
	counter_st *counter = (counter_st *)arg;
	
	while (!s_mutex_run){
		
		sleep(1);
	}
	
	unsigned int i = 0;
	int ret = 0;
	
	while (i < 10){
		
		// 先获取信号量
		ret = pthread_mutex_lock(&counter->mutex);
		if (!ret){
			
			counter->id2++;
			counter->total++;
			++i;
			sleep(2);
		}else{
			
			printf("%s:%d lock failed(%s)\n", __FILE__, __LINE__, strerror(ret));
			continue;
		}
		ret = pthread_mutex_unlock(&counter->mutex);
		if (ret){
			
			printf("%s:%d unlock failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		}
	}
	
	return (void *)2;
}

/**
 *\brief 测试线程同步mutex函数
 *
 *<code>
 *int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
 *
 *int pthread_mutex_destroy(pthread_mutex_t *mutex);
 *
 *int pthread_mutex_lock(pthread_mutex_t *mutex);
 *
 *int pthread_mutex_trylock(pthread_mutex_t *mutex);
 *
 *int pthread_mutex_unlock(pthread_mutex_t *mutex);
 * 
 *返回值：若成功则返回0， 否则返回错误编码
 *
 *</code>
 *
 *互斥量(mutex)从本质上说是一把锁， 在访问共享资源前对互斥量进行加锁，在访问完成后释放互斥量上的锁。<br/>
 *对互斥量进行加锁后， 任何其他试图再次对互斥量加锁的线程将会被阻塞直到当前线程释放该互斥锁<br/>
 *如果释放互斥锁时有多个线程阻塞，所有在该互斥锁上的阻塞线程都会变成可运行状态，第一个变为运行状态的线程可以对互斥量加锁，其他线程将会看到互斥锁依然被锁住，只能回去再次等待它重新变为可用。
 *
 *在设计时需要规定所有的线程必须遵守相同的数据访问规则， 只有这样，互斥机制才能正常工作。操作系统并不会做数据访问的串行化。如果允许其中的某个线程在没有得到锁的情况下也可以访问共享资源，那么即使其他的线程在使用共享资源前都获取了锁，也还是会出现数据不一致的问题。
 *
 *<ul>
 *<li>互斥量初始化， 互斥量用pthread_mutex_t数据类型来表示，在使用互斥变量以前，必须首先对它进行初始化， 可以把它置为常量PTHREAD_MUTEX_INITIALIZER(只对静态分配的互斥量)， 也可以通过调用phtread_mutex_init函数进行初始化。需用默认的属性初始化互斥量，只需把attr设置为NULL。</li>
 *<li>互斥量销毁, 如果动态地分配互斥量（例如通过调用malloc函数），那么在释放内存前需要调用pthread_mutex_destroy。</li>
 *<li>互斥量加锁,  需要调用pthread_mutex_lock，如果互斥量已经上锁，调用线程将阻塞直到互斥量被解锁。如果线程不希望被阻塞， 它可以使用pthread_mutex_trylock尝试对互斥量进行加锁。如果调用pthread_mutex_trylock时互斥量处于未锁住状态， 那么pthread_mutex_trylock将锁住互斥量， 不会出现阻塞并返回0，否则pthread_mutex_trylock就会失败，不能锁住互斥量， 而返回EBUSY</li>
 *<li>对互斥量解锁，需要调用pthread_mutex_unlock。 </li>
 *</ul>
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_pthread_mutex()
{
	pthread_t tid1, tid2;
	counter_st cntr;
	void *tret;
	int ret = 0;
	
	// 初始化工作 初始化计数器， 新建2个计数进程
	memset(&cntr, 0, sizeof(counter_st));
	ret = pthread_mutex_init(&(cntr.mutex), NULL);
	if (ret != 0){
		
		printf("%s:%d mutex init failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
	ret = pthread_create(&tid1, NULL, mutex_thr1, (void*)&cntr);
	if (ret != 0){
		
		printf("%s:%d create thread failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		goto exit;
	}
	ret = pthread_create(&tid2, NULL, mutex_thr2, (void*)&cntr);
	if (ret != 0){
		
		printf("%s:%d create thread failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		goto exit;
	}
	s_mutex_run = 1;  // 初始化成功后 线程可真正工作
	
	ret = pthread_join(tid1, &tret);
	if (ret){
		
		printf("%s:%d pthread exit failed(%s)\n", __FILE__, __LINE__, strerror(ret));
	}
	printf("%s:%d code(%p)\n", __FILE__, __LINE__, tret);
	
	ret = pthread_join(tid2, &tret);
	if (ret){
		
		printf("%s:%d pthread exit failed(%s)\n", __FILE__, __LINE__, strerror(ret));
	}
	printf("%s:%d code(%p)\n", __FILE__, __LINE__, tret);	
	
	printf("%s:%d result id1(%u) id2(%u) total(%u)\n", __FILE__, __LINE__, cntr.id1, cntr.id2, cntr.total);
exit:
	pthread_mutex_destroy(&(cntr.mutex));
	
	return ret;
/*	
# ./c11_thread pthread_mutex
c11_thread.cc:579 code(0x1)
c11_thread.cc:586 code(0x2)
c11_thread.cc:588 result id1(10) id2(10) total(20)*/	
}


/**
 *\brief 测试线程同步rwlock函数
 *
 *读写锁与互斥量类似， 不过读写锁允许更高的并行性。互斥量要么是锁住状态要么是不加锁状态，而且一次只有一个线程可以对其加锁。读写锁可以有三种状态：
 *<ol>
 *<li>写模式加锁状态，在这个锁被解锁之前，所有试图对这个锁加锁的线程都会被阻塞。</li>
 *<li>不加锁状态</li>
 *<li>读模式加锁状态，所有试图以读模式对它进行加锁的线程都可以得到访问权，但是如果线程希望以写模式对此锁进行加锁，它必须阻塞直到所有的线程释放读锁。</li>
 *</ol>
 *一次只有一个线程可以占有写模式的读写锁， 但是多个线程可以同时占有读模式的读写锁。
 *
 *初始化 和 销毁操作
 *<code>
 *
 *int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
 *
 *int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
 *
 *int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
 *
 *int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
 *
 *int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
 *
 *返回值：若成功则返回0， 否则返回错误编号
 *</code>
 *
 *<ul>
 *<li>读写锁通过调用pthread_rwlock_init初始化，如果希望读写锁有默认的属性，可以给atrr设置为NULL</li>
 *<li>调用pthread_rwlock_destroy销毁工作</li>
 *<li>要在读模式下锁定读写锁，需调用pthread_rwlock_rdlock</li>
 *<li>要在写模式下锁定读写锁，需调用pthread_rwlock_wrlock</li>
 *<li>不论以何种方式锁住读写锁，都可以调用pthread_rwlock_unlock解锁</li>
 *</ul>
 *
 *非阻塞版本加锁
 *<code>
 *
 *int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
 *
 *int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
 *
 *返回值：若成功则返回0， 否则返回错误编号
 *
 *</code>
 *
 *可以获取锁时，函数返回0； 否则， 返回错误EBUSY
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_pthread_rwlock()
{
	return 0;
}

static pthread_mutex_t s_cond_mutex;
static pthread_cond_t s_cond_cond;
static int s_cond_num;

// 等待
static void *cond_thr1(void *arg)
{
	int ret = 0;
	
	printf("%s:%d enter thr1\n", __FILE__, __LINE__);
	ret = pthread_mutex_lock(&s_cond_mutex);
	printf("%s:%d 1 lock mutex\n", __FILE__, __LINE__);
	if (ret){
		
		printf("%s:%d mutex lock failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return (void*)__LINE__;
	}
	while (!s_cond_num){
		
		printf("%s:%d waiting cond\n", __FILE__, __LINE__);
		ret = pthread_cond_wait(&s_cond_cond, &s_cond_mutex);
		if (ret){
			
			printf("%s:%d cond wait failed(%s)\n", __FILE__, __LINE__, strerror(ret));
			
			return (void*)__LINE__;
		}
	}
	printf("%s:%d cond status is ok\n", __FILE__, __LINE__);
	s_cond_num -= 1;
	ret = pthread_mutex_unlock(&s_cond_mutex);
	printf("%s:%d unlock mutex\n", __FILE__, __LINE__);
	if (ret){
		
		printf("%s:%d mutex unlock failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return (void *)__LINE__;
	}
	
	return (void *)1;
}

static void *cond_thr2(void *arg)
{
	int ret = 0;
	
	printf("%s:%d enter thr2 \n", __FILE__, __LINE__);
	ret = pthread_mutex_lock(&s_cond_mutex);
	printf("%s:%d lock mutex\n", __FILE__, __LINE__);
	if (ret){
		
		printf("%s:%d mutex lock failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return (void *)__LINE__;
	}
	if (s_cond_num == 0){
		
		printf("%s:%d send signal\n", __FILE__, __LINE__);
		ret = pthread_cond_signal(&s_cond_cond);
		if (ret){
			
			printf("%s:%d cond signal failed(%s)\n", __FILE__, __LINE__, strerror(ret));
			return (void *)__LINE__;
		}
	}
	
	sleep(3);
	s_cond_num += 1;
	ret = pthread_mutex_unlock(&s_cond_mutex);
	printf("%s:%d unlock mutex \n", __FILE__, __LINE__);
	if (ret){
		
		printf("%s:%d mutex unlock failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return (void *)__LINE__;
	}
	
	return (void *)2;
}

/**
 *\brief 测试线程同步cond函数
 *
 *条件变量是利用线程间共享的全局变量进行同步的一种机制，主要包括两个动作，线程等待“条件变量的条件成立”而挂起，另外的线程使“条件成立”（给出条件成立信号）。为了防止竞争，条件变量的本身是由互斥量保护的。 线程在改变条件状态前必须首先锁住互斥量，其他线程在获得互斥量之前不会察觉到这种改变，因为必须锁定互斥量以后才能计算条件。
 *<code>
 *
 *int pthread_cond_init(pthread_cond_t *restrict cond, pthread_condattr_t restrict attr);
 *
 *int pthread_cond_destroy(pthread_cond_t *cond);
 *
 *int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t  restrict mutex);
 *
 *int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict timeout);
 *
 *int pthread_cond_signal(pthread_cond_t *cond);
 *
 *int pthread_cond_brodcast(pthread_cond_t *cond);
 *
 *返回值：若成功则返回0， 否则返回错误编号
 *</code>
 *
 *<ul>
 *<li>条件变量使用之前必须首先进行初始化，可以把常量PTHREAD_COND_INITIALIZER赋给静态分配的条件变量， 但是如果条件变量是动态分配的， 可以使用pthread_cond_init函数进行初始化</li>
 *<li>对pthread_mutex_destroy函数对条件变量进行去除初始化</li>
 *<li>使用pthread_cond_wait等待条件变为真，也称作无条件等待</li>
 *<li>使用pthread_cond_timedwait如果在给定的时间内条件不能满足，那么会生成一个代表出错码的返回值(ETIMEOUT)，也称作计时等待。
 *
 *struct timespec{<br/>
	 time_t tv_sec;	// 秒<br/>
	 long tv_nsec;	// 纳秒<br/>
 };<br/>
 *使用这个结构时， 需要制定愿意等待多长时间，时间值是一个绝对数而不是相对数。例如，如果能等待3分钟，就需要把当前时间加上3分钟再转换到timespec结构，而不是把3分钟转换成timespec结构
 *</li>
 *<li>最后2个函数用于通知线程条件已经满足， pthread_cond_signal函数将唤醒等待该条件的某个线程， 而pthread_cond_broadcast函数将唤醒等待该条件的所有线程。必须注意一定要在改变条件状态以后再给线程发送信号</li>
 *</ul>
 *\retval 0 成功
 *\retval !0 失败
 */
int test_pthread_cond()
{
	pthread_t tid1, tid2;
	void *tret;
	int ret = 0;
	
	ret = pthread_mutex_init(&s_cond_mutex, NULL);
	if (ret){
		
		printf("%s:%d mutex init failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		return 1;
	}
	ret = pthread_cond_init(&s_cond_cond, NULL);
	if (ret){
		
		printf("%s:%d cond init failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		pthread_mutex_destroy(&s_cond_mutex);
		return 2;
	}
	ret = pthread_create(&tid1, NULL, cond_thr1, NULL);
	if (ret){
		
		printf("%s:%d creat thread failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		goto exit_label;
	}
	ret = pthread_create(&tid2, NULL, cond_thr2, NULL);
	if (ret){
		
		printf("%s:%d creat thread failed(%s)\n", __FILE__, __LINE__, strerror(ret));
		goto exit_label;
	}
	
	pthread_join(tid1, &tret);
	pthread_join(tid2, &tret);
exit_label:	
	pthread_mutex_destroy(&s_cond_mutex);
	pthread_cond_destroy(&s_cond_cond);

/*
# ./c11_thread pthread_cond
c11_thread.cc:666 enter thr1
c11_thread.cc:668 1 lock mutex
c11_thread.cc:676 waiting cond
c11_thread.cc:702 enter thr2 
c11_thread.cc:704 lock mutex
c11_thread.cc:712 send signal
c11_thread.cc:724 unlock mutex 
c11_thread.cc:685 cond status is ok
c11_thread.cc:688 unlock mutex
*/	
	return ret;
}

static void show_help()
{
	printf("pthread_self, 测试pthread_self函数\n\n"
			"pthread_create, 测试pthread_create函数\n\n"
			"pthread_exit, 测试pthread_exit函数\n\n"
			"pthread_cancel, 测试pthread_cancel函数\n\n"
			"pthread_cleanup, 测试pthread_cleanup函数\n\n"
			"pthread_detach, 测试pthread_detach函数\n\n"
			"pthread_mutex, 测试pthread_mutex函数\n\n"
			"pthread_rwlock, 测试pthread_rwlock函数\n\n"
			"pthread_cond, 测试pthread_cond函数\n\n");
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
	}else if (argc == 2 and strcmp("pthread_cleanup", argv[1]) == 0){
		
		test_pthread_cleanup();
	}else if (argc == 2 and strcmp("pthread_detach", argv[1]) == 0){
		
		test_pthread_detach();
	}else if (argc == 2 and strcmp("pthread_mutex", argv[1]) == 0){
		
		test_pthread_mutex();
	}else if (argc == 2 and strcmp("pthread_cond", argv[1]) == 0){
		
		test_pthread_cond();
	}else{
		
		show_help();
	}
	
	return 0;
}