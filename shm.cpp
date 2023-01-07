//
// Created by mac on 2023/1/7.
//
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/ipc.h"
#include "sys/types.h"
#include "sys/shm.h"
#include "sys/wait.h"
#include "string"

int main(){
    pid_t  pid;
    int shmid;
    char* share_memory = NULL;
    key_t key = ftok("./shm.cpp",328);
    if((shmid = shmget(key,4096,IPC_CREAT| IPC_EXCL |0666)) < 0){
        perror("shmget");
        exit(1);
    }
    if((share_memory =(char*) shmat(shmid,NULL,0)) == NULL){
        perror("shmat");
    }
    if((pid = fork()) <0){
        perror("fork");
        exit(1);
    }
    if(pid){
        while(1){
            scanf("%[^\n]s",share_memory);
            getchar();
        }
    }else{
        while (1){
            if(strlen(share_memory) == 0) continue;
            printf("<child> : %s\n",share_memory);
            memset(share_memory,0,4096);
        }
    }
}