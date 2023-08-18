#! /bin/bash

/install/cmake-3.23.0-rc2-linux-x86_64/bin/cmake -DCMAKE_INSTALL_PREFIX=~/install -S . -B build
cd build
make clean