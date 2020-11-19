#include "Extractor/FunctionDataExtractor.h"
#include "TestCommon.h"
#include "Utilities/StringUtilities.h"

namespace Extractor {
    class IsSpecialFunctionTest : public ::testing::Test {
    protected:
        bool Run(std::string line) {
            return FunctionDataExtractor::IsSpecialFunction(line);
        }
    };

    TEST_F(IsSpecialFunctionTest, BasicConstructorDeclaration) {
        ASSERT_TRUE(Run("Foo();"));
    }

    TEST_F(IsSpecialFunctionTest, BasicConstuctorDefinition) {
        ASSERT_TRUE(Run("Foo() {}"));
    }

    TEST_F(IsSpecialFunctionTest, VirtualConstructor) {
        ASSERT_TRUE(Run("virtual Foo()"));
    }

    TEST_F(IsSpecialFunctionTest, ConstructorWithSplitArgs) {
        ASSERT_TRUE(Run("Foo("));
    }

    TEST_F(IsSpecialFunctionTest, ConstructorWithArgs) {
        ASSERT_TRUE(Run("Foo(int a, int b);"));
    }

    TEST_F(IsSpecialFunctionTest, DefaultConstructor) {
        ASSERT_TRUE(Run("Foo() = default;"));
    }

    TEST_F(IsSpecialFunctionTest, DeletedConstructor) {
        ASSERT_TRUE(Run("Foo() = delete;"));
    }

    TEST_F(IsSpecialFunctionTest, BasicDestructorDeclaration) {
        ASSERT_TRUE(Run("~Foo();"));
    }

    TEST_F(IsSpecialFunctionTest, BasicDestructorDefinition) {
        ASSERT_TRUE(Run("~Foo() {}"));
    }

    TEST_F(IsSpecialFunctionTest, VirtualDestructor) {
        ASSERT_TRUE(Run("virtual ~Foo()"));
    }

    TEST_F(IsSpecialFunctionTest, InlineTemplateDefinitionIsSpecial) {
        ASSERT_TRUE(Run("template <typename T> Foo<T>::Foo()"));
    }

    TEST_F(IsSpecialFunctionTest, InlineTemplateQualifiedDestructorIsSpecial) {
        ASSERT_TRUE(Run("template <typename T> Foo<T>::~Foo()"));
    }

    TEST_F(IsSpecialFunctionTest, StaticConstructorNotSpecial) {
        ASSERT_FALSE(Run("Foo CreateClass();"));
    }

    class ExtractSpecialFunctionTest : public ::testing::Test {
    protected:
        SpecialFunctionData RunLine(std::string line) {
            return FunctionDataExtractor::ExtractSpecialFunction(line, ss, m_Namespace, m_Visibility);
        }

        SpecialFunctionData RunArgs(std::string args) {
            return FunctionDataExtractor::ExtractSpecialFunction("Foo(" + args + ")", ss, m_Namespace, m_Visibility);
        }

        SpecialFunctionData Run() {
            return FunctionDataExtractor::ExtractSpecialFunction("Foo()", ss, m_Namespace, m_Visibility);
        }

        std::stringstream ss;
        std::string m_Namespace{"Testing"};
        Visibility m_Visibility{Visibility::PUBLIC};
    };

    TEST_F(ExtractSpecialFunctionTest, PopulatesClassName) {
        auto result = RunLine("Foo();");
        ASSERT_EQ(result.ClassName, "Foo");
    }

    TEST_F(ExtractSpecialFunctionTest, PopulatesNamespace) {
        auto result = Run();
        ASSERT_EQ(result.Namespace, m_Namespace);
    }

    TEST_F(ExtractSpecialFunctionTest, PopulatesVisibility) {
        auto result = Run();
        ASSERT_EQ(result.Visibility, m_Visibility);
    }

    TEST_F(ExtractSpecialFunctionTest, RecognizesVirtual) {
        auto result = RunLine("virtual Foo();");
        ASSERT_TRUE(result.IsVirtual);
    }

    TEST_F(ExtractSpecialFunctionTest, NonVirtualIsNotCalledVirtual) {
        auto result = Run();
        ASSERT_FALSE(result.IsVirtual);
    }

    TEST_F(ExtractSpecialFunctionTest, SingleArgHasAirityOfOne) {
        auto result = RunArgs("int a");
        ASSERT_EQ(result.Airity, 1);
    }

    TEST_F(ExtractSpecialFunctionTest, ZeroArgsHasAirityOfZero) {
        auto result = Run();
        ASSERT_EQ(result.Airity, 0);
    }

    TEST_F(ExtractSpecialFunctionTest, TwoArgsHasAirityOfTwo) {
        auto result = RunArgs("int a, int b");
        ASSERT_EQ(result.Airity, 2);
    }

    TEST_F(ExtractSpecialFunctionTest, SingleArgWithMultiTemplateHasAirityOfOne) {
        auto result = RunArgs("std::pair<int, int> p");
        ASSERT_EQ(result.Airity, 1);
    }

    TEST_F(ExtractSpecialFunctionTest, SingleArgWithTemplateHasAirityOfOne) {
        auto result = RunArgs("std::vector<int> v");
        ASSERT_EQ(result.Airity, 1);
    }

    TEST_F(ExtractSpecialFunctionTest, NoDefaultArgsHasDefaultArgCountOfZero) {
        auto result = Run();
        ASSERT_EQ(result.DefaultParameterCount, 0);
    }

    TEST_F(ExtractSpecialFunctionTest, SingleDefaultArgHasDefaultArgCountOfOne) {
        auto result = RunArgs("int a = 3");
        ASSERT_EQ(result.DefaultParameterCount, 1);
    }

    TEST_F(ExtractSpecialFunctionTest, RemovesFunctionBodyFromStream) {
        ss << R"({
                return true;
            })";

        Run();
        std::string line;
        ASSERT_FALSE(std::getline(ss, line)) << line;
    }

    TEST_F(ExtractSpecialFunctionTest, RemovesFunctionBodyFromStreamWithEgyptionBraces) {
        ss << R"(
                return true;
            })";

        RunLine("Foo() {");
        std::string line;
        ASSERT_FALSE(std::getline(ss, line));
    }

    TEST_F(ExtractSpecialFunctionTest, DoesNotAlterOriginalStream) {
        ss << R"({
                return true;
            })";
        std::string original = ss.str();
        Run();
        ASSERT_EQ(ss.str(), original);
    }

    TEST_F(ExtractSpecialFunctionTest, DoesNotRemoveLinesIfDefinitionIsOnASingleLine) {
        ss << "Testing";
        RunLine("Foo() {}");

        std::string line;
        ASSERT_TRUE(std::getline(ss, line));
        ASSERT_EQ(line, "Testing");
    }

    TEST_F(ExtractSpecialFunctionTest, CombinesMultipleParameterLines) {
        ss << R"(
                int a,
                int b
                )
                {
                    return true;
                })";

        auto result = RunLine("Foo(");
        std::string line;
        ASSERT_FALSE(std::getline(ss, line));
        ASSERT_EQ(result.Airity, 2);
    }

    TEST_F(ExtractSpecialFunctionTest, RemovesCommentsFromParameters) {
        auto result = RunLine("Foo(int a /*1, 3, or 5*/, int b /*2, 4, or 6*/);");
        ASSERT_EQ(result.Airity, 2);
    }

    TEST_F(ExtractSpecialFunctionTest, AtLeastOneLine) {
        auto result = Run();
        ASSERT_EQ(result.LineCount, 1);
    }

    TEST_F(ExtractSpecialFunctionTest, CountsEverythingBeforeCurliesAsOneLine) {
        ss << R"(
            : m_Int(3)
            , m_Bool(false)

            {}
        )";

        auto result = Run();
        ASSERT_EQ(result.LineCount, 1);
    }

    TEST_F(ExtractSpecialFunctionTest, CountsLinesInBody) {
        ss << R"({
            int i = 0;

            i++;
        })";

        auto result = Run();
        ASSERT_EQ(result.LineCount, 5);
    }
} // namespace Extractor
