#include "head_compute.h"

head_compute::head_compute()
{

}
head_struct * head_compute::create_head(char *buff,int buff_size,int *head_length){
    char cf[]="\r\n\r\n";
    char *h_s=nullptr;
    int head_len=0;
    for(int i=0;i<buff_size-2;i++){
        if(memcmp(buff+i+1,&cf,4)==0){
            head_len=i+5;
        }
    }
    *head_length=head_len;
    if(head_length!=0){
        h_s= (char *)malloc(head_len);
        memcpy(h_s,buff,head_len);
        h_s[head_len]='\0';
    }
    //分解头部
//    printf("%s",h_s);
    chars_to_head_struct(h_s,head_len);
    free(h_s);
    return  nullptr;
}

head_struct * head_compute::chars_to_head_struct(char * buff,int buff_size){
    char *cf="\n";
    char *tmp=strtok(buff,cf); //切分第一个
    head_struct *head=(head_struct *)malloc(sizeof (head_struct));
    //取出方法
    if(memcmp(tmp,"GET",strlen("GET"))==0){
        head->method=(char *)malloc(sizeof("GET"));
        head->method="GET";
    }else if (memcmp(tmp,"POST",strlen("POST"))==0) {
        head->method=(char *)malloc(sizeof("POST"));
        head->method="POST";
    }else {
          printf("没有方法退出！");
          return  nullptr;
    }
//    printf("方法%s\n",head->method);
    //取出路径．
    char * path=tmp+strlen(head->method);
    head->path=path;
    while (tmp!=nullptr) { //处理第一个参数．
        tmp=strtok(nullptr,cf);
        char *fenge=": ";
        if(tmp!=nullptr and strlen(tmp)>strlen(fenge)){
            printf("输入%s\n",tmp);
            char_dict t_d= split(tmp,fenge);
            if(t_d.key==nullptr){continue;};//返回为空跳过本次．
                        if (memcmp(t_d.key,"Content-Type",strlen("Content-Type"))==0) {
                            printf("%s",t_d.value);
                        }
            if(memcmp(t_d.key,"Host",strlen("Host"))==0){
                head->host=t_d.value;
                free(t_d.key);
            }else if (memcmp(t_d.key,"User-Agent",strlen("User-Agent"))==0) {
                head->user_agent=t_d.value;
                free(t_d.key);
            }else if (memcmp(t_d.key,"Accept",strlen("Accept"))==0) {
                head->accept=t_d.value;
                free(t_d.key);
            }else if (memcmp(t_d.key,"Accept-Encoding",strlen("Accept-Encoding"))==0) {
                head->accept_encoding=t_d.value;
                free(t_d.key);
            }else if (memcmp(t_d.key,"Content-Length",strlen("Content-Length"))==0) {
                head->content_length=t_d.value;
                free(t_d.key);
            }else if (memcmp(t_d.key,"Connection",strlen("Connection"))==0) {
                head->connection=t_d.value;
                free(t_d.key);
            }else if (memcmp(t_d.key,"Content-Type",strlen("Content-Type"))==0) {
                head->content_type=t_d.value;
                free(t_d.key);
            }
            else {
                free(t_d.key);
                free(t_d.value);
            }

        }

    }
    return  nullptr;
}
char_dict head_compute::split(char *buff,char *cf){
    char_dict dict;
    if(buff==nullptr){
        printf("传入空的buff");
        return dict;
    }
    int value_len=0;
    for (int i=0;i<strlen(buff);i++){
        if(memcmp(buff+i,cf,strlen(cf))==0){
            char *key=(char *)malloc(i+1);
            memcpy(key,buff,i);
            *(key+i+1)='\0';
            dict.key=key;
            value_len=strlen(buff)-i;
            char *value=(char*)malloc(value_len+1);
            memcpy(value,buff+i+2,value_len); //2表不：号和空格．
            *(value+value_len+1)='\0';
            dict.value=value;
            break;
        }else {
            dict.key=nullptr;
            dict.value=nullptr;
        }
    }
//    dict.key=strtok(buff,cf);
//    dict.value=strtok(nullptr,cf);
//    printf("%s,%s,%s",buff,dict.key,dict.value);
    return  dict;
}
