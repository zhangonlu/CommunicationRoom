#include "login.h"
#include "initia_page.h"

void login_page(user_opt * user)
{
    printf("\n⭐⭐⭐⭐⭐⭐⭐登录界面⭐⭐⭐⭐⭐⭐⭐\n");
    printf("⭐请输入用户名:");
    fflush(stdin);
    scanf("%s", user->username);
    printf("⭐请输入密码:");
    fflush(stdin);
    scanf("%s", user->passwd);
    getchar();
}
