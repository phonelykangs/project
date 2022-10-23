#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>

#define PATH_NAME   "/home/phonelykangs/project/Linux"
#define PROJECT_ID  123
#define NAME_SIZE   20

struct msg{
    long mtype;
    char name[NAME_SIZE];
    int math;
    int chinese;
};
