//姓名：刘哲
//学号：3150103520
#include "header.h"

//全局变量：
char **environ; //环境变量

//提示符处理
int NO_PROMPT=0; //当运行批处理文件时，应该不输出提示符
int IN_EXEC=0;  //当运行exec命令时,由于重定向了标准输入，不输出提示符


//myshell的当前路径和原路径
char home_name[MAX_NAME_LENGTH];//myshell 源文件的路径
char path_name[MAX_PATH_LENGTH];//myshell 当前的路径


//parse后的结果
int indicator;//标识parse结果是否存在重定向，管道等特殊操作
char* input_file_name;//如果有重定向，输入重定向的文件名
char* output_file_name;//如果有重定向，输出重定向的文件名
char* command_after_pipe;//如果有管道，那么是管道后的命令
char** parameters_after_pipe;//如果有管道，那么是管道后的参数

//解析完的参数和命令
char *command = NULL;
char **parameters;


//后台进程表
pid_t background_process_table[MAX_PID_NUMBER];

//当前的命令提示符
char prompt[MAX_PROMPT];




/*
**
** 函数：main_loop
** 功能：读取用户命令并执行
** 参数：无 返回值 无
** 关联函数：signal_function(),get_path,builtin_command(),read_command(),parse_command()
** 
** 内部逻辑：
** 1.获得myshell当前路径，决定指令执行位置
** 2.读取用户输入
** 3.解析用户输入，转化为指令和参数形式
** 4.判断用户输入的是否是内建命令，如果是内建命令，调用builtin_command函数执行
** 5.如果不是内建命令，判断是否存在管道，重定向，后台等指令，调用exec函数族执行
** 6.回到步骤1（如果中途有不可恢复性错误，则跳出循环，退出程序）
**
*/



void main_loop()
{
    if(signal(SIGCHLD,signal_function) == SIG_ERR){ //注册信号处理函数，当后台进程状态变化时，调用信号处理函数
        perror("signal() error"); //如果信号出错，打印错误信息
    }   
    pid_t temp_pid,temp_pid2;//创建子进程临时pid
    parameters = malloc(sizeof(char *)*(MAX_ARG_NUMBER+2));//存放参数的数组
    int parameter_numer;//解析的参数个数
    int AGAIN_EXEC = 0;
    while(1)
    {
        if(AGAIN_EXEC){
            exit(0);
        }
        if(IN_EXEC){
            AGAIN_EXEC=1;
        }
        // printf("in loop\n");
        get_path(); //获取myshell的当前执行路径
        parameter_numer = get_command(&command,parameters); //获取命令，返回值代表参数个数，如果参数个数是-1，说明读取失败
        if(-1 == parameter_numer){ // 命令解析失败，重新读取命令
            continue;
        }
        parse_command(parameters,parameter_numer); //解析用户命令，判断是否IO重定向，管道等指令

        int fd[2],in_fd,out_fd;//管道句柄和临时文件描述符
        
        if(builtin_command(command,parameters,parameter_numer)){//如果是内建命令，那么调用内建命令执行函数，执行完成后跳过这轮循环
            continue;
        }
        if(indicator & PIPED) //命令使用了管道
        {                
            if( pipe(fd) < 0) //开启管道
            {
                printf("myshell error : open pipe failed.\n");//开启管道失败，提示错误信息
                exit(0);//退出程序
            }
        }  
        if((temp_pid = fork())!=0) //myshell 进程（父进程）
        {
            //注册子进程变化时的信号处理函数
            if(signal(SIGCONT, signal_function) == SIG_ERR ){
                perror("signal error!");
            }
            //使用了管道
            if(indicator & PIPED)   
            {
                if((temp_pid2=fork()) == 0) //子进程执行管道命令
                {
                    close(fd[1]);//关闭管道写
                    close(fileno(stdin));//关闭stdlin 
                    dup2(fd[0], fileno(stdin));//文件描述符修改，实现重定向
                    close(fd[0]); 
                    execvp(command_after_pipe,parameters_after_pipe);//执行管道命令
                }
                else    //主进程等待子进程
                {
                    close(fd[0]);
                    close(fd[1]);
                    waitpid(temp_pid2,NULL,0); //wait command_after_pipe
                }
            }

            if(indicator & BACKGROUND) //指令要求后台执行
            {
                printf("child pid is :%u\n",temp_pid); //后台执行的进程pid
                int i;
                for(i = 0 ; i < MAX_PID_NUMBER ; i++ )
                {
                    if(background_process_table[i]==0)//寻找后台进程表的空位置
                    {
                        background_process_table[i] = temp_pid; //注册一个后台进程
                        break;
                    }
                }
                if(i == MAX_PID_NUMBER)//达到后台进程表的上限
                    perror("Too much background processes!\n");                    
            }
            else
            {          
                waitpid(temp_pid,NULL,0);//等待进程
            } 
        }
        else //myshell进程的子进程
        {
            //发送信号给处理函数
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
            signal(SIGCONT, SIG_DFL);

			if(indicator & PIPED) //使用管道
            {                
                if(!(indicator & OUT_REDIRECT) && !(indicator & OUT_REDIRECT_WITH_APPEND)) //没有输出重定向
	           {
                    close(fd[0]);
                    close(fileno(stdout)); 
                    dup2(fd[1], fileno(stdout));
                    close(fd[1]);
                }
                else //存在输出重定向和管道
	           {
                    close(fd[0]);
                    close(fd[1]);
                    if(indicator & OUT_REDIRECT)//输出重定向，用O_CREAT 的参数来调用open
    	               out_fd = open(output_file_name, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                    else//输出重定向附加，用O_APPEND来调用open
    	               out_fd = open(output_file_name, O_WRONLY|O_APPEND|O_TRUNC, 0666);
                    close(fileno(stdout)); 
                    dup2(out_fd, fileno(stdout));
                    close(out_fd);	        
                }
            }
            else
            {
                if(indicator & OUT_REDIRECT) // 使用输出重定向
	           {
                    out_fd = open(output_file_name, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                    close(fileno(stdout)); 
                    dup2(out_fd, fileno(stdout));
                    close(out_fd);
                }
                if(indicator & OUT_REDIRECT_WITH_APPEND) //使用附加的输出重定向
	           {
                    out_fd = open(output_file_name, O_WRONLY|O_CREAT|O_APPEND, 0666);
                    close(fileno(stdout)); 
                    dup2(out_fd, fileno(stdout));
                    close(out_fd);
                }
            }
            
            if(indicator & IN_REDIRECT) //使用输入重定向
            {
                in_fd = open(input_file_name, O_CREAT |O_RDONLY, 0666);
                close(fileno(stdin)); 
                dup2(in_fd, fileno(stdin));
                close(in_fd); 
            }
            execvp(command,parameters);//执行外部命令
        }
        
    }
    free(parameters);
}

/*
**
** 函数：get_path
** 功能：获取程序当前路径
** 参数：无 返回值 无
** 关联函数：main_loop()
** 
** 内部逻辑：
** 1.获得myshell当前路径，决定指令执行位置
** 2.修改提示符的内容
** 3.退出程序
**
*/

void get_path()
{
    extern struct passwd *pwd; //系统内建的passwd结构，储存当前用户名和文件位置
    sprintf(prompt,"\e[33m[myshell]:"); //输出myshell提示符
    int length = strlen(prompt);
    pwd = getpwuid(getuid());   //获取用户信息
    getcwd(path_name,MAX_PATH_LENGTH); //获取当前路径位置

    if(strlen(path_name) < strlen(pwd->pw_dir) ||     //如果不包含home路径，用~替代
            strncmp(path_name,pwd->pw_dir,strlen(pwd->pw_dir))!=0)
        sprintf(prompt+length,"%s",path_name);
    else        //如果包含home路径，用~替代
        sprintf(prompt+length,"~%s",path_name+strlen(pwd->pw_dir));

    length = strlen(prompt); 
    sprintf(prompt+length,"$\e[0m"); //附加打印颜色终止符
    return;
}

/*
**
** 函数：main
** 功能：获取程序当前路径
** 参数：argc,argv,envp 返回值 int
** 关联函数：main_loop()
** 
** 内部逻辑：
** 1.在程序开始运行时，获得主机名和用户名，并且输出
** 2.设置环境变量，并获得外部环境变量
** 3.根据argc和argv判断是否使用批处理文件，并针对两种情况用不同方式调用main_loop
** 4.进入main_loop函数
** 5.main_loop结束，退出程序
**
*/


int main(int argc,char** argv,char **envp) {
    environ=envp;   //获得环境变量
    char hostname[MAX_NAME_LENGTH];//主机名字
    pwd = getpwuid(getuid());//获取用户信息
    getcwd(path_name,MAX_PATH_LENGTH);//获取路径名

    if(gethostname(hostname,MAX_NAME_LENGTH)==0)//打印主机名和用户名
        printf("\e[34m %s \e[0m in \e[34m %s \e[0m\n",pwd->pw_name,hostname);
    else
        printf("\e[34m %s \e[0m in unknown\n",pwd->pw_name);


    strcpy(home_name,path_name);
    strcat(path_name,"/myshell");
    if(setenv("shell" ,path_name,1) != 0){ //设置环境变量shell
		perror("myshell error : SET ENV ERROR");
	}

    for(int i=0;i<MAX_PID_NUMBER;i++){ //初始化后台进程空表
        background_process_table[i] = 0;
    }

    if(argc == 1){ //不使用批处理文件
        main_loop();
    }
    else{   //使用批处理文件
        freopen(argv[1],"r",stdin);
        NO_PROMPT=1;
        main_loop();
    }
    
    return 0;
}
