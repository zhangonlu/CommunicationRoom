#include "client.h"
#include "../header.h"
void sigfun(int signo)
{
    if(signo == SIGINT)
    {
        //打印当前时间到屏幕上
        time_t tm = 0;
        time(&tm);
        char *current_time = asctime(localtime(&tm));
        fprintf(stdout,"当前时间:%s",current_time);
    }

    if(signo == SIGTSTP)
    {
        system("cmatrix");
    }

    if(signo == SIGQUIT)
    {
        system("sl");
    }
}

int main(int argc,char *argv[])
{
    if(argc < 3)
    {
        perror("./name + ip +port\n");
        return -1;
    }

            //查看当前时间
    signal(SIGINT,sigfun);

        //代码雨 ctrl + z
    signal(SIGTSTP,sigfun);

        //小火车 ctrl + /
    signal(SIGQUIT,sigfun);
    client_init(argv[1],argv[2]);

    return 0;
}