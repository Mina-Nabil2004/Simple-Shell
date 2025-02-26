#include <sys/wait.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
//////////

// void on_child_exit(){
//     reap_child_zombie()
//     write_to_log_file("Child terminated")
// }

void setup_environment(){
    chdir("/");
}

void execute_command(char *args[], bool foreground){
    pid_t pid = fork();
    if(pid == 0){
        execvp(args[0], args);
        printf("Error");
        exit(0);
    }
    else if(foreground){
        waitpid(pid, NULL, -1);
    }
}

void shell(){
    do{
        char command[100];
        int i = 0;
        char ch;
        while (i < 99) {
            scanf("%c", &ch);  
            if (ch == '\n') { 
                break;
            }
            command[i++] = ch;
        }
        command[i] = '\0';
        char *args[10];
        i = 0;
        char *token = strtok(command, " ");
        
        while (token != NULL && i < 9) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        switch(args[0]):
            case "cd":
                execute_shell_bultin(args);
            case "echo":
                execute_shell_bultin(args);
            case "export":
                execute_shell_bultin(args);
            default:
                execute_command(args):

    }
    while(true);
}


void main(){
    //signal(SIGINT, on_child_exit);
    setup_environment();
    shell();
}