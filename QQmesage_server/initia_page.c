#include "login.h"
#include "registe.h"


int initia_page(char *opt,char *username,char *passwd)
{
    if (strncmp(opt,"B",1) == 0)
    {
        //登录 成功返回1 失败返回0
        int flag = login(username,passwd);
        if(flag == 1)
        {
            printf("欢迎登录:%s\n",username);
            return flag;
        }
        else if(flag == 2)
        {
            return flag;    
        }
        else {
            printf("用户名或密码错误\n");
            return flag;
        }
    }

    if (strncmp(opt,"B",1) == 0)
    {
        //注册
    }




    // while (1)
    // {
    //     printf("\n⭐⭐⭐⭐⭐⭐⭐QQ聊天室⭐⭐⭐⭐⭐⭐⭐\n");    
    //     printf("⭐A:注册                          ⭐\n");
    //     printf("⭐B:登录                          ⭐\n");
    //     printf("⭐⭐⭐⭐⭐⭐⭐QQ聊天室⭐⭐⭐⭐⭐⭐⭐\n");    
    //     printf("⭐请输入您的选择：");
    //     char opt = 'B';
    //     scanf("%c",&opt);
    //     getchar();
    //     if(opt == 'A')
    //     {
    //         registe_page(); 
    //     }
    //     else if(opt == 'B')
    //     {
    //        int reuslt =  login_page();
    //        if(reuslt == 1)
    //             return 1;
    //     }

    //     else {
    //         printf("请输入正确的选项，谢谢\n");
    //     }


    // }
    
    return 0;
}