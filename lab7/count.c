//decrease_count si increase_count nu trebuie sa se poate executa simultan

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>


# define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES ;

pthread_mutex_t mtx;

int decrease_count ( int count )
{   
    // blocam mutexul
    int mtx_status = pthread_mutex_lock(&mtx);
    // daca a esuat blocarea (de ex. era deja blocat)
    if(mtx_status < 0){
        perror("Eroare blocare mutex in decrease\n");
        return errno;
    }
    // daca nu avem suficiente resurse
    if (available_resources < count )
    {   
        printf("Not enough resources\n");
        exit(-1);
        return -1;
    }
    // daca avem le alocam
    else
    {   
        available_resources -= count;
        printf("Got %d resources, %d remaining\n", count, available_resources);
    }

    // deblocam mutexul
    mtx_status = pthread_mutex_unlock(&mtx);
        if(mtx_status < 0){
        perror("Eroare deblocare mutex in decrease\n");
        return errno;
    }

}

int increase_count (int count)
{
    // status mutexului, daca lockul/unlockul a reusit/esuat
    // blocam mutexul
    int mtx_status = pthread_mutex_lock(&mtx);
    // daca a esuat lockul
    if(mtx_status < 0){
        perror("Eroare blocare mutex in increase\n");
        return errno;
    }

    // eliberam resursele
    available_resources += count;

    printf("Released %d resources, %d remaining\n", count, available_resources);

    // deblocam mutexul
    mtx_status = pthread_mutex_unlock(&mtx);

    // daca a esuat unlockul
    if(mtx_status<0){
        perror("Eroare deblocare mutex in increase\n");
        return errno;
    }
    return mtx_status;
}

// functia executata de fiecare thread
void* thread_action(void* x){

    // resurse necesare pentru threadul curent
    int count = *((int *) x); // cast catre pointer de int si deferentiere
    int dummy_var = 0;
    // aloca resursele
    decrease_count(count);
    
    // daca j e foarte mare ramane fara resurse
    for(int j=1; j<=10000;j++)
        dummy_var = 1;

    // wait()

    // printf("A fost executat ceva\n");
 
    // elibreaza resursele
    increase_count(count);

    return NULL;
}


int main(){
    // initializare mutex    
    if(pthread_mutex_init(&mtx, NULL)){
        perror("Eroare la initializare mutex");
        return perror;
    }
    // numarul de threaduri
    int nr_threads = 5;
    // resursele necesare pentru fiecare thread
    int resources_needed[5] = {2,2,1,3,2};

    // declaram si alocam spatiu pentru threaduri
    pthread_t* threads = malloc(sizeof(pthread_t) * nr_threads);

    // cream threadurile
    for(int i = 0; i<nr_threads; i++)
        pthread_create(&threads[i], NULL, thread_action, &resources_needed[i]);
    //asteptam sa se execute threadurile
    for(int i = 0; i<nr_threads; i++)
        pthread_join(threads[i], NULL);
    
    pthread_mutex_destroy(&mtx);

    return 0;
}