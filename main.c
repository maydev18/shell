#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "shell.h"
#include "utils.h"
#include "colors.h"
#include <unistd.h>

/**
 * @file main.c
 * @brief This file contains the main driver code to run the entire shell
 */

 
/**
 * @brief A handler function to handle ctrl+c signal
 * 
 * 
 * @param sig     The signal
 */
void handle_sigint(int sig) {
	// write 1 byte(\n) to stdout
    write(STDOUT_FILENO, "\n", 1);
}

/**
 * @brief The driver function, which runs an infinite loop, to take input from the user and pass the input to the `run_command()` function
 * 
 * This function takes the user input, sanitizes it and then run the command according to the requirements.
 * 
 * @param argc		Number of command line parameters passed + 1
 * @param argv		Array of strings where argv[0] = file name and rest all argv[1] ...... argv[argc-1] are the arguments passed from command line
 * 
 */
int main(int argc , char* argv[]){
	//sigint = ctrl + c
	signal(SIGINT, handle_sigint);  // Catch Ctrl+C

	//stores the input command entered by the user
	char* input = NULL;
	size_t len = 0;

	//To store cwd
	char buffer[BUFSIZ]; //BUFSIZ = 1024
	Getcwd(buffer , BUFSIZ);
	while(1){
		printf(COLOR_BLUE "MyShell" COLOR_RESET ":" COLOR_GREEN "%s" COLOR_RESET " $> ", buffer);

		//dynamically allocates memory to input, if input was pointing to NULL, it will allocate memory, but if it was already pointing to some buffer it will reallocate it if needed
		int char_read = getline(&input , &len , stdin);
		if(char_read < 0){
			perror(COLOR_RED "cannot read input");
			exit(EXIT_FAILURE);
		}
		run_command(input);
	}
	free(input);
	return 0;
}

