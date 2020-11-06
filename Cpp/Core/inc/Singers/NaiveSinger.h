#ifndef NAIVE_SINGER_H
#define NAIVE_SINGER_H

#include "Singers/IBirthdaySinger.h"
#include <string>

class NaiveSinger : public IBirthdaySinger {
    public:
    std::string Sing(std::string name) override;
};

#endif //NAIVE_SINGER_H