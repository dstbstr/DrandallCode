#include <string>
#include <iostream>
#include "happy.h"

void happy::sing(std::string name) {
    int i = 4;

    while (i > 0)
        i-- != 2 ?  std::cout << "Happy Birthday to you\n" : std::cout << "Happy Birthday to " + name + "\n";
}

void happy::usage(std::string err) {
    std::cout << "usage: " + err + " Name\n";
    this->retn = -1;
}

int happy::ret() {
    return(this->retn);
}
