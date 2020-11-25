#include "Extractor/Private/LineFetcher.h"

#include "TestCommon.h"

namespace Extractor {
    class LineFetcherTest : public ::testing::Test {
    protected:
        std::string Run() {
            LineFetcher::GetNextLine(ss, m_Line);
            return m_Line;
        }

        std::string m_Line;
        std::stringstream ss;
    };

    TEST_F(LineFetcherTest, SemicolonIsAStatement) {
        ss << R"(int i = 0;
        int j = 1;)";
        auto line = Run();
        ASSERT_EQ(line, "int i = 0;");
    }

    TEST_F(LineFetcherTest, PreprocessorDirectiveIsAStatement) {
        ss << R"(#pragma once
        #include "TestCommon.h")";
        auto line = Run();
        ASSERT_EQ(line, "#pragma once");
    }

    TEST_F(LineFetcherTest, OpenCurlyIsAStatement) {
        ss << R"(class Foo {
            int i = 0;
        };)";
        auto line = Run();
        ASSERT_EQ(line, "class Foo {");
    }

    TEST_F(LineFetcherTest, CloseCurlyIsAStatement) {
        ss << R"(class Foo{}
        ;)";
        auto line = Run();
        ASSERT_EQ(line, "class Foo{}");
    }

    TEST_F(LineFetcherTest, LinesAreJoinedWithSpace) {
        ss << R"(int i
        = 0;)";
        auto line = Run();
        ASSERT_EQ(line, "int i = 0;");
    }

    TEST_F(LineFetcherTest, AngleBracketsAreNotAStatement) {
        ss << R"(template<class T>
        T Foo();)";
        auto line = Run();
        ASSERT_EQ(line, "template<class T> T Foo();");
    }

    TEST_F(LineFetcherTest, CommasDoNotEndAStatement) {
        ss << R"(int Foo(int a,
        int b,
        int c);)";
        auto line = Run();
        ASSERT_EQ(line, "int Foo(int a, int b, int c);");
    }

    TEST_F(LineFetcherTest, ParensDoNotEndStatements) {
        ss << R"(int Foo(
            int a)
            ;
        )";
        auto line = Run();
        ASSERT_EQ(line, "int Foo( int a) ;");
    }

    TEST_F(LineFetcherTest, CommentsAreRemoved) {
        ss << R"(int Foo// {
            ();
        )";
        auto line = Run();
        ASSERT_EQ(line, "int Foo ();");
    }

    TEST_F(LineFetcherTest, RemovesBlockComments) {
        ss << R"(int Foo/*
        {};;;
        */
       ;)";
        auto line = Run();
        ASSERT_EQ(line, "int Foo;");
    }

    TEST_F(LineFetcherTest, ClearsResultStringBetweenLines) {
        ss << R"(int a;
        int b;)";
        auto line = Run();
        ASSERT_EQ(line, "int a;");
        line = Run();
        ASSERT_EQ(line, "int b;");
    }

    TEST_F(LineFetcherTest, CondensesGroupsOfSpaces) {
        ss << R"(Foo(int i, int j, int k)
        : m_I(i)
        , m_J(j)
        , m_K(k) {
        })";

        auto line = Run();
        ASSERT_EQ(line, "Foo(int i, int j, int k) : m_I(i) , m_J(j) , m_K(k) {");
    }
} // namespace Extractor