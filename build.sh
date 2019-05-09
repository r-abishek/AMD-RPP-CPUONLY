#!/bin/bash

rm -rvf build
mkdir build
cd build
cmake ..
make
./rppi_gaussianBlur
