//姓名：刘哲
//学号：3150103520
#include "header.h"

extern char **environ; //环境变量

//提示符处理
extern int NO_PROMPT; //当运行批处理文件时，应该不输出提示符
extern int IN_EXEC;  //当运行exec命令时,由于重定向了标准输入，不输出提示符


//myshell的当前路径和原路径
extern char path_name[MAX_PATH_LENGTH];//myshell 当前的路径
extern char home_name[MAX_NAME_LENGTH];//myshell 源文件的路径


//parse后的结果
extern int indicator;//标识parse结果是否存在重定向，管道等特殊操作
extern char* output_file_name;//如果有重定向，输出重定向的文件名

//后台进程表
extern  pid_t background_process_table[MAX_PID_NUMBER];

//当前的命令提示符
extern  char prompt[MAX_PROMPT];