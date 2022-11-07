#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    // printf(argv[1]);
    // printf(argv[2]);
    int s = open(argv[1], O_RDONLY);

    if (s < 0 ){
        perror("source file not found\n");
        return errno;
    }

    char content;
    // int ct = read(s, content, sizeof(content) - 1);

    int t = open(argv[2], O_CREAT | O_WRONLY | S_IWUSR | S_IRWXU);

    // if(t < 0){
        // perror("destination file error\n");
        // return errno;
    // }

    while(read(s, &content, 1)){
        write(t, &content, 1);
    }

    int c1 = close(s);
    if(c1<0){
        perror("file not closing");
        return errno;
    }
    int c2 = close(t);
    if(c2<0){

    }


    return 0;


}