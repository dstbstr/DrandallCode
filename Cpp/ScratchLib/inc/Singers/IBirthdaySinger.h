#ifndef IBIRTHDAY_SINGER_H
#define IBIRTHDAY_SINGER_H

#include <string>

class IBirthdaySinger {
    public:
    virtual std::string Sing(std::string name) = 0;
    virtual ~IBirthdaySinger() = default;
};

#endif //IBIRTHDAY_SINGER_H
