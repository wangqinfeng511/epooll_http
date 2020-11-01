#ifndef SERVER_H
#define SERVER_H
#define MAX_EVENTS 10
//epoll最大事件数．
#define MAX_EVENT_FDS_FREQUENCY 3
//写入事件和读取事件失败最大次数．
#define MAX_CPU_NUMBER 4
//最大的CPU数

#include "work.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <QDebug>
#include "thread_obj.h"

class server
{
public:
    server(char * ip,int port);

private:
    int server_fd;
    int epoll_pool_fd;
    struct epoll_event ev,evs[MAX_EVENTS];
    void epoll_coment();
//    work *work_pool=nullptr;
    thread_obj* tos[4];
};

#endif // SERVER_H
