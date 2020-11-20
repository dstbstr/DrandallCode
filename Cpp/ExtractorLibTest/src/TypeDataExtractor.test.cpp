#include "Extractor/TypeDataExtractor.h"

#include "TestCommon.h"
#include "Utilities/StringUtilities.h"

namespace Extractor {

    using TypeDataExtractor::IsAType;

    TEST(IsATypeTest, ClassIsAType) {
        ASSERT_TRUE(IsAType("class Foo"));
    }

    TEST(IsATypeTest, StructIsAType) {
        ASSERT_TRUE(IsAType("struct Foo"));
    }

    TEST(IsATypeTest, EnumIsAType) {
        ASSERT_TRUE(IsAType("enum Foo"));
    }

    TEST(IsATypeTest, UnionIsAType) {
        ASSERT_TRUE(IsAType("union Foo"));
    }

    TEST(IsATypeTest, OpenParenOnType) {
        ASSERT_TRUE(IsAType("class Foo{"));
    }

    TEST(IsATypeTest, OpenCloseParenOnType) {
        ASSERT_TRUE(IsAType("class Foo{}"));
    }

    TEST(IsATypeTest, OpenCloseParenSemiOnType) {
        ASSERT_TRUE(IsAType("class Foo{};"));
    }

    TEST(IsATypeTest, SingleLineClass) {
        ASSERT_TRUE(IsAType("class Foo{int i;};"));
    }

    TEST(IsATypeTest, TemplateClass) {
        ASSERT_TRUE(IsAType("template<class T> class Foo"));
    }

    TEST(IsATypeTest, TemplateSpecializationClass) {
        ASSERT_TRUE(IsAType("template<> class Foo"));
    }

    TEST(IsATypeTest, DefaultBaseClass) {
        ASSERT_TRUE(IsAType("class Foo : Bar"));
    }

    TEST(IsATypeTest, PublicBaseClass) {
        ASSERT_TRUE(IsAType("class Foo : public Bar"));
    }

    TEST(IsATypeTest, ProtectedBaseClass) {
        ASSERT_TRUE(IsAType("class Foo : protected Bar"));
    }

    TEST(IsATypeTest, PrivateBaseClass) {
        ASSERT_TRUE(IsAType("class Foo : private Bar"));
    }

    TEST(IsATypeTest, GlobalScopeBaseClass) {
        ASSERT_TRUE(IsAType("class Foo : ::Bar"));
    }

    TEST(IsATypeTest, QualifiedBaseClass) {
        ASSERT_TRUE(IsAType("class Foo : Bar::Baz"));
    }

    TEST(IsATypeTest, GlobalQualifiedBaseClass) {
        ASSERT_TRUE(IsAType("class Foo : ::Bar::Baz"));
    }

    TEST(IsATypeTest, TemplatedBaseClass) {
        ASSERT_TRUE(IsAType("class Foo : Bar<Baz>"));
    }

    TEST(IsATypeTest, VirtualInheritence) {
        ASSERT_TRUE(IsAType("class Foo : virtual Bar"));
    }

    TEST(IsATypeTest, ForwardDeclarationIsNotAType) {
        ASSERT_FALSE(IsAType("class Foo;"));
    }

    class ExtractTypeTest : public ::testing::Test {
    protected:
        std::stringstream ss;
        std::string m_FileName{"Test.h"};
        std::string m_Namespace{"Testing"};

        TypeData Extract() {
            std::string line;
            std::getline(ss, line);
            return TypeDataExtractor::Extract(line, m_FileName, m_Namespace, ss);
        }
    };

    TEST_F(ExtractTypeTest, CanExtractClassName) {
        ss << "class Foo";
        auto result = Extract();
        ASSERT_EQ(result.ClassName, "Foo");
    }

    TEST_F(ExtractTypeTest, PopulatesFileName) {
        ss << "class Foo";
        auto result = Extract();
        ASSERT_EQ(result.FileName, m_FileName);
    }

    TEST_F(ExtractTypeTest, PopulatesNamespace) {
        ss << "class Foo";
        auto result = Extract();
        ASSERT_EQ(result.Namespace, m_Namespace);
    }

    TEST_F(ExtractTypeTest, RecognizesClassType) {
        ss << "class Foo";
        auto result = Extract();
        ASSERT_EQ(result.TypeKind, TypeKeyword::CLASS);
    }

    TEST_F(ExtractTypeTest, RecognizesStructType) {
        ss << "struct Foo";
        auto result = Extract();
        ASSERT_EQ(result.TypeKind, TypeKeyword::STRUCT);
    }

    TEST_F(ExtractTypeTest, RecognizesEnumType) {
        ss << "enum Foo";
        auto result = Extract();
        ASSERT_EQ(result.TypeKind, TypeKeyword::ENUM);
    }

    TEST_F(ExtractTypeTest, RecognizesUnionType) {
        ss << "union Foo";
        auto result = Extract();
        ASSERT_EQ(result.TypeKind, TypeKeyword::UNION);
    }

    TEST_F(ExtractTypeTest, CanExtractBaseClass) {
        ss << "class Foo : Bar";
        auto result = Extract();
        ASSERT_TRUE(result.HasBaseClass);
    }

    TEST_F(ExtractTypeTest, CanExtractTemplatedBaseClass) {
        ss << "class Foo : Bar<Baz>";
        auto result = Extract();
        ASSERT_TRUE(result.HasBaseClass);
    }

    TEST_F(ExtractTypeTest, NonBaseClassIsNotMarkedAsHavingBaseClass) {
        ss << "class Foo";
        auto result = Extract();
        ASSERT_FALSE(result.HasBaseClass);
    }

    TEST_F(ExtractTypeTest, CanExtractTemplateClass) {
        ss << "template<class T> class Foo";
        auto result = Extract();
        ASSERT_TRUE(result.IsTemplated);
    }

    TEST_F(ExtractTypeTest, NonTemplateClassIsNotMarkedAsTemplate) {
        ss << "class Foo";
        auto result = Extract();
        ASSERT_FALSE(result.IsTemplated);
    }

    TEST_F(ExtractTypeTest, DefaultsToPublicForStructs) {
        ss << R"(struct Foo
            {
                int i;
            };)";

        auto result = Extract();
        ASSERT_EQ(result.PublicDataMemberCount, 1);
    }

    TEST_F(ExtractTypeTest, DefaultsToPrivateForClasses) {
        ss << R"(class Foo
            {
                int i;
            };)";

        auto result = Extract();
        ASSERT_EQ(result.PrivateDataMemberCount, 1);
    }

    TEST_F(ExtractTypeTest, RecognizesInlineInitializedVariables) {
        ss << R"(class Foo
            {
                int i{0};
            };)";

        auto result = Extract();
        ASSERT_EQ(result.PrivateDataMemberCount, 1);
    }

    TEST_F(ExtractTypeTest, ExtractsInnerTypes) {
        ss << R"(class Foo
            {
                struct Bar {
                    int a;
                    int b;
                };
                enum Baz {QUX, QUXX, QUXXX};
                int i {0};
            };)";

        auto result = Extract();

        ASSERT_EQ(result.InnerTypes.size(), 2);
        ASSERT_EQ(result.InnerTypes[0].ClassName, "Bar");
        ASSERT_EQ(result.InnerTypes[1].ClassName, "Baz");
        ASSERT_EQ(result.PrivateDataMemberCount, 1);
    }

    TEST_F(ExtractTypeTest, RemovesComments) {
        ss << R"(class Foo
            {
                //int a;
            };)";

        auto result = Extract();
        ASSERT_EQ(result.PrivateDataMemberCount, 0);
    }

    TEST_F(ExtractTypeTest, RemovesCommentsBeforeCountingCurlyBraces) {
        ss << R"(class Foo
            {
                //}}}}}}};;;;;
                int a;
            };)";

        auto result = Extract();
        ASSERT_EQ(result.PrivateDataMemberCount, 1);
    }

    TEST_F(ExtractTypeTest, RemovesBlockComments) {
        ss << R"(class Foo
            {
                /*
                int a;
                int b;
                */
               int c;
            };)";

        auto result = Extract();
        ASSERT_EQ(result.PrivateDataMemberCount, 1);
    }

    TEST_F(ExtractTypeTest, StopsReadingLinesAtClassClose) {
        ss << R"(class Foo
            {
                int i;
            };
            Testing)";
        Extract();
        std::string result;
        std::getline(ss, result);

        ASSERT_EQ(StrUtil::Trim(result), "Testing");
    }

    TEST_F(ExtractTypeTest, DoesNotAlterOriginalStream) {
        ss << R"(class Foo
            {
                int i;
            };)";
        std::string original = ss.str();
        Extract();
        ASSERT_EQ(ss.str(), original);
    }

    TEST_F(ExtractTypeTest, AtLeastOneNonBlankLine) {
        ss << "class Foo{};";
        auto result = Extract();
        ASSERT_EQ(result.LineCount, 1);
    }

    TEST_F(ExtractTypeTest, CountsNonBlankLines) {
        ss << R"(class Foo
        {
            int a;


            int b;
        };)";
        auto result = Extract();

        ASSERT_EQ(result.LineCount, 5);
    }

    TEST_F(ExtractTypeTest, CountsLinesFromFunctions) {
        ss << R"(class Foo
        {
            protected: bool Func() {
                return true;
            }

            void Method() {
                i++;
            }
        })";

        auto result = Extract();
        ASSERT_EQ(result.LineCount, 9);
    }

    TEST_F(ExtractTypeTest, CountsLinesFromInnerTypes) {
        ss << R"(class Foo
        {
            public:
            enum Stuff {
                ONE,
                TWO,
                THREE
            };
        })";

        auto result = Extract();
        ASSERT_EQ(result.LineCount, 9);
    }
} // namespace Extractor
