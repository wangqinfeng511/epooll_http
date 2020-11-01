#include "work.h"
bucket * buck_pool=nullptr;

void insert(int fd){
    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    //队列
    list *tmp_dom=(list *)malloc(sizeof (list));
    tmp_dom->fd=fd;
    tmp_dom->next=nullptr;
    work * tmp_work=(work *) malloc(sizeof (work));
    tmp_work->fd=fd;
    tmp_work->buff=nullptr;
    tmp_work->head=nullptr;
    tmp_work->next=nullptr;
    int mod=fd%MOD_VALUE;
    //初始化首桶．
    if(buck_pool==nullptr){
        buck_pool=(bucket *)malloc(sizeof (bucket));
        //为桶设置桶号
        buck_pool->mod=mod;
        buck_pool->works=tmp_work;
        buck_pool->next=nullptr;
        buck_pool->up=nullptr;
        //任务链表初始化．
        buck_pool->works=tmp_work;
    }else {
    bucket * tmp_buck_pool=buck_pool;
    //匹配桶．
    while (tmp_buck_pool->next!=nullptr) {
        if(tmp_buck_pool->mod==mod){
            break;
        }
        tmp_buck_pool=tmp_buck_pool->next;
    }
    //没匹配到桶时创建桶．并指向桶链表尾部．
    if(tmp_buck_pool->mod!=mod){
        //创建桶到桶池的尾部
        tmp_buck_pool->next=(bucket *)malloc(sizeof (bucket));
        tmp_buck_pool->next->up=tmp_buck_pool->next;
        //当前桶指向最后一个桶的指针．
        tmp_buck_pool=tmp_buck_pool->next;
        //设置当前桶的模值
        tmp_buck_pool->mod=mod;
        //当前桶的首个任务．
        tmp_buck_pool->works=tmp_work;
        //维护任务列表．
        list *ls=buck_pool->work_list;
        if(ls==nullptr){ //没有待处理任务时，直接指向新任务链表．
            buck_pool->work_list=tmp_dom;
            return;
        }else {
            //徇环到待处理任务的尾部追加任务．
            while (ls->next!=nullptr) {
                ls=ls->next;
            };
            ls->next=tmp_dom;
            return;
        }
    }
    //匹配mod的桶
    //不是新桶必然有任务链表．
        work * tmp_works=tmp_buck_pool->works;
        bool is_add=true;
        while (tmp_works->next!=nullptr) {
            if(tmp_works->fd==fd){
                free(tmp_work);
                is_add=false;
                break;
            }else {
               tmp_works=tmp_works->next;
            }
        }
        if(is_add){
           tmp_works->next=tmp_work;
           tmp_work->up=tmp_works->next;
           printf("添加任务桶号%d描述符%d\n",tmp_buck_pool->mod,tmp_work->fd);
        }

    }
    //维护任务列表．
    list *ls=buck_pool->work_list;
    if(ls==nullptr){ //没有待处理任务时，直接指向新任务链表．
        buck_pool->work_list=tmp_dom;
        return;
    }else {
        //徇环到待处理任务的尾部追加任务．
        while (ls->next!=nullptr) {
            ls=ls->next;
        };
        ls->next=tmp_dom;
        return;
    }
};

work * get(){
    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    bucket *tmp_buck_pool=buck_pool;
    //桶池为空．
    if(tmp_buck_pool==nullptr){return  nullptr;};
    list *tmp_fd=tmp_buck_pool->work_list;
    //任务列表为空．
    if(tmp_fd==nullptr){return  nullptr;};
    //取出任务描术符．
    int fd=tmp_fd->fd;
    //算出桶号．
    int mod=fd%MOD_VALUE;
    //设置下一个任务为起始任务．
    buck_pool->work_list=tmp_fd->next;
//    printf("%d",buck_pool->work_list->fd);
    //删除当前任务
    free(tmp_fd);
    //找对应桶．
    while (tmp_buck_pool!=nullptr) {
        if(tmp_buck_pool->mod==mod){
//            printf("找到描述符%d对应桶%d\n",fd,mod);
            break;
        }else {
            tmp_buck_pool=tmp_buck_pool->next;
        }
    };
    if(tmp_buck_pool==nullptr){
        //桶都匹配不到，更别说任务了．
          return  nullptr;
    }else{ //匹配到桶了
        work *works=tmp_buck_pool->works;//桶的任务链表．
//        printf("描述符%d的任务链表%p\n",fd,works);
        while (works!=nullptr) {
            if(works->fd==fd){
//                printf("匹配到描述符%d\n",works->fd);
                return works;
            }else {
//                printf("没匹配的描术符%d\n",works->fd);
                works=works->next;
                }
        }
//        printf("没有找到描述符%d对应的任务\n",fd);
    }
    return  nullptr;
}
void remove(work *w){ 
    if(w==nullptr){return;};
    int fd=w->fd;
    if(fd<=0){return;};
    int mod=fd%MOD_VALUE;
    bucket *tmp_buck_pool=buck_pool;
    if(tmp_buck_pool==nullptr){return;};
    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    while (tmp_buck_pool->next!=nullptr) {
        if(tmp_buck_pool->mod==mod){
            break;
        }else {
            tmp_buck_pool=tmp_buck_pool->next;
        }
    }
    //tmp_buck_pool两种情况：１,tmp_buck_pool->mod等于mod.2，tmp_buck_pool->mod!=mod　并且next为nullptr;
    if(tmp_buck_pool->mod!=mod){return;};
//    printf("remove 桶号%d,描述符:%d;",tmp_buck_pool->mod,fd);
    work *tmp_work =tmp_buck_pool->works;
    while (tmp_work!=nullptr) {
        if(tmp_work->fd==w->fd){
            if(tmp_work->up==nullptr and tmp_work->next==nullptr){//最后一个任务
                free(tmp_buck_pool); //销掉空桶．
                break;
            }
            if(tmp_work->up==nullptr){//桶的首任务时．下个任务就变成第一个任务了．
                tmp_buck_pool->works=tmp_work->next;//桶的works就指向当前任务的下个任务．
                tmp_buck_pool->works->up=nullptr; //桶的首任务是没有上个任务指向，所以为空．
            }else {//不是桶的首个任务时．
                tmp_work->up->next=tmp_work->next;//上个任务的下个指向当前任务的下个任务．
                if(tmp_work->next!=nullptr){
                    tmp_work->next->up=tmp_work->up;//下个任务的上个任务指向自然变成当前任务的上个任务了．
                }
            }
            //先要销掉当前任务中的其它结构体．
            if(tmp_work->buff!=nullptr){
                free(tmp_work->buff);
            }else if (tmp_work->head!=nullptr) {
                free(tmp_work->head);
            };
            printf("remove 桶号%d,描述符:%d;",tmp_buck_pool->mod,tmp_work->fd);
            free(tmp_work);//销掉当前任务．
            break;
        }
     tmp_work=tmp_work->next;
    }
}
