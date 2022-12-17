#ifndef __RUNNER_H__
#define __RUNNER_H__

#include "Platform/Types.h"

namespace Threading {
    enum struct ExpectedRunTime : u8 { MICROSECONDS, MILLISECONDS, SECONDS };
}

#ifdef MSVC
#include "Threading/Private/MsvcRunner.h"
using Runner = MsvcRunner;
#elif defined(CLANG)
#include "Threading/Private/ClangRunner.h"
using Runner = ClangRunner;
#elif defined(GNU)
#include "Threading/Private/GnuRunner.h"
using Runner = GnuRunner;
#else
#error "Unknown compiler"
#endif

#endif // __RUNNER_H__