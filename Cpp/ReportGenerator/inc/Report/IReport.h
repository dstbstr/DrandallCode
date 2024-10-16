#pragma once

#include <iostream>

namespace Report {
    struct IReport {
        virtual ~IReport() = default;
        virtual void WriteReport(std::string fileName) const = 0;
    };
} // namespace Report