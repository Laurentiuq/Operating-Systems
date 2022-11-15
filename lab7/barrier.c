#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <errno.h>



pthread_mutex_t mtx;
sem_t sem;
int nr_threads, current_thread ;

// initializeaza variabile, reseteaza threadul curent si variabila semafor
void init(int n){
    nr_threads = n;
    current_thread = 0;
    sem_init(&sem, 0, 0);
}

void barrier_point(){

    // blocam mutextul
    pthread_mutex_lock(&mtx);

    // actualizam threadul la care ne aflam
    current_thread++;

    // daca am ajuns la ultimul thread deblocam mutexul crestem valoarea variabilei semafor, deci sem_wait poate sa o scada si sa inceapa executia firelor
    if(current_thread == nr_threads){
        pthread_mutex_unlock(&mtx);
        for(int i = 0; i < current_thread; i++)
            sem_post(&sem);
    }
    
    pthread_mutex_unlock(&mtx);
    // cat timp variabila semafor e 0(nu suntem la ultimul thread) blocheaza executia firului de executie
    sem_wait(&sem);
}

void* tfun(void *v)
{
    int *tid = (int *)v;
    printf("%d reached the barrier\n", *tid);
    barrier_point();
    printf("%d passed the barrier\n", *tid);

    free(tid);
    return NULL;
}

int main(){
    init(5);
    pthread_mutex_init(&mtx, NULL);
    pthread_t* threads = malloc(sizeof(pthread_t) * nr_threads);
    for(int i = 0; i < nr_threads; i++){
        int* arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&threads[i], NULL, tfun, arg);
    }
    for(int i = 0; i < nr_threads; i++)
        pthread_join(threads[i], NULL);

    sem_destroy(&sem);
    pthread_mutex_destroy(&mtx);
    free(threads);
    return 0;
}