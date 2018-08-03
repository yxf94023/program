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
#include <ctype.h>


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
 *fstat函数对管道的每一端都返回一个FIFO类型的文件描述符，可以用S_ISFIFO宏来测试管道。<br/>
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
 *\warning 注意，popen绝不应由设置用户ID或设置组ID程序调用。当它执行命令时，popen等同于：execl("/bin/sh", "sh", "-c", command, NULL); 它在从调用者继承的环境中执行shell，并由shell解释执行command。一个心怀不轨的用户可以操纵这种环境，使得shell能以设置ID文件模式所授予的提升了的权限以及非预期的方式执行命令。
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

static int test_echo2()
{
	int c;
	
	while ((c = getchar()) != EOF){
		
		if (isupper(c)){
			
			c = tolower(c);
		}
		
		if (putchar(c) == EOF){
			
			printf("%s:%d putchar failed\n", __FILE__, __LINE__);
		}
		
		if (c == '\n'){
			
			fflush(stdout);
		}
	}
	
	exit(0);
}

/**
 *\brief 测试利用管道进行回显功能
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_echo()
{
	char line[80] = {0};
	FILE *fp = NULL;
	
	if ((fp = popen("./c15_process_ipc test_echo", "r")) == NULL){
		
		printf("%s:%d popen failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return 1;
	}
	
	while (1){
		
		fputs("prompt> ", stdout);
		fflush(stdout);
		if (fgets(line, sizeof(line), fp) == NULL){
			
			break;
		}
		if (fputs(line, stdout) == EOF){
			
			printf("%s:%d fputs failed(%s)\n", __FILE__, __LINE__, strerror(errno));
			pclose(fp);
			return 1;
		}
	}
	if (pclose(fp) == -1){
		
		printf("%s:%d pclose failed\n", __FILE__, __LINE__);
	}
	putchar('\n');
	exit(0);
}

static void sig_pipe(int signo)
{
	printf("%s:%d SIGPIPE caught\n", __FILE__, __LINE__);
	exit(1);
}

/**
 *\brief 测试协同进程
 *
 * UNIX系统过滤程序从标准输入读取数据，对其进行适当处理后写到标准输出。几个过滤程序通常在shell管道命令行中线性地连接。当一个程序产生某个过滤程序的输入， 同时又读取该过滤程序的输出时，则该过滤程序就成为协同进程。
 *
 *popen只提供连接到另一个进程的标准输入或标准输出的一个单向管道，而对于协同进程，则它有连接到另一个进程的两个单向管道------一个连到其标准输入，另一个则来自其标准输出。我们先要将数据写到其标准输入， 经其处理后， 再从其标准输出读取数据。
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_ipc_add()
{
	// 先创建2个管道
	int n, fd1[2], fd2[2];
	pid_t pid;
	char line[80];
	
	if (signal(SIGPIPE, sig_pipe) == SIG_ERR){
		
		printf("%s:%d signal error\n", __FILE__, __LINE__);
		return 1;
	}
	
	if (pipe(fd1) < 0 || pipe(fd2) < 0){
		
		printf("%s:%d pipe error\n", __FILE__, __LINE__);
		return 2;
	}
	
	if ((pid = fork()) < 0){
		
		printf("%s:%d fork error\n", __FILE__, __LINE__);
		return 3;
	}else if (pid > 0){// 父进程
	
		close(fd1[0]); // 关掉读
		close(fd2[1]); // 关掉写
		while (fgets(line, 80, stdin) != NULL){
			
			n = strlen(line);
			if (write(fd1[1], line, n) != n){
				
				printf("%s:%d write error to pipe\n", __FILE__, __LINE__);
			}
			if ((n = read(fd2[0], line, 80)) < 0){
				
				printf("%s:%d read error from pipe\n", __FILE__, __LINE__);
			}
			if (n == 0){
				
				printf("%s:%d child closed pipe\n", __FILE__, __LINE__);
				break;
			}
			line[n] = 0;
			
			if (fputs(line, stdout) == EOF){
				
				printf("%s:%d fputs error\n", __FILE__, __LINE__);
			}
		}
		
		if (ferror(stdin)){
			
			printf("%s:%d fgets error on stdin\n", __FILE__, __LINE__);
		}
		exit(0);
	} else {
		
		close(fd1[1]);
		close(fd2[0]);
		if (fd1[0] != STDIN_FILENO){
			
			if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO){}
			close(fd1[0]);
		}
		
		if (fd2[1] != STDOUT_FILENO){
			
			if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO){}
			close(fd2[1]);
		}
		
		if (execl("./c15_process_ipc", "c15_process_ipc", "ipc_add2", (char*)0)< 0){
			
			printf("%s:%d execl error\n", __FILE__, __LINE__);
		}
	}
	exit(0);
	
	return 0;
// 调用过程
/* 
root      3036  0.0  0.3 105476  3188 ?        Ss   Apr22   0:00 /usr/sbin/sshd -D
root       338  0.0  0.5 148880  5664 ?        Ss   00:42   0:00  \_ sshd: root@pts/1,pts/3
root       344  0.0  0.2 115380  2100 pts/1    Ss   00:42   0:00      \_ -bash
root       998  0.0  0.0  14648   844 pts/1    S+   00:58   0:00      |   \_ ./c15_process_ipc ipc_add1
root       999  0.0  0.0  14640   844 pts/1    S+   00:58   0:00      |       \_ c15_process_ipc ipc_add2	

[root@iZwz9899g8tgius3aqueaqZ admin]# ./c15_process_ipc ipc_add1
7 8
15
100 1000
1100
^C

*/
}
static int add2()
{
	int n, int1, int2;
	char line[80] = {0};
	
	while ((n = read(STDIN_FILENO, line, 80)) > 0){
		
		line[n] = 0;
		if (sscanf(line, "%d%d", &int1, &int2) == 2){
			
			sprintf(line, "%d\n", int1 + int2);
			n = strlen(line);
			if (write(STDOUT_FILENO, line, n) != n){
				
				printf("%s:%d write error\n", __FILE__, __LINE__);
			}
		}else{
			
			if (write(STDOUT_FILENO, "invalid args\n", 13) != 13){
				
				printf("%s:%d write error\n", __FILE__, __LINE__);
			}
		}
	}
	
	return 0;
}

/**
 *\brief 测试FIFO
 *
 *FIFO有时称为命名管道。管道只能由相关进程使用， 这些相关进程的共同的祖先进程创建了管道。 但是， 通过FIFO，不相关的进程也能交换数据。
 *
 *因为FIFO是一种文件类型， 所以， 创建FIFO类似于创建文件。
 *
 *<code>
 *#include <sys/stat.h>
 *
 *int mkfifo(const char *pathname, mode_t mode);
 *
 *</code>
 *
 *一旦已经用mkfifo创建了一个FIFO， 就可用open打开它。 其实， 一般的文件I/O函数（close，read，write，unlink等）都可用于FIFO
 *
 *当打开一个FIFO时， 非阻塞标志（O_NONBLOCK）产生下列影响：
 *<ol>
 *<li>在一般情况中（没有指定O_NONBLOCK）， 只读open要阻塞到某个其他进程为写而打开此FIFO。类似地， 只写open要阻塞到某个其他进程为读而打开它</li>
 *<li>如果指定了O_NONBLOCK，则只读open立即返回。 但是， 如果没有进程已经为读而打开一个FIFO， 那么只写open将出错返回-1， 其errno是ENXIO</li>
 *</ol>
 *
 *类似于管道， 若用write写一个尚无进程为读而打开的FIFO， 则产生信号SIGPIPE。
 *
 *一个给定的FIFO有多个写进程是很常见的。这意味着如果不希望多个进程所写的数据互相穿插， 则需考虑原子写操作。正如对于管道一样，常量PIPE_BUF说明了可被原子地写到FIFO的最大数据量。
 *
 *FIFO有下面2种用途：
 *<ol>
 *<li>FIFO由shell命令使用以便将数据从一条管道线传送到另一条， 为此无需创建中间临时文件</li>
 *<li>FIFO用于客户进程-服务器进程应用程序中， 以在客户进程和服务器进程之间传递数据。</li>
 *</ol>
 *
 *\param[in] path fifo文件名
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_read_fifo(const char *path)
{// 读fifo

	char buf[80] = {0};
	int fd = -1, res = 0;
	
	// 如果文件不存在  则创建
	if (access(path, F_OK) != 0){
		
		printf("%s:%d create fifo[%s]\n", __FILE__, __LINE__, path);
		if (mkfifo(path, 0777)){
			
			printf("%s:%d error(%s)\n", __FILE__, __LINE__, strerror(errno));
			return -1;
		}
	}
	
	fd = open(path, O_RDONLY);
	if (fd < 0){
		
		printf("%s:%d error(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	
	res = read(fd, buf, sizeof(buf));
	if (res <= 0){
		
		printf("%s:%d error(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;		
	}
	
	printf("%s\n", buf);
	
	return 0;
}

int test_write_fifo(const char *path)
{// 写fifo

	char buf[80] = {0};
	int fd = -1, res = 0;
	
	// 如果文件不存在  则创建
	if (access(path, F_OK) != 0){
		
		printf("%s:%d create fifo[%s]\n", __FILE__, __LINE__, path);
		if (mkfifo(path, 0777)){
			
			printf("%s:%d error(%s)\n", __FILE__, __LINE__, strerror(errno));
			return -1;
		}
	}
	
	fd = open(path, O_WRONLY);
	if (fd < 0){
		
		printf("%s:%d error(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	
	printf("Enter string:");
	scanf("%s", buf);
	
	res = write(fd, buf, strlen(buf));
	if (res <= 0){
		
		printf("%s:%d error(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;		
	}
	
	printf("%s\n", buf);
	
	return 0;	
}

/**
 *\brief XSI IPC
 *
 *有三种IPC我们称作XSI IPC， 即消息队列、信号量以及共享存储器，它们之间有很多相似之处。
 *
 *#include <sys/ipc.h>
 *
 *key_t ftok(const char *path, int id);
 *
 *若成功则返回键， 若出错则返回(key_t)-1;
 *
 *path参数必须引用一个现存文件。 当产生键时， 只使用id参数的低8位。
 *<table>
 *<caption>XSI IPC基本概念</caption>
 *<tr><th width="40">序号</th><th width="80">名字</th><th width="400">描述</th><th>对比</th></tr>
 *<tr><td rowspan="3">01</td><td rowspan="3">标识符和键</td><td>每个内核中的IPC的结构（消息队列、信号量或共享存储段）都用一个非负整数的标识符加以引用。</td><td>当一个IPC结构被创建， 以后又被删除时， 与这种结构相关的标识符连续加1，直至达到一个整数的最大正值，然后又回转到0。</td></tr>
 *<tr><td>标识符是IPC对象的内部名，为了使多个合作进程能够在同一IPC对象上会合，需要提供一个外部名方案。为此使用了键，每个IPC对象都与一个键相关联，于是键就用作为该对象的外部名</td><td>有多种方法使客户进程和服务器进程在同一IPC结构上会合：
 <ol>
 <li>服务器进程可以指定键IPC_PRIVATE创建一个新IPC结构，将返回的标识符存放在某处（例如一个文件）以便客户进程取用。键IPC_PRIVATE保证服务器进程创建一个新IPC结构。这种技术的缺点是：服务器进程要将整型标识符写到文件中，此后客户进程又要读文件取得此标识符。</li>
 <li>在一个公用头文件中定义一个客户进程和服务器进程都认可的键。然后服务器进程指定此键创建一个新的IPC结构。这种方法的问题是该键可能已与一个IPC结构相结合，在此情况下， get函数（msgget、semget或shmget）出错返回。服务器进程必须处理这一错误，删除已存在的IPC结构，然后试着再创建它。</li>
 <li>客户进程和服务器进程认同一个路径名和项目ID（项目ID是0~255之间的字符值），接着调用函数ftok将这两个值变换为一个键。然后在方法（2）中使用此键。ftok提供的唯一服务就是有一个路径名和项目ID产生一个键。</li>
 </ol></td></tr>
 *<tr><td>三个get函数（msgget、semget和shmget）都有两个类似的参数：一个key和一个整型flag。若满足下列两个条件之一，则创建一个新的IPC结构（通常由服务器进程创建）
 <ol>
 <li>key是IPC_PRIVATE</li>
 <li>key当前未与特定类型的IPC结构相结合，并且flag中指定了IPC_CREAT位</li>
 </ol></td><td>为访问现存的队列（通常由客户进程进行），key必须等于创建该队列时所指定的键， 并且不应指定IPC_CREAT<br/><br/>
 为了访问一个现存队列， 决不能指定IPC_PRIVATE作为键。因为这是一个特殊的键值，它总是用于创建一个新队列。为了访问一个用IPC_PRIVATE键创建的现存队列，一定要知道与该队列相结合的标识符，然后在其他IPC调用中使用该标识符。<br/><br/>
 如果希望创建一个新的IPC结构， 而且要确保不是引用具有同一标识符的一个现行IPC结构，那么必须在flag中同时制定IPC_CREAT和IPC_EXCL位。这样做了以后 如果IPC结构已存在就会造成出错， 返回EEXIST（这与制定O_CREAT和O_EXCL标志的open相类似）</td></tr>
 *</table>
 *\retval 0 成功
 *\retval !0 失败
 */
int test_xsi_ipc()
{
	return 0;
}

static void show_help()
{
	printf("pipe, 测试pipe管道\n\n"
			"popen, 测试popen管道\n\n"
			"echo, 测试提示回显功能\n\n"
			"ipc_add1, 测试协同进程\n\n"
			"ipc_add2, 测试协同进程\n\n"
			"fifo_write path, 测试命令管道 写\n\n"
			"fifo_read path, 测试命令管道 读\n\n");
}

int main(int argc, char **argv)
{
	if (argc == 2 and strcmp(argv[1], "pipe") == 0){
		
		test_pipe();
	}else if (argc == 2 and strcmp(argv[1], "popen") == 0){
		
		test_popen();
	}else if (argc == 2 and strcmp(argv[1], "echo") == 0){
		
		test_echo();
	}else if (argc == 2 and strcmp(argv[1], "test_echo") == 0){
		
		test_echo2();
	}else if (argc == 2 and strcmp(argv[1], "ipc_add1") == 0){
		
		test_ipc_add();
	}else if (argc == 2 and strcmp(argv[1], "ipc_add2") == 0){
		
		add2();
	}else if (argc == 3 and strcmp(argv[1], "fifo_write") == 0){
		
		test_write_fifo(argv[2]);
	}else if (argc == 3 and strcmp(argv[1], "fifo_read") == 0){
		
		test_read_fifo(argv[2]);	
	}else{
		
		show_help();
	}
	
	return 0;
}

