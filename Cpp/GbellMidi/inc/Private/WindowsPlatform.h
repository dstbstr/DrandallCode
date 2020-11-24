#include <winsock.h>

#include <share.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <iostream>

char* index(char* str, char toFind);
char* rindex(char* str, char toFind);
int getline(char** pline_buf, size_t* pn, FILE * fin);