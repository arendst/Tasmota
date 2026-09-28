/* Minimal host implementation of the Arduino String API used by the library. */
#ifndef TASMOTA_PUBSUB_TEST_ARDUINO_STRING_H
#define TASMOTA_PUBSUB_TEST_ARDUINO_STRING_H

#include <cstddef>

class String {
public:
    String();
    String(const char* value);
    String(const String& other);
    ~String();

    String& operator=(const char* value);
    String& operator=(const String& other);

    const char* c_str() const;
    size_t length() const;
    bool isEmpty() const;
    void clear();

    bool operator==(const String& other) const;
    bool operator==(const char* other) const;

private:
    void assign(const char* value);
    void grow(size_t length);

    char* data_;
    size_t length_;
    size_t capacity_;
};

inline bool operator==(const char* lhs, const String& rhs) {
    return rhs == lhs;
}

#endif  // TASMOTA_PUBSUB_TEST_ARDUINO_STRING_H
