#ifndef TERNARY_SINGER_H
#define TERNARY_SINGER_H

#include "Singers/IBirthdaySinger.h"
#include <string>

class TernarySinger : public IBirthdaySinger {
    public:
    std::string Sing(std::string name) override;
};

#endif // TERNARY_SINGER_H