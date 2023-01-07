//
// Created by mac on 2023/1/2.
//
#include <iostream>
#include <stdio.h>
#include "unistd.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"

int main(int argc, char** argv){
    int opt;
    getopt(argc,argv,"r:w:");
}