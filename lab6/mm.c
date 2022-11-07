#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>




#define nrLin1 3
#define nrCol1 2
#define nrLin2 2
#define nrCol2 3
// const int nrLin1 = 3, nrCol1 = 2,  nrLin2 = 2, nrCol2 = 3;

int m1[nrLin1][nrCol1] = {
    {1,2},
    {2,1},
    {4,5}
};

int m2[nrLin2][nrCol2] = {
    {1,2,3},
    {2,1,1}
};

int mRez[nrLin1][nrCol2];

struct Params{
    int lin;
    int col;
};

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
    pthread_t threads[nrLin1 * nrCol2];
    int nrThread = 0;
    for(int i = 0; i < nrLin1; i++){
        for(int j = 0; j < nrCol2; j++){
            struct Params* linCol = calloc(1, sizeof(struct Params));
            linCol -> lin = i;
            linCol -> col = j;
            if(pthread_create(&threads[nrThread++], NULL, inmultire, linCol)){
                perror("Eroare la crearea unui thread");
                return errno;
            }
        }
    }

    for(int i = 0; i < nrThread; i++){
        if(pthread_join(threads[i], NULL)){
            perror("Eroare la asteparea unui thread");
            return errno;
        }
    }
    for(int i = 0; i < nrLin1; i++){
        for(int j = 0; j < nrCol2; j++)
            printf("%d ", mRez[i][j]);
        printf("\n");
    }
    return 0;
}