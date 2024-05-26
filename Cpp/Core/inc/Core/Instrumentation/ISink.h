#pragma once

#include "Core/DesignPatterns/ServiceLocator.h"
#include "Core/Instrumentation/Logging.h"

#include <optional>
#include <regex>
#include <functional>

template<typename T>
struct PubSub;

namespace Log {
	struct Filter {
		Filter& WithLevel(Level level);
		Filter& WithFilePattern(const std::regex& pattern);
		Filter& WithCustomMatcher(std::function<bool(const Entry&)> matcher);

		bool Matches(const Entry& entry) const;

	private:
		std::optional<Level> m_Level;
		std::optional<std::regex> m_FilePattern;
		std::optional<std::function<bool(const Entry&)>> m_CustomMatcher;
	};

	struct ISink {
		ISink(Filter filter);
		virtual ~ISink();

		virtual void Write(const Entry& entry) = 0;

	private:
		PubSub<Entry>* m_PubSub{ nullptr };
		size_t m_Handle{ std::numeric_limits<size_t>::max() };
		Filter m_Filter;
	};
}