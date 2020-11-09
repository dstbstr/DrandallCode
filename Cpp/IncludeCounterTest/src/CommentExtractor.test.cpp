#include "IncludeCounter/Extractors/CommentExtractor.h"

#include "TestCommon.h"

#include <string>

namespace IncludeCounter {
    namespace Extractors {
        class CommentExtractorTest : public ::testing::Test {
        protected:
            void Run() {
                CommentExtractor::StripComments(m_Line, m_IsInBlockComment);
            }
            bool m_IsInBlockComment{false};
            std::string m_Line;
        };

        TEST_F(CommentExtractorTest, DoesNotRemoveUncommentedLines) {
            m_Line = "int foo = 4;";
            Run();

            ASSERT_EQ(m_Line, "int foo = 4;");
            ASSERT_FALSE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, EmptiesSingleLineCommentLine) {
            m_Line = "//int foo = 4;";
            Run();
            ASSERT_EQ(m_Line, "");
            ASSERT_FALSE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, RemovesCommentFromEndOfLine) {
            m_Line = "int foo = 4; //my comment";
            Run();
            ASSERT_EQ(m_Line, "int foo = 4; ");
            ASSERT_FALSE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, RecognizesBlockCommentBegin) {
            m_Line = "/*";
            Run();
            ASSERT_EQ(m_Line, "");
            ASSERT_TRUE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, RemovesTextAfterBlockCommentBegin) {
            m_Line = "/* hello";
            Run();
            ASSERT_EQ(m_Line, "");
            ASSERT_TRUE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, RemovesBlockCommentBeginIfCommentedOut) {
            m_Line = "// testing /*";
            Run();
            ASSERT_EQ(m_Line, "");
            ASSERT_FALSE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, LineCommentInBlockCommentIsUnaffected) {
            m_Line = "/* testing //";
            Run();
            ASSERT_EQ(m_Line, "");
            ASSERT_TRUE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, CommentBlockInMidLineIsRemoved) {
            m_Line = "hello(1 /*val*/);";
            Run();
            ASSERT_EQ(m_Line, "hello(1 );");
            ASSERT_FALSE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, MultipleInlineCommentsRemoved) {
            m_Line = "hello(1 /*val*/, false /*something else*/, nullptr /*for good measure*/);";
            Run();
            ASSERT_EQ(m_Line, "hello(1 , false , nullptr );");
            ASSERT_FALSE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, BlockCommentWithLineCommentAtEnd) {
            m_Line = "hello(1 /*val*/); //good stuff";
            Run();
            ASSERT_EQ(m_Line, "hello(1 ); ");
            ASSERT_FALSE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, InBlockCommentWithNoEndIsBlank) {
            m_IsInBlockComment = true;
            m_Line = "int foo = 4;";
            Run();
            ASSERT_EQ(m_Line, "");
            ASSERT_TRUE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, InBlockCommentWithJustEndIsBlank) {
            m_IsInBlockComment = true;
            m_Line = "*/";
            Run();
            ASSERT_EQ(m_Line, "");
            ASSERT_FALSE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, InBlockCommentWithLineCommentIsBlank) {
            m_IsInBlockComment = true;
            m_Line = "// testing";
            Run();
            ASSERT_EQ(m_Line, "");
            ASSERT_TRUE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, InBlockCommentWithEndGrabsRemainingText) {
            m_IsInBlockComment = true;
            m_Line = "*/ int foo = 4;";
            Run();
            ASSERT_EQ(m_Line, " int foo = 4;");
            ASSERT_FALSE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, InBlockCommentWithAdditionalBlockIsRemoved) {
            m_IsInBlockComment = true;
            m_Line = "*/ hello(1 /*val*/);";
            Run();
            ASSERT_EQ(m_Line, " hello(1 );");
            ASSERT_FALSE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, InBlockCommentEndsAndStartsNew) {
            m_IsInBlockComment = true;
            m_Line = "*/ /*";
            Run();
            ASSERT_EQ(m_Line, " ");
            ASSERT_TRUE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, InBlockCommentEndsAndHasLineComment) {
            m_IsInBlockComment = true;
            m_Line = "*/ int foo = 4; //testing";
            Run();
            ASSERT_EQ(m_Line, " int foo = 4; ");
            ASSERT_FALSE(m_IsInBlockComment);
        }

        TEST_F(CommentExtractorTest, DoesNotTripOnDivision) {
            m_Line = "int foo = 3/4;";
            Run();
            ASSERT_EQ(m_Line, "int foo = 3/4;");
            ASSERT_FALSE(m_IsInBlockComment);
        }
    } // namespace Extractors
} // namespace IncludeCounter