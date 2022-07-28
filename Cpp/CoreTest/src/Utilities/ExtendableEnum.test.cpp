#include "Platform/ExtendableEnum.h"

#include "TestCommon.h"

#if !_HAS_CXX20
struct BaseEnum : ExtendableEnum<BaseEnum> {
    static const BaseEnum Zero;
    static const BaseEnum One;
    static const BaseEnum Two;
    static const BaseEnum All[];
};

inline constexpr BaseEnum BaseEnum::Zero{};
inline constexpr BaseEnum BaseEnum::One{};
inline constexpr BaseEnum BaseEnum::Two{};
inline constexpr BaseEnum BaseEnum::All[] = {BaseEnum::Zero, BaseEnum::One, BaseEnum::Two};

static_assert(EXTENDABLE_ENUM_COUNT(BaseEnum) == 3, "Count before derived is only base");
struct DerivedEnum : public BaseEnum {
    static const DerivedEnum Three;
    static const DerivedEnum Four;
    static const DerivedEnum Five;
    static const DerivedEnum All[];
};

inline constexpr DerivedEnum DerivedEnum::Three{};
inline constexpr DerivedEnum DerivedEnum::Four{};
inline constexpr DerivedEnum DerivedEnum::Five{};
inline constexpr DerivedEnum DerivedEnum::All[] = {DerivedEnum::Three, DerivedEnum::Four, DerivedEnum::Five};

static_assert(BaseEnum::Zero.id == 0, "index is zero based");
static_assert(BaseEnum::Zero.id == BaseEnum::One.id - 1, "Ids are sequential");
static_assert(BaseEnum::Zero.id != DerivedEnum::Three.id, "Derived enum can continue the count");

static_assert(EXTENDABLE_ENUM_COUNT(BaseEnum) == 6, "Count should be 6");

bool TestFunction(BaseEnum) {
    return true;
}

TEST(ExtendableEnum, DerivedEnumCanBePassedAsBase) {
    ASSERT_TRUE(TestFunction(BaseEnum::One));
    ASSERT_TRUE(TestFunction(DerivedEnum::Three));
}

TEST(ExtendableEnum, DerivedEnumIsSameSizeAsBase) {
    ASSERT_TRUE(sizeof(BaseEnum) == sizeof(DerivedEnum));
}

TEST(ExtendableEnum, EnumCanBeEnumerated) {
    int counter = 0;
    for(BaseEnum e: BaseEnum::All) {
        counter++;
    }

    ASSERT_EQ(3, counter);
}

TEST(ExtendableEnum, EnumCanBeUsedInSwitch) {
    BaseEnum e = BaseEnum::Two;

    switch(e) {
    case BaseEnum::Zero: FAIL() << "Expected Two, but got Zero";
    case BaseEnum::One: FAIL() << "Expected Two, but got One";
    case BaseEnum::Two: ASSERT_TRUE(true); break;
    default: FAIL() << "Expected Two, but got default";
    }
}
#endif