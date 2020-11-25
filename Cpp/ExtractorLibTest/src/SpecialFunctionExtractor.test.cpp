#include "Extractor/FunctionDataExtractor.h"
#include "TestCommon.h"
#include "Utilities/StringUtils.h"

namespace Extractor {
    using FunctionDataExtractor::IsSpecialFunction;

    TEST(IsSpecialFunctionTest, BasicConstructorDeclaration) {
        ASSERT_TRUE(IsSpecialFunction("Foo();"));
    }

    TEST(IsSpecialFunctionTest, BasicConstuctorDefinition) {
        ASSERT_TRUE(IsSpecialFunction("Foo() {}"));
    }

    TEST(IsSpecialFunctionTest, VirtualConstructor) {
        ASSERT_TRUE(IsSpecialFunction("virtual Foo()"));
    }

    TEST(IsSpecialFunctionTest, ConstructorWithSplitArgs) {
        ASSERT_TRUE(IsSpecialFunction("Foo("));
    }

    TEST(IsSpecialFunctionTest, ConstructorWithArgs) {
        ASSERT_TRUE(IsSpecialFunction("Foo(int a, int b);"));
    }

    TEST(IsSpecialFunctionTest, DefaultConstructor) {
        ASSERT_TRUE(IsSpecialFunction("Foo() = default;"));
    }

    TEST(IsSpecialFunctionTest, DeletedConstructor) {
        ASSERT_TRUE(IsSpecialFunction("Foo() = delete;"));
    }

    TEST(IsSpecialFunctionTest, BasicDestructorDeclaration) {
        ASSERT_TRUE(IsSpecialFunction("~Foo();"));
    }

    TEST(IsSpecialFunctionTest, BasicDestructorDefinition) {
        ASSERT_TRUE(IsSpecialFunction("~Foo() {}"));
    }

    TEST(IsSpecialFunctionTest, VirtualDestructor) {
        ASSERT_TRUE(IsSpecialFunction("virtual ~Foo()"));
    }

    TEST(IsSpecialFunctionTest, InlineTemplateDefinitionIsSpecial) {
        ASSERT_TRUE(IsSpecialFunction("template <typename T> Foo<T>::Foo()"));
    }

    TEST(IsSpecialFunctionTest, InlineTemplateQualifiedDestructorIsSpecial) {
        ASSERT_TRUE(IsSpecialFunction("template <typename T> Foo<T>::~Foo()"));
    }

    TEST(IsSpecialFunctionTest, ExplicitConstructor) {
        ASSERT_TRUE(IsSpecialFunction("explicit Foo(int i);"));
    }

    TEST(IsSpecialFunctionTest, StaticConstructorNotSpecial) {
        ASSERT_FALSE(IsSpecialFunction("Foo CreateClass();"));
    }

    class ExtractSpecialFunctionTest : public ::testing::Test {
    protected:
        SpecialFunctionData Extract() {
            std::string line;
            std::getline(ss, line);
            return FunctionDataExtractor::ExtractSpecialFunction(line, ss, m_Namespace, m_Visibility);
        }

        std::stringstream ss;
        std::string m_Namespace{"Testing"};
        Visibility m_Visibility{Visibility::PUBLIC};
    };

    TEST_F(ExtractSpecialFunctionTest, PopulatesClassName) {
        ss << "Foo();";
        auto result = Extract();
        ASSERT_EQ(result.ClassName, "Foo");
    }

    TEST_F(ExtractSpecialFunctionTest, PopulatesNamespace) {
        ss << "Foo();";
        auto result = Extract();
        ASSERT_EQ(result.Namespace, m_Namespace);
    }

    TEST_F(ExtractSpecialFunctionTest, PopulatesVisibility) {
        ss << "Foo();";
        auto result = Extract();
        ASSERT_EQ(result.Visibility, m_Visibility);
    }

    TEST_F(ExtractSpecialFunctionTest, RecognizesVirtual) {
        ss << "virtual Foo();";
        auto result = Extract();
        ASSERT_TRUE(result.IsVirtual);
    }

    TEST_F(ExtractSpecialFunctionTest, NonVirtualIsNotCalledVirtual) {
        ss << "Foo();";
        auto result = Extract();
        ASSERT_FALSE(result.IsVirtual);
    }

    TEST_F(ExtractSpecialFunctionTest, ExplicitConstructorIsExplicit) {
        ss << "explicit Foo();";
        auto result = Extract();
        ASSERT_TRUE(result.IsExplicit);
    }

    TEST_F(ExtractSpecialFunctionTest, NonExplicitConstructorIsNotExplicit) {
        ss << "Foo();";
        auto result = Extract();
        ASSERT_FALSE(result.IsExplicit);
    }

    TEST_F(ExtractSpecialFunctionTest, SingleArgHasAirityOfOne) {
        ss << "Foo(int a);";
        auto result = Extract();
        ASSERT_EQ(result.Airity, 1);
    }

    TEST_F(ExtractSpecialFunctionTest, ZeroArgsHasAirityOfZero) {
        ss << "Foo();";
        auto result = Extract();
        ASSERT_EQ(result.Airity, 0);
    }

    TEST_F(ExtractSpecialFunctionTest, TwoArgsHasAirityOfTwo) {
        ss << "Foo(int a, int b);";
        auto result = Extract();
        ASSERT_EQ(result.Airity, 2);
    }

    TEST_F(ExtractSpecialFunctionTest, SingleArgWithMultiTemplateHasAirityOfOne) {
        ss << "Foo(std::pair<int, int> p);";
        auto result = Extract();
        ASSERT_EQ(result.Airity, 1);
    }

    TEST_F(ExtractSpecialFunctionTest, SingleArgWithTemplateHasAirityOfOne) {
        ss << "Foo(std::vector<int> v);";
        auto result = Extract();
        ASSERT_EQ(result.Airity, 1);
    }

    TEST_F(ExtractSpecialFunctionTest, NoDefaultArgsHasDefaultArgCountOfZero) {
        ss << "Foo();";
        auto result = Extract();
        ASSERT_EQ(result.DefaultParameterCount, 0);
    }

    TEST_F(ExtractSpecialFunctionTest, SingleDefaultArgHasDefaultArgCountOfOne) {
        ss << "Foo(int a = 3);";
        auto result = Extract();
        ASSERT_EQ(result.DefaultParameterCount, 1);
    }

    TEST_F(ExtractSpecialFunctionTest, RemovesFunctionBodyFromStream) {
        ss << R"(Foo()
            {
                return true;
            })";

        Extract();
        std::string line;
        ASSERT_FALSE(std::getline(ss, line)) << line;
    }

    TEST_F(ExtractSpecialFunctionTest, RemovesFunctionBodyFromStreamWithEgyptionBraces) {
        ss << R"(Foo() {
                return true;
            })";

        Extract();
        std::string line;
        ASSERT_FALSE(std::getline(ss, line));
    }

    TEST_F(ExtractSpecialFunctionTest, DoesNotAlterOriginalStream) {
        ss << R"(Foo() {
                return true;
            })";
        std::string original = ss.str();
        Extract();
        ASSERT_EQ(ss.str(), original);
    }

    TEST_F(ExtractSpecialFunctionTest, DoesNotRemoveLinesIfDefinitionIsOnASingleLine) {
        ss << R"(Foo() {}
        Testing;)";
        Extract();

        std::string line;
        ASSERT_TRUE(std::getline(ss, line));
        ASSERT_EQ(StrUtil::Trim(line), "Testing;");
    }

    TEST_F(ExtractSpecialFunctionTest, CombinesMultipleParameterLines) {
        ss << R"(Foo(
                int a,
                int b
                )
                {
                    return true;
                })";

        auto result = Extract();
        std::string line;
        ASSERT_FALSE(std::getline(ss, line));
        ASSERT_EQ(result.Airity, 2);
    }

    TEST_F(ExtractSpecialFunctionTest, RemovesCommentsFromParameters) {
        ss << "Foo(int a /*1, 3, or 5*/, int b /* = 2 */);";
        auto result = Extract();
        ASSERT_EQ(result.Airity, 2);
    }

    TEST_F(ExtractSpecialFunctionTest, AtLeastOneLine) {
        ss << "Foo();";
        auto result = Extract();
        ASSERT_EQ(result.LineCount, 1);
    }

    TEST_F(ExtractSpecialFunctionTest, CountsEverythingBeforeCurliesAsOneLine) {
        ss << R"(Foo()
            : m_Int(3)
            , m_Bool(false)

            {}
        )";

        auto result = Extract();
        ASSERT_EQ(result.LineCount, 1);
    }

    TEST_F(ExtractSpecialFunctionTest, CountsLinesInBody) {
        ss << R"(Foo() {
            int i = 0;

            i++;
        })";

        auto result = Extract();
        ASSERT_EQ(result.LineCount, 5);
    }
} // namespace Extractor
