#include "online_write.h"
#include "../header.h"

const char *user_id[] = {
    "->",
    "#",
    "))",
    "]",
    ">",
    "/",
    "[",
    "@",
    "$",
    "^"
};
//标识符使用情况 每次给用户分配当前未使用的最小小标的标识符给用户
int user_id_user[10] = {0};

void write_user_info(user_info *user_info)
{
    static int executed = 0; // 静态变量

    if (!executed) {
        remove("./user_info.txt");
        executed = 1;
    }

    FILE *fd = fopen("./user_info.txt", "a+");
    if (fd == NULL)
    {
        perror("file open error\n");
        return ;
    }


    

    int count = -1;
    for (int i = 0; i < sizeof(user_id_user) / sizeof(user_id_user[0]); i++)
    {
        if(user_id_user[i] == 0)
        {
            user_id_user[i] = 1;
            count = i;
            break;
        }
    }

    if(count == -1)
    {
        printf("标识符用完\n");
        fclose(fd);
        return ;
    }   

    int len = fprintf(fd, "%d %s %s %d %d %s\n",user_info->cid, user_info->username, user_info->ip, user_info->port, count ,user_id[count]);
    if (len < 0)
    {
        printf("写入文件失败\n");
    }

    fclose(fd);
    return;
}



void delete_user_info(char * username)
{
    FILE *fd = fopen("./user_info.txt", "a+");
    if (fd == NULL)
    {
        perror("file open error\n");
        return ;
    }

    FILE *fp = fopen("./user_info_new.txt", "a+");
    if (fp == NULL)
    {
        perror("file open error\n");
        return ;
    }

    char * flag = "";
    char buf[128] = "";
    char buf_orginal[129] = "";
    while (fgets(buf,sizeof(buf),fd) != NULL)
    {
        strcpy(buf_orginal,buf);

        strtok(buf," ");
        char *uname = strtok(NULL," ");
        if(strcmp(uname,username) == 0)
        {
            strtok(NULL," ");
            strtok(NULL," ");
            flag = strtok(NULL," ");
            char s[0];
            strncpy(s,flag,1);
                //释放掉用户的标志符
            int num = s[0] - 48;
            printf("num = %d\n",num);
            user_id_user[num] = 0;
            //找到要删除的登录信息的用户名
            continue;
        }
        fprintf(fp,"%s",buf_orginal);
    }





    fclose(fd);
    fclose(fp);
    
    remove("./user_info.txt");
    rename("./user_info_new.txt","./user_info.txt"); 

}
