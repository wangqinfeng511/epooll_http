#include "server.h"

server::server(char * ip,int port)
{
   int status;
   server_fd= socket(AF_INET,SOCK_STREAM,0);
   if(server_fd<=0){
       throw "创建文件描述符错误！";
   }
   struct sockaddr_in servaddr;

   servaddr.sin_family=AF_INET;
   servaddr.sin_addr.s_addr=inet_addr(ip);
   servaddr.sin_port=htons(port);
   status=bind(server_fd,(sockaddr *)&servaddr,sizeof (servaddr));
   if(status==-1){
//       throw  std::to_string(errno);
       throw "socket bind() 错误! erron:";
   }

   status=listen(server_fd,25);
   if(status==-1){
       throw "监听错误！error错误码："+std::to_string(errno);
   }
   //处理线程
   for(int i=0;i<MAX_CPU_NUMBER;i++){
       thread_obj *tmp_obj=(thread_obj*)malloc(sizeof (thread_obj));
       tos[i]=tmp_obj;
       tos[i]->thread_run();
   }

   epoll_coment(); //epoll开始．
}
void server::epoll_coment(){
    epoll_pool_fd=epoll_create(1);//epoll_create(int size) size2.6.8后没有意义但要大于０
    ev.data.fd=server_fd;
    ev.events=EPOLLIN|EPOLLET;
    epoll_ctl(epoll_pool_fd,EPOLL_CTL_ADD,server_fd,&ev);

    struct sockaddr clinet_addr;
    socklen_t addr_len=sizeof(clinet_addr);
    int client_fd=0,read_size=0,status=0;;
    while (true) {
         int evs_number=epoll_wait(epoll_pool_fd,evs,200,300);
         for(int i=0;i<evs_number;i++){
             if(evs[i].data.fd==server_fd){ //连接ＥＰＯＬＬ监视．
                 //取出新连接的socket文件描述符．
                 client_fd=accept(server_fd,(sockaddr*)&clinet_addr,&addr_len);
                 if (client_fd==-1){
                     close(client_fd);
                     continue;
                 }
                 //描述符设置为非阻塞模试
                 int iFlags=fcntl(client_fd,F_GETFL,0);
                 fcntl(client_fd,F_SETFL,iFlags|O_NONBLOCK);
                 //epoll事件设置．读｜边缘｜只处发一次，线程修改后继续下次监控．
                 ev.data.fd=client_fd;
                 ev.events=EPOLLIN|EPOLLET|EPOLLONESHOT;
                 status =epoll_ctl(epoll_pool_fd,EPOLL_CTL_ADD,client_fd,&ev);
                 if(status==-1){
                     close(client_fd);
                 }
             }else if(evs[i].events==EPOLLIN) {
                 int fd=evs[i].data.fd;
                 insert(fd);
                 printf("添加任务完成\n");
                 for(int i=0;i<MAX_CPU_NUMBER;i++){
                     tos[i]->action();
                 }
                  printf("触发任务执行完成\n");
             }
         }
    }
}
