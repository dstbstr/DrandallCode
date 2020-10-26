#ifndef __SERVICELOCATOR_H__
#define __SERVICELOCATOR_H__

template<class T>
class ServiceLocator {
public:
    static void Set(T& toRegister) {
        service = &toRegister;
    }

    static T& Get() {
        return *service;
    }

    static bool IsSet() {
        return service != nullptr;
    }

private:
    static T* service;
};

template<class T>
T* ServiceLocator<T>::service = nullptr;
#endif // __SERVICELOCATOR_H__