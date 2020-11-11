#include "IncludeCounter/Extractors/FunctionDataExtractor.h"

#include "TestCommon.h"

namespace IncludeCounter {
    namespace Extractors {
        class IsFunctionTest : public ::testing::Test {
        protected:
            bool Run() {
                return FunctionDataExtractor::IsAFunction(m_Line);
            }

            std::string m_Line;
        };

        TEST_F(IsFunctionTest, FunctionsReturnTrue) {
            m_Line = "template<class T> virtual static const Foo::Bar& Baz(int a, int b, int c = 1) const final override = 0;";
            ASSERT_TRUE(Run());
        }

        class ExtractFunctionTest : public ::testing::Test {
        protected:
            FunctionData Run(std::string line) {
                return FunctionDataExtractor::Extract(line, ss, m_ClassName, m_Visibility);
            }

            std::stringstream ss;
            std::string m_ClassName{"className"};
            Visibility m_Visibility{Visibility::PUBLIC};
        };

        TEST_F(ExtractFunctionTest, ExtractsStuff) {
            auto result = Run("template<class T> virtual static const Foo::Bar& Baz(int a, int b, int c = 1) const final override = 0;");
            ASSERT_TRUE(result.IsVirtual);
        }
    } // namespace Extractors
} // namespace IncludeCounter
