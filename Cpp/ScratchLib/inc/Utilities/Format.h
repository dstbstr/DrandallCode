#ifndef FORMAT_H
#define FORMAT_H
#include <string>
//#include <iostream>
#include <memory>
#include <type_traits>
//#include <stdio.h>

#pragma warning(push)
#pragma warning(disable:4840)
namespace StrUtil {

    template<class T>
    auto StringForward(T&& arg) {
        if constexpr (std::is_same_v<std::string&&, std::remove_cv<std::remove_reference<T>>>) {
            return arg.c_str();
        } else {
            return std::forward<T>(arg);
        }
    }

    template<class... Args>
    std::string _FormatImpl(const std::string fmt, Args&&... args) {
        std::unique_ptr<char[]> buffer(new char[2048]);
        _snprintf_s(buffer.get(), 2048, _TRUNCATE, fmt.c_str(), std::forward<Args>(args)...);
        return std::string(buffer.get());
    }

    template<class... Args>
    std::string Format(const std::string fmt, Args&&... args) {
        return _FormatImpl(fmt, StringForward(args)...);
    }
}
#pragma warning(pop)
#endif // FORMAT_H