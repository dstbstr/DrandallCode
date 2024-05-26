#include "Todo/TodoReader.h"

#include "Core/Utilities/FileUtils.h"
#include "Core/Utilities/PathUtils.h"
#include "Core/Utilities/StringUtils.h"

#include <filesystem>
#include <fstream>
#include <regex>

const std::regex TodoRegex(R"(\/[\/\*]\s*[^a-zA-Z]?TODO:?)");

namespace {
    void Parse(const std::string& line, const std::string& separator, Todo& outTodo) {
        auto parts = StrUtil::Split(line, separator);

        if(parts.size() > 0) {
            outTodo.Classification = std::string(parts[0]);
        }
        if(parts.size() > 1) {
            outTodo.Status = std::string(parts[1]);
        }
        if(parts.size() > 2) {
            outTodo.Notes = std::string(parts[2]);
            for(auto i = 3; i < parts.size(); i++) {
                outTodo.Notes += separator;
                outTodo.Notes += parts[i];
            }
        }
    }

    void PopulateRelevantLine(std::vector<Todo>& awaitingNextLine, std::vector<Todo>& result, const std::string& line) {
        if(awaitingNextLine.size() == 0)
            return;

        for(auto& todo: awaitingNextLine) {
            todo.RelevantLine = line;
        }

        result.insert(result.end(), awaitingNextLine.begin(), awaitingNextLine.end());
        awaitingNextLine.clear();
    }

    std::string GetSuffix(const std::smatch& match) {
        auto rest = match.suffix().str();
        std::size_t starSlashIdx;
        if((starSlashIdx = rest.find("*/")) != rest.npos) {
            rest = rest.substr(0, starSlashIdx);
        }

        return StrUtil::Trim(rest);
    }
} // namespace

namespace TodoLib {
    std::vector<Todo> TodoReader::Execute() const {
        std::vector<Todo> result{};
        std::string fileName = PathUtils::GetFileName(m_FilePath);

        if(!m_InputStream) {
            m_InputStream = FileUtils::OpenForRead(m_FilePath);
        }

        static const std::regex regex = [&]() {
            if(m_Settings.TodoStart.length() > 0) {
                return std::regex(R"(\/[\/\*]\s*)" + m_Settings.TodoStart);
            } else {
                return TodoRegex;
            }
        }();

        std::string line;
        std::smatch match;
        u64 lineNumber = 0;
        std::vector<Todo> awaitingNextLine{};

        while(std::getline(*m_InputStream, line)) {
            lineNumber++;
            if(std::regex_search(line, match, regex)) {
                Todo todo{};
                auto suffix = GetSuffix(match);

                if(suffix.find(m_Settings.TodoSeparator) == std::string::npos) {
                    todo.Notes = suffix;
                } else {
                    Parse(suffix, m_Settings.TodoSeparator, todo);
                }
                todo.FilePath = m_FilePath;
                todo.FileName = fileName;
                todo.LineNumber = lineNumber;

                auto prefix = StrUtil::Trim(match.prefix());
                if(prefix.length() > 0) {
                    todo.RelevantLine = prefix;
                    result.push_back(todo);
                    PopulateRelevantLine(awaitingNextLine, result, prefix);
                } else {
                    awaitingNextLine.push_back(todo);
                }
            } else {
                auto relevantLine = StrUtil::Trim(line);
                PopulateRelevantLine(awaitingNextLine, result, relevantLine);
            }
        }

        for(auto& todo: awaitingNextLine) {
            result.push_back(todo);
        }

        return result;
    }
} // namespace TodoLib