#include "Core/Instrumentation/Logging.h"
#include "Core/DesignPatterns/ServiceLocator.h"
#include "Core/DesignPatterns/PubSub.h"

namespace {
	struct Store {
		static Store& Get() {
			static Store store;
			return store;
		}

		static void Initialize() {			
			ServiceLocator::Get().CreateIfMissing<PubSub<Log::Entry>>();
		}

		void Flush() {
			auto* pubSub = ServiceLocator::Get().Get<PubSub<Log::Entry>>();
			if (!pubSub) return;

			for (const auto& log : m_Logs) {
				pubSub->Publish(log);
			}

			m_Logs.clear();
		}

		void AddMessage(const Log::Entry& entry) {
			m_Logs.push_back(entry);
		}

		void WriteImmediate(const Log::Entry& entry) {
			ServiceLocator::Get().GetRequired<PubSub<Log::Entry>>().Publish(entry);
		}

	private:
		Store() = default;
		std::vector<Log::Entry> m_Logs{};
	};
}

namespace Log {
	void Initialize() {
		Store::Initialize();
	}

	void Flush() {
		Store::Get().Flush();
	}

#ifndef FINAL
	void Info(const std::string& message, std::source_location loc) {
		Store::Get().AddMessage({ Level::Info, message, Debug::Context(loc) });
	}
	void Warn(const std::string& message, std::source_location loc) {
		Store::Get().AddMessage({ Level::Warning, message, Debug::Context(loc) });
	}
	void Error(const std::string& message, std::source_location loc) {
		Store::Get().WriteImmediate({ Level::Error, message, Debug::Context(loc) });
	}
#else
	void Info(const std::string& message, std::source_location loc) {}
	void Warn(const std::string& message, std::source_location loc) {}
	void Error(const std::string& message, std::source_location loc) {}
#endif
}