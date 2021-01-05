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

class ParseLineTest : public ::testing::Test {
public:
  void SetUp() override {
    leftArg = "";
    rightArg = "";
    line = strdup("Left:Right\n");
  }

  void TearDown() override { free(line); }

protected:
  char *leftArg;
  char *rightArg;
  char *line;
};

TEST_F(ParseLineTest, GivenNullReturnsNegativeOne) {
  ASSERT_EQ(-1, parselin("", Separator, leftArg, rightArg));
}

TEST_F(ParseLineTest, GivenValidStringReturnsZero) {
  ASSERT_EQ(0, parselin(line, Separator, leftArg, rightArg));
}

TEST_F(ParseLineTest, LeftOfSeparatorAssignedToArg1) {
  ASSERT_EQ(0, parselin(line, Separator, leftArg, rightArg));
  ASSERT_STREQ(leftArg, "Left");
}

TEST_F(ParseLineTest, RightOfSeparatorAssignedToArg2) {
  ASSERT_EQ(0, parselin(line, Separator, leftArg, rightArg));
  ASSERT_STREQ(rightArg, "Right");
}

/*
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