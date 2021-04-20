#ifndef __MIXIN_H__
#define __MIXIN_H__

/*
In order to create a mixin class:

template<template<typename> typename... Mixins>
class MyBaseClass : public Mixins<MyBaseClass<Mixins...>>... {
public:
    //methods needed by the possible mixins
private:
    //data needed by public methods
};

*/

#endif // __MIXIN_H__