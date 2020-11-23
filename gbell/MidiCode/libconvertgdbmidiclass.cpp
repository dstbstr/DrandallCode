#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <strings.h>
#include "midiclass.h"
#include "libmidiclass.h"
#define HDRSIZ 8

void
error(const char *str)

{
    fprintf(stderr,"%s\n",str);
    exit(1);
}

void
fatal(const char *str)

{
    perror(str);
    exit(1);
}

void
warning(const char *str)

{
    perror(str);
}

void
trkfill(track *&tp,void *var1,int sz1,void *var2,int sz2,int type)

{
    int i;

    if ((tp->datap = (char *)realloc(tp->datap,HDRSIZ + tp->datalen + sz1 + sz2)) == NULL)
        fatal("realloc");
    if (type == 1) {
        memcpy(tp->datap + HDRSIZ + tp->datalen,var1,sz1);
        (tp->datalen) += sz1;
    }
    else  {
        if (type == 2) {
            memcpy(tp->datap + HDRSIZ + tp->datalen,var1,1);
            (tp->datalen)++;
            for (i = 1;i < sz1;i++) {
                memcpy(tp->datap + HDRSIZ + tp->datalen,(char *)var1+i,1);
                (tp->datalen)++;
            }
        }
    }
    memcpy(tp->datap + HDRSIZ + tp->datalen,var2,sz2);
    (tp->datalen) += sz2;
}

void
parsekey(char *arg,keysig &keystr,track *tp)

{
    int dp = 0;
    int type = 1;

    if (!strcmp(arg,"Cb")) {
        keystr.fltshrp = htons(-7)/256;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"Fb")) {
        keystr.fltshrp = htons(-6)/256;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"Db")) {
        keystr.fltshrp = htons(-5)/256;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"Ab")) {
        keystr.fltshrp = htons(-4)/256;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"Eb")) {
        keystr.fltshrp = htons(-3)/256;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"Bb")) {
        keystr.fltshrp = htons(-2)/256;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"F")) {
        keystr.fltshrp = htons(-1)/256;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"C")) {
        keystr.fltshrp = 0;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"G")) {
        keystr.fltshrp = htons(1)/256;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"D")) {
        keystr.fltshrp = htons(2)/256;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"A")) {
        keystr.fltshrp = htons(3)/256;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"E")) {
        keystr.fltshrp = htons(4)/256;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"B")) {
        keystr.fltshrp = htons(5)/256;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"F#")) {
        keystr.fltshrp = htons(6)/256;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"C#")) {
        keystr.fltshrp = htons(7)/256;
        keystr.majmin = 0;
    }
    if (!strcmp(arg,"ab")) {
        keystr.fltshrp = htons(-7)/256;
        keystr.majmin = htons(1)/256;
    }
    if (!strcmp(arg,"db")) {
        keystr.fltshrp = htons(-6)/256;
        keystr.majmin = htons(1)/256;
    }
    if (!strcmp(arg,"bb")) {
        keystr.fltshrp = htons(-5)/256;
        keystr.majmin = htons(1)/256;
    }
    if (!strcmp(arg,"f")) {
        keystr.fltshrp = htons(-4)/256;
        keystr.majmin = htons(1)/256;
    }
    if (!strcmp(arg,"c")) {
        keystr.fltshrp = htons(-3)/256;
        keystr.majmin = htons(1)/256;
    }
    if (!strcmp(arg,"g")) {
        keystr.fltshrp = htons(-2)/256;
        keystr.majmin = htons(1)/256;
    }
    if (!strcmp(arg,"d")) {
        keystr.fltshrp = htons(-1)/256;
        keystr.majmin = htons(1)/256;
    }
    if (!strcmp(arg,"a")) {
        keystr.fltshrp = 0;
        keystr.majmin = htons(1)/256;
    }
    if (!strcmp(arg,"e")) {
        keystr.fltshrp = htons(1)/256;
        keystr.majmin = htons(1)/256;
    }
    if (!strcmp(arg,"b")) {
        keystr.fltshrp = htons(2)/256;
        keystr.majmin = htons(1)/256;
    }
    if (!strcmp(arg,"f#")) {
        keystr.fltshrp = htons(3)/256;
        keystr.majmin = htons(1)/256;
    }
    if (!strcmp(arg,"c#")) {
        keystr.fltshrp = htons(4)/256;
        keystr.majmin = htons(1)/256;
    }
    if (!strcmp(arg,"g#")) {
        keystr.fltshrp = htons(5)/256;
        keystr.majmin = htons(1)/256;
    }
    if (!strcmp(arg,"d#")) {
        keystr.fltshrp = htons(6)/256;
        keystr.majmin = htons(1)/256;
    }
    if (!strcmp(arg,"a#")) {
        keystr.fltshrp = htons(7)/256;
        keystr.majmin = htons(1)/256;
    }
    trkfill(tp,&dp,1,&keystr,5,type);
}

int
parselin(char *line,char parsechr,char *&arg1,char *&arg2)

{
    char *cp;
    char *iptr;

    cp=line;
    if ((iptr=index(cp,parsechr)) == NULL)
        return(-1);
    arg2 = (iptr + 1);
    *iptr = '\0';
    arg1 = cp;
    for (cp = arg2;*cp != '\n';cp++)
        ;
    *cp = '\0';
    return(0);
}

int
writevarlen(unsigned char *buf,unsigned long value)

{
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

void
parsetim(char *arg,tsig &ts,track *tp)

{
    char *cp = index(arg,'/');
    int len;
    unsigned char timbuf[4] = {'\0','\0','\0','\0'};
    int type = 2;

    *cp = '\0';
    cp++;

    ts.nn = htons(atoi(arg))/256;
    ts.dd = atoi(cp);
    switch (ts.dd) {
        case 2:
            ts.dd = 0x01;
            break;
        case 4:
            ts.dd = 0x02;
            break;
        case 8:
            ts.dd = 0x03;
            break;
        case 16:
            ts.dd = 0x04;
            break;
        case 32:
            ts.dd = 0x05;
            break;
        default:
            ts.dd = 0x02;
            break;
    }

    len = writevarlen(timbuf,tp->notelen);
    trkfill(tp,timbuf,len,&ts,7,type);
}

void
parsetmp(char *arg,tmpo &tmp,int skip,track *tp)

{
    int dp = 0;
    int len;
    unsigned char tmpobuf[4] = {'\0','\0','\0','\0'};
    int tmpo = atoi(arg);
    long bpm = 60000000 / tmpo;
    int type=2;

    tmp.buf[2] = bpm;
    bpm >>= 8;
    tmp.buf[1] = bpm;
    bpm >>= 8;
    tmp.buf[0] = bpm;

    if (skip == 0) {
        type = 1;
        trkfill(tp,&dp,1,&tmp,6,type);
    }
    else {
        len = writevarlen(tmpobuf,skip);
        trkfill(tp,tmpobuf,len,&tmp,6,type);
    }
}

int
chnlnum(track *tp)

{
    int trk = tp->trck;
    int ornum;

    if (trk < 9)
        ornum = trk;
    else
        ornum = trk + 1;
    if (tp->chnl != -1)
        ornum = tp->chnl;
    return(ornum);
}

void
sustain(char *arg,track *tp)

{
    susmsg sus;
    int len;
    unsigned char susbuf[4] = {'\0','\0','\0','\0'};
    int type=2;

    len = writevarlen(susbuf,tp->notelen);
    sus.mnum = 0xb0 | chnlnum(tp);
    if (!strcmp(arg,"ON") ||
        !strcmp(arg,"on"))
        sus.onoff = 0x7f;
    else
        sus.onoff = 0x00;
    trkfill(tp,susbuf,len,&sus,3,type);
    tp->ticks += tp->notelen;
}

char *
getnot(char *cp,int *note)

{
    char *cpp = cp;
    int oct;

    switch (*cpp) {
        case 'c':
             *note = 12;
             break;
        case 'd':
             *note = 14;
             break;
        case 'e':
             *note = 16;
             break;
        case 'f':
             *note = 17;
             break;
        case 'g':
             *note = 19;
             break;
        case 'a':
             *note = 21;
             break;
        case 'b':
             *note = 23;
             break;
        case 'r':
             *note = 0;
             break;
        default:
             *note = 0;
             break;
    }
    cpp++;
    if (*cpp == 's' || *cpp == '#') {
        (*note)++;
        cpp++;
    }
    if (*cpp == 'b') {
        (*note)--;
        cpp++;
    }
    oct = atoi(cpp);
    *note = *note + (12 * oct);
    return(cpp);
}

void
parsechord(char *arg,track *tp,int notelen)

{
    char *cp;
    int note;
    unsigned char buf[4] = {'\0','\0','\0','\0'};
    notemsg msg;
    int len;
    int type;

    buf[0] = '\0';
    buf[1] = '\0';
    buf[2] = '\0';
    buf[3] = '\0';

    for (cp = arg;*cp;cp++) {
        len = 0;
        cp = getnot(cp,&note);
        msg.note = note;
        msg.vel = tp->vel;
        if (note!= 0) {
            msg.mnum = 0x90 | chnlnum(tp);
            type=1;
            trkfill(tp,&len,1,&msg,3,type);
        }
    }
    tp->ticks += notelen;
    len = writevarlen(buf,notelen);
    cp = arg;
    cp = getnot(cp,&note);
    msg.note = note;
    msg.mnum = 0x80 | chnlnum(tp);
    type=2;
    trkfill(tp,buf,len,&msg,3,type);
    cp++;
    for (;*cp;cp++) {
        len = 0;
        cp = getnot(cp,&note);
        msg.note = note;
        msg.vel = tp->vel;
        if (note != 0) {
            msg.mnum = 0x80 | chnlnum(tp);
            type=1;
            trkfill(tp,&len,1,&msg,3,type);
        }
    }
}

void
parsenot(char *arg,track *tp,int notelen)

{
    char *cp;
    int note;
    unsigned char buf[4] = {'\0','\0','\0','\0'};
    notemsg msg;
    int len;
    int type;

    for (cp = arg;*cp;cp++) {
        len = 0;
        cp = getnot(cp,&note);
        msg.note = note;
        msg.vel = tp->vel;
        if (note!= 0) {
            msg.mnum = 0x90 | chnlnum(tp);
            type=1;
            trkfill(tp,&len,1,&msg,3,type);
        }
        tp->ticks += notelen;
        len = writevarlen(buf,notelen);
        msg.mnum = 0x80 | chnlnum(tp);
        type=2;
        trkfill(tp,buf,len,&msg,3,type);
    }
}

void
parsestacnot(char *arg,track *tp,int notelen)

{
    char *cp;
    int note;
    unsigned char buf[4] = {'\0','\0','\0','\0'};
    notemsg msg;
    int len;
    int type;
    int staclen = notelen - notelen / 4;

    for (cp = arg;*cp;cp++) {
        len = 0;
        cp = getnot(cp,&note);
        msg.note = note;
        msg.vel = tp->vel;
        if (note!= 0) {
            msg.mnum = 0x90 | chnlnum(tp);
            type=1;
            trkfill(tp,&len,1,&msg,3,type);
        }
        tp->ticks += staclen;
        len = writevarlen(buf,staclen);
        msg.mnum = 0x80 | chnlnum(tp);
        type=2;
        trkfill(tp,buf,len,&msg,3,type);

        tp->ticks += (notelen - staclen);
        len = writevarlen(buf,(notelen - staclen));
        msg.mnum = 0x80 | chnlnum(tp);
        type=2;
        trkfill(tp,buf,len,&msg,3,type);

    }
}

void
parsetype(char *arg,track *tp,const char *ptype)

{
    char *cp;
    int note;
    notemsg msg;
    int len;
    unsigned char buf[4] = {'\0','\0','\0','\0'};
    int type;

    cp = arg;
    if (!strcmp(ptype,"STN") || !strcmp(ptype,"STP"))
        cp = getnot(cp,&note);
    if (!strcmp(ptype,"STNN") || !strcmp(ptype,"STPN"))
        note= atoi(cp);
    msg.note = note;
    msg.vel = tp->vel;
    len = writevarlen(buf,tp->notelen);
    if (!strcmp(ptype,"STN") || !strcmp(ptype,"STNN"))
        msg.mnum = 0x90 | chnlnum(tp);
    if (!strcmp(ptype,"STP") || !strcmp(ptype,"STPN"))
        msg.mnum = 0x80 | chnlnum(tp);;
    type = 2;
    trkfill(tp,buf,len,&msg,3,type);
    if (!strcmp(ptype,"STP") || !strcmp(ptype,"STPN"))
        tp->ticks += tp->notelen;
}

int
parselen(char *arg,track *tp)

{
    int len;
    int totlen = 0;
    char *cp;
    char *cp2;

    for (cp=arg;cp != NULL;) {
        if ((cp2 = index(cp,'+')) == NULL)
            len = atoi(cp);
        else {
            *cp2++ = '\0';
            len = atoi(cp);
            cp = cp2;
        }
        if (tp->nu)
            totlen += len;
        else {
            switch (len) {
                case 11: totlen += 8 * ppqn;
                    break;
                case 1: totlen += 4 * ppqn;
                    break;
                case 2: totlen += 2 * ppqn;
                    break;
                case 4: totlen += ppqn;
                    break;
                case 43: totlen += (2*ppqn)/3;
                    break;
                case 83: totlen += ppqn/3;
                    break;
                case 8: totlen += ppqn/2;
                    break;
                case 16: totlen += ppqn / 4;
                    break;
                case 163: totlen += ppqn / 6;
                    break;
                case 32: totlen += ppqn / 8;
                    break;
                case 323: totlen += ppqn / 12;
                    break;
                case 64: totlen += ppqn / 16;
                    break;
                case 643: totlen += ppqn / 24;
                    break;
                default:
                    break;
            }
        }

        if (cp2 == NULL)
            break;
    }
    return(totlen);
}

void
parsestuff(char *arg,track *tp,void (*parse)(char *,track *,int))

{
    int notelen;
    char *cp;
    char *cp2 = arg;
    int trk = tp->trck;

    cp = cp2;
    while (*cp != '\0') {
        cp2 = index(cp,',');
        if (cp2 == NULL) {
            char tmpstr[100];
            sprintf(tmpstr,"index: %d comma expected",trk);
            error(tmpstr);
        }
        *cp2 = '\0';
        cp2++;

        notelen = parselen(cp,tp);
        cp = cp2;
        cp2 = index(cp,';');
        if (cp2 == NULL) {
            char tmpstr[100];
            sprintf(tmpstr,"index: %d semi-colon expected",trk);
            error(tmpstr);
        }
        *cp2 = '\0';
        cp2++;
        (*parse)(cp,tp,notelen);
        cp = cp2;
    }
}

void
parseinst(char *arg,track *tp)

{
    int i;
    chgprog chp;
    int len=0;
    unsigned char buf[4] = {'\0','\0','\0','\0'};
    int type;
    extern const char *inst[];

    for (i = 0;i < 128;i++)
        if (!strcmp(inst[i],arg))
            break;

    if (i == 128) {
        fprintf(stderr,"%s not found\n",arg);
        i = 0;
    }

    len = writevarlen(buf,tp->notelen);
    chp.mnum = 0xc0 | chnlnum(tp);
    chp.instr = i;
    type=2;
    trkfill(tp,buf,len,&chp,2,type);

}

void
endtrk(mtrk_hdr &mh,eot end,track *&tp)

{
    int dp=0;
    int type=1;
    extern track trk;

    for (tp = &trk;tp->np;tp = tp->np)
        ;
    trkfill(tp,&dp,1,&end,3,type);
    mh.length = htonl(tp->datalen);
    memcpy(tp->datap + 4,&(mh.length),4);
}

void
newendtrk(mtrk_hdr &mh,eot end,track *&tp)

{
    int dp=0;
    int type=1;
    extern track trk;

    for (tp = &trk;tp->np;tp = tp->np) {
        trkfill(tp,&dp,1,&end,3,type);
        mh.length = htonl(tp->datalen);
        memcpy(tp->datap + 4,&(mh.length),4);
    }
    trkfill(tp,&dp,1,&end,3,type);
    mh.length = htonl(tp->datalen);
    memcpy(tp->datap + 4,&(mh.length),4);
}

void
initMTrk(mtrk_hdr &mh,mthd_hdr &mc,track *&tp)

{
    if ((tp->datap = (char *)malloc(HDRSIZ)) == NULL)
        fatal("malloc");
    tp->datalen = 0;
    tp->ticks = 0;
    mh.length = htonl(tp->datalen);
    memcpy(tp->datap,&mh,HDRSIZ);
    mc.numtrks++;
}

void
newtrack(track *&tp)

{
    if ((tp->np = (track *)malloc(sizeof(track))) == NULL) 
        fatal("malloc");
    tp = tp->np;
    tp->datap = NULL;
    tp->datalen = 0;
    tp->vel = 60;
    tp->nu = 1;
    tp->chnl = -1;
    tp->np = NULL;
}

int
parsevel(char *arg,track *tp)

{
    char *cp;
    int vel;

    if (tp->nu)
        vel = atoi(arg);
    else {
        if (arg == NULL)
            return(0);
        if (*arg == 0)
            return(0);
        if ((cp = index(arg,'+')) != NULL)
            *cp++ = '\0';
        if (!strcmp(arg,"ppp"))
            vel = 18;
        if (!strcmp(arg,"pp"))
            vel = 30;
        if (!strcmp(arg,"p"))
            vel = 42;
        if (!strcmp(arg,"mp"))
            vel = 54;
        if (!strcmp(arg,"mf"))
            vel = 66;
        if (!strcmp(arg,"f"))
            vel = 78;
        if (!strcmp(arg,"ff"))
            vel = 90;
        if (!strcmp(arg,"fff"))
            vel = 102;
        if (!strcmp(arg,"ffff"))
            vel = 114;
        if (!strcmp(arg,"FF"))
            vel = 127;
        if (cp != NULL)
            vel += atoi(cp);
    }

    return(vel);
}

void
print(track *tp)

{
    printf("Track %d ticks: %d\n",tp->trck,tp->ticks);
}

void
dofuncs(int trck,char *arg1,char *arg2)

{
    keysig ks;
    tsig ts;
    tmpo tmp;
    int skip = 0;
    extern track trk;
    extern mthd_hdr mc;
    extern mtrk_hdr mh;
    extern eot end;
    track *tp;

    for (tp = &trk;tp->np;tp = tp->np)
        if (tp->trck == trck)
            break;
    if (!strcmp(arg1,"Trk")) {
        if (mc.numtrks > 0) {
            endtrk(mh,end,tp);
            newtrack(tp);
        }
        tp->trck = atoi(arg2);
        initMTrk(mh,mc,tp);
    }

    if (!strcmp(arg1,"Key"))
        parsekey(arg2,ks,tp);

    if (!strcmp(arg1,"Tim"))
        parsetim(arg2,ts,tp);

    if (!strcmp(arg1,"Tmpo"))
        parsetmp(arg2,tmp,tp->notelen,tp);

    if (!strcmp(arg1,"V"))
        tp->vel = parsevel(arg2,tp);

    if (!strcmp(arg1,"R"))
        tp->notelen = parselen(arg2,tp);

    if (!strcmp(arg1,"STN"))
        parsetype(arg2,tp,"STN");

    if (!strcmp(arg1,"STNN"))
        parsetype(arg2,tp,"STNN");

    if (!strcmp(arg1,"STP"))
        parsetype(arg2,tp,"STP");

    if (!strcmp(arg1,"STPN"))
        parsetype(arg2,tp,"STPN");

    if (!strcmp(arg1,"RN"))
        parsestuff(arg2,tp,parsenot);

    if (!strcmp(arg1,"RS"))
        parsestuff(arg2,tp,parsestacnot);

    if (!strcmp(arg1,"RCN"))
        parsestuff(arg2,tp,parsechord);

    if (!strcmp(arg1,"I"))
        parseinst(arg2,tp);

    if (!strcmp(arg1,"Tcks"))
        print(tp);

    if (!strcmp(arg1,"Sus"))
        sustain(arg2,tp);

    if (!strcmp(arg1,"NEW"))
        tp->nu = atoi(arg2);

    if (!strcmp(arg1,"PPQN")) {
        ppqn = atoi(arg2);
        mc.division = htons(ppqn);
    }
    if (!strcmp(arg1,"CHNL"))
        tp->chnl = atoi(arg2);
}

void
checkargs(FILE *&fp,int &fd,int argc,char *argv[])

{
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
        cp = (rindex(name,'.')+1);
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

int preparselin(char *&buf)
{
    char *cp = buf;
    char *cp2;
    int mnum;
    int tnum = 0;
    extern track trk;
    extern mthd_hdr mc;
    extern mtrk_hdr mh;
    extern eot end;
    extern int trkexists[];
    track *tp;

    if (*cp != 'M')
        return(-1);
    if ((cp2 = index(++cp,':')) == NULL)
        return(-1);
    *cp2++ = '\0';
    mnum = atoi(cp);
    cp = cp2;
    if (*cp != 'T')
        return(-1);
    if ((cp2 = index(++cp,':')) == NULL)
        return(-1);
    *cp2++ = '\0';
    tnum = atoi(cp);
    if (!trkexists[tnum]) {
        trkexists[tnum] = 1;
        for (tp = &trk;tp->np;tp = tp->np)
            ;
        if (tnum)
            newtrack(tp);
        tp->trck = tnum;
        initMTrk(mh,mc,tp);
    }
    buf = cp2;
    return(tnum);
}
