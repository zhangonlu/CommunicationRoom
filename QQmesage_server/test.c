#include<stdio.h>
#include <unistd.h>
int main(int argc,const char * argv[])
{

        char filename1[40] = "zhang_li";
        char filename2[40] = "li_zhang";
        int flag1 = 0;
        int flag2 = 0;
        access(filename1, F_OK) == -1 ? (flag1 = 0) : (flag1 = 1);
        access(filename2, F_OK) == -1 ? (flag2 = 0) : (flag2 = 1);
        printf("flag1 = %d\n",flag1);
        printf("flag2 = %d\n",flag2);

    return 0;
}