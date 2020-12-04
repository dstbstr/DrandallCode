#ifndef __PROGRESSBAR_H__
#define __PROGRESSBAR_H__

#include "Platform/Types.h"

class ProgressBar {
public:
    ProgressBar(u32 finalNumber) : m_FinalNumber(finalNumber) {}
    ~ProgressBar();
    void Update(u32 currentProgress);

private:
    u32 m_FinalNumber;
};

#endif // __PROGRESSBAR_H__