#ifndef __DEPENDENCYTREE_H__
#define __DEPENDENCYTREE_H__

#include "Experiments/Dependency.h"

#include <bitset>

namespace Dependency {
    class LoadingDependency;

    class DependencyTree {
        std::unordered_map<Dependency::Name, Bits> m_Tree{};
        Bits m_LoadedBits{};

    public:
        DependencyTree(Dependency::Name root);
        std::vector<Dependency::LoadingDependency*> ReadyToLoad();
        std::vector<Dependency::LoadingDependency*> ToLoadInOrder();

    private:
        void ConstructTree(Dependency::Name bit, std::vector<Dependency::Name>& parents);
    };
} // namespace Dependency

#endif // __DEPENDENCYTREE_H__