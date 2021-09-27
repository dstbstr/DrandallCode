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
    auto [name, value] = Extract("#define Foo");
    ASSERT_EQ(name, "Foo");
    ASSERT_EQ(value, "");
}

TEST(DefineExtractorExtract, ExtractsValueLikeMacroWithValue) {
    auto [name, value] = Extract("#define Foo 4");
    ASSERT_EQ(name, "Foo");
    ASSERT_EQ(value, "4");
}

TEST(DefineExtractorExtract, ExtractsFunctionLikeMacro) {
    auto [name, value] = Extract("#define Foo()");
    ASSERT_EQ(name, "Foo()");
    ASSERT_EQ(value, "");
}

TEST(DefineExtractorExtract, ExtractsFunctionLikeMacroWithDefinition) {
    auto [name, value] = Extract("#define Foo() Bar");
    ASSERT_EQ(name, "Foo()");
    ASSERT_EQ(value, "Bar");
}

TEST(DefineExtractorExtract, ExtractsFunctionLikeMacroWithArguments) {
    auto [name, value] = Extract("#define Foo(a, b) Bar(b, a)");
    ASSERT_EQ(name, "Foo(a, b)");
    ASSERT_EQ(value, "Bar(b, a)");
}