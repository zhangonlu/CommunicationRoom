#ifndef __LOGIN_H
#define __LOGIN_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int login(char *username,char *passwd);
int login_page();



#endif