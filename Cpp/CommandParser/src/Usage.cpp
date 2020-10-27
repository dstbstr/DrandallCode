#include "CommandParser/Usage.h"

namespace CommandParser {
    void Usage::Print(std::ostream targetStream) const {
        targetStream << m_Description;
    }
} // namespace CommandParser
