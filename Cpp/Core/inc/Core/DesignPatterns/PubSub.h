#pragma once

#include <unordered_map>
#include <functional>

template<typename TEvent>
struct PubSub {
	~PubSub() {
		subscribers.clear();
	}

	size_t Subscribe(std::function<void(const TEvent&)> subscriber) {
		auto handle = maxHandle++;
		subscribers.emplace(handle, subscriber);
		return handle;
	}

	void Unsubscribe(size_t handle) {
		subscribers.erase(handle);
	}

	void Publish(const TEvent& event) {
		for (auto& [handle, subscriber]: subscribers) {
			subscriber(event);
		}
	}

private:
	std::unordered_map<size_t, std::function<void(const TEvent&)>> subscribers{};
	inline static size_t maxHandle = 0;
};