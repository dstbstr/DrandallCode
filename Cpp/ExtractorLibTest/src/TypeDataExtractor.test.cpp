#include "Extractor/TypeDataExtractor.h"

#include "Extractor/Private/LineFetcher.h"
#include "TestCommon.h"
#include "Utilities/StringUtils.h"

namespace Extractor {

    using TypeDataExtractor::IsAType;

    TEST(IsATypeTest, ClassIsAType) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo {", match));
    }

    TEST(IsATypeTest, StructIsAType) {
        std::smatch match;
        ASSERT_TRUE(IsAType("struct Foo {", match));
    }

    TEST(IsATypeTest, EnumIsAType) {
        std::smatch match;
        ASSERT_TRUE(IsAType("enum Foo {", match));
    }

    TEST(IsATypeTest, UnionIsAType) {
        std::smatch match;
        ASSERT_TRUE(IsAType("union Foo {", match));
    }

    TEST(IsATypeTest, InterfaceIsAType) {
        std::smatch match;
        ASSERT_TRUE(IsAType("interface Foo {", match));
    }

    TEST(IsATypeTest, OpenParenOnType) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo{", match));
    }

    TEST(IsATypeTest, OpenCloseCurlyOnType) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo{};", match));
    }

    TEST(IsATypeTest, OpenCloseCurlySemiOnType) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo{};", match));
    }

    TEST(IsATypeTest, SingleLineClass) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo{int i;};", match));
    }

    TEST(IsATypeTest, TemplateClass) {
        std::smatch match;
        ASSERT_TRUE(IsAType("template<class T> class Foo {", match));
    }

    TEST(IsATypeTest, TemplateSpecializationClass) {
        std::smatch match;
        ASSERT_TRUE(IsAType("template<> class Foo {", match));
    }

    TEST(IsATypeTest, DefaultBaseClass) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo : Bar {", match));
    }

    TEST(IsATypeTest, PublicBaseClass) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo : public Bar {", match));
    }

    TEST(IsATypeTest, ProtectedBaseClass) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo : protected Bar {", match));
    }

    TEST(IsATypeTest, PrivateBaseClass) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo : private Bar {", match));
    }

    TEST(IsATypeTest, GlobalScopeBaseClass) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo : ::Bar {", match));
    }

    TEST(IsATypeTest, QualifiedBaseClass) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo : Bar::Baz {", match));
    }

    TEST(IsATypeTest, GlobalQualifiedBaseClass) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo : ::Bar::Baz {", match));
    }

    TEST(IsATypeTest, MultipleInheritence) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo : public Bar, protected Baz {", match));
    }

    TEST(IsATypeTest, TemplatedBaseClass) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo : Bar<Baz> {", match));
    }

    TEST(IsATypeTest, VirtualInheritence) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo : virtual Bar {", match));
    }

    TEST(IsATypeTest, WithDeclSpec) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class __declspec(dllexport) Foo {", match));
    }

    TEST(IsATypeTest, TemplateWithNestedTemplateDefaultParameters) {
        // not making this up, this came from a real code base
        std::smatch match;
        ASSERT_TRUE(IsAType("template<typename DataType, typename ValidationPolicy = Range<DataType>, u32 BitCount = MinMax<DataType>::NumBits, bool ServerOwned = false> class SyncedInt : public SyncedVar {", match));
    }
    TEST(IsATypeTest, WithMacroDeclSpec) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class DLL_EXPORT Foo {", match));
    }

    TEST(IsATypeTest, ClassNameWithTemplateArgs) {
        std::smatch match;
        ASSERT_TRUE(IsAType("template<class T> class Foo<T, false> {", match));
    }

    TEST(IsATypeTest, ClassWithAttribute) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class [[nodiscard]] Foo {", match));
    }

    TEST(IsATypeTest, FinalClass) {
        std::smatch match;
        ASSERT_TRUE(IsAType("class Foo final {", match));
    }

    TEST(IsATypeTest, ForwardDeclarationIsNotAType) {
        std::smatch match;
        ASSERT_FALSE(IsAType("class Foo;", match));
    }

    TEST(IsATypeTest, ForwardDeclarationWithTemplateIsNotAType) {
        std::smatch match;
        ASSERT_FALSE(IsAType("template<class Foo> class Bar;", match));
    }

    TEST(IsATypeTest, TemplateFunctionIsNotAType) {
        std::smatch match;
        ASSERT_FALSE(IsAType("template<class Foo> void RunAll() {", match));
    }

    class ExtractTypeTest : public ::testing::Test {
    protected:
        std::stringstream ss;
        std::string m_FileName{"Test.h"};
        std::string m_Namespace{"Testing"};

        TypeData Extract() {
            std::string line;
            LineFetcher::GetNextLine(ss, line);
            std::smatch match;
            IsAType(line, match);
            return TypeDataExtractor::Extract(match, m_FileName, m_Namespace, ss);
        }
    };

    TEST_F(ExtractTypeTest, CanExtractClassName) {
        ss << "class Foo {";
        auto result = Extract();
        ASSERT_EQ(result.ClassName, "Foo");
    }

    TEST_F(ExtractTypeTest, CanExtractClassNameFromFinalClass) {
        ss << "class Foo final {";
        auto result = Extract();
        ASSERT_EQ(result.ClassName, "Foo");
    }

    TEST_F(ExtractTypeTest, CanExtractClassNameWithAttribute) {
        ss << "class [[nodiscard]] Foo {";
        auto result = Extract();
        ASSERT_EQ(result.ClassName, "Foo");
    }

    TEST_F(ExtractTypeTest, PopulatesFileName) {
        ss << "class Foo {";
        auto result = Extract();
        ASSERT_EQ(result.FileName, m_FileName);
    }

    TEST_F(ExtractTypeTest, PopulatesNamespace) {
        ss << "class Foo {";
        auto result = Extract();
        ASSERT_EQ(result.Namespace, m_Namespace);
    }

    TEST_F(ExtractTypeTest, RecognizesClassType) {
        ss << "class Foo {";
        auto result = Extract();
        ASSERT_EQ(result.TypeKind, TypeKeyword::CLASS);
    }

    TEST_F(ExtractTypeTest, RecognizesStructType) {
        ss << "struct Foo {";
        auto result = Extract();
        ASSERT_EQ(result.TypeKind, TypeKeyword::STRUCT);
    }

    TEST_F(ExtractTypeTest, RecognizesEnumType) {
        ss << "enum Foo {";
        auto result = Extract();
        ASSERT_EQ(result.TypeKind, TypeKeyword::ENUM);
    }

    TEST_F(ExtractTypeTest, RecognizesUnionType) {
        ss << "union Foo {";
        auto result = Extract();
        ASSERT_EQ(result.TypeKind, TypeKeyword::UNION);
    }

    TEST_F(ExtractTypeTest, CanExtractBaseClass) {
        ss << "class Foo : Bar {";
        auto result = Extract();
        ASSERT_TRUE(result.HasBaseClass);
    }

    TEST_F(ExtractTypeTest, CanExtractTemplatedBaseClass) {
        ss << "class Foo : Bar<Baz> {";
        auto result = Extract();
        ASSERT_TRUE(result.HasBaseClass);
    }

    TEST_F(ExtractTypeTest, NonBaseClassIsNotMarkedAsHavingBaseClass) {
        ss << "class Foo {";
        auto result = Extract();
        ASSERT_FALSE(result.HasBaseClass);
    }

    TEST_F(ExtractTypeTest, CanExtractTemplateClass) {
        ss << "template<class T> class Foo {";
        auto result = Extract();
        ASSERT_TRUE(result.IsTemplated);
    }

    TEST_F(ExtractTypeTest, NonTemplateClassIsNotMarkedAsTemplate) {
        ss << "class Foo {";
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

        ASSERT_EQ(result.LineCount, 4);
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
        ASSERT_EQ(result.LineCount, 8);
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
        ASSERT_EQ(result.LineCount, 4);
        /*
        translates to:
        class Foo { //1
            public: enum Stuff { //2
                ONE, TWO, THREE }; //3
            }; //4
        } //5
        */
    }

} // namespace Extractor
