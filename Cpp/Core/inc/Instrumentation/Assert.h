#include "Instrumentation/Log.h"

#ifndef FINAL
#define ASSERT_FATAL(condition)       \
    do {                              \
        if(!(condition)) {            \
            LOG_CRITICAL(#condition); \
        }                             \
    } while(false)
#define ASSERT_FATAL_MSG(condition) \
    do {                            \
        if(!(condition)) {          \
            LOG_CRITICAL(message);  \
        }                           \
    } while(false)
#define ASSERT(condition)          \
    do {                           \
        if(!(condition)) {         \
            LOG_ERROR(#condition); \
        }                          \
    } while(false)
#define ASSERT_MSG(condition, message) \
    do {                               \
        if(!(condition)) {             \
            LOG_ERROR(message);        \
        }                              \
    } while(false)
#define ASSERT_FAIL(message) \
    do {                     \
        LOG_ERROR(message);  \
    } while(false)
// code to run only if asserts are enabled (normally to store a value to validate later in an assert)
#define ASSERT_CODE(code) code

#else
#define ASSERT_FATAL(condition)
#define ASSERT_FATAL_MSG(condition, message)
#define ASSERT(condition)
#define ASSERT_MSG(condition, message)
#define ASSERT_FAIL(message)
#define ASSERT_CODE(code)
#endif