#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "libmidiclass.h"
#include "libdefs.h"

mthd_hdr mc;
mtrk_hdr mh;
eot end;
track trk = {0,NULL,0,60,0,0,1,-1,NULL};
int trkexists[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int
main(int argc,char *argv[])
{
    FILE *fp;
    char *buf = NULL;
    size_t linesz = 0;
    char *arg1;
    char *arg2;
    int fd;
    track *tp;
    int preparse = 0;
    int trck = 0;

    checkargs(fp,fd,argc,argv);
    while (getline(&buf,&linesz,fp) != EOF) {
        char *cp = buf;
        if ((trck = preparselin(cp)) == -1)
            ;
        else
            if (!preparse)
                preparse = 1;
        if (parselin(cp,':',arg1,arg2) == -1)
            continue;
        dofuncs(trck,arg1,arg2);
    }
    free(buf);
    if (preparse)
        newendtrk(mh,end,tp);
    else
        endtrk(mh,end,tp);
    writemidifil(fd,mc);
    if (fclose(fp))
        warning("fclose");
    if (close(fd))
        warning("close");
    exit(0);
}