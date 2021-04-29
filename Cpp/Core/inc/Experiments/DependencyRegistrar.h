#ifndef __DEPENDENCYREGISTRAR_H__
#define __DEPENDENCYREGISTRAR_H__

#include <unordered_map>

namespace Dependency {
    enum Name : uint8_t;
    class LoadingDependency;

    namespace Registrar {
        // void Register(const Dependency::LoadingDependency& dependency);
        void Register(const Dependency::LoadingDependency& dependency);
        LoadingDependency& GetDependencyByName(Dependency::Name name);
        LoadingDependency* TryGetDependencyByName(Dependency::Name name);
        void ClearForTests();
    } // namespace Registrar
} // namespace Dependency
#endif // __DEPENDENCYREGISTRAR_H__