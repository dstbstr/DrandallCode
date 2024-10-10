#pragma once

#include "Runner/IInputReader.h"
#include <vector>
#include <map>
#include <chrono>
#include <functional>

class SolutionRunner {
public:
	SolutionRunner(std::unique_ptr<IInputReader>&& inputReader, bool sync = false);
	SolutionRunner(size_t year, std::unique_ptr<IInputReader>&& inputReader, bool sync = false);
	SolutionRunner(size_t year, size_t day, std::unique_ptr<IInputReader>&& inputReader, bool sync = false);

	void Run();
	void LogTimingData(size_t maxResults = 0, std::chrono::microseconds minElapsed = std::chrono::microseconds(0)) const;

private:
	std::unique_ptr<IInputReader> m_InputReader{nullptr};
	bool m_Sync{false};
	std::map<std::string, std::chrono::microseconds> m_TimingData{};
	std::vector<std::function<void()>> m_ToRun;

	bool CheckTestsPass(size_t year, size_t day);
	void AddSolution(size_t year, size_t day);
};