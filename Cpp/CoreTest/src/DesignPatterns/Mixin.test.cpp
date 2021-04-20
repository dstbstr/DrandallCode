#include "DesignPatterns/Crtp.h"
#include "TestCommon.h"

template<typename Derived>
class Square : public Crtp<Derived, Square<Derived>> {
public:
    Square() = default;
    void square() {
        auto& self = this->Self();
        self.SetValue(self.GetValue() * self.GetValue());
    }
};

template<typename Derived>
class Scale : public Crtp<Derived, Scale<Derived>> {
public:
    Scale() = default;
    void scale(double factor) {
        auto& self = this->Self();
        self.SetValue(self.GetValue() * factor);
    }
};

template<template<typename> typename... Mixins>
class MixinWith : public Mixins<MixinWith<Mixins...>>... {
public:
    double GetValue() const {
        return m_Value;
    }
    void SetValue(double val) {
        m_Value = val;
    }

private:
    double m_Value{0.0};
};

class ByMixin : public MixinWith<Square, Scale> {};

TEST(Mixin, SimpleTest) {
    auto test = MixinWith<Square, Scale>();

    test.SetValue(3);
    test.square();

    ASSERT_EQ(9.0, test.GetValue());

    test.scale(2.0);

    ASSERT_EQ(18.0, test.GetValue());
}