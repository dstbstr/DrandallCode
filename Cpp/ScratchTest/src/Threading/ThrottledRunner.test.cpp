#include "Threading/ThrottledRunner.h"

#include "TestCommon.h"
#include "Threading/IRunnable.h"

#include <memory>

using JobList = std::vector<std::unique_ptr<IRunnable<int>>>;

class ExampleRunnable : public IRunnable<int> {
public:
    ExampleRunnable(int number) : m_Num(number) {}
    int Execute() {
        return m_Num;
    }

private:
    int m_Num;
};

TEST(ThrottledRunnerTest, WorksWithAnEmptyCollection) {
    JobList jobs;
    auto result = ThrottledRunner::Get().RunAll(jobs);

    ASSERT_TRUE(result.empty());
}

TEST(ThrottledRunnerTest, WorksWithASingleEntry) {
    JobList jobs;
    jobs.push_back(std::move(std::make_unique<ExampleRunnable>(1)));
    auto result = ThrottledRunner::Get().RunAll(jobs);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result[0], 1);
}

TEST(ThrottledRunnerTest, WorksWithLargeNumberOfEntries) {
    JobList jobs;
    for(int i = 0; i < 500; i++) {
        jobs.push_back(std::move(std::make_unique<ExampleRunnable>(i)));
    }

    auto result = ThrottledRunner::Get().RunAll(jobs);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result.size(), 500);
}