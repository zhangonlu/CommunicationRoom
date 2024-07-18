#include "tcp.h"

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>


int main(int argc, char * argv[])
{

    if(argc < 3)
    {
        perror("./name + ip +port\n");
        return -1;
    }
    
    //清空文件userinfo.txt
    remove("./user_info.txt");

    tcp_init(argv[1],argv[2]);
    

    return 0;
}