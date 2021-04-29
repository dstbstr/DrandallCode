#include "Experiments/DependencyTree.h"

#include "Experiments/Dependency.h"
#include "TestCommon.h"

namespace {
    Dependency::LoadingState Update() {
        return Dependency::LoadingState::Done;
    }

    Dependency::LoadingState Reset() {
        return Dependency::LoadingState::Done;
    }
} // namespace

namespace Dependency {
    struct DependencyTreeTest : public ::testing::Test {
        LoadingDependency zero{Name::Zero, Update, Reset};
        LoadingDependency one{Name::One, Update, Reset, Name::Zero};
        LoadingDependency two{Name::Two, Update, Reset, Name::Zero, Name::One};

        void TearDown() override {
            Dependency::Registrar::ClearForTests();
        }

        bool FirstMatchesExpected(Dependency::Name expected, std::vector<Dependency::LoadingDependency*>& toLoad) {
            // registration is done by value, not by reference, so we can't simply compare 'zero'
            return &Registrar::GetDependencyByName(expected) == toLoad[0];
        }

        void LoadDependencies(std::vector<Dependency::LoadingDependency*>& toLoad) {
            for(auto dep: toLoad) {
                dep->Load();
                dep->Update();
            }
        }
    };

    TEST_F(DependencyTreeTest, EmptyDependencyTree_IsInvalid) {
        ASSERT_ANY_THROW(auto tree = DependencyTree(Name::Three));
    }

    TEST_F(DependencyTreeTest, FreeDependency_ReturnsTheDependency) {
        DependencyTree tree(Name::Zero);
        auto toLoad = tree.ReadyToLoad();

        ASSERT_EQ(1u, toLoad.size());
        ASSERT_TRUE(FirstMatchesExpected(Name::Zero, toLoad));
    }

    TEST_F(DependencyTreeTest, ReadyToLoad_WhenAllCompleted_ReturnsEmptyVector) {
        DependencyTree tree(Name::Zero);
        auto toLoad = tree.ReadyToLoad();
        LoadDependencies(toLoad);

        toLoad = tree.ReadyToLoad();
        ASSERT_TRUE(toLoad.empty());
    }

    TEST_F(DependencyTreeTest, ReadyToLoad_OnlyReturnsDependenciesWhichAreReady) {
        DependencyTree tree(Name::Two);
        auto toLoad = tree.ReadyToLoad();

        ASSERT_EQ(1u, toLoad.size());
        ASSERT_TRUE(FirstMatchesExpected(Name::Zero, toLoad));
    }

    TEST_F(DependencyTreeTest, ReadyToLoad_UpdatesStateOnEachCall) {
        DependencyTree tree(Name::Two);
        auto toLoad = tree.ReadyToLoad();

        LoadDependencies(toLoad);
        toLoad = tree.ReadyToLoad();

        ASSERT_EQ(1u, toLoad.size());
        ASSERT_TRUE(FirstMatchesExpected(Name::One, toLoad));

        LoadDependencies(toLoad);
        toLoad = tree.ReadyToLoad();

        ASSERT_EQ(1u, toLoad.size());
        ASSERT_TRUE(FirstMatchesExpected(Name::Two, toLoad));
    }

    TEST_F(DependencyTreeTest, ReadyToLoad_CanReturnMultipleEntries) {
        LoadingDependency three{Name::Three, Update, Reset, Name::Zero};
        LoadingDependency four{Name::Four, Update, Reset, Name::Zero};
        LoadingDependency five{Name::Five, Update, Reset, Name::Three, Name::Four};

        DependencyTree tree(Name::Five);
        auto toLoad = tree.ReadyToLoad();
        LoadDependencies(toLoad); // zero

        toLoad = tree.ReadyToLoad();
        ASSERT_EQ(2u, toLoad.size()); // Three and Four
    }

    TEST_F(DependencyTreeTest, DependencyTree_WithSimpleCyclicalDependency_Throws) {
        LoadingDependency d1{Name::Three, Update, Reset, Name::Four};
        LoadingDependency d2{Name::Four, Update, Reset, Name::Three};

        ASSERT_ANY_THROW(auto _ = DependencyTree(Name::Three));
    }

    TEST_F(DependencyTreeTest, DependencyTree_WithLongerCyclicalDependency_Throws) {
        LoadingDependency d1{Name::Three, Update, Reset, Name::Four};
        LoadingDependency d2{Name::Four, Update, Reset, Name::Five};
        LoadingDependency d3{Name::Five, Update, Reset, Name::Three};

        ASSERT_ANY_THROW(auto _ = DependencyTree(Name::Three));
    }

    TEST_F(DependencyTreeTest, ToLoadInOrder_ProducesListInOrder) {
        DependencyTree tree(Name::Two);
        auto toLoad = tree.ToLoadInOrder();

        ASSERT_EQ(3u, toLoad.size());
        ASSERT_EQ(Name::Zero, toLoad[0]->MyName);
        ASSERT_EQ(Name::One, toLoad[1]->MyName);
        ASSERT_EQ(Name::Two, toLoad[2]->MyName);
    }
} // namespace Dependency
