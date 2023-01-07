#include <iostream>
#include <stdio.h>
#include "unistd.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"

int main(int arg, char** argv) {
    int fd,fd2;
    if(arg != 3){
        fprintf(stderr,"Lack of Argument");
        exit(1);
    }
    if((fd = open(argv[1],O_RDONLY)) < 0){
        fprintf(stderr,"Error!\n");
        perror("Open");
        exit(1);
    }
    if((fd2 = open(argv[2],O_CREAT | O_RDWR,0666))< 0){
        perror("open");
        exit(1);
    }
    while(1){
        char buff[512] = {0};
        ssize_t size = read(fd,buff,sizeof(buff));
        write(fd2,buff,size);
        if(size <= 0){ break;}
    }
    close(fd);
    close(fd2);
    return 0;
}
