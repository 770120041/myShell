//姓名：刘哲
//学号：3150103520
#include "signal_function.h"

/*
**
** 函数：signal
** 功能：接受sig信号，根据sig信号控制程序的执行方式
** 参数：sig 返回值  无
** 关联函数：main_loop()
** 
** 内部逻辑：
** 1.判断接收到的信号类型
** 2.根据信号类型处理程序运行状态
**
*/

void signal_function(int sig)
{
    if(sig == SIGCHLD){
        pid_t pid; //临时pid用于接收wait信号
        int i;
        for(i=0;i<MAX_PID_NUMBER;i++)
            if(background_process_table[i] != 0) //只处理后台进程
            {
                pid = waitpid(background_process_table[i],NULL,WNOHANG);
                if(pid > 0)//进程退出
                {
                    printf("Process %d finished.\n",pid);
                    background_process_table[i] = 0; //清零后台表
                }
            }
    }
    else if(sig == SIGINT){
        signal(SIGINT,SIG_IGN);
    }
    else if(sig == SIGTSTP){
        signal(SIGINT,SIG_IGN);
    }
    
    return;
}