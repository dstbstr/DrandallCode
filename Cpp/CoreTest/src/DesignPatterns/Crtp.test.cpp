#include "TestCommon.h"

#include "Core/DesignPatterns/Crtp.h"

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

class ByCrtp
    : public Square<ByCrtp>
    , public Scale<ByCrtp> {
public:
    double GetValue() const {
        return m_Value;
    }
    void SetValue(double val) {
        m_Value = val;
    }

private:
    double m_Value{0};
};

TEST(Crtp, SimpleTest) {
    ByCrtp test;

    test.SetValue(3);
    test.square();

    ASSERT_EQ(9.0, test.GetValue());

    test.scale(2.0);

    ASSERT_EQ(18.0, test.GetValue());
}
