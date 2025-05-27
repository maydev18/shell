#ifndef UTILS_H
#define UTILS_H
#define MAX_TOKEN_LEN 200
/**
 * @file utils.h
 * @brief Contains all utility functions to help shell operations 
 *
 */

/**
 * @brief It tells the current working directory
 * @param buffer    to hold the current working directory string
 * @param size      tells the size of the buffer (max = 1024)
 * 
 * @note Modifies buffer to point to the cwd
 */

void Getcwd(char* buffer , size_t size);

/**
 * @brief It tells whether the given character is a space/newline/tab
 * 
 * @param c    Character to check for space/newline/tab
 * 
 * @returns a boolean value, 0 -> not a space, 1 -> yes a space/tab/newline
 */
int is_space(char c);

/**
 * @brief It removes all types of white spaces and newline characters from the beginning and ending of the given string.
 * 
 * @param       input     the raw string
 * 
 * @note    Modifies the input string such that it is completely sanitized now
 */

void trim(char* input);

/**
 * @brief It generates token from the given input string and stores them in args;
 * 
 * @param input     The given input string
 * @param args      Array of tokens generated
 * @note Modifies the args input such that it contains an array to tokens now
 */

void tokenize(char* input , char* args[]);

/**
 * @brief It frees up memory taken up by args
 * 
 * @param args      The given array of tokens which was dynamically generated
 * 
 * @note Modifies the args input such that all array values are NULL, (resolves dangling pointers)
 */
void free_mem(char* args[]);

#endif