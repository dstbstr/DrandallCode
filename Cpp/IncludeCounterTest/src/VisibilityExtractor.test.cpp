#include "IncludeCounter/Extractors/VisibilityExtractor.h"

#include "TestCommon.h"

namespace IncludeCounter {
    namespace Extractors {
        TEST(VisibilityExtractorTest, RecognizesPublicVisibility) {
            std::string line = "public:";
            ASSERT_TRUE(VisibilityExtractor::HasVisibility(line));
        }

        TEST(VisibilityExtractorTest, ExtractsPublicVisibility) {
            std::string line = "public:";
            auto visibility = VisibilityExtractor::ExtractVisibility(line);
            ASSERT_EQ(visibility, Visibility::PUBLIC);
            ASSERT_EQ(line, "");
        }

        TEST(VisibilityExtractorTest, RecognizesProtectedVisibility) {
            std::string line = "protected:";
            ASSERT_TRUE(VisibilityExtractor::HasVisibility(line));
        }

        TEST(VisibilityExtractorTest, ExtractsProtectedVisibility) {
            std::string line = "protected:";
            auto visibility = VisibilityExtractor::ExtractVisibility(line);
            ASSERT_EQ(visibility, Visibility::PROTECTED);
            ASSERT_EQ(line, "");
        }

        TEST(VisibilityExtractorTest, RecognizesPrivateVisibility) {
            std::string line = "private:";
            ASSERT_TRUE(VisibilityExtractor::HasVisibility(line));
        }

        TEST(VisibilityExtractorTest, ExtractsPrivateVisibility) {
            std::string line = "private:";
            auto visibility = VisibilityExtractor::ExtractVisibility(line);
            ASSERT_EQ(visibility, Visibility::PRIVATE);
            ASSERT_EQ(line, "");
        }

        TEST(VisibilityExtractorTest, DoesNotRecognizeNonVisibility) {
            std::string line = "default:";
            ASSERT_FALSE(VisibilityExtractor::HasVisibility(line));
        }

        TEST(VisibilityExtractorTest, ThrowsIfNonVisibilityPassedToExtract) {
            std::string line = "default:";
            ASSERT_ANY_THROW(VisibilityExtractor::ExtractVisibility(line));
        }

        TEST(VisibilityExtractorTest, IsCaseSensitive) {
            std::string line = "Public:";
            ASSERT_FALSE(VisibilityExtractor::HasVisibility(line));
        }

        TEST(VisibilityExtractorTest, RequiresTheColon) {
            std::string line = "public";
            ASSERT_FALSE(VisibilityExtractor::HasVisibility(line));
        }

        TEST(VisibilityExtractorTest, KeepsRestOfLine) {
            std::string line = "public: int m_Foo;";
            VisibilityExtractor::ExtractVisibility(line);
            ASSERT_EQ(line, " int m_Foo;");
        }
    } // namespace Extractors
} // namespace IncludeCounter