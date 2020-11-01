#ifndef THREAD_OBJ_H
#define THREAD_OBJ_H
#define BUFF_SIZE 1500
#include <thread>
#include <QDebug>
#include <mutex>
#include <condition_variable>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include "work.h"

class thread_obj
{

public:
    thread_obj();
    void thread_run();
    void action();
private:
    bool status=false;
    void start();
    head_compute head;
    char buff[BUFF_SIZE];
};

#endif // THREAD_OBJ_H
