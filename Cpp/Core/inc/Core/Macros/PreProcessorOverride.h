#pragma once

#define EXPAND(...) __VA_ARGS__
#define VA_NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define VA_NARGS(...) EXPAND(VA_NARGS_IMPL(X, __VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0))
#define VARARG_IMPL2(base, count, ...) EXPAND(base##count(__VA_ARGS__))
#define VARARG_IMPL(base, count, ...) VARARG_IMPL2(base, count, __VA_ARGS__)
#define MACRO_OVERRIDE(base, ...) VARARG_IMPL(base, VA_NARGS(__VA_ARGS__), __VA_ARGS__)

// HOW TO USE

// #define _MY_MACRO_1(arg1) SINGLE_ARG_DEF(arg1)
// #define _MY_MACRO_2(arg1, arg2) TWO_ARG_DEF(arg1, arg2)
// #define _MY_MACRO_3(arg1, arg2, arg3) THREE_ARG_DEF(arg1, arg2, arg3)
//
// #define MY_MACRO(...) MACRO_OVERRIDE(_MY_MACRO_, __VA_ARGS__)

// NOTE: This does not support a zero arg version