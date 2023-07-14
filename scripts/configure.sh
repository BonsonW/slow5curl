#! /bin/bash

cmake -DCMAKE_INSTALL_PREFIX=~/install -S . -B build
cd build
make clean