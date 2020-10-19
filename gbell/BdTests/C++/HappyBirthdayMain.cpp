#include <iostream>
#include <string>
#include "HappyBirthday.h"

int
main(int argc,char *argv[])
{
    HappyBirthday Person;

    if (argc != 2)
        Person.Usage(argv[0]);
    else
        Person.Sing(argv[1]);
    return(Person.Return());
}
