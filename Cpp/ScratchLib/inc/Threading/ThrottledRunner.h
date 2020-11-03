#ifndef __THROTTLEDRUNNER_H__
#define __THROTTLEDRUNNER_H__

#include "Platform/Types.h"
#include "Threading/IRunnable.h"

#include <memory>
#include <ppltasks.h>
#include <thread>
#include <vector>

class ThrottledRunner {
public:
    static ThrottledRunner Get() {
        static ThrottledRunner runner;
        return runner;
    }

    template<class ReturnType>
    std::vector<ReturnType> RunAll(std::vector<std::unique_ptr<IRunnable<ReturnType>>>& runnables) {
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

            concurrency::when_all(tasks.begin(), tasks.end());
            for(auto&& completed: tasks) {
                result.push_back(completed.get());
            }
            tasks.clear();
            jobs.clear();
        }

        return result;
    }

private:
    ThrottledRunner() = default;
    const u32 m_MaxConcurrency{std::thread::hardware_concurrency()};
};
#endif // __THROTTLEDRUNNER_H__