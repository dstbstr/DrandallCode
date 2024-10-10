#pragma once

#include "Core/Instrumentation/Logging.h"

#include <unordered_map>
#include <functional>

template<typename TEvent>
struct PubSub {
	~PubSub() {
		maxHandle = 0;
	}

	size_t Subscribe(std::function<void(const TEvent&)> subscriber) {
		auto handle = maxHandle++;
		subscribers.emplace(handle, subscriber);
		return handle;
	}

	void Alarm(std::function<void(const TEvent&)> onAlarm) {
		alarms.push_back(onAlarm);
	}

	void Unsubscribe(size_t handle) {
		subscribers.erase(handle);
	}

	void Publish(const TEvent& event) {
		for (auto& [handle, subscriber] : subscribers) {
			subscriber(event);
		}
		for (auto& alarm : alarms) {
			alarm(event);
		}
		alarms.clear();
	}

private:
	std::unordered_map<size_t, std::function<void(const TEvent&)>> subscribers{};
	std::vector<std::function<void(const TEvent&)>> alarms{};
	inline static size_t maxHandle = 0;
};