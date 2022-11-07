#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>



void* inverseaza(void *param){
    int len = 0;
    char *str = (char *) param;
    char *rez = malloc(sizeof(char) * len);
    len = strlen(str);

    for(int i = 0; i < len; i++){
        rez[len - i - 1] = str[i]; 
    }
    // printf("%s", rez);
    return rez;
}

int main(int argc, char* argv[]){
    pthread_t t1;
    void *rez;
    if(pthread_create(&t1, NULL, inverseaza, argv[1])){
        perror("Eroare la creare thread");
        return errno;
    }
    if(pthread_join(t1, &rez)){
        perror("Eroare la join");
        return errno;
    }
    printf("%s\n", (char *)rez);
    free(rez);
    return 0;
}