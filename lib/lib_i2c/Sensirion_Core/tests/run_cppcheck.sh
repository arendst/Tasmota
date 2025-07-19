#!/bin/bash
set -euxo pipefail
cppcheck --std=c++11 --language=c++ --error-exitcode=1 --enable=warning,style,performance,portability src/*
