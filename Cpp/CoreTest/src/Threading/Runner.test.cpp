#include "Threading/Runner.h"

#include "TestCommon.h"
#include "Threading/IRunnable.h"

#include <memory>


using JobList = std::vector<std::unique_ptr<IRunnable<int>>>;
using VoidJobList = std::vector<std::unique_ptr<IRunnable<VOID>>>;

class ExampleRunnable : public IRunnable<int> {
public:
    ExampleRunnable(int number) : m_Num(number) {}
    int Execute() {
        return m_Num;
    }

private:
    int m_Num;
};

class VoidRunnable : public IRunnable<VOID> {
public:
    VoidRunnable(bool& hasRun) : m_HasRun(&hasRun) {}
    VOID Execute() {
        *m_HasRun = true;
        VOID result;
        return result;
    }

private:
    bool* m_HasRun;
};

TEST(ThrottledRunnerTest, WorksWithAnEmptyCollection) {
    JobList jobs;
    auto result = Runner::Get().RunAll(jobs);

    ASSERT_TRUE(result.empty());
}

TEST(ThrottledRunnerTest, WorksWithASingleEntry) {
    JobList jobs;
    jobs.push_back(std::move(std::make_unique<ExampleRunnable>(1)));
    auto result = Runner::Get().RunAll(jobs);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result[0], 1);
}

TEST(ThrottledRunnerTest, WorksWithLargeNumberOfEntries) {
    JobList jobs;
    for(int i = 0; i < 5000; i++) {
        jobs.push_back(std::move(std::make_unique<ExampleRunnable>(i)));
    }

    auto result = Runner::Get().RunAll(jobs);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result.size(), 5000);
}

TEST(ThrottledRunnerTest, WorksWithVoidReturns) {
    VoidJobList jobs;
    bool hasRun = false;
    jobs.push_back(std::move(std::make_unique<VoidRunnable>(hasRun)));
    Runner::Get().RunAll(jobs);

    ASSERT_TRUE(hasRun);
}