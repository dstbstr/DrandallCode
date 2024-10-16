#pragma once

#include "Todo/Todo.h"
#include "Todo/TodoSettings.h"

#include <vector>

namespace TodoLib {
    class TodoReader {
    public:
        TodoReader(TodoSettings settings, std::string filePath) : m_Settings(settings), m_FilePath(filePath), m_InputStream(nullptr) {}
        TodoReader(TodoSettings settings, std::shared_ptr<std::istream> inputStream) : m_Settings(settings), m_FilePath(""), m_InputStream(inputStream) {}
        TodoReader(const TodoReader& other) = delete;
        TodoReader& operator=(const TodoReader& other) = delete;

        std::vector<Todo> Execute() const;

    private:
        TodoSettings m_Settings;
        std::string m_FilePath;
        mutable std::shared_ptr<std::istream> m_InputStream;
    };
} // namespace TodoLib