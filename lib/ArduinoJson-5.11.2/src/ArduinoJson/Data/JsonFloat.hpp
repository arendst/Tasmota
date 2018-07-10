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

#if ARDUINOJSON_USE_DOUBLE
typedef double JsonFloat;
#else
typedef float JsonFloat;
#endif
}
}
