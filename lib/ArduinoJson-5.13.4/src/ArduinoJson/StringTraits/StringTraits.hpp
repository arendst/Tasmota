// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <string.h>
#include "../Configuration.hpp"
#include "../TypeTraits/EnableIf.hpp"
#include "../TypeTraits/IsBaseOf.hpp"
#include "../TypeTraits/IsChar.hpp"
#include "../TypeTraits/IsConst.hpp"
#include "../TypeTraits/RemoveReference.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TString, typename Enable = void>
struct StringTraits {
  static const bool has_append = false;
  static const bool has_equals = false;
};

template <typename TString>
struct StringTraits<const TString, void> : StringTraits<TString> {};

template <typename TString>
struct StringTraits<TString&, void> : StringTraits<TString> {};
}
}

#include "ArduinoStream.hpp"
#include "CharPointer.hpp"
#include "FlashString.hpp"
#include "StdStream.hpp"
#include "StdString.hpp"
