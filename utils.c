#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

int is_space(char c){
    return (c == ' ' || c == '\t' || c == '\n');
}
void Getcwd(char* buffer , size_t size){
    if(getcwd(buffer , size) == NULL){
        perror("CWD");
        exit(0);
    }
}
void trim(char* input){
    char* start = input;
    char* end;

    while(is_space(*start)){
        start++;
    }
    //all white string
    if(*start == 0){
        input[0] = '\0';
        return;
    }
    //move end to last non non-whitespace character
    end = start + strlen(start) - 1;
    while(end > start && is_space(*end)){
        end--;
    }
    *(end+1) = '\0';
    //move the string to beginning
    if(start != input){
        int i = 0;
        while(start <= end){
            input[i++] = *start++;
        }
        input[i] = '\0';
    }
}
void tokenize(char* input , char* args[]){
    char tmp[100];
    int i = 0 , j = 0 , k = 0;
    while(input[j] != '\0'){
        if(is_space(input[j])){
            if(i > 0){
                tmp[i] = '\0';
                args[k] = (char*)malloc((i+1));
                strcpy(args[k] , tmp);
                k++;
                i = 0;
            }
        }
        else{
            tmp[i] = input[j];
            i++;
        }
        j++;
    }
    if(i > 0){
        tmp[i] = '\0';
        args[k] = (char*)malloc((i+1));
        strcpy(args[k] , tmp);
        k++;
    }
    args[k] = NULL;
}
void free_mem(char* args[]){
    int j = 0;
    while(args[j]){
        free(args[j]);
        j++;
    }
}