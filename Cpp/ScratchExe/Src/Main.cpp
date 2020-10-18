#include "Singers/IBirthdaySinger.h"
#include "Singers/NaiveSinger.h"

#include <iostream>

int main() {
    auto singer = std::unique_ptr<IBirthdaySinger>(new NaiveSinger());

    std::cout << "Who are we singing to?" << std::endl;
    std::string name;
    std::cin >> name;
    std::cout << std::endl;

    std::cout << singer->Sing("Lenore");
    return 0;
}