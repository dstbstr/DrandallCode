#ifndef __GNURUNNER_H__
#define __GNURUNNER_H__

#include "Threading/IRunnable.h"

#include <memory>
#include <vector>

class GnuRunner {
public:
    static GnuRunner Get() {
        static GnuRunner runner;
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
    GnuRunner() = default;
};

#endif // __GNURUNNER_H__