// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

// If Visual Studo <= 2012
#if defined(_MSC_VER) && _MSC_VER <= 1700

#include <float.h>

namespace ArduinoJson {
namespace Polyfills {
template <typename T>
bool isNaN(T x) {
  return _isnan(x) != 0;
}

template <typename T>
bool isInfinity(T x) {
  return !_finite(x);
}
}
}

#else

#include <math.h>

// GCC warning: "conversion to 'float' from 'double' may alter its value"
#ifdef __GNUC__
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9)
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#else
#pragma GCC diagnostic ignored "-Wconversion"
#endif
#endif

// Workaround for libs that #undef isnan or isinf
// https://github.com/bblanchon/ArduinoJson/issues/284
#if !defined(isnan) || !defined(isinf)
namespace std {}
#endif

namespace ArduinoJson {
namespace Polyfills {

template <typename T>
bool isNaN(T x) {
// Workaround for libs that #undef isnan
// https://github.com/bblanchon/ArduinoJson/issues/284
#ifndef isnan
  using namespace std;
#endif

  return isnan(x);
}

#if defined(_GLIBCXX_HAVE_ISNANL) && _GLIBCXX_HAVE_ISNANL
template <>
inline bool isNaN<double>(double x) {
  return isnanl(x);
}
#endif

#if defined(_GLIBCXX_HAVE_ISNANF) && _GLIBCXX_HAVE_ISNANF
template <>
inline bool isNaN<float>(float x) {
  return isnanf(x);
}
#endif

template <typename T>
bool isInfinity(T x) {
// Workaround for libs that #undef isinf
// https://github.com/bblanchon/ArduinoJson/issues/284
#ifndef isinf
  using namespace std;
#endif

  return isinf(x);
}

#if defined(_GLIBCXX_HAVE_ISINFL) && _GLIBCXX_HAVE_ISINFL
template <>
inline bool isInfinity<double>(double x) {
  return isinfl(x);
}
#endif

#if defined(_GLIBCXX_HAVE_ISINFF) && _GLIBCXX_HAVE_ISINFF
template <>
inline bool isInfinity<float>(float x) {
  return isinff(x);
}
#endif

#if defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
#endif
}
}
#endif
