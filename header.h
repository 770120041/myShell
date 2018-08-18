//姓名：刘哲
//学号：3150103520
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h> 
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <readline/readline.h>
#include <readline/history.h>


//函数声明:
void get_path(); //获取路径，输出提示符
int get_command(char **,char **); //获取命令
void parse_command(char **,int); //解析命令
int builtin_command(char *,char **,int);//内置命令的处理函数
void main_loop();//主循环
void signal_function(int sig);//信号处理函数



//宏定义：

struct passwd *pwd;//用于获取当前路径的系统内置结构结构
//用于解析命令状态的一些flag（用位运算实现）
#define PIPED  1//使用管道时，第一位是1
#define OUT_REDIRECT  2//使用输出重定向时，第二位是1
#define OUT_REDIRECT_WITH_APPEND  4//使用输出重定向的附加时，第三位是1
#define IN_REDIRECT  8//使用输入重定向时，第四位是1
#define BACKGROUND  16//使用后台运行时，第五位是1


#define MAX_NAME_LENGTH  512 //最大用户名长度
#define MAX_PATH_LENGTH  512 //最大路径长度

#define MAX_PROMPT  4096   //提示符的最大长度
#define MAX_LINE  4096   //命令的缓冲Buffer的读取长度
#define MAX_ARG_NUMBER  128  //最大的参数个数
#define MAX_PID_NUMBER  4096 //后台进程表的最大大小
