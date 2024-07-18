#ifndef __ONLINE_WRITE_H
#define __ONLINE_WRITE_H
#include "../header.h"


void write_user_info(user_info *user_info, int len);

//更新在线好友 
void update_online_user(char *username,int cid);
#endif
