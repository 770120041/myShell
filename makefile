#姓名：刘哲
#学号：3150103520
myshell: myshell.c  builtin_command.c builtin_command.h header.h parse_command.c parse_command.h  get_command.c get_command.h signal_function.c signal_function.h
	gcc -o myshell myshell.c builtin_command.c parse_command.c get_command.c signal_function.c