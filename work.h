#ifndef HEAD_H
#define HEAD_H
#include <stdio.h>
#include <mutex>
#include "head_compute.h"
#define MOD_VALUE 32;
//模的值．
struct work{
    int fd;
    work * next;
    work *up;
    head_struct *head;
    char * buff;
};
struct list{
    int fd;
    list * next;
};
struct bucket{
    int mod;
    work *works;
    bucket *next;
    bucket *up;
    list *work_list;
};
extern bucket *buck_pool;

void insert(int fd);
work * get();
void remove(work *w);
#endif // HEAD_H
