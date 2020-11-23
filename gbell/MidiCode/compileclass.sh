#!/bin/ksh

set -x

g++ -g -c convertgdbmidiclass.cpp
g++ -g -c libconvertgdbmidiclass.cpp
g++ -g -c classdefs.cpp
g++ -g -o convertgdbmidiclass convertgdbmidiclass.o libconvertgdbmidiclass.o classdefs.o
if [ $? -eq 0 ]
then
    rm *.o
fi
