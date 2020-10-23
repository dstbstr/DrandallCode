#ifndef LAMBDAUTILS_H
#define LAMBDAUTILS_H

namespace LambdaUtils {
    using expander = int[];

    template<typename T, typename... Args>
    inline void ForEach(T toCall, Args&&... args) {
        (void)expander{0, (void(toCall(std::forward<Args>(args))), 0)...};
    }

} // namespace LambdaUtils
#endif // LAMDAUTILS_H