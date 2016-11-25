#!/bin/bash
./build.sh
valgrind --leak-check=yes ./FunctionCalculator.out
