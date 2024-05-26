#pragma once

#include "Core/Instrumentation/Logging.h"
#include "Core/Platform/Types.h"
#include "Core/Threading/IRunnable.h"
#include "Core/Utilities/ProgressBar.h"
#include "Core/Utilities/ScopedTimer.h"

#include <algorithm>
#include <concurrent_vector.h>
#include <memory>
#include <ppl.h>
#include <ppltasks.h>
#include <thread>
#include <vector>

template<typename ReturnType>
struct TaskQueue {
    constexpr void push(auto func) {
        Tasks.push_back(concurrency::create_task(func));
    }

    std::vector<concurrency::task<ReturnType>> Tasks;
};

class MsvcRunner {
public:
    static MsvcRunner Get() {
        static MsvcRunner runner;
        return runner;
    }

private:
    template<class ReturnType>
    struct IRunner {
        virtual std::vector<ReturnType> Run(u32 maxConcurrency, const std::vector<std::unique_ptr<IRunnable<ReturnType>>>& runnables) = 0;
        virtual ~IRunner() = default;
    };

    template<class ReturnType>
    class SerialRunner : public IRunner<ReturnType> {
    public:
        std::vector<ReturnType> Run(u32, const std::vector<std::unique_ptr<IRunnable<ReturnType>>>& runnables) final {
            std::vector<ReturnType> result;

            for(auto&& runnable: runnables) {
                result.push_back(runnable->Execute());
            }

            return result;
        }
    };

    template<class ReturnType>
    class BatchRunner : public IRunner<ReturnType> {
    public:
        std::vector<ReturnType> Run(u32 maxConcurrency, const std::vector<std::unique_ptr<IRunnable<ReturnType>>>& runnables) final {
            ProgressBar progress(runnables.size());
            Log::Info(StrUtil::Format("Processing %d jobs with %u tasks", runnables.size(), maxConcurrency));

            std::vector<concurrency::task<ReturnType>> tasks; // working 'threads'
            std::vector<ReturnType> result;
            result.reserve(runnables.size());

            u32 workIndex = 0;
            while(workIndex < runnables.size()) {
                u32 toFill = std::min(maxConcurrency, static_cast<u32>(runnables.size() - workIndex));
                for(u32 i = 0; i < toFill; i++) {
                    tasks.push_back(concurrency::create_task([&runnables, workIndex] { return runnables[workIndex]->Execute();}));
                    workIndex++;
                }

                concurrency::when_all(tasks.begin(), tasks.end()).wait();
                for(auto&& completed: tasks) {
                    result.push_back(completed.get());
                }

                progress.Update(result.size());
                tasks.clear();
            }

            return result;
        }
    };

    template<class ReturnType>
    class WorkStealRunner : public IRunner<ReturnType> {
    public:
        std::vector<ReturnType> Run(u32 maxConcurrency, const std::vector<std::unique_ptr<IRunnable<ReturnType>>>& runnables) final {
            ProgressBar progress(runnables.size());

            std::vector<concurrency::task<ReturnType>> tasks;
            std::vector<ReturnType> result;
            result.reserve(runnables.size());
            u32 workIndex = 0;
            u32 concurrentTasks = std::min(maxConcurrency, static_cast<u32>(runnables.size()));

            Log::Info(StrUtil::Format("Processing %d jobs with %u tasks", runnables.size(), concurrentTasks));

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
    };

public:
    template<class ReturnType>
    std::vector<ReturnType> RunAll(Threading::ExpectedRunTime expectedRunTime, const std::vector<std::unique_ptr<IRunnable<ReturnType>>>& runnables) {
        ScopedTimer timer("Runner::RunAll");
        if (runnables.size() < (u64)m_MaxConcurrency * 2 && expectedRunTime < Threading::ExpectedRunTime::SECONDS) {
            return SerialRunner<ReturnType>().Run(m_MaxConcurrency, runnables);
        }
        else if (expectedRunTime < Threading::ExpectedRunTime::SECONDS) {
            return BatchRunner<ReturnType>().Run(m_MaxConcurrency, runnables);
        }
        else {
            return WorkStealRunner<ReturnType>().Run(m_MaxConcurrency, runnables);
        }
    }

    template<typename ReturnType>
    std::vector<ReturnType> RunAll(const TaskQueue<ReturnType> tasks) {
        ScopedTimer timer("Runner::RunAll");
        concurrency::when_all(tasks.Tasks.begin(), tasks.Tasks.end()).wait();
        std::vector<ReturnType> result;
        for (const auto& t : tasks.Tasks) {
            result.push_back(t.get());
        }
        return result;
    }

private:
    MsvcRunner() = default;
    const u32 m_MaxConcurrency{std::max(1u, std::thread::hardware_concurrency())};
};