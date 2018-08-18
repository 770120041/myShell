//姓名：刘哲
//学号：3150103520
#include "get_command.h"

/*
**
** 函数：get_command
** 功能：读取用户输入并做初步解析，分离用户命令和输入参数
** 参数：command,parameters 返回值 int(代表参数个数，-1代表输入有误)
** 关联函数：main_loop()
** 
** 内部逻辑：
** 1.判断当前状态，是否需要输出命令提示符
** 2.从标准输入读取命令
** 3.从输入中分离命令和参数
** 4.执行结束，回到主循环
**
*/

int get_command(char **command,char **parameters)
{
    if(!NO_PROMPT && !IN_EXEC){  //不在exec 或者 批处理情况下，输出提示符
        printf("%s",prompt);    
    }
    
    char *read_buffer = (char*)malloc(MAX_LINE); //读取用户输入用的临时buffer
    int parameter_number = 0;   //读取的参数个数
    char *start_pointer = read_buffer; //命令解析使用的头指针
    char *end_pointer = read_buffer;//命令解析使用的尾指针
    int finish_flag = 0;//命令解析完成的标识符
    
    fgets(read_buffer,MAX_LINE,stdin); //从标准输入获取命令
    
    if(read_buffer[0] == '\0')//无命令
        return -1;
    do
    {
        //跳过空白部分
        while((*end_pointer == ' ' && *start_pointer == ' ') || (*end_pointer == '\t' && *start_pointer == '\t'))
        {
            end_pointer++;   start_pointer++;
        }
        //star和end同时到了结尾
        if(*end_pointer == '\0')
        {
            if(parameter_number == 0)
            {
                return -1;//无命令，全是空字符
            }
            break;
        }

        if(*end_pointer == '\n') //到达了换行符位置
        {
            if(parameter_number == 0)
            {
                return -1;//无命令，全是空字符
            }
            break;
        }

        //尾指针不断移动扫描参数
        while(*end_pointer != ' ' && *end_pointer != '\0' && *end_pointer != '\n'){
            end_pointer++;
        }

        //如果还没有读取任何参数，那么第一个读取到的字符串就是命令
        if(parameter_number == 0)
        {
            char *temp_pointer = end_pointer;
            *command = start_pointer;
            //buffer最开始的一个参数是command本身
            while(temp_pointer!=start_pointer && *temp_pointer !='/')
                temp_pointer--;
            if(*temp_pointer == '/')
                temp_pointer++;
            //找到反斜杠之前
            parameters[0] = temp_pointer;
            parameter_number += 2;
        }
        else if(parameter_number <= MAX_ARG_NUMBER) //如果读取的参数在最大参数限制内
        {
            parameters[parameter_number-1] = start_pointer;
            parameter_number++;
        }
        else
        {
            break;
        }

        if(*end_pointer == '\0' || *end_pointer == '\n')//读到行末尾了，结束读取
        {
            *end_pointer = '\0';
            finish_flag = 1;
        }
        else//还没到行末尾，继续扫描
        {
            *end_pointer = '\0';
            end_pointer++;
			start_pointer = end_pointer;//继续之前流程扫描
        }
    }
    while(finish_flag == 0);

    parameters[--parameter_number] = NULL;

    return parameter_number;
}