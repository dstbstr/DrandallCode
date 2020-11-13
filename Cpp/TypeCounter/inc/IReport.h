#ifndef __IREPORT_H__
#define __IREPORT_H__

#include <iostream>

namespace TypeCounter {

    struct IReport {
        virtual ~IReport() = default;
        virtual void PrintResultToStream(std::ostream& targetStream) const = 0;
    };
} // namespace TypeCounter
#endif // __IREPORT_H__