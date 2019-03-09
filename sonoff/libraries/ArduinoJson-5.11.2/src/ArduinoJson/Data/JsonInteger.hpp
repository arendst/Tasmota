// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "../Configuration.hpp"

namespace ArduinoJson {
namespace Internals {

#if ARDUINOJSON_USE_LONG_LONG
typedef long long JsonInteger;
typedef unsigned long long JsonUInt;
#elif ARDUINOJSON_USE_INT64
typedef __int64 JsonInteger;
typedef unsigned _int64 JsonUInt;
#else
typedef long JsonInteger;
typedef unsigned long JsonUInt;
#endif
}
}
