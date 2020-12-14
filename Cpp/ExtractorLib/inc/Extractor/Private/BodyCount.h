#ifndef __BODYCOUNT_H__
#define __BODYCOUNT_H__

#include "Platform/Types.h"
#include <iostream>
#include <string>

namespace Extractor {
    //what can I say, it was too tempting
    namespace BodyCount {
        u32 GetBodyCount(const std::string& line, std::istream& stream);
    }
}
#endif // __BODYCOUNT_H__