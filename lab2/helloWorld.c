#include <errno.h>
#include <stdio.h>

int main(){

    int t = write(1, "Hello, world!\n", 14);
    if (t < 0) {
        perror("fail");
        return errno;
    }
    return 0;
}