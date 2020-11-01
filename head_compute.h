#ifndef HEAD_COMPUTE_H
#define HEAD_COMPUTE_H
#include <string.h>
#include <stdio.h>
#include <malloc.h>
struct head_struct{
    char *method=nullptr;
    char *path=nullptr;
    char *host=nullptr;
    char *user_agent=nullptr;
    char *accept=nullptr;
    char *accept_encoding=nullptr;
    char *accept_language=nullptr;
    char *connection=nullptr;
    char *referer=nullptr;
    char *content_length=nullptr;
    char *content_type=nullptr;
};
struct char_dict{
    char *key;
    char *value;
};
class head_compute
{
public:
    head_compute();
    head_struct * create_head(char *buff,int buff_size,int *head_length);

private:
    head_struct * chars_to_head_struct(char * buff,int buff_size);
    char_dict split(char *buff,char *cf);

};

#endif // HEAD_H
