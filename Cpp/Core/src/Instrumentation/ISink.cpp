#include "Core/Instrumentation/ISink.h"
#include "Core/DesignPatterns/PubSub.h"

namespace Log {
	Filter& Filter::WithLevel(Level level) {
		m_Level = level;
		return *this;
	}
	Filter& Filter::WithFilePattern(const std::regex& pattern) {
		m_FilePattern = pattern;
		return *this;
	}

	Filter& Filter::WithCustomMatcher(std::function<bool(const Entry&)> matcher) {
		m_CustomMatcher = matcher;
		return *this;
	}

	bool Filter::Matches(const Entry& entry) const {
		if (m_Level.has_value() && m_Level.value() > entry.LogLevel) {
			return false;
		}

		if (m_FilePattern.has_value() && !std::regex_match(entry.Context.FileName, m_FilePattern.value())) {
			return false;
		}
		if (m_CustomMatcher.has_value() && !m_CustomMatcher.value()(entry)) {
			return false;
		}
		return true;
	}

	ISink::ISink(Filter filter)
		: m_Filter(filter)
	{
		m_PubSub = ServiceLocator::Get().Get<PubSub<Entry>>();
		if (!m_PubSub) return;

		m_Handle = m_PubSub->Subscribe([this](const Entry& entry) {
			if (m_Filter.Matches(entry)) {
				Write(entry);
			}
			});
	}

	ISink::~ISink() {
		if (m_Handle != std::numeric_limits<size_t>::max()) {
			m_PubSub->Unsubscribe(m_Handle);
		}
	}
}