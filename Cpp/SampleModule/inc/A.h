#ifndef __A_H__
#define __A_H__

/*
//#include "B.h"
struct A {
    static void DoAThing() { }

    template<typename T>
    void DoStuff() {
        B::DoBThing();
    }
};
*/

#include "C.h"

struct A {
    template<typename T>
    void DoStuff() {
        DoBThing();
    }
};

#endif // __A_H__