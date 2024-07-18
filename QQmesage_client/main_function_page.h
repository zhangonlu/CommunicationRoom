#ifndef __MAIN_FUNCTION_H_
#define __MAIN_FUNCTION_H_


typedef struct main_page{
    char opt[5];         //功能选择   
    char msg[1024];     //消息内容
    char  user_flag[10]; //用户标识符 username + 标识
    // char ip[18];
    // char port[18];
    char username[18];
}Messages;

void  mainfunction_page(Messages *msg,int cid);
void user_list(char list[][128]);
char * select_user(char uname_list[][128]);
void get_user_flag(char *user_flag,char *username);

int send_file(char *filename,int sockfd);


#endif