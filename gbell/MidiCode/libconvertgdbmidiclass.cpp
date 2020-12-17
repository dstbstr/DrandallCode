#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include "midiclass.h"
#include "libmidiclass.h"
#include "libdefs.h"

void
addtrkdata(track *&tp,void *var1,int sz1,void *var2,int sz2,int type)

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
setkey(char *arg,keysig &keystr,track *tp)

{
    int dp = 0;
    int type = 1;

    if (!strcmp(arg,"Cb"))
        keystr.fltshrp = -7;
    if (!strcmp(arg,"Fb"))
        keystr.fltshrp = -6;
    if (!strcmp(arg,"Db"))
        keystr.fltshrp = -5;
    if (!strcmp(arg,"Ab")) 
        keystr.fltshrp = -4;
    if (!strcmp(arg,"Eb"))
        keystr.fltshrp = -3;
    if (!strcmp(arg,"Bb"))
        keystr.fltshrp = -2;
    if (!strcmp(arg,"F"))
        keystr.fltshrp = -1;
    if (!strcmp(arg,"C"))
        keystr.fltshrp = 0;
    if (!strcmp(arg,"G"))
        keystr.fltshrp = 1;
    if (!strcmp(arg,"D"))
        keystr.fltshrp = 2;
    if (!strcmp(arg,"A"))
        keystr.fltshrp = 3;
    if (!strcmp(arg,"E"))
        keystr.fltshrp = 4;
    if (!strcmp(arg,"B"))
        keystr.fltshrp = 5;
    if (!strcmp(arg,"F#"))
        keystr.fltshrp = 6;
    if (!strcmp(arg,"C#"))
        keystr.fltshrp = 7;
    if (!strcmp(arg,"ab"))
        keystr.fltshrp = -7;
    if (!strcmp(arg,"db"))
        keystr.fltshrp = -6;
    if (!strcmp(arg,"bb"))
        keystr.fltshrp = -5;
    if (!strcmp(arg,"f"))
        keystr.fltshrp = -4;
    if (!strcmp(arg,"c"))
        keystr.fltshrp = -3;
    if (!strcmp(arg,"g"))
        keystr.fltshrp = -2;
    if (!strcmp(arg,"d"))
        keystr.fltshrp = -1;
    if (!strcmp(arg,"a"))
        keystr.fltshrp = 0;
    if (!strcmp(arg,"e"))
        keystr.fltshrp = 1;
    if (!strcmp(arg,"b"))
        keystr.fltshrp = 2;
    if (!strcmp(arg,"f#"))
        keystr.fltshrp = 3;
    if (!strcmp(arg,"c#"))
        keystr.fltshrp = 4;
    if (!strcmp(arg,"g#"))
        keystr.fltshrp = 5;
    if (!strcmp(arg,"d#"))
        keystr.fltshrp = 6;
    if (!strcmp(arg,"a#"))
        keystr.fltshrp = 7;

    if (isupper(arg[0]))
        keystr.majmin = 0;
    else
        keystr.majmin = 1;

    keystr.fltshrp = htons(keystr.fltshrp)/256;
    keystr.majmin = htons(keystr.majmin)/256;

    addtrkdata(tp,&dp,1,&keystr,5,type);
}

void
settimesig(char *arg,tsig &ts,track *tp)

{
    char *cp = strchr(arg,'/');
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
    addtrkdata(tp,timbuf,len,&ts,7,type);
}

void
settempo(char *arg,tmpo &tmp,int skip,track *tp)

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
        addtrkdata(tp,&dp,1,&tmp,6,type);
    }
    else {
        len = writevarlen(tmpobuf,skip);
        addtrkdata(tp,tmpobuf,len,&tmp,6,type);
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
    addtrkdata(tp,susbuf,len,&sus,3,type);
    tp->ticks += tp->notelen;
}

char *
getnote(char *cp,int *note)

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
setchords(char *arg,track *tp,int notelen)

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
        cp = getnote(cp,&note);
        msg.note = note;
        msg.vel = tp->vel;
        if (note!= 0) {
            msg.mnum = 0x90 | chnlnum(tp);
            type=1;
            addtrkdata(tp,&len,1,&msg,3,type);
        }
    }
    tp->ticks += notelen;
    len = writevarlen(buf,notelen);
    cp = arg;
    cp = getnote(cp,&note);
    msg.note = note;
    msg.mnum = 0x80 | chnlnum(tp);
    type=2;
    addtrkdata(tp,buf,len,&msg,3,type);
    cp++;
    for (;*cp;cp++) {
        len = 0;
        cp = getnote(cp,&note);
        msg.note = note;
        msg.vel = tp->vel;
        if (note != 0) {
            msg.mnum = 0x80 | chnlnum(tp);
            type=1;
            addtrkdata(tp,&len,1,&msg,3,type);
        }
    }
}

void
setnotes(char *arg,track *tp,int notelen)

{
    char *cp;
    int note;
    unsigned char buf[4] = {'\0','\0','\0','\0'};
    notemsg msg;
    int len;
    int type;

    for (cp = arg;*cp;cp++) {
        len = 0;
        cp = getnote(cp,&note);
        msg.note = note;
        msg.vel = tp->vel;
        if (note!= 0) {
            msg.mnum = 0x90 | chnlnum(tp);
            type=1;
            addtrkdata(tp,&len,1,&msg,3,type);
        }
        tp->ticks += notelen;
        len = writevarlen(buf,notelen);
        msg.mnum = 0x80 | chnlnum(tp);
        type=2;
        addtrkdata(tp,buf,len,&msg,3,type);
    }
}

void
setstacnotes(char *arg,track *tp,int notelen)

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
        cp = getnote(cp,&note);
        msg.note = note;
        msg.vel = tp->vel;
        if (note!= 0) {
            msg.mnum = 0x90 | chnlnum(tp);
            type=1;
            addtrkdata(tp,&len,1,&msg,3,type);
        }
        tp->ticks += staclen;
        len = writevarlen(buf,staclen);
        msg.mnum = 0x80 | chnlnum(tp);
        type=2;
        addtrkdata(tp,buf,len,&msg,3,type);

        tp->ticks += (notelen - staclen);
        len = writevarlen(buf,(notelen - staclen));
        msg.mnum = 0x80 | chnlnum(tp);
        type=2;
        addtrkdata(tp,buf,len,&msg,3,type);

    }
}

void
strtstpnote(char *arg,track *tp,const char *ptype)

{
    char *cp;
    int note;
    notemsg msg;
    int len;
    unsigned char buf[4] = {'\0','\0','\0','\0'};
    int type;

    cp = arg;
    if (!strcmp(ptype,"STN") || !strcmp(ptype,"STP"))
        cp = getnote(cp,&note);
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
    addtrkdata(tp,buf,len,&msg,3,type);
    if (!strcmp(ptype,"STP") || !strcmp(ptype,"STPN"))
        tp->ticks += tp->notelen;
}

int
setnotelen(char *arg,track *tp)

{
    int len;
    int totlen = 0;
    char *cp;
    char *cp2;

    for (cp=arg;cp != NULL;) {
        if ((cp2 = strchr(cp,'+')) == NULL)
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
setstuff(char *arg,track *tp,void (*set)(char *,track *,int)) //great descriptive name here. :)

{
    int notelen;
    char *cp;
    char *cp2 = arg;
    int trk = tp->trck;

    cp = cp2;
    while (*cp != '\0') {
        cp2 = strchr(cp,',');
        if (cp2 == NULL) {
            char tmpstr[100];
            sprintf(tmpstr,"strchr: %d comma expected",trk);
            error(tmpstr);
        }
        *cp2 = '\0';
        cp2++;

        notelen = setnotelen(cp,tp);
        cp = cp2;
        cp2 = strchr(cp,';');
        if (cp2 == NULL) {
            char tmpstr[100];
            sprintf(tmpstr,"strchr: %d semi-colon expected",trk);
            error(tmpstr);
        }
        *cp2 = '\0';
        cp2++;
        (*set)(cp,tp,notelen);
        cp = cp2;
    }
}

void
setinstr(char *arg,track *tp)

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
    addtrkdata(tp,buf,len,&chp,2,type);

}

void
endtrk(mtrk_hdr &mh,eot end,track *&tp)

{
    int dp=0;
    int type=1;
    extern track trk;

    for (tp = &trk;tp->np;tp = tp->np)
        ;
    addtrkdata(tp,&dp,1,&end,3,type);
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
        addtrkdata(tp,&dp,1,&end,3,type);
        mh.length = htonl(tp->datalen);
        memcpy(tp->datap + 4,&(mh.length),4);
    }
    addtrkdata(tp,&dp,1,&end,3,type);
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
setvel(char *arg,track *tp)

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
        if ((cp = strchr(arg,'+')) != NULL)
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
        setkey(arg2,ks,tp);

    if (!strcmp(arg1,"Tim"))
        settimesig(arg2,ts,tp);

    if (!strcmp(arg1,"Tmpo"))
        settempo(arg2,tmp,tp->notelen,tp);

    if (!strcmp(arg1,"V"))
        tp->vel = setvel(arg2,tp);

    if (!strcmp(arg1,"R"))
        tp->notelen = setnotelen(arg2,tp);

    if (!strcmp(arg1,"STN"))
        strtstpnote(arg2,tp,"STN");

    if (!strcmp(arg1,"STNN"))
        strtstpnote(arg2,tp,"STNN");

    if (!strcmp(arg1,"STP"))
        strtstpnote(arg2,tp,"STP");

    if (!strcmp(arg1,"STPN"))
        strtstpnote(arg2,tp,"STPN");

    if (!strcmp(arg1,"RN"))
        setstuff(arg2,tp,setnotes);

    if (!strcmp(arg1,"RS"))
        setstuff(arg2,tp,setstacnotes);

    if (!strcmp(arg1,"RCN"))
        setstuff(arg2,tp,setchords);

    if (!strcmp(arg1,"I"))
        setinstr(arg2,tp);

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
