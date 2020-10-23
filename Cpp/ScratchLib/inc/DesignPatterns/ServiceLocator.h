#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

template<class T>
class ServiceLocator {
public:
    static void Set(T& toRegister) {
        service = &toRegister;
    }

    static T& Get() {
        return *service;
    }

private:
    inline static T* service{nullptr};
};
#endif