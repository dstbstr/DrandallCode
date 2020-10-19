#include "Singers/IBirthdaySinger.h"
#include "Singers/NaiveSinger.h"

#include <iostream>
#include <string>

int main() {
    auto singer = std::unique_ptr<IBirthdaySinger>(new NaiveSinger());

    std::cout << "Who are we singing to?" << std::endl;
    std::string name;
    std::cin >> name;
    std::cout << std::endl;

    std::cout << singer->Sing(name) << std::endl;
    std::cin.ignore();
    std::cin.ignore(); //Need two of these, one to capture the newline in the cout apparently
    return 0;
}