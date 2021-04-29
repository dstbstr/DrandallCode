#include "Experiments/DependencyTree.h"

#include "Experiments/Dependency.h"
#include "Experiments/DependencyRegistrar.h"
#include "Utilities/StringUtils.h"

#include <algorithm>

namespace {
    using Tree = std::unordered_map<Dependency::Name, Dependency::Bits>;
    using Kvp = std::pair<Dependency::Name, Dependency::Bits>;

    std::vector<Dependency::LoadingDependency*> ToDependencies(const Tree& tree) {
        std::vector<Dependency::LoadingDependency*> result{};
        std::transform(tree.begin(), tree.end(), std::back_inserter(result), [](Kvp kvp) { return &Dependency::Registrar::GetDependencyByName(kvp.first); });

        return result;
    }

    void UpdateLoadedBits(const Tree& tree, Dependency::Bits& loadedBits) {
        for(const auto& kvp: tree) {
            if(loadedBits[kvp.first]) {
                continue;
            }

            const auto& dep = Dependency::Registrar::GetDependencyByName(kvp.first);
            if(dep.CurrentState == Dependency::LoadingState::Done) {
                loadedBits.set(dep.MyName);
            }
        }
    }

    std::vector<Dependency::LoadingDependency*> FindReadyToLoad(Tree& tree, Dependency::Bits& loadedBits) {
        std::vector<Dependency::LoadingDependency*> result{};

        for(auto& kvp: tree) {
            kvp.second &= ~loadedBits;
            if(loadedBits[kvp.first] || kvp.second.any())
                continue;
            auto& dep = Dependency::Registrar::GetDependencyByName(kvp.first);
            if(dep.CurrentState == Dependency::LoadingState::NotLoaded) {
                result.push_back(&dep);
            }
        }

        return result;
    }

} // namespace

namespace Dependency {
    DependencyTree::DependencyTree(Dependency::Name root) {
        ConstructTree(root, std::vector<Dependency::Name>{});
    }

    std::vector<Dependency::LoadingDependency*> DependencyTree::ReadyToLoad() {
        UpdateLoadedBits(m_Tree, m_LoadedBits);
        return FindReadyToLoad(m_Tree, m_LoadedBits);
    }

    std::vector<Dependency::LoadingDependency*> DependencyTree::ToLoadInOrder() {
        std::vector<Kvp> asVec{m_Tree.begin(), m_Tree.end()};

        std::sort(asVec.begin(), asVec.end(), [](const Kvp& lhs, const Kvp& rhs) { return lhs.second.count() < rhs.second.count(); });
        return ToDependencies(m_Tree);
    }

    void DependencyTree::ConstructTree(Dependency::Name name, std::vector<Dependency::Name>& parents) {
        if(m_Tree.find(name) != m_Tree.end()) {
            return;
        }

        if(std::find(parents.begin(), parents.end(), name) != parents.end()) {
            parents.push_back(name);
            std::string cycle = StrUtil::JoinVec(" -> ", parents);
            throw "Cycle Detected: " + cycle;
        }

        parents.push_back(name);

        auto& dependency = Registrar::GetDependencyByName(name);
        if(dependency.MyName == Name::Invalid) {
            throw "Invalid dependency: " + name;
        }
        auto dependentBits = dependency.Dependencies;
        if(dependentBits.any()) {
            for(auto i = 0; i < Dependency::Name::COUNT; i++) {
                if(dependentBits[i]) {
                    auto name = static_cast<Dependency::Name>(i);
                    ConstructTree(name, parents);
                    dependentBits |= m_Tree[name];
                }
            }
        }

        m_Tree[name] = dependentBits;
        parents.pop_back();
    }
} // namespace Dependency