#include "Singers/IBirthdaySinger.h"
#include "Singers/NaiveSinger.h"
#include "Singers/TernarySinger.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::MatchesRegex;

class HappyBirthdayTests : public ::testing::Test {
    public:
    void SetUp() override {
        singers.push_back(new NaiveSinger());
        singers.push_back(new TernarySinger());
    }

    void TearDown() override {
        for(auto singer : singers) {
            delete singer;
        }
    }

    protected:
    std::vector<IBirthdaySinger*> singers;
};

TEST_F(HappyBirthdayTests, Sings) {
    for(auto&& singer : singers) {
        ASSERT_NE("", singer->Sing("Test"));
    }
}

TEST_F(HappyBirthdayTests, IncludesTargetName) {
    for(auto&& singer : singers) {
        ASSERT_EQ("Happy birthday to you.  Happy birthday to you.  Happy birthday dear Test.  Happy birthday to you.", singer->Sing("Test"));
    }
}