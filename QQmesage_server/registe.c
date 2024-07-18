#include "registe.h"

int registe(char * username,char * passwd)
{
    FILE * fd = fopen("./user.txt","a+");
    if(fd  == NULL)
    {
        perror("file open error\n");
        return -1;
    }

    char buf[128] = "";
    //fgets每次读一行
    int flag = 0;
    while (fgets(buf,sizeof(buf),fd) != NULL)
    {
        char *uname = strtok(buf," ");
        char *pwd = strtok(NULL," ");
        if(strcmp(uname,username) == 0 )
        {
            //用户名已被占用 
            flag = -1;
            break;
        }
    }


    fclose(fd); 
    return flag;
}

/*
void  registe_page()
{

    char username[30] = "";
    char passwd[18] = "";
    while (1)
    {
        printf("\n⭐⭐⭐⭐⭐⭐⭐注册界面⭐⭐⭐⭐⭐⭐⭐\n");
        printf("⭐请输入用户名:");
        fflush(stdin);
        scanf("%s", username);
        printf("⭐请输入密码:");
        fflush(stdin);
        scanf("%s", passwd);
        getchar();

        if (registe(username, passwd) == 0)
        {

            //注册成功 把用户名 密码 写入文件
            FILE * fd = fopen("./user.txt","a+");
            if(fd  == NULL)
            {
                perror("file open error\n");
                return -1;
            }
            int len = fprintf(fd,"%s:%s\n",username,passwd);
            if(len > 0)
            printf("注册成功\n");
            fclose(fd);
            break;
        }
        else
        {
            printf("\n⭐⭐⭐⭐⭐⭐⭐注册界面⭐⭐⭐⭐⭐⭐⭐\n");
            printf("⭐用户名已被占用\n");
            printf("⭐A：重新注册\n⭐B:退出注册(输入其他默认为B)\n⭐请输入选项：");
            char opt = 'B';
            scanf("%c",&opt);
            if(opt == 'B')
                break;
        }
    }




}
*/