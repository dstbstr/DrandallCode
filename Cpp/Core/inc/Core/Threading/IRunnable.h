#pragma once

template<class ReturnType>
struct IRunnable {
    virtual ReturnType Execute() const = 0;
    virtual ~IRunnable() = default;
};

struct VOID {};