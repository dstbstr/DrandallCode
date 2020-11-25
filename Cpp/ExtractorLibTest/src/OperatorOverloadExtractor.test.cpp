#include "Extractor/FunctionDataExtractor.h"
#include "TestCommon.h"
#include "Utilities/StringUtils.h"

namespace Extractor {
    using FunctionDataExtractor::IsOperatorOverload;

    TEST(IsOperatorOverload, AssignmentOperator) {
        ASSERT_TRUE(IsOperatorOverload("X& X::operator=(X rhs)"));
    }
    TEST(IsOperatorOverload, LeftShiftOperator) {
        ASSERT_TRUE(IsOperatorOverload("std::ostream& operator<<(std::ostream& os, const T& obj)"));
    }
    TEST(IsOperatorOverload, RightShiftOperator) {
        ASSERT_TRUE(IsOperatorOverload("std::istream& operator>>(std::istream& is, T& obj)"));
    }
    TEST(IsOperatorOverload, FunctionCallOperator) {
        ASSERT_TRUE(IsOperatorOverload("int operator()(const std::string& y)"));
    }
    TEST(IsOperatorOverload, EqualityOperator) {
        ASSERT_TRUE(IsOperatorOverload("inline bool operator==(const X& lhs, const X& rhs)"));
        ASSERT_TRUE(IsOperatorOverload("bool operator==(const X& rhs) const"));
    }
    TEST(IsOperatorOverload, InequalityOperator) {
        ASSERT_TRUE(IsOperatorOverload("inline bool operator!=(const X& lhs, const X& rhs)"));
        ASSERT_TRUE(IsOperatorOverload("bool operator!=(const X& rhs) const"));
    }
    TEST(IsOperatorOverload, LessThanOperator) {
        ASSERT_TRUE(IsOperatorOverload("inline bool operator<(const X& lhs, const X& rhs)"));
        ASSERT_TRUE(IsOperatorOverload("bool operator<(const X& rhs) const"));
    }
    TEST(IsOperatorOverload, GreaterThanOperator) {
        ASSERT_TRUE(IsOperatorOverload("inline bool operator>(const X& lhs, const X& rhs)"));
        ASSERT_TRUE(IsOperatorOverload("bool operator>(const X& rhs) const"));
    }
    TEST(IsOperatorOverload, LessOrEqualOperator) {
        ASSERT_TRUE(IsOperatorOverload("inline bool operator<=(const X& lhs, const X& rhs)"));
        ASSERT_TRUE(IsOperatorOverload("bool operator<=(const X& rhs) const"));
    }
    TEST(IsOperatorOverload, GreaterOrEqualOperator) {
        ASSERT_TRUE(IsOperatorOverload("inline bool operator>=(const X& lhs, const X& rhs)"));
        ASSERT_TRUE(IsOperatorOverload("bool operator>=(const X& rhs) const"));
    }
    TEST(IsOperatorOverload, OrOperator) {
        ASSERT_TRUE(IsOperatorOverload("inline bool operator||(const X& lhs, const X& rhs)"));
        ASSERT_TRUE(IsOperatorOverload("bool operator||(const X& rhs) const"));
    }
    TEST(IsOperatorOverload, AndOperator) {
        ASSERT_TRUE(IsOperatorOverload("inline bool operator&&(const X& lhs, const X& rhs)"));
        ASSERT_TRUE(IsOperatorOverload("bool operator&&(const X& rhs) const"));
    }
    TEST(IsOperatorOverload, PrefixOperators) {
        ASSERT_TRUE(IsOperatorOverload("X& operator++()"));
        ASSERT_TRUE(IsOperatorOverload("X& operator--()"));
    }
    TEST(IsOperatorOverload, PostfixOperators) {
        ASSERT_TRUE(IsOperatorOverload("X& operator++(int)"));
        ASSERT_TRUE(IsOperatorOverload("X& operator--(int)"));
    }
    TEST(IsOperatorOverload, UnaryPrefixOperators) {
        ASSERT_TRUE(IsOperatorOverload("X& operator-()"));
        ASSERT_TRUE(IsOperatorOverload("inline X operator-(X& x)"));
        ASSERT_TRUE(IsOperatorOverload("X& operator+()"));
        ASSERT_TRUE(IsOperatorOverload("inline X operator+(X& x)"));
        ASSERT_TRUE(IsOperatorOverload("X& operator!()"));
        ASSERT_TRUE(IsOperatorOverload("inline X operator!(X& x)"));
        ASSERT_TRUE(IsOperatorOverload("X& operator~()"));
        ASSERT_TRUE(IsOperatorOverload("inline X operator~(X& x)"));
        ASSERT_TRUE(IsOperatorOverload("X& operator|()"));
        ASSERT_TRUE(IsOperatorOverload("inline X operator|(X& x)"));
    }

    TEST(IsOperatorOverload, ThreeCharacterOperators) {
        ASSERT_TRUE(IsOperatorOverload("X& operator<<=(X& rhs)"));
        ASSERT_TRUE(IsOperatorOverload("inline X& operator<<=(X& lhs, const X& rhs)"));
    }

    TEST(IsOperatorOverload, ArraySubscriptOperator) {
        ASSERT_TRUE(IsOperatorOverload("X& operator[](int index)"));
        ASSERT_TRUE(IsOperatorOverload("X operator[](int index) const"));
    }

    TEST(IsOperatorOverload, PointerLikeOperators) {
        ASSERT_TRUE(IsOperatorOverload("X& operator*()"));
        ASSERT_TRUE(IsOperatorOverload("const X& operator*() const"));
        ASSERT_TRUE(IsOperatorOverload("X* operator->();"));
        ASSERT_TRUE(IsOperatorOverload("const X* operator->() const;"));
    }

    TEST(IsOperatorOverload, NewOperators) {
        ASSERT_TRUE(IsOperatorOverload("void* operator new();"));
        ASSERT_TRUE(IsOperatorOverload("void* operator new(std::size_t);"));
        ASSERT_TRUE(IsOperatorOverload("void* operator new(std::size_t, void*p);"));
        ASSERT_TRUE(IsOperatorOverload("void * operator new[](std::size_t, void* p);"));
    }

    TEST(IsOperatorOverload, DeleteOperators) {
        ASSERT_TRUE(IsOperatorOverload("void operator delete(void* p, void*)"));
        ASSERT_TRUE(IsOperatorOverload("void operator delete[](void* p, void*);"));
    }

    TEST(IsOperatorOverload, ExplicitOperator) {
        ASSERT_TRUE(IsOperatorOverload("explicit operator const char*() const"));
        ASSERT_TRUE(IsOperatorOverload("explicit operator bool() const"));
    }

    TEST(IsOperatorOverload, FriendOperator) {
        ASSERT_TRUE(IsOperatorOverload("friend bool operator<(const X& rhs);"));
    }

    TEST(IsOperatorOverload, SpaceBeforeOperator) {
        ASSERT_TRUE(IsOperatorOverload("bool operator <(const X& rhs);"));
    }

    TEST(IsOperatorOverload, SpaceAfterOperator) {
        ASSERT_TRUE(IsOperatorOverload("bool operator< (const X& rhs);"));
    }

    TEST(IsNotOperatorOverload, ClassOperator) {
        ASSERT_FALSE(IsOperatorOverload("class operator {"));
    }

    class ExtractOperatorOverloadTest : public ::testing::Test {
    protected:
        OperatorOverloadData Extract() {
            std::string line;
            std::getline(ss, line);
            return FunctionDataExtractor::ExtractOperatorOverload(line, ss, m_Namespace, m_ClassName, m_Visibility);
        }

        std::stringstream ss;
        std::string m_Namespace{"namespace"};
        std::string m_ClassName{"className"};
        Visibility m_Visibility{Visibility::PUBLIC};
    };

    TEST_F(ExtractOperatorOverloadTest, PopulatesNamespace) {
        ss << "X& operator++()";
        auto result = Extract();
        ASSERT_EQ(result.Namespace, m_Namespace);
    }

    TEST_F(ExtractOperatorOverloadTest, PopulatesClassName) {
        ss << "X& operator++()";
        auto result = Extract();
        ASSERT_EQ(result.ClassName, m_ClassName);
    }

    TEST_F(ExtractOperatorOverloadTest, PopulatesVisibility) {
        ss << "X& operator++()";
        auto result = Extract();
        ASSERT_EQ(result.Visibility, m_Visibility);
    }

    TEST_F(ExtractOperatorOverloadTest, ExplicitOperatorIsExplicit) {
        ss << "explicit operator bool() const";
        auto result = Extract();
        ASSERT_TRUE(result.IsExplicit);
    }

    TEST_F(ExtractOperatorOverloadTest, NonExplicitOperatorIsNotExplicit) {
        ss << "X& operator++()";
        auto result = Extract();
        ASSERT_FALSE(result.IsExplicit);
    }

    TEST_F(ExtractOperatorOverloadTest, FriendOperatorIsFriend) {
        ss << "friend X& operator++()";
        auto result = Extract();
        ASSERT_TRUE(result.IsFriend);
    }

    TEST_F(ExtractOperatorOverloadTest, NonFriendOperatorIsNotFriend) {
        ss << "X& operator++()";
        auto result = Extract();
        ASSERT_FALSE(result.IsFriend);
    }

    TEST_F(ExtractOperatorOverloadTest, InlineOperatorIsInline) {
        ss << "inline X& operator++()";
        auto result = Extract();
        ASSERT_TRUE(result.IsInline);
    }

    TEST_F(ExtractOperatorOverloadTest, NonInlineOperatorIsNotInline) {
        ss << "X& operator++()";
        auto result = Extract();
        ASSERT_FALSE(result.IsInline);
    }

    TEST_F(ExtractOperatorOverloadTest, ExtractsTheOperator) {
        ss << "X& operator++()";
        auto result = Extract();
        ASSERT_EQ(result.Operator, "++");
    }

    TEST_F(ExtractOperatorOverloadTest, ExtractsOperatorCast) {
        ss << "explicit operator bool() const";
        auto result = Extract();
        ASSERT_EQ(result.Operator, "bool");
    }

    TEST_F(ExtractOperatorOverloadTest, ExtractsNewOperator) {
        ss << "void* operator new()";
        auto result = Extract();
        ASSERT_EQ(result.Operator, "new");
    }

    TEST_F(ExtractOperatorOverloadTest, ExtractsNewArrayOperator) {
        ss << "void* operator new[]()";
        auto result = Extract();
        ASSERT_EQ(result.Operator, "new[]");
    }

    TEST_F(ExtractOperatorOverloadTest, ExtractsOperatorWithSpaces) {
        ss << "X& operator ++ ()";
        auto result = Extract();
        ASSERT_EQ(result.Operator, "++");
    }

    TEST_F(ExtractOperatorOverloadTest, ExtractsOperatorCastWithSpaces) {
        ss << "explicit operator const char*() const";
        auto result = Extract();
        ASSERT_EQ(result.Operator, "const char*");
    }

    TEST_F(ExtractOperatorOverloadTest, HasAtLeastOneLine) {
        ss << "X& operator++()";
        auto result = Extract();
        ASSERT_EQ(result.LineCount, 1);
    }

    TEST_F(ExtractOperatorOverloadTest, CountsLinesFromTheBody) {
        ss << R"(X& operator++() {
            ++this.Val;
            })";
        auto result = Extract();
        ASSERT_EQ(result.LineCount, 3);
    }

    TEST_F(ExtractOperatorOverloadTest, RemovesBodyFromStream) {
        ss << R"(X& operator++() {
            ++this.Val;
            })";
        Extract();
        std::string line;
        ASSERT_FALSE(std::getline(ss, line));
    }

    TEST_F(ExtractOperatorOverloadTest, DoesNotRemoveLinesBeyondBody) {
        ss << R"(X& operator++() {
            ++this.Val;
        }
        int i = 0;)";

        Extract();
        std::string line;
        ASSERT_TRUE(std::getline(ss, line));

        ASSERT_EQ(StrUtil::Trim(line), "int i = 0;");
    }

} // namespace Extractor