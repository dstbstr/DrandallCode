#include "Experiments/DependencyRegistrar.h"

#include "Experiments/Dependency.h"

#include <array>
#include <unordered_map>

namespace {
    /*
    template<class T, size_t N>
    constexpr std::array<T, N> MakeArray(T toRepeat) {
        std::vector<T> vec{};
        for(int i = 0; i < N; i++) {
            vec.push_back(toRepeat);
        }

        std::array<T, N> result;
        std::move(vec.begin(), N, result.begin());
        return result;
    }
    */

    // std::unordered_map<Dependency::Name, Dependency::LoadingDependency> knownDependencies{};
    // Dependency::LoadingDependency dependencies[Dependency::Name::COUNT];
    Dependency::LoadingDependency DefaultDependency{};
    std::array<Dependency::LoadingDependency, Dependency::Name::COUNT> dependencies; // = MakeArray<Dependency::LoadingDependency, Dependency::Name::COUNT>(Invalid);
    bool initialized = false;

} // namespace

namespace Dependency {
    namespace Registrar {
        void Register(const Dependency::LoadingDependency& dependency) {
            dependencies[dependency.MyName] = dependency;
            // knownDependencies.emplace(dependency.MyName, dependency);
        }

        LoadingDependency& GetDependencyByName(Dependency::Name name) {
            return dependencies[name];
            // return knownDependencies.at(name);
        }

        void ClearForTests() {
            // knownDependencies.clear();
            dependencies.fill(DefaultDependency);
        }
    } // namespace Registrar
} // namespace Dependency