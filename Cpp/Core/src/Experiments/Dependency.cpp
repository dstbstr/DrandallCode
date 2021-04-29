#include "Experiments/Dependency.h"

namespace Dependency {
    std::string ToString(Name name) {
        switch(name) {
        case Name::Invalid: return "INVALID";
        case Name::Zero: return "Zero";
        case Name::One: return "One";
        case Name::Two: return "Two";
        case Name::Three: return "Three";
        case Name::Four: return "Four";
        case Name::Five: return "Five";
        default: throw "Unknown name";
        }
    }

    void LoadingDependency::Load() {
        if(CurrentState != LoadingState::NotLoaded) {
            throw "Called load from invalid state";
        }

        CurrentState = LoadingState::Loading;
    }

    void LoadingDependency::Update() {
        if(CurrentState == LoadingState::Loading) {
            CurrentState = OnUpdate();
        }
    }

    void LoadingDependency::Reset() {
        if(CurrentState != LoadingState::Done) {
            CurrentState = LoadingState::NotLoaded;
            return;
        }

        if(OnReset) {
            CurrentState = OnReset();
        }
    }
} // namespace Dependency