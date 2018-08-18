//姓名：刘哲
//学号：3150103520
#include "parse_command.h"

/*
**
** 函数：parse_command
** 功能：解析arameters数组，分析其中是否需要实现IO重定向等特殊功能
** 参数：parameters，parameter_number 返回值  void
** 关联函数：main_loop()
** 
** 内部逻辑：
** 1.初始化有关parse结果的全局变量
** 2.解析参数数组，判断是否需要后台执行。如果需要，置indicator 的 background位为位1
** 3.解析参数数组，判断是否需要IO重定向.如果需要，置indicator 的 redirect或者redirect_append位为位1
** 4.解析参数数组，判断是否需要管道.如果需要，置indicator 的 piped位为位1
**
*/
void parse_command(char **parameters,int parameter_number)
{
    //initialize the parse result struct
    indicator = 0;  //命令解析的位标志，判断这个命令是否需要管道，IO重定向
    input_file_name = NULL; //需要IO重定向时的输入文件
    output_file_name = NULL;//需要IO重定向时的输出文件
    command_after_pipe = NULL;//需要管道时管道后面的命令
    parameters_after_pipe = NULL;//需要管道时管道后面的参数

    if(strcmp(parameters[parameter_number-1],"&") ==0) //判断是否需要后台执行
    {
        indicator |= BACKGROUND;//后台
        parameters[--parameter_number] = NULL;
    }

    //逐个扫描每个参数，判断是否需要置indicator的相应位为1
    for(int i=0;i<parameter_number;) //注意这里i的自增由每个分支执行
    {   
        if(strcmp(parameters[i],">")==0) //输出重定向
        {
            indicator |= OUT_REDIRECT; 
            output_file_name = parameters[i+1];
            parameters[i] = NULL; //第i个参数是< 或者 << ，无意义,置为null
            i+=2; //parse下两个参数
        }
        else if(strcmp(parameters[i],">>")==0) //输出重定向 附加
        {
            indicator |= OUT_REDIRECT_WITH_APPEND; 
            output_file_name = parameters[i+1];  //输出文件
            parameters[i] = NULL; //第i个参数是< 或者 << ，无意义,置为null
            i+=2; //parse下两个参数
        }

        else if(strcmp(parameters[i],"<<")==0 || strcmp(parameters[i],"<")==0)//输入重定向
        {
            indicator |= IN_REDIRECT; 
            input_file_name = parameters[i+1]; //输入文件的来源是下一个参数
            parameters[i] = NULL; //第i个参数是< 或者 << ，无意义,置为null
            i+=2; //parse下两个参数
        }
        else if(strcmp(parameters[i],"|")==0) //管道
        {
            parameters[i] = NULL; //第i个参数是 | 无意义

            char* temp_str;
            indicator |= PIPED; //置indicator的相应位位1
            command_after_pipe = parameters[i+1]; //提取第二个命令
            parameters_after_pipe = &parameters[i+1];//提取第二个命令的参数
            for(temp_str = parameters_after_pipe[0]+strlen(parameters_after_pipe[0]);
                    temp_str!=&(parameters_after_pipe[0][0]) && *temp_str!='/';temp_str--)
            //跳过无关命令
            {
                ;
            }
            if(*temp_str == '/')
            {
                temp_str++;
            }
            
            parameters_after_pipe[0] = temp_str;
            break;
        }
        else{
            i++; //正常参数
        }
    }
}
