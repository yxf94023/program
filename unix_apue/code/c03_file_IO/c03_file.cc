/**
 *\file c03_file.cc
 *\brief 练习文件相关操作
 *\date 2017/07/27
 */
#include <fcntl.h>
#include <unistd.h>

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

/**
 *\brief 测试lseek函数
 *
 *	<code>
 *	off_t lseek(int filedes, off_t offset, int whence);
 *	
 *	返回值： 若成功则返回新的文件偏移量， 若出错则返回-1
 *	</code>
 *
 *	<ul>
 *		<li>每个打开的文件都有一个与其关联的"当前文件偏移量"</li>
 *		<li>通常读，写操作都是从当前文件偏移量处开始，并使偏移量增加所读写的字节数</li>
 *		<li>默认地，第一次打开文件（除非指定O_APPEND选项），该偏移量被设置为0</li>
 *		<li>lseek仅将当前的文件偏移量记录在内核中，它并不会引起任何I/O操作</li>
 *		<li>文件偏移量可设置大于文件的长度，下次写文件会在文件中构成一个空洞，空洞并不要求在磁盘上占用存储区，空洞在读取的时候内容是为0的</li>
 *	</ul>
 *
 *<table>
 *<caption>whence的意义</caption> 
 *<tr><th width="100">标记</th><th width="450">描述</th><th>备注</th>
 *<tr><td>SEEK_SET</td><td>将该文件的偏移量设置为距文件开处offset个字节</td><td></td></tr>
 *<tr><td>SEEK_CUR</td><td>将该文件的偏移量设置为其当前值加offset个字节</td><td>offset可为正或负</td></tr>
 *<tr><td>SEEK_END</td><td>将该文件的偏移量设置为文件长度加offset个字节</td><td>offset可为正或负</td></tr>
 *</table>
 *\warning 若lseek成功执行， 则返回新的文件偏移量， 为此可以用下列方式确定打开的当前偏移量:
 *\warning	<code>
 *\warning	off_t currpos = lseek(fd, 0, SEEK_CUR);
 *\warning	</code>
 *\warning	这种方法也可用来确定文件是否可以设置偏移量。若文件描述符引用的是一个管理、FIFO或网络套接字， 则lseek返回-1，并将errno设置为ESPIPE
 *\warning	不要测试lseek返回值是否小于0， 而要测试它是否等于-1
 *\param[in] filedes 文件描述符
 *\retval 0 成功
 *\retval !0 失败  
 */
int test_lseek(int filedes)
{
	return 0;
}

/**
 *\brief 测试read函数
 *
 *	<code>
 *	ssize_t read(int filedes, void *buf, size_t nbytes)
 *
 *	返回值： 若成功则返回读到的字节数， 若已到文件末尾则返回0， 若出错返回-1
 *	</code>
 *
 *	下面列举的情况下，实际读到的字节数少于要求读的字节数：
 *	<ul>
 *	<li>读普通文件， 在读到要求字节数之前已到达了文件末尾</li>
 *	<li>读终端设备， 通常一次最多读一行</li>
 *	<li>读网络数据， 网络中的缓冲机制会导致返回的字节少于要求读的</li>
 *	<li>读管道或FIFO， 若管道包含的字节少于所需的数量，那么，read只返回实际可用的字节数</li>
 *	<li>信号造成中断， 而已经读了部分数据时</li>
 *	</ul>
 *\param[in] filedes 文件描述符
 *\param[in] buf 将接受读取内容的缓存
 *\param[in] nbytes 缓存可以接受的缓存最大字节
 *\retval 0 成功
 *\retval !0 失败  
 */
int test_read(int filedes, void *buf, size_t nbytes)
{
	return 0;
}

/**
 *\brief 测试write函数
 *
 *	<code>
 *	ssize_t write(int filedes, const void *buf, size_t nbytes)
 *
 *	返回值：若成功则返回已写的字节数， 若出错则返回-1
 *	</code>
 *\warning 通常与参数nbytes的值相同，否则表示出错
 *\warning 磁盘已写满 或 超过了给定进程的文件长度限制 是write常见的出错原因
 *\param[in] filedes 文件描述符
 *\param[in] buf 将写文件的内容
 *\param[in] nbytes 将写文件的长度
 *\retval 0 成功
 *\retval !0 失败 
 */
int test_write(int filedes, const void *buf, size_t nbytes)
{
	return 0;
}


/**
 *\brief 测试dup/dup2函数
 *
 *<table>
 *<caption>不同进程间共享打开的文件【内核数据结构】</caption>
 *<tr><th width="100">分类</th><th width="400">描述</th><th>备注</th></tr>
 *<tr><td>进程表</td><td>每个进程在进程表中都有一个记录项，记录项中包含有一张打开的文件描述符表<br/>每个文件描述符相关联的有：<ul><li>文件描述符标志（close_on_exec）</li><li>指向一个文件表项的指针</li></ul></td><td></td></tr>
 *<tr><td>文件表</td><td>内核为所有打开的文件维持一张文件表<br>每个文件表项包含：<ul><li>文件状态标志（读、写、添加、同步和非阻塞等）</li><li>当前文件偏移量</li><li>指向该文件v节点表项指针</li></ul></td><td></td></tr>
 *<tr><td>v-node节点</td><td rowspan="2">每个打开的文件（或设备）都有一个v节点（v-node）结构<br/>v节点包含：<ul><li>文件类型</li><li>该文件进行各种操作的函数指针</li><li>大多数文件，v节点还包含了该文件的i节点（索引节点）</li></ul></td><td rowspan="2">这些信息是在打开文件时从磁盘上读入内存的，所以所有关于文件的信息都是快速可供使用的<br/>Linux没有使用v节点，而是使用了通用i节点结构。虽然两种实现有所不同，但在概念上，v节点和i节点是一样的。两者都是执行文件系统特有的i节点结构</td></tr>
 *<tr><td>i-node节点</td></tr> 
 *</table>
 *
 *分解两个进程共享过程<br/>
 *若两个独立进程各自打开同一文件，打开该文件的每个进程都会得到一个文件表项，但对于一个给定的文件只有一个v节点表项（进程之间是共享的）<br/>
 *每个进程拥有队里的文件表项，是使得每个进程都有它自己的对该文件的当前偏移量<br/>
 *<br/>
 *\warning 存在多个文件描述符项指向同一个文件表项，例如dup操作， fork后父、子进程对于每一个打开文件描述符共享同一个文件表项
 *
 *<code>
 *	int dup(int filedes);
 *	int dup2(int filedes, int filedes2);
 *
 *	返回值：若成功则返回新的文件描述符，若出错则返回-1<br/>
 *</code>
 *
 *\warning dup返回的新文件描述符一定是当前可用文件描述符中的最小数值；
 *\warning 用dup2则可以用filedes2参数指定新描述符的数值；
 *\warning 若filedes2已经打开，则先将其关闭。若filedes等于filedes2，则返回filedes2，而不关闭它；
 *\warning dup和dup2返回的新文件描述符与参数filedes共享同一个文件表项。
 *
 *\param[in] filedes 文件描述符
 *\retval 0 成功
 *\retval !0 失败 
 */
int test_dup(int filedes)
{
	return 0;
}

int main(int argc, char **argv)
{
	return 0;
}