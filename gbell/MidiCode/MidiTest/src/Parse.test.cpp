#include "Parse.h"
#include "TestCommon.h"
#include "libmidiclass.h"

track trk;
mthd_hdr mc;
mtrk_hdr mh;
eot end;
int trkexists[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

constexpr char Separator = ':';

TEST(ParseLine, GivenNullReturnsNegativeOne) {
  char *arg = "";

  ASSERT_EQ(-1, parselin("", Separator, arg, arg));
}

TEST(ParseLine, GivenValidStringReturnsZero) {
  char *leftArg = "";
  char *rightArg = "";
  char *line = strdup("A:B\n");

  ASSERT_EQ(0, parselin(line, Separator, leftArg, rightArg));
  free(line);
}

/*
int
parselin(char *line,char parsechr,char *&arg1,char *&arg2)

{
    char *cp;
    char *iptr;

    cp=line;
    if ((iptr=strchr(cp,parsechr)) == NULL)
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
preparselin(char *&buf)
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
    if ((cp2 = strchr(++cp,':')) == NULL)
        return(-1);
    *cp2++ = '\0';
    mnum = atoi(cp);
    cp = cp2;
    if (*cp != 'T')
        return(-1);
    if ((cp2 = strchr(++cp,':')) == NULL)
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

*/