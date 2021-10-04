#ifndef __IREPORT_H__
#define __IREPORT_H__

#include <iostream>

namespace Report {
    struct IReport {
        virtual ~IReport() = default;
        virtual void WriteReport(std::string fileName) const = 0;
    };
} // namespace Report
#endif // __IREPORT_H__