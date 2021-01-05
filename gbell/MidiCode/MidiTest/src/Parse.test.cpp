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
    line = new char[]{"Left:Right\n"};
  }

  void TearDown() override { delete line; }

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

// All of these tests leak because preparselin reassigns the provided pointer,
// making it impossible to free/delete
class PreParseLineTest : public ::testing::Test {
public:
  void TearDown() override {
    for (int i = 0; i < 20; i++) {
      trkexists[i] = 0;
    }
  }

protected:
  char *line;
};

TEST_F(PreParseLineTest, BlankStringReturnsNegativeOne) {
  line = new char[]{""};
  ASSERT_EQ(-1, preparselin(line));
}

TEST_F(PreParseLineTest, MissingMeasureReturnsNegativeOne) {
  line = new char[]{"ABC"};
  ASSERT_EQ(-1, preparselin(line));
}

TEST_F(PreParseLineTest, MissingColonReturnsNegativeOne) {
  line = new char[]{"MBA"};
  ASSERT_EQ(-1, preparselin(line));
}

TEST_F(PreParseLineTest, MissingTrackReturnsNegativeOne) {
  line = new char[]{"M3:A"};
  ASSERT_EQ(-1, preparselin(line));
}

TEST_F(PreParseLineTest, MissingColonAfterTrackReturnsNegativeOne) {
  line = new char[]{"M3:T4"};
  ASSERT_EQ(-1, preparselin(line));
}

TEST_F(PreParseLineTest, PreParseReturnsTrackNumber) {
  line = new char[]{"M3:T1:ABC"};
  ASSERT_EQ(1, preparselin(line));
}

TEST_F(PreParseLineTest, StripsMeasureAndTrackInfo) {
  line = new char[]{"M3:T1:ABC"};
  preparselin(line);
  ASSERT_STREQ(line, "ABC");
}

TEST_F(PreParseLineTest, MarksTrackAsConstructed) {
  line = new char[]{"M3:T1:ABC"};
  ASSERT_EQ(0, trkexists[1]);
  preparselin(line);
  ASSERT_EQ(1, trkexists[1]);
}