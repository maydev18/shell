#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "shell.h"
#include "utils.h"
#define MAX_ARGS 64
int create_command_for_pipeline(char* args[] , char* cmnd[] , int current_arg){
    int j = 0;
    while(args[current_arg]){
        if(strcmp(args[current_arg] , "|") == 0) break; 
        cmnd[j++] = args[current_arg++];
    }
    cmnd[j] = NULL;
    if(args[current_arg] == NULL) return current_arg;
    current_arg++;
    return current_arg;
}
void pipe_command_handler(char* args[]){
    pid_t pid[100];
    int current_arg = 0 , current_process = 0 , inputfd = 0;

    while(args[current_arg] != NULL){
        char* command[MAX_ARGS];
        current_arg = create_command_for_pipeline(args , command , current_arg);
        int j = 0;
        int fd[2];
        if(pipe(fd) < 0){
            perror("Error creating pipe");
            exit(EXIT_FAILURE);
        }
        pid_t p = fork();
        if(p == 0){
            if(inputfd != 0){
                dup2(inputfd , STDIN_FILENO);
                close(inputfd);
            }
            if(args[current_arg] != NULL){
                dup2(fd[1] , STDOUT_FILENO);
            }
            close(fd[0]);
            close(fd[1]);
            execvp(command[0] , command);
            perror("error in running command");
            exit(EXIT_FAILURE);
        }
        else if(p > 0){
            pid[current_process++] = p;
            close(fd[1]);
            if(inputfd != 0) close(inputfd);
            inputfd = fd[0];
        }
        else{
            perror("Error creating process");
            exit(EXIT_FAILURE);
        }
    } 
    for(int i = 0 ; i < current_process ; i++){
        int status;
        waitpid(pid[i] , &status , 0);
    }
}
void run_command(char* input){
    char* tokens[MAX_ARGS];
    //sanitize the input
    trim(input);
    if(strlen(input) == 0) return;
    tokenize(input , tokens);
    char* args[MAX_ARGS];
    int input_redirect = 0 , output_redirect = 0 , is_background = 0 , pipeline = 0 , j = 0;
    char* input_file = NULL , *output_file = NULL;
    for(int i = 0  ; tokens[i] != NULL ; i++){
        if(strcmp(tokens[i] , ">") == 0){
            output_redirect = 1;
            output_file = tokens[++i];
        }
        else if(strcmp(tokens[i] , "<") == 0){
            input_redirect = 1;
            input_file = tokens[++i];
        }
        else if(strcmp(tokens[i] , "&") == 0){
            is_background = 1;
        }
        else if(strcmp(tokens[i] , "|") == 0){
            pipeline = 1;
            args[j++] = tokens[i];
        }
        else{
            args[j++] = tokens[i];
        }
    }
    args[j] = NULL;
    if(pipeline){
        pipe_command_handler(args);
        free_mem(args);
        return;
    }
    pid_t pid = fork();
    int status;
    if(pid == 0){
        //child process
        if(input_redirect && input_file){
            int fd = open(input_file, O_RDONLY);
            if (fd < 0) {
                perror("Input file error\n");
                exit(1);
            }
            //make stdin point to the file descriptor, Disconnect the keyboard, and connect this file instead as the input source.
            dup2(fd, STDIN_FILENO);
            close(fd);
        } 
        if(output_redirect && output_file){
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            //O_WRONLY = write only, O_CREAT = create if doesnot exist, O_TRUNC = clear the contents, 0644 = permissions (6 = R+W for owner, 4 = read for grp, 4 = read for others)
            if (fd < 0) {
                perror("Output file error\n");
                exit(1);
            }
            //make stdout point to the file descriptor, "Disconnect the monitor, and connect this file instead as the output source."
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execvp(args[0] , args);
        perror("error\n");
        exit(1);
    }

    else if(pid > 0){
        //parent process;
        if(!is_background){
            //foreground process
            waitpid(pid, NULL, 0);
        }
        else{
            //background process
            printf("[Background pid %d]\n", pid);
        }
    }
    else{
        perror("fork failed");
    }
    free_mem(args);
}
