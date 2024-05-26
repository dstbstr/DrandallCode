#pragma once
/*
Example of creating a capability for CRTP or Mixin

template<typename Derived>
class MyCapability : public Crtp<Derived, MyCapability<Derived>> {
public:
    void myMethod() {
        auto& self = this->Self();
        //do stuff with/to self
    }
};


Example of using the Capability through CRTP:
class MyClass : public MyCapability<MyClass> {
public:
    //methods used by MyCapability
}
*/
template<class T, template<typename> class CrtpType>
class Crtp {
public:
    T& Self() {
        return static_cast<T&>(*this);
    }
    const T& Self() const {
        return static_cast<const T&>(*this);
    }

private:
    Crtp() = default;
    friend CrtpType<T>;
};