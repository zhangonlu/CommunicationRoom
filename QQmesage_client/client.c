#include "../header.h"
#include "initia_page.h"
#include "client.h"
#include "main_function_page.h"
#include "online_write.h"
// 解析json
void parse_json_user(cJSON *json_user)
{
    cJSON *userlist = cJSON_GetObjectItem(json_user, "userlist"); // 获取 "userlist" 字段的值（数组）

    // 遍历数组中的每个用户信息
    cJSON *user_entry = NULL;
    cJSON_ArrayForEach(user_entry, userlist)
    {
        cJSON *username = cJSON_GetObjectItem(user_entry, "username"); // 获取用户名字段
        cJSON *ip = cJSON_GetObjectItem(user_entry, "ip");             // 获取 IP 地址字段
        cJSON *port = cJSON_GetObjectItem(user_entry, "port");         // 获取端口号字段

        // 打印用户信息
        printf("Username: %s ", username->valuestring);
        printf("IP: %s ", ip->valuestring);
        printf("Port: %s\n", port->valuestring);
    }
}



int client_init(char *ip, char *port)
{

    int cid = socket(AF_INET, SOCK_STREAM, 0);
    if (cid < 0)
    {
        perror("创建套接字失败\n");
        return -1;
    }
    printf("创建套接字成功...\n");
    struct sockaddr_in saddr = {0};
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(ip);
    saddr.sin_port = htons(atoi(port));

    if (connect(cid, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    {
        perror("连接失败\n");
        return -1;
    }
    printf("连接成功...\n");

    struct pollfd fd[2] = {0};
    fd[0].fd = cid;
    fd[0].events = POLLIN;
    fd[1].fd = STDIN_FILENO;
    fd[1].events = POLLIN;
    int flag = -1;
    user_opt uu = {0};
    while (1)
    {
        user_opt *us = &uu;
        initia_page(us);
        if (strncmp(us->opt, "C", 1) == 0)
        {
            send(cid, "quit", sizeof("quit"), 0);
            close(cid);
            return 0;
        }
        cJSON *user_info = cJSON_CreateObject();
        cJSON_AddStringToObject(user_info, "username", us->username);
        cJSON_AddStringToObject(user_info, "passwd", us->passwd);
        cJSON_AddStringToObject(user_info, "opt", us->opt);
        char *str = cJSON_Print(user_info);
        // printf("str = %s\n", str);
        send(cid, str, strlen(str), 0);
        char buf[5] = "";
        recv(cid, buf, sizeof(buf) - 1, 0);
        // printf("buf = %s\n", buf);
        if (strncmp(us->opt, "A", 1) == 0)
        {
            // 注册结果处理
        }

        if (strncmp(us->opt, "B", 1) == 0)
        {
            // printf("buf = %s\n", buf);
            // 登录结果处理
            if (strncmp("0", buf, 1) == 0)
            {
                // 登录失败
                printf("用户名或密码错误\n");
            }

            if (strncmp("1", buf, 1) == 0)
            {
                // 登录成功
                printf("登录成功\n");
                flag = 1;
                break;
            }
            if (strncmp("2", buf, 1) == 0)
            {
                // 登录失败
                printf("账号已登录 不支持重复登录\n");
            }
        }
    }

    // 登录成功 进入主功能页面
    if (flag == 1)
    {
        while (1)
        {
            Messages msg = {0};
            strcpy(msg.username, uu.username);
            // 得到选项
            mainfunction_page(&msg,cid);
            
            if (strncmp(msg.opt, "A", 1) == 0)
            {
                // 查看在线好友 给服务器发消息 服务器返回在线好友
                 update_online_user(msg.username,cid);
            }

            if (strncmp(msg.opt, "B", 1) == 0)
            {
                // 私聊好友  必须确保好友存在 ：先从本地得到好友的用户名  确保好友存在
                char user_if[10][128] = {0};
                user_list(user_if);
                // printf("%s %s %s",user_if[0],user_if[1],user_if[2]);
                char *use = select_user(user_if);

                // 提前退出私聊
                if (use == NULL)
                    continue;
                printf("私聊对象：%s\n", use);
                // 取出用户标识符
                char user_flag_old[10] = "";
                get_user_flag(user_flag_old, uu.username);
                char user_flag_new[10] = "";
                strncpy(user_flag_new, user_flag_old, strlen(user_flag_old) - 1);

                // 清空输入缓冲区
                // int c;
                // while ((c = getchar()) != '\n' && c != EOF);
                // fflush(stdin);
                while (1)
                {
                    int ret = poll(fd, 2, 3000); // 设置超时时间为 3000 毫秒

                    // if (ret == -1)
                    // {
                    //     perror("poll error");
                    //     break;
                    // }

                    // if (ret == 0)
                    // {
                    //     // 超时事件处理，可以添加相应的逻辑
                    //     continue;
                    // }

                    // 监测标准输入文件描述符
                    if (fd[1].revents & POLLIN)
                    {
                        // printf("%s%s",uu.username,user_flag_new);
                        char mesge[512] = "";
                        fgets(mesge, sizeof(mesge), stdin);

                        // mesge[strlen(mesge) - 1] = '\0';
                        cJSON *msg_info = cJSON_CreateObject();
                        cJSON_AddStringToObject(msg_info, "opt", msg.opt);
                        // 对方的用户名和自己的
                        cJSON_AddStringToObject(msg_info, "username", uu.username);
                        cJSON_AddStringToObject(msg_info, "fusername", use);
                        cJSON_AddStringToObject(msg_info, "msg", mesge);
                        // 从文件中拿到用户标识符
                        cJSON_AddStringToObject(msg_info, "user_flag", user_flag_new);
                        char *str = cJSON_Print(msg_info);
                        // printf("str= %s",str);

                        send(cid, str, strlen(str), 0);
                        if (strncmp("quit", mesge, 4) == 0)
                        {
                            // 退出私聊
                            break;
                        }
                    }

                    // 监测套接字文件描述符 接收私聊消息 
                    if (fd[0].revents & POLLIN)
                    {
                        char rev_buf[512] = "";
                        int flg = recv(cid, rev_buf, sizeof(rev_buf), 0);
                        if (flg == 0)
                        {
                            printf("服务器断开连接\n");
                            close(cid);
                        }
                        else if (flg == -1)
                        {
                            perror("recv error\n");
                            close(cid);
                            return -1;
                        }
                        else
                        {
                            cJSON *res = cJSON_Parse(rev_buf);
                            char msg1[512] = "";
                            cJSON* json_username = cJSON_GetObjectItem(res,"username");
                            cJSON* json_user_flag = cJSON_GetObjectItem(res,"user_flag");
                            cJSON* json_msg = cJSON_GetObjectItem(res,"msg");
                            sprintf(msg1,"%s%s%s",json_username->valuestring,json_user_flag->valuestring,json_msg->valuestring);
                            printf("%s", msg1);
                        }
                    }
                }
            }

            if (strncmp(msg.opt, "C", 1) == 0)
            {

                printf("群发模式：\n");
                // int c;
                // while ((c = getchar()) != '\n' && c != EOF);

                while (1)
                {
                    poll(fd, 2, 3000);
                    // 监测到键盘输入
                    if (fd[1].revents == POLLIN)
                    {
                        // 群聊 把消息发封装为cjson字符串发送到服务器 然后服务器群发所有在线好友
                        cJSON *json_userinfo = cJSON_CreateObject();
                        // printf("230 username: %s\n",msg.username);
                        cJSON_AddStringToObject(json_userinfo, "username", msg.username);
                        cJSON_AddStringToObject(json_userinfo, "opt", msg.opt);

                        char user_flag_old[10] = "";
                        get_user_flag(user_flag_old, uu.username);
                        char user_flag_new[10] = "";
                        strncpy(user_flag_new, user_flag_old, strlen(user_flag_old) - 1);
                        cJSON_AddStringToObject(json_userinfo, "user_flag", user_flag_new);
                        char msg[512] = "";
                        fgets(msg, sizeof(msg), stdin);

                        cJSON_AddStringToObject(json_userinfo, "msg", msg);
                        char *str = cJSON_Print(json_userinfo);
                        
                        send(cid, str, strlen(str), 0);
                        if (strncmp("quit", msg, 4) == 0)
                        {
                            // 退出群聊
                            break;
                        }
                    }
                        //接到群聊消息
                    if (fd[0].revents == POLLIN)
                    {
                        int flags = fcntl(cid, F_GETFL, 0);
                        flags |= O_NONBLOCK;
                        fcntl(cid, F_SETFL, flags);

                        char rev_buf[512] = "";
                        int flg = recv(cid, rev_buf, sizeof(rev_buf), 0);
                        if (flg == 0)
                        {
                            printf("服务器断开连接\n");
                            close(cid);
                        }
                        else if (flg == -1)
                        {
                            perror("recv error\n");
                            close(cid);
                            return -1;
                        }
                        else
                        {
                            cJSON *res = cJSON_Parse(rev_buf);
                            char msg1[512] = "";
                            cJSON* json_username = cJSON_GetObjectItem(res,"username");
                            cJSON* json_user_flag = cJSON_GetObjectItem(res,"user_flag");
                            cJSON* json_msg = cJSON_GetObjectItem(res,"msg");
                            sprintf(msg1,"%s%s%s",json_username->valuestring,json_user_flag->valuestring,json_msg->valuestring);
                            printf("%s", msg1);
                        }
                        flags = fcntl(cid, F_GETFL, 0);
                        flags &= ~O_NONBLOCK;
                        fcntl(cid, F_SETFL, flags);
                    }
                }
            }


            if (strncmp(msg.opt, "D", 1) == 0)
            {
                //发送文件 
                printf("请输入发送的文件名：");
                char filename[100] = "";
                fgets(filename,sizeof(filename),stdin);
                filename[strlen(filename)-1] = '\0';
                char *username = msg.username;
                printf("300 filename = %s\n",filename);
                cJSON * json_file = cJSON_CreateObject();
                cJSON_AddStringToObject(json_file,"filename",filename);
                cJSON_AddStringToObject(json_file,"opt",msg.opt);  
                cJSON_AddStringToObject(json_file,"username",username);   
                char *str = cJSON_Print(json_file);
                //通知服务器准备接受文件
                send(cid,str,strlen(str),0);
                if (send_file(filename,cid) < 0 )
                    continue;
                while (1)
                {
                    printf("文件发送完成，输入“quit”结束发送\n");
                    char buf_1[10] = "";
                    fgets(buf_1,sizeof(buf_1),stdin);
                    send(cid,buf_1,strlen(buf_1),0);
                    if(strncmp("quit",buf_1,4) == 0)
                        break;
                }
                
                
            }

            if (strncmp(msg.opt, "E", 1) == 0)
            {
                printf("已退出\n");
                cJSON *msg_info = cJSON_CreateObject();
                cJSON_AddStringToObject(msg_info, "username", msg.username);
                cJSON_AddStringToObject(msg_info, "opt", msg.opt);
                char *str = cJSON_Print(msg_info);
                // 向服务器发送退出信号
                send(cid, str, strlen(str), 0);
                close(cid);
                break;
            }


        }

    }
}
