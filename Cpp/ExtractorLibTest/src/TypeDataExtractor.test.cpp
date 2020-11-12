#include "Extractor/TypeDataExtractor.h"

#include "TestCommon.h"
#include "Utilities/StringUtilities.h"

namespace Extractor {

    class IsATypeTest : public ::testing::Test {
    protected:
        std::vector<std::string> m_TemplatePrefixes{"template<class T>", "template<>", ""};
        std::vector<std::string> m_Types{"struct", "class", "enum", "union"};
        std::string m_TypeName = "Foo";
        std::vector<std::string> m_Scopes{"public", "private", "protected", ""};
        std::vector<std::string> m_BaseClasses{"Bar", "::Bar", "Bar::Baz", "::Bar::baz"};
        std::vector<std::string> m_InheritenceModifiers{"virtual", ""};
        std::vector<std::string> m_TypeEndings{"", "{", "{}", "{};", "{int i;};"};
        std::vector<std::string> m_NonTypeEndings{";"};
    };

    TEST_F(IsATypeTest, TypesReturnTrue) {
        for(auto&& templatePrefix: m_TemplatePrefixes) {
            for(auto&& type: m_Types) {
                for(auto&& typeEnding: m_TypeEndings) {
                    auto line = StrUtil::Trim(StrUtil::Join(" ", templatePrefix, type, m_TypeName, typeEnding));
                    ASSERT_TRUE(TypeDataExtractor::IsAType(line)) << line;
                }

                for(auto&& scope: m_Scopes) {
                    for(auto&& baseClass: m_BaseClasses) {
                        for(auto&& inheritenceModifier: m_InheritenceModifiers) {
                            for(auto&& typeEnding: m_TypeEndings) {
                                auto line = StrUtil::Trim(
                                    StrUtil::Join(" ", templatePrefix, type, m_TypeName, ":", scope, inheritenceModifier, baseClass, typeEnding));
                                ASSERT_TRUE(TypeDataExtractor::IsAType(line)) << line;
                            }
                        }
                    }
                }
            }
        }
    }

    TEST_F(IsATypeTest, NonTypesReturnFalse) {
        for(auto&& templatePrefix: m_TemplatePrefixes) {
            for(auto&& type: m_Types) {
                for(auto&& typeEnding: m_NonTypeEndings) {
                    auto line = StrUtil::Trim(StrUtil::Join(" ", templatePrefix, type, typeEnding));
                    ASSERT_FALSE(TypeDataExtractor::IsAType(line)) << line;
                }

                for(auto&& scope: m_Scopes) {
                    for(auto&& baseClass: m_BaseClasses) {
                        for(auto&& inheritenceModifier: m_InheritenceModifiers) {
                            for(auto&& typeEnding: m_NonTypeEndings) {
                                auto line =
                                    StrUtil::Trim(StrUtil::Join(" ", templatePrefix, type, ":", scope, baseClass, inheritenceModifier, typeEnding));
                                ASSERT_FALSE(TypeDataExtractor::IsAType(line)) << line;
                            }
                        }
                    }
                }
            }
        }
    }

    class ExtractTypeTest : public ::testing::Test {
    protected:
        std::stringstream ss;
        std::string m_FileName{"Test.h"};

        TypeData Run(std::string firstLine) {
            return TypeDataExtractor::Extract(firstLine, m_FileName, ss);
        }
    };

    TEST_F(ExtractTypeTest, CanExtractClassName) {
        auto result = Run("class Foo");
        ASSERT_EQ(result.ClassName, "Foo");
    }

    TEST_F(ExtractTypeTest, PopulatesFileName) {
        auto result = Run("class Foo");
        ASSERT_EQ(result.FileName, m_FileName);
    }

    TEST_F(ExtractTypeTest, RecognizesClassType) {
        auto result = Run("class Foo");
        ASSERT_EQ(result.TypeKind, TypeKeyword::CLASS);
    }

    TEST_F(ExtractTypeTest, RecognizesStructType) {
        auto result = Run("struct Foo");
        ASSERT_EQ(result.TypeKind, TypeKeyword::STRUCT);
    }

    TEST_F(ExtractTypeTest, RecognizesEnumType) {
        auto result = Run("enum Foo");
        ASSERT_EQ(result.TypeKind, TypeKeyword::ENUM);
    }

    TEST_F(ExtractTypeTest, RecognizesUnionType) {
        auto result = Run("union Foo");
        ASSERT_EQ(result.TypeKind, TypeKeyword::UNION);
    }

    TEST_F(ExtractTypeTest, CanExtractBaseClass) {
        auto result = Run("class Foo : Baz");
        ASSERT_TRUE(result.HasBaseClass);
    }

    TEST_F(ExtractTypeTest, NonBaseClassIsNotMarkedAsHavingBaseClass) {
        auto result = Run("class Foo");
        ASSERT_FALSE(result.HasBaseClass);
    }

    TEST_F(ExtractTypeTest, CanExtractTemplateClass) {
        auto result = Run("template<class T> class Foo");
        ASSERT_TRUE(result.IsTemplated);
    }

    TEST_F(ExtractTypeTest, NonTemplateClassIsNotMarkedAsTemplate) {
        auto result = Run("class Foo");
        ASSERT_FALSE(result.IsTemplated);
    }

    TEST_F(ExtractTypeTest, DefaultsToPublicForStructs) {
        ss << R"(
            {
                int i;
            };)";

        auto result = Run("struct Foo");
        ASSERT_EQ(result.PublicDataMemberCount, 1);
    }

    TEST_F(ExtractTypeTest, DefaultsToPrivateForClasses) {
        ss << R"(
            {
                int i;
            };)";

        auto result = Run("class Foo");
        ASSERT_EQ(result.PrivateDataMemberCount, 1);
    }

    TEST_F(ExtractTypeTest, RecognizesInlineInitializedVariables) {
        ss << R"({
                int i{0};
            };)";

        auto result = Run("class Foo");
        ASSERT_EQ(result.PrivateDataMemberCount, 1);
    }

    TEST_F(ExtractTypeTest, ExtractsInnerTypes) {
        ss << R"({
                struct Bar {
                    int a;
                    int b;
                };
                enum Baz {QUX, QUXX, QUXXX};
                int i {0};
            };)";

        auto result = Run("class Foo");

        ASSERT_EQ(result.InnerTypes.size(), 2);
        ASSERT_EQ(result.InnerTypes[0].ClassName, "Bar");
        ASSERT_EQ(result.InnerTypes[1].ClassName, "Baz");
        ASSERT_EQ(result.PrivateDataMemberCount, 1);
    }

    TEST_F(ExtractTypeTest, RemovesComments) {
        ss << R"({
                //int a;
            };)";

        auto result = Run("class Foo");
        ASSERT_EQ(result.PrivateDataMemberCount, 0);
    }

    TEST_F(ExtractTypeTest, RemovesCommentsBeforeCountingCurlyBraces) {
        ss << R"({
                //}}}}}}};;;;;
                int a;
            };)";

        auto result = Run("class Foo");
        ASSERT_EQ(result.PrivateDataMemberCount, 1);
    }

    TEST_F(ExtractTypeTest, RemovesBlockComments) {
        ss << R"({
                /*
                int a;
                int b;
                */
               int c;
            };)";

        auto result = Run("class Foo");
        ASSERT_EQ(result.PrivateDataMemberCount, 1);
    }

    TEST_F(ExtractTypeTest, StopsReadingLinesAtClassClose) {
        ss << R"({
                int i;
            };
            Testing)";
        Run("class Foo");
        std::string result;
        std::getline(ss, result);

        ASSERT_EQ(StrUtil::Trim(result), "Testing");
    }

    TEST_F(ExtractTypeTest, DoesNotAlterOriginalStream) {
        ss << R"({
                int i;
            };)";
        std::string original = ss.str();
        Run("class Foo");
        ASSERT_EQ(ss.str(), original);
    }
} // namespace Extractor
