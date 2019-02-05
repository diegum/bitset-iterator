#!/bin/sh

cppcheck --language=c++ --std=c++14 --enable=all --inline-suppr src/bitset_iter.h
