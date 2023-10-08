
/**
 * Mobizt's SRAM/PSRAM supported String, version 1.2.9
 *
 * Created December 3, 2022
 *
 * Changes Log
 *
 * v1.2.9
 * - substring optimization
 *
 * v1.2.8
 * - Add support StringSumHelper class in Arduino
 *
 * v1.2.7
 * - Fix string sub type checking issue
 *
 * v1.2.6
 * - Update trim() function
 *
 * v1.2.5
 * - Fixed double string issue and add support long double
 *
 * v1.2.4
 * - Check PSRAM availability before allocating the memory
 *
 * v1.2.3
 * - Fixed flash string F and PSTR handle
 *
 * v1.2.2
 * - Add supports more MCUs.
 *
 * v1.2.1
 * - Add flash string manipulation functions.
 *
 * v1.2.0
 * - Add supports bool, integer and float manipulation.
 *
 * v1.1.2
 * - Fix substring with zero length returns the original string issue.
 *
 * v1.1.1
 * - Fix possible ESP8266 code exit without resetting the external heap stack
 *
 * v1.1.0
 * - Add support ESP8266 external virtual RAM (SRAM or PSRAM)
 *
 * v1.0.1
 * - Add trim function
 * - Add version enum
 *
 * v1.0.0
 * - Initial release
 *
 * The MIT License (MIT)
 * Copyright (c) 2023 K. Suwatchai (Mobizt)
 *
 *
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MB_String_H
#define MB_String_H

#include <Arduino.h>
#if !defined(__AVR__)
#include <string>
#include <strings.h>
#include <algorithm>
#endif

#define MB_STRING_MAJOR 1
#define MB_STRING_MINOR 2
#define MB_STRING_PATCH 5

#if defined(ESP8266) && defined(MMU_EXTERNAL_HEAP) && defined(MB_STRING_USE_PSRAM)
#include <umm_malloc/umm_malloc.h>
#include <umm_malloc/umm_heap_select.h>
#define ESP8266_USE_EXTERNAL_HEAP
#endif

#if defined(ESP8266) || defined(ESP32)
#define MBSTRING_FLASH_MCR FPSTR
#elif defined(ARDUINO_ARCH_SAMD) || defined(__AVR_ATmega4809__) || defined(ARDUINO_NANO_RP2040_CONNECT)
#define MBSTRING_FLASH_MCR PSTR
#else
#define MBSTRING_FLASH_MCR(s) (s)
#endif

class MB_String;

#define pgm2Str(p) (MB_String().appendP(p).c_str())
#define num2Str(v, p) (MB_String().appendNum(v, p).c_str())

#if defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4)
#define MB_IS_SAME std::is_same
#define MB_ENABLE_IF std::enable_if
#else
#define MB_IS_SAME is_same
#define MB_ENABLE_IF enable_if
#endif

namespace mb_string
{
    enum mb_string_sub_type
    {
        mb_string_sub_type_none,
        mb_string_sub_type_bool,
        mb_string_sub_type_float,
        mb_string_sub_type_double,
        mb_string_sub_type_int8,
        mb_string_sub_type_uint8,
        mb_string_sub_type_int16,
        mb_string_sub_type_uint16,
        mb_string_sub_type_int32,
        mb_string_sub_type_uint32,
        mb_string_sub_type_int64,
        mb_string_sub_type_uint64,
        mb_string_sub_type_cstring,
        mb_string_sub_type_chars,
        mb_string_sub_type_mb_string,
        mb_string_sub_type_arduino_string,
        mb_string_sub_type_std_string,
        mb_string_sub_type_fptr,
        mb_string_sub_type_string_sum_helper

    };

    typedef struct mb_string_ptr_t
    {

    public:
        mb_string_ptr_t(uint32_t addr = 0, mb_string_sub_type type = mb_string_sub_type_cstring, int precision = -1, const StringSumHelper *s = nullptr)
        {
            _addr = addr;
            _type = type;
            _precision = precision;
            _ssh = s;
        }
        int precision() { return _precision; }
        mb_string_sub_type type() { return _type; }
        uint32_t address() { return _addr; }
        const StringSumHelper *stringsumhelper() { return _ssh; }

    private:
        mb_string_sub_type _type = mb_string_sub_type_none;
        int _precision = -1;
        uint32_t _addr = 0;
        const StringSumHelper *_ssh = nullptr;

    } MB_StringPtr;

    template <bool, typename T = void>
    struct enable_if
    {
    };
    template <typename T>
    struct enable_if<true, T>
    {
        typedef T type;
    };
    template <typename T, typename U>
    struct is_same
    {
        static bool const value = false;
    };
    template <typename T>
    struct is_same<T, T>
    {
        static bool const value = true;
    };

    template <typename T>
    struct is_num_int8
    {
        static bool const value = MB_IS_SAME<T, int8_t>::value || MB_IS_SAME<T, signed char>::value;
    };

    template <typename T>
    struct is_num_uint8
    {
        static bool const value = MB_IS_SAME<T, uint8_t>::value || MB_IS_SAME<T, unsigned char>::value;
    };

    template <typename T>
    struct is_num_int16
    {
        static bool const value = MB_IS_SAME<T, int16_t>::value || MB_IS_SAME<T, signed short>::value;
    };

    template <typename T>
    struct is_num_uint16
    {
        static bool const value = MB_IS_SAME<T, uint16_t>::value || MB_IS_SAME<T, unsigned short>::value;
    };

    template <typename T>
    struct is_num_int32
    {
        static bool const value = MB_IS_SAME<T, signed int>::value || MB_IS_SAME<T, int>::value ||
                                  MB_IS_SAME<T, int32_t>::value || MB_IS_SAME<T, long>::value ||
                                  MB_IS_SAME<T, signed long>::value;
    };

    template <typename T>
    struct is_num_uint32
    {
        static bool const value = MB_IS_SAME<T, unsigned int>::value || MB_IS_SAME<T, uint32_t>::value ||
                                  MB_IS_SAME<T, unsigned long>::value;
    };

    template <typename T>
    struct is_num_int64
    {
        static bool const value = MB_IS_SAME<T, int64_t>::value || MB_IS_SAME<T, signed long long>::value;
    };

    template <typename T>
    struct is_num_uint64
    {
        static bool const value = MB_IS_SAME<T, uint64_t>::value || MB_IS_SAME<T, unsigned long long>::value;
    };

    template <typename T>
    struct is_num_neg_int
    {
        static bool const value = is_num_int8<T>::value || is_num_int16<T>::value ||
                                  is_num_int32<T>::value || is_num_int64<T>::value;
    };

    template <typename T>
    struct is_num_pos_int
    {
        static bool const value = is_num_uint8<T>::value || is_num_uint16<T>::value ||
                                  is_num_uint32<T>::value || is_num_uint64<T>::value;
    };

    template <typename T>
    struct is_num_int
    {
        static bool const value = is_num_pos_int<T>::value || is_num_neg_int<T>::value;
    };

    template <typename T>
    struct is_num_float
    {
        static bool const value = MB_IS_SAME<T, float>::value || MB_IS_SAME<T, double>::value || MB_IS_SAME<T, long double>::value;
    };

    template <typename T>
    struct is_bool
    {
        static bool const value = MB_IS_SAME<T, bool>::value;
    };

    template <typename T>
    struct cs_t
    {
        static bool const value = MB_IS_SAME<T, char *>::value;
    };

    template <typename T>
    struct ccs_t
    {
        static bool const value = MB_IS_SAME<T, const char *>::value;
    };

    template <typename T>
    struct as_t
    {
        static bool const value = MB_IS_SAME<T, String>::value;
    };

    template <typename T>
    struct cas_t
    {
        static bool const value = MB_IS_SAME<T, const String>::value;
    };

    template <typename T>
    struct ss_t
    {
#if !defined(__AVR__)
        static bool const value = MB_IS_SAME<T, std::string>::value;
#else
        static bool const value = false;
#endif
    };

    template <typename T>
    struct css_t
    {
#if !defined(__AVR__)
        static bool const value = MB_IS_SAME<T, const std::string>::value;
#else
        static bool const value = false;
#endif
    };

    template <typename T>
    struct ssh_t
    {
        static bool const value = MB_IS_SAME<T, StringSumHelper>::value;
    };

    template <typename T>
    struct fs_t
    {
        static bool const value = MB_IS_SAME<T, const __FlashStringHelper *>::value;
    };

    template <typename T>
    struct mbs_t
    {
        static bool const value = MB_IS_SAME<T, MB_String>::value;
    };

    template <typename T>
    struct cmbs_t
    {
        static bool const value = MB_IS_SAME<T, const MB_String>::value;
    };

    template <typename T>
    struct pgm_t
    {
        static bool const value = MB_IS_SAME<T, PGM_P>::value;
    };

    template <typename T>
    struct is_const_chars
    {
        static bool const value = cs_t<T>::value || ccs_t<T>::value;
    };

    template <typename T>
    struct is_arduino_string
    {
        static bool const value = as_t<T>::value || cas_t<T>::value;
    };

    template <typename T>
    struct is_std_string
    {
        static bool const value = ss_t<T>::value || css_t<T>::value;
    };

    template <typename T>
    struct is_mb_string
    {
        static bool const value = mbs_t<T>::value || cmbs_t<T>::value;
    };

    template <typename T>
    struct is_arduino_string_sum_helper
    {
        static bool const value = ssh_t<T>::value;
    };

    template <typename T>
    struct is_arduino_flash_string_helper
    {
        static bool const value = fs_t<T>::value;
    };

    template <typename T>
    struct is_string
    {
        static bool const value = is_const_chars<T>::value || is_arduino_string<T>::value ||
                                  is_arduino_string_sum_helper<T>::value || is_arduino_flash_string_helper<T>::value ||
                                  is_std_string<T>::value || is_mb_string<T>::value;
    };

    template <typename T>
    uint32_t toAddr(T &v) { return reinterpret_cast<uint32_t>(&v); }

#if defined(__AVR__)
    template <typename T>
    T addrTo(int address)
    {
        return reinterpret_cast<T>(address);
    }
#else
    template <typename T>
    auto addrTo(int address) -> typename MB_ENABLE_IF<!MB_IS_SAME<T, nullptr_t>::value, T>::type
    {
        return reinterpret_cast<T>(address);
    }
#endif

    template <typename T>
    auto getSubType(T val) -> typename MB_ENABLE_IF<is_num_int<T>::value || is_num_float<T>::value || MB_IS_SAME<T, bool>::value || is_const_chars<T>::value || is_arduino_flash_string_helper<T>::value || is_arduino_string<T>::value || is_std_string<T>::value || is_mb_string<T>::value || MB_IS_SAME<T, StringSumHelper>::value, mb_string_sub_type>::type
    {
        if (is_num_uint64<T>::value)
            return mb_string_sub_type_uint64;
        else if (is_num_int64<T>::value)
            return mb_string_sub_type_int64;
        else if (is_num_uint32<T>::value)
            return mb_string_sub_type_uint32;
        else if (is_num_int32<T>::value)
            return mb_string_sub_type_int32;
        else if (is_num_uint16<T>::value)
            return mb_string_sub_type_uint16;
        else if (is_num_int16<T>::value)
            return mb_string_sub_type_int16;
        else if (is_num_uint8<T>::value)
            return mb_string_sub_type_uint8;
        else if (is_num_int8<T>::value)
            return mb_string_sub_type_int8;
        else if (MB_IS_SAME<T, bool>::value)
            return mb_string_sub_type_bool;
        else if (MB_IS_SAME<T, float>::value)
            return mb_string_sub_type_float;
        else if (MB_IS_SAME<T, double>::value)
            return mb_string_sub_type_double;
        else if (is_arduino_string<T>::value)
            return mb_string_sub_type_arduino_string;
        else if (is_std_string<T>::value)
            return mb_string_sub_type_std_string;
        else if (is_mb_string<T>::value)
            return mb_string_sub_type_mb_string;
        else if (is_arduino_flash_string_helper<T>::value)
            return mb_string_sub_type_fptr;
        else if (MB_IS_SAME<T, StringSumHelper>::value)
            return mb_string_sub_type_string_sum_helper;
        else if (ccs_t<T>::value)
            return mb_string_sub_type_cstring;
        else if (cs_t<T>::value)
            return mb_string_sub_type_chars;

        return mb_string_sub_type_int8;
    }

    template <typename T>
    auto getSubType(T *val) -> typename MB_ENABLE_IF<is_num_int<T>::value || is_num_float<T>::value || MB_IS_SAME<T, bool>::value || is_const_chars<T>::value || is_arduino_flash_string_helper<T>::value || is_arduino_string<T>::value || is_std_string<T>::value || is_mb_string<T>::value || MB_IS_SAME<T, StringSumHelper>::value, mb_string_sub_type>::type
    {
        return getSubType(*val);
    }

    template <typename T>
    auto toStringPtr(const T &val) -> typename MB_ENABLE_IF<is_std_string<T>::value || is_arduino_string<T>::value || is_mb_string<T>::value, MB_StringPtr>::type
    {
        return MB_StringPtr(reinterpret_cast<uint32_t>(&val), getSubType(val));
    }

    template <typename T>
    auto toStringPtr(const T &val) -> typename MB_ENABLE_IF<MB_IS_SAME<T, StringSumHelper>::value, MB_StringPtr>::type
    {
#if defined(ESP8266)
        return MB_StringPtr(reinterpret_cast<uint32_t>(&val), getSubType(val), -1);

#else
        return MB_StringPtr(reinterpret_cast<uint32_t>(&val), getSubType(val), -1, &val);
#endif
    }

    template <typename T>
    auto toStringPtr(T val) -> typename MB_ENABLE_IF<is_const_chars<T>::value, MB_StringPtr>::type { return MB_StringPtr(reinterpret_cast<uint32_t>(val), getSubType(val)); }

    template <typename T>
    auto toStringPtr(T &val) -> typename MB_ENABLE_IF<is_arduino_flash_string_helper<T>::value, MB_StringPtr>::type { return MB_StringPtr(reinterpret_cast<uint32_t>(val), getSubType(val)); }

#if !defined(__AVR__)
    template <typename T>
    auto toStringPtr(T val) -> typename MB_ENABLE_IF<MB_IS_SAME<T, std::nullptr_t>::value, MB_StringPtr>::type
    {
        return MB_StringPtr();
    }
#endif

    template <typename T>
    auto toStringPtr(T val) -> typename MB_ENABLE_IF<MB_IS_SAME<T, MB_StringPtr>::value, MB_StringPtr>::type
    {
        return val;
    }

    template <typename T>
    auto toStringPtr(T &val, int precision = -1) -> typename MB_ENABLE_IF<is_num_int<T>::value || is_num_float<T>::value || MB_IS_SAME<T, bool>::value, MB_StringPtr>::type { return MB_StringPtr(reinterpret_cast<uint32_t>(&val), getSubType(val), precision); }
}

using namespace mb_string;

class MB_String
{
public:
    MB_String()
    {
#if defined(ESP8266_USE_EXTERNAL_HEAP)
        // reserve default 1 byte external heap to refer to its pointer later
        reset(1);
#endif
    };

    ~MB_String()
    {
        allocate(0, false);
    };

    MB_String(const char *cstr)
    {
        if (cstr)
            copy(cstr, strlen_P(cstr));
    }

    MB_String(const MB_String &value)
    {
        *this = value;
    }

    MB_String(const __FlashStringHelper *str)
    {
        *this = str;
    }

#if !defined(ESP8266)
    MB_String(StringSumHelper rval)
    {
        *this = rval;
    }
#endif

    MB_String(MB_StringPtr value)
    {
        *this = value;
    }

    MB_String(String value)
    {
        *this = value;
    }

    MB_String(bool value)
    {
        appendNum(value);
    }

    MB_String(unsigned char value, unsigned char base = 10)
    {
        int len = 1 + 8 * sizeof(unsigned char);
        reserve(len);

        if (bufLen > 0)
            utoa(value, buf, base);
    }

    MB_String(int value, unsigned char base = 10)
    {
        int len = 2 + 8 * sizeof(int);
        reserve(len);

        if (bufLen > 0)
        {
            if (base == 10)
                sprintf(buf, (const char *)MBSTRING_FLASH_MCR("%d"), value);
            else
                itoa(value, buf, base);
        }
    }

    MB_String(unsigned int value, unsigned char base = 10)
    {
        int len = 1 + 8 * sizeof(unsigned int);
        reserve(len);

        if (bufLen > 0)
            utoa(value, buf, base);
    }

    MB_String(long value, unsigned char base = 10)
    {
        int len = 2 + 8 * sizeof(long);
        reserve(len);

        if (bufLen > 0)
        {
            if (base == 10)
                sprintf(buf, (const char *)MBSTRING_FLASH_MCR("%ld"), value);
            else
                ltoa(value, buf, base);
        }
    }

    MB_String(unsigned long value, unsigned char base = 10)
    {
        int len = 1 + 8 * sizeof(unsigned long);
        reserve(len);

        if (bufLen > 0)
            ultoa(value, buf, base);
    }

    MB_String(float value, unsigned char decimalPlaces = 2)
    {
        reserve(33);
        if (bufLen > 0)
        {
            char *v = toFloatStr(value, 0, decimalPlaces);
            if (v)
            {
                strcpy(buf, v);
                delP(&v);
            }
        }
    }

    MB_String(double value, unsigned char decimalPlaces = 3)
    {
        reserve(33);

        if (bufLen > 0)
        {
            char *v = toFloatStr(value, 1, decimalPlaces);
            if (v)
            {
                strcpy(buf, v);
                delP(&v);
            }
        }
    }

    MB_String(long double value, unsigned char decimalPlaces = 3)
    {
        reserve(65);
        if (bufLen > 0)
        {
            char *v = toFloatStr(value, 2, decimalPlaces);
            if (v)
            {
                strcpy(buf, v);
                delP(&v);
            }
        }
    }

#if !defined(__AVR__)
    MB_String &operator=(const std::string &rhs)
    {
        if (rhs.length() > 0)
            copy(rhs.c_str(), rhs.length());
        else
            clear();

        return *this;
    }
#endif
    MB_String &operator=(const String &rhs)
    {

        if (rhs.length() > 0)
            copy(rhs.c_str(), rhs.length());
        else
            clear();

        return *this;
    }

    MB_String &operator=(const __FlashStringHelper *str)
    {
        if (str)
            appendF(str, true);

        return *this;
    }

#if !defined(ESP8266)
    MB_String &operator=(StringSumHelper rval)
    {
        String temp = rval;
        *this = temp;
        return *this;
    }

    MB_String &operator+=(StringSumHelper rval)
    {
        String temp = rval;
        *this += temp;
        return *this;
    }
#endif

    MB_String &operator+=(const __FlashStringHelper *str)
    {
        if (str)
            appendF(str);

        return *this;
    }

    unsigned char operator==(const MB_String &rhs) const
    {
        return equals(rhs);
    }

    unsigned char operator==(const char *cstr) const
    {
        return equals(cstr);
    }

    unsigned char operator!=(const MB_String &rhs) const
    {
        return !equals(rhs);
    }

    unsigned char operator!=(const char *cstr) const
    {
        return !equals(cstr);
    }
#if !defined(__AVR__)
    MB_String &operator+=(const std::string &rhs)
    {
        concat(rhs.c_str());
        return (*this);
    }
#endif
    MB_String &operator+=(const String &rhs)
    {
        concat(rhs.c_str());
        return (*this);
    }

    MB_String &operator=(const MB_String &rhs)
    {
        if (this == &rhs)
            return *this;

        if (rhs.length() > 0)
            copy(rhs.buf, rhs.length());
        else
            clear();

        return *this;
    }

    MB_String &operator+=(const MB_String &rhs)
    {
        concat(rhs);
        return (*this);
    }

    MB_String &operator+=(const char *cstr)
    {
        size_t len = strlen_P(cstr);
        size_t slen = length();

        if (_reserve(slen + len, false))
        {
            strcat_P(buf, (PGM_P)cstr);
            *(buf + slen + len) = '\0';
        }

        return (*this);
    }

    MB_String &operator+=(char cstr)
    {
        append(1, cstr);
        return (*this);
    }

    MB_String &operator+=(bool value)
    {
        appendNum(value);
        return (*this);
    }

    MB_String &operator=(MB_StringPtr ptr)
    {
        setPtr(ptr);
        return (*this);
    }

    MB_String &operator+=(MB_StringPtr ptr)
    {
        appendPtr(ptr);
        return (*this);
    }

    template <typename T = int>
    auto operator=(T value) -> typename MB_ENABLE_IF<is_num_int<T>::value || is_num_float<T>::value || is_bool<T>::value, MB_String &>::type
    {
        clear();
        appendNum(value);
        return (*this);
    }

    template <typename T = int>
    auto operator+=(T value) -> typename MB_ENABLE_IF<is_num_int<T>::value || is_num_float<T>::value || is_bool<T>::value, MB_String &>::type
    {
        appendNum(value);
        return (*this);
    }

    MB_String &appendP(PGM_P pgms, bool clear = false)
    {
        if (clear)
            this->clear();

        char *t = pgmStr(pgms);
        if (t)
        {
            *this += t;
            delP(&t);
        }

        return (*this);
    }

    MB_String &appendF(const __FlashStringHelper *pstr, bool clear = false)
    {
        if (clear)
            this->clear();

        int len = strlen_P((PGM_P)pstr);
        if (len > 0)
        {
            unsigned int newlen = length() + len;
            reserve(newlen);

            if (bufLen > 0)
                memcpy_P(buf + length(), (PGM_P)pstr, len + 1);
        }

        return (*this);
    }

    MB_String &setPtr(MB_StringPtr src)
    {
        clear();
        appendPtr(src);
        return (*this);
    };

    MB_String &appendPtr(MB_StringPtr src)
    {
        if (src.type() == mb_string_sub_type_fptr)
            appendF(addrTo<const __FlashStringHelper *>(src.address()));
        else if (src.type() == mb_string_sub_type_cstring || src.type() == mb_string_sub_type_chars)
            *this += addrTo<const char *>(src.address());
        else if (src.type() == mb_string_sub_type_arduino_string)
            *this += *addrTo<String *>(src.address());
        else if (src.type() == mb_string_sub_type_string_sum_helper)
#if !defined(ESP8266)
            *this += *src.stringsumhelper();
#else
            *this += *addrTo<String *>(src.address());
#endif
#if !defined(__AVR__)
        else if (src.type() == mb_string_sub_type_std_string)
            *this += *addrTo<std::string *>(src.address());
#endif
        else if (src.type() == mb_string_sub_type_mb_string)
            *this += *addrTo<MB_String *>(src.address());
        else if (src.type() == mb_string_sub_type_uint64)
            appendNum(*addrTo<uint64_t *>(src.address()));
        else if (src.type() == mb_string_sub_type_int64)
            appendNum(*addrTo<int64_t *>(src.address()));
        else if (src.type() == mb_string_sub_type_uint32)
            appendNum(*addrTo<uint32_t *>(src.address()));
        else if (src.type() == mb_string_sub_type_int32)
            appendNum(*addrTo<int32_t *>(src.address()));
        else if (src.type() == mb_string_sub_type_uint16)
            appendNum(*addrTo<uint16_t *>(src.address()));
        else if (src.type() == mb_string_sub_type_int16)
            appendNum(*addrTo<int16_t *>(src.address()));
        else if (src.type() == mb_string_sub_type_uint8)
            appendNum(*addrTo<uint8_t *>(src.address()));
        else if (src.type() == mb_string_sub_type_int8)
            appendNum(*addrTo<int8_t *>(src.address()));
        else if (src.type() == mb_string_sub_type_bool)
            appendNum(*addrTo<bool *>(src.address()));
        else if (src.type() == mb_string_sub_type_float)
            appendNum(*addrTo<float *>(src.address()), src.precision());
        else if (src.type() == mb_string_sub_type_double)
            appendNum(*addrTo<double *>(src.address()), src.precision());

        return (*this);
    };

    template <typename T = int>
    auto appendNum(T value, int precision = 0) -> typename MB_ENABLE_IF<is_num_int<T>::value || is_bool<T>::value, MB_String &>::type
    {
        char *s = NULL;

        if (is_bool<T>::value)
            s = boolStr(value);
        else if (is_num_neg_int<T>::value)
        {
#if defined(ARDUINO_ARCH_SAMD) || defined(__AVR_ATmega4809__) || defined(ARDUINO_NANO_RP2040_CONNECT)
            s = int32Str(value);
#else
            s = int64Str(value);
#endif
        }
        else if (is_num_pos_int<T>::value)
        {
#if defined(ARDUINO_ARCH_SAMD) || defined(__AVR_ATmega4809__) || defined(ARDUINO_NANO_RP2040_CONNECT)
            s = uint32Str(value);
#else
            s = uint64Str(value);
#endif
        }

        if (s)
        {
            *this += s;
            delP(&s);
        }

        return (*this);
    }

    MB_String &appendNum(float value, int precision = 5)
    {
        if (precision < 0)
            precision = 5;

        char *s = toFloatStr(value, 0, precision);
        if (s)
        {
            *this += s;
            delP(&s);
        }
        return (*this);
    }

    MB_String &appendNum(double value, int precision = 9)
    {
        if (precision < 0)
            precision = 9;

        char *s = toFloatStr(value, 1, precision);
        if (s)
        {
            *this += s;
            delP(&s);
        }
        return (*this);
    }

    MB_String &appendNum(long double value, int precision = 9)
    {
        if (precision < 0)
            precision = 9;

        char *s = toFloatStr(value, 2, precision);
        if (s)
        {
            *this += s;
            delP(&s);
        }
        return (*this);
    }

    MB_String &operator=(const char *cstr)
    {
        if (cstr)
            copy(cstr, strlen_P(cstr));
        else
            clear();

        return *this;
    }

    MB_String &operator=(char c)
    {
        clear();
        if (_reserve(1, false))
        {
            *(buf) = c;
            *(buf + 1) = '\0';
        }

        return *this;
    }

    void trim()
    {
        int p1 = 0, p2 = length() - 1;
        while (p1 < (int)length())
        {
            if (buf[p1] > 32)
                break;
            p1++;
        }

        while (p2 >= 0)
        {
            if (buf[p2] > 32)
                break;
            p2--;
        }

        if (p1 == (int)length() && p2 < 0)
        {
            clear();
            return;
        }

        if (p2 >= p1 && p2 >= 0 && p1 < (int)length())
        {
            memmove(buf, buf + p1, p2 - p1 + 1);
            buf[p2 - p1 + 1] = '\0';
            _reserve(p2 - p1 + 1, true);
        }
    }

    void append(const char *cstr, size_t n)
    {
        if (!cstr)
            return;

        size_t slen = length();

        if (n > strlen(cstr))
            n = strlen(cstr);

        if (_reserve(slen + n, false))
        {
            memmove(buf + slen, cstr, n);
            *(buf + slen + n) = '\0';
        }
    }

    void append(size_t n, char c)
    {
        size_t slen = length();

        if (_reserve(slen + n, false))
        {
            for (size_t i = 0; i < n; i++)
                *(buf + slen + i) = c;
            *(buf + slen + n) = '\0';
        }
    }

    void prepend(char c)
    {
        size_t slen = length();
        size_t len = 1;

        if (maxLength() < slen + len)
            _reserve(slen + len, false);

        memmove(buf + len, buf, slen);
        buf[0] = c;
        buf[len + slen] = '\0';
    }

    void prepend(const char *cstr)
    {
        size_t slen = length();
        size_t len = strlen(cstr);

        if (maxLength() < slen + len)
            _reserve(slen + len, false);

        memmove(buf + len, buf, slen);
        memmove(buf, cstr, len);
        buf[len + slen] = '\0';
    }

    const char *c_str() const
    {
        if (!buf)
            return "";
        return (const char *)buf;
    }

    char operator[](size_t index) const
    {
        if (index >= bufLen || !buf)
            return 0;
        return buf[index];
    }

    char &operator[](size_t index)
    {
        static char c;
        if (index >= bufLen || !buf)
        {
            c = '\0';
            return c;
        }
        return buf[index];
    }

    void swap(MB_String &rhs)
    {
        rhs.clear();
    }

    void shrink_to_fit()
    {
        size_t slen = length();
        _reserve(slen, true);
    }

    void pop_back()
    {
        if (length() > 0)
        {
            size_t slen = length();
            if (slen > 0)
                buf[slen - 1] = '\0';
            _reserve(slen, true);
        }
    }

    size_t size() const
    {
        return length();
    }

    size_t bufferLength() const
    {
        return bufLen;
    }

    size_t find(const MB_String &s, size_t index = 0) const
    {
        if (!s.buf)
            return -1;
        return strpos(buf, s.buf, index);
    }

    size_t find(const char *s, size_t index = 0) const
    {
        return strpos(buf, s, index);
    }

    size_t find(char c, size_t index = 0) const
    {
        return strpos(buf, c, index);
    }

    size_t rfind(const char *s, size_t index = npos) const
    {
        return rstrpos(buf, s, index);
    }

    size_t rfind(char c, size_t index = npos) const
    {
        return rstrpos(buf, c, index);
    }

    void erase(size_t index = 0, size_t len = npos)
    {

        if (!buf || index >= length())
            return;

        if (index + len > length() || len == npos)
            len = length() - index;

        int rightLen = length() - index - len;

        memmove(buf + index, buf + index + len, rightLen);

        buf[index + rightLen] = '\0';

        _reserve(length(), true);
    }

    size_t length() const
    {
        if (!buf)
            return 0;
        return strlen(buf);
    }

    MB_String substr(size_t offset, size_t len = npos) const
    {
        MB_String out;
        substr(out, offset, len);
        return out;
    }

    void substr(MB_String &out, size_t offset, size_t len = npos) const
    {
        if (len > 0 && length() > 0 && offset < length())
        {
            if (len > length() - offset)
                len = length() - offset;
            out.copy(buf + offset, len);
        }
    }

    void clear()
    {
#if defined(ESP8266_USE_EXTERNAL_HEAP)
        reset(1);
#else
        allocate(0, false);
#endif
    }

#if defined(ESP8266_USE_EXTERNAL_HEAP)
    void reset(size_t len)
    {
        if (len == 0)
            len = 4;
        ESP.setExternalHeap();
        if (buf)
            buf = (char *)realloc(buf, len);
        else
            buf = (char *)malloc(len);
        ESP.resetHeap();

        if (buf)
        {
            bufLen = len;
            memset(buf, 0, len);
        }
    }
#endif

    void resize(size_t len)
    {
        if (_reserve(len, true))
            buf[len] = '\0';
    }

    MB_String &replace(size_t pos, size_t len, const char *replace)
    {
        size_t repLen = strlen(replace);

        if (length() > 0 && length() > pos && repLen > 0)
        {
            if (pos + len > length())
                len = length() - pos;

            if (repLen > len)
            {
                size_t rightLen = length() - pos - len;

                if (maxLength() < length() + repLen - len)
                    _reserve(length() + repLen - len, false);

                memmove(buf + pos + repLen, buf + pos + len, rightLen);
                buf[pos + repLen + rightLen] = '\0';
            }

            memmove(buf + pos, replace, repLen);
        }

        return *this;
    }

    MB_String &replace(size_t pos, size_t len, const MB_String &replace)
    {
        return this->replace(pos, len, replace.c_str());
    }

    MB_String &insert(size_t pos, size_t n, char c)
    {
        size_t slen = length();

        size_t rightLen = slen - pos;

        if (maxLength() < slen + n)
            _reserve(slen + n, false);

        if (maxLength() >= slen + n)
        {
            memmove(buf + pos + n, buf + pos, rightLen);

            for (size_t i = 0; i < n; i++)
                *(buf + pos + i) = c;

            buf[pos + n + rightLen] = '\0';
        }

        return *this;
    }

    MB_String &insert(size_t pos, const char *cstr)
    {
        size_t insLen = strlen(cstr);

        if (length() > 0 && length() > pos && insLen > 0)
        {

            size_t rightLen = length() - pos;

            if (maxLength() < length() + insLen)
                _reserve(length() + insLen, false);

            memmove(buf + pos + insLen, buf + pos, rightLen);
            buf[pos + insLen + rightLen] = '\0';
            memmove(buf + pos, cstr, insLen);
        }

        return *this;
    }

    MB_String &insert(size_t pos, const MB_String &str)
    {
        return insert(pos, str.c_str());
    }

    MB_String &insert(size_t pos, char c)
    {
        char temp[2]{c, '\0'};
        return insert(pos, temp);
    }

    size_t find_first_of(const char *cstr, size_t pos = 0) const
    {
        if (!cstr)
            return -1;

        return strpos(buf, cstr, pos);
    }

    size_t find_first_of(const MB_String &str, size_t pos = 0) const
    {
        if (length() == 0 || pos >= length())
            return -1;

        return find_first_of(str.buf, pos);
    }

    size_t find_first_not_of(const char *cstr, size_t pos = 0) const
    {
        if (length() == 0 || pos >= length())
            return -1;

        int size = strcspn(buf + pos, cstr);
        if (size == 0)
        {
            while (size == 0 && pos < length())
            {
                size = strcspn(buf + pos, cstr);
                pos++;
            }

            if (pos > 0)
                pos--;
        }

        return pos;
    }

    size_t find_first_not_of(const MB_String &str, size_t pos = 0) const
    {
        if (length() == 0 || pos >= length() || str.length() == 0)
            return -1;

        return find_first_not_of(str.buf, pos);
    }

    size_t find_last_of(const char *cstr, size_t pos = npos) const
    {
        if (!cstr)
            return -1;

        return rstrpos(buf, cstr, pos);
    }

    size_t find_last_of(const MB_String &str, size_t pos = npos) const
    {
        if (str.length() == 0)
            return -1;

        return find_last_of(str.buf, pos);
    }

    size_t find_last_not_of(const char *cstr, size_t pos = npos) const
    {
        if (length() == 0)
            return -1;

        if (pos >= length())
            pos = length() - 1;

        int p = length() - 1;
        int size = strcspn(buf + p, cstr);
        if (size == 0)
        {
            while (size == 0 && p > 0)
            {
                size = strcspn(buf + p, cstr);
                p--;
            }
            p++;
        }

        return p;
    }

    size_t find_last_not_of(const MB_String &str, size_t pos = npos) const
    {
        if (str.length() == 0)
            return -1;

        return find_last_not_of(str.buf, pos);
    }

    MB_String & replaceAll(const char *find, const char *replace)
    {
        if (length() == 0)
            return *this;;

        int i, cnt = 0;
        int repLen = strlen(replace);
        int findLen = strlen(find);

        MB_String temp = buf;
        char *s = temp.buf;
        clear();

        for (i = 0; s[i] != '\0'; i++)
        {
            if (strstr(&s[i], find) == &s[i])
            {
                cnt++;
                i += findLen - 1;
            }
        }

        if (_reserve(i + cnt * (repLen - findLen) + 1, false))
        {
            i = 0;
            while (*s)
            {
                if (strstr(s, find) == s)
                {
                    strcpy(&buf[i], replace);
                    i += repLen;
                    s += findLen;
                }
                else
                    buf[i++] = *s++;
            }

            buf[i] = '\0';
        }

        temp.clear();

        return *this;
    }

    void replaceAll(const MB_String &find, const MB_String &replace)
    {
        replaceAll(find.c_str(), replace.c_str());
    }

    bool empty() const
    {
        return length() == 0;
    }

    void reserve(size_t len)
    {
        if (_reserve(len, false))
            buf[len] = '\0';
    }

    static const size_t npos = -1;

private:
#if defined(ARDUINO_ARCH_SAMD) || defined(__AVR_ATmega4809__) || defined(ARDUINO_NANO_RP2040_CONNECT)

    char *int32Str(signed long value)
    {
        char *t = (char *)newP(64);
        sprintf(t, (const char *)MBSTRING_FLASH_MCR("%ld"), value);
        return t;
    }

    char *uint32Str(unsigned long value)
    {
        char *t = (char *)newP(64);
        sprintf(t, (const char *)MBSTRING_FLASH_MCR("%lu"), value);
        return t;
    }

#endif

    char *int64Str(signed long long value)
    {
        char *t = (char *)newP(64);
        sprintf(t, (const char *)MBSTRING_FLASH_MCR("%lld"), value);
        return t;
    }

    char *uint64Str(unsigned long long value)
    {
        char *t = (char *)newP(64);
        sprintf(t, (const char *)MBSTRING_FLASH_MCR("%llu"), value);
        return t;
    }

    char *boolStr(bool value)
    {
        char *t = (char *)newP(8);
        value ? strcpy(t, (const char *)MBSTRING_FLASH_MCR("true")) : strcpy(t, (const char *)MBSTRING_FLASH_MCR("false"));
        return t;
    }

    char *toFloatStr(long double value, int type, int precision)
    {
        int width = type == 0 ? 32 : 64;

        char *t = (char *)newP(width);

        if (t)
        {
            MB_String fmt = MBSTRING_FLASH_MCR("%.");
            fmt += precision;
            if (type == 2)
                fmt += MBSTRING_FLASH_MCR("L");
            fmt += MBSTRING_FLASH_MCR("f");
            sprintf(t, fmt.c_str(), value);
            trim(t);
        }

        return t;
    }

    char *nullStr()
    {
        char *t = (char *)newP(6);
        strcpy(t, (const char *)MBSTRING_FLASH_MCR("null"));
        return t;
    }

    char *pgmStr(PGM_P p)
    {
        char *t = (char *)newP(strlen_P(p));
        strcpy_P(t, p);
        return t;
    }

    void trim(char *s)
    {
        if (!s)
            return;
        size_t i = strlen(s) - 1;
        while (s[i] == '0' && i > 0)
        {
            if (s[i - 1] == '.')
            {
                i--;
                break;
            }
            if (s[i - 1] != '0')
                break;
            i--;
        }
        if (i < strlen(s) - 1)
            s[i] = '\0';
    }

    void *newP(size_t len)
    {
        void *p;
        size_t newLen = getReservedLen(len);
#if defined(BOARD_HAS_PSRAM) && defined(MB_STRING_USE_PSRAM)
        if (ESP.getPsramSize() > 0)
            p = (void *)ps_malloc(newLen);
        else
            p = (void *)malloc(newLen);
        if (!p)
            return NULL;

#else

#if defined(ESP8266_USE_EXTERNAL_HEAP)
        ESP.setExternalHeap();
#endif

        p = (void *)malloc(newLen);
        bool nn = p ? true : false;

#if defined(ESP8266_USE_EXTERNAL_HEAP)
        ESP.resetHeap();
#endif

        if (!nn)
            return NULL;

#endif
        memset(p, 0, newLen);
        return p;
    }

    void delP(void *ptr)
    {
        void **p = (void **)ptr;
        if (*p)
        {
            free(*p);
            *p = 0;
        }
    }

    size_t getReservedLen(size_t len)
    {
        int blen = len + 1;

        int newlen = (blen / 4) * 4;

        if (newlen < blen)
            newlen += 4;

        return (size_t)newlen;
    }

    size_t maxLength() const
    {
        if (bufferLength() == 0)
            return 0;
        return bufferLength() - 1;
    }

    void concat(const MB_String &s)
    {
        if (s.length() == 0)
            return;

        if (&s == this)
        {
            size_t slen = length();

            if (2 * slen > maxLength())
            {
                if (!_reserve(2 * slen, false))
                    return;
            }

            memmove(buf + slen, buf, slen);
            buf[2 * slen] = '\0';
        }
        else
        {
            concat(s.buf, s.length());
        }
    }

    void concat(const char *cstr, size_t len)
    {
        if (!cstr)
            return;

        size_t slen = length();

        if (slen + len > maxLength())
        {
            if (!_reserve(slen + len, false))
                return;
        }

        memmove(buf + slen, cstr, len);
        buf[slen + len] = '\0';
    }

    void concat(const char *cstr)
    {
        if (!cstr)
            return;

        concat(cstr, strlen(cstr));
    }

    void move(MB_String &rhs)
    {
        if (buf)
        {
            if (bufLen >= rhs.bufLen)
            {
                strcpy(buf, rhs.buf);
                bufLen = rhs.bufLen;
                rhs.bufLen = 0;
                return;
            }
            else
            {
                free(buf);
            }
        }
        buf = rhs.buf;
        bufLen = rhs.bufLen;
        rhs.buf = NULL;
    }

    void allocate(size_t len, bool shrink)
    {

        if (len == 0)
        {
            if (buf)
                free(buf);
            buf = NULL;
            bufLen = 0;
            return;
        }

        if (len > bufLen || shrink)
        {

#if defined(ESP8266_USE_EXTERNAL_HEAP)
            ESP.setExternalHeap();
#endif

            if (shrink || (bufLen > 0 && buf))
            {
                int slen = length();

#if defined(BOARD_HAS_PSRAM) && defined(MB_STRING_USE_PSRAM)
                if (ESP.getPsramSize() > 0)
                    buf = (char *)ps_realloc(buf, len);
                else
                    buf = (char *)realloc(buf, len);
#else
                buf = (char *)realloc(buf, len);
#endif
                if (buf)
                {
                    buf[slen] = '\0';
                    bufLen = len;
                }
            }
            else
            {
#if defined(BOARD_HAS_PSRAM) && defined(MB_STRING_USE_PSRAM)
                if (ESP.getPsramSize() > 0)
                    buf = (char *)ps_malloc(len);
                else
                    buf = (char *)malloc(len);
#else
                buf = (char *)malloc(len);
#endif
                if (buf)
                {
                    buf[0] = '\0';
                    bufLen = len;
                }
            }

#if defined(ESP8266_USE_EXTERNAL_HEAP)
            ESP.resetHeap();
#endif
        }
    }

    MB_String &copy(const char *cstr, size_t length)
    {

        if (!_reserve(length, false))
        {
            clear();
            return *this;
        }

        memcpy_P(buf, (PGM_P)cstr, length);
        buf[length] = '\0';

        return *this;
    }

    bool _reserve(size_t len, bool shrink)
    {

        size_t newlen = getReservedLen(len);
        if (shrink)
            allocate(newlen, true);
        else if (newlen > bufLen)
            allocate(newlen, false);

        return newlen <= bufLen;
    }

    int strpos(const char *haystack, const char *needle, int offset) const
    {
        if (!haystack || !needle)
            return -1;

        int hlen = strlen(haystack);
        int nlen = strlen(needle);

        if (hlen == 0 || nlen == 0)
            return -1;

        int hidx = offset, nidx = 0;
        while ((*(haystack + hidx) != '\0') && (*(needle + nidx) != '\0') && hidx < hlen)
        {
            if (*(needle + nidx) != *(haystack + hidx))
            {
                hidx++;
                nidx = 0;
            }
            else
            {
                nidx++;
                hidx++;
                if (nidx == nlen)
                    return hidx - nidx;
            }
        }

        return -1;
    }

    int strpos(const char *haystack, char needle, int offset) const
    {
        if (!haystack || needle == 0)
            return -1;

        int hlen = strlen(haystack);

        if (hlen == 0)
            return -1;

        int hidx = offset;
        while ((*(haystack + hidx) != '\0') && hidx < hlen)
        {
            if (needle == *(haystack + hidx))
                return hidx;
            hidx++;
        }

        return -1;
    }

    int rstrpos(const char *haystack, const char *needle, int offset /* start search from this offset to the left string */) const
    {
        if (!haystack || !needle)
            return -1;

        int hlen = strlen(haystack);
        int nlen = strlen(needle);

        if (hlen == 0 || nlen == 0)
            return -1;

        int hidx = offset;

        if (hidx >= hlen || (size_t)offset == npos)
            hidx = hlen - 1;

        int nidx = nlen - 1;

        while (hidx >= 0)
        {
            if (*(needle + nidx) != *(haystack + hidx))
            {
                hidx--;
                nidx = nlen - 1;
            }
            else
            {
                if (nidx == 0)
                    return hidx + nidx;
                nidx--;
                hidx--;
            }
        }

        return -1;
    }

    int rstrpos(const char *haystack, char needle, int offset /* start search from this offset to the left char */) const
    {
        if (!haystack || needle == 0)
            return -1;

        int hlen = strlen(haystack);

        if (hlen == 0)
            return -1;

        int hidx = offset;

        if (hidx >= hlen || (size_t)offset == npos)
            hidx = hlen - 1;

        while (hidx >= 0)
        {
            if (needle == *(haystack + hidx))
                return hidx;
            hidx--;
        }

        return -1;
    }

    int compareTo(const MB_String &s) const
    {
        if (!buf || !s.buf)
        {
            if (s.buf && s.length() > 0)
                return 0 - *(unsigned char *)s.buf;
            if (buf && length() > 0)
                return *(unsigned char *)buf;
            return 0;
        }
        return strcmp(buf, s.buf);
    }

    unsigned char equals(const MB_String &s2) const
    {
        return (length() == s2.length() && compareTo(s2) == 0);
    }

    unsigned char equals(const char *cstr) const
    {
        if (length() == 0)
            return (cstr == NULL || *cstr == 0);
        if (cstr == NULL)
            return buf[0] == 0;
        return strcmp(buf, cstr) == 0;
    }

    char *ultoa(unsigned long value, char *str, int radix)
    {
        const char *format = NULL;

        switch (radix)
        {
        case 8:
            format = "%o";
            break;
        case 10:
            format = "%ul";
            break;
        case 16:
            format = "%x";
            break;
        }

        if (format == NULL)
            return str;

        int size = sprintf(str, format, value);
        return &str[size];
    }

#if (!defined(ESP32) && !defined(ESP8266) && !defined(ARDUINO_ARCH_STM32) && !defined(ARDUINO_ARCH_SAMD)) || defined(ARDUINO_NANO_RP2040_CONNECT)
    char *ltoa(long value, char *str, int radix)
    {
        const char *format = NULL;

        switch (radix)
        {
        case 8:
            format = "%o";
            break;
        case 10:
            format = "%l";
            break;
        case 16:
            format = "%x";
            break;
        }

        if (format == NULL)
            return str;

        int size = sprintf(str, format, value);
        return &str[size];
    }

    char *utoa(unsigned int value, char *str, int radix)
    {
        const char *format = NULL;

        switch (radix)
        {
        case 8:
            format = "%o";
            break;
        case 10:
            format = "%u";
            break;
        case 16:
            format = "%x";
            break;
        }

        if (format == NULL)
            return str;

        int size = sprintf(str, format, value);
        return &str[size];
    }

    char *itoa(int value, char *str, int radix)
    {
        const char *format = NULL;

        switch (radix)
        {
        case 8:
            format = "%o";
            break;
        case 10:
            format = "%d";
            break;
        case 16:
            format = "%x";
            break;
        }

        if (format == NULL)
            return str;

        int size = sprintf(str, format, value);
        return &str[size];
    }
#endif

    char *buf = NULL;
    size_t bufLen = 0;
};

inline MB_String operator+(const MB_String &lhs, const MB_String &rhs)
{
    MB_String res;
    res.reserve(lhs.length() + rhs.length());
    res += lhs;
    res += rhs;
    return res;
}

#if !defined(__AVR__)

inline MB_String operator+(MB_String &&lhs, const MB_String &rhs)
{
    lhs += rhs;
    return std::move(lhs);
}

inline MB_String operator+(MB_String &lhs, MB_String &&rhs)
{
    lhs += rhs;
    return std::move(lhs);
}

inline MB_String operator+(MB_String &lhs, char rhs)
{
    lhs += rhs;
    return std::move(lhs);
}

inline MB_String operator+(char lhs, MB_String &rhs)
{
    rhs.insert(0, lhs);
    return rhs;
}

inline MB_String operator+(MB_String &&lhs, char rhs)
{
    return std::move(lhs.insert(0, rhs));
}

#endif

#endif