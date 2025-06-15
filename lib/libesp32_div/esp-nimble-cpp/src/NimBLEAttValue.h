/*
 * Copyright 2020-2025 Ryan Powell <ryan@nable-embedded.io> and
 * esp-nimble-cpp, NimBLE-Arduino contributors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NIMBLE_CPP_ATTVALUE_H
#define NIMBLE_CPP_ATTVALUE_H

#include "nimconfig.h"
#if CONFIG_BT_ENABLED

# ifdef NIMBLE_CPP_ARDUINO_STRING_AVAILABLE
#  include <Arduino.h>
# endif

# include <string>
# include <vector>
# include <ctime>
# include <cstring>
# include <cstdint>

# ifndef CONFIG_NIMBLE_CPP_ATT_VALUE_TIMESTAMP_ENABLED
#  define CONFIG_NIMBLE_CPP_ATT_VALUE_TIMESTAMP_ENABLED 0
# endif

# ifndef BLE_ATT_ATTR_MAX_LEN
#  define BLE_ATT_ATTR_MAX_LEN 512
# endif

# if !defined(CONFIG_NIMBLE_CPP_ATT_VALUE_INIT_LENGTH)
#  define CONFIG_NIMBLE_CPP_ATT_VALUE_INIT_LENGTH 20
# elif CONFIG_NIMBLE_CPP_ATT_VALUE_INIT_LENGTH > BLE_ATT_ATTR_MAX_LEN
#  error CONFIG_NIMBLE_CPP_ATT_VALUE_INIT_LENGTH cannot be larger than 512 (BLE_ATT_ATTR_MAX_LEN)
# elif CONFIG_NIMBLE_CPP_ATT_VALUE_INIT_LENGTH < 1
#  error CONFIG_NIMBLE_CPP_ATT_VALUE_INIT_LENGTH cannot be less than 1; Range = 1 : 512
# endif

/* Used to determine if the type passed to a template has a data() and size() method. */
template <typename T, typename = void, typename = void>
struct Has_data_size : std::false_type {};

template <typename T>
struct Has_data_size<T, decltype(void(std::declval<T&>().data())), decltype(void(std::declval<T&>().size()))>
    : std::true_type {};

/* Used to determine if the type passed to a template has a c_str() and length() method. */
template <typename T, typename = void, typename = void>
struct Has_c_str_length : std::false_type {};

template <typename T>
struct Has_c_str_length<T, decltype(void(std::declval<T&>().c_str())), decltype(void(std::declval<T&>().length()))>
    : std::true_type {};

/**
 * @brief A specialized container class to hold BLE attribute values.
 * @details This class is designed to be more memory efficient than using\n
 * standard container types for value storage, while being convertible to\n
 * many different container classes.
 */
class NimBLEAttValue {
    uint8_t* m_attr_value{};
    uint16_t m_attr_max_len{};
    uint16_t m_attr_len{};
    uint16_t m_capacity{};
# if CONFIG_NIMBLE_CPP_ATT_VALUE_TIMESTAMP_ENABLED
    time_t m_timestamp{};
# endif
    void deepCopy(const NimBLEAttValue& source);

  public:
    /**
     * @brief Default constructor.
     * @param[in] init_len The initial size in bytes.
     * @param[in] max_len The max size in bytes that the value can be.
     */
    NimBLEAttValue(uint16_t init_len = CONFIG_NIMBLE_CPP_ATT_VALUE_INIT_LENGTH, uint16_t max_len = BLE_ATT_ATTR_MAX_LEN);

    /**
     * @brief Construct with an initial value from a buffer.
     * @param value A pointer to the initial value to set.
     * @param[in] len The size in bytes of the value to set.
     * @param[in] max_len The max size in bytes that the value can be.
     */
    NimBLEAttValue(const uint8_t* value, uint16_t len, uint16_t max_len = BLE_ATT_ATTR_MAX_LEN);

    /**
     * @brief Construct with an initial value from a const char string.
     * @param value A pointer to the initial value to set.
     * @param[in] max_len The max size in bytes that the value can be.
     */
    NimBLEAttValue(const char* value, uint16_t max_len = BLE_ATT_ATTR_MAX_LEN)
        : NimBLEAttValue((uint8_t*)value, (uint16_t)strlen(value), max_len) {}

    /**
     * @brief Construct with an initializer list.
     * @param list An initializer list containing the initial value to set.
     * @param[in] max_len The max size in bytes that the value can be.
     */
    NimBLEAttValue(std::initializer_list<uint8_t> list, uint16_t max_len = BLE_ATT_ATTR_MAX_LEN)
        : NimBLEAttValue(list.begin(), list.size(), max_len) {}

    /**
     * @brief Construct with an initial value from a std::string.
     * @param str A std::string containing to the initial value to set.
     * @param[in] max_len The max size in bytes that the value can be.
     */
    NimBLEAttValue(const std::string str, uint16_t max_len = BLE_ATT_ATTR_MAX_LEN)
        : NimBLEAttValue(reinterpret_cast<const uint8_t*>(&str[0]), str.length(), max_len) {}

    /**
     * @brief Construct with an initial value from a std::vector<uint8_t>.
     * @param vec A std::vector<uint8_t> containing to the initial value to set.
     * @param[in] max_len The max size in bytes that the value can be.
     */
    NimBLEAttValue(const std::vector<uint8_t> vec, uint16_t max_len = BLE_ATT_ATTR_MAX_LEN)
        : NimBLEAttValue(&vec[0], vec.size(), max_len) {}

# ifdef NIMBLE_CPP_ARDUINO_STRING_AVAILABLE
    /**
     * @brief Construct with an initial value from an Arduino String.
     * @param str An Arduino String containing to the initial value to set.
     * @param[in] max_len The max size in bytes that the value can be.
     */
    NimBLEAttValue(const String str, uint16_t max_len = BLE_ATT_ATTR_MAX_LEN)
        : NimBLEAttValue(reinterpret_cast<const uint8_t*>(str.c_str()), str.length(), max_len) {}
# endif

    /** @brief Copy constructor */
    NimBLEAttValue(const NimBLEAttValue& source) { deepCopy(source); }

    /** @brief Move constructor */
    NimBLEAttValue(NimBLEAttValue&& source) { *this = std::move(source); }

    /** @brief Destructor */
    ~NimBLEAttValue();

    /** @brief Returns the max size in bytes */
    uint16_t max_size() const { return m_attr_max_len; }

    /** @brief Returns the currently allocated capacity in bytes */
    uint16_t capacity() const { return m_capacity; }

    /** @brief Returns the current length of the value in bytes */
    uint16_t length() const { return m_attr_len; }

    /** @brief Returns the current size of the value in bytes */
    uint16_t size() const { return m_attr_len; }

    /** @brief Returns a pointer to the internal buffer of the value */
    const uint8_t* data() const { return m_attr_value; }

    /** @brief Returns a pointer to the internal buffer of the value as a const char* */
    const char* c_str() const { return reinterpret_cast<const char*>(m_attr_value); }

    /** @brief Iterator begin */
    const uint8_t* begin() const { return m_attr_value; }

    /** @brief Iterator end */
    const uint8_t* end() const { return m_attr_value + m_attr_len; }

# if CONFIG_NIMBLE_CPP_ATT_VALUE_TIMESTAMP_ENABLED
    /** @brief Returns a timestamp of when the value was last updated */
    time_t getTimeStamp() const { return m_timestamp; }

    /** @brief Set the timestamp to the current time */
    void setTimeStamp() { m_timestamp = time(nullptr); }

    /**
     * @brief Set the timestamp to the specified time
     * @param[in] t The timestamp value to set
     */
    void setTimeStamp(time_t t) { m_timestamp = t; }
# else
    time_t getTimeStamp() const { return 0; }
    void   setTimeStamp() {}
    void   setTimeStamp(time_t t) {}
# endif

    /**
     * @brief Set the value from a buffer
     * @param[in] value A pointer to a buffer containing the value.
     * @param[in] len The length of the value in bytes.
     * @returns True if successful.
     */
    bool setValue(const uint8_t* value, uint16_t len);

    /**
     * @brief Set value to the value of const char*.
     * @param [in] s A pointer to a const char value to set.
     * @param [in] len The length of the value in bytes, defaults to strlen(s).
     */
    bool setValue(const char* s, uint16_t len = 0) {
        if (len == 0) {
            len = strlen(s);
        }
        return setValue(reinterpret_cast<const uint8_t*>(s), len);
    }

    const NimBLEAttValue& getValue(time_t* timestamp = nullptr) const {
        if (timestamp != nullptr) {
# if CONFIG_NIMBLE_CPP_ATT_VALUE_TIMESTAMP_ENABLED
            *timestamp = m_timestamp;
# else
            *timestamp = 0;
# endif
        }
        return *this;
    }

    /**
     * @brief Append data to the value.
     * @param[in] value A ponter to a data buffer with the value to append.
     * @param[in] len The length of the value to append in bytes.
     * @returns A reference to the appended NimBLEAttValue.
     */
    NimBLEAttValue& append(const uint8_t* value, uint16_t len);

    /*********************** Template Functions ************************/

# if __cplusplus < 201703L
    /**
     * @brief Template to set value to the value of <type\>val.
     * @param [in] v The <type\>value to set.
     * @details Only used for types without a `c_str()` and `length()` or `data()` and `size()` method.
     * <type\> size must be evaluatable by `sizeof()`.
     */
    template <typename T>
#  ifdef _DOXYGEN_
    bool
#  else
    typename std::enable_if<!std::is_pointer<T>::value && !Has_c_str_length<T>::value && !Has_data_size<T>::value, bool>::type
#  endif
    setValue(const T& v) {
        return setValue(reinterpret_cast<const uint8_t*>(&v), sizeof(T));
    }

    /**
     * @brief Template to set value to the value of <type\>val.
     * @param [in] s The <type\>value to set.
     * @details Only used if the <type\> has a `c_str()` method.
     */
    template <typename T>
#  ifdef _DOXYGEN_
    bool
#  else
    typename std::enable_if<Has_c_str_length<T>::value && !Has_data_size<T>::value, bool>::type
#  endif
    setValue(const T& s) {
        return setValue(reinterpret_cast<const uint8_t*>(s.c_str()), s.length());
    }

    /**
     * @brief Template to set value to the value of <type\>val.
     * @param [in] v The <type\>value to set.
     * @details Only used if the <type\> has a `data()` and `size()` method.
     */
    template <typename T>
#  ifdef _DOXYGEN_
    bool
#  else
    typename std::enable_if<Has_data_size<T>::value, bool>::type
#  endif
    setValue(const T& v) {
        return setValue(reinterpret_cast<const uint8_t*>(v.data()), v.size());
    }

# else
    /**
     * @brief Template to set value to the value of <type\>val.
     * @param [in] s The <type\>value to set.
     * @note This function is only available if the type T is not a pointer.
     */
    template <typename T>
    typename std::enable_if<!std::is_pointer<T>::value, bool>::type setValue(const T& s) {
        if constexpr (Has_data_size<T>::value) {
            return setValue(reinterpret_cast<const uint8_t*>(s.data()), s.size());
        } else if constexpr (Has_c_str_length<T>::value) {
            return setValue(reinterpret_cast<const uint8_t*>(s.c_str()), s.length());
        } else {
            return setValue(reinterpret_cast<const uint8_t*>(&s), sizeof(s));
        }
    }
# endif

    /**
     * @brief Template to return the value as a <type\>.
     * @tparam T The type to convert the data to.
     * @param [in] timestamp A pointer to a time_t struct to store the time the value was read.
     * @param [in] skipSizeCheck If true it will skip checking if the data size is less than\n
     * <tt>sizeof(<type\>)</tt>.
     * @return The data converted to <type\> or NULL if skipSizeCheck is false and the data is\n
     * less than <tt>sizeof(<type\>)</tt>.
     * @details <b>Use:</b> <tt>getValue<type>(&timestamp, skipSizeCheck);</tt>
     */
    template <typename T>
    T getValue(time_t* timestamp = nullptr, bool skipSizeCheck = false) const {
        if (timestamp != nullptr) {
# if CONFIG_NIMBLE_CPP_ATT_VALUE_TIMESTAMP_ENABLED
            *timestamp = m_timestamp;
# else
            *timestamp = 0;
# endif
        }

        if (!skipSizeCheck && size() < sizeof(T)) {
            return T();
        }
        return *(reinterpret_cast<const T*>(m_attr_value));
    }

    /*********************** Operators ************************/

    /** @brief Subscript operator */
    uint8_t operator[](int pos) const;

    /** @brief Operator; Get the value as a std::vector<uint8_t>. */
    operator std::vector<uint8_t>() const { return std::vector<uint8_t>(m_attr_value, m_attr_value + m_attr_len); }

    /** @brief Operator; Get the value as a std::string. */
    operator std::string() const { return std::string(reinterpret_cast<char*>(m_attr_value), m_attr_len); }

    /** @brief Operator; Get the value as a const uint8_t*. */
    operator const uint8_t*() const { return m_attr_value; }

    /** @brief Operator; Append another NimBLEAttValue. */
    NimBLEAttValue& operator+=(const NimBLEAttValue& source) { return append(source.data(), source.size()); }

    /** @brief Operator; Set the value from a std::string source. */
    NimBLEAttValue& operator=(const std::string& source) {
        setValue(reinterpret_cast<const uint8_t*>(&source[0]), source.size());
        return *this;
    }

    /** @brief Move assignment operator */
    NimBLEAttValue& operator=(NimBLEAttValue&& source);

    /** @brief Copy assignment operator */
    NimBLEAttValue& operator=(const NimBLEAttValue& source);

    /** @brief Equality operator */
    bool operator==(const NimBLEAttValue& source) const {
        return (m_attr_len == source.size()) ? memcmp(m_attr_value, source.data(), m_attr_len) == 0 : false;
    }

    /** @brief Inequality operator */
    bool operator!=(const NimBLEAttValue& source) const { return !(*this == source); }

# ifdef NIMBLE_CPP_ARDUINO_STRING_AVAILABLE
    /** @brief Operator; Get the value as an Arduino String value. */
    operator String() const { return String(reinterpret_cast<char*>(m_attr_value)); }
# endif
};

#endif // CONFIG_BT_ENABLED
#endif // NIMBLE_CPP_ATTVALUE_H_
