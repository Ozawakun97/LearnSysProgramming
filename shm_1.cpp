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

struct SHM{
    int flag;
    int type;
    char msg[512];
};

int main(int argc,char** argv){
    int opt;
    struct SHM shm_data;
    memset(&shm_data,0,sizeof(shm_data));
    if(argc != 5){
        fprintf(stderr,"Usage : %s  -t 1|2 -m msg \n",argv[0]);
        exit(1);
    }
    while((opt = getopt(argc,argv,"t:m:")) != -1){
        switch (opt) {
            case 't':
                shm_data.type = atoi(optarg);
                break;
            case 'm':
                strcpy(shm_data.msg,optarg);
                break;
            default:
                fprintf(stderr,"Usage : %s  -t 1|2 -m msg \n",argv[0]);
                exit(1);
        }
    }
    pid_t  pid;
    int shmid;
    SHM* share_memory = NULL;
    key_t key = ftok("./shm_1.cpp",328);
    if((shmid = shmget(key,sizeof(SHM),IPC_CREAT| IPC_EXCL |0666)) < 0){
        if(errno == EEXIST){
            shmid = shmget(key,sizeof(SHM),0666);
        }
        else
        {
            perror("shmget");
            exit(1);
        }
    }
    else{
      printf("Success!\n");
    }
    if((share_memory =(SHM *)shmat(shmid,NULL,0)) == NULL){
        perror("shmat");
        exit(1);
    }
    while (1){
        if(!share_memory->flag){
            printf("<%d> : get shm data\n",shm_data.type);
            share_memory->flag = 1;
            sprintf(share_memory->msg,"<%d> : %s",shm_data.type,shm_data.msg);
            sleep(1);
        }else{
            printf("%s\n",share_memory->msg);
            share_memory->flag = 0;
        }
    }
}