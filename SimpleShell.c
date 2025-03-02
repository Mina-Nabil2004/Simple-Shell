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
bool foreground = true;

void execute_command(char **args){
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
    else if(!foreground){
        foreground = true;
    }
}

void export(char command[], Variable *newVariable){
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
    int len = strlen(newVariable->value);
    while (len > 0 && newVariable->value[len - 1] == ' ') {
        newVariable->value[--len] = '\0';
    }
    variables[nextVariable++] = newVariable;
}

void echo(char command[]){
    char output[MAX_INPUT];
    bool flag = false;
    int j = 0;

    for (int i = 5; command[i] != '\0'; i++) {
        if(command[i]=='"')continue;
        output[j++] = command[i]; 
    }
    output[j] = '\0'; 
    printf("%s\n", output);
}

char* evaluate(char *token){
    static char temp[MAX_INPUT]; 

    for (int i = 0; i < nextVariable; i++) {
        sprintf(temp, "$%s", variables[i]->name);
        if (strcmp(token, temp) == 0) {
            return variables[i]->value;
        }
        snprintf(temp, sizeof(temp), "\"$%s\"", variables[i]->name);
        if (strcmp(token, temp) == 0) {
            return variables[i]->value;
        }
        snprintf(temp, sizeof(temp), "\"$%s", variables[i]->name);
        if (strcmp(token, temp) == 0) {
            return variables[i]->value;
        }
        snprintf(temp, sizeof(temp), "$%s\"", variables[i]->name);
        if (strcmp(token, temp) == 0) {
            return variables[i]->value;
        }
    }
    return " ";
}

void parse(char command[], char *args[]){
    int i = 0;
    char *command_copy = strdup(command);
    char *token = strtok(command_copy, " ");
    char updated_command[MAX_INPUT];
    updated_command[0] = '\0';

    while (token != NULL && i < MAX_ARGS-1) {
        if(token[0] == '$' || token[1] == '$') token = evaluate(token);
        if (strcmp(token, "&") == 0) {
            foreground = false;
            token = "";
        }
        else args[i++] = token;
        strcat(updated_command, token);
        strcat(updated_command, " "); 
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    strcpy(command, updated_command);
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
            echo(command);
        }
        else if(strcmp(args[0], "cd") == 0){
            chdir(args[1]);
        }
        else if(strcmp(args[0], "exit") == 0){
            exit(0);
        }else {
            execute_command(args);
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