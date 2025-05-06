#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "shell.h"
#include "utils.h"
#include "colors.h"

/**
 * @brief Creates a command for the pipeline operator (`|`) from the provided arguments.
 * 
 * This function processes the arguments and separates a command to be executed 
 * in a pipeline. It stores the individual command in the `cmnd[]` array.
 * 
 * @param args       The array of arguments (sanitized input from stdin).
 * @param cmnd       The array to store a single command extracted from `args[]`.
 * @param current_arg The current index in `args[]` being processed.
 * 
 * @return The updated value of `current_arg` to be used for the next command.
 * 
 * @note This function modifies the `cmnd[]` array to store the extracted command.
 */
int create_command_for_pipe(char* args[], char* cmnd[], int current_arg){

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
/**
 * @brief Executes the command being made by `create_command_for_pipe()` 
 * 
 * This function extracts the next command to be run from `args` and then fork a process to execute that command.
 * As we are using this function for `pipe(|)`, hence the output from the prevous command is passed as an input to the last command, except two exceptions  * the first command's input remains `stdin` and the last command's output remains `stdout`.
 * `pipe()` system call is being used to create pipes between 2 processes to read and write from.
 * 
 * @param args       The array of arguments (sanitized input from stdin).
 */
void pipe_command_handler(char* args[]){
    //Array to store pids of child processes to finally call wait and to release all the resouces occupied by them. It prevents the child processes from becoming zombies.
    pid_t pid[MAX_CHILD_PROCESS];
    int current_arg = 0 , current_process = 0 , inputfd = 0; //initially inputfd points to stdin(0)

    while(args[current_arg] != NULL){
        char* command[MAX_ARGS];

        //creating current command
        current_arg = create_command_for_pipe(args , command , current_arg);

        int fd[2];
        //fd[0] = read file descriptor , fd[1] = write file descriptor
        if(pipe(fd) < 0){
            perror(COLOR_RED "Error creating pipe");
            exit(EXIT_FAILURE);
        }
        pid_t p = fork();
        if(p == 0){
            //child process
            if(inputfd != 0){
                //disconnect keyboard from stdin and use inputfd (input from the previous command)
                dup2(inputfd , STDIN_FILENO);
                close(inputfd);
            }
            if(args[current_arg] != NULL){
                //disconnect terminal from stdout and use fd[1] (Will become input to the next command)
                dup2(fd[1] , STDOUT_FILENO);
            }
            //close all file descriptors
            close(fd[0]);
            close(fd[1]);
            //execute the command
            execvp(command[0] , command);
            perror(COLOR_RED "error in running command");
            exit(EXIT_FAILURE);
        }
        else if(p > 0){
            //parent process
            //store the pid of the child process
            pid[current_process++] = p;
            //close all the file descriptors of the parent, and make the inputfd hold the value of fd[0](read location)
            close(fd[1]);
            if(inputfd != 0) close(inputfd);
            inputfd = fd[0];
            //Parent must not close fd[0] as it serves input to the next child
        }
        else{
            //error creating process
            perror(COLOR_RED "Error creating process");
            exit(EXIT_FAILURE);
        }
    } 
    //Calling wait for all child processes
    for(int i = 0 ; i < current_process ; i++){
        int status;
        waitpid(pid[i] , &status , 0);
    }
}
/**
 * @brief Takes the raw command entered by the user and runs it.
 * 
 * This functions accepts the command being entered by the user and then it first sanitizes it and then tokenize it and then after it checks for presence of * any redirection or pipe or background process and handles different conditions accordingly.
 * 
 * @param args       The raw string of command entered by the user
 * 
 * 
 * @note This function modifies the `input`
 */
void run_command(char* input){
    char* tokens[MAX_ARGS];
    //sanitize the input
    trim(input);
    if(strlen(input) == 0) return;
    //tokenize the input
    tokenize(input , tokens);

    //array to store the arguments from tokens
    char* args[MAX_ARGS];

    //different flags to check for different conditions
    int input_redirect = 0 , output_redirect = 0 , is_background = 0 , pipeline = 0 , j = 0;

    //pointers to input and output file if redirection is used
    char* input_file = NULL , *output_file = NULL;
    for(int i = 0  ; tokens[i] != NULL ; i++){
        //output redirection
        if(strcmp(tokens[i] , ">") == 0){
            output_redirect = 1;
            output_file = tokens[++i];
        }
        //input redirection
        else if(strcmp(tokens[i] , "<") == 0){
            input_redirect = 1;
            input_file = tokens[++i];
        }
        // Background process
        else if(strcmp(tokens[i] , "&") == 0){
            is_background = 1;
        }
        //Pipe
        else if(strcmp(tokens[i] , "|") == 0){
            pipeline = 1;
            args[j++] = tokens[i];
        }
        //normal arguments
        else{
            args[j++] = tokens[i];
        }
    }
    //terminate the args
    args[j] = NULL;

    //execute pipe and return
    if(pipeline){
        pipe_command_handler(args);
        free_mem(args);
        return;
    }
    pid_t pid = fork();
    if(pid == 0){
        //child process

        //handle input redirect
        if(input_redirect && input_file){
            int fd = open(input_file, O_RDONLY);
            if (fd < 0) {
                perror(COLOR_RED "Input file error\n");
                exit(1);
            }
            //make stdin point to the file descriptor, Disconnect the keyboard, and connect this file instead as the input source.
            dup2(fd, STDIN_FILENO);
            close(fd);
        } 
        //handle output redirect
        if(output_redirect && output_file){
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            //O_WRONLY = write only, O_CREAT = create if doesnot exist, O_TRUNC = clear the contents, 0644 = permissions (6 = R+W for owner, 4 = read for grp, 4 = read for others)
            if (fd < 0) {
                perror(COLOR_RED "Output file error\n");
                exit(1);
            }
            //make stdout point to the file descriptor, "Disconnect the monitor, and connect this file instead as the output source."
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        //Load the command present in args into child process image
        execvp(args[0] , args);
        perror(COLOR_RED "error\n");
        exit(1);
    }

    else if(pid > 0){
        //parent process;

        //if a foreground process wait for it to finish
        if(!is_background){
            //foreground process
            waitpid(pid, NULL, 0);
        }
        //Dont wait for a background process
        else{
            //background process
            printf(COLOR_GREEN "[Background pid %d]\n", pid);
        }
    }
    else{
        perror(COLOR_RED "fork failed");
    }

    //Free the memory allocated to args
    free_mem(args);
}
