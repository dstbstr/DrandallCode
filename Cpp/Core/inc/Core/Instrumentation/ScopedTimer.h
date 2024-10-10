#pragma once

#include <chrono>
#include <functional>

class ScopedTimer {
public:
    ScopedTimer(std::string&& label, std::function<void(std::string_view, std::chrono::microseconds)> onExit) 
		: m_Label(std::move(label))
        , m_StartTime(std::chrono::high_resolution_clock::now())
		, m_OnExit(onExit)
    {}

    ~ScopedTimer() {
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_StartTime);
        m_OnExit(m_Label, elapsed);
    }

private:
    std::string m_Label;
    std::chrono::steady_clock::time_point m_StartTime;
    std::function<void(std::string_view, std::chrono::microseconds)> m_OnExit{};
};