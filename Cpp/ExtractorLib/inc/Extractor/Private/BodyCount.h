#pragma once

#include "Core/Platform/Types.h"

#include <iostream>
#include <string>

namespace Extractor {
    //what can I say, it was too tempting
    namespace BodyCount {
        u32 GetBodyCount(const std::string& line, std::istream& stream);
    }
}