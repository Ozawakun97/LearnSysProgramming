//
// Created by mac on 2023/1/4.
//
#include "stdio.h"
#include "sys/select.h"
#include "sys/time.h"
#include "stdlib.h"
int main(){
    int age;
    fd_set rfds;
    timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    FD_ZERO(&rfds);
    FD_SET(0,&rfds);
    int ret = select(1,&rfds,NULL,NULL,&tv);
    if(ret <0){
        perror("select");
        exit(1);
    } else if(ret > 0){
        scanf("%d",&age);
    } else{
        age = 100;
    }

    printf("He is %d years old",age);
    return 0;
}