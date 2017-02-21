#!/bin/sh -eux

curl https://cmake.org/files/v3.4/cmake-3.4.0-Linux-x86_64.tar.gz | tar xz -C /tmp --strip 1

/tmp/bin/cmake -DCOVERAGE=true .
make 
make test

pip install --user cpp-coveralls 'requests[security]'
coveralls --exclude third-party --gcov-options '\-lp'; fi
