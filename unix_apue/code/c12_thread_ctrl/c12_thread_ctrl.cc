/**
 *\file c11_thread_ctrl.cc
 *\brief 练习线程
 *\date 2018/04
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

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
	if (true){
		
		show_help();
	}
	
	return 0;
}