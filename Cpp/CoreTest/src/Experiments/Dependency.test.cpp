#include "Experiments/Dependency.h"

#include "Experiments/DependencyRegistrar.h"
#include "TestCommon.h"

namespace {
    bool resetCalled = false;

    Dependency::LoadingState Update() {
        return Dependency::LoadingState::Done;
    }

    Dependency::LoadingState ResetNeeded() {
        resetCalled = true;
        return Dependency::LoadingState::NotLoaded;
    }

    Dependency::LoadingState ResetNotNeeded() {
        resetCalled = true;
        return Dependency::LoadingState::Done;
    }
} // namespace

namespace Dependency {
    struct DependencyTest : public ::testing::Test {
        LoadingDependency noReset{Name::Zero, Update, ResetNotNeeded};
        LoadingDependency needsReset{Name::One, Update, ResetNeeded};

        void TearDown() override {
            Dependency::Registrar::ClearForTests();
            resetCalled = false;
        }
    };

    TEST_F(DependencyTest, CurrentState_BeforeLoad_IsNotLoaded) {
        ASSERT_EQ(noReset.CurrentState, LoadingState::NotLoaded);
    }

    TEST_F(DependencyTest, CurrentState_AfterLoad_IsLoading) {
        noReset.Load();
        ASSERT_EQ(noReset.CurrentState, LoadingState::Loading);
    }

    TEST_F(DependencyTest, CurrentState_AfterLoadCOmplete_IsDone) {
        noReset.Load();
        noReset.Update();

        ASSERT_EQ(noReset.CurrentState, LoadingState::Done);
    }

    TEST_F(DependencyTest, CurrentState_AfterLoadThenReset_NeedsReset_IsNotLoaded) {
        needsReset.Load();
        needsReset.Update();
        needsReset.Reset();

        ASSERT_EQ(needsReset.CurrentState, LoadingState::NotLoaded);
    }

    TEST_F(DependencyTest, CurrentState_AfterLoadThenReset_NoResetNeeded_IsDone) {
        noReset.Load();
        noReset.Update();
        noReset.Reset();

        ASSERT_EQ(noReset.CurrentState, LoadingState::Done);
    }

    TEST_F(DependencyTest, Reset_BeforeLoad_DoesNotInvokeResetMethod) {
        needsReset.Reset();

        ASSERT_FALSE(resetCalled);
    }

    TEST_F(DependencyTest, Reset_BeforeDone_DoesNotInvokeResetMethod) {
        needsReset.Load();
        needsReset.Reset();

        ASSERT_FALSE(resetCalled);
        ASSERT_EQ(needsReset.CurrentState, LoadingState::NotLoaded);
    }

    TEST_F(DependencyTest, Reset_AfterDone_InvokesResetMethod) {
        needsReset.Load();
        needsReset.Update();
        needsReset.Reset();

        ASSERT_TRUE(resetCalled);
    }

    TEST_F(DependencyTest, Construction_RegistersDependency_WithRegistrar) {
        auto& zero = Registrar::GetDependencyByName(Name::Zero);

        ASSERT_EQ(Name::Zero, zero.MyName);
        ASSERT_EQ(LoadingState::NotLoaded, zero.CurrentState);
    }

    TEST_F(DependencyTest, Dependency_IsSmall) {
        ASSERT_LE(sizeof(needsReset), 32ul);
    }

} // namespace Dependency
