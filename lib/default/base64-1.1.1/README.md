# base64_arduino

Base64 encoder/decoder for arduino repo

[![npm](https://img.shields.io/npm/l/express.svg)]()
[![Build Status](https://travis-ci.org/Densaugeo/base64_arduino.svg?branch=master)](https://travis-ci.org/Densaugeo/base64_arduino)

## Installation

Add base64.cpp and base64.hpp to your project folder or library search path, put `#include "base64.hpp"` in your source, and pass base64.cpp to your compiler

## Usage

~~~
unsigned char binary[] = {133, 244, 117, 206, 178, 195};
unsigned char base64[9];

unsigned int base64_length = encode_base64(binary, 6, base64);

printf("%d\n", base64_length); // Prints "8"
printf((char *) base64); // Prints "hfR1zrLD"
~~~

~~~
unsigned char base64[] = "hfR1zrLD";
unsigned char binary[6];

unsigned int binary_length = decode_base64(base64, binary);

printf("[%d, %d, %d, %d, %d, %d]\n", // Prints "[133, 244, 117, 206, 178, 195]"
       binary[0], binary[1], binary[2],
       binary[3], binary[4], binary[5]);
printf("%d\n", binary_length); // Prints "6"
~~~

## Details

Uses common web conventions - '+' for 62, '/' for 63, '=' for padding. Note that invalid base64 characters are interpreted as padding.

Can be compiled as C, uses .*pp extensions because it is usually used in C++ projects and is tested for C++.

## License

MIT
