#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
    int fd[2];
    pipe(fd);
    int pid = fork();

    if(pid  == 0){//Child
        int input = open("file1.txt", O_RDONLY);

        dup2(input, STDIN_FILENO);
        dup2(fd[1], STDOUT_FILENO);

        close(fd[0]);
        close(fd[1]);

        execlp("sort", "sort", NULL);
        perror("excelp sort");

        exit(1);
    }else if(pid > 0){
        int output = open("file2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

        dup2(fd[0], STDIN_FILENO);
        dup2(output, STDOUT_FILENO);

        close(fd[1]);
        close(fd[0]);

        execlp("uniq", "uniq", NULL);
        perror("excel uniq");

        exit(1);
    }else{
        perror("fork failed");
        return 1;
    }
    return 0;
}