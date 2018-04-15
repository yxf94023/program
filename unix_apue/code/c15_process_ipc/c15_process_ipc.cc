/**
 *\file c15_process_ipc.cc
 *\brief 练习进程间通信
 *\date 2018/04/15
 */

//#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>


/**
 *\brief 测试pipe管道
 *
 *<code>
 *#include <unistd.h>
 *
 *int pipe(int filedes[2]);
 *
 *返回值：若成功则返回0， 若出错则返回-1
 *</code>
 *
 *管道是由调用pipe函数而创建的， 经由参数filedes返回两个文件描述符：filedes[0]为读而打开，filedes[1]为写而打开，filedes[1]的输出时fildes[0]的输入。<br/>
 *fstat函数对管道的每一端都返回一个FIFI类型的文件描述符，可以用S_ISFIFO宏来测试管道。<br/>
 *<ol>
 *<li>管道是UNIX系统IPC的最古老形式，并且所有UNIX系统都提供此种通信机制。历史上它是半双工的。现在，某些系统提供全双工管道，但是为了最佳的可移植性，我们绝不应预先假定系统使用此特性</li>
 *<li>管道只能在具有公共祖先的进程之间使用。通常，一个管道由一个进程创建，然后该进程调用fork，此后父、子进程之间就可应用该管道</li>
 *</ol>
 *
 *尽管有上面2种局限性，半双工管道仍是最常用的IPC形式。每当你在管道线中键入一个由shell执行的命令序列时，shell为每一条命令单独创建一进程，然后将前一条命令进程的标准输出用管道与后一条命令的标准输入相连接。<br/>
 *
 *单个进程中的管道几乎没有任何用处。通常，调用pipe的进程接着调用fork，这样就创建了从父进程到子进程（或反向）的IPC通道。调用fork后做什么取决于我们想要有的数据流的方向。对于父进程到子进程的管道，父进程关闭管道的读端（fd[0]），子进程关闭写端（fd[1]）；为了构造从子进程到父进程的管道，父进程关闭fd[1]，子进程关闭fd[0]。<br/>
 *当管道的一端被关闭后， 下列两条规则起作用：
 *<ol>
 *<li>当读一个写端已被关闭的管道时，在所有数据都被读取后，read返回0，以指示达到了文件结束处。</li>
 *<li>如果写一个读端已经关闭的管道，则产生信号SIGPIPE。如果忽略该信号或者捕捉该信号并从其处理程序返回，则write返回-1，errno设置为EPIPE。</li>
 *</ol>
 *
 *\warning 在写管道（或FIFO）时，常量PIPE_BUF规定了内核中管道缓冲区的大小。如果对管道调用write，而且要求写的字节数小于等于PIPE_BUF，则此操作不会与其他进程对同一管道（或FIFO）的write操作穿插进行。但是，若有多个进程同时写一个管道（或FIFO），而且有进程要求写的字节数超过PIPE_BUF字节数时，则写操作的数据可能相互穿插。用pathconf或fpathconf函数可以确定PIPE_BUF的值。
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_pipe()
{
	int fd[2];	// 定义2个描述符，将用于pipe创建管道
	//int ret = 0;
	pid_t pid = -1;
	
	// 先创建管道
	if (pipe(fd)){
		
		printf("%s:%d call pipe failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return 1;
	}
	
	// 将管道dup到标准输入输出上
	if (-1 == dup2(fd[0], STDIN_FILENO)){
		
		printf("%s:%d call dup2 failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return 1;		
	}
	if (-1 == dup2(fd[1], STDOUT_FILENO)){
		
		printf("%s:%d call dup2 failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return 1;		
	}
	
	// 创建子进程
	if ((pid = fork()) < 0){
		
		printf("%s:%d call fork failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return 1;
	}else if (pid == 0){// 子
	
		close(fd[0]);
		printf("helloworled\\n\n");
		
		sleep(3);
		exit(0);
	}
	close(fd[1]);
	char buf[80];
	sleep(1);
	scanf("%s", buf);
	//printf("%s\n", buf);
	write(STDERR_FILENO, buf, strlen(buf));
	sleep(3);
	waitpid(pid, NULL, 0);
/*
这里的输出， 要写到标准出错中， 不然直接调用printf是打印不出来的， 原因是 标准输出和输入都被管道了
# ./c15_process_ipc pipe 
helloworled\n
*/	
	return 0;
}

/**
 *\brief 测试popen管道
 *
 *<code>
 *#include <stdio.h>
 *
 *FILE *popen(const char *cmdstring, const char *type);
 *
 *返回值：若成功则返回文件指针，若出错则返回NULL
 *
 *int pclose(FILE *fp);
 *
 *返回值：cmdstring的终止状态，若出错则返回-1
 *</code>
 *
 *标准I/O库提供了popen和pclose，这两个函数实现的操作是：创建一个管道，调用fork产生一个子进程， 关闭管道的不使用端，调用exec执行一个shell以运行命令，然后等待命令终止，并且返回一个标准I/O文件指针。 如果type是“r”，则文件指针连接到cmdstring的标准输出；如果type是“w”，则文件指针连接到cmdstring的标准输入。<br/>
 *pclose函数关闭标准I/O流，等待命令执行结束，然后返回shell的终止状态。如果shell不能被执行，则pclose返回的终止状态与shell已执行exit(127)一样。
 *
 *\retval 0 成功
 *\retval !0 失败 
 */
int test_popen()
{
	// 将源文件.cc 以more的形式查看
	const char *PAGER = "${PAGER:-more}";
	char line[80];
	FILE *fpin, *fpout;
	
	if ((fpin = fopen("./c15_process_ipc.cc", "r")) == NULL){
		
		printf("%s:%d fopen failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return 1;
	}
	if ((fpout = popen(PAGER, "w")) == NULL){

		printf("%s:%d popen failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		fclose(fpin);
		return 1;	
	}
	
	while (fgets(line, sizeof(line), fpin) != NULL){
		
		if (fputs(line, fpout) == EOF){
			
			printf("%s:%d fputs failed(%s)\n", __FILE__, __LINE__, strerror(errno));
			fclose(fpin);
			pclose(fpout);
			return 1;
		}
	}

	fclose(fpin);
	pclose(fpout);	
	return 0;
}

static void show_help()
{
	printf("pipe, 测试pipe管道\n\n"
			"popen, 测试popen管道\n\n");
}

int main(int argc, char **argv)
{
	if (argc == 2 and strcmp(argv[1], "pipe") == 0){
		
		test_pipe();
	}else if (argc == 2 and strcmp(argv[1], "popen") == 0){
		
		test_popen();
	}else{
		
		show_help();
	}
	
	return 0;
}