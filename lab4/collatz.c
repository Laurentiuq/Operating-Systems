#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


// functia care calculeaza collatz
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
    
    // cream un proces copul care apeleaza functia
    pid_t pid = fork();
    if (pid < 0)
        return errno;
    else if (pid == 0)
    {
        int nr;
        nr = atoi(argv[1]);
        // apelam functie pentru numarul dat ca parametru
        collatz(nr);
        printf("\n");
    }
    // asteptam sa se execute procesul copil
    else{
        wait(NULL);
        printf("Child process %d ended", pid);
    }
}