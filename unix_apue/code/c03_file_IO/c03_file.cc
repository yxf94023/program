/**
 *\file c03_file.cc
 *\brief 练习文件相关操作
 *\date 2017/07/27
 */
#include <fcntl.h>

/**
 *\brief 测试open函数
 *
 *	<code>
 *  int open(const char *pathname, int flags);	
 *  int open(const char *pathname, int flags, mode_t mode);	
 *	
 *	返回值：成功返回文件描述符，出错返回-1
 *	</code>
 *	
 *
 *\warning open, 仅当创建新文件时才使用第三个参数
 *\warning open, 返回的文件描述符一定是最小的未用描述符数值
 *
 *<table>
 *<caption>标记为意义</caption>
 *<tr><th width="100">标记</th><th width="450">描述</th><th>备注</th>
 *<tr><td>O_RDONLY</td><td>只读打开</td><td rowspan="3">三个常量中必须指定一个且只能指定一个</td></tr>
 *<tr><td>O_WRONLY</td><td>只写打开</td></tr>
 *<tr><td>O_RDWR</td><td>读、写打开</td></tr>
 *<tr><td colspan="3">下面列出的常量则是可选择的</td></tr>
 *<tr><td>O_APPEND</td><td>每次写文件都以追加的方式写到文件末尾</td><td></td></tr>
 *<tr><td>O_CREAT</td><td><ul><li>打开的文件不存在，则创建它</li><li><b>打开的文件存在，会不会修改文件的权限?</b></li></ul></td><td>使用该选项是，需要指定第三个参数，用来指定新文件的访问权限位</td></tr>
 *<tr><td>O_EXCL</td><td><ul><li>可以测试一个文件是否存在</li><li>若文件不存在，则创建此文件，这使测试和创建两者称为一个原子操作</li></ul></td><td>若同时指定了O_CREAT， 而文件已经存在， 则会出错。<br/><b>新创建的文件权限是怎样的？</b></td></tr>
 *<tr><td>O_TRUNC</td><td>若文件存在，而且为只写或读写成功打开，则将其长度截短为0</td><td><b>打开后立马关闭会截短么？</b></td></tr>
 *<tr><td>O_NOCTTY</td><td>若pathname指的是终端设备，则不将改设备分配作为此进程的控制终端</td><td><b>这个完全不明其意，懂了后再回来备注？</b></td></tr>
 *<tr><td>O_NONBLOCK</td><td>若pathname指的是一个FIFO、块特殊文件或字符特殊文件，则此选项为文件的本次打开操作和后续的I/O操作设置非阻塞模式</td><td></td></tr>
 *<tr><td>O_DSYNC</td><td>使每次write等待物理I/O操作完成，但是，若写操作并不影响读取刚写入的数据，则不等待文件属性被更新</td><td></td></tr>
 *<tr><td>O_RSYNC</td><td>使每个以文件描述符作为参数的read操作等待，直至任何对文件同一部分进行的未决写操作完成</td><td></td></tr>
 *<tr><td>O_SYNC</td><td>使每次write都等到物理I/O操作完成，包括由write操作引起的文件属性更新所需的I/O</td><td></td></tr>
 *</table>
 *\param[in] pathname 文件路径
 *\param[in] flags	标记参照
 *\retval 0 成功
 *\retval !0 失败
 */
int test_open(const char *pathname, int flags)
{
	return 0;
}

/**
 *\brief 测试creat函数
 *
 *	<code>
 *  int creat(const char *pathname, mode_t mode);	
 *	
 *	返回值：成功返回为只写打开的文件描述符，出错返回-1
 *	</code>
 *	<ul>
 *	 	<li><b>过去:</b> 早期的unix系统，open无法打开一个尚未存在的文件，所以需要creat系统调用创建新文件
 *		<li><b>现在:</b> 如今open函数支持O_CREAT和O_TRUNC选项，于是我们就可以抛弃creat了
 *		<li><b>等价:</b> creat等价于 open(pathname, O_WRONLY | O_CREAT | O_TRUNC, mode)
 *	</ul>
 *\warning creat只能以写方式打开创建的文件
 *\param[in] pathname 文件路径
 *\param[in] mode	文件创建模式
 *\retval 0 成功
 *\retval !0 失败 
 */
int test_creat(const char *pathname, mode_t mode)
{
	return 0;
}

/**
 *\brief 测试close函数
 *
 *	<code>
 *  int close(int filedes);	
 *	
 *	返回值：成功返回0，出错返回-1
 *	</code>
 *	<ul>
 *	 	<li> 关闭一个文件时还会释放改进程加载改文件上的所有<a href="http://www.cnblogs.com/xuyh/p/3278881.html"><b>记录锁?</b></a>
 *		<li> 当一个进程终止时，内核自动关闭它所有打开的文件
 *	</ul>
 *
 *\param[in] filedes 文件描述符
 *\retval 0 成功
 *\retval !0 失败  
 */
int test_close(int filedes)
{
	return 0;
}

int main(int argc, char **argv)
{
	return 0;
}