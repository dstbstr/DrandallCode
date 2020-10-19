#include <string>
#include <iostream>
#include "HappyBirthday.h"

void HappyBirthday::Sing(std::string Name)
{
    std::string EveryLine = "Happy Birthday ";
    std::string Generic = EveryLine + "to you\n";
    std::string Specific = EveryLine + "dear " + Name + "\n";

    for (int i = 0;i < 4;i++)
        i != 2 ?  std::cout << Generic : std::cout << Specific;
}

void HappyBirthday::Usage(std::string Error)
{
    std::cout << "usage: " + Error + " Name\n";
    this->Retn = -1;
}

int HappyBirthday::Return()
{
    return(this->Retn);
}
