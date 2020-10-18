#include "Singers/TernarySinger.h"
#include "Utilities/StringUtilities.h"

std::string TernarySinger::Sing(std::string name) {
    int i = 4;
    std::string song = "";

    while (i > 0) {
        i-- != 2 ? song += "Happy birthday to you.  " :
            song += "Happy birthday dear " + name + ".  ";
    }
    
    return StrUtil::trim(song);
}