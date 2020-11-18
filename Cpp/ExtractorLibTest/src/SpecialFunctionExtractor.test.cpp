#include "Extractor/FunctionDataExtractor.h"

#include "TestCommon.h"
#include "Utilities/StringUtilities.h"

namespace Extractor {
    class IsSpecialFunctionTest : public ::testing::Test {
    protected:
        bool Run(std::string line) {
            return FunctionDataExtractor::IsSpecialFunction(line, m_ClassName);
        }

        std::string m_ClassName{"TestClass"};
    };

    TEST_F(IsSpecialFunctionTest, BasicConstructorDeclaration) {
        ASSERT_TRUE(Run(m_ClassName + "();"));
    }

    TEST_F(IsSpecialFunctionTest, BasicConstuctorDefinition) {
        ASSERT_TRUE(Run(m_ClassName + "() {}"));
    }

    TEST_F(IsSpecialFunctionTest, VirtualConstructor) {
        ASSERT_TRUE(Run("virtual " + m_ClassName + "()"));
    }

    TEST_F(IsSpecialFunctionTest, ConstructorWithSplitArgs) {
        ASSERT_TRUE(Run(m_ClassName + "("));
    }

    TEST_F(IsSpecialFunctionTest, ConstructorWithArgs) {
        ASSERT_TRUE(Run(m_ClassName + "(int a, int b);"));
    }

    TEST_F(IsSpecialFunctionTest, DefaultConstructor) {
        ASSERT_TRUE(Run(m_ClassName + "() = default;"));
    }

    TEST_F(IsSpecialFunctionTest, DeletedConstructor) {
        ASSERT_TRUE(Run(m_ClassName + "() = delete;"));
    }

    TEST_F(IsSpecialFunctionTest, BasicDestructorDeclaration) {
        ASSERT_TRUE(Run("~" + m_ClassName + "();"));
    }

    TEST_F(IsSpecialFunctionTest, BasicDestructorDefinition) {
        ASSERT_TRUE(Run("~" + m_ClassName + "() {}"));
    }

    TEST_F(IsSpecialFunctionTest, VirtualDestructor) {
        ASSERT_TRUE(Run("virtual ~" + m_ClassName + "()"));
    }

    TEST_F(IsSpecialFunctionTest, MismatchedClassNameNotSpecialFunction) {
        std::string line = "Foo();";
        ASSERT_FALSE(FunctionDataExtractor::IsSpecialFunction(line, "Bar"));
    }

    TEST_F(IsSpecialFunctionTest, StaticConstructorNotSpecial) {
        ASSERT_FALSE(Run(m_ClassName + " CreateClass();"));
    }

    class ExtractSpecialFunctionTest : public ::testing::Test {
    protected:
        SpecialFunctionData RunLine(std::string line) {
            return FunctionDataExtractor::ExtractSpecialFunction(line, ss, m_Namespace, m_ClassName, m_Visibility);
        }

        SpecialFunctionData RunArgs(std::string args) {
            return FunctionDataExtractor::ExtractSpecialFunction(m_ClassName + "(" + args + ")", ss, m_Namespace, m_ClassName, m_Visibility);
        }

        SpecialFunctionData Run() {
            return FunctionDataExtractor::ExtractSpecialFunction(m_ClassName + "()", ss, m_Namespace, m_ClassName, m_Visibility);
        }

        std::stringstream ss;
        std::string m_ClassName{"className"};
        std::string m_Namespace{"Testing"};
        Visibility m_Visibility{Visibility::PUBLIC};
    };

    TEST_F(ExtractSpecialFunctionTest, PopulatesClassName) {
        auto result = Run();
        ASSERT_EQ(result.ClassName, m_ClassName);
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
        auto result = RunLine("virtual " + m_ClassName + "();");
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

        RunLine(m_ClassName + "() {");
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
        RunLine(m_ClassName + "() {}");

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

        auto result = RunLine(m_ClassName + "(");
        std::string line;
        ASSERT_FALSE(std::getline(ss, line));
        ASSERT_EQ(result.Airity, 2);
    }

    TEST_F(ExtractSpecialFunctionTest, RemovesCommentsFromParameters) {
        auto result = RunLine(m_ClassName + "(int a /*1, 3, or 5*/, int b /*2, 4, or 6*/);");
        ASSERT_EQ(result.Airity, 2);
    }

} // namespace Extractor
