#include "Threading/Runner.h"

#include "TestCommon.h"
#include "Threading/IRunnable.h"

#include <memory>

using JobList = std::vector<std::unique_ptr<IRunnable<int>>>;
using VoidJobList = std::vector<std::unique_ptr<IRunnable<VOID>>>;

class ExampleRunnable : public IRunnable<int> {
public:
    ExampleRunnable(int number) : m_Num(number) {}
    int Execute() const {
        return m_Num;
    }

private:
    int m_Num;
};

class VoidRunnable : public IRunnable<VOID> {
public:
    VoidRunnable(bool& hasRun) : m_HasRun(&hasRun) {}
    VOID Execute() const {
        *m_HasRun = true;
        VOID result;
        return result;
    }

private:
    bool* m_HasRun;
};

TEST(ThrottledRunnerTest, ShortLengthRuntimeWorksWithAnEmptyCollection) {
    JobList jobs;
    auto result = Runner::Get().RunAll(Threading::ExpectedRunTime::MICROSECONDS, jobs);

    ASSERT_TRUE(result.empty());
}

TEST(ThrottledRunnerTest, MediumLengthRuntimeWorksWithEmptyCollection) {
    JobList jobs;
    auto result = Runner::Get().RunAll(Threading::ExpectedRunTime::MILLISECONDS, jobs);

    ASSERT_TRUE(result.empty());
}

TEST(ThrottledRunnerTest, LongLengthRuntimeWorksWithEmptyCollection) {
    JobList jobs;
    auto result = Runner::Get().RunAll(Threading::ExpectedRunTime::SECONDS, jobs);
}

TEST(ThrottledRunnerTest, ShortLengthRuntimeWorksWithASingleEntry) {
    JobList jobs;
    jobs.push_back(std::move(std::make_unique<ExampleRunnable>(1)));
    auto result = Runner::Get().RunAll(Threading::ExpectedRunTime::MICROSECONDS, jobs);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result[0], 1);
}

TEST(ThrottledRunnerTest, MediumLengthRuntimeWorksWithASingleEntry) {
    JobList jobs;
    jobs.push_back(std::move(std::make_unique<ExampleRunnable>(1)));
    auto result = Runner::Get().RunAll(Threading::ExpectedRunTime::MILLISECONDS, jobs);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result[0], 1);
}

TEST(ThrottledRunnerTest, LongLengthRuntimeWorksWithASingleEntry) {
    JobList jobs;
    jobs.push_back(std::move(std::make_unique<ExampleRunnable>(1)));
    auto result = Runner::Get().RunAll(Threading::ExpectedRunTime::SECONDS, jobs);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result[0], 1);
}

TEST(ThrottledRunnerTest, SmallLengthRuntimeWorksWithLargeNumberOfEntries) {
    JobList jobs;
    const u32 jobSize = 1233;
    for(int i = 0; i < jobSize; i++) {
        jobs.push_back(std::move(std::make_unique<ExampleRunnable>(i)));
    }

    auto result = Runner::Get().RunAll(Threading::ExpectedRunTime::MICROSECONDS, jobs);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result.size(), jobSize);
    ASSERT_THAT(result, ::testing::Contains(0));
    ASSERT_THAT(result, ::testing::Contains(jobSize - 1));
}

TEST(ThrottledRunnerTest, MediumLengthRuntimeWorksWithLargeNumberOfEntries) {
    JobList jobs;
    const u32 jobSize = 1233;
    for(int i = 0; i < jobSize; i++) {
        jobs.push_back(std::move(std::make_unique<ExampleRunnable>(i)));
    }

    auto result = Runner::Get().RunAll(Threading::ExpectedRunTime::MILLISECONDS, jobs);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result.size(), jobSize);
    ASSERT_THAT(result, ::testing::Contains(0));
    ASSERT_THAT(result, ::testing::Contains(jobSize - 1));
}

TEST(ThrottledRunnerTest, LongLengthRuntimeWorksWithLargeNumberOfEntries) {
    JobList jobs;
    const u32 jobSize = 1233;
    for(int i = 0; i < jobSize; i++) {
        jobs.push_back(std::move(std::make_unique<ExampleRunnable>(i)));
    }

    auto result = Runner::Get().RunAll(Threading::ExpectedRunTime::SECONDS, jobs);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result.size(), jobSize);
    ASSERT_THAT(result, ::testing::Contains(0));
    ASSERT_THAT(result, ::testing::Contains(jobSize - 1));
}

TEST(ThrottledRunnerTest, WorksWithVoidReturns) {
    VoidJobList jobs;
    bool hasRun = false;
    jobs.push_back(std::move(std::make_unique<VoidRunnable>(hasRun)));
    Runner::Get().RunAll(Threading::ExpectedRunTime::MICROSECONDS, jobs);

    ASSERT_TRUE(hasRun);
}