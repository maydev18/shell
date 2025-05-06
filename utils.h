#ifndef UTILS_H
#define UTILS_H

// Add utility function declarations here
int is_space(char c);
void Getcwd(char* buffer , size_t size);
void trim(char* input);
void tokenize(char* input , char* args[]);
void free_mem(char* args[]);
#endif