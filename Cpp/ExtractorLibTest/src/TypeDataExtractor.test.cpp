#include "Extractor/TypeDataExtractor.h"

#include "TestCommon.h"
#include "Utilities/StringUtilities.h"

namespace Extractor {

    class IsATypeTest : public ::testing::Test {
    protected:
        bool Run(std::string line) {
            return TypeDataExtractor::IsAType(line);
        }
    };

    TEST_F(IsATypeTest, ClassIsAType) {
        ASSERT_TRUE(Run("class Foo"));
    }

    TEST_F(IsATypeTest, StructIsAType) {
        ASSERT_TRUE(Run("struct Foo"));
    }

    TEST_F(IsATypeTest, EnumIsAType) {
        ASSERT_TRUE(Run("enum Foo"));
    }

    TEST_F(IsATypeTest, UnionIsAType) {
        ASSERT_TRUE(Run("union Foo"));
    }

    TEST_F(IsATypeTest, OpenParenOnType) {
        ASSERT_TRUE(Run("class Foo{"));
    }

    TEST_F(IsATypeTest, OpenCloseParenOnType) {
        ASSERT_TRUE(Run("class Foo{}"));
    }

    TEST_F(IsATypeTest, OpenCloseParenSemiOnType) {
        ASSERT_TRUE(Run("class Foo{};"));
    }

    TEST_F(IsATypeTest, SingleLineClass) {
        ASSERT_TRUE(Run("class Foo{int i;};"));
    }

    TEST_F(IsATypeTest, TemplateClass) {
        ASSERT_TRUE(Run("template<class T> class Foo"));
    }

    TEST_F(IsATypeTest, TemplateSpecializationClass) {
        ASSERT_TRUE(Run("template<> class Foo"));
    }

    TEST_F(IsATypeTest, DefaultBaseClass) {
        ASSERT_TRUE(Run("class Foo : Bar"));
    }

    TEST_F(IsATypeTest, PublicBaseClass) {
        ASSERT_TRUE(Run("class Foo : public Bar"));
    }

    TEST_F(IsATypeTest, ProtectedBaseClass) {
        ASSERT_TRUE(Run("class Foo : protected Bar"));
    }

    TEST_F(IsATypeTest, PrivateBaseClass) {
        ASSERT_TRUE(Run("class Foo : private Bar"));
    }

    TEST_F(IsATypeTest, GlobalScopeBaseClass) {
        ASSERT_TRUE(Run("class Foo : ::Bar"));
    }

    TEST_F(IsATypeTest, QualifiedBaseClass) {
        ASSERT_TRUE(Run("class Foo : Bar::Baz"));
    }

    TEST_F(IsATypeTest, GlobalQualifiedBaseClass) {
        ASSERT_TRUE(Run("class Foo : ::Bar::Baz"));
    }

    TEST_F(IsATypeTest, TemplatedBaseClass) {
        ASSERT_TRUE(Run("class Foo : Bar<Baz>"));
    }

    TEST_F(IsATypeTest, VirtualInheritence) {
        ASSERT_TRUE(Run("class Foo : virtual Bar"));
    }

    TEST_F(IsATypeTest, ForwardDeclarationIsNotAType) {
        ASSERT_FALSE(Run("class Foo;"));
    }

    class ExtractTypeTest : public ::testing::Test {
    protected:
        std::stringstream ss;
        std::string m_FileName{"Test.h"};
        std::string m_Namespace{"Testing"};

        TypeData Run(std::string firstLine) {
            return TypeDataExtractor::Extract(firstLine, m_FileName, m_Namespace, ss);
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

    TEST_F(ExtractTypeTest, PopulatesNamespace) {
        auto result = Run("class Foo");
        ASSERT_EQ(result.Namespace, m_Namespace);
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

    TEST_F(ExtractTypeTest, CanExtractTemplatedBaseClass) {
        auto result = Run("class Foo : Bar<Baz>");
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

    TEST_F(ExtractTypeTest, AtLeastOneNonBlankLine) {
        auto result = Run("class Foo{};");
        ASSERT_EQ(result.LineCount, 1);
    }

    TEST_F(ExtractTypeTest, CountsNonBlankLines) {
        ss << R"({
            int a;


            int b;
        };)";
        auto result = Run("class Foo");

        ASSERT_EQ(result.LineCount, 5);
    }

    TEST_F(ExtractTypeTest, CountsLinesFromFunctions) {
        ss << R"({
            protected: bool Func() {
                return true;
            }

            void Method() {
                i++;
            }
        })";

        auto result = Run("class Foo");
        ASSERT_EQ(result.LineCount, 9);
    }

    TEST_F(ExtractTypeTest, CountsLinesFromInnerTypes) {
        ss << R"({
            public:
            enum Stuff {
                ONE,
                TWO,
                THREE
            };
        })";

        auto result = Run("class Foo");
        ASSERT_EQ(result.LineCount, 9);
    }
} // namespace Extractor
