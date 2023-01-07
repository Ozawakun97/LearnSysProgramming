//
// Created by mac on 2023/1/5.
//
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"

int main(int argc,char** argv){
    pid_t pid;
    char filename[512] = {0};
    char o_name[512] = {0};
    char f_type[256] = {0};
    char cmd[512] = {0};
    if(argc < 2){
        fprintf(stderr,"File path required");
        return 1;
    }
    strcpy(filename,argv[1]);
    char* sub;
    sub = strstr(filename,".");
    strncpy(o_name,filename,sub-filename);
    strcpy(f_type,sub);

    if(!strcmp(f_type,".c")){
        strcpy(cmd,"gcc");
    }else if(!strcmp(f_type,".cpp")){
        strcpy(cmd,"g++");
    }
    else{
        fprintf(stderr,"Unsupported File types");
        return 2;
    }
    if((pid = fork()) < 0){
        perror("fork");
        return 3;
    }
    if(pid == 0){
        execlp("vim","vim",filename,NULL);
    }
    wait(NULL);
    if((pid = fork()) < 0){
        perror("fork");
        return 3;
    }
    if(pid == 0){
        execlp(cmd,cmd,filename,"-o",o_name,NULL);
    }
    int status;
    wait(&status);
    if(status == 0){
        printf("OK\n");
        char name[25] = {0};
        sprintf(name,"./%s",o_name);
        execlp(name,o_name, NULL);
    }
    else{
        printf("Failed");
    }
    return 0;
}