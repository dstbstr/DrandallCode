#ifndef __LIBCONVERTGDBMIDICLASS_H__
#define __LIBCONVERTGDBMIDICLASS_H__

#include "libmidiclass.h"

void addtrkdata(track *&tp,void *var1,int sz1,void *var2,int sz2,int type);
void setkey(char *arg,keysig &keystr,track *tp);
void settimesig(char *arg,tsig &ts,track *tp);
void settempo(char *arg,tmpo &tmp,int skip,track *tp);
int chnlnum(track *tp);
void sustain(char *arg,track *tp);
char* getnote(char *cp,int *note);
void setchords(char *arg,track *tp,int notelen);
void setnotes(char *arg,track *tp,int notelen);
void setstacnotes(char *arg,track *tp,int notelen);
void strtstpnote(char *arg,track *tp,const char *ptype);
int setnotelen(char *arg,track *tp);
void setstuff(char *arg,track *tp,void (*set)(char *,track *,int));
void setinstr(char *arg,track *tp);
void endtrk(mtrk_hdr &mh,eot end,track *&tp);
void newendtrk(mtrk_hdr &mh,eot end,track *&tp);
void initMTrk(mtrk_hdr &mh,mthd_hdr &mc,track *&tp);
void newtrack(track *&tp);
int setvel(char *arg,track *tp);
void print(track *tp);
void dofuncs(int trck,char *arg1,char *arg2);
#endif // __LIBCONVERTGDBMIDICLASS_H__