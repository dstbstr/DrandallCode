#ifndef __CLANGRUNNER_H__
#define __CLANGRUNNER_H__

#include "Threading/IRunnable.h"

#include <memory>
#include <vector>

class ClangRunner {
public:
    static ClangRunner Get() {
        static ClangRunner runner;
        return runner;
    }

    template<class ReturnType>
    std::vector<ReturnType> RunAll(std::vector<std::unique_ptr<IRunnable<ReturnType>>>& runnables) {
        // TODO: Implement with threading
        std::vector<ReturnType> result;
        result.reserve(runnables.size());
        for(auto&& runnable: runnables) {
            result.push_back(runnable->Execute());
        }
        return result;
    }

private:
    ClangRunner() = default;
};
#endif // __CLANGRUNNER_H__