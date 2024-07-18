#include "online_write.h"

void write_user_info(user_info *user_info,int len)
{

    remove("./user_info.txt");


    FILE *fd = fopen("./user_info.txt", "a+");
    if (fd == NULL)
    {
        perror("file open error\n");
        return ;
    }
    // printf("15\n");

    char buf[128] = "";
    // printf("len = %d\n",len);
    for (int l = 0; l < len; l++)
    {
        // printf("%s %s %d %s",user_info[l].username,user_info[l].ip,user_info[l].port,user_info[l].user_flag);
        int size = fprintf(fd,"%s %s %d %s",user_info[l].username,user_info[l].ip,user_info[l].port,user_info[l].user_flag);
        if(size < 0)
        {
            printf("写入失败\n");
        }
    }
    
    // printf("25\n");


    fclose(fd);

}



//更新在线好友 
void update_online_user(char *username,int cid)
{                
    // 查看在线好友 给服务器发消息 服务器返回在线好友
    cJSON *msg_info = cJSON_CreateObject();
    cJSON_AddStringToObject(msg_info, "opt", "A");
    cJSON_AddStringToObject(msg_info, "username", username);
    char *str = cJSON_Print(msg_info);

    send(cid, str, strlen(str), 0);

    char buf[512] = "";
    recv(cid, buf, sizeof(buf), 0);

    // printf("48 %s\n",buf);

    cJSON *json_user = cJSON_Parse(buf);
    cJSON *userlist = cJSON_GetObjectItem(json_user, "userlist"); // 获取 "userlist" 字段的值（数组）

    // 遍历数组中的每个用户信息
    cJSON *user_entry = NULL;
    user_info user_info[10] = {0};
    int i = 0;
   if(userlist == NULL)
   {
    printf("NULL userlist\n");
   }

  if( cJSON_IsArray(userlist) == 0)
  {
    printf("cJSONanoarray\n");
  }

    int array_size = cJSON_GetArraySize(userlist);

    // printf("arraysize = %d\n",array_size);

    for (int i = 0; i < array_size; i++) {
        cJSON *user_entry = cJSON_GetArrayItem(userlist, i);
        // 处理每个用户条目
        cJSON *username = cJSON_GetObjectItem(user_entry, "username");
        cJSON *ip = cJSON_GetObjectItem(user_entry, "ip");
        cJSON *port = cJSON_GetObjectItem(user_entry, "port");
        cJSON *flag_id = cJSON_GetObjectItem(user_entry, "flag_id");
        cJSON *user_flag = cJSON_GetObjectItem(user_entry, "user_flag");
        strcpy(user_info[i].ip, ip->valuestring);
        user_info[i].flag_id = atoi(flag_id->valuestring);
        strcpy(user_info[i].username, username->valuestring);
        strcpy(user_info[i].user_flag, user_flag->valuestring);
                    // 打印用户信息
        printf("Username: %s", username->valuestring);
        printf(" IP: %s ", ip->valuestring);
        printf(" Port: %s\n", port->valuestring);
    }


//    for (int l = 0; l < array_size; l++)
//     {
//         printf("%s %s %d %s",user_info[l].username,user_info[l].ip,user_info[l].port,user_info[l].user_flag);

//     }
    write_user_info(user_info,array_size);
            
}