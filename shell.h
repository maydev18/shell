#ifndef SHELL_H
#define SHELL_H
#define MAX_ARGS 200
#define MAX_CHILD_PROCESS 100
/**
 * @file shell.h
 * @brief This file contains the declarations of functions for implementing the custom shell.
 */

/**
 * @brief Runs the command being provided to it by the user.
 * 
 * This function takes the user input, sanitizes it and then run the command according to the requirements.
 * 
 * @param input     raw user input
 */
void run_command(char* input);

/**
 * @brief It tells the current working directory
 * @param buffer    to hold the current working directory string
 * @param size      tells the size of the buffer (max = 1024)
 * 
 * @note Modifies buffer to point to the cwd
 */

void Getcwd(char* buffer , size_t size);

#endif