#ifndef __DEPENDENCY_H__
#define __DEPENDENCY_H__

#include "Experiments/DependencyRegistrar.h"
#include "Utilities/LambdaUtils.h"

#include <bitset>
#include <functional>
#include <unordered_set>

namespace Dependency {
    enum Name : uint8_t { Invalid, Zero, One, Two, Three, Four, Five, COUNT };
    enum class LoadingState { NotLoaded, Loading, Done };
    using Bits = std::bitset<Name::COUNT>;

    std::string ToString(Name name);

    auto bitsetInitializer = [](auto&&... directDependencies) -> Bits {
        Bits result;
        LambdaUtils::ForEach([&result](Dependency::Name name) { result.set(name); }, directDependencies...);
        return result;
    };

    class LoadingDependency {
    public:
        /*const*/ Dependency::Name MyName;
        /*const*/ Bits Dependencies;

        using UpdateFun = Dependency::LoadingState (*)();
        /*const*/ UpdateFun OnUpdate;
        /*const*/ UpdateFun OnReset;

        Dependency::LoadingState CurrentState{LoadingState::NotLoaded};

    private:
    public:
        template<class... DirectDependencies>
        LoadingDependency(Dependency::Name name, UpdateFun onUpdate, UpdateFun onReset, DirectDependencies&&... directDependencies) : MyName(name)
                                                                                                                                    , OnUpdate(onUpdate)
                                                                                                                                    , OnReset(onReset)
                                                                                                                                    , Dependencies(bitsetInitializer(directDependencies...)) {
            Dependency::Registrar::Register(*this);
        }

        LoadingDependency() : MyName(Name::Invalid), OnUpdate(nullptr), OnReset(nullptr), Dependencies(Bits{}) {}
        void Load();
        void Update();
        void Reset();
    };

} // namespace Dependency
#endif // __DEPENDENCY_H__