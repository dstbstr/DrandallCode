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

        FileData& Get(std::string fileName) {
            for(auto& file: m_Files) {
                if(file.FileName == fileName + ".h") {
                    return file;
                }
            }

            Require::False(true, "Failed to locate " + fileName);
            return m_Files[0];
        }

        FileData MakeData(std::string name, u8 lineCount) {
            FileData result{name + ".h", name + "/" + name + ".h"};
            result.LineCount = lineCount;
            return result;
        }
        void AddDependency(FileData& data, const FileData& dependency) {
            data.IncludeFiles.insert(dependency.FileName);
        }

        void Run(bool failOnCirc = false) {
            IncludeMapGenerator(m_Files, failOnCirc).Generate();
        }
    };

    TEST_F(IncludeMapGeneratorTest, FileWithNoIncludesHasZeroIncludeCount) {
        Run();
        ASSERT_EQ(Get("A").TotalIncludeCount, 0);
    }

    TEST_F(IncludeMapGeneratorTest, FileWithSingleDependencyHasOneIncludeCount) {
        Run();
        ASSERT_EQ(Get("B").TotalIncludeCount, 1);
        ASSERT_EQ(Get("C").TotalIncludeCount, 1);
    }

    TEST_F(IncludeMapGeneratorTest, FileWithTransitiveDependenciesCountsAll) {
        Run();
        ASSERT_EQ(Get("D").TotalIncludeCount, 2);
        ASSERT_EQ(Get("F").TotalIncludeCount, 2);
    }

    TEST_F(IncludeMapGeneratorTest, FileWithMultipleCopiesOfDependencyCountsUnique) {
        Run();
        ASSERT_EQ(Get("E").TotalIncludeCount, 3); // B and C both depend on A.
    }

    TEST_F(IncludeMapGeneratorTest, FileNotIncludedByOthersHasZeroIncludedByCount) {
        Run();
        ASSERT_EQ(Get("D").IncludedByCount, 0);
    }

    TEST_F(IncludeMapGeneratorTest, FileIncludedByTwoFilesHasIncludedByCountOfTwo) {
        Run();
        ASSERT_EQ(Get("B").IncludedByCount, 2);
    }

    TEST_F(IncludeMapGeneratorTest, FileIncludedByAllDoesNotCountDuplicates) {
        Run();
        ASSERT_EQ(Get("A").IncludedByCount, 5); // B, C, D, E, F
    }

    TEST_F(IncludeMapGeneratorTest, CircularDependencyDoesNotFailByDefault) {
        auto G = MakeData("G", 1);
        auto H = MakeData("H", 1);
        AddDependency(G, H);
        AddDependency(H, G);
        m_Files.push_back(G);
        m_Files.push_back(H);

        Run();
        ASSERT_EQ(Get("G").TotalIncludeCount, 2); // Included by G and H
        ASSERT_EQ(Get("H").TotalIncludeCount, 2);
    }

    TEST_F(IncludeMapGeneratorTest, CircularDependencyFailsIfFlagSet) {
        auto G = MakeData("G", 1);
        auto H = MakeData("H", 1);
        AddDependency(G, H);
        AddDependency(H, G);
        m_Files.push_back(G);
        m_Files.push_back(H);

        ASSERT_ANY_THROW(Run(true));
    }

    TEST_F(IncludeMapGeneratorTest, NoDependenciesHasLineCountEqualToTotalLineCount) {
        Run();
        ASSERT_EQ(Get("A").LineCount, 1);
        ASSERT_EQ(Get("A").TotalLineCount, 1);
    }

    TEST_F(IncludeMapGeneratorTest, SingleDependencyShouldCountAllLines) {
        Run();
        auto B = Get("B");
        ASSERT_EQ(B.LineCount, 2);
        ASSERT_EQ(B.TotalLineCount, 3);

        auto C = Get("C");
        ASSERT_EQ(C.LineCount, 3);
        ASSERT_EQ(C.TotalLineCount, 4);
    }

    TEST_F(IncludeMapGeneratorTest, TransitiveDependencyLineCountShouldBeAggregated) {
        Run();
        auto D = Get("D");
        ASSERT_EQ(D.LineCount, 4);
        ASSERT_EQ(D.TotalLineCount, 7); // 4 + 2 + 1

        auto E = Get("E");
        ASSERT_EQ(E.LineCount, 5);
        ASSERT_EQ(E.TotalLineCount, 11); // 5 + 3 + 2 + 1

        auto F = Get("F");
        ASSERT_EQ(F.LineCount, 6);
        ASSERT_EQ(F.TotalLineCount, 10); // 6 + 3 + 1
    }
} // namespace Extractor