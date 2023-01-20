//
// Created by mac on 2023/1/20.
//
#include "stdio.h"
#include "pthread.h"
#include <unistd.h>
#define INS 5
int now;
int sum;
pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;
void * work(void * arg){
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if(now > 100){
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        sum += now;
        now++;
        pthread_mutex_unlock(&mutex);
        usleep(15);
    }
}

int main(){
    pthread_t thread;
    for(int i = 0; i< INS;i++){
        pthread_create(&thread,NULL,work,NULL);
    }
    usleep(15000);
    printf("now = %d sum = %d\n",now,sum);
}