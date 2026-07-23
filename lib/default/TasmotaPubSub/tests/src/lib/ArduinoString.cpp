/*
  ArduinoString.cpp - Host-side minimal Arduino `String` shim implementation.
*/

#include "ArduinoString.h"

String::String() : _str() {}

String::String(const char* cstr) : _str(cstr ? cstr : "") {}

String& String::operator=(const char* cstr) {
    _str = cstr ? cstr : "";
    return *this;
}

size_t String::length() const {
    return _str.length();
}

const char* String::c_str() const {
    return _str.c_str();
}
