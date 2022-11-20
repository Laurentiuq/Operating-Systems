#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){

    // cream un proces copil
    pid_t pid = fork();
    if (pid < 0)
        return errno;
    else if (pid == 0)
    {
        // printf("Child PID: %s", getpid());
        
        char *argv[] = {"ls", NULL};
        // afisam fisierele din bin
        execve("/bin/ls", argv, NULL);
    }
    // asteptam sa se execute copilul
    else{
        printf("My PID: %d\nChild PID: %d\n", getppid(), pid);
        wait(NULL);
    }
}