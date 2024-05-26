#include "TestCommon.h"

#include "Extractor/FunctionDataExtractor.h"

#include "Core/Utilities/StringUtils.h"

namespace Extractor {
    using FunctionDataExtractor::IsOperatorOverload;

    TEST(IsOperatorOverload, AssignmentOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("X& X::operator=(X rhs)", match));
    }
    TEST(IsOperatorOverload, LeftShiftOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("std::ostream& operator<<(std::ostream& os, const T& obj)", match));
    }
    TEST(IsOperatorOverload, RightShiftOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("std::istream& operator>>(std::istream& is, T& obj)", match));
    }
    TEST(IsOperatorOverload, FunctionCallOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("int operator()(const std::string& y)", match));
    }
    TEST(IsOperatorOverload, EqualityOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("inline bool operator==(const X& lhs, const X& rhs)", match));
        ASSERT_TRUE(IsOperatorOverload("bool operator==(const X& rhs) const", match));
    }
    TEST(IsOperatorOverload, InequalityOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("inline bool operator!=(const X& lhs, const X& rhs)", match));
        ASSERT_TRUE(IsOperatorOverload("bool operator!=(const X& rhs) const", match));
    }
    TEST(IsOperatorOverload, LessThanOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("inline bool operator<(const X& lhs, const X& rhs)", match));
        ASSERT_TRUE(IsOperatorOverload("bool operator<(const X& rhs) const", match));
    }
    TEST(IsOperatorOverload, GreaterThanOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("inline bool operator>(const X& lhs, const X& rhs)", match));
        ASSERT_TRUE(IsOperatorOverload("bool operator>(const X& rhs) const", match));
    }
    TEST(IsOperatorOverload, LessOrEqualOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("inline bool operator<=(const X& lhs, const X& rhs)", match));
        ASSERT_TRUE(IsOperatorOverload("bool operator<=(const X& rhs) const", match));
    }
    TEST(IsOperatorOverload, GreaterOrEqualOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("inline bool operator>=(const X& lhs, const X& rhs)", match));
        ASSERT_TRUE(IsOperatorOverload("bool operator>=(const X& rhs) const", match));
    }
    TEST(IsOperatorOverload, OrOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("inline bool operator||(const X& lhs, const X& rhs)", match));
        ASSERT_TRUE(IsOperatorOverload("bool operator||(const X& rhs) const", match));
    }
    TEST(IsOperatorOverload, AndOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("inline bool operator&&(const X& lhs, const X& rhs)", match));
        ASSERT_TRUE(IsOperatorOverload("bool operator&&(const X& rhs) const", match));
    }
    TEST(IsOperatorOverload, PrefixOperators) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("X& operator++()", match));
        ASSERT_TRUE(IsOperatorOverload("X& operator--()", match));
    }
    TEST(IsOperatorOverload, PostfixOperators) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("X& operator++(int)", match));
        ASSERT_TRUE(IsOperatorOverload("X& operator--(int)", match));
    }
    TEST(IsOperatorOverload, UnaryPrefixOperators) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("X& operator-()", match));
        ASSERT_TRUE(IsOperatorOverload("inline X operator-(X& x)", match));
        ASSERT_TRUE(IsOperatorOverload("X& operator+()", match));
        ASSERT_TRUE(IsOperatorOverload("inline X operator+(X& x)", match));
        ASSERT_TRUE(IsOperatorOverload("X& operator!()", match));
        ASSERT_TRUE(IsOperatorOverload("inline X operator!(X& x)", match));
        ASSERT_TRUE(IsOperatorOverload("X& operator~()", match));
        ASSERT_TRUE(IsOperatorOverload("inline X operator~(X& x)", match));
        ASSERT_TRUE(IsOperatorOverload("X& operator|()", match));
        ASSERT_TRUE(IsOperatorOverload("inline X operator|(X& x)", match));
    }

    TEST(IsOperatorOverload, ThreeCharacterOperators) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("X& operator<<=(X& rhs)", match));
        ASSERT_TRUE(IsOperatorOverload("inline X& operator<<=(X& lhs, const X& rhs)", match));
    }

    TEST(IsOperatorOverload, ArraySubscriptOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("X& operator[](int index)", match));
        ASSERT_TRUE(IsOperatorOverload("X operator[](int index) const", match));
    }

    TEST(IsOperatorOverload, PointerLikeOperators) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("X& operator*()", match));
        ASSERT_TRUE(IsOperatorOverload("const X& operator*() const", match));
        ASSERT_TRUE(IsOperatorOverload("X* operator->();", match));
        ASSERT_TRUE(IsOperatorOverload("const X* operator->() const;", match));
    }

    TEST(IsOperatorOverload, NewOperators) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("void* operator new();", match));
        ASSERT_TRUE(IsOperatorOverload("void* operator new(std::size_t);", match));
        ASSERT_TRUE(IsOperatorOverload("void* operator new(std::size_t, void*p);", match));
        ASSERT_TRUE(IsOperatorOverload("void * operator new[](std::size_t, void* p);", match));
    }

    TEST(IsOperatorOverload, DeleteOperators) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("void operator delete(void* p, void*)", match));
        ASSERT_TRUE(IsOperatorOverload("void operator delete[](void* p, void*);", match));
    }

    TEST(IsOperatorOverload, ExplicitOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("explicit operator const char*() const", match));
        ASSERT_TRUE(IsOperatorOverload("explicit operator bool() const", match));
    }

    TEST(IsOperatorOverload, FriendOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("friend bool operator<(const X& rhs);", match));
    }

    TEST(IsOperatorOverload, SpaceBeforeOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("bool operator <(const X& rhs);", match));
    }

    TEST(IsOperatorOverload, SpaceAfterOperator) {
        std::smatch match;
        ASSERT_TRUE(IsOperatorOverload("bool operator< (const X& rhs);", match));
    }

    TEST(IsNotOperatorOverload, ClassOperator) {
        std::smatch match;
        ASSERT_FALSE(IsOperatorOverload("class operator {", match));
    }

    class ExtractOperatorOverloadTest : public ::testing::Test {
    protected:
        OperatorOverloadData Extract() {
            std::string line;
            std::getline(ss, line);
            std::smatch match;
            IsOperatorOverload(line, match);
            return FunctionDataExtractor::ExtractOperatorOverload(line, match, ss, m_Namespace, m_ClassName, m_Visibility);
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

    TEST_F(ExtractOperatorOverloadTest, ExtractsTheOperatorWithArguments) {
        ss << "X& operator+(const X& rhs) const";
        auto result = Extract();
        ASSERT_EQ(result.Operator, "+");
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