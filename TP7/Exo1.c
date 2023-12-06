#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>


int main(int argc, char const *argv[]){
    
    const char *f = argv[1];
    unsigned char buffer[16];
    int fd;
    ssize_t b;

    if (argc !=2){
        exit -1;
    }


    fd = open(f, O_RDONLY);
    if(fd == -1){
        exit -1;
    }

    b = read(fd,buffer,16);
    if (b == -1){
        close(fd);
        exit -1;
    }
    
    printf("Les 16 premiers octet:\n");
    for (ssize_t i = 0; i < b; i++){
        printf("%02x\n", buffer[i]);
    }
    printf("\n");
    off_t p = lseek(fd, -16, SEEK_END);
    if(p == -1){
        close(fd);
        exit -1;
    }

    printf("Les 16 derniers octet:\n");
    b = read(fd, buffer, 16);
    if (b == -1){
        close(fd);
        exit -1;
    }
    for (ssize_t i = 0; i < b; i++){
        printf("%02x\n", buffer[i]);
    }
    printf("\n");
    close(fd);

    return 0;
}
