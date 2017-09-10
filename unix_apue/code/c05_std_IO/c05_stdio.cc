/**
 *\file c05_stdio.cc
 *\brief 练习标准I/O库
 *\date 2017/08/27
 */

#include <stdio.h>
#include <wchar.h>

/**
 *\brief 测试setbuf函数
 *
 * void setbuf(FILE *restrict fp, char *restrict buf);<br/>
 * void setvbuf(FILE *restrict fp, char *restrict buf, int mode, size_t size);<br/>
 *	返回值： 若成功则返回0， 若出错则返回非0值<br/>
 *
 *对任何一个给定的流，如果我们并不喜欢这些系统默认的情况，则可调用上面2个函数进行缓冲类型修改<br/>
 *可以使用setbuf函数打开或关闭缓冲机制，为了带缓冲进行I/O，参数buf必须指向一个长度为BUFSIZ的缓冲区，通常在此之后该流就是全缓冲的，但是如果该流与一个终端设备相关，那么某些系统也可将其设置为行缓冲。为了关闭缓冲，将buf设置为NULL<br/>
 *使用setvbuf可以精确地指定所需的缓冲类型，这是用mode参数实现的， _IOFBF(全缓冲)，_IOLBF(行缓冲)，_IONBF(不带缓冲)。 如果指定一个不带缓冲的流， 则忽略buf和size参数。 如果指定全缓冲或行缓冲，则buf和size可选择地指定一个缓冲区及其长度。如果该流是带缓冲的，而buf是NULL，则标准I/O库将自动地为该流分配适当长度的缓冲区。适当长度指的是由常亮BUFSIZ所指定的值<br/>
 *<table>
 *<caption>setbuf和setvbuf函数</caption>
 *<tr><th>函数</th><th>mode</th><th>buf</th><th>缓冲区及长度</th><th>缓冲类型</th></tr>
 *<tr><td rowspan="2">setbuf</td><td rowspan="2"></td><td>非空</td><td>长度为BUFSIZ的用户buf</td><td>全缓冲或行缓冲</td></tr>
 *<tr><td>NULL</td><td>（五缓冲区）</td><td>不带缓冲</td></tr>
 *<tr><td rowspan="5">setbuf</td><td rowspan="2">_IOFBF</td><td>非空</td><td>长度为size的用户buf</td><td rowspan="2">全缓冲</td></tr>
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
 *\retval !		失败
 */
int test_setbuf(int type)
{
	return 0;
}

/**
 *\brief 测试fflush函数
 *
 *int fflush(FILE *fp);<br/>
 *返回值：若成功则返回0， 若出错则返回EOF<br/>
 *<br/>
 *任何时候都可以强制冲洗一个流，函数使该流所有未写的数据都被传送到内核，作为一个特例，如若fp是NULL，则此函数将导致所有输出流被冲洗<br/>
 *\param[in] fp 	文件指针
 *\retval	0	成功
 *\retval 	!0	失败
 */
int test_fflush(FILE *fp)
{
	return 0;
}