#include "thread_obj.h"
std::mutex locks;
std::unique_lock<std::mutex> un_lock(locks,std::defer_lock);
std::condition_variable var_lock;
thread_obj::thread_obj()
{
}
void thread_obj::thread_run(){
    std::thread obj(&thread_obj::start,this);
    obj.detach();

}
void thread_obj::action(){
    var_lock.notify_all();

}
void thread_obj::start(){
    while (true) {
       var_lock.wait(un_lock);
       while (true) {
            work *w=get();
             if(w==nullptr){
                 break;
             }
             int read_size= recv(w->fd,buff,BUFF_SIZE,0);
             int *head_length=(int *)malloc(sizeof (int));
             if(read_size>=0 and w->head==nullptr){
                head.create_head(buff,read_size,head_length);
             }
             printf("线程%p:描述符%d\n",this,w->fd);
       }
    }
}

