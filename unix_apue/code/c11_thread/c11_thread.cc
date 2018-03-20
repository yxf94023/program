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

/**
 *\brief 测试pthread_self函数
 *
 *pthread_t pthread_self(void);
 *
 *返回值：调用线程的线程ID
 *
 *<ol>
 *<li>每个线程具有唯一的线程ID，线程ID只在所属进程环境中有效</li>
 *<li>线程ID用结构pthread_t数据类型表示</li>
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
 
/**
 *\brief 测试pthread_create函数
 *
 *int pthread_create(pthread_t *restrict tidp,
 *					 const pthread_attr_t *restrict attr,
 *					 void *(*start_rtn)(void), void *restrict arg);
 *
 *返回值：若成功则返回0， 否则返回错误编号
 *
 *
 *
 */
int test_pthread_create()
{
	return 0;
}

static void show_help()
{
	printf("pthread_self, 测试pthread_self函数\n\n");
}

int main(int argc, char **argv)
{
	if (argc == 2 and strcmp("pthread_self", argv[1]) == 0){
		
		test_pthread_self();
	}else{
		
		show_help();
	}
	
	return 0;
}