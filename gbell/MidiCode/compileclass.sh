#!/bin/ksh

set -x

rm *.o
g++ -g -c convertgdbmidiclass.cpp
if [ $? -ne 0 ]
then
    exit 1
fi
g++ -g -c libconvertgdbmidiclass.cpp
if [ $? -ne 0 ]
then
    exit 1 
fi
g++ -g -c classdefs.cpp
if [ $? -ne 0 ]
then
    exit 1 
fi
g++ -g -c returnstatus.cpp
if [ $? -ne 0 ]
then
    exit 1 
fi
g++ -g -c parse.cpp
if [ $? -ne 0 ]
then
    exit 1 
fi
g++ -g -c utils.cpp
if [ $? -ne 0 ]
then
    exit 1 
fi
g++ -g -c midiout.cpp
if [ $? -ne o ]
then
    exit 1
fi
g++ -g -o convertgdbmidiclass convertgdbmidiclass.o libconvertgdbmidiclass.o classdefs.o returnstatus.o parse.o utils.cpp midiout.cpp
if [ $? -eq 0 ]
then
    rm *.o
fi
