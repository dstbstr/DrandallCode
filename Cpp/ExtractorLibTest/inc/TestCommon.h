#pragma once

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::A;
using ::testing::An;
using ::testing::EndsWith;
using ::testing::Matches;
using ::testing::Not;
using ::testing::StartsWith;

#define CAPTURE_EXCEPTION_E(code, exceptionType)                                                 \
    std::exception_ptr exceptionToSave;                                                          \
    try {                                                                                        \
        code;                                                                                    \
        FAIL() << #code << "did not throw any exceptions";                                       \
    } catch(const exceptionType&) { exceptionToSave = std::current_exception(); } catch(...) {   \
        FAIL() << #code << "Threw an exception of the wrong type.  Expected " << #exceptionType; \
    }                                                                                            \
    try {                                                                                        \
        std::rethrow_exception(exceptionToSave);                                                 \
    } catch(const exceptionType& e)
