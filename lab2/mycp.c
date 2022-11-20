#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    // printf(argv[1]);
    

    // deschidem fisierul sursa cu read only
    int s = open(argv[1], O_RDONLY);

    if (s < 0 ){
        perror("Source file not found\n");
        return errno;
    }

    char content;
    // int ct = read(s, content, sizeof(content) - 1);

    // daca nu exista il cream, daca exista il deschidem cu write only si dam userului permisiuni de read/write
    int t = open(argv[2],  O_CREAT | O_WRONLY, S_IRWXU);

    if(t < 0){
        perror("Destination file error\n");
        return errno;
    }


    // citim cate un byte din fisier si il scriem pana la final
    while(read(s, &content, 1)){
        write(t, &content, 1);
    }

    int c1 = close(s);
    if(c1<0){
        perror("Source file not closing");
        return errno;
    }
    int c2 = close(t);
    if(c2<0){
        perror("Dest file not closing");
        return errno;
    }


    return 0;


}