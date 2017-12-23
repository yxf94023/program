/**
 *\file c05_stdio.cc
 *\brief 练习标准I/O库
 *\date 2017/08/27
 */

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/**
 *\brief 测试setbuf函数
 *
 *<code>
 *void setbuf(FILE *restrict fp, char *restrict buf);
 *
 *void setvbuf(FILE *restrict fp, char *restrict buf, int mode, size_t size);
 *
 *返回值： 若成功则返回0， 若出错则返回非0值
 *</code>
 *
 *对任何一个给定的流，如果我们并不喜欢这些系统默认的情况，则可调用上面2个函数进行缓冲类型修改<br/>
 *可以使用setbuf函数打开或关闭缓冲机制，为了带缓冲进行I/O，参数buf必须指向一个长度为BUFSIZ的缓冲区，通常在此之后该流就是全缓冲的，但是如果该流与一个终端设备相关，那么某些系统也可将其设置为行缓冲。为了关闭缓冲，将buf设置为NULL<br/>
 *使用setvbuf可以精确地指定所需的缓冲类型，这是用mode参数实现的， _IOFBF(全缓冲)，_IOLBF(行缓冲)，_IONBF(不带缓冲)。 如果指定一个不带缓冲的流， 则忽略buf和size参数。 如果指定全缓冲或行缓冲，则buf和size可选择地指定一个缓冲区及其长度。如果该流是带缓冲的，而buf是NULL，则标准I/O库将自动地为该流分配适当长度的缓冲区。适当长度指的是由常亮BUFSIZ所指定的值<br/>
 *<table>
 *<caption>setbuf和setvbuf函数</caption>
 *<tr><th>函数</th><th>mode</th><th>buf</th><th>缓冲区及长度</th><th>缓冲类型</th></tr>
 *<tr><td rowspan="2">setbuf</td><td rowspan="2"></td><td>非空</td><td>长度为BUFSIZ的用户buf</td><td>全缓冲或行缓冲</td></tr>
 *<tr><td>NULL</td><td>（无缓冲区）</td><td>不带缓冲</td></tr>
 *<tr><td rowspan="5">setvbuf</td><td rowspan="2">_IOFBF</td><td>非空</td><td>长度为size的用户buf</td><td rowspan="2">全缓冲</td></tr>
 *<tr><td>NULL</td><td>合适长度的系统缓冲区</td></tr>
 *<tr><td rowspan="2">_IOLBF</td><td>非空</td><td>长度为size的用户buf</td><td rowspan="2">行缓冲</td></tr>
 *<tr><td>NULL</td><td>合适长度的系统缓冲区</td></tr>
 *<tr><td>_IONBF</td><td>（忽略）</td><td>（无缓冲区）</td><td>不带缓冲区</td></tr>
 *</table>
 *
 * <b>标准输入、标准输出和标准出错</b>
 *	<ol>
 *		<li>一个进程预定义有三个流，且三个流可以自动地被进程使用</li>
 *		<li>三个流对应的文件描述符分别是， 标准输入(STDIN_FILENO)、标准输出(STDOUT_FILENO)、标准出错(STDERR_FILENO)</li>
 *	</ol>
 * 
 * <b>缓冲</b>
 *	<ol>
 *		<li> 标准I/O库提供缓冲的目的是尽可能减少使用read和write调用的次数</li>
 *		<li>它也对每个I/O流自动地进行缓冲管理，避免应用程序需要考虑这点所带来的麻烦</li>
 *	</ol>
 *<br/>
 *<table>
 *<caption>标准I/O缓存类型</caption>
 *<tr><th width="40">序号</th><th width="80">缓冲类型</th><th>描述</th></tr>
 *<tr><td>1</td><td>全缓冲区</td><td>在填满标准I/O缓冲区后才进行实际I/O操作，对于驻留在磁盘上的文件通常是由标准I/O库实施全缓冲的，在一个流上执行第一次I/O操作时，相关标准I/O函数通常调用malloc获得需要使用的缓冲区。</td></tr>
 *<tr><td>2</td><td>行缓冲</td><td>当在输入和输出中遇到换行符时，标准I/O库执行I/O操作，这允许我们一次输出一个字符（用标准I/O fputc函数），但只有写了一行之后才进行实际I/O操作，当流涉及一个终端时，通常使用行缓冲。<br/>行缓冲的两个限制<br/>1.因为标准I/O库用来收集每一行的缓冲区的长度是固定的，所以只要填满了缓冲区，那么即使还没有写一个换行符，也进行I/O操作<br/>2.任何时候只要通过标准I/O库要求从一个不带缓冲的流，或者一个行缓冲的流得到输入数据，那么就会造成冲洗所有行缓冲输出流。</td></tr>
 *<tr><td>3</td><td>不带缓冲</td><td>标准I/O库不对字符进行缓冲存。标准出错流stderr通常是不带缓冲的，这就使得出错信息可以尽快显示出来，而不管它们是否含有一个换行符。</td></tr>
 *</table>
 *<br/>
 *IOS C要求下列缓冲特点<br/>
 *1.当且仅当标准输入和标准输出并不涉及交互式设备时，它们才是全缓冲的<br/>
 *2.标准出错绝不会是全缓冲的<br/>
 *3.通常，标准出错是不带缓冲的<br/>
 *4.通常，如若是涉及终端设备的其他流，则它们是航缓冲的；否则是全缓冲的<br/>
 *\param[in] type  缓冲区类型
 *
 *\retval 0 	成功
 *\retval !0	失败
 */
int test_setbuf(int type)
{
	int i = 0;
	int size = 10;
	int ret = 0;
	const int BUF_SIZE = 1024;
	char buf[BUF_SIZE];
	
	ret = setvbuf(stdin, buf, type, sizeof(buf) -1);
	if (ret){
		
		printf("%s:%d setvbuf failed(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	for (i = 0; i < size; ++i){
		
		printf("%d ", i);
	}
	sleep(5);
	
	return 0;
}

static int convert_iobuf_type(const char *szbuftype)
{
	int size = 0, i = 0;
	int buf_mode = _IOFBF;
	
	struct mode_pair{
		
		const char *str_buf;
		int	buf_mode;
	}mode_pairs[] = {
		
		{"_IOFBF", _IOFBF}, {"_IOLBF", _IOLBF}, {"_IONBF", _IONBF}
	};
	
	size = sizeof(mode_pairs)/sizeof(mode_pairs[0]);
	
	for (i = 0; i < size; ++i){
		
		if (strstr(szbuftype, mode_pairs[i].str_buf)){
			
			buf_mode = mode_pairs[i].buf_mode;
			break;
		}
	}
	
	printf("%s:%d size[%d], buf_mode[%d]\n", __FILE__, __LINE__, size, buf_mode);
	return buf_mode;
}

/**
 *\brief 测试fflush函数
 *
 *<code>
 *int fflush(FILE *fp);
 *
 *返回值：若成功则返回0， 若出错则返回EOF
 *</code>
 *
 *任何时候都可以强制冲洗一个流，函数使该流所有未写的数据都被传送到内核，作为一个特例，如若fp是NULL，则此函数将导致所有输出流被冲洗<br/>
 *\param[in] fp 	文件指针
 *\retval	0	成功
 *\retval 	!0	失败
 */
int test_fflush(FILE *fp)
{
	return 0;
}

/**
 *\brief 测试gets函数
 *
 *<code>
 *char *fgets(char *restrict buf, int n, FILE *restrict fp);
 *
 *char *gets(char *buf);
 *
 *两个函数返回值：若成功则返回buf，若已到达文件结尾出错则返回NULL
 *</code>
 *
 *
 *1.两个函数都是每次输入一行
 *2.gets从标准输入读，fgets则从指定的流读
 *3.fgets需指定缓冲区的长度n，函数一直读到下一个换行符为止，但是不超过n-1个字符，读入的字符被送入缓冲区，缓冲区总是以null字符结尾
 *4.gets是一个不推荐使用的函数，函数不能指定缓冲区的长度，可能造成缓冲区溢出
 *\retval 0 	成功
 *\retval !0	失败
 */
int test_gets()
{
	return 0;
}

/**
 *\brief 测试puts函数
 *
 *<code>
 *int fputs(const char *restrict str, FILE *restrict fp);
 *
 *int puts(const char *str);
 *
 *返回值：若成功则返回非负值， 若出错则返回EOF
 *</code>
 *
 *1.函数fputs将一个以null符种植的字符串写到指定流，尾端的终止符null不写出，函数并不一定是每次输出一行，因为它并不要求在null字符前一定是换行符。
 *2.puts将一个null符终止的字符串写到标准输出， 终止符不写出，但是， puts然后又将一个换行符写到标准输出
 *
 *\retval 0 	成功
 *\retval !0	失败
 */
int test_puts()
{
	return 0;
}

/**
 *\brief 测试fread/fwrite函数
 *
 *<code>
 *size_t fread(void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
 *
 *size_t fwrite(const void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
 *
 *两个函数的返回值：读或写的对象数
 *</code>
 *
 *使用场景
 *<ol>
 *<li>读或写一个二进制数组</li>
 *<li>读写一个结构体</li>
 *</ol>
 *注意事项
 *<ol>
 *<li>同一结构体，同一成员的的偏移量可能因编译器和系统而异</li>
 *<li>用来存储多字节整数和浮点值得二进制格式在不同的机器体系结构间可能不同</li>
 *</ol>
 *
 *\retval 0 	成功
 *\retval !0	失败 
 */
int test_fread_fwrite()
{
	return 0;
}

/**
 *\brief 测试ftell函数
 *
 *<code>
 *long ftell(FILE *fp);
 *
 *返回值：成功返回当前文件位置指示， 若出错则返回-1L
 *</code>
 *
 *\retval 0 	成功
 *\retval !0	失败
 */
int test_ftell()
{
	return 0;
}

/**
 *\brief 测试fseek函数
 *
 *<code>
 *int fseek(FILE *fp, long offset, int whence);
 *
 *返回值：若成功则返回0， 若出错则返回非0值
 *</code>
 *
 *whence的取值
 *1.SEEK_SET 表示从文件的起始位置开始<br/>
 *2.SEEK_CUR 表示从当前文件位置开始<br/>
 *3.SEEK_END 表示从文件的尾端开始<br/>
 *
 *\retval 0 	成功
 *\retval !0	失败 
 */
int test_fseek()
{
	return 0;
}

/**
 *\brief 测试fgetpos函数
 *
 *<code>
 *int fgetpos(FILE *restrict fp, fpos_t *restrict pos);
 *
 *itn fsetpos(FILE *fp, const fpos_t *pos);
 *
 *返回值：若成功则返回0， 若出错则返回非0值
 *</code>
 *
 *fgetspos将文件位置指示器的当前值存入由pos指向的对象中，在以后调用fsetpos时，可以使用此值将流重新定位到该位置。
 *
 *\retval 0 	成功
 *\retval !0	失败 
 */
int test_fgetpos()
{
	return 0;
}

/**
 *\brief 测试tmpfile/tmpname函数
 *
 *<code>
 *char *tmpnam(char *ptr);
 *
 *char *tempnam(const char *directory, const char *prefix);
 *
 *返回值: 指向唯一路径名的指针
 *
 *FILE *tmpfile(void);
 *
 *返回值: 若成功则返回文件指针， 若出错则返回NULL
 *
 *int mkstemp(char *tmplate);
 *
 *返回值：若成功则返回文件描述符， 若出错则返回-1
 *</code>
 *
 *<ol>
 *<li>tmpname产生一个与现有文件名不同的一个有效路径名字符串， 每次调用它时，都会产生一个不同的路径名，最多调用次数是TMP_MAX</li>
 *<li>tmpname，若ptr是NULL，则所产生的路径名存放在一个静态区中，指向该静态区的指针作为函数值返回，下次再调用tmpname时，会重写该静态区， 若ptr不是NULL，则认为它指向长度至少是L_tmpnam个字符的数组，所产生的路径名存放在该数组中，ptr也作为函数值返回</li>
 *<li>tmpfile，创建一个临时二进制文件（类型wb+），在关闭该文件或进程结束的时将自动删除这种文件。</li>
 *<li>tempnam是tmpnam的一个变体，它允许调用者为所产生的路径名指定目录和前缀，对于目录有4种不同的选择，按照下列顺序判断其条件是否为真， 并且使用第一个为真的作为目录：
 <ul>
 <li>如果定义了环境变量TMPDIR，则用其作为目录</li>
 <li>如果参数directory非NULL，则用其作为目录</li>
 <li>将<stdio.h>中的字符串P_tmpdir作为目录</li>
 <li>将本地目录（通常是/tmp）作为目录</li>
 </ul>
 </li>
 *<li>tempnam，如果prefix非NULL， 则它应该是最多包含5个字符的字符串， 用其作为文件名的头几个字符，该函数调用malloc函数分配动态存储区， 用其存放所构造的路径名，当不再使用此路径时就可释放此存储区</li>
 *<li>mkstemp，返回的文件描述符可用于读、写该文件，临时文件的名字是用template字符串参数选择的，该字符串是一个路径名， 其最后6个字符将会被不同的字符代换，以创建唯一路径名，若函数成功返回，它会修改template字符串以反映临时文件的名字</li>
 *<li>与tempfile不同的是，mkstemp创建的临时文件不会自动被删除，如若想从文件系统名字空间中删该除文件，则我们需要自行unlink它</li>
 *<li>tmpnam和tempnam的一个不足之处是，在返回唯一路径名和应用程序用该路径名创建文件之间有一个时间窗口。在该时间窗口期间，另一个进程可能创建一个同名文件。tempfile和mkstemp函数则不会产生此种问题， 可以使用它们代替tmpnam和tempnam</li>
 *</ol>
 *\retval 0 	成功
 *\retval !0	失败 
 */
int test_tmpfile_tmpname()
{
	return 0;
}

static void show_help()
{
	printf("setbuf type, 设置输出缓冲区的类型\n"
			"\ttype  _IOFBF|_IOLBF|_IONBF  \n\n");
}

int main(int argc, char **argv)
{
	if (argc == 3 && strcmp(argv[1], "setbuf") == 0){
		
		test_setbuf(convert_iobuf_type(argv[2]));
	}else{
		
		show_help();
	}
	return 0;
}


