#include "Experiments/DependencyRegistrar.h"

#include "Experiments/Dependency.h"
#include "TestCommon.h"

namespace Dependency {

    LoadingState Update() {
        return LoadingState::Done;
    }
    LoadingState Reset() {
        return LoadingState::Done;
    }

    struct RegistrarTest : public ::testing::Test {
        void TearDown() override {
            Registrar::ClearForTests();
        }
    };

    TEST_F(RegistrarTest, HoldsDependenciesInContiguousMemory) {
        auto d1 = LoadingDependency(Name::Zero, Update, Reset);
        auto d2 = LoadingDependency(Name::One, Update, Reset);
        auto d3 = LoadingDependency(Name::Two, Update, Reset);

        auto& zero = Registrar::GetDependencyByName(Name::Zero);
        auto& one = Registrar::GetDependencyByName(Name::One);
        auto& two = Registrar::GetDependencyByName(Name::Two);

        auto sizeDelta = (&one) - (&zero);
        ASSERT_EQ(sizeDelta, (&two) - (&one));
    }
} // namespace Dependency