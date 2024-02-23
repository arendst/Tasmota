#!/bin/bash

set -e
mkdir -p build
cd build
cmake ..
make
cd ..
