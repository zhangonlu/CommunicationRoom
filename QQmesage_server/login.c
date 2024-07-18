#include "login.h"

// 返回1登录成功 返回0登录失败
int login(char *username, char *passwd)
{
    FILE *fd = fopen("./user.txt", "r");
    if (fd == NULL)
    {
        perror("file open error\n");
        return -1;
    }

    printf("usernmae = %s\n", username);
    // 防止重复登录

    FILE *fp = fopen("./user_info.txt", "r");
    if (fp != NULL)
    {

        int flg = 0;
        fseek(fp, 0, SEEK_SET);
        char buf[128] = "";
        while (fgets(buf, sizeof(buf), fp) != NULL)
        {
            strtok(buf, " ");
            char *uname = strtok(NULL, " ");
            printf("uname = %s\n", uname);
            if (strcmp(uname, username) == 0)
            {
                flg = 1;
                break;
            }
        }

        fseek(fp, 0, SEEK_END);

        if (flg == 1)
        {
            printf("当前账号已登录，请勿重复登录\n");
            fclose(fp);
            return 2;
        }
    }

    char buf1[128] = "";
    // fgets每次读一行
    int flag = 0;
    while (fgets(buf1, sizeof(buf1), fd) != NULL)
    {
        char *uname = strtok(buf1, ":");
        char *pwd = strtok(NULL, ":");
        if (strcmp(uname, username) == 0 && strncmp(pwd, passwd, strlen(pwd)-1) == 0)
        {
            // 登录成功
            flag = 1;
            break;
        }
    }
    fclose(fd);
    return flag;
}

int login_page()
{
    char username[30] = "";
    char passwd[18] = "";
    while (1)
    {
        printf("\n⭐⭐⭐⭐⭐⭐⭐登录界面⭐⭐⭐⭐⭐⭐⭐\n");
        printf("⭐请输入用户名:");
        scanf("%s", username);
        printf("⭐请输入密码:");
        scanf("%s", passwd);
        getchar();

        if (login(username, passwd) == 1)
        {
            printf("⭐登录成功\n");
            return 1;
        }
        else
        {
            printf("\n⭐⭐⭐⭐⭐⭐⭐登录界面⭐⭐⭐⭐⭐⭐⭐\n");
            printf("⭐用户名或密码错误\n");
            printf("⭐A：重新登录\n⭐B:退出登录(输入其他默认为B)\n⭐请输入选项：");
            char opt = 'B';
            scanf("%c", &opt);
            if (opt == 'B')
                break;
        }
    }
    return 0;
}