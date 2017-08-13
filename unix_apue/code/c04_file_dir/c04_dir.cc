/**
 *\file c03_file.cc
 *\brief 练习文件和目录操作
 *\date 2017/08/12
 */
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

/**
 *\brief 测试stat函数
 *
 *<table>
 *<caption>stat函数三种形态</caption>
 *<tr><th width="40">序号</th><th width="500">原型</th><th>描述</th></tr>
 *<tr><td>1</td><td>int stat(const char *restrict pathname, struct stat *restrict buf);</td><td><ul><li>成功返回0， 出错返回-1</li></ul></td></tr>
 *<tr><td>2</td><td>int fstat(int filedes, struct stat *buf);</td><td><ul><li>成功返回0， 出错返回-1</li></ul></td></tr>
 *<tr><td>3</td><td>int lstat(const char *restrict pathname, struct stat *restrict buf);</td><td><ul><li>成功返回0， 出错返回-1</li><li>类似stat，但当文件是符号链接时，返回该符号链接的信息，而不是由该符号链接引用文件的信息</li></ul></td></tr>
 *</table>
 *<table>
 *<caption>struct stat结构体分析</caption>
 *<tr><th width="40">序号</th><th width="300">成员</th><th>描述</th></tr>
 *<tr><td>1</td><td>类型：mode_t --->> 名称：<b>st_mode</b></td><td>文件类型 和 模式权限
 <table>
 <caption>文件类型</caption>
 <tr><th width="40">No.</th><th>类型</th><th>概述</th></tr>
 <tr><td>1</td><td><b>普通文件</b>(regular file)</td><td><ul><li>常用文件类型</li><li>内核不区分文件的数据形式(文本或二进制)</li><li>文件数据内容的解释由处理该文件的应用程序负责</li><li>测试宏，S_ISREG()</li></ul></td></tr>
 <tr><td>2</td><td><b>目录文件</b>(directory file)</td><td><ul><li>包含目录下的文件名字及文件信息有关指针</li><li>对目录有读权限的进程都可读目录，但只有内核可以直接写目录文件</li><li>进程必须使用特定的函数才能更改目录</li><li>测试宏，S_ISDIR()</li></ul></td></tr>
 <tr><td>3</td><td><b>块特殊文件</b>(block special file)</td><td><ul><li>提供对设备带缓冲的访问</li><li>每次访问以固定长度为单位</li><li>测试宏，S_ISBLK()</li></ul></td></tr>
 <tr><td>4</td><td><b>字符特殊文件</b>(character special file)</td><td><ul><li>提供对设备不带缓冲区的访问</li><li>每次访问长度可变</li><li>系统中设备要么是字符特殊文件，要么是块特殊文件</li><li>测试宏，S_ISCHR()</li></ul></td></tr> 
 <tr><td>5</td><td><b>命名管道</b>(FIFO)</td><td><ul><li>用于进程间通信</li><li>测试宏，S_ISFIFO()</li></ul></td></tr>  
 <tr><td>6</td><td><b>套接字</b>(socket)</td><td><ul><li>用于进程间的网络通信</li><li>一台宿主机上进程间的非网络通信</li><li>测试宏，S_ISSOCK()</li></ul></td></tr> 
 <tr><td>7</td><td><b>符号链接</b>(symbolic link)</td><td><ul><li>指向另一个文件</li><li>测试宏，S_ISLNK()</li></ul></td></tr>  
 </table></td></tr> 
 *<tr><td>2</td><td>类型：ino_t --->> 名称：<b>st_ino</b></td><td>i-node号（serial number）</td></tr>
 *<tr><td>3</td><td>类型：dev_t --->> 名称：<b>st_dev</b></td><td>device number(file system)</td></tr>
 *<tr><td>4</td><td>类型：dev_t --->> 名称：<b>st_rdev</b></td><td>device number of special files</td></tr>
 *<tr><td>5</td><td>类型：nlink_t --->> 名称：<b>st_nlink</b></td><td>number of links</td></tr>
 *<tr><td>6</td><td>类型：uid_t --->> 名称：<b>st_uid</b></td><td>user ID of owner </td></tr>
 *<tr><td>7</td><td>类型：gid_t --->> 名称：<b>st_gid</b></td><td>group ID of owner </td></tr>
 *<tr><td>8</td><td>类型：off_t --->> 名称：<b>st_size</b></td><td>size in bytes, for regular files </td></tr>
 *<tr><td>9</td><td>类型：time_t --->> 名称：<b>st_actime</b></td><td>time of last access </td></tr>
 *<tr><td>10</td><td>类型：time_t --->> 名称：<b>st_mtime</b></td><td>time of modification </td></tr>
 *<tr><td>11</td><td>类型：time_t --->> 名称：<b>st_ctime</b></td><td>time of last file status change</td></tr>
 *<tr><td>12</td><td>类型：blksize_t --->> 名称：<b>st_blksize</b></td><td>best I/O block size </td></tr> 
 *<tr><td>13</td><td>类型：blkcnt_t --->> 名称：<b>st_blocks</b></td><td>number of disk blocks allocated</td></tr>  
 *</table> 
 *\retval 0 成功
 *\retval !0 失败 
 */
int test_stat()
{
	return 0;
}
