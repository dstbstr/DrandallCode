#ifndef __CONSTEXPRCOUNTER_H__
#define __CONSTEXPRCOUNTER_H__

namespace ConstexprCounter {
    template<typename T, int N>
    struct Flag {
        friend constexpr int adl_flag(Flag<T, N>);
    };

    template<typename T, int N>
    struct Writer {
        friend constexpr int adl_flag(Flag<T, N>) {
            return N;
        }

        static constexpr int value = N;
    };

    template<typename T, int N, class = char[noexcept(adl_flag(Flag<T, N>())) ? +1 : -1]>
    int constexpr Reader(int, Flag<T, N>) {
        return N;
    }

    template<typename T, int N>
    int constexpr Reader(float, Flag<T, N>, int R = Reader<T>(0, Flag<T, N - 1>())) {
        return R;
    }

    template<typename T>
    int constexpr Reader(float, Flag<T, 0>) {
        return 0;
    }

    template<typename T, int N = 1, int C = Reader<T>(0, Flag<T, 256>())> // 256 represents the max count for a counter
    int constexpr Next(int R = Writer<T, C + N>::value) {
        return R - 1;
    }

    template<typename T, int N = Reader<T>(0, Flag<T, 256>())>
    int constexpr Count() {
        return N;
    }
} // namespace ConstexprCounter

#endif // __CONSTEXPRCOUNTER_H__