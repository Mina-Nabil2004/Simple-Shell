#include <sys/wait.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#define MAX_INPUT 100
#define MAX_ARGS 10

void execute_command(char **args, bool foreground){
    pid_t pid = fork();
    if(pid < 0){
        printf("Error");
        perror("fork");
        exit(1);
    }
    else if(pid == 0){
        execvp(args[0], args);
        printf("Error");
        perror("execvp");
        exit(1);
    }
    else if(foreground){
        waitpid(pid, NULL, -1);
    }
}

void shell(){
    do{
        char command[MAX_INPUT];
        int i = 0;
        char ch;
        while (i < MAX_INPUT-1) {
            scanf("%c", &ch);  
            if (ch == '\n') { 
                if(i == 0) continue;
                break;
            }
            command[i++] = ch;
        }
        command[i] = '\0';
        char *args[MAX_ARGS];
        i = 0;
        char *token = strtok(command, " ");
        
        while (token != NULL && i < MAX_ARGS-1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (strcmp(args[0], "echo") == 0 || 
            strcmp(args[0], "export") == 0) {
            //execute_shell_builtin(args);
        }
        else if(strcmp(args[0], "cd") == 0){
            chdir(args[1]);
        }
        else if(strcmp(args[0], "exit") == 0){
            exit(0);
        }else {
            execute_command(args, true);
        }
    }
    while(true);
}

void on_child_exit(){
    waitpid(-1, NULL, 0);
    printf("Child terminated");
}

void setup_environment(){
    chdir("/");
}

void main(){
    signal(SIGCHLD, on_child_exit);
    setup_environment();
    shell();
}