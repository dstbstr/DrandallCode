#pragma once

#include <utility>

namespace LambdaUtils {
    using expander = int[];

    template<typename T, typename... Args>
    constexpr inline void ForEach(T toCall, Args&&... args) {
        (void)expander{0, (void(toCall(std::forward<Args>(args))), 0)...};
    }

} // namespace LambdaUtils