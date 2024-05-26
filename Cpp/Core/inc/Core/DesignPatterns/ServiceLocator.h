#pragma once

#include <memory>
#include <unordered_map>

struct ServiceLocator {
    static ServiceLocator& Get() {
        static ServiceLocator instance{};
		return instance;
	}

    ~ServiceLocator() {
        services.clear();
    }

    ServiceLocator(const ServiceLocator&) = delete;
    ServiceLocator& operator=(const ServiceLocator&) = delete;

    template<typename T, typename... Args>
    void Set(Args&&... args) {
        SetThisAsThat<T, T, Args...>(std::forward<Args>(args)...);
	}

    template<typename TThis, typename TThat, typename... Args>
    void SetThisAsThat(Args&&... args) {
        auto id = TypeId<TThat>();
        if(services.contains(id)) abort();

        services[id] = std::make_shared<TThis>(std::forward<Args>(args)...);
    }

    template<typename T>
    void Reset() {
        services.erase(TypeId<T>());
	}

    void ResetAll() {
        services.clear();
    }

    template<typename T>
    bool IsSet() const {
        return services.contains(TypeId<T>());
    }

    template<typename T>
    T* Get() const {
        auto id = TypeId<T>();
        if (services.contains(id)) {
            return static_cast<T*>(services.at(id).get());
        }

        return nullptr;
    }

    template<typename T>
    T& GetRequired() const {
        return *static_cast<T*>(services.at(TypeId<T>()).get());
    }

    template<typename T, typename... Args>
    T& GetOrCreate(Args&&... args) {
        auto id = TypeId<T>();
        if (!services.contains(id)) {
            Set<T>(std::forward<Args>(args)...);
        }

		return *static_cast<T*>(services.at(id).get());
    }

    template<typename T, typename... Args>
    void CreateIfMissing(Args&&... args) {
        if (!services.contains(TypeId<T>())) {
            Set<T>(std::forward<Args>(args)...);
        }
    }

private:
    ServiceLocator() = default;

    // must be shared_ptr because unique_ptr can't delete void*
    std::unordered_map<size_t, std::shared_ptr<void>> services;

    template<typename T>
    size_t TypeId() const {
        static size_t index = maxIndex++;
        return index;
    }

    inline static size_t maxIndex = 0;
};