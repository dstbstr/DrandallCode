#include "CommandParser/CommandSplitter.h"

#include "Macros/PreProcessorOverride.h"
#include "TestCommon.h"
#include "Utilities/LambdaUtils.h"

#define ARGV_1(str, ...) \
    { (char*)str }
#define ARGV_2(str, ...) {(char*)str}, ARGV_1(__VA_ARGS__)
#define ARGV_3(str, ...) {(char*)str}, ARGV_2(__VA_ARGS__)
#define ARGV_4(str, ...) {(char*)str}, ARGV_3(__VA_ARGS__)
#define ARGV(...) \
    { {(char*)"ProgramName"}, MACRO_OVERRIDE(ARGV_, __VA_ARGS__) }

#define GET_PAIRS_1(arg)               \
    ArgType args = EXPAND(ARGV(arg));  \
    CommandSplitter splitter(2, args); \
    auto pairs = GetOptions(splitter);

#define GET_PAIRS_2(arg1, arg2)        \
    ArgType args = ARGV(arg1, arg2);   \
    CommandSplitter splitter(3, args); \
    auto pairs = GetOptions(splitter);

namespace CommandParser {
    using ArgType = char*[];

    std::vector<OptionValuePair> GetOptions(CommandSplitter& splitter) {
        std::vector<OptionValuePair> pairs;
        while(splitter.HasMoreArguments()) {
            pairs.push_back(splitter.GetNext());
        }

        return pairs;
    }

    TEST(CommandSplitter, NoArgsHasNoArguments) {
        ArgType args = {{(char*)"ProgramName"}};
        CommandSplitter splitter(1, args);

        ASSERT_FALSE(splitter.HasMoreArguments());
    }

    TEST(CommandSplitter, SingleOptionHasArgumentsToProcess) {
        ArgType args = ARGV("-a");

        CommandSplitter splitter(2, args);

        ASSERT_TRUE(splitter.HasMoreArguments());
    }

    TEST(CommandSplitter, SingleOptionArgumentStripsThePrefix) {
        ArgType args = ARGV("-a");
        CommandSplitter splitter(2, args);

        auto pair = splitter.GetNext();
        ASSERT_EQ(pair.Name, "a");
    }

    TEST(CommandSplitter, ShortOptionWithNoValueHasNoValue) {
        GET_PAIRS_1("-a");
        ASSERT_FALSE(pairs[0].HasValue);
    }

    TEST(CommandSplitter, SingleShortOptionCanTakeAValue) {
        GET_PAIRS_2("-a", "test");

        ASSERT_EQ(pairs[0].Name, "a");
        ASSERT_TRUE(pairs[0].HasValue);
        ASSERT_EQ(pairs[0].Value, "test");
    }

    TEST(CommandSplitter, MultipleShortOptionsAreSplitIntoFlags) {
        GET_PAIRS_1("-abc");

        ASSERT_EQ(3, pairs.size());
        ASSERT_FALSE(pairs[0].HasValue);
        ASSERT_EQ(pairs[0].Name, "a");
    }

    TEST(CommandSplitter, MultipleShortOptionsDoNotTakeAValue) {
        GET_PAIRS_2("-abc", "test");

        ASSERT_EQ(4, pairs.size());
        ASSERT_FALSE(pairs[0].HasValue);
        ASSERT_FALSE(pairs[2].HasValue);
    }

    TEST(CommandSplitter, MultipleShortOptionsIncludesValueAsFreeFloater) {
        GET_PAIRS_2("-abc", "test");

        ASSERT_EQ(4, pairs.size());
        ASSERT_EQ(pairs[3].Name, "");
        ASSERT_TRUE(pairs[3].HasValue);
        ASSERT_EQ(pairs[3].Value, "test");
    }

    TEST(CommandSplitter, SingleLongOptionStripsPrefix) {
        GET_PAIRS_1("--test");

        ASSERT_EQ(1, pairs.size());
        ASSERT_EQ(pairs[0].Name, "test");
    }

    TEST(CommandSplitter, SingleLongOptionHasNoValue) {
        GET_PAIRS_1("--test");

        ASSERT_EQ(1, pairs.size());
        ASSERT_FALSE(pairs[0].HasValue);
    }

    TEST(CommandSplitter, MultipleLongOptionsHaveNoValues) {
        GET_PAIRS_2("--test1", "--test2");

        ASSERT_EQ(2, pairs.size());
        ASSERT_FALSE(pairs[0].HasValue);
        ASSERT_FALSE(pairs[1].HasValue);
        ASSERT_EQ(pairs[0].Name, "test1");
        ASSERT_EQ(pairs[1].Name, "test2");
    }

    TEST(CommandSplitter, LongArgumentCanTakeAValue) {
        GET_PAIRS_2("--test", "val");

        ASSERT_EQ(1, pairs.size());
        ASSERT_TRUE(pairs[0].HasValue);
        ASSERT_EQ(pairs[0].Value, "val");
    }

    TEST(CommandSplitter, HandlesShortAndLongValues) {
        GET_PAIRS_2("-test", "--test");

        ASSERT_EQ(5, pairs.size());
        ASSERT_EQ(pairs[0].Name, "t");
        ASSERT_EQ(pairs[4].Name, "test");
    }

    TEST(CommandSplitter, SlashOptionsAreAlwaysLong) {
        GET_PAIRS_1("/test");

        ASSERT_EQ(1, pairs.size());
        ASSERT_EQ(pairs[0].Name, "test");
    }

    TEST(CommandSplitter, SupportsSingleCharacterSlashOptions) {
        GET_PAIRS_1("/t");

        ASSERT_EQ(1, pairs.size());
        ASSERT_EQ(pairs[0].Name, "t");
    }

    TEST(CommandSplitter, SlashOptionsSupportValues) {
        GET_PAIRS_2("/t", "val");

        ASSERT_EQ(1, pairs.size());
        ASSERT_EQ(pairs[0].Name, "t");
        ASSERT_TRUE(pairs[0].HasValue);
        ASSERT_EQ(pairs[0].Value, "val");
    }
} // namespace CommandParser