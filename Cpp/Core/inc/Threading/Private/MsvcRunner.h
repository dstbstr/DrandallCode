#ifndef __MSVCRUNNER_H__
#define __MSVCRUNNER_H__

#include "Instrumentation/Log.h"
#include "Platform/Types.h"
#include "Threading/IRunnable.h"
#include "Utilities/ProgressBar.h"
#include "Utilities/ScopedTimer.h"

#include <algorithm>
#include <concurrent_vector.h>
#include <memory>
#include <ppl.h>
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
    std::vector<ReturnType> RunAsBatch(std::vector<std::unique_ptr<IRunnable<ReturnType>>>& runnables) {
        ProgressBar progress(runnables.size());
        LOG_INFO(StrUtil::Format("Processing %d jobs with %u tasks", runnables.size(), m_MaxConcurrency));

        std::vector<concurrency::task<ReturnType>> tasks; // working 'threads'
        std::vector<std::unique_ptr<IRunnable<ReturnType>>> jobs; // ensure that the unique_ptrs survive beyond the when_all
        std::vector<ReturnType> result;
        result.reserve(runnables.size());

        while(!runnables.empty()) {
            for(u32 i = 0; i < m_MaxConcurrency && !runnables.empty(); i++) {
                jobs.push_back(std::move(runnables.back()));
                runnables.pop_back();
                concurrency::task<ReturnType> task = concurrency::create_task([&jobs, i] { return jobs[i]->Execute(); });
                tasks.push_back(task);
            }

            concurrency::when_all(tasks.begin(), tasks.end()).wait();
            for(auto&& completed: tasks) {
                result.push_back(completed.get());
            }

            progress.Update(result.size());
            tasks.clear();
            jobs.clear();
        }

        return result;
    }

    template<class ReturnType>
    std::vector<ReturnType> RunAsWorkStealing(std::vector<std::unique_ptr<IRunnable<ReturnType>>>& runnables) {
        ProgressBar progress(runnables.size());

        std::vector<concurrency::task<ReturnType>> tasks;
        std::vector<ReturnType> result;
        result.reserve(runnables.size());
        u32 workIndex = 0;
        u32 concurrentTasks = std::min(m_MaxConcurrency, static_cast<u32>(runnables.size()));

        LOG_INFO(StrUtil::Format("Processing %d jobs with %u tasks", runnables.size(), concurrentTasks));

        for(; workIndex < concurrentTasks; workIndex++) {
            tasks.push_back(concurrency::create_task([&runnables, workIndex] { return runnables[workIndex]->Execute(); }));
        }

        while(workIndex < runnables.size()) {
            concurrency::when_any(tasks.begin(), tasks.end()).wait();
            progress.Update(result.size());
            if(workIndex < runnables.size()) {
                bool keepWorking = true;
                for(concurrency::task<ReturnType>& task: tasks) {
                    if(task.is_done()) {
                        result.push_back(task.get());
                        task = concurrency::create_task([&runnables, workIndex] { return runnables[workIndex]->Execute(); });
                        workIndex++;
                        if(workIndex >= runnables.size()) {
                            keepWorking = false;
                            break;
                        }
                    }
                }
                if(!keepWorking) {
                    break;
                }
            }
        }

        concurrency::when_all(tasks.begin(), tasks.end()).wait();
        for(auto&& completed: tasks) {
            result.push_back(completed.get());
        }

        return result;
    }

    template<class ReturnType>
    std::vector<ReturnType> RunAll(std::vector<std::unique_ptr<IRunnable<ReturnType>>>& runnables) {
        ScopedTimer timer("Runner::RunAll");
        return RunAsWorkStealing(runnables);
        //return RunAsBatch(runnables);
    }

private:
    MsvcRunner() = default;
    const u32 m_MaxConcurrency{std::max(u32(1), std::thread::hardware_concurrency())};
};
#endif // __MSVCRUNNER_H__