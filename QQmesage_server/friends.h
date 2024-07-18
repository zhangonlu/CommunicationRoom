#ifndef __FRIENDS_FUN_H
#define __FRIENDS_FUN_H

#include "../header.h"

void select_user_online(cJSON * json_user);
int select_cid_byusername(char *username);
void save_conversition(char *str);
void select_user_cid(int *num);
#endif