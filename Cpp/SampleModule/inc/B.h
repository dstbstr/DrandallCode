#ifndef __B_H__
#define __B_H__

/*
#include "A.h"
struct B {
    static void DoBThing() {}

    template<typename T>
    void DoStuff() {
        A::DoAThing();
    }
};
*/

#include "C.h"

struct B {
    template<typename T>
    void DoStuff() {
        DoAThing();
    }
};
#endif // __B_H__