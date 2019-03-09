// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "JsonVariantCasts.hpp"
#include "JsonVariantComparisons.hpp"
#include "JsonVariantSubscripts.hpp"
#include "Serialization/JsonPrintable.hpp"

namespace ArduinoJson {

template <typename TImpl>
class JsonVariantBase : public Internals::JsonPrintable<TImpl>,
                        public JsonVariantCasts<TImpl>,
                        public JsonVariantComparisons<TImpl>,
                        public JsonVariantSubscripts<TImpl>,
                        public TypeTraits::JsonVariantTag {};
}
