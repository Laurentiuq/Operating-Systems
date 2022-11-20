#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>


// #define nrLin1 3
// #define nrCol1 2
// #define nrLin2 2
// #define nrCol2 3



// int m1[nrLin1][nrCol1] = {
//     {1,2},
//     {2,1},
//     {4,5}
// };

// int m2[nrLin2][nrCol2] = {
//     {1,2,3},
//     {2,1,1}
// };


int nrLin1, nrCol1, nrLin2, nrCol2;

int m1[100][100], m2[100][100];

// matricea rezultat
int mRez[100][100];

// parametrii dati functiei utilizata in thread
struct Params{
    int lin;
    int col;
};

void citire(){
    printf("Numar linii prima matrice: \n");
    scanf("%d", &nrLin1);
    printf("Numar coloane prima matrice: \n");
    scanf("%d", &nrCol1);
    printf("Elemente prima matrice: \n");

    for(int i = 0; i<nrLin1; i++)
        for(int j = 0; j<nrCol1; j++)
            scanf("%d", &m1[i][j]);

    printf("Numar linii a doua matrice: \n");
    scanf("%d", &nrLin2);
    printf("Numar coloane a doua matrice: \n");
    scanf("%d", &nrCol2);
    printf("Elemente a doua matrice: \n");

    for(int i = 0; i<nrLin2; i++)
        for(int j = 0; j<nrCol2; j++)
            scanf("%d", &m2[i][j]);

    printf("\n\n");
}

// functia care face inmultirea 
void* inmultire(void* params){
   struct Params *linCol = params;
   int lin = linCol->lin;
   int col = linCol->col;
   for(int i = 0; i < nrCol2; i++){
        mRez[lin][col] += m1[lin][i] * m2[i][col];
   }
   return NULL;
}

int main(){
    citire();
    // o sa avem mai multe thread-uri
    pthread_t threads[nrLin1 * nrCol2];
    // la ce thread suntem la un moment dat si la final cate threaduri am creat
    int nrThread = 0;

    // calculam cu cate un thread fiecare pozitie din matricea rezultat
    for(int i = 0; i < nrLin1; i++){
        for(int j = 0; j < nrCol2; j++){
            // alocam spatiu pentru parametrii care vor fi dati threadului(functiei din thread)
            struct Params* linCol = calloc(1, sizeof(struct Params));
            linCol -> lin = i;
            linCol -> col = j;
            // cream un thread pentru pozitia actuala
            if(pthread_create(&threads[nrThread++], NULL, inmultire, linCol)){
                perror("Eroare la crearea unui thread");
                return errno;
            }
        }
    }


    // asteptam executia threadurilor
    for(int i = 0; i < nrThread; i++){
        if(pthread_join(threads[i], NULL)){
            perror("Eroare la asteparea unui thread");
            return errno;
        }
    }
    // afisam rezultatul
    for(int i = 0; i < nrLin1; i++){
        for(int j = 0; j < nrCol2; j++)
            printf("%d ", mRez[i][j]);
        printf("\n");
    }
    return 0;
}