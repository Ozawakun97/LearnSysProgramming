//
// Created by mac on 2023/1/19.
//
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string"
#include "sys/ipc.h"
#include <sys/msg.h>
#include <sys/types.h>
struct msgbuff{
    long mtype;
    char mtext[512];
};
void send_msg(int qid, int type, char* msg,int size){
    struct msgbuff buffer;
    buffer.mtype = type;
    strcpy(buffer.mtext,msg);
    printf("%d",buffer.mtype);
    if(msgsnd(qid,(void *)&buffer,sizeof(buffer.mtext),IPC_NOWAIT) == -1){
        perror("msgsnd");
        exit(1);
    }
}
void get_msg(int qid, int type){
    while (1){
        struct msgbuff msg;
        bzero(&msg,sizeof(msg));
        if(msgrcv(qid,(void *)& msg,sizeof(msg.mtext),type,IPC_NOWAIT | MSG_NOERROR) == -1){
            perror("msgrcv");
            exit(1);
        }
        printf("recieved message <%d> <%s>\n",type,msg.mtext);
    }

}

int main(int argc, char** argv){
    int opt,mode,msgtype;
    char mtext[512];
    int msgq_id;
    // mode == 1 -> send  ;
    while((opt = getopt(argc,argv,"st:m:")) != -1){
        switch (opt) {
            case 's':
                mode = 1;
                break;
            case 'r':
                mode = 2;
                break;
            case 't':
                msgtype = atoi(optarg);
                break;
            case 'm':
                strcpy(mtext,optarg);
                break;
            default:
                fprintf(stderr,"Usage: %s -[s|r] -t type -m msg\n",argv[0]);
                exit(1);
        }
       if((msgq_id = msgget(2022,IPC_CREAT | 0666)) < 0){
           perror("msgget");
           exit(1);
       }
       if(mode == 1){
           send_msg(msgq_id,msgtype,mtext,sizeof(mtext));
       }else{
           get_msg(msgq_id,msgtype);
       }
    }

}
