#include <stdio.h>

int
main(int argc,char *argv[])
{
    int i = 0;

    while (i < 4) {
        argc == 2 ? i++ != 2 ? printf("Happy Birthday to you\n") : \
        printf("Happy Birthday dear %s\n",argv[1]) : \
        (i = 4) && printf("usage: %s name\n",argv[0]);
    }
}
