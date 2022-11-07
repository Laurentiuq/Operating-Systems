#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){

    pid_t pid = fork();
    if (pid < 0)
        return errno;
    else if (pid == 0)
    {
        // printf("Child PID: %s", getpid());
        
        char *argv[] = {"ls", NULL};
        execve("/bin/ls", argv, NULL);
        // perror(NULL);
    }
    else{
        wait(NULL);
        printf("Parent PID: %d\nChild PID: %d\n", pid, getpid());
    }
}