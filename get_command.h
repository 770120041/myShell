//姓名：刘哲
//学号：3150103520
#include "header.h"


//提示符处理
extern int NO_PROMPT; //当运行批处理文件时，应该不输出提示符
extern int IN_EXEC;  //当运行exec命令时,由于重定向了标准输入，不输出提示符

extern  char prompt[MAX_PROMPT];