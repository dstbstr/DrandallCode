#include "IncludeCounter/IncludeMapGenerator.h"

#include "IncludeCounter/Data/FileData.h"
#include "TestCommon.h"
#include "Utilities/Require.h"

//            A
//          /   \ 
//        B       C
//       /  \   /  \ 
//      D     E     F

namespace IncludeCounter {
    class IncludeMapGeneratorTest : public ::testing::Test {
    protected:
        void SetUp() override {
            auto A = MakeData("A");
            auto B = MakeData("B");
            auto C = MakeData("C");
            auto D = MakeData("D");
            auto E = MakeData("E");
            auto F = MakeData("F");

            AddDependency(B, A);
            AddDependency(D, B);
            AddDependency(C, A);
            AddDependency(E, B);
            AddDependency(E, C);
            AddDependency(F, C);

            m_Files = {A, B, C, D, E, F};
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

        FileData MakeData(std::string name) {
            return FileData{name + ".h", name + "/" + name + ".h"};
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
        ASSERT_EQ(Get("A").IncludedByCount, 5); //B, C, D, E, F
    }

    TEST_F(IncludeMapGeneratorTest, CircularDependencyDoesNotFailByDefault) {
        auto G = MakeData("G");
        auto H = MakeData("H");
        AddDependency(G, H);
        AddDependency(H, G);
        m_Files.push_back(G);
        m_Files.push_back(H);

        Run();
        ASSERT_EQ(Get("G").TotalIncludeCount, 2); //Included by G and H
        ASSERT_EQ(Get("H").TotalIncludeCount, 2);
    }

    TEST_F(IncludeMapGeneratorTest, CircularDependencyFailsIfFlagSet) {
        auto G = MakeData("G");
        auto H = MakeData("H");
        AddDependency(G, H);
        AddDependency(H, G);
        m_Files.push_back(G);
        m_Files.push_back(H);

        ASSERT_ANY_THROW(Run(true));
    }
} // namespace IncludeCounter