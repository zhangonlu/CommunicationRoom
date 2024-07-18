#ifndef __INITIA_PAGE_H
#define __INITIA_PAGE_H

#define LOGIN_SUCESS 1


typedef struct user{
    char username[18];
    char passwd[18];
    char opt[2];
}user_opt; 



void  initia_page(user_opt * user);
#endif