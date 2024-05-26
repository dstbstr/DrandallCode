#include "TestCommon.h"
#include "Core/DesignPatterns/ServiceLocator.h"

struct ServiceLocatorTest : public testing::Test {
    ServiceLocator& loc = ServiceLocator::Get();

    void TearDown() override {
        loc.Reset<int>();
    }
};

TEST_F(ServiceLocatorTest, IsSet_WithEmptyLocator_ReturnsFalse) {
    ASSERT_FALSE(loc.IsSet<int>());
}

TEST_F(ServiceLocatorTest, Get_WithEmptyLocator_ReturnsNullopt) {
    ASSERT_EQ(nullptr, loc.Get<int>());
}

TEST_F(ServiceLocatorTest, IsSet_AfterSetting_ReturnsTrue) {
    loc.Set<int>();

    ASSERT_TRUE(loc.IsSet<int>());
}

TEST_F(ServiceLocatorTest, Get_AfterSetting_ReturnsSetValue) {
    loc.Set<int>(42);

    auto* result = loc.Get<int>();
    ASSERT_NE(nullptr, result);
    ASSERT_EQ(42, *result);
}

TEST_F(ServiceLocatorTest, Get_AfterDifferentQueryAndSet_ReturnsSetValue) {
    loc.IsSet<long>();
    loc.Set<int>(42);

    auto result = loc.Get<int>();
    ASSERT_NE(nullptr, result);
    ASSERT_EQ(42, *result);
    ASSERT_FALSE(loc.IsSet<long>());
}

struct IFoo {
    virtual ~IFoo() = default;
    virtual int Get() const = 0;
};

struct FooImpl : public IFoo {
    FooImpl(int val) : val(val) {}
    int Get() const override {
		return val;
	}

private:
	int val;
};

TEST_F(ServiceLocatorTest, Get_WithInterface_ReturnsSetValue) {
    loc.SetThisAsThat<FooImpl, IFoo>(42);

	auto result = loc.Get<IFoo>();
	ASSERT_NE(nullptr, result);
	ASSERT_EQ(42, result->Get());
}