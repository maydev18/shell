#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "utils.h"
int main(int argc , char* argv[]){
	char* input = NULL;
	size_t len = 0;
	char buffer[BUFSIZ]; //BUFSIZ = 1024
	Getcwd(buffer , BUFSIZ);
	while(1){
		printf("%s %s" , buffer , "/ $>");
		int char_read = getline(&input , &len , stdin);
		if(char_read == -1){
			perror("getline");
			break;
		}
		run_command(input);
	}
	free(input);
	return 0;
}

