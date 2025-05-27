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

#endif