#ifndef __IRUNNABLE_H__
#define __IRUNNABLE_H__

template<class ReturnType>
struct IRunnable {
    virtual ReturnType Execute() const = 0;
    virtual ~IRunnable() = default;
};

struct VOID {};
#endif // __IRUNNABLE_H__