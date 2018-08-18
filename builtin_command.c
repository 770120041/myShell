//姓名：刘哲
//学号：3150103520
#include "builtin_command.h"

/*
**
** 函数：builtin_command
** 功能：判断命令是否是内建命令，如果是，在本程序中执行。
** 参数：command,parameters,parameter_number 返回值 int(返回1代表是内建命令，返回0代表不是)
** 关联函数：main_loop()
** 
** 内部逻辑：
** 1.逐个判断读入命令是否是内建命令
** 2.如果是，判断参数等是否符合条件
** 3.如果符合条件，那么执行命令。
** 4.如果是内建命令但是参数不正确，那么返回1，重新读取命令。如果执行成功，也返回1，继续读取命令
** 5.执行结束，回到主循环
**
*/

int builtin_command(char *command, char **parameters,int parameter_number)
{
	extern struct passwd *pwd;

    /* 内建命令：quit
    ** 功能：退出myshell
    ** 参数：无
    ** 内部逻辑：无
    */
    if(strcmp(command,"quit")==0 || strcmp(command,"exit")==0 ) 
        exit(0);
    
    /* 内建命令：pwd
    ** 功能：显示myshell当前目录
    ** 参数：无
    ** 内部逻辑：无
    */
    else if(strcmp(command,"pwd")==0)
    {
        printf("%s\n",path_name);
        return 1;
    }

    /* 内建命令：help
    ** 功能：显示myshell当前目录
    ** 参数：无
    ** 内部逻辑：
    **   1.获取用户手册的路径 
    **   2.用more命令过滤，读取用户手册
    */
    else if(strcmp(command,"help") == 0)
    {
        char temp_path[MAX_PATH_LENGTH]; //获得用户手册的路径
        strcat(temp_path,home_name); 
        strcat(temp_path,"/readme");

        char *arguments[]={ "more",temp_path , NULL};

        pid_t pid=fork();
        if( pid < 0){
            printf("myshell error: fork error!\n");
        }
        strcpy(command,"more");
        if( pid == 0){//调用more来过滤，读取用户手册
            execvp(command,arguments);
        }
        if( pid > 0){
            waitpid(pid,NULL,0);//等待子进程
        }
        
        return 1;
    }
    /* 内建命令：time
    ** 功能：显示当前时间
    ** 参数：无
    ** 内部逻辑：无
    */
    else if(strcmp(command,"time")==0)
    {
        time_t t = time(0);
        char tmp[1024];
        strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A ",localtime(&t) );
        printf("%s\n",tmp);
        return 1;
    }

    /* 内建命令：clr
    ** 功能：清楚屏幕信息
    ** 参数：无
    ** 内部逻辑：无
    */
    else if(strcmp(command,"clr")==0) 
    {
        printf("\033[2J");
        return 1;
    }

    /* 内建命令：dir
    ** 功能：获取目录信息
    ** 参数：dir_name
    ** 内部逻辑：
    ** 1.判断dir_name是否为空，如果为空显示当前目录信息
    ** 2.如果不为空，判断dir_name是否为有效目录
    ** 3.判断是否需要输出重定向
    ** 4.执行命令
    */
    else if(strcmp(command,"dir")==0) //dir目录来获取目录信息
    {
        DIR *dir;
        struct dirent *ptr;
        if(parameter_number == 1){  //代表dir命令无参数，获取当前目录信息
            dir = opendir(path_name);
        }
        else{
            char dirname[MAX_PATH_LENGTH];
            dirname[0]='\0';

            //找到dir要获取的目录的名字
            for(int i=1;i<parameter_number;i++){
                if(parameters[i] != NULL){
                    strcpy(dirname,parameters[i]);
                    break;
                }
                if(parameters[i] == NULL){
                    break;
                }
            }
            //要获取的是当前目录
            if(dirname[0] == '\0'){
                dir = opendir(path_name);
            }
            else{
                if( (dir = opendir(dirname)) == NULL ){
                    printf("myshell error in dir:bad argument\n"); //获取的目录是无效目录
                    return 1;
                }
            }
        }
        int out_fd;
        if(indicator & OUT_REDIRECT)//判断是否存在重定向
        {
            //打开目标文件的文件描述符
            out_fd = open(output_file_name, O_WRONLY|O_TRUNC, 0666);
            if(out_fd<0){
                printf("myshell error in dir:bad redirect arguemnt!\n");
                return 1;
            }
        }
        else{
            out_fd = 1;//不需要重定向，打开标准输出的文件描述符
        }

        int temp_cnt=0;
        char temp_str[MAX_NAME_LENGTH];

        //输出目录下的文件名
        while((ptr = readdir(dir)) != NULL){
            temp_cnt++;
            if(ptr->d_name[0] != '.' ){
                strcpy(temp_str,ptr->d_name);
                strcat(temp_str,"   \t\t");
                write(out_fd,temp_str,strlen(temp_str));
            }
            else{
                temp_cnt--;
            }
            if(temp_cnt == 3){
                strcpy(temp_str,"\n");
                write(out_fd,temp_str,strlen(temp_str));
                temp_cnt=0;
            }
        }
        strcpy(temp_str,"\n");
        write(out_fd,temp_str,strlen(temp_str));
    
        // dup2(100, fileno(stdout));
        return 1;
    }

  

    /* 内建命令：cd
    ** 功能：切换当前目录
    ** 参数：path_name
    ** 内部逻辑：
    ** 1.判断参数是否为空，如果为空显示当前目录
    ** 2.如果不为空，判断path_name是否为有效目录
    ** 3.如果有效，那么切换到指定目录
    */
    else if(strcmp(command,"cd")==0)
    {
        char target_path[MAX_PATH_LENGTH];
        int no_parameters = 0;

        if(parameters[1] == NULL)         //cd 无参数 显示当前目录
        {
            no_parameters = 1;
        }
        else{   //cd 参数为 ~ ，等同于cd home_path
            if(parameters[1][0] == '~')
            {
                int temp_len = strlen(pwd->pw_dir)+strlen(parameters[1]);
                strcpy(target_path,pwd->pw_dir);
                strncpy(target_path+strlen(pwd->pw_dir),parameters[1]+1,strlen(parameters[1]));
            }
            else
            {
                strcpy(target_path,parameters[1]);
            }
        }
        if(no_parameters == 1){//如果没有参数，那么输出当前目录
            printf("%s\n",path_name);
        }
        else{
            if(chdir(target_path)!= 0)//如果切换木目录失败，打印错误信息
                printf("myshell error in cd : no such file or directory! \n");
        }
        return 1;
    }

    
    //environ  命令
    else if( strcmp(command,"environ")==0 ) //environ 获取环境变量
    {
        // int out_fd;
        // if(indicator & OUT_REDIRECT) //如果需要重定向
        // {
        //     out_fd = open(output_file_name, O_WRONLY|O_TRUNC, 0666);
        //     if(out_fd<0){
        //         printf("myshell error in dir:bad redirect arguemnt!\n");
        //         return 1;
        //     }
        // }
        // else{
        //     out_fd = 1;
        // }
        // int temp_cnt=0;
        // while(*environ){ //输出环境变量
        //     char temp_str[1024];
        //     strcpy(temp_str,*(environ++));
        //     strcat(temp_str,"\n");
        //     write(out_fd,temp_str,strlen(temp_str));
        // }
        strcpy(command,"env");
        return 0;
    }


    /* 内建命令：umask
    ** 功能：设置文件默认权限
    ** 参数：mask_number
    ** 内部逻辑：
    ** 1.判断参数是否为空，如果为空显示当前umask值
    ** 2.如果不为空，判断参数是否合法
    ** 3.如果参数合法，设置umask
    */
    else if(strcmp(command,"umask")==0) 
    {
        if(parameter_number==1){ //如果没有参数，那么输出当前umask值
            return 0;
        }
        else{
            mode_t temp_mask;
    
            //判断umaks值是否合理
            if(sscanf(parameters[1],"%ho",&temp_mask) != 1 || temp_mask>0777){
                printf("myshell umask : bad argument\n");
                return 1;
            }
            //如果合理，设置umaks值
            umask(temp_mask);
        }
        return 1;
    }

    //echo  命令
    else if(strcmp(command,"echo")==0)
    {
        int out_fd;
        if(indicator & OUT_REDIRECT)//如果存在重定向
        {
            out_fd = open(output_file_name, O_WRONLY|O_TRUNC, 0666);
            if(out_fd<0){
                printf("myshell error in dir:bad redirect arguemnt!\n");
                return 1;
            }
        }
        else{
            out_fd = 1;
        }
        int temp_cnt=0;

        //输出echo命令的参数
        char temp_str[MAX_PATH_LENGTH];
        for(int i=1;i<parameter_number;i++){
            if(parameters[i]==NULL)
                break;
            strcpy(temp_str,parameters[i]);
            strcat(temp_str," ");
            write(out_fd,temp_str,strlen(temp_str));
        }
        strcpy(temp_str,"\n");
        write(out_fd,temp_str,strlen(temp_str));
        return 1;
    }

    /* 内建命令：bg
    ** 功能：把挂起程序放到后台运行
    ** 参数：pid_number
    ** 内部逻辑：
    ** 1.判断参数是否为空，如果为空则提示错误信息
    ** 2.如果不为空，判断是否合法
    ** 3.执行bg命令
    */
    else if(strcmp(command,"bg")==0) 
    {
        pid_t ppid;
        if(parameter_number == 1) //没有要bg的pid参数读入
        {
            printf("myshell error in bg:two few arguments!\n");
        }
        ppid = atoi(parameters[1]);
        int status;
        if (kill(ppid, SIGCONT) < 0)
        {
            printf("myshell error in bg: job not found: %d\n", ppid);
        }
        else 
        {
            waitpid(ppid, &status, WUNTRACED);
        }
        
        return 1;
    }
    /* 内建命令：fg
    ** 功能：把后台程序放到前台执行
    ** 参数：pid_number
    ** 内部逻辑：
    ** 1.判断参数是否为空，如果为空则提示错误信息
    ** 2.如果不为空，判断是否合法
    ** 3.执行fg命令
    */
    else if(strcmp(command,"fg")==0) 
    {
        pid_t ppid;
        if(parameter_number == 1)//没有要fg的pid参数读入
        {
            printf("myshell error in fg:two few arguments!\n");
        }
        ppid = atoi(parameters[1]);

        setpgid(ppid, ppid);
        int status;
        if (tcsetpgrp(1, getpgid(ppid)) == 0) {
            kill(ppid, SIGCONT);        /* sucess */
            waitpid(ppid, &status, WUNTRACED);
        }
        else 
            printf("fg: job not found: %d\n", ppid);
        
        return 1;
    }

    /* 内建命令：exec
    ** 功能：执行命令
    ** 参数：命令和对应参数
    ** 内部逻辑：
    ** 1.开启管道
    ** 2.把exec 的命令和参数重定向到标准输入
    ** 3.通过标准输入来执行
    */
    else if(strcmp(command,"exec") == 0) 
    {
        IN_EXEC=1;
        int file_pipes[2];
        if(pipe(file_pipes)<0)
        {
            printf("myshell error:pipe failed.\n");
            exit(0);
        }

        pid_t pid=fork();
        
        if( pid < 0){
            printf("myshell error: fork error!\n");
        }
        if( pid == 0){
            //exec的命令写入管道，重定向到标准输入
            close(file_pipes[0]);  
            for(int i=1;i<parameter_number;i++)
            {
                write(file_pipes[1], parameters[i], strlen(parameters[i]));
                write(file_pipes[1], " ", strlen(" "));  
            }
            close(file_pipes[1]); 
            exit(0);
        }
        if( pid > 0){
            waitpid(pid,NULL,0);
            close(0); 
            dup(file_pipes[0]);  
            close(file_pipes[1]);
            close(file_pipes[0]);

            return 1;
        }


        return 1;
    }

    /* 内建命令：jobs
    ** 功能：显示后台进程
    ** 参数：无
    ** 内部逻辑：无
    */
    else if(strcmp(command,"jobs")==0) 
    {
        if(parameter_number != 1)
        {
            printf("myshell error in jobs:two many arguments!\n");
            return 1;
        }
        int temp_cnt=0;
        for(int i=0;i<MAX_PID_NUMBER;i++)
        {
            if(background_process_table[i] != 0) //only handler the background processes
            {
                printf("[%d]BG:process %d\n",temp_cnt++,background_process_table[i]);
            }
        }
        
        return 1;
    }


    /* 内建命令：set
    ** 功能：设置环境变量
    ** 参数：环境变量和值
    ** 内部逻辑：无
    */
    else if(strcmp(command,"set")==0) 
    {
        if(parameter_number == 1)
        {
            printf("myshell error in jobs:two few arguments!\n");
            return 1;
        }
        char var_name[1024];
        char env_name[1024];
        strcpy(var_name,parameters[1]);
        strcpy(env_name,parameters[3]);

        if(setenv(var_name ,env_name,1) != 0){
		    perror("myshell error : SET ENV ERROR");
	    }
        
        return 1;
    }

    /* 内建命令：unset
    ** 功能：取消环境变量
    ** 参数：环境变量
    ** 内部逻辑：无
    */
    else if(strcmp(command,"unset")==0) 
    {
        if(parameter_number == 1)
        {
            printf("myshell error in jobs:two few arguments!\n");
            return 1;
        }
        char var_name[1024];
        strcpy(var_name,parameters[1]);

        if(unsetenv(var_name) != 0){
		    perror("myshell error : UNSET ENV ERROR");
	    }
        
        return 1;
    }
    return 0;
}