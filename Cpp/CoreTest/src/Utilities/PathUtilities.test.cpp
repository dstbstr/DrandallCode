#include "Utilities/PathUtilities.h"

#include "TestCommon.h"

TEST(PathUtilities, GetFileNameWithNoSeparatorsReturnsFileName) {
    std::string path = "foo.txt";
    ASSERT_EQ(PathUtils::GetFileName(path), path);
}

TEST(PathUtilities, GetFileNameWithSingleForwardSlashReturnsFileName) {
    std::string path = "one/two.txt";
    ASSERT_EQ(PathUtils::GetFileName(path), "two.txt");
}

TEST(PathUtilities, GetFileNameWithSingleBackSlashReturnsFileName) {
    std::string path = "one\\two.txt";
    ASSERT_EQ(PathUtils::GetFileName(path), "two.txt");
}

TEST(PathUtilities, GetFileNameFromBlankPathReturnsBlank) {
    std::string path = "";
    ASSERT_EQ(PathUtils::GetFileName(path), "");
}

TEST(PathUtilities, GetFileWithMultipleForwardSlashesReturnsFileName) {
    std::string path = "one/two/three.txt";
    ASSERT_EQ(PathUtils::GetFileName(path), "three.txt");
}

TEST(PathUtilities, GetFileWithMultipleBackSlashesReturnsFileName) {
    std::string path = R"(one\two\three.txt)";
    ASSERT_EQ(PathUtils::GetFileName(path), "three.txt");
}

TEST(PathUtilities, GetFileFromDirectoryReturnsBlank) {
    std::string path = "one/two/three/";
    ASSERT_EQ(PathUtils::GetFileName(path), "");
}

TEST(PathUtilities, GetFileFromRelativePathReturnsFileName) {
    std::string path = "/one/two.txt";
    ASSERT_EQ(PathUtils::GetFileName(path), "two.txt");
}

TEST(PathUtilities, GetFileWithoutExtensionReturnsFileName) {
    std::string path = "one/two/three";
    ASSERT_EQ(PathUtils::GetFileName(path), "three");
}