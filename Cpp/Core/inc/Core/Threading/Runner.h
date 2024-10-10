#pragma once

#include "Core/Platform/Types.h"

/*
* There are two aspects to the Runner, the runner itself, and its tasks
* The runner boils down to the following bits
* 
* std::vector<std::unique_ptr<IRunnable<ReturnType>>> tasks;
* std::vector<ReturnType> result = Runner::Get().RunAll(ExpectedRunTime::Seconds, tasks);
*
* Where ReturnType is whatever your tasks return.
* 
* The second part is the IRunnable.  Because the Runner does not know what arguments would go to the tasks
* The runner doesn't pass any, and your IRunnable should take all of its state in a constructor.
* 
* struct MyTask : IRunnable<std::string> {
*     MyTask(int i, std::string s) : Num(i), Str(s) {}
*     const std::string Execute() const override {
*         return s + std::to_string(i);
*     }
*     int Num {0};
*     std::string Str {};
* };
* 
* Constructing the tasks now looks like the following
* 
* for(auto i = 0; i < 10; i++) {
*     tasks.push_back(std::move(std::make_unique<MyTask>(i, "Hello ")));
* }
* 
* Alternatively, you can create a TaskQueue and push lambdas to it. (Currently only works for MSVC)
* 
* auto tasks = TaskQueue<std::string>{};
* for(auto i = 0; i < 10; i++) {
*     tasks.push([=]() { return "Hello " + std::to_string(i); });
* }
* std::vector<std::string> result = Runner::RunAll(tasks);
*/
namespace Threading {
    enum struct ExpectedRunTime : u8 { MICROSECONDS, MILLISECONDS, SECONDS };
}

#ifdef MSVC
#include "Core/Threading/Private/MsvcRunner.h"
using Runner = MsvcRunner;
#elif defined(CLANG)
#include "Core/Threading/Private/ClangRunner.h"
using Runner = ClangRunner;
#elif defined(GNU)
#include "Core/Threading/Private/GnuRunner.h"
using Runner = GnuRunner;
#else
#error "Unknown compiler"
#endif
