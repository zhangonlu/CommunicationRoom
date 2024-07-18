#include "initia_page.h"
#include "tcp.h"
#include "login.h"
#include "online_write.h"
#include "friends.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *pthread_fun(void *arg)
{

    user_info user = *(user_info *)arg;
    char *ip = user.ip;
    int port = user.port;
    int cid = user.cid;
    int flag = -1;

    while (1)
    {
        cJSON *json_res = NULL;
        cJSON *json_opt = NULL;
        cJSON *json_username = NULL;
        cJSON *json_passwd = NULL;
        // 等待客户端发送注册登录信息
        char buf[512] = "";
        int flg = recv(cid, buf, sizeof(buf), 0);
        if (flg == 0)
        {
            close(cid);
            pthread_exit(NULL);
        }
        else if (flg == -1)
        {
            perror("recv error\n");
            close(cid);
            pthread_exit(NULL);
        }
        if (strncmp("quit", buf, 4) == 0)
        {
            close(cid);
            pthread_exit(NULL);
        }
        json_res = cJSON_Parse(buf);
        json_opt = cJSON_GetObjectItem(json_res, "opt");
        json_username = cJSON_GetObjectItem(json_res, "username");
        json_passwd = cJSON_GetObjectItem(json_res, "passwd");
        // 打印客户端传过来的登录信息：选择 （A B） 用户名 密码
        // printf("opt: %s  username = %s  passwd = %s\n", json_opt->valuestring, json_username->valuestring, json_passwd->valuestring);
        flag = initia_page(json_opt->valuestring, json_username->valuestring, json_passwd->valuestring);
        // flag = 1成功 =0失败 把登录注册结果发送回去
        char f_buf[5] = "";
        f_buf[0] = flag + 48;
        send(cid, f_buf, strlen(f_buf), 0);
        if (flag == 1)
        {
            strncpy(user.username, json_username->valuestring, strlen(json_username->valuestring));
            break;
        }
    }

    if (flag == 1)
    {
        // 1上线提示 群发

        // 将登录成功的用户信息：名称 ip 端口 写入临时文件(存放当前在线的用户)
        write_user_info(&user);
        cJSON *login_mg = cJSON_CreateObject();
        int num[10] = {0};
        select_user_cid(num);
        int i = 0;
        char login_msg[30] = "";
        sprintf(login_msg, "%s is online\n", user.username);
        cJSON_AddStringToObject(login_mg, "msg", login_msg);
        cJSON_AddStringToObject(login_mg, "opt", "login");
        char *str_login = cJSON_Print(login_mg);
        while (i < 10)
        {
            if (num[i] != 0 && cid != num[i])
                send(num[i], str_login, strlen(str_login), 0);
            i++;
        }

        struct pollfd fd[1] = {0};
        fd[0].fd = cid;
        fd[0].events = POLLIN;

        while (1)
        {
            int count = poll(fd, 1, 3000);

            if (fd[0].revents == POLLIN)
            {
                char buf[512] = "";
                int flg = recv(cid, buf, sizeof(buf), 0);
                // printf("100 recv : %s\nflag = %d", buf, flag);
                if (flg == 0)
                {
                    printf("用户:%s已经退出\n", user.username);
                    delete_user_info(user.username);
                    close(cid);
                    pthread_exit(NULL);
                }
                else if (flg == -1)
                {
                    perror("recv error\n");
                    close(cid);
                    pthread_exit(NULL);
                }
                else
                {
                    cJSON *json_res = NULL;
                    cJSON *json_opt = NULL;
                    cJSON *json_username = NULL;
                    cJSON *json_user_flag = NULL;
                    cJSON *json_msg = NULL;
                    cJSON *json_friend_username = NULL;
                    json_res = cJSON_Parse(buf);
                    json_opt = cJSON_GetObjectItem(json_res, "opt");
                    json_username = cJSON_GetObjectItem(json_res, "username");

                    if (strncmp("A", json_opt->valuestring, 1) == 0)
                    {
                        cJSON *json_user = cJSON_CreateObject();
                        // 查询好友列表 用json传回客户端
                        select_user_online(json_user);
                        char *str = cJSON_Print(json_user);
                        // printf("str = %s",str);
                        send(cid, str, strlen(str), 0);
                    }

                    if (strncmp("B", json_opt->valuestring, 1) == 0)
                    {
                        // 私聊好友 收到客户端的消息 将消息转发给特定用户
                        cJSON *userlist = cJSON_CreateObject();
                        // 解析标识符和消息
                        json_msg = cJSON_GetObjectItem(json_res, "msg");
                        json_user_flag = cJSON_GetObjectItem(json_res, "user_flag");
                        json_friend_username = cJSON_GetObjectItem(json_res, "fusername");

                        char mmsg[512] = "";
                        sprintf(mmsg, "%s%s%s", json_username->valuestring, json_user_flag->valuestring, json_msg->valuestring);
                        // 找到和username 匹配的cid
                        int scid = select_cid_byusername(json_friend_username->valuestring);
                        if (strncmp("quit", json_msg->valuestring, 4) == 0)
                        {
                            
                        }
                        else
                        {
                            save_conversition(buf);
                            send(scid, buf, strlen(buf), 0);
                        }
                    }

                    if (strncmp("C", json_opt->valuestring, 1) == 0)
                    {
                        // 群聊好友 :接受到cJson字符串 处理之后从文件读取到当前在线用户 把消息群发所有在线用户

                        // 保存群聊记录
                        save_conversition(buf);

                        // 读取当前在线好友cid
                        int num[10] = {0};
                        select_user_cid(num);

                        int i = 0;
                        while (i < 10)
                        {
                            if (num[i] != 0 && num[i] != cid)
                                send(num[i], buf, strlen(buf), 0);
                            i++;
                        }
                    }

                    if (strncmp("D", json_opt->valuestring, 1) == 0)
                    {
                        // 接收文件
                        cJSON *filename = cJSON_GetObjectItem(json_res, "filename");
                        static int executed0 = 0; // 静态变量
                        if (!executed0)
                        {
                            int fd = open(filename->valuestring, O_CREAT | O_WRONLY, 0666);
                            close(fd);
                            executed0 = 1;
                        }
                        struct pollfd filefd[1] = {0};
                        filefd[0].fd = cid;
                        filefd[0].events = POLLIN;

                        while (1)
                        {
                            poll(filefd, 1, 3000);
                            if (filefd[0].revents == POLLIN)
                            {
                                char buf_file[1024] = "";
                                int file_flag = recv(cid, buf_file, sizeof(buf), 0);
                                if (file_flag == 0)
                                {
                                    printf("用户:%s已经退出\n", user.username);
                                    delete_user_info(user.username);
                                    close(cid);
                                    pthread_exit(NULL);
                                }
                                else if (file_flag == -1)
                                {
                                    perror("recv error\n");
                                    close(cid);
                                    pthread_exit(NULL);
                                }

                                // printf("%d\n",strncmp(buf_file, "", 4));
                                if (strncmp(buf_file, "quit", 4) == 0)
                                {
                                    printf("传输文件结束\n");
                                    break;
                                }
                                // printf("202 recv : %s\n", buf_file);
                                FILE *fp = fopen(filename->valuestring, "wb");
                                if (fp == NULL)
                                {
                                    perror("open file error\n");
                                }
                                else
                                {
                                    int len = fwrite(buf_file, 1,strlen(buf_file),  fp);
                                    if (len < 0)
                                        printf("写入失败\n");
                                }
                                fclose(fp);
                            }
                        }
                    }

                    if (strncmp("E", json_opt->valuestring, 1) == 0)
                    {
                        // 用户退出 修改当前在线用户的文件
                        delete_user_info(json_username->valuestring);
                        close(cid);
                        pthread_exit(NULL);
                    }
                }
            }
        }
    }
}

int tcp_init(char *ip, char *port)
{
    pthread_mutex_init(&mutex, NULL);

    int sid = socket(AF_INET, SOCK_STREAM, 0);
    if (sid < 0)
    {
        perror("创建套接字失败\n");
        return -1;
    }
    printf("创建套接字成功...\n");
    struct sockaddr_in saddr = {0};
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(ip);
    saddr.sin_port = htons(atoi(port));
    if (bind(sid, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    {
        perror("绑定失败\n");
        return -1;
    }
    printf("绑定成功...\n");

    if (listen(sid, 10) < 0)
    {
        perror("监听失败\n");
        return -1;
    }
    printf("正在监听...\n");
    int cid = -1;
    struct sockaddr_in caddr = {0};
    while (1)
    {
        memset(&caddr, 0, sizeof(caddr));
        cid = -1;
        int size = sizeof(caddr);
        pthread_mutex_lock(&mutex);
        cid = accept(sid, (struct sockaddr *)&caddr, &size);
        pthread_mutex_unlock(&mutex);
        if (cid == -1)
        {
            perror("链接失败\n");
            continue;
        }

        if (cid != -1)
        {
            printf("%d online\n", cid);
            pthread_t tid = 0;
            char ip[18] = "";
            int port = 0;
            port = ntohs(caddr.sin_port);
            inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, sizeof(ip));
            user_info user = {0};
            user.cid = cid;
            strcpy(user.ip, ip);
            user.port = port;
            printf("172port = %d\n", port);
            pthread_create(&tid, NULL, pthread_fun, &user);
            pthread_detach(tid);
        }
    }

    close(sid);
    remove("./user_info.txt");
}