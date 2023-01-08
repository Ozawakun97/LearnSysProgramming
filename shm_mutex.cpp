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
#include <pthread.h>

struct SHM{
    int flag;
    int type;
    char msg[512];
    pthread_mutex_t mutex;
    pthread_cond_t cond;
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
                printf("shm_data_type : %d",shm_data.type);
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
    if(shm_data.type == 1){
        pthread_mutexattr_t mattr;
        pthread_mutexattr_init(&mattr);
        pthread_mutexattr_setpshared(&mattr,1);
        pthread_mutex_init(&share_memory->mutex,&mattr);
        pthread_condattr_t cattr;
        pthread_condattr_init(&cattr);
        pthread_condattr_setpshared(&cattr,1);
        pthread_cond_init(&share_memory->cond,&cattr);
    }
    if(shm_data.type == 1){
        while(1){
            char buffer[512] = {0};
            printf("Input : \n");
            scanf("%[^\n]s",buffer);
            getchar();
            if(strlen(buffer)){
                pthread_mutex_lock(&share_memory->mutex);
                strcpy(share_memory->msg,buffer);
                pthread_mutex_unlock(&share_memory->mutex);
                pthread_cond_signal(&share_memory->cond);
            }
        }
    }
    else{
        printf("\n");
        while (1){
            pthread_mutex_lock(&share_memory->mutex);
            pthread_cond_wait(&share_memory->cond,&share_memory->mutex);
            printf("\033[32m❤️\033[0m: %s \n",share_memory->msg);
            memset(share_memory->msg,0,sizeof(share_memory->msg));
            sleep(2);
            pthread_mutex_unlock(&share_memory->mutex);
        }
    }
}