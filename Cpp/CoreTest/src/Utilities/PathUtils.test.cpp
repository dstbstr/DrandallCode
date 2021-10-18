#include "Utilities/PathUtils.h"

#include "TestCommon.h"

TEST(GetFileName, WithNoSeparators_ReturnsFileName) {
    std::string path = "foo.txt";
    ASSERT_EQ(PathUtils::GetFileName(path), path);
}

TEST(GetFileName, WithSingleForwardSlash_ReturnsFileName) {
    std::string path = "one/two.txt";
    ASSERT_EQ(PathUtils::GetFileName(path), "two.txt");
}

TEST(GetFileName, WithSingleBackSlash_ReturnsFileName) {
    std::string path = "one\\two.txt";
    ASSERT_EQ(PathUtils::GetFileName(path), "two.txt");
}

TEST(GetFileName, FromBlankPath_ReturnsBlank) {
    std::string path = "";
    ASSERT_EQ(PathUtils::GetFileName(path), "");
}

TEST(GetFileName, WithMultipleForwardSlashes_ReturnsFileName) {
    std::string path = "one/two/three.txt";
    ASSERT_EQ(PathUtils::GetFileName(path), "three.txt");
}

TEST(GetFileName, WithMultipleBackSlashes_ReturnsFileName) {
    std::string path = R"(one\two\three.txt)";
    ASSERT_EQ(PathUtils::GetFileName(path), "three.txt");
}

TEST(GetFileName, FromDirectory_ReturnsBlank) {
    std::string path = "one/two/three/";
    ASSERT_EQ(PathUtils::GetFileName(path), "");
}

TEST(GetFileName, FromRelativePath_ReturnsFileName) {
    std::string path = "/one/two.txt";
    ASSERT_EQ(PathUtils::GetFileName(path), "two.txt");
}

TEST(GetFileName, WithoutExtension_ReturnsFileName) {
    std::string path = "one/two/three";
    ASSERT_EQ(PathUtils::GetFileName(path), "three");
}

TEST(GetFileDirectory, WithDirectoryAndFile_ReturnsDirectory) {
    std::string path = "one/two/three";
    ASSERT_EQ(PathUtils::GetFileDirectory(path), "one/two/");
}

TEST(GetFileDirectory, FromRelativePath_ReturnsRelativePath) {
    std::string path = "/one/two/three";
    ASSERT_EQ(PathUtils::GetFileDirectory(path), "/one/two/");
}

TEST(GetFileDirectory, WithDirectory_ReturnsDirectory) {
    std::string path = "one/two/three/";
    ASSERT_EQ(PathUtils::GetFileDirectory(path), path);
}

TEST(GetFileDirectory, WithBackslashes_ReturnsDirectory) {
    std::string path = "one\\two\\three";
    ASSERT_EQ(PathUtils::GetFileDirectory(path), "one\\two\\");
}

TEST(ContainsWildcard, WithStarReturnsTrue) {
    ASSERT_TRUE(PathUtils::PathContainsWildcard("Test*.csv"));
}

TEST(ContainsWildcard, WithQuestionmarkReturnsTrue) {
    ASSERT_TRUE(PathUtils::PathContainsWildcard("Test.?"));
}

TEST(ContainsWildcard, WithoutWildcardReturnsFalse) {
    ASSERT_FALSE(PathUtils::PathContainsWildcard("Test.txt"));
}

TEST(PathToRegex, WithStarMatchesSingleWildcard) {
    auto regex = PathUtils::PathWithWildcardToRegex("Test*.csv");
    ASSERT_TRUE(std::regex_match("Test1.csv", regex));
}

TEST(PathToRegex, WithStarMatchesMultipleCharacters) {
    auto regex = PathUtils::PathWithWildcardToRegex("Test*.csv");
    ASSERT_TRUE(std::regex_match("Test123.csv", regex));
}

TEST(PathToRegex, WithStarDoesNotMatchForwardSlash) {
    auto regex = PathUtils::PathWithWildcardToRegex("Test*.csv");
    ASSERT_FALSE(std::regex_match("Test/.csv", regex));
}

TEST(PathToRegex, WithStartDoesNotMatchBackSlash) {
    auto regex = PathUtils::PathWithWildcardToRegex("Test*.csv");
    ASSERT_FALSE(std::regex_match("Test\\.csv", regex));
}

TEST(PathToRegex, DotInPathIsEscpaed) {
    auto regex = PathUtils::PathWithWildcardToRegex("Test.csv");
    ASSERT_FALSE(std::regex_match("Test_csv", regex));
}

TEST(PathToRegex, ForwardSlashInPathIsEscaped) {
    auto regex = PathUtils::PathWithWildcardToRegex("Test/One/Two");
    ASSERT_TRUE(std::regex_match("Test/One/Two", regex));
}

TEST(PathToRegex, BackSlashInPathIsEscaped) {
    auto regex = PathUtils::PathWithWildcardToRegex("Test\\One\\Two");
    ASSERT_TRUE(std::regex_match("Test\\One\\Two", regex));
}

TEST(PathToRegex, QuestionMatchesSingleCharacter) {
    auto regex = PathUtils::PathWithWildcardToRegex("Test?.csv");
    ASSERT_TRUE(std::regex_match("Test1.csv", regex));
}

TEST(PathToRegex, QuestionDoesNotMatchMultipleCharacters) {
    auto regex = PathUtils::PathWithWildcardToRegex("Test?.csv");
    ASSERT_FALSE(std::regex_match("Test12.csv", regex));
}

TEST(PathToRegex, QuestionDoesNotMatchForwardSlash) {
    auto regex = PathUtils::PathWithWildcardToRegex("Test?.csv");
    ASSERT_FALSE(std::regex_match("Test/.csv", regex));
}

TEST(PathToRegex, SupportsMultipleWildcards) {
    auto regex = PathUtils::PathWithWildcardToRegex("Test*.*");
    ASSERT_TRUE(std::regex_match("Test1.h", regex));
    ASSERT_TRUE(std::regex_match("Test25.csv", regex));
}

TEST(WildcardPathParent, ReturnsValueUnchangedWhenNoWildcardPresent) {
    ASSERT_EQ(PathUtils::GetWildcardPathParent("Test/One"), "Test/One");
}

TEST(WildcardPathParent, ReturnsParentWhenStarPresent) {
    ASSERT_EQ(PathUtils::GetWildcardPathParent("Test/One*"), "Test");
}

TEST(WildcardPathParent, ReturnsParentWhenMultipleDirectoriesProvided) {
    ASSERT_EQ(PathUtils::GetWildcardPathParent("Test/One/Two/Three*"), "Test/One/Two");
}

TEST(WildcardPathParent, ReturnsLongestAncestorWhenMultipleWildcards) {
    ASSERT_EQ(PathUtils::GetWildcardPathParent("Test/One*/Two/Three*"), "Test/One*/Two");
}

TEST(WildcardPathParent, ReturnsParentWhenQuestionPresent) {
    ASSERT_EQ(PathUtils::GetWildcardPathParent("Test/One/Two?"), "Test/One");
}