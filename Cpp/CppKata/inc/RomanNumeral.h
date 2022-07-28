#ifndef __ROMANNUMERAL_H__
#define __ROMANNUMERAL_H__

#include <string>

namespace RomanNumeral {
    std::string ToRoman(uint32_t input);
    uint32_t FromRoman(std::string input);
} // namespace RomanNumeral

#endif // __ROMANNUMERAL_H__