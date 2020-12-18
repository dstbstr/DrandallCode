#ifndef __UTILS_H__
#define __UTILS_H__

#include "Platform.h"

int writevarlen(unsigned char *buf,unsigned long value);
void checkargs(FILE *&fp,int &fd,int argc,char *argv[]);

#endif // __UTILS_H__