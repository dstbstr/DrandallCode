#ifndef __MSVCRUNNER_H__
#define __MSVCRUNNER_H__

#include "Instrumentation/Log.h"
#include "Platform/Types.h"
#include "Threading/IRunnable.h"
#include "Utilities/ScopedTimer.h"

#include <algorithm>
#include <memory>
#include <ppltasks.h>
#include <thread>
#include <vector>

class MsvcRunner {
public:
    static MsvcRunner Get() {
        static MsvcRunner runner;
        return runner;
    }

    template<class ReturnType>
    std::vector<ReturnType> RunAll(std::vector<std::unique_ptr<IRunnable<ReturnType>>>& runnables) {
        ScopedTimer timer("Runner::RunAll");
        std::vector<concurrency::task<ReturnType>> tasks; // working 'threads'
        std::vector<std::unique_ptr<IRunnable<ReturnType>>> jobs; // ensure that the unique_ptrs survive beyond the when_all
        std::vector<ReturnType> result;
        result.reserve(runnables.size());

        auto initialSize = runnables.size();
        LOG_INFO(StrUtil::Format("Received %d jobs to run over %u tasks", initialSize, m_MaxConcurrency));
        int lastPercentPrinted = -1;
        while(!runnables.empty()) {
            auto percentComplete = 100 - static_cast<int>(std::floor(100 * runnables.size() / initialSize));
            if(percentComplete % 10 == 0 && percentComplete != lastPercentPrinted) {
                LOG_INFO(StrUtil::Format("%d%% complete", percentComplete));
                lastPercentPrinted = percentComplete;
            }

            for(u32 i = 0; i < m_MaxConcurrency && !runnables.empty(); i++) {
                jobs.push_back(std::move(runnables.back()));
                runnables.pop_back();
                concurrency::task<ReturnType> task = concurrency::create_task([&jobs, i] { return jobs[i]->Execute(); });
                tasks.push_back(task);
            }

            concurrency::when_all(tasks.begin(), tasks.end());
            for(auto&& completed: tasks) {
                result.push_back(completed.get());
            }
            tasks.clear();
            jobs.clear();
        }

        return result;
    }

    template<class ReturnType>
    std::vector<ReturnType> RunAll2(std::vector<std::unique_ptr<IRunnable<ReturnType>>>& runnables) {
        ScopedTimer timer("Runner::RunAll2");
        std::vector<ReturnType> result;
        result.reserve(runnables.size());
    }

private:
    MsvcRunner() = default;
    const u32 m_MaxConcurrency{std::max(u32(1), std::thread::hardware_concurrency())};
};
#endif // __MSVCRUNNER_H__