#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


void collatz(int nr){
    
    printf("%d: ", nr);
    while(nr>0 && nr!=1)
    {
        
        if(nr % 2 == 0)
            nr = nr/2;
        else
            nr = 3 * nr + 1;
        printf("%d ", nr);

    }    
}

int main(int argc, const char *argv []){
    
    
        int nr;
        for(int i = 1; i<argc; i++){
            pid_t pid = fork();
            if (pid < 0)
                return errno;
            else if (pid ==0)
            {
                nr = atoi(argv[i]);
                collatz(nr);
                printf("\n");
                exit(0);
                
            }
            // else{
                // wait(NULL);
                // printf("Done parent %d, Me: %d \n", getpid(), pid);
            // }   
        }
        for(int i = 1; i<argc; i++){
            wait(NULL);
            printf("Done parent %d, Me: %d \n", getppid(), getpid());
        }
        
}