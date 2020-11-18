#include "Extractor/FunctionDataExtractor.h"

#include "TestCommon.h"
#include "Utilities/StringUtilities.h"

namespace Extractor {
    class IsFunctionTest : public ::testing::Test {
    protected:
        bool Run(std::string line) {
            return FunctionDataExtractor::IsAFunction(line);
        }
    };

    TEST_F(IsFunctionTest, SimpleFunctionDeclaration) {
        ASSERT_TRUE(Run("void Foo();"));
    }

    TEST_F(IsFunctionTest, SimpleFunctionDefinition) {
        ASSERT_TRUE(Run("void Foo() {return;}"));
    }

    TEST_F(IsFunctionTest, FunctionWithReturnValueDeclaration) {
        ASSERT_TRUE(Run("int Foo();"));
    }

    TEST_F(IsFunctionTest, FunctionWithReturnValueDefinition) {
        ASSERT_TRUE(Run("int Foo() { return 4; }"));
    }

    TEST_F(IsFunctionTest, FunctionWithArgsOnSeparateLine) {
        ASSERT_TRUE(Run("void Foo("));
    }

    TEST_F(IsFunctionTest, FunctionWithReferenceReturn) {
        ASSERT_TRUE(Run("int& Foo()"));
    }

    TEST_F(IsFunctionTest, FunctionWithPointerReturn) {
        ASSERT_TRUE(Run("int* Foo()"));
    }

    TEST_F(IsFunctionTest, FunctionWithNamespaceReturn) {
        ASSERT_TRUE(Run("Bar::Baz Foo()"));
    }

    TEST_F(IsFunctionTest, FunctionWithGlobalQualifiedTypeReturn) {
        ASSERT_TRUE(Run("::Bar Foo()"));
    }

    TEST_F(IsFunctionTest, FunctionWithTemplateReturn) {
        ASSERT_TRUE(Run("std::vector<int> Foo()"));
    }

    TEST_F(IsFunctionTest, FunctionWithArrayReturn) {
        ASSERT_TRUE(Run("int[] Foo()"));
    }

    TEST_F(IsFunctionTest, FunctionWithMultiDimensionalArrayReturn) {
        ASSERT_TRUE(Run("int[][] Foo()"));
    }

    TEST_F(IsFunctionTest, FunctionWithPointerInReturnType) {
        ASSERT_TRUE(Run("std::vector<int*> Foo()"));
    }

    TEST_F(IsFunctionTest, FunctionWithConstReturnValue) {
        ASSERT_TRUE(Run("const int& Foo()"));
    }

    TEST_F(IsFunctionTest, StaticFunction) {
        ASSERT_TRUE(Run("static void Foo()"));
    }

    TEST_F(IsFunctionTest, VirtualFunction) {
        ASSERT_TRUE(Run("virtual void Foo()"));
    }

    TEST_F(IsFunctionTest, AbstractFunction) {
        ASSERT_TRUE(Run("virtual void Foo() = 0;"));
    }

    TEST_F(IsFunctionTest, OverrideFunction) {
        ASSERT_TRUE(Run("void Foo() override"));
    }

    TEST_F(IsFunctionTest, FinalFunction) {
        ASSERT_TRUE(Run("void Foo() final"));
    }

    TEST_F(IsFunctionTest, SingleArgFunction) {
        ASSERT_TRUE(Run("void Foo(int a)"));
    }

    TEST_F(IsFunctionTest, SingleQualifiedArgFunction) {
        ASSERT_TRUE(Run("void Foo(std::string a)"));
    }

    TEST_F(IsFunctionTest, SingleTemplatedArgFunction) {
        ASSERT_TRUE(Run("void Foo(std::vector<int> a"));
    }

    TEST_F(IsFunctionTest, SingleMultiTemplateArgFunction) {
        ASSERT_TRUE(Run("void Foo(std::unordered_map<int, int> a)"));
    }

    TEST_F(IsFunctionTest, SinglePointerArgFunction) {
        ASSERT_TRUE(Run("void Foo(int* a)"));
    }

    TEST_F(IsFunctionTest, SingleReferenceArgFunction) {
        ASSERT_TRUE(Run("void Foo(int& a)"));
    }

    TEST_F(IsFunctionTest, SingleArrayArgFunction) {
        ASSERT_TRUE(Run("void Foo(int[] a)"));
    }

    TEST_F(IsFunctionTest, MultipleArgFunction) {
        ASSERT_TRUE(Run("void Foo(int a, int b"));
    }

    TEST_F(IsFunctionTest, DefaultParameterFunction) {
        ASSERT_TRUE(Run("void Foo(int a = 3)"));
    }

    TEST_F(IsFunctionTest, ArgsWithParensFunction) {
        ASSERT_TRUE(Run("void Foo(int a = Bar())"));
    }

    TEST_F(IsFunctionTest, TemplatedFunction) {
        ASSERT_TRUE(Run("template<class T> void Foo()"));
    }

    TEST_F(IsFunctionTest, TemplateSpecializationFunction) {
        ASSERT_TRUE(Run("template<> void Foo()"));
    }

    TEST_F(IsFunctionTest, InlineFunction) {
        ASSERT_TRUE(Run("inline void Foo()"));
    }

    TEST_F(IsFunctionTest, UnderscoreInlineFunction) {
        ASSERT_TRUE(Run("__inline void Foo()"));
    }

    TEST_F(IsFunctionTest, ForceInlineFunction) {
        ASSERT_TRUE(Run("__forceinline void Foo()"));
    }

    TEST_F(IsFunctionTest, StaticInlineFunction) {
        ASSERT_TRUE(Run("static inline void Foo()"));
    }

    TEST_F(IsFunctionTest, InlineStaticFunction) {
        ASSERT_TRUE(Run("inline static void Foo()"));
    }

    TEST_F(IsFunctionTest, VirtualInlineFunction) {
        ASSERT_TRUE(Run("virtual inline void Foo()"));
    }

    TEST_F(IsFunctionTest, InlineVirtualFunction) {
        ASSERT_TRUE(Run("inline virtual void Foo()"));
    }

    TEST_F(IsFunctionTest, ComplexFunction) {
        // The following isn't a valid definition, but exercises about every available option
        ASSERT_TRUE(Run("template<class T, class U> virtual static const Foo::Bar<Baz>& One::Two(const std::pair<int, int>& arg1, int arg2 = 3 "
                        "const final override = 0"));
    }

    TEST_F(IsFunctionTest, StatementIsNotAFunction) {
        ASSERT_FALSE(Run("int i =0;"));
    }

    TEST_F(IsFunctionTest, VariableDeclarationIsNotAFunction) {
        ASSERT_FALSE(Run("int i;"));
    }

    TEST_F(IsFunctionTest, FunctionCallIsNotAFunction) {
        ASSERT_FALSE(Run("int i = b();"));
    }

    TEST_F(IsFunctionTest, ConstructorInitializerIsNotAFunction) {
        ASSERT_FALSE(Run(": m_Files(files)"));
    }

    class ExtractFunctionTest : public ::testing::Test {
    protected:
        FunctionData Run(std::string line) {
            return FunctionDataExtractor::Extract(line, ss, m_Namespace, m_ClassName, m_Visibility);
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

} // namespace Extractor
