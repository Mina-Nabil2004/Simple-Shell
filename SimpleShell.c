#include <sys/wait.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#define MAX_INPUT 100
#define MAX_VARIABLES 100
#define MAX_ARGS 10


typedef struct {
    char name[MAX_INPUT/2 + 1];
    char value[MAX_INPUT/2 + 1];
} Variable;

Variable *variables[MAX_VARIABLES];
int nextVariable = 0;

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
        waitpid(pid, NULL, 0);
    }
}

void export(char command[], Variable *newVariable){
    int i;
    bool flag = false;
    newVariable->name[0] = '\0'; 
    newVariable->value[0] = '\0';

    for (int i = 7; i < MAX_INPUT; i++) { 
        if (command[i] == '\0') break;  
        if (command[i] == '=' && !flag) {
            flag = true;
            continue;
        }
        if (command[i] == '"') {
            continue; 
        }
        if (!flag) {
            strncat(newVariable->name, &command[i], 1); 
        } else{
            strncat(newVariable->value, &command[i], 1); 
        }
    }

    variables[nextVariable++] = newVariable; 
    printf("Stored Variable: Name: %s, Value: %s\n", newVariable->name, newVariable->value);
}

void echo(char **args){
    
}

void parse(char command[], char *args[]){
    int i = 0;
    char *command_copy = strdup(command);
    char *token = strtok(command_copy, " ");
    
    while (token != NULL && i < MAX_ARGS-1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
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
        parse(command, args);
        
        if (strcmp(args[0], "export") == 0) {
            Variable newVariable;
            export(command, &newVariable);
        }
        else if(strcmp(args[0], "echo") == 0){
            echo(args);
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

void on_child_exit(int signo){
    if(signo == SIGCHLD){
        pid_t pid = waitpid(-1, NULL, WNOHANG);
    }
}

void setup_environment(){
    chdir("/");
}

void main(){
    signal(SIGCHLD, on_child_exit);
    setup_environment();
    shell();
}