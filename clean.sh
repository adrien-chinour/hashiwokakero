#! /bin/bash

make clean
rm -rf CMakeFiles
rm -f CMakeCache.txt cmake_install.cmake Makefile

rm -f src/solver/*~
rm -f src/gui/*~
rm -f src/core/*~
rm -f src/text/*~
rm -f *~
rm -f src/solver/*#*
rm -f src/gui/*#*
rm -f src/core/*#*
rm -f src/text/*#*
