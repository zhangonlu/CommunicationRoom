#include "../header.h"
#include "main_function_page.h"

void mainfunction_page(Messages *msg, int cid)
{
    printf("\n⭐⭐⭐⭐⭐⭐⭐QQ聊天室⭐⭐⭐⭐⭐⭐⭐\n");
    printf("⭐A:查看在线好友                  ⭐\n");
    printf("⭐B:私聊好友                      ⭐\n");
    printf("⭐C:群聊好友                      ⭐\n");
    printf("⭐D:发送文件                      ⭐\n");
    printf("⭐E:退出                          ⭐\n");
    printf("⭐⭐⭐⭐⭐⭐⭐QQ聊天室⭐⭐⭐⭐⭐⭐⭐\n");
    printf("⭐请输入您的选择：\n");
    char opt = ' ';
    char c;

    struct pollfd fds[2];
    fds[0].fd = cid;
    fds[1].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    fds[1].events = POLLIN;

    while (1)
    {
        poll(fds, 2, 3000);
        if (fds[0].revents & POLLIN)
        {
            char buf[512] = "";
            recv(cid, buf, sizeof(buf), 0);
            cJSON *res = cJSON_Parse(buf);
            cJSON *opt = cJSON_GetObjectItem(res,"opt");
            cJSON *msg = cJSON_GetObjectItem(res,"msg");
            if (strncmp(opt->valuestring, "login", 5) == 0)
            {
                printf("%s", msg->valuestring);
            }

            if (strncmp(opt->valuestring, "C", 1) == 0)
            {
                char msg1[512] = "";
                cJSON* json_username = cJSON_GetObjectItem(res,"username");
                cJSON* json_user_flag = cJSON_GetObjectItem(res,"user_flag");
                cJSON* json_msg = cJSON_GetObjectItem(res,"msg");
                sprintf(msg1,"%s%s%s",json_username->valuestring,json_user_flag->valuestring,json_msg->valuestring);
                printf("群聊消息：%s", msg1);
            }

            if (strncmp(opt->valuestring, "B", 1) == 0)
            {
                char msg1[512] = "";
                cJSON* json_username = cJSON_GetObjectItem(res,"username");
                cJSON* json_user_flag = cJSON_GetObjectItem(res,"user_flag");
                cJSON* json_msg = cJSON_GetObjectItem(res,"msg");
                sprintf(msg1,"%s%s%s",json_username->valuestring,json_user_flag->valuestring,json_msg->valuestring);
                printf("私聊消息：%s", msg1);
            }
        }

        if (fds[1].revents & POLLIN)
        {
            scanf("%c", &opt);
            while ((c = getchar()) != '\n' && c != EOF)
                ;

            switch (opt)
            {
            case 'A':
                msg->opt[0] = opt;
                break;
            case 'B':
                msg->opt[0] = opt;
                break;
            case 'C':
                msg->opt[0] = opt;
                break;
            case 'D':
                msg->opt[0] = opt;
                break;
            case 'E':
                msg->opt[0] = opt;
                break;
            default:
                printf("请输入正确的选项，谢谢\n");
                break;
            }
            break;
        }
    }
}

// 在登录成功的时候 将用户名和标识符写入本地文件

void personal_info(Messages *msg)
{
    static int executed = 0; // 静态变量

    if (!executed)
    {
        remove("./personal_info.txt");
        executed = 1;
    }

    FILE *fd = fopen("./personal_info.txt", "a+");
    if (fd == NULL)
    {
        perror("file open error\n");
        return;
    }

    char buf[128] = "";

    int len = fprintf(fd, "%s%s\n", msg->username, msg->user_flag);

    if (len < 0)
    {
        printf("写入文件失败\n");
    }

    fclose(fd);
    return;
}

// 读取本地的在线好友列表

void user_list(char list[][128])
{
    FILE *fd = fopen("./user_info.txt", "r");
    if (fd == NULL)
    {
        perror("file open error\n");
        return;
    }

    char buf[128] = "";
    int i = 0;
    while (fgets(buf, sizeof(buf), fd) != NULL)
    {
        // 复制用户名到字符串数组中
        char *username = strtok(buf, " ");
        strcpy(list[i], username);
        i++;
    }

    // for ( i = 0; i < 10 && (strlen(list[i]) != 0); i++)
    // {
    //     printf("105:%s\n",list[i]);
    // }

    fclose(fd);
}

char *select_user(char uname_list[][128])
{

    printf("\n⭐⭐⭐⭐⭐⭐⭐私聊好友⭐⭐⭐⭐⭐⭐⭐\n");

    for (int i = 0; i < 10 && uname_list[i] != NULL && (strlen(uname_list[i]) != 0); i++)
    {
        printf("%d : %s\n", i + 1, uname_list[i]);
    }

    printf("⭐(输入0退出选择)\n");
    printf("⭐请输入您的私聊对象：");
    int opt = -1;
    scanf("%d", &opt);
    getchar();
    if (opt == 0)
    {
        return NULL;
    }

    if (uname_list[opt - 1] == NULL)
    {
        printf("选择对象不和法\n");
    }
    return uname_list[opt - 1];
}

// 将用户的标识符从文件取出
void get_user_flag(char *user_flag, char *username)
{
    FILE *fd = fopen("./user_info.txt", "r");
    if (fd == NULL)
    {
        perror("file open error\n");
        return;
    }
    char buf[128] = "";
    char *uname;
    while (fgets(buf, sizeof(buf), fd) != NULL)
    {
        // 复制用户标识符到字符串数组中
        uname = strtok(buf, " ");
        strtok(NULL, " ");
        strtok(NULL, " ");
        char *us_flag = strtok(NULL, " ");
        if (strcmp(username, uname) == 0)
        {
            strcpy(user_flag, us_flag);
            fclose(fd);
            return;
        }
    }

    fclose(fd);
}

// 保存聊天记录:私聊

void save_conversition(char *str)
{
    cJSON *res = cJSON_Parse(str);
    cJSON *username = cJSON_GetObjectItem(res, "username");
    cJSON *fusername = cJSON_GetObjectItem(res, "fusername");
    cJSON *user_flag = cJSON_GetObjectItem(res, "user_flag");
    cJSON *msg = cJSON_GetObjectItem(res, "msg");
    char filename[40] = "";
    sprintf(filename, "%s_%s", username->valuestring, fusername->valuestring);
    FILE *fp = fopen(filename, "a+");
    if (fp == NULL)
    {
        perror("file open error\n");
        return;
    }

    time_t tm;
    time(&tm);
    fprintf(fp, "%s:%s%s%s\n", asctime(localtime(&tm)), username->valuestring, user_flag->valuestring, msg->valuestring);

    fclose(fp);
}






int send_file(char *filename,int sockfd)
{   

    if(access(filename,F_OK) == -1)
    {
        perror("文件不存在\n");
        return -1;
    }
     // 打开要发送的文件
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
    }
    char buffer[1024] = "";
    int i = 0;
    // 读取文件内容 封装为cJson对象 发送

    while (!feof(file)) {
        size_t bytesRead = fread(buffer,  1,sizeof(buffer), file);
        if (bytesRead < 0) {
            perror("Error reading from file");
        }
        printf("i = %d\n",i);
        i++;

        if (send(sockfd,buffer, strlen(buffer), 0) < 0) {
            perror("Error sending data");
        }

    }

    // 关闭文件和套接字
    fclose(file);
    return 0;
}