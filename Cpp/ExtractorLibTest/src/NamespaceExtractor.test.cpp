#include "Extractor/NamespaceExtractor.h"

#include "TestCommon.h"
#include "Utilities/StringUtilities.h"

namespace Extractor {

    class IsNamespaceTest : public ::testing::Test {
    public:
    protected:
        bool Run(std::string line) const {
            return m_Extractor.IsNamespace(line);
        }
        NamespaceExtractor m_Extractor{};
    };

    TEST_F(IsNamespaceTest, NamedNamespace) {
        ASSERT_TRUE(Run("namespace Extractor"));
    }

    TEST_F(IsNamespaceTest, NamedNamespaceWithCurly) {
        ASSERT_TRUE(Run("namespace Extractor {"));
    }

    TEST_F(IsNamespaceTest, AnonymousNamespace) {
        ASSERT_TRUE(Run("namespace"));
    }

    TEST_F(IsNamespaceTest, AnonymousNamespaceWithCurly) {
        ASSERT_TRUE(Run("namespace {"));
    }

    TEST_F(IsNamespaceTest, UsingNamespaceIsNotNamespace) {
        ASSERT_FALSE(Run("using namespace std"));
    }

    class GetNamespaceTest : public ::testing::Test {
    protected:
        NamespaceExtractor m_Extractor{};
    };

    TEST_F(GetNamespaceTest, EmptyNamespaceIsEmpty) {
        ASSERT_EQ(m_Extractor.GetNamespace(), "");
    }

    TEST_F(GetNamespaceTest, SingleNamespaceHasNoSeparator) {
        std::string ns = "namespace One";
        m_Extractor.ExtractNamespace(ns);
        ASSERT_EQ(m_Extractor.GetNamespace(), "One");
    }

    TEST_F(GetNamespaceTest, MultipleNamespaceHasSeparator) {
        std::string ns1 = "namespace One";
        std::string ns2 = "namespace Two";
        m_Extractor.ExtractNamespace(ns1);
        m_Extractor.ExtractNamespace(ns2);

        ASSERT_EQ(m_Extractor.GetNamespace(), "One" + m_Extractor.Separator + "Two");
    }

    TEST_F(GetNamespaceTest, PopNamespaceRemovesMostRecent) {
        std::string ns1 = "namespace One";
        std::string ns2 = "namespace Two";
        m_Extractor.ExtractNamespace(ns1);
        m_Extractor.ExtractNamespace(ns2);

        m_Extractor.PopNamespace();

        ASSERT_EQ(m_Extractor.GetNamespace(), "One");
    }

    TEST_F(GetNamespaceTest, AnonymousNamespaceShowsAsAnonymous) {
        std::string ns = "namespace";
        m_Extractor.ExtractNamespace(ns);

        ASSERT_EQ(m_Extractor.GetNamespace(), "Anonymous");
    }

    TEST_F(GetNamespaceTest, AnonymousNamespacesCanBeNested) {
        std::string ns1 = "namespace";
        std::string ns2 = "namespace";
        m_Extractor.ExtractNamespace(ns1);
        m_Extractor.ExtractNamespace(ns2);

        ASSERT_EQ(m_Extractor.GetNamespace(), "Anonymous" + m_Extractor.Separator + "Anonymous");
    }

    TEST_F(GetNamespaceTest, MultipleNamespacesCanBeOnSingleLine) {
        std::string ns = "namespace One { namespace Two";
        m_Extractor.ExtractNamespace(ns);

        ASSERT_EQ(m_Extractor.GetNamespace(), "One" + m_Extractor.Separator + "Two");
    }

    TEST_F(GetNamespaceTest, MultipleNamespacesOnSingleLineRemovesThemFromLine) {
        std::string ns = "namespace One { namespace Two { class Foo{};";
        m_Extractor.ExtractNamespace(ns);

        ASSERT_EQ(StrUtil::Trim(ns), "class Foo{};");
    }

    TEST_F(GetNamespaceTest, CanAddNewNamespaceSyntax) {
        std::string ns = "namespace One::Two::Three";
        m_Extractor.ExtractNamespace(ns);

        ASSERT_EQ(m_Extractor.GetNamespace(), "One::Two::Three");
    }

    TEST_F(GetNamespaceTest, CanPopNewNamespaceSyntax) {
        std::string ns = "namespace One::Two::Three";
        m_Extractor.ExtractNamespace(ns);
        m_Extractor.PopNamespace();

        ASSERT_EQ(m_Extractor.GetNamespace(), "");
    }

    TEST_F(GetNamespaceTest, CanPopNestedCurlies) {
        std::string ns = "namespace One {";
        m_Extractor.ExtractNamespace(ns);
        m_Extractor.PushNestedCurly();

        ASSERT_EQ(m_Extractor.GetNamespace(), "One");
        m_Extractor.PopNamespace();
        ASSERT_EQ(m_Extractor.GetNamespace(), "One");
        m_Extractor.PopNamespace();
        ASSERT_EQ(m_Extractor.GetNamespace(), "");
    }
} // namespace Extractor