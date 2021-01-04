#include "Extractor/Private/DefineExtractor.h"

#include "TestCommon.h"


using Extractor::DefineExtractor::CanExtract;
using Extractor::DefineExtractor::Extract;

TEST(DefineExtractorCanExtract, CanExtractValueLikeMacro) {
    ASSERT_TRUE(CanExtract("#define Foo"));
}

TEST(DefineExtractorCanExtract, CanExtractValueLikeMacroWithValue) {
    ASSERT_TRUE(CanExtract("#define Foo 3"));
}

TEST(DefineExtractorCanExtract, CanExtractValueLikeMacroWithFunctionLikeValue) {
    ASSERT_TRUE(CanExtract("#define Foo Bar()"));
}

TEST(DefineExtractorCanExtract, CanExtractDefineFunctionLikeMacro) {
    ASSERT_TRUE(CanExtract("#define Foo()"));
}

TEST(DefineExtractorCanExtract, CanExtractFunctionLikeMacroWithDefinition) {
    ASSERT_TRUE(CanExtract("#define Foo() Bar()"));
}

TEST(DefineExtractorCanExtract, CanNotExtractIfHashIsMissing) {
    ASSERT_FALSE(CanExtract("define Foo"));
}

TEST(DefineExtractorCanExtract, CanNotExtractCommentedDefine) {
    ASSERT_FALSE(CanExtract("//#define Foo"));
}

TEST(DefineExtractorExtract, ExtractsValueLikeMacro) {
    auto result = Extract("#define Foo");
    ASSERT_EQ(result.first, "Foo");
    ASSERT_EQ(result.second, "");
}

TEST(DefineExtractorExtract, ExtractsValueLikeMacroWithValue) {
    auto result = Extract("#define Foo 4");
    ASSERT_EQ(result.first, "Foo");
    ASSERT_EQ(result.second, "4");
}

TEST(DefineExtractorExtract, ExtractsFunctionLikeMacro) {
    auto result = Extract("#define Foo()");
    ASSERT_EQ(result.first, "Foo()");
    ASSERT_EQ(result.second, "");
}

TEST(DefineExtractorExtract, ExtractsFunctionLikeMacroWithDefinition) {
    auto result = Extract("#define Foo() Bar");
    ASSERT_EQ(result.first, "Foo()");
    ASSERT_EQ(result.second, "Bar");
}

TEST(DefineExtractorExtract, ExtractsFunctionLikeMacroWithArguments) {
    auto result = Extract("#define Foo(a, b) Bar(b, a)");
    ASSERT_EQ(result.first, "Foo(a, b)");
    ASSERT_EQ(result.second, "Bar(b, a)");
}