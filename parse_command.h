//姓名：刘哲
//学号：3150103520
#include "header.h"



//parse后的结果
extern int indicator;//标识parse结果是否存在重定向，管道等特殊操作
extern char* input_file_name;//如果有重定向，输入重定向的文件名
extern char* output_file_name;//如果有重定向，输出重定向的文件名
extern char* command_after_pipe;//如果有管道，那么是管道后的命令
extern char** parameters_after_pipe;//如果有管道，那么是管道后的参数

//后台进程表
extern  pid_t background_process_table[MAX_PID_NUMBER];

