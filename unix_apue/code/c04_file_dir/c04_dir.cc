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
 *<tr><th width="40">序号</th><th width="500">成员</th><th>描述</th></tr>
 *<tr><td>1</td><td>类型：mode_t --->> 名称：<b>st_mode</b></td><td>文件类型 和 模式权限</td></tr>
 *<tr><td>2</td><td>类型：ino_t --->> 名称：<b>st_ino</b></td><td>i-node号（serial number）</td></tr>
 *<tr><td>3</td><td>类型：dev_t --->> 名称：<b>st_dev</b></td><td>device number(file system)</td></tr>
 *<tr><td>4</td><td>类型：dev_t --->> 名称：<b>st_rdev</b></td><td>device number of special files</td></tr>
 *<tr><td>5</td><td>类型：nlink_t --->> 名称：<b>st_nlink</b></td><td></td></tr>
 *</table> 
 *\retval 0 成功
 *\retval !0 失败 
 */
int test_stat()
{
	return 0;
}
