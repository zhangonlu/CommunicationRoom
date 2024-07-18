#include "login.h"
#include "registe.h"
#include "../header.h"
#include "initia_page.h"
void  initia_page(user_opt *user)
{
        printf("\n⭐⭐⭐⭐⭐⭐⭐QQ聊天室⭐⭐⭐⭐⭐⭐⭐\n");    
        printf("⭐A:注册                          ⭐\n");
        printf("⭐B:登录                          ⭐\n");
        printf("⭐C:退出                          ⭐\n");
        printf("⭐⭐⭐⭐⭐⭐⭐QQ聊天室⭐⭐⭐⭐⭐⭐⭐\n");    
        printf("⭐请输入您的选择：");
        char opt = 'B';
        scanf("%c",&opt);
        getchar();
        if(opt == 'A')
        {   
            user->opt[0] = opt;
            registe_page(user); 
        }
        else if(opt == 'B')
        {
            user->opt[0] = opt;
            login_page(user);
        }

        else if(opt == 'C')
        {
            user->opt[0] = opt;
        }
        else {
            printf("请输入正确的选项，谢谢\n");
        }
}






