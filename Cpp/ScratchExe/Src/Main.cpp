#include "Singers/IBirthdaySinger.h"
#include "Singers/NaiveSinger.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // construct an argument parser with argc and argv
    // pass the root directory and the recurse option to a directory navigator
    // fan out the files to find include directives in each file
    // construct including and included maps
    auto singer = std::unique_ptr<IBirthdaySinger>(new NaiveSinger());

    std::cout << "Who are we singing to?" << std::endl;
    std::string name;
    std::cin >> name;
    std::cout << std::endl;

    std::cout << singer->Sing(name) << std::endl;
    std::cin.ignore();
    std::cin.ignore(); // Need two of these, one to capture the newline in the cout apparently
    return 0;
}