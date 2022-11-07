#include<stdlib.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/wait.h>


int main(int argc, char* argv[]){
    char *shm_name = "collatz";
    int shm_fd;
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if(shm_fd < 0){
        perror(NULL);
        return errno;
    }
        
    int shm_size = argc * 4096;

    int ftrunc = ftruncate(shm_fd, shm_size);
    if(ftrunc ==  -1){
        perror("Eroare ftrunc\n");
        shm_unlink(shm_name);
        return errno;
    }
    printf("Parinte start: %d\n", getpid());

    for(int nr=1; nr<argc; ++nr){
        pid_t pid = fork();
        
        if(pid < 0){
            printf("dfs");
            perror("Fork error");
            return errno;
        }
        else if(pid ==0){
            char * shm_ptr = mmap(NULL, 4096, PROT_WRITE, MAP_SHARED, shm_fd, (nr-1) * 4096);
            if(shm_ptr == MAP_FAILED){
                perror(NULL);
                shm_unlink(shm_name);
                return errno;
            }
        
            int currentNr = atoi(argv[nr]);
            shm_ptr += sprintf(shm_ptr, "Numarul actual %d: ", currentNr);
            shm_ptr += sprintf(shm_ptr, "%d ", currentNr);
            while(currentNr != 1){
                if(currentNr % 2 == 0)
                    currentNr = currentNr/2;
                else
                    currentNr = 3 * currentNr + 1;
                shm_ptr += sprintf(shm_ptr, "%d ", currentNr);
        
            }
            // shm_ptr += sprintf(shm_ptr, "%d ", currentNr);
            shm_ptr += sprintf(shm_ptr, "\n");
            printf("Copil: %d, Parinte: %d\n", getpid(), getppid());
            munmap(shm_ptr, 4096);
            exit(0);
        }
    }

    for(int nr=1; nr<argc; nr++){
        wait(NULL);
    }

    for(int nr = 1; nr < argc; nr++){
        char* shm_ptr = mmap(NULL, 4096, PROT_READ, MAP_SHARED, shm_fd, (nr-1) * 4096);
        if(shm_ptr == MAP_FAILED){
                perror("eroare");
                shm_unlink(shm_name);
                return errno;
            }

        printf("%s", shm_ptr);
        munmap(shm_ptr, 4096);
    }
    printf("Stop parinte: %d\n", getppid());
    shm_unlink(shm_name);
    return 0;
}