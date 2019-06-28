#!/bin/bash

rm -rvf build
mkdir build
cd build
#cmake ..
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j12
