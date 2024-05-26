#pragma once

#include <memory>
#include <string>
#include <type_traits>

#pragma warning(push)
#pragma warning(disable : 4840)
namespace StrUtil {
    template<class T>
    auto StringForward(T&& arg) {
        if constexpr (std::is_same_v<std::string, std::remove_cv_t<std::remove_reference_t<T>>>) {
            return std::forward<T>(arg).c_str();
        } else {
            return std::forward<T>(arg);
        }
    }

    namespace detail {
        template<class... Args>
        std::string _FormatImpl(const std::string fmt, Args&&... args) {
            std::unique_ptr<char[]> buffer(new char[2048]);
            _snprintf_s(buffer.get(), 2048, _TRUNCATE, fmt.c_str(), std::forward<Args>(args)...);
            return std::string(buffer.get());
        }
    }
    template<class... Args>
    std::string Format(const std::string fmt, Args&&... args) {
        return detail::_FormatImpl(fmt, StringForward(std::forward<Args>(args))...);
    }
} // namespace StrUtil
#pragma warning(pop)