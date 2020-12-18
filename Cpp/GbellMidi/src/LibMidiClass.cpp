#include "Platform.h"
#include <string.h>
#include "libmidiclass.h"

extern long ppqn;

mthd_hdr::mthd_hdr()
{
    memcpy(id,"MThd",4);
    length = htonl(6);
    format = htons(1);
    numtrks = 0;
    division = htons(ppqn);
}

mtrk_hdr::mtrk_hdr()
{
    memcpy(id,"MTrk",4);
    length = 0;
}

eot::eot()
{
    stat = 0xff;
    eotm = 0x2f;
    eots = 0x00;
}

tsig::tsig()
{
    stat = 0xff;
    mnum = 0x58;
    msgbytes = 0x04;
    cc = 0x60;
    bb = 0x08;
}

tmpo::tmpo()
{
    stat = 0xff;
    mnum = 0x51;
    msgbytes = 0x03;
}

keysig::keysig()
{
    stat = 0xff;
    mnum = 0x59;
    msgbytes = 0x02;
}

susmsg::susmsg()
{
    sus = 0x40;
}

