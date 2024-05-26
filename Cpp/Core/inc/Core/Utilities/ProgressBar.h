#pragma once

class ProgressBar {
public:
    ProgressBar(size_t finalNumber) : m_FinalNumber(finalNumber) {}
    ~ProgressBar();
    void Update(size_t currentProgress);

private:
    size_t m_FinalNumber{0};
};