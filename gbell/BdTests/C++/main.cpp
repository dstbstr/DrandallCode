#include <iostream>
#include <string>
#include "happy.h"

int
main(int argc,char *argv[])
{
    happy bdperson;

    argc == 2 ? bdperson.sing(argv[1]) : bdperson.usage(argv[0]);
    return(bdperson.ret());
}
