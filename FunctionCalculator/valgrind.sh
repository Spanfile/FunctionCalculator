#!/bin/bash
./build.sh
valgrind --leak-check=yes --track-origins=yes ./FunctionCalculator.out
