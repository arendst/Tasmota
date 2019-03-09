ArduinoJson: change log
=======================

v5.13.4
-------

* Removed spurious files in the Particle library

v5.13.3
-------

* Improved float serialization when `-fsingle-precision-constant` is used
* Fixed `JsonVariant::is<int>()` that returned true for empty strings
* Fixed `JsonVariant::is<String>()` (closes #763)

v5.13.2
-------

* Fixed `JsonBuffer::parse()` not respecting nesting limit correctly (issue #693)
* Fixed inconsistencies in nesting level counting (PR #695 from Zhenyu Wu)
* Fixed null values that could be pass to `strcmp()` (PR #745 from Mike Karlesky)
* Added macros `ARDUINOJSON_VERSION`, `ARDUINOJSON_VERSION_MAJOR`...

v5.13.1
-------

* Fixed `JsonVariant::operator|(int)` that returned the default value if the variant contained a double (issue #675)
* Allowed non-quoted key to contain underscores (issue #665)

v5.13.0
-------

* Changed the rules of string duplication (issue #658)
* `RawJson()` accepts any kind of string and obeys to the same rules for duplication
* Changed the return type of `strdup()` to `const char*` to prevent double duplication
* Marked `strdup()` as deprecated

> ### New rules for string duplication
>
> | type                       | duplication |
> |:---------------------------|:------------|
> | const char*                | no          |
> | char*                      | ~~no~~ yes  |
> | String                     | yes         |
> | std::string                | yes         |
> | const __FlashStringHelper* | yes         |
>
> These new rules make `JsonBuffer::strdup()` useless.

v5.12.0
-------

* Added `JsonVariant::operator|` to return a default value (see below)
* Added a clear error message when compiled as C instead of C++ (issue #629)
* Added detection of MPLAB XC compiler (issue #629)
* Added detection of Keil ARM Compiler (issue #629)
* Added an example that shows how to save and load a configuration file
* Reworked all other examples

> ### How to use the new feature?
>
> If you have a block like this:
>
> ```c++
> const char* ssid = root["ssid"];
> if (!ssid)
>   ssid = "default ssid";
> ```
>
> You can simplify like that:
>
> ```c++
> const char* ssid = root["ssid"] | "default ssid";
> ```

v5.11.2
-------

* Fixed `DynamicJsonBuffer::clear()` not resetting allocation size (issue #561)
* Fixed incorrect rounding for float values (issue #588)

v5.11.1
-------

* Removed dependency on `PGM_P` as Particle 0.6.2 doesn't define it (issue #546)
* Fixed warning "dereferencing type-punned pointer will break strict-aliasing rules [-Wstrict-aliasing]"
* Fixed warning "floating constant exceeds range of 'float' [-Woverflow]" (issue #544)
* Fixed warning "this statement may fall through" [-Wimplicit-fallthrough=] (issue #539)
* Removed `ARDUINOJSON_DOUBLE_IS_64BITS` as it became useless.
* Fixed too many decimals places in float serialization (issue #543)

v5.11.0
-------

* Made `JsonBuffer` non-copyable (PR #524 by @luisrayas3)
* Added `StaticJsonBuffer::clear()`
* Added `DynamicJsonBuffer::clear()`

v5.10.1
-------

* Fixed IntelliSense errors in Visual Micro (issue #483)
* Fixed compilation in IAR Embedded Workbench (issue #515)
* Fixed reading "true" as a float (issue #516)
* Added `ARDUINOJSON_DOUBLE_IS_64BITS`
* Added `ARDUINOJSON_EMBEDDED_MODE`

v5.10.0
-------

* Removed configurable number of decimal places (issues #288, #427 and #506)
* Changed exponentiation thresholds to `1e7` and `1e-5` (issues #288, #427 and #506)
* `JsonVariant::is<double>()` now returns `true` for integers
* Fixed error `IsBaseOf is not a member of ArduinoJson::TypeTraits` (issue #495)
* Fixed error `forming reference to reference` (issue #495)

> ### BREAKING CHANGES :warning:
>
> | Old syntax                      | New syntax          |
> |:--------------------------------|:--------------------|
> | `double_with_n_digits(3.14, 2)` | `3.14`              |
> | `float_with_n_digits(3.14, 2)`  | `3.14f`             |
> | `obj.set("key", 3.14, 2)`       | `obj["key"] = 3.14` |
> | `arr.add(3.14, 2)`              | `arr.add(3.14)`     |
>
> | Input     | Old output | New output |
> |:----------|:-----------|:-----------|
> | `3.14159` | `3.14`     | `3.14159`  |
> | `42.0`    | `42.00`    | `42`       |
> | `0.0`     | `0.00`     | `0`        |
>
> | Expression                     | Old result | New result |
> |:-------------------------------|:-----------|:-----------|
> | `JsonVariant(42).is<int>()`    | `true`     | `true`     |
> | `JsonVariant(42).is<float>()`  | `false`    | `true`     |
> | `JsonVariant(42).is<double>()` | `false`    | `true`     |

v5.9.0
------

* Added `JsonArray::remove(iterator)` (issue #479)
* Added `JsonObject::remove(iterator)`
* Renamed `JsonArray::removeAt(size_t)` into `remove(size_t)`
* Renamed folder `include/` to `src/`
* Fixed warnings `floating constant exceeds range of float`and `floating constant truncated to zero` (issue #483)
* Removed `Print` class and converted `printTo()` to a template method (issue #276)
* Removed example `IndentedPrintExample.ino`
* Now compatible with Particle 0.6.1, thanks to Jacob Nite (issue #294 and PR #461 by @foodbag)

v5.8.4
------

* Added custom implementation of `strtod()` (issue #453)
* Added custom implementation of `strtol()` (issue #465)
* `char` is now treated as an integral type (issue #337, #370)

v5.8.3
------

* Fixed an access violation in `DynamicJsonBuffer` when memory allocation fails (issue #433)
* Added operators `==` and `!=` for two `JsonVariant`s (issue #436)
* Fixed `JsonVariant::operator[const FlashStringHelper*]` (issue #441)

v5.8.2
------

* Fixed parsing of comments (issue #421)
* Fixed ignored `Stream` timeout (issue #422)
* Made sure we don't read more that necessary (issue #422)
* Fixed error when the key of a `JsonObject` is a `char[]` (issue #423)
* Reduced code size when using `const` references
* Fixed error with string of type `unsigned char*` (issue #428)
* Added `deprecated` attribute on `asArray()`, `asObject()` and `asString()` (issue #420)

v5.8.1
------

* Fixed error when assigning a `volatile int` to a `JsonVariant` (issue #415)
* Fixed errors with Variable Length Arrays (issue #416)
* Fixed error when both `ARDUINOJSON_ENABLE_STD_STREAM` and `ARDUINOJSON_ENABLE_ARDUINO_STREAM` are set to `1`
* Fixed error "Stream does not name a type" (issue #412)

v5.8.0
------

* Added operator `==` to compare `JsonVariant` and strings (issue #402)
* Added support for `Stream` (issue #300)
* Reduced memory consumption by not duplicating spaces and comments

> ### BREAKING CHANGES :warning:
>
> `JsonBuffer::parseObject()` and  `JsonBuffer::parseArray()` have been pulled down to the derived classes `DynamicJsonBuffer` and `StaticJsonBufferBase`.
>
> This means that if you have code like:
>
> ```c++
> void myFunction(JsonBuffer& jsonBuffer);
> ```
>
> you need to replace it with one of the following:
>
> ```c++
> void myFunction(DynamicJsonBuffer& jsonBuffer);
> void myFunction(StaticJsonBufferBase& jsonBuffer);
> template<typename TJsonBuffer> void myFunction(TJsonBuffer& jsonBuffer);
> ```

v5.7.3
------

* Added an `printTo(char[N])` and `prettyPrintTo(char[N])` (issue #292)
* Added ability to set a nested value like this: `root["A"]["B"] = "C"` (issue #352)
* Renamed `*.ipp` to `*Impl.hpp` because they were ignored by Arduino IDE (issue #396)

v5.7.2
------

* Made PROGMEM available on more platforms (issue #381)
* Fixed PROGMEM causing an exception on ESP8266 (issue #383)

v5.7.1
------

* Added support for PROGMEM (issue #76)
* Fixed compilation error when index is not an `int` (issue #381)

v5.7.0
------

* Templatized all functions using `String` or `std::string`
* Removed `ArduinoJson::String`
* Removed `JsonVariant::defaultValue<T>()`
* Removed non-template `JsonObject::get()` and `JsonArray.get()`
* Fixed support for `StringSumHelper` (issue #184)
* Replaced `ARDUINOJSON_USE_ARDUINO_STRING` by `ARDUINOJSON_ENABLE_STD_STRING` and `ARDUINOJSON_ENABLE_ARDUINO_STRING` (issue #378)
* Added example `StringExample.ino` to show where `String` can be used
* Increased default nesting limit to 50 when compiled for a computer (issue #349)

> ### BREAKING CHANGES :warning:
>
> The non-template functions `JsonObject::get()` and `JsonArray.get()` have been removed. This means that you need to explicitely tell the type you expect in return.
>
> Old code:
>
> ```c++
> #define ARDUINOJSON_USE_ARDUINO_STRING 0
> JsonVariant value1 = myObject.get("myKey");
> JsonVariant value2 = myArray.get(0);
> ```
>
> New code:
>
> ```c++
> #define ARDUINOJSON_ENABLE_ARDUINO_STRING 0
> #define ARDUINOJSON_ENABLE_STD_STRING 1
> JsonVariant value1 = myObject.get<JsonVariant>("myKey");
> JsonVariant value2 = myArray.get<JsonVariant>(0);
> ```

v5.6.7
------

* Fixed `array[idx].as<JsonVariant>()` and `object[key].as<JsonVariant>()`
* Fixed return value of `JsonObject::set()` (issue #350)
* Fixed undefined behavior in `Prettyfier` and `Print` (issue #354)
* Fixed parser that incorrectly rejected floats containing a `+` (issue #349)

v5.6.6
------

* Fixed `-Wparentheses` warning introduced in v5.6.5 (PR #335 by @nuket)
* Added `.mbedignore` for ARM mbdeb (PR #334 by @nuket)
* Fixed  `JsonVariant::success()` which didn't propagate `JsonArray::success()` nor `JsonObject::success()` (issue #342).

v5.6.5
------

* `as<char*>()` now returns `true` when input is `null` (issue #330)

v5.6.4
------

* Fixed error in float serialization (issue #324)

v5.6.3
------

* Improved speed of float serialization (about twice faster)
* Added `as<JsonArray>()` as a synonym for `as<JsonArray&>()`... (issue #291)
* Fixed `call of overloaded isinf(double&) is ambiguous` (issue #284)

v5.6.2
------

* Fixed build when another lib does `#undef isnan` (issue #284)

v5.6.1
------

* Added missing `#pragma once` (issue #310)

v5.6.0
------

* ArduinoJson is now a header-only library (issue #199)

v5.5.1
------

* Fixed compilation error with Intel Galileo (issue #299)

v5.5.0
------

* Added `JsonVariant::success()` (issue #279)
* Renamed `JsonVariant::invalid<T>()` to `JsonVariant::defaultValue<T>()`

v5.4.0
------

* Changed `::String` to `ArduinoJson::String` (issue #275)
* Changed `::Print` to `ArduinoJson::Print` too

v5.3.0
------

* Added custom implementation of `ftoa` (issues #266, #267, #269 and #270)
* Added `JsonVariant JsonBuffer::parse()` (issue #265)
* Fixed `unsigned long` printed as `signed long` (issue #170)

v5.2.0
------

* Added `JsonVariant::as<char*>()` as a synonym for `JsonVariant::as<const char*>()` (issue #257)
* Added example `JsonHttpClient` (issue #256)
* Added `JsonArray::copyTo()` and `JsonArray::copyFrom()` (issue #254)
* Added `RawJson()` to insert pregenerated JSON portions (issue #259)

v5.1.1
------

* Removed `String` duplication when one replaces a value in a `JsonObject` (PR #232 by @ulion)

v5.1.0
------

* Added support of `long long` (issue #171)
* Moved all build settings to `ArduinoJson/Configuration.hpp`

> ### BREAKING CHANGE :warning:
>
> If you defined `ARDUINOJSON_ENABLE_STD_STREAM`, you now need to define it to `1`.

v5.0.8
------

* Made the library compatible with [PlatformIO](http://platformio.org/) (issue #181)
* Fixed `JsonVariant::is<bool>()` that was incorrectly returning false (issue #214)

v5.0.7
------

* Made library easier to use from a CMake project: simply `add_subdirectory(ArduinoJson/src)`
* Changed `String` to be a `typedef` of `std::string` (issues #142 and #161)

> ### BREAKING CHANGES :warning:
>
> - `JsonVariant(true).as<String>()` now returns `"true"` instead of `"1"`
> - `JsonVariant(false).as<String>()` now returns `"false"` instead of `"0"`

v5.0.6
------

* Added parameter to `DynamicJsonBuffer` constructor to set initial size (issue #152)
* Fixed warning about library category in Arduino 1.6.6 (issue #147)
* Examples: Added a loop to wait for serial port to be ready (issue #156)

v5.0.5
------

* Added overload `JsonObjectSuscript::set(value, decimals)` (issue #143)
* Use `float` instead of `double` to reduce the size of `JsonVariant` (issue #134)

v5.0.4
------

* Fixed ambiguous overload with `JsonArraySubscript` and `JsonObjectSubscript` (issue #122)

v5.0.3
------

* Fixed `printTo(String)` which wrote numbers instead of strings (issue #120)
* Fixed return type of `JsonArray::is<T>()` and some others (issue #121)

v5.0.2
------

* Fixed segmentation fault in `parseObject(String)` and `parseArray(String)`, when the
  `StaticJsonBuffer` is too small to hold a copy of the string
* Fixed Clang warning "register specifier is deprecated" (issue #102)
* Fixed GCC warning "declaration shadows a member" (issue #103)
* Fixed memory alignment, which made ESP8266 crash (issue #104)
* Fixed compilation on Visual Studio 2010 and 2012 (issue #107)

v5.0.1
------

* Fixed compilation with Arduino 1.0.6 (issue #99)

v5.0.0
------

* Added support of `String` class (issues #55, #56, #70, #77)
* Added `JsonBuffer::strdup()` to make a copy of a string (issues #10, #57)
* Implicitly call `strdup()` for `String` but not for `char*` (issues #84, #87)
* Added support of non standard JSON input (issue #44)
* Added support of comments in JSON input (issue #88)
* Added implicit cast between numerical types (issues #64, #69, #93)
* Added ability to read number values as string (issue #90)
* Redesigned `JsonVariant` to leverage converting constructors instead of assignment operators (issue #66)
* Switched to new the library layout (requires Arduino 1.0.6 or above)

> ### BREAKING CHANGES :warning:
>
> - `JsonObject::add()` was renamed to `set()`
> - `JsonArray::at()` and `JsonObject::at()` were renamed to `get()`
> - Number of digits of floating point value are now set with `double_with_n_digits()`

**Personal note about the `String` class**:
Support of the `String` class has been added to the library because many people use it in their programs.
However, you should not see this as an invitation to use the `String` class.
The `String` class is **bad** because it uses dynamic memory allocation.
Compared to static allocation, it compiles to a bigger, slower program, and is less predictable.
You certainly don't want that in an embedded environment!

v4.6
----

* Fixed segmentation fault in `DynamicJsonBuffer` when memory allocation fails (issue #92)

v4.5
----

* Fixed buffer overflow when input contains a backslash followed by a terminator (issue #81)

**Upgrading is recommended** since previous versions contain a potential security risk.

Special thanks to [Giancarlo Canales Barreto](https://github.com/gcanalesb) for finding this nasty bug.

v4.4
----

* Added `JsonArray::measureLength()` and `JsonObject::measureLength()` (issue #75)

v4.3
----

* Added `JsonArray::removeAt()` to remove an element of an array (issue #58)
* Fixed stack-overflow in `DynamicJsonBuffer` when parsing huge JSON files (issue #65)
* Fixed wrong return value of `parseArray()` and `parseObject()` when allocation fails (issue #68)

v4.2
----

* Switched back to old library layout (issues #39, #43 and #45)
* Removed global new operator overload (issue #40, #45 and #46)
* Added an example with EthernetServer

v4.1
----

* Added DynamicJsonBuffer (issue #19)

v4.0
----

* Unified parser and generator API (issue #23)
* Updated library layout, now requires Arduino 1.0.6 or newer

> ### BREAKING CHANGES :warning:
>
> API changed significantly since v3, see [Migrating code to the new API](https://arduinojson.org/doc/migration/).

