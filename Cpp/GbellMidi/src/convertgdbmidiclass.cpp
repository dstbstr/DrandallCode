#include "Platform.h"
#include "libdefs.h"
#include "libmidiclass.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

mthd_hdr mc;
mtrk_hdr mh;
eot end;
track trk = {0, NULL, 0, 60, 0, 0, 1, -1, NULL};
int trkexists[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int main(int argc, char* argv[]) {
    FILE* fp;
    char buf[1024];
    int linesz = 1024;
    char* arg1;
    char* arg2;
    int fd;
    track* tp;
    int preparse = 0;
    int trck = 0;

    checkargs(fp, fd, argc, argv);
    while(fgets(buf, linesz, fp) != NULL) {
        char* cp = buf;
        if((trck = preparselin(cp)) == -1)
            ;
        else if(!preparse)
            preparse = 1;
        if(parselin(cp, ':', arg1, arg2) == -1)
            continue;
        dofuncs(trck, arg1, arg2);
    }
    if(preparse)
        newendtrk(mh, end, tp);
    else
        endtrk(mh, end, tp);
    writemidifil(fd, mc);
    if(fclose(fp))
        warning("fclose");
    if(close(fd))
        warning("close");
    exit(0);
}
