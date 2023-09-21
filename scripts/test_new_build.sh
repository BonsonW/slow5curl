#! /bin/bash

make clean && make slow5_mt=1 -j && make test