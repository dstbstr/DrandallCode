#include "Utilities/Require.h"

#include "TestCommon.h"

#include <vector>

TEST(RequireTrue, DoesNotThrowWithTrue) {
    ASSERT_NO_THROW(Require::True(true));
}

TEST(RequireTrue, DoesNotThrowWithTrueAndMessage) {
    ASSERT_NO_THROW(Require::True(true, "message"));
}

TEST(RequireTrue, ThrowsWithFalse) {
    ASSERT_ANY_THROW(Require::True(false));
}

TEST(RequireTrue, ThrowsWithFalseAndMessage) {
    ASSERT_ANY_THROW(Require::True(false, "message"));
}

TEST(RequireTrue, HasDefaultThrowMessage) {
    CAPTURE_EXCEPTION_E(Require::True(false), std::runtime_error) {
        ASSERT_STRNE(e.what(), "");
    }
}

TEST(RequireTrue, PopulatesExceptionWithProvidedMessage) {
    CAPTURE_EXCEPTION_E(Require::True(false, "test message"), std::runtime_error) {
        ASSERT_STREQ(e.what(), "test message");
    }
}

TEST(RequireTrue, RecognizesNonNullPointerAsTrue) {
    int* foo = new int(4);
    ASSERT_NO_THROW(Require::True(foo));
    delete foo;
}

TEST(RequireTrue, RecognizesNullPointerAsFalse) {
    int* foo = nullptr;
    ASSERT_ANY_THROW(Require::True(foo));
}

TEST(RequireFalse, DoesNotThrowWithFalse) {
    ASSERT_NO_THROW(Require::False(false));
}

TEST(RequireFalse, DoesNotThrowWithFalseAndMessage) {
    ASSERT_NO_THROW(Require::False(false, "message"));
}

TEST(RequireFalse, ThrowsWithTrue) {
    ASSERT_ANY_THROW(Require::False(true));
}

TEST(RequireFalse, ThrowsWithTrueAndMessage) {
    ASSERT_ANY_THROW(Require::False(true, "message"));
}

TEST(RequireFalse, HasDefaultThrowMessage) {
    CAPTURE_EXCEPTION_E(Require::False(true), std::runtime_error) {
        ASSERT_STRNE(e.what(), "");
    }
}

TEST(RequireFalse, PopulatesExceptionWithProvidedMessage) {
    CAPTURE_EXCEPTION_E(Require::False(true, "test message"), std::runtime_error) {
        ASSERT_STREQ(e.what(), "test message");
    }
}

TEST(RequireFalse, RecognizesNonNullPointerAsTrue) {
    int* foo = new int(4);
    ASSERT_ANY_THROW(Require::False(foo));
    delete foo;
}

TEST(RequireFalse, RecognizesNullPointerAsFalse) {
    int* foo = nullptr;
    ASSERT_NO_THROW(Require::False(foo));
}

class RequireNullTest : public ::testing::Test {
protected:
    int* m_Null{nullptr};
    int* m_NonNull;

    void SetUp() override {
        m_NonNull = new int(4);
    }

    void TearDown() override {
        delete m_NonNull;
    }
};

TEST_F(RequireNullTest, DoesNotThrowWithNull) {
    ASSERT_NO_THROW(Require::Null(m_Null));
}

TEST_F(RequireNullTest, DoesNotThrowWithNullAndMessage) {
    ASSERT_NO_THROW(Require::Null(m_Null, "message"));
}

TEST_F(RequireNullTest, ThrowsWithNonNull) {
    ASSERT_ANY_THROW(Require::Null(m_NonNull));
}

TEST_F(RequireNullTest, ThrowsWithNonNullAndMessage) {
    ASSERT_ANY_THROW(Require::Null(m_NonNull, "message"));
}

TEST_F(RequireNullTest, HasDefaultThrowMessage) {
    CAPTURE_EXCEPTION_E(Require::Null(m_NonNull), std::runtime_error) {
        ASSERT_STRNE(e.what(), "");
    }
}

TEST_F(RequireNullTest, PopulatesExceptionWithProvidedMessage) {
    CAPTURE_EXCEPTION_E(Require::Null(m_NonNull, "test message"), std::runtime_error) {
        ASSERT_STREQ(e.what(), "test message");
    }
}

class RequireNotNullTest : public ::testing::Test {
protected:
    int* m_Null{nullptr};
    int* m_NonNull;

    void SetUp() override {
        m_NonNull = new int(4);
    }

    void TearDown() override {
        delete m_NonNull;
    }
};

TEST_F(RequireNotNullTest, DoesNotThrowWithNonNull) {
    ASSERT_NO_THROW(Require::NotNull(m_NonNull));
}

TEST_F(RequireNotNullTest, DoesNotThrowWithNonNullAndMessage) {
    ASSERT_NO_THROW(Require::NotNull(m_NonNull, "message"));
}

TEST_F(RequireNotNullTest, ThrowsWithNull) {
    ASSERT_ANY_THROW(Require::NotNull(m_Null));
}

TEST_F(RequireNotNullTest, ThrowsWithNullAndMessage) {
    ASSERT_ANY_THROW(Require::NotNull(m_Null, "message"));
}

TEST_F(RequireNotNullTest, HasDefaultThrowMessage) {
    CAPTURE_EXCEPTION_E(Require::NotNull(m_Null), std::runtime_error) {
        ASSERT_STRNE(e.what(), "");
    }
}

TEST_F(RequireNotNullTest, PopulatesExceptionWithProvidedMessage) {
    CAPTURE_EXCEPTION_E(Require::NotNull(m_Null, "test message"), std::runtime_error) {
        ASSERT_STREQ(e.what(), "test message");
    }
}

class RequireStringTest : public ::testing::Test {
protected:
    std::string m_Empty = "";
    std::string m_WhiteSpace = " \t\n\r";
    std::string m_Full = "a";
};

TEST_F(RequireStringTest, RequireNotEmptyWithNonEmptyDoesNotThrow) {
    ASSERT_NO_THROW(Require::NotEmpty(m_Full));
}

TEST_F(RequireStringTest, RequireNotEmptyWithNonEmptyAndMessageDoesNotThrow) {
    ASSERT_NO_THROW(Require::NotEmpty(m_Full, "message"));
}

TEST_F(RequireStringTest, RequireNotEmptyWithWhiteSpaceDoesNotThrow) {
    ASSERT_NO_THROW(Require::NotEmpty(m_WhiteSpace));
}

TEST_F(RequireStringTest, RequireNotEmptyWithWhiteSpaceAndMessageDoesNotThrow) {
    ASSERT_NO_THROW(Require::NotEmpty(m_WhiteSpace, "message"));
}

TEST_F(RequireStringTest, RequireNotEmptyWithEmptyThrows) {
    ASSERT_ANY_THROW(Require::NotEmpty(m_Empty));
}

TEST_F(RequireStringTest, RequireNotEmptyWithEmptyAndMessageThrows) {
    ASSERT_ANY_THROW(Require::NotEmpty(m_Empty, "message"));
}

TEST_F(RequireStringTest, RequireNotEmptyHasDefaultThrowMessage) {
    CAPTURE_EXCEPTION_E(Require::NotEmpty(m_Empty), std::runtime_error) {
        ASSERT_STRNE(e.what(), "");
    }
}

TEST_F(RequireStringTest, RequireNotEmptyPopulatesExceptionWithProvidedMessage) {
    CAPTURE_EXCEPTION_E(Require::NotEmpty(m_Empty, "message"), std::runtime_error) {
        ASSERT_STREQ(e.what(), "message");
    }
}

TEST_F(RequireStringTest, RequireNotWhitespaceWithFullDoesNotThrow) {
    ASSERT_NO_THROW(Require::NotJustWhitespace(m_Full));
}

TEST_F(RequireStringTest, RequireNotWhitespaceWIthFullAndMessageDoesNotThrow) {
    ASSERT_NO_THROW(Require::NotJustWhitespace(m_Full, "message"));
}

TEST_F(RequireStringTest, RequireNotWhitespaceWithWhitespaceThrows) {
    ASSERT_ANY_THROW(Require::NotJustWhitespace(m_WhiteSpace));
}

TEST_F(RequireStringTest, RequireNotWhitespaceWIthWhitespaceAndMessageThrows) {
    ASSERT_ANY_THROW(Require::NotJustWhitespace(m_WhiteSpace, "Message"));
}

TEST_F(RequireStringTest, RequireNotWhitespaceWithEmptyThrows) {
    ASSERT_ANY_THROW(Require::NotJustWhitespace(m_Empty));
}

TEST_F(RequireStringTest, RequireNotWhitespaceWithEmptyAndMessageThrows) {
    ASSERT_ANY_THROW(Require::NotJustWhitespace(m_Empty, "message"));
}

TEST_F(RequireStringTest, RequireNotWhitespaceHasDefaultThrowMessage) {
    CAPTURE_EXCEPTION_E(Require::NotJustWhitespace(m_WhiteSpace), std::runtime_error) {
        ASSERT_STRNE(e.what(), "");
    }
}

TEST_F(RequireStringTest, RequireNotWhitespacePopulatesExceptionWithProvidedMessage) {
    CAPTURE_EXCEPTION_E(Require::NotJustWhitespace(m_WhiteSpace, "message"), std::runtime_error) {
        ASSERT_STREQ(e.what(), "message");
    }
}

class RequireVectorTest : public ::testing::Test {
protected:
    std::vector<int> m_Full{1, 2, 3};
    std::vector<int> m_Empty{};
};

TEST_F(RequireVectorTest, RequireEmptyWithEmptyDoesNotThrow) {
    ASSERT_NO_THROW(Require::Empty(m_Empty));
}

TEST_F(RequireVectorTest, RequireEmptyWithEmptyAndMessageDoesNotThrow) {
    ASSERT_NO_THROW(Require::Empty(m_Empty, "message"));
}

TEST_F(RequireVectorTest, RequireEmptyWithFullThrows) {
    ASSERT_ANY_THROW(Require::Empty(m_Full));
}

TEST_F(RequireVectorTest, RequireEmptyWithFullAndMessageThrows) {
    ASSERT_ANY_THROW(Require::Empty(m_Full, "message"));
}

TEST_F(RequireVectorTest, RequireEmptyHasDefaultThrowMessage) {
    CAPTURE_EXCEPTION_E(Require::Empty(m_Full), std::runtime_error) {
        ASSERT_STRNE(e.what(), "");
    }
}

TEST_F(RequireVectorTest, RequireEmptyPopulatesExceptionWithProvidedMessage) {
    CAPTURE_EXCEPTION_E(Require::Empty(m_Full, "message"), std::runtime_error) {
        ASSERT_STREQ(e.what(), "message");
    }
}

TEST_F(RequireVectorTest, RequireNotEmptyWithFullDoesNotThrow) {
    ASSERT_NO_THROW(Require::NotEmpty(m_Full));
}

TEST_F(RequireVectorTest, RequireNotEmptyWithFullAndMessageDoesNotThrow) {
    ASSERT_NO_THROW(Require::NotEmpty(m_Full, "message"));
}

TEST_F(RequireVectorTest, RequireNotEmptyWithEmptyThrows) {
    ASSERT_ANY_THROW(Require::NotEmpty(m_Empty));
}

TEST_F(RequireVectorTest, RequireNotEmptyWithEmptyAndMessageThrows) {
    ASSERT_ANY_THROW(Require::NotEmpty(m_Empty, "message"));
}

TEST_F(RequireVectorTest, RequireNotEmptyHasDefaultThrowMessage) {
    CAPTURE_EXCEPTION_E(Require::NotEmpty(m_Empty), std::runtime_error) {
        ASSERT_STRNE(e.what(), "");
    }
}

TEST_F(RequireVectorTest, RequireNotEmptyPopulatesExceptionWithProvidedMessage) {
    CAPTURE_EXCEPTION_E(Require::NotEmpty(m_Empty, "message"), std::runtime_error) {
        ASSERT_STREQ(e.what(), "message");
    }
}
