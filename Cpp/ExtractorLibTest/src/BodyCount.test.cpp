#include "Extractor/BodyCount.h"
#include "TestCommon.h"
#include "Extractor/Private/LineFetcher.h"

#include <sstream>

class BodyCountTest : public ::testing::Test {
public:
    u64 GetBodyCount() {
        std::string line;
        Extractor::LineFetcher::GetNextLine(ss, line);

        return Extractor::BodyCount::GetBodyCount(line, ss);
    }
protected:
    std::stringstream ss;
};

TEST_F(BodyCountTest, SingleLineReturnsZero) {
    ss << "class Foo {};";

    ASSERT_EQ(GetBodyCount(), 0);
}

TEST_F(BodyCountTest, ArrayInitializerReturnsOne) {
    ss << R"(int Foo[] = {
            1,
            2,
            3};
        )";

    ASSERT_EQ(GetBodyCount(), 1);
}