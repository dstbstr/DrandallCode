#include "MidiOut.h"

#include "Platform.h"
#include "Constants.h"
#include "ReturnStatus.h"

#include <stdio.h>

void writemidifil(int fd,mthd_hdr mc) {
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
