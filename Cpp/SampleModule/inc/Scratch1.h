#pragma once

template<typename T, size_t N>
char (*NumberOf_(T (&)[N]))[N];

#define lengthof(A) (sizeof(*NumberOf_(A)))

template<typename T>
constexpr size_t LengthOf(T) {
    return (sizeof(*NumberOf_(T)));
}
