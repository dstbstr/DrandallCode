#include "Core/Instrumentation/Logging.h"

#include "Core/DesignPatterns/PubSub.h"
#include "Core/DesignPatterns/ServiceLocator.h"

namespace {
    struct Store {
        void Flush() {
            auto copy = m_Logs;
            m_Logs.clear();

            if(auto* pubSub = ServiceLocator::Get().Get<PubSub<Log::Entry>>()) {
                for(const auto& log: copy) {
                    pubSub->Publish(log);
                }
            }
        }

        void AddMessage(const Log::Entry& entry) {
            //m_Logs.push_back(entry);
            if(auto* listener = ServiceLocator::Get().Get<PubSub<Log::Entry>>()) {
				listener->Publish(entry);
			}
        }

        void WriteImmediate(const Log::Entry& entry) {
            if(auto* listener = ServiceLocator::Get().Get<PubSub<Log::Entry>>()) {
                listener->Publish(entry);
            }
        }

        private:
            std::vector<Log::Entry> m_Logs{};
        };

    Store store;
    }

    namespace Log {
        void Flush() {
            store.Flush();
        }

#ifndef FINAL
        void Debug(const std::string& message, std::source_location loc) {
            store.AddMessage({Level::Debug, message, Debug::Context(loc)});
        }
        void Info(const std::string& message, std::source_location loc) {
            store.AddMessage({Level::Info, message, Debug::Context(loc)});
        }
        void Warn(const std::string& message, std::source_location loc) {
            store.AddMessage({Level::Warning, message, Debug::Context(loc)});
        }
        void Error(const std::string& message, std::source_location loc) {
            store.WriteImmediate({Level::Error, message, Debug::Context(loc)});
        }
#else
        void Debug(const std::string& message, std::source_location loc) {}
        void Info(const std::string& message, std::source_location loc) {}
        void Warn(const std::string& message, std::source_location loc) {}
        void Error(const std::string& message, std::source_location loc) {}
#endif
    }