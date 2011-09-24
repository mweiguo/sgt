#!bin/sh


if [ $1 = all ]
then
    echo "**********build all"
    cd renderer/renderer2d
    make 
    cd ../../unittest
    make
    cd ../../
elif [ $1 = clean ]
then
    echo "**********clean all"
    cd renderer/renderer2d
    make clean
    cd ../../unittest
    make clean
    cd ../../
elif [ $1 = install ]
then
    echo "**********install all"
    cd renderer/renderer2d
    make install
    cd ../../unittest
    make install
    cd ../../
elif [ $1 = runtest ]
then
    echo "**********run test"
    cd renderer/renderer2d
    make runtest
    cd ../../unittest
    make runtest
    cd ../../
fi