#include "ArduinoString.h"

#include <cstring>

String::String() : data_(nullptr), length_(0), capacity_(0) {}

String::String(const char* value) : String() {
    assign(value);
}

String::String(const String& other) : String(other.c_str()) {}

String::~String() {
    delete[] data_;
}

String& String::operator=(const char* value) {
    assign(value);
    return *this;
}

String& String::operator=(const String& other) {
    if (this != &other) {
        assign(other.c_str());
    }
    return *this;
}

void String::grow(size_t length) {
    if (length <= capacity_) {
        return;
    }
    char* replacement = new char[length + 1];
    if (data_ != nullptr) {
        std::memcpy(replacement, data_, length_ + 1);
    }
    delete[] data_;
    data_ = replacement;
    capacity_ = length;
}

void String::assign(const char* value) {
    if (value == nullptr) {
        clear();
        return;
    }
    const size_t newLength = std::strlen(value);
    grow(newLength);
    if (newLength != 0) {
        std::memmove(data_, value, newLength);
    }
    if (data_ != nullptr) {
        data_[newLength] = '\0';
    }
    length_ = newLength;
}

const char* String::c_str() const {
    return data_ != nullptr ? data_ : "";
}

size_t String::length() const {
    return length_;
}

bool String::isEmpty() const {
    return length_ == 0;
}

void String::clear() {
    length_ = 0;
    if (data_ != nullptr) {
        data_[0] = '\0';
    }
}

bool String::operator==(const String& other) const {
    return length_ == other.length_ && std::memcmp(c_str(), other.c_str(), length_) == 0;
}

bool String::operator==(const char* other) const {
    return other != nullptr && std::strcmp(c_str(), other) == 0;
}
