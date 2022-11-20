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
    // cream zona de memorie partajata
    // obiect cu numele "collatz"
    char *shm_name = "collatz";
    // descripotrul asociat
    int shm_fd;
    // daca nu exista este creat, daca exista este deschis pentru read/write cu drepturi de read/write pentru utilizator care a creat obiectul
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if(shm_fd < 0){
        perror(NULL);
        return errno;
    }

    // dimensiunea obiectului - numarul de argumente * 4096 de bytes        
    int shm_size = argc * 4096;

    // mareste/micosreasa obiectul asociat descriptorului
    int ftrunc = ftruncate(shm_fd, shm_size);
    if(ftrunc ==  -1){
        perror("Eroare ftrunc\n");
        shm_unlink(shm_name);
        return errno;
    }
    printf("Starting parent: %d\n", getpid());

    for(int nr=1; nr<argc; ++nr){
        // cream un copil pentru fiecare numar
        pid_t pid = fork();
        
        if(pid < 0){
            perror("Fork error");
            return errno;
        }
        else if(pid == 0){
            // incarcam memoria partajata in spatiul procesului 
            // adresa la care sa fie incarcata in proces(NULL = decide kernelul)
            // dimensiunea memoriei incarcate
            // drepturile de acces(scriere in acest caz)
            // tipul de memorie(partajata cu restul proceselor in acest caz)
            // descriptorul obiectului de memorie
            // locul in obiectul de memorie de la care sa fie incarcat in spatiul procesului (numarul parametrului la care ne aflam)
            char * shm_ptr = mmap(NULL, 4096, PROT_WRITE, MAP_SHARED, shm_fd, (nr-1) * 4096);

            // daca esueaza stergem obiectul
            if(shm_ptr == MAP_FAILED){
                perror(NULL);
                shm_unlink(shm_name);
                return errno;
            }
        
            int currentNr = atoi(argv[nr]);
            // scrie numerele calculate conform Collatz in zona de memorie
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
            printf("Me: %d, Parent: %d\n", getpid(), getppid());
            // 
            munmap(shm_ptr, 4096);
            exit(0);
        }
    }

    // asteptam fiecare proces
    for(int nr=1; nr<argc; nr++){
        wait(NULL);
    }

    // Afisare 
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
    printf("Done parent: %d, Me: %d\n", getppid(), getpid());
    shm_unlink(shm_name);
    return 0;
}