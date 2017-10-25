/**
 *\file c10_signal.cc
 *\brief 练习信号
 *\date 2017/10/07
 */

#include <unistd.h>

/**
 *\brief 测试signal函数
 *
 *<ul>
 *<li>信号是软件中断</li>
 *<li>信号提供了一种处理异步事件的方法</li>
 *<li>每个信号都有一个名字，这些名字都以三个字符SIG开头</li>
 *<li>信号都被定义为正整数（信号编号）</li>
 *<li>不存在编号为0的信号，kill函数对信号编号0有特殊应用，称这种信号为空信号</li>
 *</ul>
 *
 *<table>
 *<caption>UNIX系统信号列表</caption>
 *<tr><th width="40">序号</th><th width="60">名字</th><th>描述</th><th>默认动作</th></tr>
 *<tr><td>1</td><td>SIGABRT</td><td>异常终止(abort)</td><td>终止+core</td></tr>
 *<tr><td>2</td><td>SIGALRM</td><td>超时（alarm）</td><td>终止</td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr>
 *<tr><td>1</td><td></td><td></td></tr> 
 *</table>
 *\retval 0 成功
 *\retval !0 失败
 */
int test_signal()
{
	return 0;
}