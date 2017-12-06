/**
 *\file c04_dir.cc
 *\brief 练习文件和目录操作
 *\date 2017/08/12
 */
#include <fcntl.h>
#include <unistd.h>
#include <utime.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


static void show_stat_info(struct stat *pstat)
{
	assert(pstat != NULL);
	
	printf("mode_t -->>[%d]\n"
		"ino_t -->>[%ld]\n"
		"dev_t -->>[%ld]\n"
		"dev_t -->>[%ld]\n"
		"nlink_t -->>[%ld]\n"
		"uid_t -->>[%d]\n"
		"git_t -->>[%d]\n"
		"st_size -->>[%ld]\n"
		"st_atime -->>[%lu]\n"
		"st_mtime -->>[%lu]\n"
		"st_ctime -->>[%lu]\n"
		"st_blksize -->>[%ld]\n"
		"st_blocks -->>[%ld]\n\n", 
		pstat->st_mode, 
		pstat->st_ino,
		pstat->st_dev,
		pstat->st_rdev,
		pstat->st_nlink,
		pstat->st_uid,
		pstat->st_gid,
		pstat->st_size,
		pstat->st_atime,
		pstat->st_mtime,
		pstat->st_ctime,
		pstat->st_blksize,
		pstat->st_blocks);
		
	if (S_ISREG(pstat->st_mode)){
		
		printf("regular\n");
	}else if (S_ISDIR(pstat->st_mode)){
		
		printf("directory\n");
	}else if (S_ISCHR(pstat->st_mode)){
		
		printf("charactor special\n");
	}else if (S_ISBLK(pstat->st_mode)){
		
		printf("block special\n");
	}else if (S_ISFIFO(pstat->st_mode)){
		
		printf("fifo\n");
	}else if (S_ISLNK(pstat->st_mode)){
		
		printf("symbolic link\n");
	}else if (S_ISSOCK(pstat->st_mode)){
		
		printf("socket\n");
	}
}

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
 </td></tr> 
 *<tr><td>2</td><td>类型：ino_t --->> 名称：<b>st_ino</b></td><td>i-node号（serial number）</td></tr>
 *<tr><td>3</td><td>类型：dev_t --->> 名称：<b>st_dev</b></td><td>device number(file system)</td></tr>
 *<tr><td>4</td><td>类型：dev_t --->> 名称：<b>st_rdev</b></td><td>device number of special files</td></tr>
 *<tr><td>5</td><td>类型：nlink_t --->> 名称：<b>st_nlink</b></td><td>number of links</td></tr>
 *<tr><td>6</td><td>类型：uid_t --->> 名称：<b>st_uid</b></td><td>user ID of owner </td></tr>
 *<tr><td>7</td><td>类型：gid_t --->> 名称：<b>st_gid</b></td><td>group ID of owner </td></tr>
 *<tr><td>8</td><td>类型：off_t --->> 名称：<b>st_size</b></td><td>size in bytes, for regular files <br/>此字段只对普通文件、目录文件和符号链接有意义<br/>
 <ul><li>普通文件，文件长度可以是0， 在读这种文件时，将得到文件结束（end-of-file）指示</li>
 <li>目录文件，文件长度常是一个数（例如16或512）的倍数</li>
 <li>符号链接文件，文件长度是文件名中的实际字节数</li>
 </ul></td></tr>
 *<tr><td>9</td><td>类型：time_t --->> 名称：<b>st_atime</b></td><td>time of last access<br/><br/>cat 会修改这个访问时间戳， 当然修改类的操作也会修改这个时间<br/> </td></tr>
 *<tr><td>10</td><td>类型：time_t --->> 名称：<b>st_mtime</b></td><td>time of modification </td></tr>
 *<tr><td>11</td><td>类型：time_t --->> 名称：<b>st_ctime</b></td><td>time of last file status change</td></tr>
 *<tr><td>12</td><td>类型：blksize_t --->> 名称：<b>st_blksize</b></td><td>best I/O block size </td></tr> 
 *<tr><td>13</td><td>类型：blkcnt_t --->> 名称：<b>st_blocks</b></td><td>number of disk blocks allocated</td></tr>  
 *</table> 
 <table>
 <caption>文件类型</caption>
 <tr><th width="40">序号</th><th>类型</th><th>概述</th></tr>
 <tr><td>1</td><td><b>普通文件</b>(regular file)</td><td><ul><li>常用文件类型</li><li>内核不区分文件的数据形式(文本或二进制)</li><li>文件数据内容的解释由处理该文件的应用程序负责</li><li>测试宏，S_ISREG()</li></ul></td></tr>
 <tr><td>2</td><td><b>目录文件</b>(directory file)</td><td><ul><li>包含目录下的文件名字及文件信息有关指针</li><li>对目录有读权限的进程都可读目录，但只有内核可以直接写目录文件</li><li>进程必须使用特定的函数才能更改目录</li><li>测试宏，S_ISDIR()</li></ul></td></tr>
 <tr><td>3</td><td><b>块特殊文件</b>(block special file)</td><td><ul><li>提供对设备带缓冲的访问</li><li>每次访问以固定长度为单位</li><li>测试宏，S_ISBLK()</li></ul></td></tr>
 <tr><td>4</td><td><b>字符特殊文件</b>(character special file)</td><td><ul><li>提供对设备不带缓冲区的访问</li><li>每次访问长度可变</li><li>系统中设备要么是字符特殊文件，要么是块特殊文件</li><li>测试宏，S_ISCHR()</li></ul></td></tr> 
 <tr><td>5</td><td><b>命名管道</b>(FIFO)</td><td><ul><li>用于进程间通信</li><li>测试宏，S_ISFIFO()</li></ul></td></tr>  
 <tr><td>6</td><td><b>套接字</b>(socket)</td><td><ul><li>用于进程间的网络通信</li><li>一台宿主机上进程间的非网络通信</li><li>测试宏，S_ISSOCK()</li></ul></td></tr> 
 <tr><td>7</td><td><b>符号链接</b>(symbolic link)</td><td><ul><li>指向另一个文件</li><li>测试宏，S_ISLNK()</li></ul></td></tr>  
 </table>
 *<table>
 *<caption>文件访问权限</caption>
 *<tr><th width="40">序号</th><th width="40">分类</th><th width="100">st_mode屏蔽</th><th>描述</th></tr>
 *<tr><td>1</td><td rowspan="3">用户</td><td>S_IRUSR</td><td>用户-读</td></tr>
 *<tr><td>2</td><td>S_IWUSR</td><td>用户-写</td></tr>
 *<tr><td>3</td><td>S_IXUSR</td><td>用户-执行</td></tr>
 *<tr><td>4</td><td rowspan="3">组</td><td>S_IRGRP</td><td>组-读</td></tr>
 *<tr><td>5</td><td>S_IWGRP</td><td>组-写</td></tr>
 *<tr><td>6</td><td>S_IXGRP</td><td>组-执行</td></tr>
 *<tr><td>7</td><td rowspan="3">其他</td><td>S_IROTH</td><td>其他-读</td></tr>
 *<tr><td>8</td><td>S_IWOTH</td><td>其他-写</td></tr>
 *<tr><td>9</td><td>S_IXOTH</td><td>其他-执行</td></tr> 
 *<tr><td colspan="4"><b>补充说明</b></td></tr>
 *<tr><td colspan="4"><ul><li>我们用名字打开任一类型的文件时，对文件路径上的所有目录都必须具有执行权限，目录执行权限位常被称为搜索位的原因。<br/>目录的读权限和执行权限的差异性<br/>1.读权限允许我们读目录，获得在该目录中所有文件名的列表；<br/>2.当要访问目录下的一个文件，对该目录的执行权限使得可以通过该目录，找到特定的文件名。</li><li>一个文件的读权限决定了我们是否能够打开该文件进行读操作，这与open函数的O_RDONLY和O_RDWR标志相关</li><li>一个文件的写权限决定了我们是否能够打开该文件进行写操作，这与open函数的O_WRONLY和O_RDWR标志相关。</li><li>当在open函数中对一个文件制定O_TRUNC标志，必须对该文件具有写权限</li><li>当想在一个目录中创建一个新文件，必须对该目录具有写权限和执行权限</li><li>当想删除一个文件，必须对包含该文件的目录具有写权限和执行权限，对该文件本身则不需要有读、写权限</li><li>若用6个exec函数中的任何一个执行某个文件，都必须对该文件具有执行权限，该文件还必须是一个普通文件</li></ul>当我们每次打开、创建或删除一个文件时，内核都会进行文件权限的测试，这种测试可能会涉及到2个所有者ID（文件的性质）和2个有效ID（进程的性质），内核测试的过程如下：<ol><li>若进程的有效用户ID为0（超级用户），则允许访问</li><li>若进程的有效用户ID等于文件的所有者ID（也就是该进程拥有此文件），若所有者适当的访问权限位被设置，则允许访问，否则拒绝访问</li><li>若进程的有效组ID或进程的附加组ID之一等于文件的组ID，那么，如果适当的访问权限位被设置，则允许访问，否则拒绝访问</li><li>若其他用户适当的访问权限位被设置，则允许访问，否则拒绝访问</li></ol></td></tr>
 *</table>
 *<table>
 *<caption>与每个进程相关联的用户ID和组ID</caption>
 *<tr><th width="40">序号</th><th width="300">名称</th><th>描述</th></tr>
 *<tr><td>1</td><td>实际用户ID</td><td rowspan="2">我们实际上是谁<br>
 用户登录时取自口令文件中的登录项<br/>登录期间一般不改变，但超级用户进程能改变它们</td></tr>
 *<tr><td>2</td><td>实际组ID</td></tr>
 *<tr><td>3</td><td>有效用户ID</td><td rowspan="3">用于文件访问权限检查，决定了我们的文件访问权限</td></tr>
 *<tr><td>4</td><td>有效组ID</td></tr>
 *<tr><td>5</td><td>附加组ID</td></tr>
 *<tr><td>6</td><td>保存的设置用户ID</td><td rowspan="2">由exec函数保存</td></tr>
 *<tr><td>7</td><td>保存的设置组ID</td></tr>
 *<tr><td colspan="3"><b>补充说明</b></td></tr>
 *<tr><td colspan="3"><ul><li>一般地，有效用户ID等于实际用户ID，有效组ID等于实际组ID</li><li>当执行一个可执行文件时，进程的有效用户ID、有效组ID通常就是对应的实际用户ID和实际组ID。<br/>俗话说万事都有例外，在文件模式字（st_mode）有那么两位分别为设置用户ID（set-user-ID）位和设置组ID（set-groupID ）位。<br/>它们的作用是，当执行此程序文件时，将进程的有效用户ID或有效组ID 设置为文件所有者的用户ID（st_uid）或文件的组所有者ID（st_gid）</li><li>举个栗子，若文件所有者是超级用户，而且设置了该文件的设置用户ID位， 然后当该程序由一个进程执行时，则该进程具有超级用户权限。不管执行此文件的进程的进程的实际用户ID是什么，都进行这种处理。像passwd程序，允许任意用户改变其口令，它就是一个设置用户ID程序。<br/>-rwsr-xr-x. 1 root root 27832 Jun 10  2014 /usr/bin/passwd
</li><li>就像上面描述的一样，因为运行设置用户ID程序的进程通常得到额外的权限，所以编写这种程序时要特别谨慎。</li></ul></td></tr>
 *</table>
 *\param[in] file_name 文件路径
 *\retval 0 成功
 *\retval !0 失败 
 */
int test_stat(const char *file_name)
{
	struct stat st_stat;
	int ret = -1;
	//int fd = -1;
	
	ret = stat(file_name, &st_stat);
	if (ret == -1){
		
		printf("%s:%d (%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	
	show_stat_info(&st_stat);
	printf("\n");
	
	ret = lstat(file_name, &st_stat);
	if (ret == -1){
		
		printf("%s:%d (%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	
	show_stat_info(&st_stat);
	
	return 0;
}

static int convert_modes(const char *str_modes)
{
	int mode = 0;
	int size = 0, idx = 0;
	
	struct st_mode_pair{
		
		const char *str_mode;
		int int_mode;
	}mode_pairs[] = {
		{"R_OK", R_OK},{"W_OK", W_OK},{"X_OK", X_OK},{"F_OK", F_OK}
	};
	
	size = sizeof(mode_pairs)/sizeof(mode_pairs[0]);
	printf("%s:%d size[%d]\n", __FILE__, __LINE__, size);
	for (idx = 0; idx < size; ++idx){
		
		if (strstr(str_modes, mode_pairs[idx].str_mode)){
			
			mode |= mode_pairs[idx].int_mode;
		}
	}
	printf("%s:%d mode[%d]\n", __FILE__, __LINE__, mode);
	
	return mode;
}

/**
 *\brief 测试access函数
 *
 *<code>
 *int access(const char *pathname, int mode);
 *
 *返回值：成功返回0， 失败返回-1
 *
 *除非测试的文件不存在， 一般错误返回的提示都是Permission denied
 *</code>
 *<table>
 *<caption>access函数的mode常量</caption>
 *<tr><th width="40">序号</th><th width="100">模式</th><th>说明</th></tr>
 *<tr><td>1</td><td>R_OK</td><td>测试读权限</td></tr>
 *<tr><td>2</td><td>W_OK</td><td>测试写权限</td></tr>
 *<tr><td>3</td><td>X_OK</td><td>测试执行权限</td></tr>
 *<tr><td>4</td><td>F_OK</td><td>测试文件是否存在</td></tr>  
 *</table>
 *\warning 当用open函数打开一个文件时，内核以进程的有效用户ID和有效组ID为基础执行访问权限测试
 *\warning 有时，进程期望按实际用户ID和实际组ID来测试其访问能力
 *\warning access函数就是按实际用户ID和实际组ID进行访问权限测试的
 *\param[in] file_path 文件路径
 *\param[in] modes 测试文件属性
 *\retval 0 成功
 *\retval !0 失败 
 */
int test_access(const char *pathname, int mode)
{
	int ret = -1;
	
	ret = access(pathname, mode);
	if (ret == -1){
		
		printf("%s:%d errno(%s)\n", __FILE__, __LINE__, strerror(errno));
		return -1;
	}
	if ((mode & F_OK) == F_OK){
		
		printf("文件存在\n");
	}
	
	if ((mode & R_OK) == R_OK){
		
		printf("文件有读权限\n");
	}
	
	if ((mode & W_OK) == W_OK){
		
		printf("文件有写权限\n");
	}
	
	if ((mode & X_OK) == X_OK){
		
		printf("文件有执行权限\n");
	}
	
	return 0;
}

/**
 *\brief 测试umask函数
 *
 *<code>
 *mode_t umask(mode_t cmask);<br/>
 *返回值：以前的文件模式创建屏蔽字，函数没有出错返回
 *</code>
 *<ul>
 *<li>一般进程不设置umask值，它由shell启动文件设置，然后不改变</li>
 *<li>当编写创建新文件的程序时，若想确保指定的访问权限位已经激活，则必须在进程运行时修改umask值<br/>
 *将umask设置为0，确保任何用户都能读文件，否则，当进程运行时，有效的umask值可能关闭该权限位</li>
 *</ul>
 *<table>
 *<caption>umask文件访问权限位</caption>
 *<tr><th width="40">序号</th><th width="40">分类</th><th width="150">屏蔽位(八进制)</th><th>描述</th></tr>
 *<tr><td>1</td><td rowspan="3">用户</td><td>0400</td><td>用户-读</td></tr>
 *<tr><td>2</td><td>0200</td><td>用户-写</td></tr>
 *<tr><td>3</td><td>0100</td><td>用户-执行</td></tr>
 *<tr><td>4</td><td rowspan="3">组</td><td>0040</td><td>组-读</td></tr>
 *<tr><td>5</td><td>0020</td><td>组-写</td></tr>
 *<tr><td>6</td><td>0010</td><td>组-执行</td></tr>
 *<tr><td>7</td><td rowspan="3">其他</td><td>0004</td><td>其他-读</td></tr>
 *<tr><td>8</td><td>0002</td><td>其他-写</td></tr>
 *<tr><td>9</td><td>0001</td><td>其他-执行</td></tr>
 *</table> 
 *\retval 0 成功
 *\retval !0 失败  
 */
int test_umask()
{
	return 0;
}

/**
 *\brief 测试chmod函数
 *
 *<code>
 *int chmod(const char *pathname, mode_t mode);<br/>
 *int fchmod(int filedes, mode_t mode);<br/>
 *返回值：成功返回0， 失败返回-1
 *</code>
 *<ul>
 *<li>chmod在指定的文件上进行操作，fchmod函数则对已打开的文件进行操作</li>
 *<li>为了改变一个文件的权限位，进程的有效用户ID必须等于文件的所有者ID，或者进程必须具有超级用户权限</li>
 *<li>参数mode是下面常量按位或运算构成的
 *<table>
 *<caption>chmod函数的mode常量</caption>
 *<tr><th width="40">序号</th><th width="40">分类</th><th width="100">mode</th><th>描述</th></tr>
 *<tr><td>1</td><td rowspan="3"></td><td>S_ISUID</td><td>执行时设置用户ID</td></tr>
 *<tr><td>2</td><td>S_ISGID</td><td>执行时设置组ID</td></tr>
 *<tr><td>3</td><td>S_ISVTX</td><td>保存正文(粘住位)</td></tr>
 *<tr><td>4</td><td rowspan="4">用户</td><td>S_IRWXU</td><td>用户(所有者)读、写和执行</td></tr>
 *<tr><td>5</td><td>S_IRUSR</td><td>用户（所有者）读</td></tr>
 *<tr><td>6</td><td>S_IWUSR</td><td>用户（所有者）写</td></tr>
 *<tr><td>7</td><td>S_IXUSR</td><td>用户（所有者）执行</td></tr>
 *<tr><td>8</td><td rowspan="4">组</td><td>S_IRWXG</td><td>组读、写和执行</td></tr>
 *<tr><td>9</td><td>S_IRGRP</td><td>组读</td></tr>
 *<tr><td>10</td><td>S_IWRGP</td><td>组写</td></tr>
 *<tr><td>11</td><td>S_IXGRP</td><td>组执行</td></tr>
 *<tr><td>12</td><td rowspan="4">其他</td><td>S_IRWXO</td><td>其他读、写和执行</td></tr>
 *<tr><td>13</td><td>S_IROTH</td><td>其他读</td></tr>
 *<tr><td>14</td><td>S_IWOTH</td><td>其他写</td></tr>
 *<tr><td>15</td><td>S_IXOTH</td><td>其他执行</td></tr>
 *</table>
 </li>
 *</ul>
 *\retval 0 成功
 *\retval !0 失败   
 */
int test_chmod()
{
	return 0;
}

/**
 *\brief 测试chown函数
 *
 *<code>
 *int chown(const char *pathname, uid_t owner, gid_t group);<br/>
 *int fchown(int filedes, uid_t owner, gid_t group);<br/>
 *int lchown(const char *pathname, uid_t owner, gid_t group);<br/>
 *返回值：成功返回0， 失败返回-1
 *</code>
 *<ul>
 *<li>三个函数的操作类似</li>
 *<li>在符号链接的情况下，lchown更改符号链接本身的所有者，而不是该符号链接所指向的文件</li>
 *<li>两个参数owner或group中的任意一个是-1， 则对应的ID不变</li>
 *<li>基于BSD的系统一直规定只有超级用户才能更改一个文件的所有者，这样是为了防止用户改变其文件的所有者从而摆脱磁盘空间限额对他们的限制<br/>系统V则允许任一用户更改他们拥有的文件的所有者</li>
 *<li>如果设置了常量__POSIX_CHOWN_RESTRICTED对指定的文件起作用，则：
 *<ol>
 *<li>只有超级用户进程能更改该文件的用户ID</li>
 *<li>若满足下列2个条件，一个非超级用户进程可以更改该文件的组ID<br/>
 (a)进程拥有此文件（其中有效用户ID等于该文件的用户ID）<br/>
 (b)参数owner等于-1或文件的用户ID，并且参数group等于进程的有效组ID或进程的附加组ID之一</li>
 *</ol>
 *<br/>
 *这预示着，当__POSIX_CHOWN_RESTRICTED起作用时，不能更改其他用户文件的用户ID<br/>
 *你可以更改你所拥有的文件组ID，但只能改到你所属的组</li>
 *<li>若这些函数由非超级用户进程调用，则在成功返回时，该文件的设置用户ID为和设置组ID位都会被清除</li>
 *</ul>
 *\retval 0 成功
 *\retval !0 失败   
 */
int test_chown()
{
	return 0;
}

/**
 *\brief 测试truncate函数
 *
 *<code>
 *int truncate(const char *pathname, off_t length);<br/>
 *int ftruncate(int filedes, off_t length);<br/>
 *返回值：若成功则返回0， 若出错则返回-1
 *</code>
 *<ul>
 *<li>若该文件以前的长度大于length，则超过length以外的数据就不能访问</li>
 *<li>若该文件以前的长度短语length，则效果与系统有关</li>
 *</ul>
 *\retval 0 成功
 *\retval !0 失败   
 */
int test_truncate()
{
	return 0;
}

/**
 *\brief 测试link函数
 *
 *<code>
 *int link(const char *existingpath, const char *newpath);<br/>
 *int ulink(const char *pathname);<br/>
 *返回值：若成功则返回0， 若出错则返回-1
 *</code>
 *<ul>
 *<li>link，函数创建一个新目录项newpath，所以任何一个文件可以有多个目录项指向i节点</li>
 *<li>link，若newpath已经存在，则返回出错</li>
 *<li>link，只创建newpath中的最后一个分量，路径中的其他部分应当已经存在</li>
 *<li>unlink，删除一个现有的目录项，并将由pathname所引用文件的链接计数减1</li>
 *<li>unlink，如果还有指向该文件的其他链接，则仍可通过其他链接访问该文件的数据，只有当链接计数达到0时，该文件的内容才可被删除，另一个条件也会阻止删除文件的内容---只要有进程打开了该文件，其内容也不能删除。unlink的这种性质经常被程序用来确保即使是在该程序崩溃时，它所创建的临时文件也不会遗留下来。进程用open或creat创建一个文件，然后立即调用unlink。因为该文件仍旧是打开的，所以不会讲其内容删除，只有当进程关闭该文件或终止时，该文件的内容才会被删除。</li>
 *<li>unlink，如果出错，则不对该文件做任何更改</li>
 *<li>unlink，如果pathname是符号链接，那么unlink删除该符号链接，而不会删除由该链接所引用的文件</li>
 *</ul>
 *\retval 0 成功
 *\retval !0 失败  
 */
int test_link()
{
	return 0;
}

/**
 *\brief 测试rename函数
 *
 *<code>
 *int rename(const char *oldname, const char *newname);<br/>
 *返回值：若成功则返回0， 若出错则返回-1
 *</code>
 *rename可以对文件或目录进行更名操作，根据oldname是指文件还是目录， 有几种情况需要说明
 *<ol>
 *<li>若oldname是文件，如果newname已存在，则它不能引用一个目录， 如果newname已存在，而且不是一个目录，则先将该目录项删除然后将oldname更名为newname。对包含oldname的目录以及包含newname的目录，调用进程必须具有写权限，因为将更改这两个目录</li>
 *<li>若oldname是目录，如果newname已存在，则它必须引用一个目录，而且该目录应当是空目录。如果newname存在且为空目录，则先将其删除， 然后将oldname更名为newname。另外，当为一个目录更名时，newname不能包含oldname作为其路径前缀。</li>
 *<li>若oldname或newname引用符号链接，则处理的石符号链接本身，而不是它所引用的文件</li>
 *<li>若oldname和newname引用同一个文件，则函数不做任何更改而成功返回。</li>
 *</ol>
 *\retval 0 成功
 *\retval !0 失败   
 */
int test_rename()
{
	return 0;
}

/**
 *\brief 测试symlink函数
 *
 *符号链接，是指一个文件的间接指针，它和硬链接不同，硬链接直接指向文件的i节点，引入符号链接的原因是为了避开硬链接的限制
 *<ol>
 *<li>硬链接通常要求链接和文件位于同一文件系统中</li>
 *<li>只有超级用户才能创建指向目录的硬链接</li>
 *</ol>
 *对于符号链接以及它指向何种对象并无任何文件系统限制，任何用户都可创建指向目录的符号链接。符号链接一般用于将一个文件或整个目录结构移到系统中的另一个位置。<br/>
 *<br/>
 *当使以名字应用文件的函数时，需要了解函数是否处理符号链接，也就是函数是否跟谁符号链接到达它所链接的文件。如果函数具有处理符号链接的功能，则其路径名参数引用由符号链接指向的文件。否则，路径名参数将引用链接本身，而不是链接指向的文件。
 *<table>
 *<caption></caption>
 *<tr><th width="40">序号</th><th width="50">函数</th><th width="120">跟随符号链接</th><th>说明</th></tr>
 *<tr><td>1</td><td>access</td><td>是</td><td></td></tr>
 *<tr><td>2</td><td>chdir</td><td>是</td><td></td></tr>
 *<tr><td>3</td><td>chmod</td><td>是</td><td></td></tr>
 *<tr><td>4</td><td>chown</td><td>是<b>/否</b></td><td></td></tr>
 *<tr><td>5</td><td>creat</td><td>是</td><td></td></tr>
 *<tr><td>6</td><td>exec</td><td>是</td><td></td></tr>
 *<tr><td>7</td><td>lchown</td><td><b>/否</b></td><td></td></tr>
 *<tr><td>8</td><td>link</td><td>是</td><td></td></tr>
 *<tr><td>9</td><td>lstat</td><td><b>/否</b></td><td></td></tr>
 *<tr><td>10</td><td>open</td><td>是</td><td></td></tr>
 *<tr><td>11</td><td>opendir</td><td>是</td><td></td></tr>
 *<tr><td>12</td><td>pathconf</td><td>是</td><td></td></tr>
 *<tr><td>13</td><td>readlink</td><td><b>/否</b></td><td></td></tr>
 *<tr><td>14</td><td>remove</td><td><b>/否</b></td><td></td></tr>
 *<tr><td>15</td><td>stat</td><td>是</td><td></td></tr>
 *<tr><td>16</td><td>truncate</td><td>是</td><td></td></tr> 
 *<tr><td>17</td><td>unlink</td><td><b>/否</b></td><td></td></tr>
 *</table>
 *
 *<br/>
 *
 *<code>
 *int symlink(const char *actualpath, const char *sympath);<br/>
 *返回值：成功则返回0， 出错则返回-1<br/>
 *ssize_t readlink(const char *restrict pathname, char *restrict buf, size_t bufsize);<br/>
 *返回值：成功则返回读到的字节数， 出错则返回-1<br/>
 *</code>
 *\warning symlink函数创建一个指向actualpath的新目录项sympath，在创建此符号链接时，并不要求actualpath已经存在，并且，actualpath和sympath并不需要位于同一文件系统中。<br/>
 *\warning readlink函数提供一种方法打开该链接本身，并读该链接中的名字。该函数组合了open、read和close的所有操作，如果函数成功执行，则它返回读入buf的字节数，在buf中返回的符号链接的内容不以null字符终止。<br/>
 *\retval 0 成功
 *\retval !0 失败    
 */
int test_symlink()
{
	return 0;
}

/**
 *\brief 测试utime函数
 *
 *<table>
 *<caption>每个文件相关的三个时间值</caption>
 *<tr><th width="20">序号</th><th width="40">字段</th><th>说明</th></tr>
 *<tr><td>1</td><td>st_atime</td><td>文件数据的最后访问时间，可以用 ls -u 命令显示查看， read函数可以修改该值</td></tr>
 *<tr><td>2</td><td>st_mtime</td><td>文件数据的最后修改时间，可以用 ls 命令显示查看，write函数可以修改该值</td></tr>
 *<tr><td>3</td><td>st_ctime</td><td>i节点状态的最后更改时间，可以用 ls -c 命令显示查看， chmod、chown函数可以修改该值</td></tr>
 *</table>
 *<code>
 *int utime(const char *pathname, const struct utimebuf *times);<br/>
 *返回值：成功返回0， 出错则返回-1
 *</code>
 *一个文件的访问和修改时间可以用utime函数更改，
 *此函数的操作以及执行它所要求的特权取决于times参数是否是NULL
 *<ul>
 *<li>如果times是一个空指针，则访问时间和修改时间两者都设置为当前时间。为了执行此操作必须满足下列两条件之一，进程的有效用户ID必须等于该文件的所有者ID，或者进程对该文件必须具有写权限</li>
 *<li>如果times是非空指针， 则访问时间和修改时间被设置为times所指向结构中的值，此时，进程的有效用户ID必须等于该文件的所有者ID，或者进程必须是一个超级用户进程，对文件只具有写权限是不够的</li>
 *<li>不能对更改状态时间st_ctime指定一个值，当调用utime函数时，此字段将自动更新</li>
 *</ul>
 *\retval 0 成功
 *\retval !0 失败
 */
int test_utime()
{
	return 0;
}

/**
 *\brief 测试mkdir函数
 *
 *<code>
 *int mkdir(const char *pathname, mode_t mode);<br/>
 *int rmdir(const char *pathname);<br/>
 *返回值：若成功则返回0， 若出错则返回-1
 *</code>
 *<ul>
 *<li>mkdir, 函数创建一个新的空目录， 所指定的文件访问权限mode由进程的文件模式创建屏蔽字修改</li>
 *<li>mkdir, 常见的错误是指定与文件相同的mode（只指定读、写权限），但是，对于目录通常至少要设置1个执行权限位，以允许访问该目录中的文件名</li>
 *<li>rmdir, 函数可以删除一个空目录</li>
 *<li>rmdir, 如果调用此函数使目录的链接计数成为0，并且也没有其他进程打开此目录，则释放由此目录占用的空间；如果在链接计数达到0时，有一个或几个进程打开了此目录，则在此函数返回前删除最后一个链接，另外，在此目录中不能在创建新文件，但是在最后一个进程关闭它之前并不释放此目录</li>
 *</ul>
 *\retval 0 成功
 *\retval !0 失败
 */
int test_mkdir()
{
	return 0;
}

/**
 *\brief 测试readdir函数
 *
 *<code>
 *DIR *opendir(const char *pathname);<br/>
 *返回值：成功返回指针，出错返回NULL<br/>
 *struct dirent *readdir(DIR *dp);<br/>
 *返回值：成功返回指针，在目录结尾或出错则返回NULL<br/>
 *void rewinddir(DIR *dp);<br/>
 *void closedir(DIR *dp);<br/>
 *返回值：若成功则返回0，若出错则返回-1<br/>
 *long telldir(DIR *dp);<br/>
 *返回值：若成功则返回0，若出错则返回-1<br/>
 *void seekdir(DIR *dp, long loc);
 *</code>
 *\retval 0 成功
 *\retval !0 失败
 */
int test_readdir()
{
	return 0;
}

/**
 *\brief 测试chdir函数
 *
 *<ul>
 *<li>每个进程都有一个当前工作目录，此目录是搜索所有相对路径名的起点</li>
 *<li>当前工作目录通常是在用户登录系统时，从口令文件（/etc/passwd）中获取的</li>
 *<li>当前工作目录是进程的一个属性，起始目录则是登录名的一个属性</li>
 *<li>进程通过调用chdir和fchdir函数可以更改当前工作目录，在这两个函数中，分别用pathname或打开文件描述符来指定新的当前工作目录</li>
 *</ul>
 *<code>
 *int chdir(const char *pathname);<br/>
 *int fchdir(int filedes);<br/>
 *返回值：成功返回0， 出错返回-1
 *</code>
 *\retval 0 成功
 *\retval !0 失败
 */
int test_chdir()
{
	return 0;
}

/**
 *\brief 测试getcwd函数
 *
 *获取当前工作目录的绝对路径
 *
 *<code>
 *char *getcwd(char *buf, size_t size);<br/>
 *返回值：成功则返回buf，出错则返回NULL
 *</code>
 *
 *\retval 0 成功
 *\retval !0 失败
 */
int test_getcwd()
{
	return 0;
}

static void show_help()
{
	printf("stat file_name, 显示文件信息\n\n"
		"access file_name modes, 测试文件的权限\n"
		"\tmodes   R_OK|W_OK|X_OK|F_OK\n\n");
}

int main(int argc, char **argv)
{
	if (argc == 3 && strcmp(argv[1], "stat") == 0){
		
		test_stat(argv[2]);
	}else if (argc == 4 && strcmp(argv[1], "access") == 0){
		
		test_access(argv[2], convert_modes(argv[3]));
	}else{
		
		show_help();
	}
	
	return 0;
}
