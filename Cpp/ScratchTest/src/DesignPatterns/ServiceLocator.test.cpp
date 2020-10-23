#include "DesignPatterns/ServiceLocator.h"
#include "TestCommon.h"

#include <string>

class TestService {
public:
    explicit TestService(const std::string name) : m_name(name) {}

    const std::string GetName() {
        return m_name;
    }

private:
    const std::string m_name;
};

TEST(ServiceLocator, ServiceLocator_CanRegister_AndReturn) {
    std::string testString = "TestString";

    auto service = std::make_unique<TestService>(testString);

    ServiceLocator<TestService>::Set(*service);

    TestService result = ServiceLocator<TestService>::Get();
    ASSERT_EQ(testString, result.GetName());
}