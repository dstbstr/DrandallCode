#include "TestCommon.h"

#include "Extractor/FunctionDataExtractor.h"
#include "Extractor/Private/LineFetcher.h"

#include "Core/Utilities/StringUtils.h"

namespace Extractor {
    using FunctionDataExtractor::IsAFunction;

    TEST(IsFunctionTest, SimpleFunctionDeclaration) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("void Foo();", match));
    }

    TEST(IsFunctionTest, SimpleFunctionDefinition) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("void Foo() {return;}", match));
    }

    TEST(IsFunctionTest, FunctionWithReturnValueDeclaration) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("int Foo();", match));
    }

    TEST(IsFunctionTest, FunctionWithReturnValueDefinition) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("int Foo() { return 4; }", match));
    }

    TEST(IsFunctionTest, FunctionWithReferenceReturn) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("int& Foo()", match));
    }

    TEST(IsFunctionTest, FunctionWithPointerReturn) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("int* Foo()", match));
    }

    TEST(IsFunctionTest, FunctionWithNamespaceReturn) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("Bar::Baz Foo()", match));
    }

    TEST(IsFunctionTest, FunctionWithGlobalQualifiedTypeReturn) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("::Bar Foo()", match));
    }

    TEST(IsFunctionTest, FunctionWithTemplateReturn) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("std::vector<int> Foo()", match));
    }

    TEST(IsFunctionTest, FunctionWithArrayReturn) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("int[] Foo()", match));
    }

    TEST(IsFunctionTest, FunctionWithMultiDimensionalArrayReturn) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("int[][] Foo()", match));
    }

    TEST(IsFunctionTest, FunctionWithPointerInReturnType) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("std::vector<int*> Foo()", match));
    }

    TEST(IsFunctionTest, FunctionWithConstReturnValue) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("const int& Foo()", match));
    }

    TEST(IsFunctionTest, FunctionWithPointerToConst) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("int const * Foo()", match));
    }

    TEST(IsFunctionTest, FunctionWithConstPointerReturnValue) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("int* const Foo()", match));
    }

    TEST(IsFunctionTest, StaticFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("static void Foo()", match));
    }

    TEST(IsFunctionTest, VirtualFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("virtual void Foo()", match));
    }

    TEST(IsFunctionTest, AbstractFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("virtual void Foo() = 0;", match));
    }

    TEST(IsFunctionTest, OverrideFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("void Foo() override", match));
    }

    TEST(IsFunctionTest, FinalFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("void Foo() final", match));
    }

    TEST(IsFunctionTest, SingleArgFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("void Foo(int a)", match));
    }

    TEST(IsFunctionTest, SingleQualifiedArgFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("void Foo(std::string a)", match));
    }

    TEST(IsFunctionTest, SingleTemplatedArgFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("void Foo(std::vector<int> a)", match));
    }

    TEST(IsFunctionTest, SingleMultiTemplateArgFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("void Foo(std::unordered_map<int, int> a)", match));
    }

    TEST(IsFunctionTest, SinglePointerArgFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("void Foo(int* a)", match));
    }

    TEST(IsFunctionTest, SingleReferenceArgFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("void Foo(int& a)", match));
    }

    TEST(IsFunctionTest, SingleArrayArgFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("void Foo(int[] a)", match));
    }

    TEST(IsFunctionTest, MultipleArgFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("void Foo(int a, int b)", match));
    }

    TEST(IsFunctionTest, DefaultParameterFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("void Foo(int a = 3)", match));
    }

    TEST(IsFunctionTest, ArgsWithParensFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("void Foo(int a = Bar())", match));
    }

    TEST(IsFunctionTest, TemplatedFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("template<class T> void Foo()", match));
    }

    TEST(IsFunctionTest, TemplateSpecializationFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("template<> void Foo()", match));
    }

    TEST(IsFunctionTest, TemplateDefinition) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("template <class T> void Foo<T>::Bar()", match));
    }

    TEST(IsFunctionTest, InlineFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("inline void Foo()", match));
    }

    TEST(IsFunctionTest, UnderscoreInlineFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("__inline void Foo()", match));
    }

    TEST(IsFunctionTest, ForceInlineFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("__forceinline void Foo()", match));
    }

    TEST(IsFunctionTest, StaticInlineFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("static inline void Foo()", match));
    }

    TEST(IsFunctionTest, InlineStaticFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("inline static void Foo()", match));
    }

    TEST(IsFunctionTest, VirtualInlineFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("virtual inline void Foo()", match));
    }

    TEST(IsFunctionTest, InlineVirtualFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("inline virtual void Foo()", match));
    }

    TEST(IsFunctionTest, ExportedFunction) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("__declspec(dllexport) void Foo()", match));
    }

    TEST(IsFunctionTest, FunctionExportedByMacro) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("DLL_EXPORT void Foo()", match));
    }

    TEST(IsFunctionTest, FunctionWithAttribute) {
        std::smatch match;
        ASSERT_TRUE(IsAFunction("[[nodiscard]] int foo()", match));
    }

    TEST(IsFunctionTest, ComplexFunction) {
        // The following isn't a valid definition, but exercises about every available option
        std::smatch match;
        ASSERT_TRUE(IsAFunction("template<class T, class U> virtual static const Foo::Bar<Baz>& One::Two(const std::pair<int, int>& arg1, int arg2 = 3 )"
                                "const final override = 0",
                                match));
    }

    TEST(IsNotAFunctionTest, Statement) {
        std::smatch match;
        ASSERT_FALSE(IsAFunction("int i =0;", match));
    }

    TEST(IsNotAFunctionTest, VariableDeclaration) {
        std::smatch match;
        ASSERT_FALSE(IsAFunction("int i;", match));
    }

    TEST(IsNotAFunctionTest, FunctionCall) {
        std::smatch match;
        ASSERT_FALSE(IsAFunction("int i = b();", match));
    }

    TEST(IsNotAFunctionTest, ConstructorInitializer) {
        std::smatch match;
        ASSERT_FALSE(IsAFunction(": m_Files(files)", match));
    }

    class ExtractFunctionTest : public ::testing::Test {
    protected:
        FunctionData Extract() {
            std::string line;
            LineFetcher::GetNextLine(ss, line);
            IsAFunction(line, match);
            return FunctionDataExtractor::ExtractFunction(line, match, ss, m_Namespace, m_ClassName, m_Visibility);
        }

        std::smatch match;
        std::stringstream ss;
        std::string m_ClassName{"className"};
        std::string m_Namespace{"Testing"};
        Visibility m_Visibility{Visibility::PUBLIC};
    };

    TEST_F(ExtractFunctionTest, ExtractsFunctionName) {
        ss << "void Foo()";
        auto result = Extract();
        ASSERT_EQ(result.FunctionName, "Foo");
    }

    TEST_F(ExtractFunctionTest, SetsTheClassName) {
        ss << "void Foo()";
        auto result = Extract();
        ASSERT_EQ(result.ClassName, m_ClassName);
    }

    TEST_F(ExtractFunctionTest, SetsTheNamespace) {
        ss << "void Foo()";
        auto result = Extract();
        ASSERT_EQ(result.Namespace, m_Namespace);
    }

    TEST_F(ExtractFunctionTest, SetsTheVisibility) {
        ss << "void Foo()";
        auto result = Extract();
        ASSERT_EQ(result.Visibility, m_Visibility);
    }

    TEST_F(ExtractFunctionTest, StaticFunctionIsStatic) {
        ss << "static void Foo()";
        auto result = Extract();
        ASSERT_TRUE(result.IsStatic);
    }

    TEST_F(ExtractFunctionTest, NonStaticFunctionIsNotStatic) {
        ss << "void Foo()";
        auto result = Extract();
        ASSERT_FALSE(result.IsStatic);
    }

    TEST_F(ExtractFunctionTest, ConstFunctionIsConst) {
        ss << "void Foo() const;";
        auto result = Extract();
        ASSERT_TRUE(result.IsConst);
    }

    TEST_F(ExtractFunctionTest, NonConstFunctionIsNotConst) {
        ss << "void Foo();";
        auto result = Extract();
        ASSERT_FALSE(result.IsConst);
    }

    TEST_F(ExtractFunctionTest, VirtualFunctionIsVirtual) {
        ss << "virtual void Foo()";
        auto result = Extract();
        ASSERT_TRUE(result.IsVirtual);
    }

    TEST_F(ExtractFunctionTest, NonVirtualFunctionIsNotVirtual) {
        ss << "void Foo();";
        auto result = Extract();
        ASSERT_FALSE(result.IsVirtual);
    }

    TEST_F(ExtractFunctionTest, TemplatedFunctionIsTemplated) {
        ss << "template<class T> void Foo()";
        auto result = Extract();
        ASSERT_TRUE(result.IsTemplated);
    }

    TEST_F(ExtractFunctionTest, NonTemplatedFunctionIsNotTemplated) {
        ss << "void Foo();";
        auto result = Extract();
        ASSERT_FALSE(result.IsTemplated);
    }

    TEST_F(ExtractFunctionTest, AbstractFunctionIsAbstract) {
        ss << "void Foo() = 0;";
        auto result = Extract();
        ASSERT_TRUE(result.IsAbstract);
    }

    TEST_F(ExtractFunctionTest, NonAbstractFunctionIsNotAbstract) {
        ss << "void Foo();";
        auto result = Extract();
        ASSERT_FALSE(result.IsAbstract);
    }

    TEST_F(ExtractFunctionTest, InlineFunctionIsInline) {
        ss << "inline void Foo();";
        auto result = Extract();
        ASSERT_TRUE(result.IsInline);
    }

    TEST_F(ExtractFunctionTest, NonInlineFunctionIsNotInline) {
        ss << "void Foo();";
        auto result = Extract();
        ASSERT_FALSE(result.IsInline);
    }

    TEST_F(ExtractFunctionTest, ForceInlineFunctionIsInline) {
        ss << "__forceinline void Foo();";
        auto result = Extract();
        ASSERT_TRUE(result.IsInline);
    }

    TEST_F(ExtractFunctionTest, CanExtractMultipleProperties) {
        ss << "template<class T> virtual inline static void Foo() const final override = 0;";
        auto result = Extract();
        ASSERT_TRUE(result.IsTemplated);
        ASSERT_TRUE(result.IsVirtual);
        ASSERT_TRUE(result.IsStatic);
        ASSERT_TRUE(result.IsConst);
        ASSERT_TRUE(result.IsInline);
        ASSERT_TRUE(result.IsAbstract);
    }

    TEST_F(ExtractFunctionTest, CanExtractMultiplePropertiesInAnotherOrder) {
        ss << "template<class T> static virtual inline void Foo() final override const = 0;";
        auto result = Extract();

        ASSERT_TRUE(result.IsTemplated);
        ASSERT_TRUE(result.IsVirtual);
        ASSERT_TRUE(result.IsStatic);
        ASSERT_TRUE(result.IsConst);
        ASSERT_TRUE(result.IsInline);
        ASSERT_TRUE(result.IsAbstract);
    }

    TEST_F(ExtractFunctionTest, NoArgsHasZeroAirity) {
        ss << "void Foo();";
        auto result = Extract();
        ASSERT_EQ(result.Airity, 0);
    }

    TEST_F(ExtractFunctionTest, SingleArgHasOneAirity) {
        ss << "void Foo(int a);";
        auto result = Extract();
        ASSERT_EQ(result.Airity, 1);
    }

    TEST_F(ExtractFunctionTest, TwoArgsHasTwoAirity) {
        ss << "void Foo(int a, int b);";
        auto result = Extract();
        ASSERT_EQ(result.Airity, 2);
    }

    TEST_F(ExtractFunctionTest, SingleTemplatedArgHasOneAirity) {
        ss << "void Foo(std::pair<int, int> a);";
        auto result = Extract();
        ASSERT_EQ(result.Airity, 1);
    }

    TEST_F(ExtractFunctionTest, NoDefaultParametersHasZeroDefaultParameterCount) {
        ss << "void Foo();";
        auto result = Extract();
        ASSERT_EQ(result.DefaultParameterCount, 0);
    }

    TEST_F(ExtractFunctionTest, OneDefaultParameterHasOneDefaultParameterCount) {
        ss << "void Foo(int i = 0);";
        auto result = Extract();
        ASSERT_EQ(result.DefaultParameterCount, 1);
    }

    TEST_F(ExtractFunctionTest, ThreeDefaultParametersHasThreeDefaultParameterCount) {
        ss << "void Foo(int a = 0, std::string b = \"hello\", float c = 1.0f);";
        auto result = Extract();
        ASSERT_EQ(result.DefaultParameterCount, 3);
    }

    TEST_F(ExtractFunctionTest, RemovesFunctionBodyFromStream) {
        ss << R"(void Foo()
            {
                return true;
            })";

        Extract();
        std::string line;
        ASSERT_FALSE(std::getline(ss, line));
    }

    TEST_F(ExtractFunctionTest, RemovesFunctionBodyFromStreamWithEgyptionBraces) {
        ss << R"(void Foo() {
                return true;
            })";

        Extract();
        std::string line;
        ASSERT_FALSE(std::getline(ss, line));
    }

    TEST_F(ExtractFunctionTest, DoesNotAlterOriginalStream) {
        ss << R"(void Foo()
            {
                return true;
            })";
        std::string original = ss.str();
        Extract();
        ASSERT_EQ(ss.str(), original);
    }

    TEST_F(ExtractFunctionTest, DoesNotRemoveLinesIfDefinitionIsOnASingleLine) {
        ss << R"(void Foo() {}
        Testing)";
        Extract();

        std::string line;
        ASSERT_TRUE(std::getline(ss, line));
        ASSERT_EQ(StrUtil::Trim(line), "Testing");
    }

    TEST_F(ExtractFunctionTest, CombinesMultipleParameterLines) {
        ss << R"(bool Foo(
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

    TEST_F(ExtractFunctionTest, RemovesCommentsFromParameters) {
        ss << "void Foo(int a /*1, 3, or 5*/, int b /*2, 4, or 6*/);";
        auto result = Extract();
        ASSERT_EQ(result.Airity, 2);
    }

    TEST_F(ExtractFunctionTest, CountsNonBlankLinesInBody) {
        ss << R"(bool Foo()
            {
            int a = 0;
            
            a++;
            return true;
            }
        )";
        auto result = Extract();

        ASSERT_EQ(result.LineCount, 5);
    }

    TEST_F(ExtractFunctionTest, AtLeastOneNonBlankLine) {
        ss << "bool Foo();";
        auto result = Extract();

        ASSERT_EQ(result.LineCount, 1);
    }
} // namespace Extractor
