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
 *<tr><td>9</td><td>类型：time_t --->> 名称：<b>st_actime</b></td><td>time of last access </td></tr>
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
 *<tr><td colspan="3"><ul><li>一般地，有效用户ID等于实际用户ID，有效组ID等于实际组ID</li><li>当执行一个可执行文件时，进程的有效用户ID、有效组ID通常就是对应的实际用户ID和实际组ID。<br/>俗话说万事都有例外，在文件模式字（st_mode）有那么两位分别为设置用户ID（set-user-ID）位和设置组ID（set-groupID ）位。<br/>它们的作用是，当执行此程序文件时，将进程的有效用户ID或有效组ID 设置为文件所有者的用户ID（st_uid）或文件的组所有者ID（st_gid）</li><li>举个栗子，若文件所有者是超级用户，而且设置了该文件的设置用户ID位， 然后当该程序由一个进程执行时，则该进程具有超级用户权限。不管执行此文件的进程的进程的实际用户ID是什么，都进行这种处理。像passwd程序，允许任意用户改变其口令，它就是一个设置用户ID程序。</li><li>就像上面描述的一样，因为运行设置用户ID程序的进程通常得到额外的权限，所以编写这种程序时要特别谨慎。</li></ul></td></tr>
 *</table>
 *\retval 0 成功
 *\retval !0 失败 
 */
int test_stat()
{
	return 0;
}

/**
 *\brief 测试access函数
 *
 *<code>
 *int access(const char *pathname, int mode);<br/>
 *返回值：成功返回0， 失败返回-1
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
 *\retval 0 成功
 *\retval !0 失败 
 */
int test_access()
{
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