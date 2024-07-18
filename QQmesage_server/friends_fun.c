#include "friends.h"

// 查询在线好友
void select_user_online(cJSON *json_user)
{
    FILE *fd = fopen("./user_info.txt", "r");
    if (fd == NULL)
    {
        perror("file open error\n");
        return;
    }

    char buf[128] = "";
    int flag = 0;
    cJSON *json_array = cJSON_CreateArray();
    while (fgets(buf, sizeof(buf), fd) != NULL)
    {
        cJSON *json_point = cJSON_CreateObject();
        // cJSON * json_uname = NULL;
        // cJSON * json_ip = NULL;
        // cJSON * json_port = NULL;
        json_point = cJSON_CreateObject();
        strtok(buf, " ");
        char *uname = strtok(NULL, " ");
        char *ip = strtok(NULL, " ");
        char *port = strtok(NULL, " ");
        char *flag_id = strtok(NULL, " ");
        char *user_flag = strtok(NULL, " ");
        cJSON_AddStringToObject(json_point, "username", uname);
        cJSON_AddStringToObject(json_point, "ip", ip);
        cJSON_AddStringToObject(json_point, "port", port);
        cJSON_AddStringToObject(json_point, "flag_id", flag_id);
        cJSON_AddStringToObject(json_point, "user_flag", user_flag);
        cJSON_AddItemToArray(json_array, json_point);
    }

    cJSON_AddItemToObject(json_user, "userlist", json_array);

    fclose(fd);
}

void select_user_cid(int *num)
{
    FILE *fd = fopen("./user_info.txt", "r");
    if (fd == NULL)
    {
        perror("file open error\n");
        return;
    }
    printf("52 msg \n");

    char buf[128] = "";
    int flag = 0;
    while (fgets(buf, sizeof(buf), fd) != NULL)
    {
        char *cid = strtok(buf, " ");
        char des[5] = "";
        strncpy(des, cid, 1);
        *num = des[0] - 48;
        num++;
    }

    fclose(fd);
}

// 根据username 找到匹配的cid

int select_cid_byusername(char *username)
{
    FILE *fd = fopen("./user_info.txt", "r");
    if (fd == NULL)
    {
        perror("file open error\n");
        return -1;
    }

    char buf[128] = "";
    // fgets每次读一行
    int flag = 0;
    cJSON *json_array = cJSON_CreateArray();
    while (fgets(buf, sizeof(buf), fd) != NULL)
    {
        char *cid = strtok(buf, " ");
        char *uname = strtok(NULL, " ");
        if (strncmp(username, uname, sizeof(username)) == 0)
        {
            int pcid = atoi(cid);
            return pcid;
        }
    }
    return 0;
}

void save_conversition(char *str)
{
    cJSON *res = cJSON_Parse(str);
    cJSON *username = cJSON_GetObjectItem(res, "username");
    cJSON *fusername = cJSON_GetObjectItem(res, "fusername");
    cJSON *user_flag = cJSON_GetObjectItem(res, "user_flag");
    cJSON *msg = cJSON_GetObjectItem(res, "msg");
    cJSON *opt = cJSON_GetObjectItem(res, "opt");

    // 私聊 zhang->li 和li->hang 写入一个文件
    if (strncmp(opt->valuestring, "B", 1) == 0)
    {
        char filename1[40] = "";
        char filename2[40] = "";
        sprintf(filename1, "%s_%s", username->valuestring, fusername->valuestring);
        sprintf(filename2, "%s_%s",fusername->valuestring,username->valuestring);

        int flag1 = 0;
        int flag2 = 0;
        access(filename1, F_OK) == -1 ? (flag1 = 0) : (flag1 = 1);
        access(filename2, F_OK) == -1 ? (flag2 = 0) : (flag2 = 1);
        FILE *fp = NULL;
        if (flag1 == 0 && flag2 == 0)
        {
            fp = fopen(filename1, "a+");
            fclose(fp);
        }

        access(filename1, F_OK) == -1 ? (flag1 = 0) : (flag1 = 1);
          
        if (flag1)
            fp = fopen(filename1, "a+");
        else 
            fp = fopen(filename2, "a+");

        if (fp == NULL)
        {
            perror("file open error\n");
            return;
        }

        time_t tm;
        time(&tm);

        fprintf(fp, "%s%s%s%s\n", asctime(localtime(&tm)),
                username->valuestring, user_flag->valuestring, msg->valuestring);
        
        fclose(fp);
    }

    // 群聊 文件名固定为 all_conversition
    if (strncmp(opt->valuestring, "C", 1) == 0)
    {
        char *filename = "all_conversition";
        FILE *fp = fopen(filename, "a+");
        if (fp == NULL)
        {
            perror("file open error\n");
            return;
        }
        time_t tm;
        time(&tm);

        fprintf(fp, "%s%s%s%s\n", asctime(localtime(&tm)),
                username->valuestring, user_flag->valuestring, msg->valuestring);
        fclose(fp);
    }
}