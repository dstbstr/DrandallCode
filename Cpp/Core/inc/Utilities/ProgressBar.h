#ifndef __PROGRESSBAR_H__
#define __PROGRESSBAR_H__

#include "Platform/Types.h"

class ProgressBar {
public:
    ProgressBar(size_t finalNumber) : m_FinalNumber(finalNumber) {}
    ~ProgressBar();
    void Update(size_t currentProgress);

private:
    size_t m_FinalNumber{0};
};

#endif // __PROGRESSBAR_H__