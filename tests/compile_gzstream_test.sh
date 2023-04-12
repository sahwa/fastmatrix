#!/bin/bash

g++ minimal_gzstream.cpp -I /usr/include/ -L ../include/gzstream/ -l gzstream -lm -lz -o gzstream_test
