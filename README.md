# MyShell

This is a mini shell implementation in C.
## Usage
    run make in the folder and run the executable program to use MyShell
## Features
    1. implemented built-in command like `cd` `exit` `fg` `bg` `umask` `set`
    2. support pipe redirection
    3. support `fg` to run a program in fore ground
    4. support `bg` to run something backgroud
    5. change the color of prompt
    6. read from files
    7. support I/O direction
## Mannual

### basic functions of myshell：
    1. use `ls`  to list directory contents of files and directories.
    what will be displayed when type in 'ls -al' in my computer
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

    2. use `pwd` to get the current woking directory
    
    3. use 'time' to get current time
    In my computer, when type in time
    if will display: "2017/08/04 14:36:57 Friday"
    
    4. use 'cd' to change directory
    
    5. use makedir to create a directory 
    
    6. run other bash programs in shell use commands like './script.sh' to run a program
    or even use './myshell' to run myshell program inside a shell program, and use quit to quit current shell program
    
    more info about the usage of commands can be found by using man "command name" like 'man ls' to find 
    
    8. use '>' and '<' to use the I/O direct feature and use '<<' to '>>' to append
    
    9. use '|' to use pipe feature
    
    10. use keyword 'fg' and 'bg' or '&' to run a program in the current shell or run it in background
    
## Miscellaneous
    This is a Course Project for the course <Linux Programming> in Zhejiang University in July 2017
