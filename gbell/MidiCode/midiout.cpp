#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "libmidiclass.h"
#include "libdefs.h"

void
writemidifil(int fd,mthd_hdr mc)

{
    extern track trk;
    track *tp = &trk;

    mc.numtrks = htons(mc.numtrks);
    if (write(fd,&mc,14) < 0)
        fatal("write");
    while (tp->np) {
       if (write(fd,tp->datap,tp->datalen + HDRSIZ) < 0)
            warning("write");
       tp = tp->np;
    }
    if (write(fd,tp->datap,tp->datalen + HDRSIZ) < 0)
        warning("write");
}
