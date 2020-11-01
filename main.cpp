//#include <QCoreApplication>
#include <stdio.h>
#include "server.h"
#include <string>
#include <QDebug>
#include "head_compute.h"
#include <ext/hash_map>
#include "work.h"
using namespace __gnu_cxx;
int main(int argc, char *argv[])
{
    setvbuf(stdout, 0, _IONBF, 0);
    try{
            server service("192.168.41.141",8088);
    }catch(const char* msg){
        qDebug()<<msg;
    }
    //work 测试部分
//    for(int i=1;i<10000;i++){
//        insert(i);
//    }
//    while (true) {
//        work *tmp_work=get();
//        if(tmp_work==nullptr){
//            printf("get 返回null\n");
//            break;
//        }else {
//            printf("取得描述符%d\n",tmp_work->fd);
//            remove(tmp_work);
//        }
//    }
//    for(int i=0;i<100;i++){
//        work *t=get();
//        if(t!=nullptr){
//            printf("up:%p next:%p\n",t->up,t->next);
//            printf("10取得描述符%d\n",t->fd);
//        }
//    }
}
