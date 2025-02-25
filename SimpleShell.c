#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

void main(){
    signal(SIGINT, on_child_exit);
    setup_environment();
    shell();
}

void on_child_exit(){
    reap_child_zombie()
    write_to_log_file("Child terminated")
}


void setup_environment(){
    chdir("/");
}

void shell(){
    do{
        char input[20];
        scanf("%s", &input);
        evaluate_expression():
        switch(input_type):
            case shell_builtin:
                execute_shell_bultin();
            case executable_or_error:
                execute_command():

    }
    while 1;
}

void execute_shell_bultin(){
    switch(command_type):
        case cd:
        case echo:
        case export:
}


void execute_command(char command[], bool foreground){
    pid_t pid = fork();
    if(pid == 0){
        execvp(command,NULL);
        print("Error");
        exit();
    }
    else if(foreground){
        waitpid(pid);
    }
}