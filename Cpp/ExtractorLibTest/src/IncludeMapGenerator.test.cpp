#include "Extractor/Workers/IncludeMapGenerator.h"

#include "Extractor/Data/FileData.h"
#include "TestCommon.h"
#include "Utilities/Require.h"

//            A
//          /   \ 
//        B       C
//       /  \   /  \ 
//      D     E     F

namespace Extractor {
    class IncludeMapGeneratorTest : public ::testing::Test {
    protected:
        void SetUp() override {
            auto A = MakeData("A", 1);
            auto B = MakeData("B", 2);
            auto C = MakeData("C", 3);
            auto D = MakeData("D", 4);
            auto E = MakeData("E", 5);
            auto F = MakeData("F", 6);

            AddDependency(B, A);
            AddDependency(D, B);
            AddDependency(C, A);
            AddDependency(E, B);
            AddDependency(E, C);
            AddDependency(F, C);

            m_Files = {F, E, D, C, B, A};
        }

        std::vector<FileData> m_Files;

        FileData MakeData(std::string name, u8 lineCount) {
            FileData result{name, name + "/" + name};
            result.LineCount = lineCount;
            return result;
        }
        void AddDependency(FileData& data, const FileData& dependency) {
            data.IncludeFiles.insert(dependency.FileName);
        }

        IncludeMap Run() {
            return GenerateIncludeMap(m_Files);
        }
    };

    TEST_F(IncludeMapGeneratorTest, FileWithNoIncludesHasZeroIncludeCount) {
        auto result = Run();
        ASSERT_EQ(result.Dependencies["A"].size(), 0);
    }

    TEST_F(IncludeMapGeneratorTest, FileWithSingleDependencyHasOneIncludeCount) {
        auto result = Run();
        ASSERT_EQ(result.Dependencies["B"].size(), 1);
        ASSERT_EQ(result.Dependencies["C"].size(), 1);
    }

    TEST_F(IncludeMapGeneratorTest, FileWithTransitiveDependenciesCountsAll) {
        auto result = Run();
        ASSERT_EQ(result.Dependencies["D"].size(), 2);
        ASSERT_EQ(result.Dependencies["F"].size(), 2);
    }

    TEST_F(IncludeMapGeneratorTest, FileWithMultipleCopiesOfDependencyCountsUnique) {
        auto result = Run();
        ASSERT_EQ(result.Dependencies["E"].size(), 3); // B and C both depend on A
    }

    TEST_F(IncludeMapGeneratorTest, FileNotIncludedByOthersHasZeroIncludedByCount) {
        auto result = Run();
        ASSERT_EQ(result.DependsOnMe["D"].size(), 0);
    }

    TEST_F(IncludeMapGeneratorTest, FileIncludedByTwoFilesHasIncludedByCountOfTwo) {
        auto result = Run();
        ASSERT_EQ(result.DependsOnMe["B"].size(), 2);
    }

    TEST_F(IncludeMapGeneratorTest, FileIncludedByAllDoesNotCountDuplicates) {
        auto result = Run();
        ASSERT_EQ(result.DependsOnMe["A"].size(), 5); // B, C, D, E, F
    }

    TEST_F(IncludeMapGeneratorTest, LineCountsPopulatedByName) {
        auto result = Run();
        ASSERT_EQ(result.LineCounts["A"], 1);
        ASSERT_EQ(result.LineCounts["B"], 2);
        ASSERT_EQ(result.LineCounts["C"], 3);
        ASSERT_EQ(result.LineCounts["D"], 4);
        ASSERT_EQ(result.LineCounts["E"], 5);
        ASSERT_EQ(result.LineCounts["F"], 6);
    }

} // namespace Extractor