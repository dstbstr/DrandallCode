#include "TestCommon.h"

#include "Extractor/FunctionDataExtractor.h"
#include "Extractor/Private/LineFetcher.h"

#include "Core/Utilities/StringUtils.h"

namespace Extractor {
    using FunctionDataExtractor::IsSpecialFunction;

    TEST(IsSpecialFunctionTest, BasicConstructorDeclaration) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("Foo();", match));
    }

    TEST(IsSpecialFunctionTest, BasicConstuctorDefinition) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("Foo() {}", match));
    }

    TEST(IsSpecialFunctionTest, VirtualConstructor) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("virtual Foo()", match));
    }

    TEST(IsSpecialFunctionTest, ClassWhichStartsWithUnderscore) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("_Foo();", match));
    }
    TEST(IsSpecialFunctionTest, ClassWhichEndsWithUnderscore) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("Foo_();", match));
    }

    TEST(IsSpecialFunctionTest, ClassWhichEndsWithCapitalLetter) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("FooA();", match));
    }

    TEST(IsSpecialFunctionTest, ClassWhichStartsWithMultipleCapitalLetters) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("CFoo();", match));
    }

    TEST(IsSpecialFunctionTest, ConstructorWithSplitArgs) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("Foo(", match));
    }

    TEST(IsSpecialFunctionTest, ConstructorWithArgs) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("Foo(int a, int b);", match));
    }

    TEST(IsSpecialFunctionTest, DefaultConstructor) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("Foo() = default;", match));
    }

    TEST(IsSpecialFunctionTest, DeletedConstructor) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("Foo() = delete;", match));
    }

    TEST(IsSpecialFunctionTest, BasicDestructorDeclaration) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("~Foo();", match));
    }

    TEST(IsSpecialFunctionTest, BasicDestructorDefinition) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("~Foo() {}", match));
    }

    TEST(IsSpecialFunctionTest, VirtualDestructor) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("virtual ~Foo()", match));
    }

    TEST(IsSpecialFunctionTest, InlineTemplateDefinitionIsSpecial) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("template <typename T> Foo<T>::Foo()", match));
    }

    TEST(IsSpecialFunctionTest, InlineTemplateQualifiedDestructorIsSpecial) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("template <typename T> Foo<T>::~Foo()", match));
    }

    TEST(IsSpecialFunctionTest, ExplicitConstructor) {
        std::smatch match;
        ASSERT_TRUE(IsSpecialFunction("explicit Foo(int i);", match));
    }

    TEST(IsSpecialFunctionTest, StaticConstructorNotSpecial) {
        std::smatch match;
        ASSERT_FALSE(IsSpecialFunction("Foo CreateClass();", match));
    }

    TEST(IsSpecialFunctionTest, StaticAssertNotSpecial) {
        std::smatch match;
        ASSERT_FALSE(IsSpecialFunction("static_assert(i < 25);", match));
    }

    TEST(IsSpecialFunctionTest, MacroIsNotSpecial) {
        std::smatch match;
        ASSERT_FALSE(IsSpecialFunction("ASSERT_FALSE(i < 25);", match));
    }

    class ExtractSpecialFunctionTest : public ::testing::Test {
    protected:
        SpecialFunctionData Extract() {
            std::string line;
            LineFetcher::GetNextLine(ss, line);
            std::smatch match;
            IsSpecialFunction(line, match);
            return FunctionDataExtractor::ExtractSpecialFunction(line, match, ss, m_Namespace, m_Visibility);
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

    TEST_F(ExtractSpecialFunctionTest, DeletedConstructorIsDeleted) {
        ss << "Foo() = delete;";
        auto result = Extract();
        ASSERT_TRUE(result.IsDeleted);
    }

    TEST_F(ExtractSpecialFunctionTest, NonDeletedConstructorIsNotDeleted) {
        ss << "Foo();";
        auto result = Extract();
        ASSERT_FALSE(result.IsDeleted);
    }

    TEST_F(ExtractSpecialFunctionTest, DefaultConstructorIsDefault) {
        ss << "Foo() = default;";
        auto result = Extract();
        ASSERT_TRUE(result.IsDefaulted);
    }

    TEST_F(ExtractSpecialFunctionTest, NonDefaultConstructorIsNotDefaulted) {
        ss << "Foo();";
        auto result = Extract();
        ASSERT_FALSE(result.IsDefaulted);
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
        ASSERT_EQ(result.LineCount, 4);
    }
} // namespace Extractor
