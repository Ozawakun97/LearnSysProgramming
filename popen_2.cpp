//
// Created by mac on 2023/1/8.
//
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "string"

static pid_t *childpid = NULL;
static int maxsize = 0;

FILE * _popen(const char* cmd, const char*type){
    FILE *fp;
    int pipfd[2];
    pid_t pid;
    if((type[0] != 'r' && type[1] != 'w') | type[1] != '\0'){
        errno = EINVAL;
        return NULL;
    }
    if(childpid == NULL){
        maxsize = sysconf(_SC_OPEN_MAX);
        if((childpid = (pid_t *) calloc(maxsize,sizeof(pid_t))) == NULL){
            return NULL;
        }
    }
    if(pipe(pipfd) < 0){
        return NULL;
    }
    //fork()
    if((pid = fork())<0){
        return NULL;
    }
    //execl()
    if(pid == 0){
        if(type[0] == 'r'){
            close(pipfd[0]);
            if(pipfd[1] != STDOUT_FILENO){
                dup2(pipfd[1],STDOUT_FILENO);
                close(pipfd[1]);
            }
        }
        else{
            close(pipfd[1]);
            if(pipfd[0] != STDOUT_FILENO){
                dup2(pipfd[0],STDOUT_FILENO);
                close(pipfd[0]);
            }
        }
        execl("/bin/sh","sh","-c",cmd,NULL);
    }
    if(type[0] == 'r'){
        close(pipfd[1]);
        if((fp = fdopen(pipfd[0],type)) == NULL){
            return NULL;
        } else{
            close(pipfd[1]);
            if((fp = fdopen(pipfd[1],type)) == NULL){
                return NULL;
            }
        }
    }
    childpid[fileno(fp)] = pid;
    return fp;
}
int _pclose(FILE* fp){
    int status, pid, fd;
    fd = fileno(fp);
    pid = childpid[fd];
    if(pid == 0){
        errno = EINVAL;
        return -1;
    }
    fflush(fp);
    close(fd);
    wait4(pid,&status,WUNTRACED,NULL);
    return status;
    //wait()
    //fclose()
}
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