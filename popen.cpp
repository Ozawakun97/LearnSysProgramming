//
// Created by mac on 2023/1/8.
//
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "string"

int main(){
    FILE *fp;
    char buff[512] = {0};
    if((fp = popen("cat ./popen.cpp","r")) == NULL){
        perror("fopen");
        exit(1);
    }
    while (fgets(buff,sizeof(buff),fp)){
        printf("%s",buff);
        memset(buff,0,sizeof(buff));
    }
    pclose(fp);
    return 0;
}