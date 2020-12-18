#include "Platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "libmidiclass.h"
#include "ReturnStatus.h"

int writevarlen(unsigned char *buf,unsigned long value) {
    int i;
    unsigned long val;

    val = value & 0x7f;

    while (value >>= 7) {
        val <<= 8;
        val |= ((value & 0x7f) | 0x80);
    }
    for (i = 0;i<4;i++) {
        buf[i] = val;
        if (val & 0x80)
            val >>= 8;
        else
            break;
    }
    return(i + 1);
}

void checkargs(FILE *&fp,int &fd,int argc,char *argv[]) {
    char name[1024];
    char *cp;

    if (argc == 2) {
        if ((fd = open(argv[1],O_RDONLY)) < 0)
            fatal("open");
        close(fd);
        if ((fp = fopen(argv[1],"r")) < 0)
            fatal("fopen");
        if (strlen(argv[1]) > 1020)
            fatal("filnam too long");
        memcpy(name,argv[1],strlen(argv[1]));
        cp = (strrchr(name,'.')+1);
        *cp++ = 'm';
        *cp++ = 'i';
        *cp++ = 'd';
        *cp++ = 'i';
        *cp = '\0';
        if ((fd = open(name,O_RDWR | O_CREAT,0644)) < 0)
            fatal("open");
    }
    else {
        printf("usage: %s filename\n",argv[0]);
        exit(1);
    }
}
