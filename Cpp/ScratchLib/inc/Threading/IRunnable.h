#ifndef __IRUNNABLE_H__
#define __IRUNNABLE_H__

template<class ReturnType>
struct IRunnable {
    virtual ReturnType Execute() = 0;
};
#endif // __IRUNNABLE_H__