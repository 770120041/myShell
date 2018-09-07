#姓名：刘哲


这是一个关于myshell简单的用户手册。
我实现了一个简单的shell解释器，这个编辑器支持了以下功能：
    1. 解释执行命令
    2. 内建命令`cd` `exit` `fg` `bg` `umask` `set`等等 
    3. pipe 管道
    4. `fg` 前台执行命令
    5. `bg` 后台执行命令
    6. 彩色提示符
    7. I/O重定向
    8. 从文件中提取命令输入
    9. 程序的后台执行
    10.合适的命令提示符

这个用户手册包含以下几个部分：

一、基本命令的使用教程
二、I/O重定向的基本介绍和使用实例
三、管道的介绍和使用实例
四、后台执行的介绍和使用实例

一、基本命令的使用教程
1.myshell的简介
shell是一个命令解释器。它的作用是作为操作系统内核与用户之间的交互层。
myshell能够接收命令，然后逐行的执行用户命令。

2.在myshell中运行shell脚本和可执行程序
你可以用  "./script.sh"    在myshell中运行一个位于当前目录下的shell脚本
甚至你可以使用 " ./myshell  " 在myshell中运行myshell程序，只需要使用quit命令就可以退出新运行的myshell程序

3.使用pwd命令获取当前myshell的位置
“pwd”（print working directory），在终端中显示当前工作目录的全路径
在myshell中输入 "pwd” 命令即可

例如在我的电脑上，输入pwd命令会得到以下结果
"/Users/apple/Desktop/2017codes/linux/实验3/myshell"

4.使用time命令获取当前时间
“time”命令使用标准的输出打印当前的日期和时间。
例如在我的电脑上，输入time命令会得到以下结果
"2017/08/04 14:36:57 Friday"

5.使用ls来列出当前目录内容
ls命令是列出目录内容(List Directory Contents)的意思。运行它就是列出文件夹里的内容，可能是文件也可能是文件夹

ls文件的内容关系
- 普通文件
d 目录
c 字符设备
b 块设备
l 符号链接
s 套接字
p 管道

查看目录
ls -d */
ls -F | grep "/$"
ls -l | grep "^d"

ls -l    命令已详情模式(long listing fashion)列出文件夹的内容
ls -a    命令会列出文件夹里的所有内容，包括以”.”开头的隐藏文件

如果输入 ls -al，在我的电脑上出现的是以下内容
total 208
drwxr-xr-x  18 apple  staff    612  8  4 14:25 .
drwxr-xr-x  10 apple  staff    340  8  1 19:31 ..
-rw-r--r--@  1 apple  staff   6148  8  3 21:14 .DS_Store
-rw-r--r--   1 apple  staff  13544  8  4 11:20 builtin_command.c
-rw-r--r--   1 apple  staff    751  8  4 09:14 builtin_command.h
drwxr-xr-x   4 apple  staff    136  8  4 08:57 d1
-rw-r--r--   1 apple  staff   3282  8  4 13:53 get_command.c
-rw-r--r--   1 apple  staff    250  8  4 09:14 get_command.h
-rw-r--r--   1 apple  staff   1463  8  4 13:55 header.h
-rw-r--r--   1 apple  staff    253  8  3 23:58 makefile
-rwxr-xr-x   1 apple  staff  24044  8  4 14:25 myshell
-rw-r--r--   1 apple  staff   9806  8  4 13:55 myshell.c
-rw-r--r--   1 apple  staff     17  8  4 14:26 output.txt
-rw-r--r--   1 apple  staff   3388  8  4 14:02 parse_command.c
-rw-r--r--   1 apple  staff    530  8  4 10:11 parse_command.h
-rw-r--r--   1 apple  staff   1642  8  4 14:37 readme
-rw-r--r--   1 apple  staff   1003  8  4 14:25 signal_function.c
-rw-r--r--   1 apple  staff     98  8  4 09:14 signal_function.h

可以看到 .文件是一个目录文件，因为开头是d，并且对于user具有所有权限

6.使用cd命令来切换目录
经常使用的“cd”命令代表了改变目录。它在myshell中改变工作目录来执行，复制，移动，读，写等等操作

在myshell中的常用命令：
切换到上一目录 ： cd ..
切换到当前目录下的d1文件夹中: cd d1
切换成功后,myshell的提示符也会发生相应变化

例如：运行cd d1前的命令提示符如下
[myshell]:~/Desktop/2017codes/linux/实验3/myshell$
运行cd d1后: 
[myshell]:~/Desktop/2017codes/linux/实验3/myshell/d1$

7.使用mkdir来创建目录
“mkdir”(Make directory)命令在命名路径下创建新的目录。
然而如果目录已经存在了，那么它就会返回一个错误信息”不能创建文件夹，文件夹已经存在了”(“cannot create folder, folder already exists”)

使用方法：mkdir dirpath

一次创建多个目录：
mkdir -p d1 d2 d3
注意：目录只能在用户拥有写权限的目录下才能创建


8.获取更多命令和帮助的方法
以上就是一些myshell基本命令的使用实例，如果你想了解其他命令，或者对某个命令做更深入的了解
你可以在myshell中使用"man command" 命令来获得command命令更多的内容
比如输入"man ls "就会获得ls的详细使用手册




二、I/O重定向的基本介绍和使用实例
1.IO重定向的基本概念：数据流
在myshell中，数据流可分为两类：数据输入、数据输出
相应地，每一个进程也都有两个特殊的文件描述指针：标准输入（standard input，文件描述指针为0）、
标准输出（standard output，文件描述指针为1）
这两个特殊的文件描述指针使进程能在通常情况下接受标准输入设备的输入，并让标准输出设备显示输出。

标准输入通常指传给一个命令的键盘输入。例如运行命令"ls -al"，其中"-al"是传给ls命令的标准输入。
标准输出是一个命令的执行结果，例如运行"cat /file1.doc"命令后所看到的文件内容就是标准输出。标准输出通常被定向到显示器。

2.IO重定向的方法
用 < 来改变读进的数据流(stdin)，使之从指定的文档读进；
用 > 来改变送出的数据流(stdout)，使之输出到指定的文档；

基本重定向方式
  cmd > file                    把 stdout 重定向到 file 文件中
  cmd >> file                   把 stdout 重定向到 file 文件中(追加)
  cmd < file                    cmd 命令以 file 文件作为 stdin
  cmd < file >file2             cmd 命令以 file 文件作为 stdin，以 file2 文件作为 stdout
  
3.IO重定向的简单实例
ls -al > output.txt
把ls的信息经过IO重定向输出到output.txt中


三、管道的介绍和使用实例
1.管道的基本介绍
理解IO重定向之后，对于管道的理解就比较简单了。
管道命令操作符是：”|”,它能处理经由前面一个指令传出的正确输出信息，
也就是标准输出（standard output ）的信息，然后，作为标准输入（standard input）传递给下一个命令，.

2.使用方法
用法: command 1 | command 2 
它的功能是把第一个命令command 1执行的结果作为command 2的输入传给command 2

3.具体实例：
例如：
“ls -s|sort -n"
把ls的输出给sort命令，sort命令从小到大排序后输出

或者
”ls -al | grep *.c“
把ls的输出作为标准输入给grep命令，grep命令再筛选出所有以.c为后缀的文件输出到标准输出



当然管道还可进行多次操作，如下面的功能为先去除纯数字，再由sed将竖线(这里不是管道符号)替换为空格，
再将结果取出来排序，再进行结果的选择显示。

cat filename |grep -v '^[0-9]*$' | sed 's/|/ /g' |sort -nrk 8 -nrk 9 |tail -n +1 |head -n 10

4.管道与重定向的区别
(1)管道触发两个子进程执行"|"两边的程序；而重定向是在一个进程内执行
(2)管道左边的命令应该有标准输出 | 管道右边的命令应该接受标准输入
   对于重定向 > 右边只能是文件
   对于重定向 < 右边只能是文件


四、后台执行的介绍和使用实例
1.后台执行的介绍
当运行myshell时，如果直接输入某个命令，那么是把它前台执行的，这个时候myshell就会被占据
这个命令运行完毕之前，无法运行其他指令。当把命令后台执行时，它不会占据终端，终端仍然可以运行其他指令。

2.后台执行的使用方法
在要执行的命令后面加上 &即可

3.后台执行的使用实例
当输入"sleep 100 &"
就会在后台执行sleep 100的命令了
如果没有加上‘&’,就会导致终端执行sleep 100命令，无法执行其它命令了




