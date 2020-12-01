#include "Extractor/FunctionDataExtractor.h"

#include "TestCommon.h"
#include "Utilities/StringUtils.h"

namespace Extractor {
    using FunctionDataExtractor::IsAFunction;

    TEST(IsFunctionTest, SimpleFunctionDeclaration) {
        ASSERT_TRUE(IsAFunction("void Foo();"));
    }

    TEST(IsFunctionTest, SimpleFunctionDefinition) {
        ASSERT_TRUE(IsAFunction("void Foo() {return;}"));
    }

    TEST(IsFunctionTest, FunctionWithReturnValueDeclaration) {
        ASSERT_TRUE(IsAFunction("int Foo();"));
    }

    TEST(IsFunctionTest, FunctionWithReturnValueDefinition) {
        ASSERT_TRUE(IsAFunction("int Foo() { return 4; }"));
    }

    TEST(IsFunctionTest, FunctionWithArgsOnSeparateLine) {
        ASSERT_TRUE(IsAFunction("void Foo("));
    }

    TEST(IsFunctionTest, FunctionWithReferenceReturn) {
        ASSERT_TRUE(IsAFunction("int& Foo()"));
    }

    TEST(IsFunctionTest, FunctionWithPointerReturn) {
        ASSERT_TRUE(IsAFunction("int* Foo()"));
    }

    TEST(IsFunctionTest, FunctionWithNamespaceReturn) {
        ASSERT_TRUE(IsAFunction("Bar::Baz Foo()"));
    }

    TEST(IsFunctionTest, FunctionWithGlobalQualifiedTypeReturn) {
        ASSERT_TRUE(IsAFunction("::Bar Foo()"));
    }

    TEST(IsFunctionTest, FunctionWithTemplateReturn) {
        ASSERT_TRUE(IsAFunction("std::vector<int> Foo()"));
    }

    TEST(IsFunctionTest, FunctionWithArrayReturn) {
        ASSERT_TRUE(IsAFunction("int[] Foo()"));
    }

    TEST(IsFunctionTest, FunctionWithMultiDimensionalArrayReturn) {
        ASSERT_TRUE(IsAFunction("int[][] Foo()"));
    }

    TEST(IsFunctionTest, FunctionWithPointerInReturnType) {
        ASSERT_TRUE(IsAFunction("std::vector<int*> Foo()"));
    }

    TEST(IsFunctionTest, FunctionWithConstReturnValue) {
        ASSERT_TRUE(IsAFunction("const int& Foo()"));
    }

    TEST(IsFunctionTest, FunctionWithPointerToConst) {
        ASSERT_TRUE(IsAFunction("int const * Foo()"));
    }

    TEST(IsFunctionTest, FunctionWithConstPointerReturnValue) {
        ASSERT_TRUE(IsAFunction("int* const Foo()"));
    }

    TEST(IsFunctionTest, StaticFunction) {
        ASSERT_TRUE(IsAFunction("static void Foo()"));
    }

    TEST(IsFunctionTest, VirtualFunction) {
        ASSERT_TRUE(IsAFunction("virtual void Foo()"));
    }

    TEST(IsFunctionTest, AbstractFunction) {
        ASSERT_TRUE(IsAFunction("virtual void Foo() = 0;"));
    }

    TEST(IsFunctionTest, OverrideFunction) {
        ASSERT_TRUE(IsAFunction("void Foo() override"));
    }

    TEST(IsFunctionTest, FinalFunction) {
        ASSERT_TRUE(IsAFunction("void Foo() final"));
    }

    TEST(IsFunctionTest, SingleArgFunction) {
        ASSERT_TRUE(IsAFunction("void Foo(int a)"));
    }

    TEST(IsFunctionTest, SingleQualifiedArgFunction) {
        ASSERT_TRUE(IsAFunction("void Foo(std::string a)"));
    }

    TEST(IsFunctionTest, SingleTemplatedArgFunction) {
        ASSERT_TRUE(IsAFunction("void Foo(std::vector<int> a"));
    }

    TEST(IsFunctionTest, SingleMultiTemplateArgFunction) {
        ASSERT_TRUE(IsAFunction("void Foo(std::unordered_map<int, int> a)"));
    }

    TEST(IsFunctionTest, SinglePointerArgFunction) {
        ASSERT_TRUE(IsAFunction("void Foo(int* a)"));
    }

    TEST(IsFunctionTest, SingleReferenceArgFunction) {
        ASSERT_TRUE(IsAFunction("void Foo(int& a)"));
    }

    TEST(IsFunctionTest, SingleArrayArgFunction) {
        ASSERT_TRUE(IsAFunction("void Foo(int[] a)"));
    }

    TEST(IsFunctionTest, MultipleArgFunction) {
        ASSERT_TRUE(IsAFunction("void Foo(int a, int b"));
    }

    TEST(IsFunctionTest, DefaultParameterFunction) {
        ASSERT_TRUE(IsAFunction("void Foo(int a = 3)"));
    }

    TEST(IsFunctionTest, ArgsWithParensFunction) {
        ASSERT_TRUE(IsAFunction("void Foo(int a = Bar())"));
    }

    TEST(IsFunctionTest, TemplatedFunction) {
        ASSERT_TRUE(IsAFunction("template<class T> void Foo()"));
    }

    TEST(IsFunctionTest, TemplateSpecializationFunction) {
        ASSERT_TRUE(IsAFunction("template<> void Foo()"));
    }

    TEST(IsFunctionTest, TemplateDefinition) {
        ASSERT_TRUE(IsAFunction("template <class T> void Foo<T>::Bar()"));
    }

    TEST(IsFunctionTest, InlineFunction) {
        ASSERT_TRUE(IsAFunction("inline void Foo()"));
    }

    TEST(IsFunctionTest, UnderscoreInlineFunction) {
        ASSERT_TRUE(IsAFunction("__inline void Foo()"));
    }

    TEST(IsFunctionTest, ForceInlineFunction) {
        ASSERT_TRUE(IsAFunction("__forceinline void Foo()"));
    }

    TEST(IsFunctionTest, StaticInlineFunction) {
        ASSERT_TRUE(IsAFunction("static inline void Foo()"));
    }

    TEST(IsFunctionTest, InlineStaticFunction) {
        ASSERT_TRUE(IsAFunction("inline static void Foo()"));
    }

    TEST(IsFunctionTest, VirtualInlineFunction) {
        ASSERT_TRUE(IsAFunction("virtual inline void Foo()"));
    }

    TEST(IsFunctionTest, InlineVirtualFunction) {
        ASSERT_TRUE(IsAFunction("inline virtual void Foo()"));
    }

    TEST(IsFunctionTest, ExportedFunction) {
        ASSERT_TRUE(IsAFunction("__declspec(dllexport) void Foo()"));
    }

    TEST(IsFunctionTest, FunctionExportedByMacro) {
        ASSERT_TRUE(IsAFunction("DLL_EXPORT void Foo()"));
    }

    TEST(IsFunctionTest, FunctionWithAttribute) {
        ASSERT_TRUE(IsAFunction("[[nodiscard]] int foo()"));
    }

    TEST(IsFunctionTest, ComplexFunction) {
        // The following isn't a valid definition, but exercises about every available option
        ASSERT_TRUE(IsAFunction("template<class T, class U> virtual static const Foo::Bar<Baz>& One::Two(const std::pair<int, int>& arg1, int arg2 = 3 "
                                "const final override = 0"));
    }

    TEST(IsNotAFunctionTest, Statement) {
        ASSERT_FALSE(IsAFunction("int i =0;"));
    }

    TEST(IsNotAFunctionTest, VariableDeclaration) {
        ASSERT_FALSE(IsAFunction("int i;"));
    }

    TEST(IsNotAFunctionTest, FunctionCall) {
        ASSERT_FALSE(IsAFunction("int i = b();"));
    }

    TEST(IsNotAFunctionTest, ConstructorInitializer) {
        ASSERT_FALSE(IsAFunction(": m_Files(files)"));
    }

    class ExtractFunctionTest : public ::testing::Test {
    protected:
        FunctionData Run(std::string line) {
            return FunctionDataExtractor::ExtractFunction(line, ss, m_Namespace, m_ClassName, m_Visibility);
        }

        std::stringstream ss;
        std::string m_ClassName{"className"};
        std::string m_Namespace{"Testing"};
        Visibility m_Visibility{Visibility::PUBLIC};
    };

    TEST_F(ExtractFunctionTest, ExtractsFunctionName) {
        auto result = Run("void Foo()");
        ASSERT_EQ(result.FunctionName, "Foo");
    }

    TEST_F(ExtractFunctionTest, SetsTheClassName) {
        auto result = Run("void Foo()");
        ASSERT_EQ(result.ClassName, m_ClassName);
    }

    TEST_F(ExtractFunctionTest, SetsTheNamespace) {
        auto result = Run("void Foo()");
        ASSERT_EQ(result.Namespace, m_Namespace);
    }

    TEST_F(ExtractFunctionTest, SetsTheVisibility) {
        auto result = Run("void Foo()");
        ASSERT_EQ(result.Visibility, m_Visibility);
    }

    TEST_F(ExtractFunctionTest, StaticFunctionIsStatic) {
        auto result = Run("static void Foo()");
        ASSERT_TRUE(result.IsStatic);
    }

    TEST_F(ExtractFunctionTest, NonStaticFunctionIsNotStatic) {
        auto result = Run("void Foo()");
        ASSERT_FALSE(result.IsStatic);
    }

    TEST_F(ExtractFunctionTest, ConstFunctionIsConst) {
        auto result = Run("void Foo() const;");
        ASSERT_TRUE(result.IsConst);
    }

    TEST_F(ExtractFunctionTest, NonConstFunctionIsNotConst) {
        auto result = Run("void Foo();");
        ASSERT_FALSE(result.IsConst);
    }

    TEST_F(ExtractFunctionTest, VirtualFunctionIsVirtual) {
        auto result = Run("virtual void Foo()");
        ASSERT_TRUE(result.IsVirtual);
    }

    TEST_F(ExtractFunctionTest, NonVirtualFunctionIsNotVirtual) {
        auto result = Run("void Foo()");
        ASSERT_FALSE(result.IsVirtual);
    }

    TEST_F(ExtractFunctionTest, TemplatedFunctionIsTemplated) {
        auto result = Run("template<class T> void Foo()");
        ASSERT_TRUE(result.IsTemplated);
    }

    TEST_F(ExtractFunctionTest, NonTemplatedFunctionIsNotTemplated) {
        auto result = Run("void Foo()");
        ASSERT_FALSE(result.IsTemplated);
    }

    TEST_F(ExtractFunctionTest, AbstractFunctionIsAbstract) {
        auto result = Run("void Foo() = 0;");
        ASSERT_TRUE(result.IsAbstract);
    }

    TEST_F(ExtractFunctionTest, NonAbstractFunctionIsNotAbstract) {
        auto result = Run("void Foo();");
        ASSERT_FALSE(result.IsAbstract);
    }

    TEST_F(ExtractFunctionTest, InlineFunctionIsInline) {
        auto result = Run("inline void Foo();");
        ASSERT_TRUE(result.IsInline);
    }

    TEST_F(ExtractFunctionTest, NonInlineFunctionIsNotInline) {
        auto result = Run("void Foo();");
        ASSERT_FALSE(result.IsInline);
    }

    TEST_F(ExtractFunctionTest, ForceInlineFunctionIsInline) {
        auto result = Run("__forceinline void Foo();");
        ASSERT_TRUE(result.IsInline);
    }

    TEST_F(ExtractFunctionTest, CanExtractMultipleProperties) {
        auto result = Run("template<class T> virtual inline static void Foo() const final override = 0;");
        ASSERT_TRUE(result.IsTemplated);
        ASSERT_TRUE(result.IsVirtual);
        ASSERT_TRUE(result.IsStatic);
        ASSERT_TRUE(result.IsConst);
        ASSERT_TRUE(result.IsInline);
        ASSERT_TRUE(result.IsAbstract);
    }

    TEST_F(ExtractFunctionTest, CanExtractMultiplePropertiesInAnotherOrder) {
        auto result = Run("template<class T> static virtual inline void Foo() final override const = 0;");
        ASSERT_TRUE(result.IsTemplated);
        ASSERT_TRUE(result.IsVirtual);
        ASSERT_TRUE(result.IsStatic);
        ASSERT_TRUE(result.IsConst);
        ASSERT_TRUE(result.IsInline);
        ASSERT_TRUE(result.IsAbstract);
    }

    TEST_F(ExtractFunctionTest, NoArgsHasZeroAirity) {
        auto result = Run("void Foo()");
        ASSERT_EQ(result.Airity, 0);
    }

    TEST_F(ExtractFunctionTest, SingleArgHasOneAirity) {
        auto result = Run("void Foo(int a);");
        ASSERT_EQ(result.Airity, 1);
    }

    TEST_F(ExtractFunctionTest, TwoArgsHasTwoAirity) {
        auto result = Run("void Foo(int a, int b);");
        ASSERT_EQ(result.Airity, 2);
    }

    TEST_F(ExtractFunctionTest, SingleTemplatedArgHasOneAirity) {
        auto result = Run("void Foo(std::pair<int, int> a);");
        ASSERT_EQ(result.Airity, 1);
    }

    TEST_F(ExtractFunctionTest, NoDefaultParametersHasZeroDefaultParameterCount) {
        auto result = Run("void Foo();");
        ASSERT_EQ(result.DefaultParameterCount, 0);
    }

    TEST_F(ExtractFunctionTest, OneDefaultParameterHasOneDefaultParameterCount) {
        auto result = Run("void Foo(int i = 0);");
        ASSERT_EQ(result.DefaultParameterCount, 1);
    }

    TEST_F(ExtractFunctionTest, ThreeDefaultParametersHasThreeDefaultParameterCount) {
        auto result = Run("void Foo(int a = 0, std::string b = \"hello\", float c = 1.0f);");
        ASSERT_EQ(result.DefaultParameterCount, 3);
    }

    TEST_F(ExtractFunctionTest, RemovesFunctionBodyFromStream) {
        ss << R"({
                return true;
            })";

        Run("void Foo()");
        std::string line;
        ASSERT_FALSE(std::getline(ss, line));
    }

    TEST_F(ExtractFunctionTest, RemovesFunctionBodyFromStreamWithEgyptionBraces) {
        ss << R"(
                return true;
            })";

        Run("void Foo() {");
        std::string line;
        ASSERT_FALSE(std::getline(ss, line));
    }

    TEST_F(ExtractFunctionTest, DoesNotAlterOriginalStream) {
        ss << R"({
                return true;
            })";
        std::string original = ss.str();
        Run("void Foo()");
        ASSERT_EQ(ss.str(), original);
    }

    TEST_F(ExtractFunctionTest, DoesNotRemoveLinesIfDefinitionIsOnASingleLine) {
        ss << "Testing";
        Run("void Foo() {}");

        std::string line;
        ASSERT_TRUE(std::getline(ss, line));
        ASSERT_EQ(line, "Testing");
    }

    TEST_F(ExtractFunctionTest, CombinesMultipleParameterLines) {
        ss << R"(
                int a,
                int b
                )
                {
                    return true;
                })";

        auto result = Run("bool Foo(");
        std::string line;
        ASSERT_FALSE(std::getline(ss, line));
        ASSERT_EQ(result.Airity, 2);
    }

    TEST_F(ExtractFunctionTest, RemovesCommentsFromParameters) {
        auto result = Run("void Foo(int a /*1, 3, or 5*/, int b /*2, 4, or 6*/);");
        ASSERT_EQ(result.Airity, 2);
    }

    TEST_F(ExtractFunctionTest, CountsNonBlankLinesInBody) {
        ss << R"({
            int a = 0;
            
            a++;
            return true;
            }
        )";
        auto result = Run("bool Foo()");

        ASSERT_EQ(result.LineCount, 6);
    }

    TEST_F(ExtractFunctionTest, AtLeastOneNonBlankLine) {
        auto result = Run("bool Foo();");

        ASSERT_EQ(result.LineCount, 1);
    }
} // namespace Extractor
