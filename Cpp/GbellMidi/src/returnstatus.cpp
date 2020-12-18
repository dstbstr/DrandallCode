#include "ReturnStatus.h"
#include <stdio.h>
#include <stdlib.h>

void error(const char *str) {
    fprintf(stderr,"%s\n",str);
    exit(1);
}

void fatal(const char *str) {
    perror(str);
    exit(1);
}

void warning(const char *str) {
    perror(str);
}
