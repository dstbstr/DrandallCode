#include "Extractor/Private/IfDefExtractor.h"

#include "Extractor/Private/LineFetcher.h"
#include "TestCommon.h"

#include <sstream>
#include <string>
#include <vector>

namespace Extractor {
    class IfDefExtractorTest : public ::testing::Test {
    protected:
        void SetUp() override {
            extractor = std::make_unique<IfDefExtractor>(m_KnownDefines, &ss);
        }

        bool CanExtract() {
            std::string line;
            LineFetcher::GetNextLine(ss, line);
            return extractor->CanExtract(line);
        }

        std::string Extract() {
            std::string line;
            LineFetcher::GetNextLine(ss, line);
            if(extractor->CanExtract(line)) {
                extractor->Extract(line); // should always return a blank line
                LineFetcher::GetNextLine(ss, line);
            }
            return line;
        }

        std::stringstream ss;
        std::vector<std::string> m_KnownDefines{"IS_DEFINED"};
        std::unique_ptr<IfDefExtractor> extractor{nullptr};
    };

    TEST_F(IfDefExtractorTest, IfDefCanBeExtracted) {
        ss << "#ifdef 0";
        ASSERT_TRUE(CanExtract());
    }

    TEST_F(IfDefExtractorTest, IfCanBeExtracted) {
        ss << "#if defined(0)";
        ASSERT_TRUE(CanExtract());
    }

    TEST_F(IfDefExtractorTest, IfndefCanBeExtracted) {
        ss << "#ifndef 0";
        ASSERT_TRUE(CanExtract());
    }

    TEST_F(IfDefExtractorTest, ElseCanBeExtracted) {
        ss << "#else";
        ASSERT_TRUE(CanExtract());
    }

    TEST_F(IfDefExtractorTest, ElseWithoutHashIsNotExtracted) {
        ss << "else";
        ASSERT_FALSE(CanExtract());
    }

    TEST_F(IfDefExtractorTest, ElifCanBeExtracted) {
        ss << "#elif 0";
        ASSERT_TRUE(CanExtract());
    }

    TEST_F(IfDefExtractorTest, EndifCanBeExtracted) {
        ss << "#endif";
        ASSERT_TRUE(CanExtract());
    }

    TEST_F(IfDefExtractorTest, AllowsNextLineToBeProcessedIfDefineIsKnown) {
        ss << R"(
            #ifdef IS_DEFINED
            int x;
            #endif)";

        ASSERT_EQ(Extract(), "int x;");
    }

    TEST_F(IfDefExtractorTest, SkipsBodyIfDefineIsUnknown) {
        ss << R"(
            #ifdef 0
            Line 1;
            #endif
            Line 2;)";

        ASSERT_EQ(Extract(), "Line 2;");
    }

    TEST_F(IfDefExtractorTest, CanTakeElseBlock) {
        ss << R"(
            #ifdef 0
                Line 1;
            #else
                Line 2;
            #endif
        )";

        ASSERT_EQ(Extract(), "Line 2;");
    }

    TEST_F(IfDefExtractorTest, SupportsElif) {
        ss << R"(
            #ifdef 0
                Line 1;
            #elif defined(IS_DEFINED)
                Line 2;
            #endif
        )";

        ASSERT_EQ(Extract(), "Line 2;");
    }

    TEST_F(IfDefExtractorTest, SupportsIfDefined) {
        ss << R"(
            #if defined(IS_DEFINED)
                Line 1;
            #endif
        )";

        ASSERT_EQ(Extract(), "Line 1;");
    }

    TEST_F(IfDefExtractorTest, SupportsIfndef) {
        ss << R"(
            #ifndef 0
                Line 1;
            #endif
        )";

        ASSERT_EQ(Extract(), "Line 1;");
    }

    TEST_F(IfDefExtractorTest, SupportsNotDefined) {
        ss << R"(
            #if !defined(0)
                Line 1;
            #endif
        )";

        ASSERT_EQ(Extract(), "Line 1;");
    }

    TEST_F(IfDefExtractorTest, SupportsAndStatements) {
        ss << R"(
            #if !defined(A) && defined(IS_DEFINED)
                Line 1;
            #else
                Line 2;
            #endif
        )";

        ASSERT_EQ(Extract(), "Line 1;");
    }

    TEST_F(IfDefExtractorTest, SupportsOrStatements) {
        ss << R"(
            #if defined(A) || defined(IS_DEFINED)
                Line 1;
            #else
                Line 2;
            #endif
        )";

        ASSERT_EQ(Extract(), "Line 1;");
    }

    TEST_F(IfDefExtractorTest, WillTakeFirstMatch) {
        ss << R"(
            #if defined(IS_DEFINED)
                Line 1;
            #elif !defined(0)
                Line 2;
            #else
                Line 3;
            #endif
            Line 4;
        )";

        ASSERT_EQ(Extract(), "Line 1;");
        ASSERT_EQ(Extract(), "Line 4;");
    }

    TEST_F(IfDefExtractorTest, SupportsNestedIfdefs) {
        ss << R"(
            #ifdef(IS_DEFINED)
                Line 0;
                #ifdef(0)
                    Line 1;
                #else
                    Line 2;
                #endif
                Line 3;
            #endif
        )";

        ASSERT_EQ(Extract(), "Line 0;");
        ASSERT_EQ(Extract(), "Line 2;");
        ASSERT_EQ(Extract(), "Line 3;");
    }

    TEST_F(IfDefExtractorTest, SupportsDeeplyNestedIfDefs) {
        ss << R"(
            #ifdef(IS_DEFINED)
                Line 0;
                #ifdef(IS_DEFINED)
                    Line 1;
                    #ifdef(0)
                        Line 2;
                    #elif defined(1)
                        Line 3;
                    #else
                        Line 4;
                    #endif
                Line 5;
                #endif
            Line 6;
            #else
                Line 7;
            #endif
        )";

        ASSERT_EQ(Extract(), "Line 0;");
        ASSERT_EQ(Extract(), "Line 1;");
        ASSERT_EQ(Extract(), "Line 4;");
        ASSERT_EQ(Extract(), "Line 5;");
        ASSERT_EQ(Extract(), "Line 6;");
        ASSERT_EQ(Extract(), "");
    }

    TEST_F(IfDefExtractorTest, SupportsNestingInElseBlock) {
        ss << R"(
            #ifdef 0
                Line 1;
            #else
                Line 2;
                #ifdef(IS_DEFINED)
                    Line 3;
                #elif !defined(0)
                    Line 4;
                #endif
            Line 5;
            #endif
        )";

        ASSERT_EQ(Extract(), "Line 2;");
        ASSERT_EQ(Extract(), "Line 3;");
        ASSERT_EQ(Extract(), "Line 5;");
    }

} // namespace Extractor