#include "Runner/SolutionRunner.h"
#include "Core/Constexpr/ConstexprStrUtils.h"
#include "Core/Instrumentation/Logging.h"
#include "Core/Instrumentation/ScopedTimer.h"
//#include "Core/Threading/Runner.h"
//#include "Core/Threading/IRunnable.h"
#include "Common.h"
#include "Core/Utilities/TimeUtils.h"

#include <chrono>
#include <format>
#include <future>

namespace {
    std::string MakeKey(size_t year, size_t day, size_t part) {
		return Constexpr::ToString(year) + "/" + Constexpr::ToString(day) + ":" + Constexpr::ToString(part);
	}

    void WriteLogs() {
        if (!GET_LOGS().empty()) Log::Info("## Logs ##");
        for (const auto& log : GET_LOGS()) {
            Log::Info(log);
        }
        GET_LOGS().clear();
    }

    auto GatherTiming(std::map<std::string, std::chrono::microseconds>& timingData) {
        return [&](std::string_view key, std::chrono::microseconds elapsed) {
            timingData[std::string(key)] = elapsed;
        };
    };

}

bool SolutionRunner::CheckTestsPass(size_t year, size_t day) {
    if (!GetTests().contains(year) || !GetTests()[year].contains(day)) {
        return true;
    }

    if (auto testTime = ScopedTimer(MakeKey(year, day, 0), GatherTiming(m_TimingData)); GetTests()[year][day]()) {
        Log::Info("Test Pass");
        return true;
    }
    else {
        Log::Info("Test Fail. :(");
        WriteLogs();
        return false;
    }
}

void SolutionRunner::AddSolution(size_t year, size_t day) {
	m_ToRun.emplace_back([=]() {
        Log::Info(std::format("### {} Day {} ###", year, day));
        if (!CheckTestsPass(year, day)) return;

        for (const auto& [part, func] : GetSolutions()[year][day]) {
            std::string result;
            auto input = m_InputReader->ReadInput(year, day);
            {
                auto partTime = ScopedTimer(MakeKey(year, day, part), GatherTiming(m_TimingData));
                result = func(input);
            }
            Log::Info(std::format("Part {}: {}", part, result));
            WriteLogs();
        }
    });
}

SolutionRunner::SolutionRunner(std::unique_ptr<IInputReader>&& inputReader, bool sync) 
    : m_InputReader(std::move(inputReader))
    , m_Sync(sync)
{
	for (const auto& [year, days] : GetSolutions()) {
		for (const auto& [day, parts] : days) {
            AddSolution(year, day);
		}
	}
}

SolutionRunner::SolutionRunner(size_t year, std::unique_ptr<IInputReader>&& inputReader, bool sync) 
    : m_InputReader(std::move(inputReader))
    , m_Sync(sync)
{
	for (const auto& [day, parts] : GetSolutions()[year]) {
		AddSolution(year, day);
	}
}

SolutionRunner::SolutionRunner(size_t year, size_t day, std::unique_ptr<IInputReader>&& inputReader, bool sync) 
    : m_InputReader(std::move(inputReader))
    , m_Sync(sync)
{
    AddSolution(year, day);
}


void SolutionRunner::Run() {
    if(m_Sync) {
		for (const auto& run : m_ToRun) {
			run();
		}
	}
	else {
        // TODO: Rewrite the Runner to be cross-platform
        std::vector<std::future<void>> futures;
        std::vector copy = m_ToRun;
        auto maxThreads = std::max(1ull, static_cast<size_t>(std::thread::hardware_concurrency()));
        while(!copy.empty()) {
			auto taskCount = std::min(copy.size(), maxThreads);
            for(auto i = 0; i < taskCount; i++) {
				futures.push_back(std::async(std::launch::async, copy[i]));
            }
			for (const auto& future : futures) {
                future.wait();
            }
			copy.erase(copy.begin(), copy.begin() + taskCount);
        }
    }
}

void SolutionRunner::LogTimingData(size_t maxResults, std::chrono::microseconds minElapsed) const {
    std::vector<std::pair<std::string, std::chrono::microseconds>> copy(m_TimingData.begin(), m_TimingData.end());
    std::sort(copy.begin(), copy.end(), [](auto lhs, auto rhs) { return rhs.second < lhs.second; });

    size_t resultId = 0;
    for (auto [key, elapsed] : copy) {
        if (minElapsed > std::chrono::microseconds(0) && elapsed < minElapsed) break;
        if (maxResults > 0 && resultId > maxResults) break;

        Log::Info(std::format("{}: {}", key, TimeUtils::DurationToString(elapsed)));
    }
}
