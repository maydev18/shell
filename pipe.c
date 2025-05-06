#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
int main(){
    // fd[0] = read end, fd[1] = write end
    int fd1[2];
    int fd2[2];
    pid_t pid;
    char par[100] = "Hello from parent";
    char child[100] = "Hello from child";
    if(pipe(fd1) < 0 || pipe(fd2) < 0){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if(pid < 0){
        perror("Fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){
        char read_msg_child[100];
        close(fd1[1]); // closes the write end
        read(fd1[0] , read_msg_child , sizeof(read_msg_child));
        int fd = open("child.txt" , O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd , 1);
        printf("%s\n" , read_msg_child);
        close(fd);
        close(fd1[0]); // closes the read end
        close(fd2[0]);
        write(fd2[1] , child , strlen(child)+1);
        close(fd2[1]);
    }
    else{
        close(fd1[0]); // closes the read end
        write(fd1[1] , par , strlen(par)+1);
        close(fd1[1]); // closes the write end
        close(fd2[1]);
        char read_msg_par[100];
        read(fd2[0] , read_msg_par , sizeof(read_msg_par));
        int fd = open("parent.txt" , O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd , 1);
        printf("%s\n" , read_msg_par);
        close(fd);
        close(fd2[0]);
        wait(NULL);
    }
    return 0;
}