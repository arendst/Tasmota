// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#ifdef __cplusplus

#if __cplusplus >= 201103L
#define ARDUINOJSON_HAS_LONG_LONG 1
#define ARDUINOJSON_HAS_NULLPTR 1
#define ARDUINOJSON_HAS_RVALUE_REFERENCES 1
#else
#define ARDUINOJSON_HAS_LONG_LONG 0
#define ARDUINOJSON_HAS_NULLPTR 0
#define ARDUINOJSON_HAS_RVALUE_REFERENCES 0
#endif
#if defined(_MSC_VER) && !ARDUINOJSON_HAS_LONG_LONG
#define ARDUINOJSON_HAS_INT64 1
#else
#define ARDUINOJSON_HAS_INT64 0
#endif
#ifndef ARDUINOJSON_EMBEDDED_MODE
#if defined(ARDUINO)                /* Arduino*/                 \
    || defined(__IAR_SYSTEMS_ICC__) /* IAR Embedded Workbench */ \
    || defined(__XC)                /* MPLAB XC compiler */      \
    || defined(__ARMCC_VERSION)     /* Keil ARM Compiler */      \
    || defined(__AVR)               /* Atmel AVR8/GNU C Compiler */
#define ARDUINOJSON_EMBEDDED_MODE 1
#else
#define ARDUINOJSON_EMBEDDED_MODE 0
#endif
#endif
#if !defined(ARDUINOJSON_ENABLE_STD_STREAM) && defined(__has_include)
#if __has_include(<istream>) && \
    __has_include(<ostream>) && \
    !defined(min) && \
    !defined(max)
#define ARDUINOJSON_ENABLE_STD_STREAM 1
#else
#define ARDUINOJSON_ENABLE_STD_STREAM 0
#endif
#endif
#if !defined(ARDUINOJSON_ENABLE_STD_STRING) && defined(__has_include)
#if __has_include(<string>) && !defined(min) && !defined(max)
#define ARDUINOJSON_ENABLE_STD_STRING 1
#else
#define ARDUINOJSON_ENABLE_STD_STRING 0
#endif
#endif
#if ARDUINOJSON_EMBEDDED_MODE
#ifndef ARDUINOJSON_USE_DOUBLE
#define ARDUINOJSON_USE_DOUBLE 0
#endif
#ifndef ARDUINOJSON_USE_LONG_LONG
#define ARDUINOJSON_USE_LONG_LONG 0
#endif
#ifndef ARDUINOJSON_ENABLE_STD_STRING
#define ARDUINOJSON_ENABLE_STD_STRING 0
#endif
#ifndef ARDUINOJSON_ENABLE_STD_STREAM
#define ARDUINOJSON_ENABLE_STD_STREAM 0
#endif
#ifndef ARDUINOJSON_DEFAULT_NESTING_LIMIT
#define ARDUINOJSON_DEFAULT_NESTING_LIMIT 10
#endif
#ifndef ARDUINOJSON_SLOT_OFFSET_SIZE
#if defined(__SIZEOF_POINTER__) && __SIZEOF_POINTER__ == 2
#define ARDUINOJSON_SLOT_OFFSET_SIZE 1
#else
#define ARDUINOJSON_SLOT_OFFSET_SIZE 2
#endif
#endif
#else  // ARDUINOJSON_EMBEDDED_MODE
#ifndef ARDUINOJSON_USE_DOUBLE
#define ARDUINOJSON_USE_DOUBLE 1
#endif
#ifndef ARDUINOJSON_USE_LONG_LONG
#if ARDUINOJSON_HAS_LONG_LONG || ARDUINOJSON_HAS_INT64
#define ARDUINOJSON_USE_LONG_LONG 1
#else
#define ARDUINOJSON_USE_LONG_LONG 0
#endif
#endif
#ifndef ARDUINOJSON_ENABLE_STD_STRING
#define ARDUINOJSON_ENABLE_STD_STRING 1
#endif
#ifndef ARDUINOJSON_ENABLE_STD_STREAM
#define ARDUINOJSON_ENABLE_STD_STREAM 1
#endif
#ifndef ARDUINOJSON_DEFAULT_NESTING_LIMIT
#define ARDUINOJSON_DEFAULT_NESTING_LIMIT 50
#endif
#ifndef ARDUINOJSON_SLOT_OFFSET_SIZE
#define ARDUINOJSON_SLOT_OFFSET_SIZE 4
#endif
#endif  // ARDUINOJSON_EMBEDDED_MODE
#ifdef ARDUINO
#include <Arduino.h>
#ifndef ARDUINOJSON_ENABLE_ARDUINO_STRING
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#endif
#ifndef ARDUINOJSON_ENABLE_ARDUINO_STREAM
#define ARDUINOJSON_ENABLE_ARDUINO_STREAM 1
#endif
#ifndef ARDUINOJSON_ENABLE_ARDUINO_PRINT
#define ARDUINOJSON_ENABLE_ARDUINO_PRINT 1
#endif
#else  // ARDUINO
#ifndef ARDUINOJSON_ENABLE_ARDUINO_STRING
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 0
#endif
#ifndef ARDUINOJSON_ENABLE_ARDUINO_STREAM
#define ARDUINOJSON_ENABLE_ARDUINO_STREAM 0
#endif
#ifndef ARDUINOJSON_ENABLE_ARDUINO_PRINT
#define ARDUINOJSON_ENABLE_ARDUINO_PRINT 0
#endif
#endif  // ARDUINO
#ifndef ARDUINOJSON_ENABLE_PROGMEM
#if defined(PROGMEM) && defined(pgm_read_byte) && defined(pgm_read_dword) && \
    defined(pgm_read_ptr) && defined(pgm_read_float)
#define ARDUINOJSON_ENABLE_PROGMEM 1
#else
#define ARDUINOJSON_ENABLE_PROGMEM 0
#endif
#endif
#ifndef ARDUINOJSON_DECODE_UNICODE
#define ARDUINOJSON_DECODE_UNICODE 1
#endif
#ifndef ARDUINOJSON_ENABLE_COMMENTS
#define ARDUINOJSON_ENABLE_COMMENTS 0
#endif
#ifndef ARDUINOJSON_ENABLE_NAN
#define ARDUINOJSON_ENABLE_NAN 0
#endif
#ifndef ARDUINOJSON_ENABLE_INFINITY
#define ARDUINOJSON_ENABLE_INFINITY 0
#endif
#ifndef ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD
#define ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD 1e7
#endif
#ifndef ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD
#define ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD 1e-5
#endif
#ifndef ARDUINOJSON_LITTLE_ENDIAN
#if defined(_MSC_VER) ||                                                      \
    (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || \
    defined(__LITTLE_ENDIAN__) || defined(__i386) || defined(__x86_64)
#define ARDUINOJSON_LITTLE_ENDIAN 1
#else
#define ARDUINOJSON_LITTLE_ENDIAN 0
#endif
#endif
#ifndef ARDUINOJSON_ENABLE_ALIGNMENT
#if defined(__AVR)
#define ARDUINOJSON_ENABLE_ALIGNMENT 0
#else
#define ARDUINOJSON_ENABLE_ALIGNMENT 1
#endif
#endif
#ifndef ARDUINOJSON_TAB
#define ARDUINOJSON_TAB "  "
#endif
#ifndef ARDUINOJSON_ENABLE_STRING_DEDUPLICATION
#define ARDUINOJSON_ENABLE_STRING_DEDUPLICATION 1
#endif
#ifndef ARDUINOJSON_STRING_BUFFER_SIZE
#define ARDUINOJSON_STRING_BUFFER_SIZE 32
#endif
#ifndef ARDUINOJSON_DEBUG
#ifdef __PLATFORMIO_BUILD_DEBUG__
#define ARDUINOJSON_DEBUG 1
#else
#define ARDUINOJSON_DEBUG 0
#endif
#endif
#if ARDUINOJSON_HAS_NULLPTR && defined(nullptr)
#error nullptr is defined as a macro. Remove the faulty #define or #undef nullptr
#endif
#if !ARDUINOJSON_DEBUG
#ifdef __clang__
#pragma clang system_header
#elif defined __GNUC__
#pragma GCC system_header
#endif
#endif
#define ARDUINOJSON_EXPAND6(a, b, c, d, e, f) a, b, c, d, e, f
#define ARDUINOJSON_EXPAND7(a, b, c, d, e, f, g) a, b, c, d, e, f, g
#define ARDUINOJSON_EXPAND9(a, b, c, d, e, f, g, h, i) a, b, c, d, e, f, g, h, i
#define ARDUINOJSON_EXPAND18(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, \
                             q, r)                                           \
  a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r
#define ARDUINOJSON_CONCAT_(A, B) A##B
#define ARDUINOJSON_CONCAT2(A, B) ARDUINOJSON_CONCAT_(A, B)
#define ARDUINOJSON_CONCAT4(A, B, C, D) \
  ARDUINOJSON_CONCAT2(ARDUINOJSON_CONCAT2(A, B), ARDUINOJSON_CONCAT2(C, D))
#define ARDUINOJSON_HEX_DIGIT_0000() 0
#define ARDUINOJSON_HEX_DIGIT_0001() 1
#define ARDUINOJSON_HEX_DIGIT_0010() 2
#define ARDUINOJSON_HEX_DIGIT_0011() 3
#define ARDUINOJSON_HEX_DIGIT_0100() 4
#define ARDUINOJSON_HEX_DIGIT_0101() 5
#define ARDUINOJSON_HEX_DIGIT_0110() 6
#define ARDUINOJSON_HEX_DIGIT_0111() 7
#define ARDUINOJSON_HEX_DIGIT_1000() 8
#define ARDUINOJSON_HEX_DIGIT_1001() 9
#define ARDUINOJSON_HEX_DIGIT_1010() A
#define ARDUINOJSON_HEX_DIGIT_1011() B
#define ARDUINOJSON_HEX_DIGIT_1100() C
#define ARDUINOJSON_HEX_DIGIT_1101() D
#define ARDUINOJSON_HEX_DIGIT_1110() E
#define ARDUINOJSON_HEX_DIGIT_1111() F
#define ARDUINOJSON_HEX_DIGIT_(A, B, C, D) ARDUINOJSON_HEX_DIGIT_##A##B##C##D()
#define ARDUINOJSON_HEX_DIGIT(A, B, C, D) ARDUINOJSON_HEX_DIGIT_(A, B, C, D)
#define ARDUINOJSON_VERSION "6.17.3"
#define ARDUINOJSON_VERSION_MAJOR 6
#define ARDUINOJSON_VERSION_MINOR 17
#define ARDUINOJSON_VERSION_REVISION 3
#ifndef ARDUINOJSON_NAMESPACE
#define ARDUINOJSON_NAMESPACE                                                  \
  ARDUINOJSON_CONCAT4(                                                         \
      ARDUINOJSON_CONCAT4(ArduinoJson, ARDUINOJSON_VERSION_MAJOR,              \
                          ARDUINOJSON_VERSION_MINOR,                           \
                          ARDUINOJSON_VERSION_REVISION),                       \
      _,                                                                       \
      ARDUINOJSON_HEX_DIGIT(ARDUINOJSON_ENABLE_PROGMEM,                        \
                            ARDUINOJSON_USE_LONG_LONG, ARDUINOJSON_USE_DOUBLE, \
                            ARDUINOJSON_ENABLE_STRING_DEDUPLICATION),          \
      ARDUINOJSON_HEX_DIGIT(                                                   \
          ARDUINOJSON_ENABLE_NAN, ARDUINOJSON_ENABLE_INFINITY,                 \
          ARDUINOJSON_ENABLE_COMMENTS, ARDUINOJSON_DECODE_UNICODE))
#endif
#if ARDUINOJSON_DEBUG
#include <assert.h>
#define ARDUINOJSON_ASSERT(X) assert(X)
#else
#define ARDUINOJSON_ASSERT(X) ((void)0)
#endif
#include <stddef.h>
namespace ARDUINOJSON_NAMESPACE {
class MemoryPool;
class VariantData;
class VariantSlot;
class CollectionData {
  VariantSlot *_head;
  VariantSlot *_tail;
 public:
  VariantData *addElement(MemoryPool *pool);
  VariantData *getElement(size_t index) const;
  VariantData *getOrAddElement(size_t index, MemoryPool *pool);
  void removeElement(size_t index);
  bool equalsArray(const CollectionData &other) const;
  template <typename TAdaptedString>
  VariantData *addMember(TAdaptedString key, MemoryPool *pool);
  template <typename TAdaptedString>
  VariantData *getMember(TAdaptedString key) const;
  template <typename TAdaptedString>
  VariantData *getOrAddMember(TAdaptedString key, MemoryPool *pool);
  template <typename TAdaptedString>
  void removeMember(TAdaptedString key) {
    removeSlot(getSlot(key));
  }
  template <typename TAdaptedString>
  bool containsKey(const TAdaptedString &key) const;
  bool equalsObject(const CollectionData &other) const;
  void clear();
  size_t memoryUsage() const;
  size_t nesting() const;
  size_t size() const;
  VariantSlot *addSlot(MemoryPool *);
  void removeSlot(VariantSlot *slot);
  bool copyFrom(const CollectionData &src, MemoryPool *pool);
  VariantSlot *head() const {
    return _head;
  }
  void movePointers(ptrdiff_t stringDistance, ptrdiff_t variantDistance);
 private:
  VariantSlot *getSlot(size_t index) const;
  template <typename TAdaptedString>
  VariantSlot *getSlot(TAdaptedString key) const;
  VariantSlot *getPreviousSlot(VariantSlot *) const;
};
inline VariantData *arrayAdd(CollectionData *arr, MemoryPool *pool) {
  return arr ? arr->addElement(pool) : 0;
}
template <typename TVisitor>
inline typename TVisitor::result_type arrayAccept(const CollectionData *arr,
                                                  TVisitor &visitor) {
  if (arr)
    return visitor.visitArray(*arr);
  else
    return visitor.visitNull();
}
inline bool arrayEquals(const CollectionData *lhs, const CollectionData *rhs) {
  if (lhs == rhs)
    return true;
  if (!lhs || !rhs)
    return false;
  return lhs->equalsArray(*rhs);
}
#if ARDUINOJSON_ENABLE_ALIGNMENT
inline bool isAligned(size_t value) {
  const size_t mask = sizeof(void *) - 1;
  size_t addr = value;
  return (addr & mask) == 0;
}
inline size_t addPadding(size_t bytes) {
  const size_t mask = sizeof(void *) - 1;
  return (bytes + mask) & ~mask;
}
template <size_t bytes>
struct AddPadding {
  static const size_t mask = sizeof(void *) - 1;
  static const size_t value = (bytes + mask) & ~mask;
};
#else
inline bool isAligned(size_t) {
  return true;
}
inline size_t addPadding(size_t bytes) {
  return bytes;
}
template <size_t bytes>
struct AddPadding {
  static const size_t value = bytes;
};
#endif
template <typename T>
inline bool isAligned(T *ptr) {
  return isAligned(reinterpret_cast<size_t>(ptr));
}
template <typename T>
inline T *addPadding(T *p) {
  size_t address = addPadding(reinterpret_cast<size_t>(p));
  return reinterpret_cast<T *>(address);
}
template <size_t X, size_t Y, bool MaxIsX = (X > Y)>
struct Max {};
template <size_t X, size_t Y>
struct Max<X, Y, true> {
  static const size_t value = X;
};
template <size_t X, size_t Y>
struct Max<X, Y, false> {
  static const size_t value = Y;
};
}  // namespace ARDUINOJSON_NAMESPACE
#include <stdint.h>
namespace ARDUINOJSON_NAMESPACE {
template <int Bits>
struct int_t;
template <>
struct int_t<8> {
  typedef int8_t type;
};
template <>
struct int_t<16> {
  typedef int16_t type;
};
template <>
struct int_t<32> {
  typedef int32_t type;
};
template <bool Condition, class TrueType, class FalseType>
struct conditional {
  typedef TrueType type;
};
template <class TrueType, class FalseType>
struct conditional<false, TrueType, FalseType> {
  typedef FalseType type;
};
template <bool Condition, typename T = void>
struct enable_if {};
template <typename T>
struct enable_if<true, T> {
  typedef T type;
};
template <typename T, T v>
struct integral_constant {
  static const T value = v;
};
typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;
template <typename T>
struct is_array : false_type {};
template <typename T>
struct is_array<T[]> : true_type {};
template <typename T, size_t N>
struct is_array<T[N]> : true_type {};
template <typename TBase, typename TDerived>
class is_base_of {
 protected:  // <- to avoid GCC's "all member functions in class are private"
  typedef char Yes[1];
  typedef char No[2];
  static Yes &probe(const TBase *);
  static No &probe(...);
 public:
  static const bool value =
      sizeof(probe(reinterpret_cast<TDerived *>(0))) == sizeof(Yes);
};
template <typename T>
T declval();
template <typename T>
struct is_class {
 protected:  // <- to avoid GCC's "all member functions in class are private"
  typedef char Yes[1];
  typedef char No[2];
  template <typename U>
  static Yes &probe(void (U::*)(void));
  template <typename>
  static No &probe(...);
 public:
  static const bool value = sizeof(probe<T>(0)) == sizeof(Yes);
};
template <typename T>
struct is_const : false_type {};
template <typename T>
struct is_const<const T> : true_type {};
}  // namespace ARDUINOJSON_NAMESPACE
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#endif
#ifdef __ICCARM__
#pragma diag_suppress=Pa093
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename From, typename To>
struct is_convertible {
 protected:  // <- to avoid GCC's "all member functions in class are private"
  typedef char Yes[1];
  typedef char No[2];
  static Yes &probe(To);
  static No &probe(...);
 public:
  static const bool value = sizeof(probe(declval<From>())) == sizeof(Yes);
};
}  // namespace ARDUINOJSON_NAMESPACE
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#ifdef __ICCARM__
#pragma diag_default=Pa093
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename>
struct is_floating_point : false_type {};
template <>
struct is_floating_point<float> : true_type {};
template <>
struct is_floating_point<double> : true_type {};
template <typename T, typename U>
struct is_same : false_type {};
template <typename T>
struct is_same<T, T> : true_type {};
template <typename T>
struct is_integral {
  static const bool value =
      is_same<T, signed char>::value || is_same<T, unsigned char>::value ||
      is_same<T, signed short>::value || is_same<T, unsigned short>::value ||
      is_same<T, signed int>::value || is_same<T, unsigned int>::value ||
      is_same<T, signed long>::value || is_same<T, unsigned long>::value ||
#if ARDUINOJSON_HAS_LONG_LONG
      is_same<T, signed long long>::value ||
      is_same<T, unsigned long long>::value ||
#endif
#if ARDUINOJSON_HAS_INT64
      is_same<T, signed __int64>::value ||
      is_same<T, unsigned __int64>::value ||
#endif
      is_same<T, char>::value || is_same<T, bool>::value;
};
template <typename T>
struct is_integral<const T> : is_integral<T> {};
template <typename T>
struct is_enum {
  static const bool value = is_convertible<T, int>::value &&
                            !is_class<T>::value && !is_integral<T>::value &&
                            !is_floating_point<T>::value;
};
template <typename T>
struct is_pointer : false_type {};
template <typename T>
struct is_pointer<T*> : true_type {};
template <typename>
struct is_signed : false_type {};
template <>
struct is_signed<char> : true_type {};
template <>
struct is_signed<signed char> : true_type {};
template <>
struct is_signed<signed short> : true_type {};
template <>
struct is_signed<signed int> : true_type {};
template <>
struct is_signed<signed long> : true_type {};
template <>
struct is_signed<float> : true_type {};
template <>
struct is_signed<double> : true_type {};
#if ARDUINOJSON_HAS_LONG_LONG
template <>
struct is_signed<signed long long> : true_type {};
#endif
#if ARDUINOJSON_HAS_INT64
template <>
struct is_signed<signed __int64> : true_type {};
#endif
template <typename>
struct is_unsigned : false_type {};
template <>
struct is_unsigned<bool> : true_type {};
template <>
struct is_unsigned<unsigned char> : true_type {};
template <>
struct is_unsigned<unsigned short> : true_type {};
template <>
struct is_unsigned<unsigned int> : true_type {};
template <>
struct is_unsigned<unsigned long> : true_type {};
#if ARDUINOJSON_HAS_INT64
template <>
struct is_unsigned<unsigned __int64> : true_type {};
#endif
#if ARDUINOJSON_HAS_LONG_LONG
template <>
struct is_unsigned<unsigned long long> : true_type {};
#endif
template <typename T>
struct type_identity {
  typedef T type;
};
template <typename T>
struct make_unsigned;
template <>
struct make_unsigned<char> : type_identity<unsigned char> {};
template <>
struct make_unsigned<signed char> : type_identity<unsigned char> {};
template <>
struct make_unsigned<unsigned char> : type_identity<unsigned char> {};
template <>
struct make_unsigned<signed short> : type_identity<unsigned short> {};
template <>
struct make_unsigned<unsigned short> : type_identity<unsigned short> {};
template <>
struct make_unsigned<signed int> : type_identity<unsigned int> {};
template <>
struct make_unsigned<unsigned int> : type_identity<unsigned int> {};
template <>
struct make_unsigned<signed long> : type_identity<unsigned long> {};
template <>
struct make_unsigned<unsigned long> : type_identity<unsigned long> {};
#if ARDUINOJSON_HAS_LONG_LONG
template <>
struct make_unsigned<signed long long> : type_identity<unsigned long long> {};
template <>
struct make_unsigned<unsigned long long> : type_identity<unsigned long long> {};
#endif
#if ARDUINOJSON_HAS_INT64
template <>
struct make_unsigned<signed __int64> : type_identity<unsigned __int64> {};
template <>
struct make_unsigned<unsigned __int64> : type_identity<unsigned __int64> {};
#endif
template <typename T>
struct remove_const {
  typedef T type;
};
template <typename T>
struct remove_const<const T> {
  typedef T type;
};
template <typename T>
struct remove_reference {
  typedef T type;
};
template <typename T>
struct remove_reference<T&> {
  typedef T type;
};
}  // namespace ARDUINOJSON_NAMESPACE
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4310)
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename T, typename Enable = void>
struct numeric_limits;
template <typename T>
struct numeric_limits<T, typename enable_if<is_unsigned<T>::value>::type> {
  static T lowest() {
    return 0;
  }
  static T highest() {
    return T(-1);
  }
};
template <typename T>
struct numeric_limits<
    T, typename enable_if<is_integral<T>::value && is_signed<T>::value>::type> {
  static T lowest() {
    return T(T(1) << (sizeof(T) * 8 - 1));
  }
  static T highest() {
    return T(~lowest());
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
#ifdef _MSC_VER
#pragma warning(pop)
#endif
namespace ARDUINOJSON_NAMESPACE {
namespace storage_policies {
struct store_by_address {};
struct store_by_copy {};
struct decide_at_runtime {};
}  // namespace storage_policies
#if ARDUINOJSON_USE_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif
#if ARDUINOJSON_USE_LONG_LONG
typedef int64_t Integer;
typedef uint64_t UInt;
#else
typedef long Integer;
typedef unsigned long UInt;
#endif
}  // namespace ARDUINOJSON_NAMESPACE
#if ARDUINOJSON_HAS_LONG_LONG && !ARDUINOJSON_USE_LONG_LONG
#define ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T)                  \
  static_assert(sizeof(T) <= sizeof(ARDUINOJSON_NAMESPACE::Integer),     \
                "To use 64-bit integers with ArduinoJson, you must set " \
                "ARDUINOJSON_USE_LONG_LONG to 1. See "                   \
                "https://arduinojson.org/v6/api/config/use_long_long/");
#else
#define ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T)
#endif
namespace ARDUINOJSON_NAMESPACE {
enum {
  VALUE_MASK = 0x7F,
  VALUE_IS_OWNED = 0x01,
  VALUE_IS_NULL = 0,
  VALUE_IS_LINKED_RAW = 0x02,
  VALUE_IS_OWNED_RAW = 0x03,
  VALUE_IS_LINKED_STRING = 0x04,
  VALUE_IS_OWNED_STRING = 0x05,
  VALUE_IS_BOOLEAN = 0x06,
  VALUE_IS_POSITIVE_INTEGER = 0x08,
  VALUE_IS_NEGATIVE_INTEGER = 0x0A,
  VALUE_IS_FLOAT = 0x0C,
  COLLECTION_MASK = 0x60,
  VALUE_IS_OBJECT = 0x20,
  VALUE_IS_ARRAY = 0x40,
  KEY_IS_OWNED = 0x80
};
struct RawData {
  const char *data;
  size_t size;
};
union VariantContent {
  Float asFloat;
  UInt asInteger;
  CollectionData asCollection;
  const char *asString;
  struct {
    const char *data;
    size_t size;
  } asRaw;
};
typedef int_t<ARDUINOJSON_SLOT_OFFSET_SIZE * 8>::type VariantSlotDiff;
class VariantSlot {
  VariantContent _content;
  uint8_t _flags;
  VariantSlotDiff _next;
  const char* _key;
 public:
  VariantData* data() {
    return reinterpret_cast<VariantData*>(&_content);
  }
  const VariantData* data() const {
    return reinterpret_cast<const VariantData*>(&_content);
  }
  VariantSlot* next() {
    return _next ? this + _next : 0;
  }
  const VariantSlot* next() const {
    return const_cast<VariantSlot*>(this)->next();
  }
  VariantSlot* next(size_t distance) {
    VariantSlot* slot = this;
    while (distance--) {
      if (!slot->_next)
        return 0;
      slot += slot->_next;
    }
    return slot;
  }
  const VariantSlot* next(size_t distance) const {
    return const_cast<VariantSlot*>(this)->next(distance);
  }
  void setNext(VariantSlot* slot) {
    ARDUINOJSON_ASSERT(!slot || slot - this >=
                                    numeric_limits<VariantSlotDiff>::lowest());
    ARDUINOJSON_ASSERT(!slot || slot - this <=
                                    numeric_limits<VariantSlotDiff>::highest());
    _next = VariantSlotDiff(slot ? slot - this : 0);
  }
  void setNextNotNull(VariantSlot* slot) {
    ARDUINOJSON_ASSERT(slot != 0);
    ARDUINOJSON_ASSERT(slot - this >=
                       numeric_limits<VariantSlotDiff>::lowest());
    ARDUINOJSON_ASSERT(slot - this <=
                       numeric_limits<VariantSlotDiff>::highest());
    _next = VariantSlotDiff(slot - this);
  }
  void setKey(const char* k, storage_policies::store_by_copy) {
    ARDUINOJSON_ASSERT(k != NULL);
    _flags |= KEY_IS_OWNED;
    _key = k;
  }
  void setKey(const char* k, storage_policies::store_by_address) {
    ARDUINOJSON_ASSERT(k != NULL);
    _flags &= VALUE_MASK;
    _key = k;
  }
  const char* key() const {
    return _key;
  }
  bool ownsKey() const {
    return (_flags & KEY_IS_OWNED) != 0;
  }
  void clear() {
    _next = 0;
    _flags = 0;
    _key = 0;
  }
  void movePointers(ptrdiff_t stringDistance, ptrdiff_t variantDistance) {
    if (_flags & KEY_IS_OWNED)
      _key += stringDistance;
    if (_flags & VALUE_IS_OWNED)
      _content.asString += stringDistance;
    if (_flags & COLLECTION_MASK)
      _content.asCollection.movePointers(stringDistance, variantDistance);
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
#include <string.h>
#define JSON_STRING_SIZE(SIZE) (SIZE + 1)
namespace ARDUINOJSON_NAMESPACE {
class MemoryPool {
 public:
  MemoryPool(char* buf, size_t capa)
      : _begin(buf),
        _left(buf),
        _right(buf ? buf + capa : 0),
        _end(buf ? buf + capa : 0),
        _overflowed(false) {
    ARDUINOJSON_ASSERT(isAligned(_begin));
    ARDUINOJSON_ASSERT(isAligned(_right));
    ARDUINOJSON_ASSERT(isAligned(_end));
  }
  void* buffer() {
    return _begin;
  }
  size_t capacity() const {
    return size_t(_end - _begin);
  }
  size_t size() const {
    return size_t(_left - _begin + _end - _right);
  }
  bool overflowed() const {
    return _overflowed;
  }
  VariantSlot* allocVariant() {
    return allocRight<VariantSlot>();
  }
  template <typename TAdaptedString>
  const char* saveString(const TAdaptedString& str) {
    if (str.isNull())
      return 0;
#if ARDUINOJSON_ENABLE_STRING_DEDUPLICATION
    const char* existingCopy = findString(str.begin());
    if (existingCopy)
      return existingCopy;
#endif
    size_t n = str.size();
    char* newCopy = allocString(n + 1);
    if (newCopy) {
      str.copyTo(newCopy, n);
      newCopy[n] = 0;  // force null-terminator
    }
    return newCopy;
  }
  void getFreeZone(char** zoneStart, size_t* zoneSize) const {
    *zoneStart = _left;
    *zoneSize = size_t(_right - _left);
  }
  const char* saveStringFromFreeZone(size_t len) {
#if ARDUINOJSON_ENABLE_STRING_DEDUPLICATION
    const char* dup = findString(_left);
    if (dup)
      return dup;
#endif
    const char* str = _left;
    _left += len;
    checkInvariants();
    return str;
  }
  void markAsOverflowed() {
    _overflowed = true;
  }
  void clear() {
    _left = _begin;
    _right = _end;
    _overflowed = false;
  }
  bool canAlloc(size_t bytes) const {
    return _left + bytes <= _right;
  }
  bool owns(void* p) const {
    return _begin <= p && p < _end;
  }
  void* operator new(size_t, void* p) {
    return p;
  }
  ptrdiff_t squash() {
    char* new_right = addPadding(_left);
    if (new_right >= _right)
      return 0;
    size_t right_size = static_cast<size_t>(_end - _right);
    memmove(new_right, _right, right_size);
    ptrdiff_t bytes_reclaimed = _right - new_right;
    _right = new_right;
    _end = new_right + right_size;
    return bytes_reclaimed;
  }
  void movePointers(ptrdiff_t offset) {
    _begin += offset;
    _left += offset;
    _right += offset;
    _end += offset;
  }
 private:
  void checkInvariants() {
    ARDUINOJSON_ASSERT(_begin <= _left);
    ARDUINOJSON_ASSERT(_left <= _right);
    ARDUINOJSON_ASSERT(_right <= _end);
    ARDUINOJSON_ASSERT(isAligned(_right));
  }
#if ARDUINOJSON_ENABLE_STRING_DEDUPLICATION
  template <typename TIterator>
  const char* findString(TIterator str) {
    for (char* next = _begin; next < _left; ++next) {
      char* begin = next;
      for (TIterator it = str; *it == *next; ++it) {
        if (*next++ == 0)
          return begin;
      }
      while (*next) ++next;
    }
    return 0;
  }
#endif
  char* allocString(size_t n) {
    if (!canAlloc(n)) {
      _overflowed = true;
      return 0;
    }
    char* s = _left;
    _left += n;
    checkInvariants();
    return s;
  }
  template <typename T>
  T* allocRight() {
    return reinterpret_cast<T*>(allocRight(sizeof(T)));
  }
  void* allocRight(size_t bytes) {
    if (!canAlloc(bytes)) {
      _overflowed = true;
      return 0;
    }
    _right -= bytes;
    return _right;
  }
  char *_begin, *_left, *_right, *_end;
  bool _overflowed;
};
inline int safe_strcmp(const char* a, const char* b) {
  if (a == b)
    return 0;
  if (!a)
    return -1;
  if (!b)
    return 1;
  return strcmp(a, b);
}
inline int safe_strncmp(const char* a, const char* b, size_t n) {
  if (a == b)
    return 0;
  if (!a)
    return -1;
  if (!b)
    return 1;
  return strncmp(a, b, n);
}
template <typename>
struct IsString : false_type {};
template <typename T>
struct IsString<const T> : IsString<T> {};
template <typename T>
struct IsString<T&> : IsString<T> {};
class ConstRamStringAdapter {
 public:
  ConstRamStringAdapter(const char* str = 0) : _str(str) {}
  int compare(const char* other) const {
    return safe_strcmp(_str, other);
  }
  bool equals(const char* expected) const {
    return compare(expected) == 0;
  }
  bool isNull() const {
    return !_str;
  }
  size_t size() const {
    if (!_str)
      return 0;
    return strlen(_str);
  }
  const char* data() const {
    return _str;
  }
  const char* begin() const {
    return _str;
  }
  typedef storage_policies::store_by_address storage_policy;
 protected:
  const char* _str;
};
template <>
struct IsString<const char*> : true_type {};
template <int N>
struct IsString<const char[N]> : true_type {};
inline ConstRamStringAdapter adaptString(const char* str) {
  return ConstRamStringAdapter(str);
}
class RamStringAdapter : public ConstRamStringAdapter {
 public:
  RamStringAdapter(const char* str) : ConstRamStringAdapter(str) {}
  void copyTo(char* p, size_t n) const {
    memcpy(p, _str, n);
  }
  typedef ARDUINOJSON_NAMESPACE::storage_policies::store_by_copy storage_policy;
};
template <typename TChar>
inline RamStringAdapter adaptString(const TChar* str) {
  return RamStringAdapter(reinterpret_cast<const char*>(str));
}
inline RamStringAdapter adaptString(char* str) {
  return RamStringAdapter(str);
}
template <typename TChar>
struct IsString<TChar*> {
  static const bool value = sizeof(TChar) == 1;
};
template <>
struct IsString<void*> {
  static const bool value = false;
};
class SizedRamStringAdapter {
 public:
  SizedRamStringAdapter(const char* str, size_t n) : _str(str), _size(n) {}
  int compare(const char* other) const {
    return safe_strncmp(_str, other, _size);
  }
  bool equals(const char* expected) const {
    return compare(expected) == 0;
  }
  bool isNull() const {
    return !_str;
  }
  void copyTo(char* p, size_t n) const {
    memcpy(p, _str, n);
  }
  size_t size() const {
    return _size;
  }
  const char* begin() const {
    return _str;
  }
  typedef storage_policies::store_by_copy storage_policy;
 private:
  const char* _str;
  size_t _size;
};
template <typename TChar>
inline SizedRamStringAdapter adaptString(const TChar* str, size_t size) {
  return SizedRamStringAdapter(reinterpret_cast<const char*>(str), size);
}
}  // namespace ARDUINOJSON_NAMESPACE
#if ARDUINOJSON_ENABLE_STD_STRING
#include <string>
namespace ARDUINOJSON_NAMESPACE {
template <typename TString>
class StdStringAdapter {
 public:
  StdStringAdapter(const TString& str) : _str(&str) {}
  void copyTo(char* p, size_t n) const {
    memcpy(p, _str->c_str(), n);
  }
  bool isNull() const {
    return false;
  }
  int compare(const char* other) const {
    if (!other)
      return 1;
    return _str->compare(other);
  }
  bool equals(const char* expected) const {
    if (!expected)
      return false;
    return *_str == expected;
  }
  size_t size() const {
    return _str->size();
  }
  const char* begin() const {
    return _str->c_str();
  }
  typedef storage_policies::store_by_copy storage_policy;
 private:
  const TString* _str;
};
template <typename TCharTraits, typename TAllocator>
struct IsString<std::basic_string<char, TCharTraits, TAllocator> > : true_type {
};
template <typename TCharTraits, typename TAllocator>
inline StdStringAdapter<std::basic_string<char, TCharTraits, TAllocator> >
adaptString(const std::basic_string<char, TCharTraits, TAllocator>& str) {
  return StdStringAdapter<std::basic_string<char, TCharTraits, TAllocator> >(
      str);
}
}  // namespace ARDUINOJSON_NAMESPACE
#endif
#if ARDUINOJSON_ENABLE_ARDUINO_STRING
namespace ARDUINOJSON_NAMESPACE {
class ArduinoStringAdapter {
 public:
  ArduinoStringAdapter(const ::String& str) : _str(&str) {}
  void copyTo(char* p, size_t n) const {
    memcpy(p, _str->c_str(), n);
  }
  bool isNull() const {
    return !_str->c_str();
  }
  int compare(const char* other) const {
    const char* me = _str->c_str();
    return safe_strcmp(me, other);
  }
  bool equals(const char* expected) const {
    return compare(expected) == 0;
  }
  size_t size() const {
    return _str->length();
  }
  const char* begin() const {
    return _str->c_str();
  }
  typedef storage_policies::store_by_copy storage_policy;
 private:
  const ::String* _str;
};
template <>
struct IsString< ::String> : true_type {};
template <>
struct IsString< ::StringSumHelper> : true_type {};
inline ArduinoStringAdapter adaptString(const ::String& str) {
  return ArduinoStringAdapter(str);
}
}  // namespace ARDUINOJSON_NAMESPACE
#endif
#if ARDUINOJSON_ENABLE_PROGMEM
namespace ARDUINOJSON_NAMESPACE {
struct pgm_p {
  pgm_p(const char* p) : address(p) {}
  const char* address;
};
}  // namespace ARDUINOJSON_NAMESPACE
#ifndef strlen_P
inline size_t strlen_P(ARDUINOJSON_NAMESPACE::pgm_p s) {
  const char* p = s.address;
  ARDUINOJSON_ASSERT(p != NULL);
  while (pgm_read_byte(p)) p++;
  return size_t(p - s.address);
}
#endif
#ifndef strncmp_P
inline int strncmp_P(const char* a, ARDUINOJSON_NAMESPACE::pgm_p b, size_t n) {
  const char* s1 = a;
  const char* s2 = b.address;
  ARDUINOJSON_ASSERT(s1 != NULL);
  ARDUINOJSON_ASSERT(s2 != NULL);
  while (n-- > 0) {
    char c1 = *s1++;
    char c2 = static_cast<char>(pgm_read_byte(s2++));
    if (c1 < c2)
      return -1;
    if (c1 > c2)
      return 1;
    if (c1 == 0 /* and c2 as well */)
      return 0;
  }
  return 0;
}
#endif
#ifndef strcmp_P
inline int strcmp_P(const char* a, ARDUINOJSON_NAMESPACE::pgm_p b) {
  const char* s1 = a;
  const char* s2 = b.address;
  ARDUINOJSON_ASSERT(s1 != NULL);
  ARDUINOJSON_ASSERT(s2 != NULL);
  for (;;) {
    char c1 = *s1++;
    char c2 = static_cast<char>(pgm_read_byte(s2++));
    if (c1 < c2)
      return -1;
    if (c1 > c2)
      return 1;
    if (c1 == 0 /* and c2 as well */)
      return 0;
  }
}
#endif
#ifndef memcpy_P
inline void* memcpy_P(void* dst, ARDUINOJSON_NAMESPACE::pgm_p src, size_t n) {
  uint8_t* d = reinterpret_cast<uint8_t*>(dst);
  const char* s = src.address;
  ARDUINOJSON_ASSERT(d != NULL);
  ARDUINOJSON_ASSERT(s != NULL);
  while (n-- > 0) {
    *d++ = pgm_read_byte(s++);
  }
  return dst;
}
#endif
namespace ARDUINOJSON_NAMESPACE {
class FlashStringIterator {
 public:
  explicit FlashStringIterator(const __FlashStringHelper* ptr)
      : _ptr(reinterpret_cast<const char*>(ptr)) {}
  explicit FlashStringIterator(const char* ptr) : _ptr(ptr) {}
  FlashStringIterator operator+(ptrdiff_t d) const {
    return FlashStringIterator(_ptr + d);
  }
  ptrdiff_t operator-(FlashStringIterator other) const {
    return _ptr - other._ptr;
  }
  FlashStringIterator operator++(int) {
    return FlashStringIterator(_ptr++);
  }
  FlashStringIterator operator++() {
    return FlashStringIterator(++_ptr);
  }
  bool operator!=(FlashStringIterator other) const {
    return _ptr != other._ptr;
  }
  char operator*() const {
    return char(pgm_read_byte(_ptr));
  }
 private:
  const char* _ptr;
};
class FlashStringAdapter {
 public:
  FlashStringAdapter(const __FlashStringHelper* str) : _str(str) {}
  int compare(const char* other) const {
    if (!other && !_str)
      return 0;
    if (!_str)
      return -1;
    if (!other)
      return 1;
    return -strcmp_P(other, reinterpret_cast<const char*>(_str));
  }
  bool equals(const char* expected) const {
    return compare(expected) == 0;
  }
  bool isNull() const {
    return !_str;
  }
  void copyTo(char* p, size_t n) const {
    memcpy_P(p, reinterpret_cast<const char*>(_str), n);
  }
  size_t size() const {
    if (!_str)
      return 0;
    return strlen_P(reinterpret_cast<const char*>(_str));
  }
  FlashStringIterator begin() const {
    return FlashStringIterator(_str);
  }
  typedef storage_policies::store_by_copy storage_policy;
 private:
  const __FlashStringHelper* _str;
};
inline FlashStringAdapter adaptString(const __FlashStringHelper* str) {
  return FlashStringAdapter(str);
}
template <>
struct IsString<const __FlashStringHelper*> : true_type {};
class SizedFlashStringAdapter {
 public:
  SizedFlashStringAdapter(const __FlashStringHelper* str, size_t sz)
      : _str(str), _size(sz) {}
  int compare(const char* other) const {
    if (!other && !_str)
      return 0;
    if (!_str)
      return -1;
    if (!other)
      return 1;
    return -strncmp_P(other, reinterpret_cast<const char*>(_str), _size);
  }
  bool equals(const char* expected) const {
    return compare(expected) == 0;
  }
  bool isNull() const {
    return !_str;
  }
  void copyTo(char* p, size_t n) const {
    memcpy_P(p, reinterpret_cast<const char*>(_str), n);
  }
  size_t size() const {
    return _size;
  }
  FlashStringIterator begin() const {
    return FlashStringIterator(_str);
  }
  typedef storage_policies::store_by_copy storage_policy;
 private:
  const __FlashStringHelper* _str;
  size_t _size;
};
inline SizedFlashStringAdapter adaptString(const __FlashStringHelper* str,
                                           size_t sz) {
  return SizedFlashStringAdapter(str, sz);
}
}  // namespace ARDUINOJSON_NAMESPACE
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename T>
class SerializedValue {
 public:
  explicit SerializedValue(T str) : _str(str) {}
  operator T() const {
    return _str;
  }
  const char* data() const {
    return _str.c_str();
  }
  size_t size() const {
    return _str.length();
  }
 private:
  T _str;
};
template <typename TChar>
class SerializedValue<TChar*> {
 public:
  explicit SerializedValue(TChar* p, size_t n) : _data(p), _size(n) {}
  operator TChar*() const {
    return _data;
  }
  TChar* data() const {
    return _data;
  }
  size_t size() const {
    return _size;
  }
 private:
  TChar* _data;
  size_t _size;
};
template <typename T>
inline SerializedValue<T> serialized(T str) {
  return SerializedValue<T>(str);
}
template <typename TChar>
inline SerializedValue<TChar*> serialized(TChar* p) {
  return SerializedValue<TChar*>(p, adaptString(p).size());
}
template <typename TChar>
inline SerializedValue<TChar*> serialized(TChar* p, size_t n) {
  return SerializedValue<TChar*>(p, n);
}
}  // namespace ARDUINOJSON_NAMESPACE
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif
#pragma GCC diagnostic ignored "-Wconversion"
#endif
#include <stdlib.h>
namespace ARDUINOJSON_NAMESPACE {
#ifndef isnan
template <typename T>
bool isnan(T x) {
  return x != x;
}
#endif
#ifndef isinf
template <typename T>
bool isinf(T x) {
  return x != 0.0 && x * 2 == x;
}
#endif
template <typename T, typename F>
struct alias_cast_t {
  union {
    F raw;
    T data;
  };
};
template <typename T, typename F>
T alias_cast(F raw_data) {
  alias_cast_t<T, F> ac;
  ac.raw = raw_data;
  return ac.data;
}
}  // namespace ARDUINOJSON_NAMESPACE
#if ARDUINOJSON_ENABLE_PROGMEM
namespace ARDUINOJSON_NAMESPACE {
template <typename T>
typename enable_if<is_pointer<T>::value, T>::type pgm_read(const void* p) {
  return reinterpret_cast<T>(pgm_read_ptr(p));
}
template <typename T>
typename enable_if<is_floating_point<T>::value &&
                       sizeof(T) == sizeof(float),  // on AVR sizeof(double) ==
                   T>::type
pgm_read(const void* p) {
  return pgm_read_float(p);
}
template <typename T>
typename enable_if<is_same<T, uint32_t>::value, T>::type pgm_read(
    const void* p) {
  return pgm_read_dword(p);
}
}  // namespace ARDUINOJSON_NAMESPACE
#ifndef ARDUINOJSON_DEFINE_STATIC_ARRAY
#define ARDUINOJSON_DEFINE_STATIC_ARRAY(type, name, value) \
  static type const name[] PROGMEM = value;
#endif
#ifndef ARDUINOJSON_READ_STATIC_ARRAY
#define ARDUINOJSON_READ_STATIC_ARRAY(type, name, index) \
  pgm_read<type>(name + index)
#endif
#else  // i.e. ARDUINOJSON_ENABLE_PROGMEM == 0
#ifndef ARDUINOJSON_DEFINE_STATIC_ARRAY
#define ARDUINOJSON_DEFINE_STATIC_ARRAY(type, name, value) \
  static type const name[] = value;
#endif
#ifndef ARDUINOJSON_READ_STATIC_ARRAY
#define ARDUINOJSON_READ_STATIC_ARRAY(type, name, index) name[index]
#endif
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename T, size_t = sizeof(T)>
struct FloatTraits {};
template <typename T>
struct FloatTraits<T, 8 /*64bits*/> {
  typedef uint64_t mantissa_type;
  static const short mantissa_bits = 52;
  static const mantissa_type mantissa_max =
      (mantissa_type(1) << mantissa_bits) - 1;
  typedef int16_t exponent_type;
  static const exponent_type exponent_max = 308;
  template <typename TExponent>
  static T make_float(T m, TExponent e) {
    if (e > 0) {
      for (uint8_t index = 0; e != 0; index++) {
        if (e & 1)
          m *= positiveBinaryPowerOfTen(index);
        e >>= 1;
      }
    } else {
      e = TExponent(-e);
      for (uint8_t index = 0; e != 0; index++) {
        if (e & 1)
          m *= negativeBinaryPowerOfTen(index);
        e >>= 1;
      }
    }
    return m;
  }
  static T positiveBinaryPowerOfTen(int index) {
    ARDUINOJSON_DEFINE_STATIC_ARRAY(  //
        uint32_t, factors,
        ARDUINOJSON_EXPAND18({
            0x40240000, 0x00000000,  // 1e1
            0x40590000, 0x00000000,  // 1e2
            0x40C38800, 0x00000000,  // 1e4
            0x4197D784, 0x00000000,  // 1e8
            0x4341C379, 0x37E08000,  // 1e16
            0x4693B8B5, 0xB5056E17,  // 1e32
            0x4D384F03, 0xE93FF9F5,  // 1e64
            0x5A827748, 0xF9301D32,  // 1e128
            0x75154FDD, 0x7F73BF3C   // 1e256
        }));
    return forge(
        ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, 2 * index),
        ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, 2 * index + 1));
  }
  static T negativeBinaryPowerOfTen(int index) {
    ARDUINOJSON_DEFINE_STATIC_ARRAY(  //
        uint32_t, factors,
        ARDUINOJSON_EXPAND18({
            0x3FB99999, 0x9999999A,  // 1e-1
            0x3F847AE1, 0x47AE147B,  // 1e-2
            0x3F1A36E2, 0xEB1C432D,  // 1e-4
            0x3E45798E, 0xE2308C3A,  // 1e-8
            0x3C9CD2B2, 0x97D889BC,  // 1e-16
            0x3949F623, 0xD5A8A733,  // 1e-32
            0x32A50FFD, 0x44F4A73D,  // 1e-64
            0x255BBA08, 0xCF8C979D,  // 1e-128
            0x0AC80628, 0x64AC6F43   // 1e-256
        }));
    return forge(
        ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, 2 * index),
        ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, 2 * index + 1));
  }
  static T negativeBinaryPowerOfTenPlusOne(int index) {
    ARDUINOJSON_DEFINE_STATIC_ARRAY(  //
        uint32_t, factors,
        ARDUINOJSON_EXPAND18({
            0x3FF00000, 0x00000000,  // 1e0
            0x3FB99999, 0x9999999A,  // 1e-1
            0x3F50624D, 0xD2F1A9FC,  // 1e-3
            0x3E7AD7F2, 0x9ABCAF48,  // 1e-7
            0x3CD203AF, 0x9EE75616,  // 1e-15
            0x398039D6, 0x65896880,  // 1e-31
            0x32DA53FC, 0x9631D10D,  // 1e-63
            0x25915445, 0x81B7DEC2,  // 1e-127
            0x0AFE07B2, 0x7DD78B14   // 1e-255
        }));
    return forge(
        ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, 2 * index),
        ARDUINOJSON_READ_STATIC_ARRAY(uint32_t, factors, 2 * index + 1));
  }
  static T nan() {
    return forge(0x7ff80000, 0x00000000);
  }
  static T inf() {
    return forge(0x7ff00000, 0x00000000);
  }
  static T highest() {
    return forge(0x7FEFFFFF, 0xFFFFFFFF);
  }
  static T lowest() {
    return forge(0xFFEFFFFF, 0xFFFFFFFF);
  }
  static T forge(uint32_t msb, uint32_t lsb) {
    return alias_cast<T>((uint64_t(msb) << 32) | lsb);
  }
};
template <typename T>
struct FloatTraits<T, 4 /*32bits*/> {
  typedef uint32_t mantissa_type;
  static const short mantissa_bits = 23;
  static const mantissa_type mantissa_max =
      (mantissa_type(1) << mantissa_bits) - 1;
  typedef int8_t exponent_type;
  static const exponent_type exponent_max = 38;
  template <typename TExponent>
  static T make_float(T m, TExponent e) {
    if (e > 0) {
      for (uint8_t index = 0; e != 0; index++) {
        if (e & 1)
          m *= positiveBinaryPowerOfTen(index);
        e >>= 1;
      }
    } else {
      e = -e;
      for (uint8_t index = 0; e != 0; index++) {
        if (e & 1)
          m *= negativeBinaryPowerOfTen(index);
        e >>= 1;
      }
    }
    return m;
  }
  static T positiveBinaryPowerOfTen(int index) {
    ARDUINOJSON_DEFINE_STATIC_ARRAY(
        T, factors,
        ARDUINOJSON_EXPAND6({1e1f, 1e2f, 1e4f, 1e8f, 1e16f, 1e32f}));
    return ARDUINOJSON_READ_STATIC_ARRAY(T, factors, index);
  }
  static T negativeBinaryPowerOfTen(int index) {
    ARDUINOJSON_DEFINE_STATIC_ARRAY(
        T, factors,
        ARDUINOJSON_EXPAND6({1e-1f, 1e-2f, 1e-4f, 1e-8f, 1e-16f, 1e-32f}));
    return ARDUINOJSON_READ_STATIC_ARRAY(T, factors, index);
  }
  static T negativeBinaryPowerOfTenPlusOne(int index) {
    ARDUINOJSON_DEFINE_STATIC_ARRAY(
        T, factors,
        ARDUINOJSON_EXPAND6({1e0f, 1e-1f, 1e-3f, 1e-7f, 1e-15f, 1e-31f}));
    return ARDUINOJSON_READ_STATIC_ARRAY(T, factors, index);
  }
  static T forge(uint32_t bits) {
    return alias_cast<T>(bits);
  }
  static T nan() {
    return forge(0x7fc00000);
  }
  static T inf() {
    return forge(0x7f800000);
  }
  static T highest() {
    return forge(0x7f7fffff);
  }
  static T lowest() {
    return forge(0xFf7fffff);
  }
};
template <typename TOut, typename TIn>
typename enable_if<is_integral<TOut>::value && sizeof(TOut) <= sizeof(TIn),
                   bool>::type
canStorePositiveInteger(TIn value) {
  return value <= TIn(numeric_limits<TOut>::highest());
}
template <typename TOut, typename TIn>
typename enable_if<is_integral<TOut>::value && sizeof(TIn) < sizeof(TOut),
                   bool>::type
canStorePositiveInteger(TIn) {
  return true;
}
template <typename TOut, typename TIn>
typename enable_if<is_floating_point<TOut>::value, bool>::type
canStorePositiveInteger(TIn) {
  return true;
}
template <typename TOut, typename TIn>
typename enable_if<is_floating_point<TOut>::value, bool>::type
canStoreNegativeInteger(TIn) {
  return true;
}
template <typename TOut, typename TIn>
typename enable_if<is_integral<TOut>::value && is_signed<TOut>::value &&
                       sizeof(TOut) <= sizeof(TIn),
                   bool>::type
canStoreNegativeInteger(TIn value) {
  return value <= TIn(numeric_limits<TOut>::highest()) + 1;
}
template <typename TOut, typename TIn>
typename enable_if<is_integral<TOut>::value && is_signed<TOut>::value &&
                       sizeof(TIn) < sizeof(TOut),
                   bool>::type
canStoreNegativeInteger(TIn) {
  return true;
}
template <typename TOut, typename TIn>
typename enable_if<is_integral<TOut>::value && is_unsigned<TOut>::value,
                   bool>::type
canStoreNegativeInteger(TIn) {
  return false;
}
template <typename TOut, typename TIn>
TOut convertPositiveInteger(TIn value) {
  return canStorePositiveInteger<TOut>(value) ? TOut(value) : 0;
}
template <typename TOut, typename TIn>
TOut convertNegativeInteger(TIn value) {
  return canStoreNegativeInteger<TOut>(value) ? TOut(~value + 1) : 0;
}
template <typename TOut, typename TIn>
typename enable_if<is_floating_point<TOut>::value, TOut>::type convertFloat(
    TIn value) {
  return TOut(value);
}
template <typename TOut, typename TIn>
typename enable_if<!is_floating_point<TOut>::value, TOut>::type convertFloat(
    TIn value) {
  return value >= numeric_limits<TOut>::lowest() &&
                 value <= numeric_limits<TOut>::highest()
             ? TOut(value)
             : 0;
}
}  // namespace ARDUINOJSON_NAMESPACE
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
#endif
#if defined(__GNUC__)
#if __GNUC__ >= 7
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif
#endif
namespace ARDUINOJSON_NAMESPACE {
class VariantData {
  VariantContent _content;  // must be first to allow cast from array to variant
  uint8_t _flags;
 public:
  void init() {
    _flags = 0;
  }
  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor &visitor) const {
    switch (type()) {
      case VALUE_IS_FLOAT:
        return visitor.visitFloat(_content.asFloat);
      case VALUE_IS_ARRAY:
        return visitor.visitArray(_content.asCollection);
      case VALUE_IS_OBJECT:
        return visitor.visitObject(_content.asCollection);
      case VALUE_IS_LINKED_STRING:
      case VALUE_IS_OWNED_STRING:
        return visitor.visitString(_content.asString);
      case VALUE_IS_OWNED_RAW:
      case VALUE_IS_LINKED_RAW:
        return visitor.visitRawJson(_content.asRaw.data, _content.asRaw.size);
      case VALUE_IS_NEGATIVE_INTEGER:
        return visitor.visitNegativeInteger(_content.asInteger);
      case VALUE_IS_POSITIVE_INTEGER:
        return visitor.visitPositiveInteger(_content.asInteger);
      case VALUE_IS_BOOLEAN:
        return visitor.visitBoolean(_content.asInteger != 0);
      default:
        return visitor.visitNull();
    }
  }
  template <typename T>
  T asIntegral() const;
  template <typename T>
  T asFloat() const;
  const char *asString() const;
  bool asBoolean() const;
  CollectionData *asArray() {
    return isArray() ? &_content.asCollection : 0;
  }
  const CollectionData *asArray() const {
    return const_cast<VariantData *>(this)->asArray();
  }
  CollectionData *asObject() {
    return isObject() ? &_content.asCollection : 0;
  }
  const CollectionData *asObject() const {
    return const_cast<VariantData *>(this)->asObject();
  }
  bool copyFrom(const VariantData &src, MemoryPool *pool) {
    switch (src.type()) {
      case VALUE_IS_ARRAY:
        return toArray().copyFrom(src._content.asCollection, pool);
      case VALUE_IS_OBJECT:
        return toObject().copyFrom(src._content.asCollection, pool);
      case VALUE_IS_OWNED_STRING:
        return setString(RamStringAdapter(src._content.asString), pool);
      case VALUE_IS_OWNED_RAW:
        return setOwnedRaw(
            serialized(src._content.asRaw.data, src._content.asRaw.size), pool);
      default:
        setType(src.type());
        _content = src._content;
        return true;
    }
  }
  bool isArray() const {
    return (_flags & VALUE_IS_ARRAY) != 0;
  }
  bool isBoolean() const {
    return type() == VALUE_IS_BOOLEAN;
  }
  bool isCollection() const {
    return (_flags & COLLECTION_MASK) != 0;
  }
  template <typename T>
  bool isInteger() const {
    switch (type()) {
      case VALUE_IS_POSITIVE_INTEGER:
        return canStorePositiveInteger<T>(_content.asInteger);
      case VALUE_IS_NEGATIVE_INTEGER:
        return canStoreNegativeInteger<T>(_content.asInteger);
      default:
        return false;
    }
  }
  bool isFloat() const {
    return type() == VALUE_IS_FLOAT || type() == VALUE_IS_POSITIVE_INTEGER ||
           type() == VALUE_IS_NEGATIVE_INTEGER;
  }
  bool isString() const {
    return type() == VALUE_IS_LINKED_STRING || type() == VALUE_IS_OWNED_STRING;
  }
  bool isObject() const {
    return (_flags & VALUE_IS_OBJECT) != 0;
  }
  bool isNull() const {
    return type() == VALUE_IS_NULL;
  }
  bool isEnclosed() const {
    return !isFloat();
  }
  void remove(size_t index) {
    if (isArray())
      _content.asCollection.removeElement(index);
  }
  template <typename TAdaptedString>
  void remove(TAdaptedString key) {
    if (isObject())
      _content.asCollection.removeMember(key);
  }
  void setBoolean(bool value) {
    setType(VALUE_IS_BOOLEAN);
    _content.asInteger = static_cast<UInt>(value);
  }
  void setFloat(Float value) {
    setType(VALUE_IS_FLOAT);
    _content.asFloat = value;
  }
  void setLinkedRaw(SerializedValue<const char *> value) {
    if (value.data()) {
      setType(VALUE_IS_LINKED_RAW);
      _content.asRaw.data = value.data();
      _content.asRaw.size = value.size();
    } else {
      setType(VALUE_IS_NULL);
    }
  }
  template <typename T>
  bool setOwnedRaw(SerializedValue<T> value, MemoryPool *pool) {
    const char *dup = pool->saveString(adaptString(value.data(), value.size()));
    if (dup) {
      setType(VALUE_IS_OWNED_RAW);
      _content.asRaw.data = dup;
      _content.asRaw.size = value.size();
      return true;
    } else {
      setType(VALUE_IS_NULL);
      return false;
    }
  }
  template <typename T>
  typename enable_if<is_unsigned<T>::value>::type setInteger(T value) {
    setUnsignedInteger(value);
  }
  template <typename T>
  typename enable_if<is_signed<T>::value>::type setInteger(T value) {
    setSignedInteger(value);
  }
  template <typename T>
  void setSignedInteger(T value) {
    if (value >= 0) {
      setPositiveInteger(static_cast<UInt>(value));
    } else {
      setNegativeInteger(~static_cast<UInt>(value) + 1);
    }
  }
  void setUnsignedInteger(UInt value) {
    setType(VALUE_IS_POSITIVE_INTEGER);
    _content.asInteger = static_cast<UInt>(value);
  }
  void setPositiveInteger(UInt value) {
    setType(VALUE_IS_POSITIVE_INTEGER);
    _content.asInteger = value;
  }
  void setNegativeInteger(UInt value) {
    setType(VALUE_IS_NEGATIVE_INTEGER);
    _content.asInteger = value;
  }
  void setNull() {
    setType(VALUE_IS_NULL);
  }
  void setStringPointer(const char *s, storage_policies::store_by_copy) {
    setType(VALUE_IS_OWNED_STRING);
    _content.asString = s;
  }
  void setStringPointer(const char *s, storage_policies::store_by_address) {
    setType(VALUE_IS_LINKED_STRING);
    _content.asString = s;
  }
  template <typename TAdaptedString>
  bool setString(TAdaptedString value, MemoryPool *pool) {
    return setString(value, pool, typename TAdaptedString::storage_policy());
  }
  template <typename TAdaptedString>
  inline bool setString(TAdaptedString value, MemoryPool *pool,
                        storage_policies::decide_at_runtime) {
    if (value.isStatic())
      return setString(value, pool, storage_policies::store_by_address());
    else
      return setString(value, pool, storage_policies::store_by_copy());
  }
  template <typename TAdaptedString>
  inline bool setString(TAdaptedString value, MemoryPool *,
                        storage_policies::store_by_address) {
    if (value.isNull())
      setNull();
    else
      setStringPointer(value.data(), storage_policies::store_by_address());
    return true;
  }
  template <typename TAdaptedString>
  inline bool setString(TAdaptedString value, MemoryPool *pool,
                        storage_policies::store_by_copy) {
    if (value.isNull()) {
      setNull();
      return true;
    }
    const char *copy = pool->saveString(value);
    if (!copy) {
      setNull();
      return false;
    }
    setStringPointer(copy, storage_policies::store_by_copy());
    return true;
  }
  CollectionData &toArray() {
    setType(VALUE_IS_ARRAY);
    _content.asCollection.clear();
    return _content.asCollection;
  }
  CollectionData &toObject() {
    setType(VALUE_IS_OBJECT);
    _content.asCollection.clear();
    return _content.asCollection;
  }
  size_t memoryUsage() const {
    switch (type()) {
      case VALUE_IS_OWNED_STRING:
        return strlen(_content.asString) + 1;
      case VALUE_IS_OWNED_RAW:
        return _content.asRaw.size;
      case VALUE_IS_OBJECT:
      case VALUE_IS_ARRAY:
        return _content.asCollection.memoryUsage();
      default:
        return 0;
    }
  }
  size_t nesting() const {
    return isCollection() ? _content.asCollection.nesting() : 0;
  }
  size_t size() const {
    return isCollection() ? _content.asCollection.size() : 0;
  }
  VariantData *addElement(MemoryPool *pool) {
    if (isNull())
      toArray();
    if (!isArray())
      return 0;
    return _content.asCollection.addElement(pool);
  }
  VariantData *getElement(size_t index) const {
    return isArray() ? _content.asCollection.getElement(index) : 0;
  }
  VariantData *getOrAddElement(size_t index, MemoryPool *pool) {
    if (isNull())
      toArray();
    if (!isArray())
      return 0;
    return _content.asCollection.getOrAddElement(index, pool);
  }
  template <typename TAdaptedString>
  VariantData *getMember(TAdaptedString key) const {
    return isObject() ? _content.asCollection.getMember(key) : 0;
  }
  template <typename TAdaptedString>
  VariantData *getOrAddMember(TAdaptedString key, MemoryPool *pool) {
    if (isNull())
      toObject();
    if (!isObject())
      return 0;
    return _content.asCollection.getOrAddMember(key, pool);
  }
  void movePointers(ptrdiff_t stringDistance, ptrdiff_t variantDistance) {
    if (_flags & VALUE_IS_OWNED)
      _content.asString += stringDistance;
    if (_flags & COLLECTION_MASK)
      _content.asCollection.movePointers(stringDistance, variantDistance);
  }
  uint8_t type() const {
    return _flags & VALUE_MASK;
  }
 private:
  void setType(uint8_t t) {
    _flags &= KEY_IS_OWNED;
    _flags |= t;
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
#if defined(__GNUC__)
#if __GNUC__ >= 8
#pragma GCC diagnostic pop
#endif
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename TAdaptedString>
inline bool slotSetKey(VariantSlot* var, TAdaptedString key, MemoryPool* pool) {
  if (!var)
    return false;
  return slotSetKey(var, key, pool, typename TAdaptedString::storage_policy());
}
template <typename TAdaptedString>
inline bool slotSetKey(VariantSlot* var, TAdaptedString key, MemoryPool* pool,
                       storage_policies::decide_at_runtime) {
  if (key.isStatic()) {
    return slotSetKey(var, key, pool, storage_policies::store_by_address());
  } else {
    return slotSetKey(var, key, pool, storage_policies::store_by_copy());
  }
}
template <typename TAdaptedString>
inline bool slotSetKey(VariantSlot* var, TAdaptedString key, MemoryPool*,
                       storage_policies::store_by_address) {
  ARDUINOJSON_ASSERT(var);
  var->setKey(key.data(), storage_policies::store_by_address());
  return true;
}
template <typename TAdaptedString>
inline bool slotSetKey(VariantSlot* var, TAdaptedString key, MemoryPool* pool,
                       storage_policies::store_by_copy) {
  const char* dup = pool->saveString(key);
  if (!dup)
    return false;
  ARDUINOJSON_ASSERT(var);
  var->setKey(dup, storage_policies::store_by_copy());
  return true;
}
inline size_t slotSize(const VariantSlot* var) {
  size_t n = 0;
  while (var) {
    n++;
    var = var->next();
  }
  return n;
}
inline VariantData* slotData(VariantSlot* slot) {
  return reinterpret_cast<VariantData*>(slot);
}
template <typename TResult>
struct Visitor {
  typedef TResult result_type;
};
struct Visitable {
};
template <typename T>
struct IsVisitable : is_base_of<Visitable, T> {};
template <typename T>
struct IsVisitable<T&> : IsVisitable<T> {};
}  // namespace ARDUINOJSON_NAMESPACE
#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#endif
#if ARDUINOJSON_ENABLE_STD_STRING
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename>
struct IsWriteableString : false_type {};
#if ARDUINOJSON_ENABLE_ARDUINO_STRING
template <>
struct IsWriteableString< ::String> : true_type {};
#endif
#if ARDUINOJSON_ENABLE_STD_STRING
template <typename TCharTraits, typename TAllocator>
struct IsWriteableString<std::basic_string<char, TCharTraits, TAllocator> >
    : true_type {};
#endif
class ArrayRef;
class ArrayConstRef;
class ObjectRef;
class ObjectConstRef;
class VariantRef;
class VariantConstRef;
template <typename T>
struct VariantAs {
  typedef T type;
};
template <>
struct VariantAs<char*> {
  typedef const char* type;
};
template <typename T>
struct VariantConstAs {
  typedef typename VariantAs<T>::type type;
};
template <>
struct VariantConstAs<VariantRef> {
  typedef VariantConstRef type;
};
template <>
struct VariantConstAs<ObjectRef> {
  typedef ObjectConstRef type;
};
template <>
struct VariantConstAs<ArrayRef> {
  typedef ArrayConstRef type;
};
template <typename T>
inline typename enable_if<is_integral<T>::value && !is_same<bool, T>::value,
                          T>::type
variantAs(const VariantData* data) {
  ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T);
  return data != 0 ? data->asIntegral<T>() : T(0);
}
template <typename T>
inline typename enable_if<is_enum<T>::value, T>::type variantAs(
    const VariantData* data) {
  return data != 0 ? static_cast<T>(data->asIntegral<int>()) : T();
}
template <typename T>
inline typename enable_if<is_same<T, bool>::value, T>::type variantAs(
    const VariantData* data) {
  return data != 0 ? data->asBoolean() : false;
}
template <typename T>
inline typename enable_if<is_floating_point<T>::value, T>::type variantAs(
    const VariantData* data) {
  return data != 0 ? data->asFloat<T>() : T(0);
}
template <typename T>
inline typename enable_if<is_same<T, const char*>::value ||
                              is_same<T, char*>::value,
                          const char*>::type
variantAs(const VariantData* data) {
  return data != 0 ? data->asString() : 0;
}
template <typename T>
T variantAs(VariantData* data, MemoryPool*) {
  return variantAs<T>(data);
}
template <typename T>
inline typename enable_if<is_same<ArrayConstRef, T>::value, T>::type variantAs(
    const VariantData* data);
template <typename T>
inline typename enable_if<is_same<ObjectConstRef, T>::value, T>::type variantAs(
    const VariantData* data);
template <typename T>
inline typename enable_if<is_same<VariantConstRef, T>::value, T>::type
variantAs(const VariantData* data);
template <typename T>
inline typename enable_if<IsWriteableString<T>::value, T>::type variantAs(
    const VariantData* data);
}  // namespace ARDUINOJSON_NAMESPACE
#ifdef _MSC_VER  // Visual Studio
#define FORCE_INLINE  // __forceinline causes C4714 when returning std::string
#define NO_INLINE __declspec(noinline)
#define DEPRECATED(msg) __declspec(deprecated(msg))
#elif defined(__GNUC__)  // GCC or Clang
#define FORCE_INLINE __attribute__((always_inline))
#define NO_INLINE __attribute__((noinline))
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
#define DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
#define DEPRECATED(msg) __attribute__((deprecated))
#endif
#else  // Other compilers
#define FORCE_INLINE
#define NO_INLINE
#define DEPRECATED(msg)
#endif
#if __cplusplus >= 201103L
#define NOEXCEPT noexcept
#else
#define NOEXCEPT throw()
#endif
#if defined(__has_attribute)
#if __has_attribute(no_sanitize)
#define ARDUINOJSON_NO_SANITIZE(check) __attribute__((no_sanitize(check)))
#else
#define ARDUINOJSON_NO_SANITIZE(check)
#endif
#else
#define ARDUINOJSON_NO_SANITIZE(check)
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename TVisitor>
inline typename TVisitor::result_type variantAccept(const VariantData *var,
                                                    TVisitor &visitor) {
  if (var != 0)
    return var->accept(visitor);
  else
    return visitor.visitNull();
}
inline const CollectionData *variantAsArray(const VariantData *var) {
  return var != 0 ? var->asArray() : 0;
}
inline const CollectionData *variantAsObject(const VariantData *var) {
  return var != 0 ? var->asObject() : 0;
}
inline CollectionData *variantAsObject(VariantData *var) {
  return var != 0 ? var->asObject() : 0;
}
inline bool variantCopyFrom(VariantData *dst, const VariantData *src,
                            MemoryPool *pool) {
  if (!dst)
    return false;
  if (!src) {
    dst->setNull();
    return true;
  }
  return dst->copyFrom(*src, pool);
}
inline int variantCompare(const VariantData *a, const VariantData *b);
inline bool variantIsArray(const VariantData *var) {
  return var && var->isArray();
}
inline bool variantIsBoolean(const VariantData *var) {
  return var && var->isBoolean();
}
template <typename T>
inline bool variantIsInteger(const VariantData *var) {
  return var && var->isInteger<T>();
}
inline bool variantIsFloat(const VariantData *var) {
  return var && var->isFloat();
}
inline bool variantIsString(const VariantData *var) {
  return var && var->isString();
}
inline bool variantIsObject(const VariantData *var) {
  return var && var->isObject();
}
inline bool variantIsNull(const VariantData *var) {
  return var == 0 || var->isNull();
}
inline bool variantSetBoolean(VariantData *var, bool value) {
  if (!var)
    return false;
  var->setBoolean(value);
  return true;
}
inline bool variantSetFloat(VariantData *var, Float value) {
  if (!var)
    return false;
  var->setFloat(value);
  return true;
}
inline bool variantSetLinkedRaw(VariantData *var,
                                SerializedValue<const char *> value) {
  if (!var)
    return false;
  var->setLinkedRaw(value);
  return true;
}
template <typename T>
inline bool variantSetOwnedRaw(VariantData *var, SerializedValue<T> value,
                               MemoryPool *pool) {
  return var != 0 && var->setOwnedRaw(value, pool);
}
inline void variantSetNull(VariantData *var) {
  if (!var)
    return;
  var->setNull();
}
template <typename TAdaptedString>
inline bool variantSetString(VariantData *var, TAdaptedString value,
                             MemoryPool *pool) {
  if (!var)
    return false;
  return var->setString(value, pool);
}
template <typename T>
inline bool variantSetInteger(VariantData *var, T value) {
  ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T);
  if (!var)
    return false;
  var->setInteger(value);
  return true;
}
inline size_t variantSize(const VariantData *var) {
  return var != 0 ? var->size() : 0;
}
inline CollectionData *variantToArray(VariantData *var) {
  if (!var)
    return 0;
  return &var->toArray();
}
inline CollectionData *variantToObject(VariantData *var) {
  if (!var)
    return 0;
  return &var->toObject();
}
inline NO_INLINE VariantData *variantAddElement(VariantData *var,
                                                MemoryPool *pool) {
  return var != 0 ? var->addElement(pool) : 0;
}
inline NO_INLINE VariantData *variantGetOrAddElement(VariantData *var,
                                                     size_t index,
                                                     MemoryPool *pool) {
  return var != 0 ? var->getOrAddElement(index, pool) : 0;
}
template <typename TChar>
NO_INLINE VariantData *variantGetOrAddMember(VariantData *var, TChar *key,
                                             MemoryPool *pool) {
  return var != 0 ? var->getOrAddMember(adaptString(key), pool) : 0;
}
template <typename TString>
NO_INLINE VariantData *variantGetOrAddMember(VariantData *var,
                                             const TString &key,
                                             MemoryPool *pool) {
  return var != 0 ? var->getOrAddMember(adaptString(key), pool) : 0;
}
enum CompareResult {
  COMPARE_RESULT_DIFFER = 0,
  COMPARE_RESULT_EQUAL = 1,
  COMPARE_RESULT_GREATER = 2,
  COMPARE_RESULT_LESS = 4,
  COMPARE_RESULT_GREATER_OR_EQUAL = 3,
  COMPARE_RESULT_LESS_OR_EQUAL = 5
};
template <typename T>
CompareResult arithmeticCompare(const T &lhs, const T &rhs) {
  if (lhs < rhs)
    return COMPARE_RESULT_LESS;
  else if (lhs > rhs)
    return COMPARE_RESULT_GREATER;
  else
    return COMPARE_RESULT_EQUAL;
}
template <typename T1, typename T2>
CompareResult arithmeticCompare(
    const T1 &lhs, const T2 &rhs,
    typename enable_if<is_integral<T1>::value && is_integral<T2>::value &&
                           sizeof(T1) < sizeof(T2),
                       int  // Using int instead of void to avoid C2572 on
                       >::type * = 0) {
  return arithmeticCompare<T2>(static_cast<T2>(lhs), rhs);
}
template <typename T1, typename T2>
CompareResult arithmeticCompare(
    const T1 &lhs, const T2 &rhs,
    typename enable_if<is_integral<T1>::value && is_integral<T2>::value &&
                       sizeof(T2) < sizeof(T1)>::type * = 0) {
  return arithmeticCompare<T1>(lhs, static_cast<T1>(rhs));
}
template <typename T1, typename T2>
CompareResult arithmeticCompare(
    const T1 &lhs, const T2 &rhs,
    typename enable_if<is_integral<T1>::value && is_integral<T2>::value &&
                       is_signed<T1>::value == is_signed<T2>::value &&
                       sizeof(T2) == sizeof(T1)>::type * = 0) {
  return arithmeticCompare<T1>(lhs, static_cast<T1>(rhs));
}
template <typename T1, typename T2>
CompareResult arithmeticCompare(
    const T1 &lhs, const T2 &rhs,
    typename enable_if<is_integral<T1>::value && is_integral<T2>::value &&
                       is_unsigned<T1>::value && is_signed<T2>::value &&
                       sizeof(T2) == sizeof(T1)>::type * = 0) {
  if (rhs < 0)
    return COMPARE_RESULT_GREATER;
  return arithmeticCompare<T1>(lhs, static_cast<T1>(rhs));
}
template <typename T1, typename T2>
CompareResult arithmeticCompare(
    const T1 &lhs, const T2 &rhs,
    typename enable_if<is_integral<T1>::value && is_integral<T2>::value &&
                       is_signed<T1>::value && is_unsigned<T2>::value &&
                       sizeof(T2) == sizeof(T1)>::type * = 0) {
  if (lhs < 0)
    return COMPARE_RESULT_LESS;
  return arithmeticCompare<T2>(static_cast<T2>(lhs), rhs);
}
template <typename T1, typename T2>
CompareResult arithmeticCompare(
    const T1 &lhs, const T2 &rhs,
    typename enable_if<is_floating_point<T1>::value ||
                       is_floating_point<T2>::value>::type * = 0) {
  return arithmeticCompare<double>(static_cast<double>(lhs),
                                   static_cast<double>(rhs));
}
template <typename T2>
CompareResult arithmeticCompareNegateLeft(
    UInt, const T2 &, typename enable_if<is_unsigned<T2>::value>::type * = 0) {
  return COMPARE_RESULT_LESS;
}
template <typename T2>
CompareResult arithmeticCompareNegateLeft(
    UInt lhs, const T2 &rhs,
    typename enable_if<is_signed<T2>::value>::type * = 0) {
  if (rhs > 0)
    return COMPARE_RESULT_LESS;
  return arithmeticCompare(-rhs, static_cast<T2>(lhs));
}
template <typename T1>
CompareResult arithmeticCompareNegateRight(
    const T1 &, UInt, typename enable_if<is_unsigned<T1>::value>::type * = 0) {
  return COMPARE_RESULT_GREATER;
}
template <typename T1>
CompareResult arithmeticCompareNegateRight(
    const T1 &lhs, UInt rhs,
    typename enable_if<is_signed<T1>::value>::type * = 0) {
  if (lhs > 0)
    return COMPARE_RESULT_GREATER;
  return arithmeticCompare(static_cast<T1>(rhs), -lhs);
}
struct VariantTag {};
template <typename T>
struct IsVariant : is_base_of<VariantTag, T> {};
template <typename T1, typename T2>
CompareResult compare(const T1 &lhs, const T2 &rhs);  // VariantCompare.cpp
template <typename TVariant>
struct VariantOperators {
  template <typename T>
  friend typename enable_if<!IsVariant<T>::value,
                            typename VariantAs<T>::type>::type
  operator|(const TVariant &variant, T defaultValue) {
    if (variant.template is<T>())
      return variant.template as<T>();
    else
      return defaultValue;
  }
  template <typename T>
  friend typename enable_if<IsVariant<T>::value, typename T::variant_type>::type
  operator|(const TVariant &variant, T defaultValue) {
    if (variant)
      return variant;
    else
      return defaultValue;
  }
  template <typename T>
  friend bool operator==(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) == COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend bool operator==(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) == COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend bool operator==(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) == COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator==(
      TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) == COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend bool operator!=(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) != COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend bool operator!=(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) != COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend bool operator!=(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) != COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator!=(
      TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) != COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend bool operator<(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) == COMPARE_RESULT_GREATER;
  }
  template <typename T>
  friend bool operator<(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) == COMPARE_RESULT_GREATER;
  }
  template <typename T>
  friend bool operator<(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) == COMPARE_RESULT_LESS;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator<(
      TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) == COMPARE_RESULT_LESS;
  }
  template <typename T>
  friend bool operator<=(T *lhs, TVariant rhs) {
    return (compare(rhs, lhs) & COMPARE_RESULT_GREATER_OR_EQUAL) != 0;
  }
  template <typename T>
  friend bool operator<=(const T &lhs, TVariant rhs) {
    return (compare(rhs, lhs) & COMPARE_RESULT_GREATER_OR_EQUAL) != 0;
  }
  template <typename T>
  friend bool operator<=(TVariant lhs, T *rhs) {
    return (compare(lhs, rhs) & COMPARE_RESULT_LESS_OR_EQUAL) != 0;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator<=(
      TVariant lhs, const T &rhs) {
    return (compare(lhs, rhs) & COMPARE_RESULT_LESS_OR_EQUAL) != 0;
  }
  template <typename T>
  friend bool operator>(T *lhs, TVariant rhs) {
    return compare(rhs, lhs) == COMPARE_RESULT_LESS;
  }
  template <typename T>
  friend bool operator>(const T &lhs, TVariant rhs) {
    return compare(rhs, lhs) == COMPARE_RESULT_LESS;
  }
  template <typename T>
  friend bool operator>(TVariant lhs, T *rhs) {
    return compare(lhs, rhs) == COMPARE_RESULT_GREATER;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator>(
      TVariant lhs, const T &rhs) {
    return compare(lhs, rhs) == COMPARE_RESULT_GREATER;
  }
  template <typename T>
  friend bool operator>=(T *lhs, TVariant rhs) {
    return (compare(rhs, lhs) & COMPARE_RESULT_LESS_OR_EQUAL) != 0;
  }
  template <typename T>
  friend bool operator>=(const T &lhs, TVariant rhs) {
    return (compare(rhs, lhs) & COMPARE_RESULT_LESS_OR_EQUAL) != 0;
  }
  template <typename T>
  friend bool operator>=(TVariant lhs, T *rhs) {
    return (compare(lhs, rhs) & COMPARE_RESULT_GREATER_OR_EQUAL) != 0;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator>=(
      TVariant lhs, const T &rhs) {
    return (compare(lhs, rhs) & COMPARE_RESULT_GREATER_OR_EQUAL) != 0;
  }
};
template <typename>
class ElementProxy;
template <typename TArray>
class ArrayShortcuts {
 public:
  FORCE_INLINE ElementProxy<TArray> operator[](size_t index) const;
  FORCE_INLINE ObjectRef createNestedObject() const;
  FORCE_INLINE ArrayRef createNestedArray() const;
  template <typename T>
  FORCE_INLINE bool add(const T &value) const {
    return impl()->addElement().set(value);
  }
  template <typename T>
  FORCE_INLINE bool add(T *value) const {
    return impl()->addElement().set(value);
  }
 private:
  const TArray *impl() const {
    return static_cast<const TArray *>(this);
  }
};
template <typename TParent, typename TStringRef>
class MemberProxy;
template <typename TObject>
class ObjectShortcuts {
 public:
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value, bool>::type
  containsKey(const TString &key) const;
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar *>::value, bool>::type
  containsKey(TChar *key) const;
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value,
                                  MemberProxy<TObject, TString> >::type
  operator[](const TString &key) const;
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar *>::value,
                                  MemberProxy<TObject, TChar *> >::type
  operator[](TChar *key) const;
  template <typename TString>
  FORCE_INLINE ArrayRef createNestedArray(const TString &key) const;
  template <typename TChar>
  FORCE_INLINE ArrayRef createNestedArray(TChar *key) const;
  template <typename TString>
  ObjectRef createNestedObject(const TString &key) const;
  template <typename TChar>
  ObjectRef createNestedObject(TChar *key) const;
 private:
  const TObject *impl() const {
    return static_cast<const TObject *>(this);
  }
};
template <typename TVariant>
class VariantShortcuts : public ObjectShortcuts<TVariant>,
                         public ArrayShortcuts<TVariant> {
 public:
  using ArrayShortcuts<TVariant>::createNestedArray;
  using ArrayShortcuts<TVariant>::createNestedObject;
  using ArrayShortcuts<TVariant>::operator[];
  using ObjectShortcuts<TVariant>::createNestedArray;
  using ObjectShortcuts<TVariant>::createNestedObject;
  using ObjectShortcuts<TVariant>::operator[];
};
class ArrayRef;
class ObjectRef;
template <typename TData>
class VariantRefBase : public VariantTag {
 public:
  template <typename T>
  FORCE_INLINE
      typename enable_if<is_integral<T>::value && !is_same<bool, T>::value,
                         bool>::type
      is() const {
    return variantIsInteger<T>(_data);
  }
  template <typename T>
  FORCE_INLINE typename enable_if<is_floating_point<T>::value, bool>::type is()
      const {
    return variantIsFloat(_data);
  }
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, bool>::value, bool>::type is()
      const {
    return variantIsBoolean(_data);
  }
  template <typename T>
  FORCE_INLINE typename enable_if<is_same<T, const char *>::value ||
                                      is_same<T, char *>::value ||
                                      IsWriteableString<T>::value,
                                  bool>::type
  is() const {
    return variantIsString(_data);
  }
  template <typename T>
  FORCE_INLINE typename enable_if<
      is_same<typename remove_const<T>::type, ArrayRef>::value, bool>::type
  is() const {
    return variantIsArray(_data);
  }
  template <typename T>
  FORCE_INLINE typename enable_if<
      is_same<typename remove_const<T>::type, ObjectRef>::value, bool>::type
  is() const {
    return variantIsObject(_data);
  }
#if ARDUINOJSON_HAS_NULLPTR
  template <typename T>
  FORCE_INLINE
      typename enable_if<is_same<T, decltype(nullptr)>::value, bool>::type
      is() const {
    return variantIsNull(_data);
  }
#endif
  template <typename T>
  FORCE_INLINE typename enable_if<is_enum<T>::value, bool>::type is() const {
    return variantIsInteger<int>(_data);
  }
  FORCE_INLINE bool isNull() const {
    return variantIsNull(_data);
  }
  FORCE_INLINE bool isUndefined() const {
    return !_data;
  }
  FORCE_INLINE size_t memoryUsage() const {
    return _data ? _data->memoryUsage() : 0;
  }
  FORCE_INLINE size_t nesting() const {
    return _data ? _data->nesting() : 0;
  }
  size_t size() const {
    return variantSize(_data);
  }
 protected:
  VariantRefBase(TData *data) : _data(data) {}
  TData *_data;
};
class VariantRef : public VariantRefBase<VariantData>,
                   public VariantOperators<VariantRef>,
                   public VariantShortcuts<VariantRef>,
                   public Visitable {
  typedef VariantRefBase<VariantData> base_type;
  friend class VariantConstRef;
 public:
  FORCE_INLINE VariantRef(MemoryPool *pool, VariantData *data)
      : base_type(data), _pool(pool) {}
  FORCE_INLINE VariantRef() : base_type(0), _pool(0) {}
  FORCE_INLINE void clear() const {
    return variantSetNull(_data);
  }
  template <typename T>
  FORCE_INLINE bool set(
      T value, typename enable_if<is_same<T, bool>::value>::type * = 0) const {
    return variantSetBoolean(_data, value);
  }
  template <typename T>
  FORCE_INLINE bool set(
      T value,
      typename enable_if<is_floating_point<T>::value>::type * = 0) const {
    return variantSetFloat(_data, static_cast<Float>(value));
  }
  template <typename T>
  FORCE_INLINE bool set(
      T value, typename enable_if<is_integral<T>::value &&
                                  !is_same<bool, T>::value>::type * = 0) const {
    return variantSetInteger<T>(_data, value);
  }
  FORCE_INLINE bool set(SerializedValue<const char *> value) const {
    return variantSetLinkedRaw(_data, value);
  }
  template <typename T>
  FORCE_INLINE bool set(
      SerializedValue<T> value,
      typename enable_if<!is_same<const char *, T>::value>::type * = 0) const {
    return variantSetOwnedRaw(_data, value, _pool);
  }
  template <typename T>
  FORCE_INLINE bool set(
      const T &value,
      typename enable_if<IsString<T>::value>::type * = 0) const {
    return variantSetString(_data, adaptString(value), _pool);
  }
  template <typename T>
  FORCE_INLINE bool set(
      T *value, typename enable_if<IsString<T *>::value>::type * = 0) const {
    return variantSetString(_data, adaptString(value), _pool);
  }
  template <typename TVariant>
  typename enable_if<IsVisitable<TVariant>::value, bool>::type set(
      const TVariant &value) const;
  template <typename T>
  FORCE_INLINE bool set(
      T value, typename enable_if<is_enum<T>::value>::type * = 0) const {
    return variantSetInteger(_data, static_cast<Integer>(value));
  }
#if ARDUINOJSON_HAS_NULLPTR
  FORCE_INLINE bool set(decltype(nullptr)) const {
    variantSetNull(_data);
    return true;
  }
#endif
  template <typename T>
  FORCE_INLINE typename VariantAs<T>::type as() const {
    return variantAs<typename VariantAs<T>::type>(_data, _pool);
  }
  template <typename T>
  FORCE_INLINE operator T() const {
    return variantAs<T>(_data, _pool);
  }
  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor &visitor) const {
    return variantAccept(_data, visitor);
  }
  template <typename T>
  typename enable_if<is_same<T, ArrayRef>::value, ArrayRef>::type to() const;
  template <typename T>
  typename enable_if<is_same<T, ObjectRef>::value, ObjectRef>::type to() const;
  template <typename T>
  typename enable_if<is_same<T, VariantRef>::value, VariantRef>::type to()
      const;
  VariantRef addElement() const;
  FORCE_INLINE VariantRef getElement(size_t) const;
  FORCE_INLINE VariantRef getOrAddElement(size_t) const;
  template <typename TChar>
  FORCE_INLINE VariantRef getMember(TChar *) const;
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value, VariantRef>::type
  getMember(const TString &) const;
  template <typename TChar>
  FORCE_INLINE VariantRef getOrAddMember(TChar *) const;
  template <typename TString>
  FORCE_INLINE VariantRef getOrAddMember(const TString &) const;
  FORCE_INLINE void remove(size_t index) const {
    if (_data)
      _data->remove(index);
  }
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar *>::value>::type remove(
      TChar *key) const {
    if (_data)
      _data->remove(adaptString(key));
  }
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value>::type remove(
      const TString &key) const {
    if (_data)
      _data->remove(adaptString(key));
  }
 private:
  MemoryPool *_pool;
};  // namespace ARDUINOJSON_NAMESPACE
class VariantConstRef : public VariantRefBase<const VariantData>,
                        public VariantOperators<VariantConstRef>,
                        public VariantShortcuts<VariantConstRef>,
                        public Visitable {
  typedef VariantRefBase<const VariantData> base_type;
  friend class VariantRef;
 public:
  VariantConstRef() : base_type(0) {}
  VariantConstRef(const VariantData *data) : base_type(data) {}
  VariantConstRef(VariantRef var) : base_type(var._data) {}
  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor &visitor) const {
    return variantAccept(_data, visitor);
  }
  template <typename T>
  FORCE_INLINE typename VariantConstAs<T>::type as() const {
    return variantAs<typename VariantConstAs<T>::type>(_data);
  }
  template <typename T>
  FORCE_INLINE operator T() const {
    return variantAs<T>(_data);
  }
  FORCE_INLINE VariantConstRef getElement(size_t) const;
  FORCE_INLINE VariantConstRef operator[](size_t index) const {
    return getElement(index);
  }
  template <typename TString>
  FORCE_INLINE VariantConstRef getMember(const TString &key) const {
    return VariantConstRef(
        objectGetMember(variantAsObject(_data), adaptString(key)));
  }
  template <typename TChar>
  FORCE_INLINE VariantConstRef getMember(TChar *key) const {
    const CollectionData *obj = variantAsObject(_data);
    return VariantConstRef(obj ? obj->getMember(adaptString(key)) : 0);
  }
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value, VariantConstRef>::type
      operator[](const TString &key) const {
    return getMember(key);
  }
  template <typename TChar>
  FORCE_INLINE
      typename enable_if<IsString<TChar *>::value, VariantConstRef>::type
      operator[](TChar *key) const {
    return getMember(key);
  }
};
class VariantPtr {
 public:
  VariantPtr(MemoryPool *pool, VariantData *data) : _variant(pool, data) {}
  VariantRef *operator->() {
    return &_variant;
  }
  VariantRef &operator*() {
    return _variant;
  }
 private:
  VariantRef _variant;
};
class ArrayIterator {
 public:
  ArrayIterator() : _slot(0) {}
  explicit ArrayIterator(MemoryPool *pool, VariantSlot *slot)
      : _pool(pool), _slot(slot) {}
  VariantRef operator*() const {
    return VariantRef(_pool, _slot->data());
  }
  VariantPtr operator->() {
    return VariantPtr(_pool, _slot->data());
  }
  bool operator==(const ArrayIterator &other) const {
    return _slot == other._slot;
  }
  bool operator!=(const ArrayIterator &other) const {
    return _slot != other._slot;
  }
  ArrayIterator &operator++() {
    _slot = _slot->next();
    return *this;
  }
  ArrayIterator &operator+=(size_t distance) {
    _slot = _slot->next(distance);
    return *this;
  }
  VariantSlot *internal() {
    return _slot;
  }
 private:
  MemoryPool *_pool;
  VariantSlot *_slot;
};
class VariantConstPtr {
 public:
  VariantConstPtr(const VariantData *data) : _variant(data) {}
  VariantConstRef *operator->() {
    return &_variant;
  }
  VariantConstRef &operator*() {
    return _variant;
  }
 private:
  VariantConstRef _variant;
};
class ArrayConstRefIterator {
 public:
  ArrayConstRefIterator() : _slot(0) {}
  explicit ArrayConstRefIterator(const VariantSlot *slot) : _slot(slot) {}
  VariantConstRef operator*() const {
    return VariantConstRef(_slot->data());
  }
  VariantConstPtr operator->() {
    return VariantConstPtr(_slot->data());
  }
  bool operator==(const ArrayConstRefIterator &other) const {
    return _slot == other._slot;
  }
  bool operator!=(const ArrayConstRefIterator &other) const {
    return _slot != other._slot;
  }
  ArrayConstRefIterator &operator++() {
    _slot = _slot->next();
    return *this;
  }
  ArrayConstRefIterator &operator+=(size_t distance) {
    _slot = _slot->next(distance);
    return *this;
  }
  const VariantSlot *internal() {
    return _slot;
  }
 private:
  const VariantSlot *_slot;
};
}  // namespace ARDUINOJSON_NAMESPACE
#define JSON_ARRAY_SIZE(NUMBER_OF_ELEMENTS) \
  ((NUMBER_OF_ELEMENTS) * sizeof(ARDUINOJSON_NAMESPACE::VariantSlot))
namespace ARDUINOJSON_NAMESPACE {
class ObjectRef;
template <typename>
class ElementProxy;
template <typename TData>
class ArrayRefBase {
 public:
  operator VariantConstRef() const {
    const void* data = _data;  // prevent warning cast-align
    return VariantConstRef(reinterpret_cast<const VariantData*>(data));
  }
  template <typename TVisitor>
  FORCE_INLINE typename TVisitor::result_type accept(TVisitor& visitor) const {
    return arrayAccept(_data, visitor);
  }
  FORCE_INLINE bool isNull() const {
    return _data == 0;
  }
  FORCE_INLINE operator bool() const {
    return _data != 0;
  }
  FORCE_INLINE size_t memoryUsage() const {
    return _data ? _data->memoryUsage() : 0;
  }
  FORCE_INLINE size_t nesting() const {
    return _data ? _data->nesting() : 0;
  }
  FORCE_INLINE size_t size() const {
    return _data ? _data->size() : 0;
  }
 protected:
  ArrayRefBase(TData* data) : _data(data) {}
  TData* _data;
};
class ArrayConstRef : public ArrayRefBase<const CollectionData>,
                      public Visitable {
  friend class ArrayRef;
  typedef ArrayRefBase<const CollectionData> base_type;
 public:
  typedef ArrayConstRefIterator iterator;
  FORCE_INLINE iterator begin() const {
    if (!_data)
      return iterator();
    return iterator(_data->head());
  }
  FORCE_INLINE iterator end() const {
    return iterator();
  }
  FORCE_INLINE ArrayConstRef() : base_type(0) {}
  FORCE_INLINE ArrayConstRef(const CollectionData* data) : base_type(data) {}
  FORCE_INLINE bool operator==(ArrayConstRef rhs) const {
    return arrayEquals(_data, rhs._data);
  }
  FORCE_INLINE VariantConstRef operator[](size_t index) const {
    return getElement(index);
  }
  FORCE_INLINE VariantConstRef getElement(size_t index) const {
    return VariantConstRef(_data ? _data->getElement(index) : 0);
  }
};
class ArrayRef : public ArrayRefBase<CollectionData>,
                 public ArrayShortcuts<ArrayRef>,
                 public Visitable {
  typedef ArrayRefBase<CollectionData> base_type;
 public:
  typedef ArrayIterator iterator;
  FORCE_INLINE ArrayRef() : base_type(0), _pool(0) {}
  FORCE_INLINE ArrayRef(MemoryPool* pool, CollectionData* data)
      : base_type(data), _pool(pool) {}
  operator VariantRef() {
    void* data = _data;  // prevent warning cast-align
    return VariantRef(_pool, reinterpret_cast<VariantData*>(data));
  }
  operator ArrayConstRef() const {
    return ArrayConstRef(_data);
  }
  VariantRef addElement() const {
    return VariantRef(_pool, arrayAdd(_data, _pool));
  }
  FORCE_INLINE iterator begin() const {
    if (!_data)
      return iterator();
    return iterator(_pool, _data->head());
  }
  FORCE_INLINE iterator end() const {
    return iterator();
  }
  FORCE_INLINE bool set(ArrayConstRef src) const {
    if (!_data || !src._data)
      return false;
    return _data->copyFrom(*src._data, _pool);
  }
  FORCE_INLINE bool operator==(ArrayRef rhs) const {
    return arrayEquals(_data, rhs._data);
  }
  FORCE_INLINE VariantRef getOrAddElement(size_t index) const {
    return VariantRef(_pool, _data ? _data->getOrAddElement(index, _pool) : 0);
  }
  FORCE_INLINE VariantRef getElement(size_t index) const {
    return VariantRef(_pool, _data ? _data->getElement(index) : 0);
  }
  FORCE_INLINE void remove(iterator it) const {
    if (!_data)
      return;
    _data->removeSlot(it.internal());
  }
  FORCE_INLINE void remove(size_t index) const {
    if (!_data)
      return;
    _data->removeElement(index);
  }
 private:
  MemoryPool* _pool;
};
template <typename TVisitor>
typename TVisitor::result_type objectAccept(const CollectionData *obj,
                                            TVisitor &visitor) {
  if (obj)
    return visitor.visitObject(*obj);
  else
    return visitor.visitNull();
}
inline bool objectEquals(const CollectionData *lhs, const CollectionData *rhs) {
  if (lhs == rhs)
    return true;
  if (!lhs || !rhs)
    return false;
  return lhs->equalsObject(*rhs);
}
template <typename TAdaptedString>
inline VariantData *objectGetMember(const CollectionData *obj,
                                    TAdaptedString key) {
  if (!obj)
    return 0;
  return obj->getMember(key);
}
template <typename TAdaptedString>
void objectRemove(CollectionData *obj, TAdaptedString key) {
  if (!obj)
    return;
  obj->removeMember(key);
}
template <typename TAdaptedString>
inline VariantData *objectGetOrAddMember(CollectionData *obj,
                                         TAdaptedString key, MemoryPool *pool) {
  if (!obj)
    return 0;
  return obj->getOrAddMember(key, pool);
}
class String {
 public:
  String() : _data(0), _isStatic(true) {}
  String(const char* data, bool isStaticData = true)
      : _data(data), _isStatic(isStaticData) {}
  const char* c_str() const {
    return _data;
  }
  bool isNull() const {
    return !_data;
  }
  bool isStatic() const {
    return _isStatic;
  }
  friend bool operator==(String lhs, String rhs) {
    if (lhs._data == rhs._data)
      return true;
    if (!lhs._data)
      return false;
    if (!rhs._data)
      return false;
    return strcmp(lhs._data, rhs._data) == 0;
  }
  friend bool operator!=(String lhs, String rhs) {
    if (lhs._data == rhs._data)
      return false;
    if (!lhs._data)
      return true;
    if (!rhs._data)
      return true;
    return strcmp(lhs._data, rhs._data) != 0;
  }
 private:
  const char* _data;
  bool _isStatic;
};
class StringAdapter : public RamStringAdapter {
 public:
  StringAdapter(const String& str)
      : RamStringAdapter(str.c_str()), _isStatic(str.isStatic()) {}
  bool isStatic() const {
    return _isStatic;
  }
  typedef storage_policies::decide_at_runtime storage_policy;
 private:
  bool _isStatic;
};
template <>
struct IsString<String> : true_type {};
inline StringAdapter adaptString(const String& str) {
  return StringAdapter(str);
}
class Pair {
 public:
  Pair(MemoryPool* pool, VariantSlot* slot) {
    if (slot) {
      _key = String(slot->key(), !slot->ownsKey());
      _value = VariantRef(pool, slot->data());
    }
  }
  String key() const {
    return _key;
  }
  VariantRef value() const {
    return _value;
  }
 private:
  String _key;
  VariantRef _value;
};
class PairConst {
 public:
  PairConst(const VariantSlot* slot) {
    if (slot) {
      _key = String(slot->key(), !slot->ownsKey());
      _value = VariantConstRef(slot->data());
    }
  }
  String key() const {
    return _key;
  }
  VariantConstRef value() const {
    return _value;
  }
 private:
  String _key;
  VariantConstRef _value;
};
class PairPtr {
 public:
  PairPtr(MemoryPool *pool, VariantSlot *slot) : _pair(pool, slot) {}
  const Pair *operator->() const {
    return &_pair;
  }
  const Pair &operator*() const {
    return _pair;
  }
 private:
  Pair _pair;
};
class ObjectIterator {
 public:
  ObjectIterator() : _slot(0) {}
  explicit ObjectIterator(MemoryPool *pool, VariantSlot *slot)
      : _pool(pool), _slot(slot) {}
  Pair operator*() const {
    return Pair(_pool, _slot);
  }
  PairPtr operator->() {
    return PairPtr(_pool, _slot);
  }
  bool operator==(const ObjectIterator &other) const {
    return _slot == other._slot;
  }
  bool operator!=(const ObjectIterator &other) const {
    return _slot != other._slot;
  }
  ObjectIterator &operator++() {
    _slot = _slot->next();
    return *this;
  }
  ObjectIterator &operator+=(size_t distance) {
    _slot = _slot->next(distance);
    return *this;
  }
  VariantSlot *internal() {
    return _slot;
  }
 private:
  MemoryPool *_pool;
  VariantSlot *_slot;
};
class PairConstPtr {
 public:
  PairConstPtr(const VariantSlot *slot) : _pair(slot) {}
  const PairConst *operator->() const {
    return &_pair;
  }
  const PairConst &operator*() const {
    return _pair;
  }
 private:
  PairConst _pair;
};
class ObjectConstIterator {
 public:
  ObjectConstIterator() : _slot(0) {}
  explicit ObjectConstIterator(const VariantSlot *slot) : _slot(slot) {}
  PairConst operator*() const {
    return PairConst(_slot);
  }
  PairConstPtr operator->() {
    return PairConstPtr(_slot);
  }
  bool operator==(const ObjectConstIterator &other) const {
    return _slot == other._slot;
  }
  bool operator!=(const ObjectConstIterator &other) const {
    return _slot != other._slot;
  }
  ObjectConstIterator &operator++() {
    _slot = _slot->next();
    return *this;
  }
  ObjectConstIterator &operator+=(size_t distance) {
    _slot = _slot->next(distance);
    return *this;
  }
  const VariantSlot *internal() {
    return _slot;
  }
 private:
  const VariantSlot *_slot;
};
}  // namespace ARDUINOJSON_NAMESPACE
#define JSON_OBJECT_SIZE(NUMBER_OF_ELEMENTS) \
  ((NUMBER_OF_ELEMENTS) * sizeof(ARDUINOJSON_NAMESPACE::VariantSlot))
namespace ARDUINOJSON_NAMESPACE {
template <typename TData>
class ObjectRefBase {
 public:
  operator VariantConstRef() const {
    const void* data = _data;  // prevent warning cast-align
    return VariantConstRef(reinterpret_cast<const VariantData*>(data));
  }
  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor& visitor) const {
    return objectAccept(_data, visitor);
  }
  FORCE_INLINE bool isNull() const {
    return _data == 0;
  }
  FORCE_INLINE operator bool() const {
    return _data != 0;
  }
  FORCE_INLINE size_t memoryUsage() const {
    return _data ? _data->memoryUsage() : 0;
  }
  FORCE_INLINE size_t nesting() const {
    return _data ? _data->nesting() : 0;
  }
  FORCE_INLINE size_t size() const {
    return _data ? _data->size() : 0;
  }
 protected:
  ObjectRefBase(TData* data) : _data(data) {}
  TData* _data;
};
class ObjectConstRef : public ObjectRefBase<const CollectionData>,
                       public Visitable {
  friend class ObjectRef;
  typedef ObjectRefBase<const CollectionData> base_type;
 public:
  typedef ObjectConstIterator iterator;
  ObjectConstRef() : base_type(0) {}
  ObjectConstRef(const CollectionData* data) : base_type(data) {}
  FORCE_INLINE iterator begin() const {
    if (!_data)
      return iterator();
    return iterator(_data->head());
  }
  FORCE_INLINE iterator end() const {
    return iterator();
  }
  template <typename TString>
  FORCE_INLINE bool containsKey(const TString& key) const {
    return !getMember(key).isUndefined();
  }
  template <typename TChar>
  FORCE_INLINE bool containsKey(TChar* key) const {
    return !getMember(key).isUndefined();
  }
  template <typename TString>
  FORCE_INLINE VariantConstRef getMember(const TString& key) const {
    return get_impl(adaptString(key));
  }
  template <typename TChar>
  FORCE_INLINE VariantConstRef getMember(TChar* key) const {
    return get_impl(adaptString(key));
  }
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value, VariantConstRef>::type
      operator[](const TString& key) const {
    return get_impl(adaptString(key));
  }
  template <typename TChar>
  FORCE_INLINE
      typename enable_if<IsString<TChar*>::value, VariantConstRef>::type
      operator[](TChar* key) const {
    return get_impl(adaptString(key));
  }
  FORCE_INLINE bool operator==(ObjectConstRef rhs) const {
    return objectEquals(_data, rhs._data);
  }
 private:
  template <typename TAdaptedString>
  FORCE_INLINE VariantConstRef get_impl(TAdaptedString key) const {
    return VariantConstRef(objectGetMember(_data, key));
  }
};
class ObjectRef : public ObjectRefBase<CollectionData>,
                  public ObjectShortcuts<ObjectRef>,
                  public Visitable {
  typedef ObjectRefBase<CollectionData> base_type;
 public:
  typedef ObjectIterator iterator;
  FORCE_INLINE ObjectRef() : base_type(0), _pool(0) {}
  FORCE_INLINE ObjectRef(MemoryPool* buf, CollectionData* data)
      : base_type(data), _pool(buf) {}
  operator VariantRef() const {
    void* data = _data;  // prevent warning cast-align
    return VariantRef(_pool, reinterpret_cast<VariantData*>(data));
  }
  operator ObjectConstRef() const {
    return ObjectConstRef(_data);
  }
  FORCE_INLINE iterator begin() const {
    if (!_data)
      return iterator();
    return iterator(_pool, _data->head());
  }
  FORCE_INLINE iterator end() const {
    return iterator();
  }
  void clear() const {
    if (!_data)
      return;
    _data->clear();
  }
  FORCE_INLINE bool set(ObjectConstRef src) {
    if (!_data || !src._data)
      return false;
    return _data->copyFrom(*src._data, _pool);
  }
  template <typename TString>
  FORCE_INLINE VariantRef getMember(const TString& key) const {
    return VariantRef(_pool, objectGetMember(_data, adaptString(key)));
  }
  template <typename TChar>
  FORCE_INLINE VariantRef getMember(TChar* key) const {
    return VariantRef(_pool, objectGetMember(_data, adaptString(key)));
  }
  template <typename TString>
  FORCE_INLINE VariantRef getOrAddMember(const TString& key) const {
    return VariantRef(_pool,
                      objectGetOrAddMember(_data, adaptString(key), _pool));
  }
  template <typename TChar>
  FORCE_INLINE VariantRef getOrAddMember(TChar* key) const {
    return VariantRef(_pool,
                      objectGetOrAddMember(_data, adaptString(key), _pool));
  }
  FORCE_INLINE bool operator==(ObjectRef rhs) const {
    return objectEquals(_data, rhs._data);
  }
  FORCE_INLINE void remove(iterator it) const {
    if (!_data)
      return;
    _data->removeSlot(it.internal());
  }
  template <typename TString>
  FORCE_INLINE void remove(const TString& key) const {
    objectRemove(_data, adaptString(key));
  }
  template <typename TChar>
  FORCE_INLINE void remove(TChar* key) const {
    objectRemove(_data, adaptString(key));
  }
 private:
  MemoryPool* _pool;
};
class ArrayRef;
class ObjectRef;
class VariantRef;
template <typename T>
struct VariantTo {};
template <>
struct VariantTo<ArrayRef> {
  typedef ArrayRef type;
};
template <>
struct VariantTo<ObjectRef> {
  typedef ObjectRef type;
};
template <>
struct VariantTo<VariantRef> {
  typedef VariantRef type;
};
}  // namespace ARDUINOJSON_NAMESPACE
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename TArray>
class ElementProxy : public VariantOperators<ElementProxy<TArray> >,
                     public VariantShortcuts<ElementProxy<TArray> >,
                     public Visitable,
                     public VariantTag {
  typedef ElementProxy<TArray> this_type;
 public:
  typedef VariantRef variant_type;
  FORCE_INLINE ElementProxy(TArray array, size_t index)
      : _array(array), _index(index) {}
  FORCE_INLINE ElementProxy(const ElementProxy& src)
      : _array(src._array), _index(src._index) {}
  FORCE_INLINE this_type& operator=(const this_type& src) {
    getOrAddUpstreamElement().set(src.as<VariantConstRef>());
    return *this;
  }
  template <typename T>
  FORCE_INLINE this_type& operator=(const T& src) {
    getOrAddUpstreamElement().set(src);
    return *this;
  }
  template <typename T>
  FORCE_INLINE this_type& operator=(T* src) {
    getOrAddUpstreamElement().set(src);
    return *this;
  }
  FORCE_INLINE void clear() const {
    getUpstreamElement().clear();
  }
  FORCE_INLINE bool isNull() const {
    return getUpstreamElement().isNull();
  }
  template <typename T>
  FORCE_INLINE typename VariantAs<T>::type as() const {
    return getUpstreamElement().template as<T>();
  }
  template <typename T>
  FORCE_INLINE operator T() const {
    return getUpstreamElement();
  }
  template <typename T>
  FORCE_INLINE bool is() const {
    return getUpstreamElement().template is<T>();
  }
  template <typename T>
  FORCE_INLINE typename VariantTo<T>::type to() const {
    return getOrAddUpstreamElement().template to<T>();
  }
  template <typename TValue>
  FORCE_INLINE bool set(const TValue& value) const {
    return getOrAddUpstreamElement().set(value);
  }
  template <typename TValue>
  FORCE_INLINE bool set(TValue* value) const {
    return getOrAddUpstreamElement().set(value);
  }
  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor& visitor) const {
    return getUpstreamElement().accept(visitor);
  }
  FORCE_INLINE size_t size() const {
    return getUpstreamElement().size();
  }
  template <typename TNestedKey>
  VariantRef getMember(TNestedKey* key) const {
    return getUpstreamElement().getMember(key);
  }
  template <typename TNestedKey>
  VariantRef getMember(const TNestedKey& key) const {
    return getUpstreamElement().getMember(key);
  }
  template <typename TNestedKey>
  VariantRef getOrAddMember(TNestedKey* key) const {
    return getOrAddUpstreamElement().getOrAddMember(key);
  }
  template <typename TNestedKey>
  VariantRef getOrAddMember(const TNestedKey& key) const {
    return getOrAddUpstreamElement().getOrAddMember(key);
  }
  VariantRef addElement() const {
    return getOrAddUpstreamElement().addElement();
  }
  VariantRef getElement(size_t index) const {
    return getOrAddUpstreamElement().getElement(index);
  }
  VariantRef getOrAddElement(size_t index) const {
    return getOrAddUpstreamElement().getOrAddElement(index);
  }
  FORCE_INLINE void remove(size_t index) const {
    getUpstreamElement().remove(index);
  }
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value>::type remove(
      TChar* key) const {
    getUpstreamElement().remove(key);
  }
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value>::type remove(
      const TString& key) const {
    getUpstreamElement().remove(key);
  }
 private:
  FORCE_INLINE VariantRef getUpstreamElement() const {
    return _array.getElement(_index);
  }
  FORCE_INLINE VariantRef getOrAddUpstreamElement() const {
    return _array.getOrAddElement(_index);
  }
  TArray _array;
  const size_t _index;
};
}  // namespace ARDUINOJSON_NAMESPACE
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif
namespace ARDUINOJSON_NAMESPACE {
template <typename TObject, typename TStringRef>
class MemberProxy : public VariantOperators<MemberProxy<TObject, TStringRef> >,
                    public VariantShortcuts<MemberProxy<TObject, TStringRef> >,
                    public Visitable,
                    public VariantTag {
  typedef MemberProxy<TObject, TStringRef> this_type;
 public:
  typedef VariantRef variant_type;
  FORCE_INLINE MemberProxy(TObject variant, TStringRef key)
      : _object(variant), _key(key) {}
  FORCE_INLINE MemberProxy(const MemberProxy &src)
      : _object(src._object), _key(src._key) {}
  FORCE_INLINE operator VariantConstRef() const {
    return getUpstreamMember();
  }
  FORCE_INLINE this_type &operator=(const this_type &src) {
    getOrAddUpstreamMember().set(src);
    return *this;
  }
  template <typename TValue>
  FORCE_INLINE typename enable_if<!is_array<TValue>::value, this_type &>::type
  operator=(const TValue &src) {
    getOrAddUpstreamMember().set(src);
    return *this;
  }
  template <typename TChar>
  FORCE_INLINE this_type &operator=(TChar *src) {
    getOrAddUpstreamMember().set(src);
    return *this;
  }
  FORCE_INLINE void clear() const {
    getUpstreamMember().clear();
  }
  FORCE_INLINE bool isNull() const {
    return getUpstreamMember().isNull();
  }
  template <typename TValue>
  FORCE_INLINE typename VariantAs<TValue>::type as() const {
    return getUpstreamMember().template as<TValue>();
  }
  template <typename T>
  FORCE_INLINE operator T() const {
    return getUpstreamMember();
  }
  template <typename TValue>
  FORCE_INLINE bool is() const {
    return getUpstreamMember().template is<TValue>();
  }
  FORCE_INLINE size_t size() const {
    return getUpstreamMember().size();
  }
  FORCE_INLINE void remove(size_t index) const {
    getUpstreamMember().remove(index);
  }
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar *>::value>::type remove(
      TChar *key) const {
    getUpstreamMember().remove(key);
  }
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value>::type remove(
      const TString &key) const {
    getUpstreamMember().remove(key);
  }
  template <typename TValue>
  FORCE_INLINE typename VariantTo<TValue>::type to() {
    return getOrAddUpstreamMember().template to<TValue>();
  }
  template <typename TValue>
  FORCE_INLINE bool set(const TValue &value) {
    return getOrAddUpstreamMember().set(value);
  }
  template <typename TChar>
  FORCE_INLINE bool set(TChar *value) {
    return getOrAddUpstreamMember().set(value);
  }
  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor &visitor) const {
    return getUpstreamMember().accept(visitor);
  }
  FORCE_INLINE VariantRef addElement() const {
    return getOrAddUpstreamMember().addElement();
  }
  FORCE_INLINE VariantRef getElement(size_t index) const {
    return getUpstreamMember().getElement(index);
  }
  FORCE_INLINE VariantRef getOrAddElement(size_t index) const {
    return getOrAddUpstreamMember().getOrAddElement(index);
  }
  template <typename TChar>
  FORCE_INLINE VariantRef getMember(TChar *key) const {
    return getUpstreamMember().getMember(key);
  }
  template <typename TString>
  FORCE_INLINE VariantRef getMember(const TString &key) const {
    return getUpstreamMember().getMember(key);
  }
  template <typename TChar>
  FORCE_INLINE VariantRef getOrAddMember(TChar *key) const {
    return getOrAddUpstreamMember().getOrAddMember(key);
  }
  template <typename TString>
  FORCE_INLINE VariantRef getOrAddMember(const TString &key) const {
    return getOrAddUpstreamMember().getOrAddMember(key);
  }
 private:
  FORCE_INLINE VariantRef getUpstreamMember() const {
    return _object.getMember(_key);
  }
  FORCE_INLINE VariantRef getOrAddUpstreamMember() const {
    return _object.getOrAddMember(_key);
  }
  TObject _object;
  TStringRef _key;
};
}  // namespace ARDUINOJSON_NAMESPACE
#ifdef _MSC_VER
#pragma warning(pop)
#endif
namespace ARDUINOJSON_NAMESPACE {
class JsonDocument : public Visitable {
 public:
  template <typename TVisitor>
  typename TVisitor::result_type accept(TVisitor& visitor) const {
    return getVariant().accept(visitor);
  }
  template <typename T>
  typename VariantAs<T>::type as() {
    return getVariant().template as<T>();
  }
  template <typename T>
  typename VariantConstAs<T>::type as() const {
    return getVariant().template as<T>();
  }
  void clear() {
    _pool.clear();
    _data.setNull();
  }
  template <typename T>
  bool is() const {
    return getVariant().template is<T>();
  }
  bool isNull() const {
    return getVariant().isNull();
  }
  size_t memoryUsage() const {
    return _pool.size();
  }
  bool overflowed() const {
    return _pool.overflowed();
  }
  size_t nesting() const {
    return _data.nesting();
  }
  size_t capacity() const {
    return _pool.capacity();
  }
  size_t size() const {
    return _data.size();
  }
  bool set(const JsonDocument& src) {
    return to<VariantRef>().set(src.as<VariantRef>());
  }
  template <typename T>
  typename enable_if<!is_base_of<JsonDocument, T>::value, bool>::type set(
      const T& src) {
    return to<VariantRef>().set(src);
  }
  template <typename T>
  typename VariantTo<T>::type to() {
    clear();
    return getVariant().template to<T>();
  }
  MemoryPool& memoryPool() {
    return _pool;
  }
  VariantData& data() {
    return _data;
  }
  ArrayRef createNestedArray() {
    return addElement().to<ArrayRef>();
  }
  template <typename TChar>
  ArrayRef createNestedArray(TChar* key) {
    return getOrAddMember(key).template to<ArrayRef>();
  }
  template <typename TString>
  ArrayRef createNestedArray(const TString& key) {
    return getOrAddMember(key).template to<ArrayRef>();
  }
  ObjectRef createNestedObject() {
    return addElement().to<ObjectRef>();
  }
  template <typename TChar>
  ObjectRef createNestedObject(TChar* key) {
    return getOrAddMember(key).template to<ObjectRef>();
  }
  template <typename TString>
  ObjectRef createNestedObject(const TString& key) {
    return getOrAddMember(key).template to<ObjectRef>();
  }
  template <typename TChar>
  bool containsKey(TChar* key) const {
    return !getMember(key).isUndefined();
  }
  template <typename TString>
  bool containsKey(const TString& key) const {
    return !getMember(key).isUndefined();
  }
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value,
                                  MemberProxy<JsonDocument&, TString> >::type
  operator[](const TString& key) {
    return MemberProxy<JsonDocument&, TString>(*this, key);
  }
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value,
                                  MemberProxy<JsonDocument&, TChar*> >::type
  operator[](TChar* key) {
    return MemberProxy<JsonDocument&, TChar*>(*this, key);
  }
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value, VariantConstRef>::type
      operator[](const TString& key) const {
    return getMember(key);
  }
  template <typename TChar>
  FORCE_INLINE
      typename enable_if<IsString<TChar*>::value, VariantConstRef>::type
      operator[](TChar* key) const {
    return getMember(key);
  }
  FORCE_INLINE ElementProxy<JsonDocument&> operator[](size_t index) {
    return ElementProxy<JsonDocument&>(*this, index);
  }
  FORCE_INLINE VariantConstRef operator[](size_t index) const {
    return getElement(index);
  }
  FORCE_INLINE VariantRef getElement(size_t index) {
    return VariantRef(&_pool, _data.getElement(index));
  }
  FORCE_INLINE VariantConstRef getElement(size_t index) const {
    return VariantConstRef(_data.getElement(index));
  }
  FORCE_INLINE VariantRef getOrAddElement(size_t index) {
    return VariantRef(&_pool, _data.getOrAddElement(index, &_pool));
  }
  template <typename TChar>
  FORCE_INLINE VariantConstRef getMember(TChar* key) const {
    return VariantConstRef(_data.getMember(adaptString(key)));
  }
  template <typename TString>
  FORCE_INLINE
      typename enable_if<IsString<TString>::value, VariantConstRef>::type
      getMember(const TString& key) const {
    return VariantConstRef(_data.getMember(adaptString(key)));
  }
  template <typename TChar>
  FORCE_INLINE VariantRef getMember(TChar* key) {
    return VariantRef(&_pool, _data.getMember(adaptString(key)));
  }
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value, VariantRef>::type
  getMember(const TString& key) {
    return VariantRef(&_pool, _data.getMember(adaptString(key)));
  }
  template <typename TChar>
  FORCE_INLINE VariantRef getOrAddMember(TChar* key) {
    return VariantRef(&_pool, _data.getOrAddMember(adaptString(key), &_pool));
  }
  template <typename TString>
  FORCE_INLINE VariantRef getOrAddMember(const TString& key) {
    return VariantRef(&_pool, _data.getOrAddMember(adaptString(key), &_pool));
  }
  FORCE_INLINE VariantRef addElement() {
    return VariantRef(&_pool, _data.addElement(&_pool));
  }
  template <typename TValue>
  FORCE_INLINE bool add(const TValue& value) {
    return addElement().set(value);
  }
  template <typename TChar>
  FORCE_INLINE bool add(TChar* value) {
    return addElement().set(value);
  }
  FORCE_INLINE void remove(size_t index) {
    _data.remove(index);
  }
  template <typename TChar>
  FORCE_INLINE typename enable_if<IsString<TChar*>::value>::type remove(
      TChar* key) {
    _data.remove(adaptString(key));
  }
  template <typename TString>
  FORCE_INLINE typename enable_if<IsString<TString>::value>::type remove(
      const TString& key) {
    _data.remove(adaptString(key));
  }
  FORCE_INLINE operator VariantConstRef() const {
    return VariantConstRef(&_data);
  }
  bool operator==(VariantConstRef rhs) const {
    return getVariant() == rhs;
  }
  bool operator!=(VariantConstRef rhs) const {
    return getVariant() != rhs;
  }
 protected:
  JsonDocument() : _pool(0, 0) {
    _data.setNull();
  }
  JsonDocument(MemoryPool pool) : _pool(pool) {
    _data.setNull();
  }
  JsonDocument(char* buf, size_t capa) : _pool(buf, capa) {
    _data.setNull();
  }
  ~JsonDocument() {}
  void replacePool(MemoryPool pool) {
    _pool = pool;
  }
  VariantRef getVariant() {
    return VariantRef(&_pool, &_data);
  }
  VariantConstRef getVariant() const {
    return VariantConstRef(&_data);
  }
  MemoryPool _pool;
  VariantData _data;
 private:
  JsonDocument(const JsonDocument&);
  JsonDocument& operator=(const JsonDocument&);
};
template <typename TAllocator>
class AllocatorOwner {
 public:
  AllocatorOwner() {}
  AllocatorOwner(const AllocatorOwner& src) : _allocator(src._allocator) {}
  AllocatorOwner(TAllocator a) : _allocator(a) {}
  void* allocate(size_t size) {
    return _allocator.allocate(size);
  }
  void deallocate(void* ptr) {
    if (ptr)
      _allocator.deallocate(ptr);
  }
  void* reallocate(void* ptr, size_t new_size) {
    return _allocator.reallocate(ptr, new_size);
  }
  TAllocator& allocator() {
    return _allocator;
  }
 private:
  TAllocator _allocator;
};
template <typename TAllocator>
class BasicJsonDocument : AllocatorOwner<TAllocator>, public JsonDocument {
 public:
  explicit BasicJsonDocument(size_t capa, TAllocator alloc = TAllocator())
      : AllocatorOwner<TAllocator>(alloc), JsonDocument(allocPool(capa)) {}
  BasicJsonDocument(const BasicJsonDocument& src)
      : AllocatorOwner<TAllocator>(src), JsonDocument() {
    copyAssignFrom(src);
  }
#if ARDUINOJSON_HAS_RVALUE_REFERENCES
  BasicJsonDocument(BasicJsonDocument&& src) : AllocatorOwner<TAllocator>(src) {
    moveAssignFrom(src);
  }
#endif
  BasicJsonDocument(const JsonDocument& src) {
    copyAssignFrom(src);
  }
  template <typename T>
  BasicJsonDocument(
      const T& src,
      typename enable_if<
          is_same<T, VariantRef>::value || is_same<T, VariantConstRef>::value ||
          is_same<T, ArrayRef>::value || is_same<T, ArrayConstRef>::value ||
          is_same<T, ObjectRef>::value ||
          is_same<T, ObjectConstRef>::value>::type* = 0)
      : JsonDocument(allocPool(src.memoryUsage())) {
    set(src);
  }
  BasicJsonDocument(VariantRef src)
      : JsonDocument(allocPool(src.memoryUsage())) {
    set(src);
  }
  ~BasicJsonDocument() {
    freePool();
  }
  BasicJsonDocument& operator=(const BasicJsonDocument& src) {
    copyAssignFrom(src);
    return *this;
  }
#if ARDUINOJSON_HAS_RVALUE_REFERENCES
  BasicJsonDocument& operator=(BasicJsonDocument&& src) {
    moveAssignFrom(src);
    return *this;
  }
#endif
  template <typename T>
  BasicJsonDocument& operator=(const T& src) {
    reallocPoolIfTooSmall(src.memoryUsage());
    set(src);
    return *this;
  }
  void shrinkToFit() {
    ptrdiff_t bytes_reclaimed = _pool.squash();
    if (bytes_reclaimed == 0)
      return;
    void* old_ptr = _pool.buffer();
    void* new_ptr = this->reallocate(old_ptr, _pool.capacity());
    ptrdiff_t ptr_offset =
        static_cast<char*>(new_ptr) - static_cast<char*>(old_ptr);
    _pool.movePointers(ptr_offset);
    _data.movePointers(ptr_offset, ptr_offset - bytes_reclaimed);
  }
  bool garbageCollect() {
    BasicJsonDocument tmp(*this);
    if (!tmp.capacity())
      return false;
    tmp.set(*this);
    moveAssignFrom(tmp);
    return true;
  }
  using AllocatorOwner<TAllocator>::allocator;
 private:
  MemoryPool allocPool(size_t requiredSize) {
    size_t capa = addPadding(requiredSize);
    return MemoryPool(reinterpret_cast<char*>(this->allocate(capa)), capa);
  }
  void reallocPoolIfTooSmall(size_t requiredSize) {
    if (requiredSize <= capacity())
      return;
    freePool();
    replacePool(allocPool(addPadding(requiredSize)));
  }
  void freePool() {
    this->deallocate(memoryPool().buffer());
  }
  void copyAssignFrom(const JsonDocument& src) {
    reallocPoolIfTooSmall(src.capacity());
    set(src);
  }
  void moveAssignFrom(BasicJsonDocument& src) {
    freePool();
    _data = src._data;
    _pool = src._pool;
    src._data.setNull();
    src._pool = MemoryPool(0, 0);
  }
};
struct DefaultAllocator {
  void* allocate(size_t size) {
    return malloc(size);
  }
  void deallocate(void* ptr) {
    free(ptr);
  }
  void* reallocate(void* ptr, size_t new_size) {
    return realloc(ptr, new_size);
  }
};
typedef BasicJsonDocument<DefaultAllocator> DynamicJsonDocument;
template <size_t desiredCapacity>
class StaticJsonDocument : public JsonDocument {
  static const size_t _capacity =
      AddPadding<Max<1, desiredCapacity>::value>::value;
 public:
  StaticJsonDocument() : JsonDocument(_buffer, _capacity) {}
  StaticJsonDocument(const StaticJsonDocument& src)
      : JsonDocument(_buffer, _capacity) {
    set(src);
  }
  template <typename T>
  StaticJsonDocument(const T& src,
                     typename enable_if<IsVisitable<T>::value>::type* = 0)
      : JsonDocument(_buffer, _capacity) {
    set(src);
  }
  StaticJsonDocument(VariantRef src) : JsonDocument(_buffer, _capacity) {
    set(src);
  }
  StaticJsonDocument operator=(const StaticJsonDocument& src) {
    set(src);
    return *this;
  }
  template <typename T>
  StaticJsonDocument operator=(const T& src) {
    set(src);
    return *this;
  }
  void garbageCollect() {
    StaticJsonDocument tmp(*this);
    set(tmp);
  }
 private:
  char _buffer[_capacity];
};
template <typename TArray>
inline ArrayRef ArrayShortcuts<TArray>::createNestedArray() const {
  return impl()->addElement().template to<ArrayRef>();
}
template <typename TArray>
inline ObjectRef ArrayShortcuts<TArray>::createNestedObject() const {
  return impl()->addElement().template to<ObjectRef>();
}
template <typename TArray>
inline ElementProxy<TArray> ArrayShortcuts<TArray>::operator[](
    size_t index) const {
  return ElementProxy<TArray>(*impl(), index);
}
template <typename T, size_t N, typename TDestination>
inline typename enable_if<!is_array<T>::value &&
                              !is_base_of<JsonDocument, TDestination>::value,
                          bool>::type
copyArray(T (&src)[N], const TDestination& dst) {
  return copyArray(src, N, dst);
}
template <typename T, size_t N>
inline bool copyArray(T (&src)[N], JsonDocument& dst) {
  return copyArray(src, dst.to<ArrayRef>());
}
template <typename T, typename TDestination>
inline typename enable_if<!is_array<T>::value &&
                              !is_base_of<JsonDocument, TDestination>::value,
                          bool>::type
copyArray(T* src, size_t len, const TDestination& dst) {
  bool ok = true;
  for (size_t i = 0; i < len; i++) {
    ok &= dst.add(src[i]);
  }
  return ok;
}
template <typename T>
inline bool copyArray(T* src, size_t len, JsonDocument& dst) {
  return copyArray(src, len, dst.to<ArrayRef>());
}
template <typename T, size_t N1, size_t N2, typename TDestination>
inline typename enable_if<!is_base_of<JsonDocument, TDestination>::value,
                          bool>::type
copyArray(T (&src)[N1][N2], const TDestination& dst) {
  bool ok = true;
  for (size_t i = 0; i < N1; i++) {
    ArrayRef nestedArray = dst.createNestedArray();
    for (size_t j = 0; j < N2; j++) {
      ok &= nestedArray.add(src[i][j]);
    }
  }
  return ok;
}
template <typename T, size_t N1, size_t N2>
inline bool copyArray(T (&src)[N1][N2], JsonDocument& dst) {
  return copyArray(src, dst.to<ArrayRef>());
}
template <typename T>
class ArrayCopier1D : public Visitor<size_t> {
 public:
  ArrayCopier1D(T* destination, size_t capacity)
      : _destination(destination), _capacity(capacity) {}
  size_t visitArray(const CollectionData& array) {
    size_t size = 0;
    VariantSlot* slot = array.head();
    while (slot != 0 && size < _capacity) {
      _destination[size++] = variantAs<T>(slot->data());
      slot = slot->next();
    }
    return size;
  }
  size_t visitObject(const CollectionData&) {
    return 0;
  }
  size_t visitFloat(Float) {
    return 0;
  }
  size_t visitString(const char*) {
    return 0;
  }
  size_t visitRawJson(const char*, size_t) {
    return 0;
  }
  size_t visitNegativeInteger(UInt) {
    return 0;
  }
  size_t visitPositiveInteger(UInt) {
    return 0;
  }
  size_t visitBoolean(bool) {
    return 0;
  }
  size_t visitNull() {
    return 0;
  }
 private:
  T* _destination;
  size_t _capacity;
};
template <typename T, size_t N1, size_t N2>
class ArrayCopier2D : public Visitor<void> {
 public:
  ArrayCopier2D(T (*destination)[N1][N2]) : _destination(destination) {}
  void visitArray(const CollectionData& array) {
    VariantSlot* slot = array.head();
    size_t n = 0;
    while (slot != 0 && n < N1) {
      ArrayCopier1D<T> copier((*_destination)[n++], N2);
      variantAccept(slot->data(), copier);
      slot = slot->next();
    }
  }
  void visitObject(const CollectionData&) {}
  void visitFloat(Float) {}
  void visitString(const char*) {}
  void visitRawJson(const char*, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool) {}
  void visitNull() {}
 private:
  T (*_destination)[N1][N2];
  size_t _capacity1, _capacity2;
};
template <typename TSource, typename T, size_t N>
inline typename enable_if<!is_array<T>::value, size_t>::type copyArray(
    const TSource& src, T (&dst)[N]) {
  return copyArray(src, dst, N);
}
template <typename TSource, typename T>
inline size_t copyArray(const TSource& src, T* dst, size_t len) {
  ArrayCopier1D<T> copier(dst, len);
  return src.accept(copier);
}
template <typename TSource, typename T, size_t N1, size_t N2>
inline void copyArray(const TSource& src, T (&dst)[N1][N2]) {
  ArrayCopier2D<T, N1, N2> copier(&dst);
  src.accept(copier);
}
inline bool variantEquals(const VariantData* a, const VariantData* b) {
  return variantCompare(a, b) == COMPARE_RESULT_EQUAL;
}
inline VariantSlot* CollectionData::addSlot(MemoryPool* pool) {
  VariantSlot* slot = pool->allocVariant();
  if (!slot)
    return 0;
  if (_tail) {
    _tail->setNextNotNull(slot);
    _tail = slot;
  } else {
    _head = slot;
    _tail = slot;
  }
  slot->clear();
  return slot;
}
inline VariantData* CollectionData::addElement(MemoryPool* pool) {
  return slotData(addSlot(pool));
}
template <typename TAdaptedString>
inline VariantData* CollectionData::addMember(TAdaptedString key,
                                              MemoryPool* pool) {
  VariantSlot* slot = addSlot(pool);
  if (!slotSetKey(slot, key, pool)) {
    removeSlot(slot);
    return 0;
  }
  return slot->data();
}
inline void CollectionData::clear() {
  _head = 0;
  _tail = 0;
}
template <typename TAdaptedString>
inline bool CollectionData::containsKey(const TAdaptedString& key) const {
  return getSlot(key) != 0;
}
inline bool CollectionData::copyFrom(const CollectionData& src,
                                     MemoryPool* pool) {
  clear();
  for (VariantSlot* s = src._head; s; s = s->next()) {
    VariantData* var;
    if (s->key() != 0) {
      if (s->ownsKey())
        var = addMember(RamStringAdapter(s->key()), pool);
      else
        var = addMember(ConstRamStringAdapter(s->key()), pool);
    } else {
      var = addElement(pool);
    }
    if (!var)
      return false;
    if (!var->copyFrom(*s->data(), pool))
      return false;
  }
  return true;
}
inline bool CollectionData::equalsObject(const CollectionData& other) const {
  size_t count = 0;
  for (VariantSlot* slot = _head; slot; slot = slot->next()) {
    VariantData* v1 = slot->data();
    VariantData* v2 = other.getMember(adaptString(slot->key()));
    if (!variantEquals(v1, v2))
      return false;
    count++;
  }
  return count == other.size();
}
inline bool CollectionData::equalsArray(const CollectionData& other) const {
  VariantSlot* s1 = _head;
  VariantSlot* s2 = other._head;
  for (;;) {
    if (s1 == s2)
      return true;
    if (!s1 || !s2)
      return false;
    if (!variantEquals(s1->data(), s2->data()))
      return false;
    s1 = s1->next();
    s2 = s2->next();
  }
}
template <typename TAdaptedString>
inline VariantSlot* CollectionData::getSlot(TAdaptedString key) const {
  VariantSlot* slot = _head;
  while (slot) {
    if (key.equals(slot->key()))
      break;
    slot = slot->next();
  }
  return slot;
}
inline VariantSlot* CollectionData::getSlot(size_t index) const {
  return _head->next(index);
}
inline VariantSlot* CollectionData::getPreviousSlot(VariantSlot* target) const {
  VariantSlot* current = _head;
  while (current) {
    VariantSlot* next = current->next();
    if (next == target)
      return current;
    current = next;
  }
  return 0;
}
template <typename TAdaptedString>
inline VariantData* CollectionData::getMember(TAdaptedString key) const {
  VariantSlot* slot = getSlot(key);
  return slot ? slot->data() : 0;
}
template <typename TAdaptedString>
inline VariantData* CollectionData::getOrAddMember(TAdaptedString key,
                                                   MemoryPool* pool) {
  if (key.isNull())
    return 0;
  VariantSlot* slot = getSlot(key);
  if (slot)
    return slot->data();
  return addMember(key, pool);
}
inline VariantData* CollectionData::getElement(size_t index) const {
  VariantSlot* slot = getSlot(index);
  return slot ? slot->data() : 0;
}
inline VariantData* CollectionData::getOrAddElement(size_t index,
                                                    MemoryPool* pool) {
  VariantSlot* slot = _head;
  while (slot && index > 0) {
    slot = slot->next();
    index--;
  }
  if (!slot)
    index++;
  while (index > 0) {
    slot = addSlot(pool);
    index--;
  }
  return slotData(slot);
}
inline void CollectionData::removeSlot(VariantSlot* slot) {
  if (!slot)
    return;
  VariantSlot* prev = getPreviousSlot(slot);
  VariantSlot* next = slot->next();
  if (prev)
    prev->setNext(next);
  else
    _head = next;
  if (!next)
    _tail = prev;
}
inline void CollectionData::removeElement(size_t index) {
  removeSlot(getSlot(index));
}
inline size_t CollectionData::memoryUsage() const {
  size_t total = 0;
  for (VariantSlot* s = _head; s; s = s->next()) {
    total += sizeof(VariantSlot) + s->data()->memoryUsage();
    if (s->ownsKey())
      total += strlen(s->key()) + 1;
  }
  return total;
}
inline size_t CollectionData::nesting() const {
  size_t maxChildNesting = 0;
  for (VariantSlot* s = _head; s; s = s->next()) {
    size_t childNesting = s->data()->nesting();
    if (childNesting > maxChildNesting)
      maxChildNesting = childNesting;
  }
  return maxChildNesting + 1;
}
inline size_t CollectionData::size() const {
  return slotSize(_head);
}
template <typename T>
inline void movePointer(T*& p, ptrdiff_t offset) {
  if (!p)
    return;
  p = reinterpret_cast<T*>(
      reinterpret_cast<void*>(reinterpret_cast<char*>(p) + offset));
  ARDUINOJSON_ASSERT(isAligned(p));
}
inline void CollectionData::movePointers(ptrdiff_t stringDistance,
                                         ptrdiff_t variantDistance) {
  movePointer(_head, variantDistance);
  movePointer(_tail, variantDistance);
  for (VariantSlot* slot = _head; slot; slot = slot->next())
    slot->movePointers(stringDistance, variantDistance);
}
template <typename TObject>
template <typename TString>
inline ArrayRef ObjectShortcuts<TObject>::createNestedArray(
    const TString& key) const {
  return impl()->getOrAddMember(key).template to<ArrayRef>();
}
template <typename TObject>
template <typename TChar>
inline ArrayRef ObjectShortcuts<TObject>::createNestedArray(TChar* key) const {
  return impl()->getOrAddMember(key).template to<ArrayRef>();
}
template <typename TObject>
template <typename TString>
inline ObjectRef ObjectShortcuts<TObject>::createNestedObject(
    const TString& key) const {
  return impl()->getOrAddMember(key).template to<ObjectRef>();
}
template <typename TObject>
template <typename TChar>
inline ObjectRef ObjectShortcuts<TObject>::createNestedObject(
    TChar* key) const {
  return impl()->getOrAddMember(key).template to<ObjectRef>();
}
template <typename TObject>
template <typename TString>
inline typename enable_if<IsString<TString>::value, bool>::type
ObjectShortcuts<TObject>::containsKey(const TString& key) const {
  return !impl()->getMember(key).isUndefined();
}
template <typename TObject>
template <typename TChar>
inline typename enable_if<IsString<TChar*>::value, bool>::type
ObjectShortcuts<TObject>::containsKey(TChar* key) const {
  return !impl()->getMember(key).isUndefined();
}
template <typename TObject>
template <typename TString>
inline typename enable_if<IsString<TString*>::value,
                          MemberProxy<TObject, TString*> >::type
    ObjectShortcuts<TObject>::operator[](TString* key) const {
  return MemberProxy<TObject, TString*>(*impl(), key);
}
template <typename TObject>
template <typename TString>
inline typename enable_if<IsString<TString>::value,
                          MemberProxy<TObject, TString> >::type
    ObjectShortcuts<TObject>::operator[](const TString& key) const {
  return MemberProxy<TObject, TString>(*impl(), key);
}
template <typename T>
inline typename enable_if<is_same<ArrayConstRef, T>::value, T>::type variantAs(
    const VariantData* _data) {
  return ArrayConstRef(variantAsArray(_data));
}
template <typename T>
inline typename enable_if<is_same<ObjectConstRef, T>::value, T>::type variantAs(
    const VariantData* _data) {
  return ObjectConstRef(variantAsObject(_data));
}
template <typename T>
inline typename enable_if<is_same<VariantConstRef, T>::value, T>::type
variantAs(const VariantData* _data) {
  return VariantConstRef(_data);
}
template <typename T>
inline typename enable_if<IsWriteableString<T>::value, T>::type variantAs(
    const VariantData* _data) {
  const char* cstr = _data != 0 ? _data->asString() : 0;
  if (cstr)
    return T(cstr);
  T s;
  serializeJson(VariantConstRef(_data), s);
  return s;
}
template <>
inline ArrayRef variantAs<ArrayRef>(VariantData* data, MemoryPool* pool) {
  return ArrayRef(pool, data != 0 ? data->asArray() : 0);
}
template <>
inline ObjectRef variantAs<ObjectRef>(VariantData* data, MemoryPool* pool) {
  return ObjectRef(pool, data != 0 ? data->asObject() : 0);
}
template <>
inline VariantRef variantAs<VariantRef>(VariantData* data, MemoryPool* pool) {
  return VariantRef(pool, data);
}
class CollectionData;
struct ComparerBase : Visitor<CompareResult> {
  CompareResult visitArray(const CollectionData &) {
    return COMPARE_RESULT_DIFFER;
  }
  CompareResult visitBoolean(bool) {
    return COMPARE_RESULT_DIFFER;
  }
  CompareResult visitFloat(Float) {
    return COMPARE_RESULT_DIFFER;
  }
  CompareResult visitNegativeInteger(UInt) {
    return COMPARE_RESULT_DIFFER;
  }
  CompareResult visitNull() {
    return COMPARE_RESULT_DIFFER;
  }
  CompareResult visitObject(const CollectionData &) {
    return COMPARE_RESULT_DIFFER;
  }
  CompareResult visitPositiveInteger(UInt) {
    return COMPARE_RESULT_DIFFER;
  }
  CompareResult visitRawJson(const char *, size_t) {
    return COMPARE_RESULT_DIFFER;
  }
  CompareResult visitString(const char *) {
    return COMPARE_RESULT_DIFFER;
  }
};
template <typename T, typename Enable = void>
struct Comparer;
template <typename T>
struct Comparer<T, typename enable_if<IsString<T>::value>::type>
    : ComparerBase {
  T rhs;
  explicit Comparer(T value) : rhs(value) {}
  CompareResult visitString(const char *lhs) {
    int i = adaptString(rhs).compare(lhs);
    if (i < 0)
      return COMPARE_RESULT_GREATER;
    else if (i > 0)
      return COMPARE_RESULT_LESS;
    else
      return COMPARE_RESULT_EQUAL;
  }
  CompareResult visitNull() {
    if (adaptString(rhs).isNull())
      return COMPARE_RESULT_EQUAL;
    else
      return COMPARE_RESULT_DIFFER;
  }
};
template <typename T>
struct Comparer<T, typename enable_if<is_integral<T>::value ||
                                      is_floating_point<T>::value>::type>
    : ComparerBase {
  T rhs;
  explicit Comparer(T value) : rhs(value) {}
  CompareResult visitFloat(Float lhs) {
    return arithmeticCompare(lhs, rhs);
  }
  CompareResult visitNegativeInteger(UInt lhs) {
    return arithmeticCompareNegateLeft(lhs, rhs);
  }
  CompareResult visitPositiveInteger(UInt lhs) {
    return arithmeticCompare(lhs, rhs);
  }
  CompareResult visitBoolean(bool lhs) {
    return visitPositiveInteger(static_cast<UInt>(lhs));
  }
};
struct NullComparer : ComparerBase {
  CompareResult visitNull() {
    return COMPARE_RESULT_EQUAL;
  }
};
#if ARDUINOJSON_HAS_NULLPTR
template <>
struct Comparer<decltype(nullptr), void> : NullComparer {
  explicit Comparer(decltype(nullptr)) : NullComparer() {}
};
#endif
struct ArrayComparer : ComparerBase {
  const CollectionData *_rhs;
  explicit ArrayComparer(const CollectionData &rhs) : _rhs(&rhs) {}
  CompareResult visitArray(const CollectionData &lhs) {
    if (lhs.equalsArray(*_rhs))
      return COMPARE_RESULT_EQUAL;
    else
      return COMPARE_RESULT_DIFFER;
  }
};
struct NegativeIntegerComparer : ComparerBase {
  UInt _rhs;
  explicit NegativeIntegerComparer(UInt rhs) : _rhs(rhs) {}
  CompareResult visitFloat(Float lhs) {
    return arithmeticCompareNegateRight(lhs, _rhs);
  }
  CompareResult visitNegativeInteger(UInt lhs) {
    return arithmeticCompare(_rhs, lhs);
  }
  CompareResult visitPositiveInteger(UInt) {
    return COMPARE_RESULT_GREATER;
  }
  CompareResult visitBoolean(bool) {
    return COMPARE_RESULT_GREATER;
  }
};
struct ObjectComparer : ComparerBase {
  const CollectionData *_rhs;
  explicit ObjectComparer(const CollectionData &rhs) : _rhs(&rhs) {}
  CompareResult visitObject(const CollectionData &lhs) {
    if (lhs.equalsObject(*_rhs))
      return COMPARE_RESULT_EQUAL;
    else
      return COMPARE_RESULT_DIFFER;
  }
};
struct RawComparer : ComparerBase {
  const char *_rhsData;
  size_t _rhsSize;
  explicit RawComparer(const char *rhsData, size_t rhsSize)
      : _rhsData(rhsData), _rhsSize(rhsSize) {}
  CompareResult visitRawJson(const char *lhsData, size_t lhsSize) {
    size_t size = _rhsSize < lhsSize ? _rhsSize : lhsSize;
    int n = memcmp(lhsData, _rhsData, size);
    if (n < 0)
      return COMPARE_RESULT_LESS;
    else if (n > 0)
      return COMPARE_RESULT_GREATER;
    else
      return COMPARE_RESULT_EQUAL;
  }
};
template <typename T>
struct Comparer<T, typename enable_if<IsVisitable<T>::value>::type>
    : ComparerBase {
  T rhs;
  explicit Comparer(T value) : rhs(value) {}
  CompareResult visitArray(const CollectionData &lhs) {
    ArrayComparer comparer(lhs);
    return accept(comparer);
  }
  CompareResult visitObject(const CollectionData &lhs) {
    ObjectComparer comparer(lhs);
    return accept(comparer);
  }
  CompareResult visitFloat(Float lhs) {
    Comparer<Float> comparer(lhs);
    return accept(comparer);
  }
  CompareResult visitString(const char *lhs) {
    Comparer<const char *> comparer(lhs);
    return accept(comparer);
  }
  CompareResult visitRawJson(const char *lhsData, size_t lhsSize) {
    RawComparer comparer(lhsData, lhsSize);
    return accept(comparer);
  }
  CompareResult visitNegativeInteger(UInt lhs) {
    NegativeIntegerComparer comparer(lhs);
    return accept(comparer);
  }
  CompareResult visitPositiveInteger(UInt lhs) {
    Comparer<UInt> comparer(lhs);
    return accept(comparer);
  }
  CompareResult visitBoolean(bool lhs) {
    Comparer<bool> comparer(lhs);
    return accept(comparer);
  }
  CompareResult visitNull() {
    NullComparer comparer;
    return accept(comparer);
  }
 private:
  template <typename TComparer>
  CompareResult accept(TComparer &comparer) {
    CompareResult reversedResult = rhs.accept(comparer);
    switch (reversedResult) {
      case COMPARE_RESULT_GREATER:
        return COMPARE_RESULT_LESS;
      case COMPARE_RESULT_LESS:
        return COMPARE_RESULT_GREATER;
      default:
        return reversedResult;
    }
  }
};
template <typename T1, typename T2>
CompareResult compare(const T1 &lhs, const T2 &rhs) {
  Comparer<T2> comparer(rhs);
  return lhs.accept(comparer);
}
inline int variantCompare(const VariantData *a, const VariantData *b) {
  return compare(VariantConstRef(a), VariantConstRef(b));
}
#ifndef isdigit
inline bool isdigit(char c) {
  return '0' <= c && c <= '9';
}
#endif
inline bool issign(char c) {
  return '-' == c || c == '+';
}
template <typename A, typename B>
struct choose_largest : conditional<(sizeof(A) > sizeof(B)), A, B> {};
inline bool parseNumber(const char* s, VariantData& result) {
  typedef FloatTraits<Float> traits;
  typedef choose_largest<traits::mantissa_type, UInt>::type mantissa_t;
  typedef traits::exponent_type exponent_t;
  ARDUINOJSON_ASSERT(s != 0);
  bool is_negative = false;
  switch (*s) {
    case '-':
      is_negative = true;
      s++;
      break;
    case '+':
      s++;
      break;
  }
#if ARDUINOJSON_ENABLE_NAN
  if (*s == 'n' || *s == 'N') {
    result.setFloat(traits::nan());
    return true;
  }
#endif
#if ARDUINOJSON_ENABLE_INFINITY
  if (*s == 'i' || *s == 'I') {
    result.setFloat(is_negative ? -traits::inf() : traits::inf());
    return true;
  }
#endif
  if (!isdigit(*s) && *s != '.')
    return false;
  mantissa_t mantissa = 0;
  exponent_t exponent_offset = 0;
  const mantissa_t maxUint = UInt(-1);
  while (isdigit(*s)) {
    uint8_t digit = uint8_t(*s - '0');
    if (mantissa > maxUint / 10)
      break;
    mantissa *= 10;
    if (mantissa > maxUint - digit)
      break;
    mantissa += digit;
    s++;
  }
  if (*s == '\0') {
    if (is_negative)
      result.setNegativeInteger(UInt(mantissa));
    else
      result.setPositiveInteger(UInt(mantissa));
    return true;
  }
  while (mantissa > traits::mantissa_max) {
    mantissa /= 10;
    exponent_offset++;
  }
  while (isdigit(*s)) {
    exponent_offset++;
    s++;
  }
  if (*s == '.') {
    s++;
    while (isdigit(*s)) {
      if (mantissa < traits::mantissa_max / 10) {
        mantissa = mantissa * 10 + uint8_t(*s - '0');
        exponent_offset--;
      }
      s++;
    }
  }
  int exponent = 0;
  if (*s == 'e' || *s == 'E') {
    s++;
    bool negative_exponent = false;
    if (*s == '-') {
      negative_exponent = true;
      s++;
    } else if (*s == '+') {
      s++;
    }
    while (isdigit(*s)) {
      exponent = exponent * 10 + (*s - '0');
      if (exponent + exponent_offset > traits::exponent_max) {
        if (negative_exponent)
          result.setFloat(is_negative ? -0.0f : 0.0f);
        else
          result.setFloat(is_negative ? -traits::inf() : traits::inf());
        return true;
      }
      s++;
    }
    if (negative_exponent)
      exponent = -exponent;
  }
  exponent += exponent_offset;
  if (*s != '\0')
    return false;
  Float final_result =
      traits::make_float(static_cast<Float>(mantissa), exponent);
  result.setFloat(is_negative ? -final_result : final_result);
  return true;
}
template <typename T>
inline T parseNumber(const char* s) {
  VariantData value;
  value.init();  // VariantData is a POD, so it has no constructor
  parseNumber(s, value);
  return variantAs<T>(&value);
}
template <typename T>
inline T VariantData::asIntegral() const {
  switch (type()) {
    case VALUE_IS_POSITIVE_INTEGER:
    case VALUE_IS_BOOLEAN:
      return convertPositiveInteger<T>(_content.asInteger);
    case VALUE_IS_NEGATIVE_INTEGER:
      return convertNegativeInteger<T>(_content.asInteger);
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return parseNumber<T>(_content.asString);
    case VALUE_IS_FLOAT:
      return convertFloat<T>(_content.asFloat);
    default:
      return 0;
  }
}
inline bool VariantData::asBoolean() const {
  switch (type()) {
    case VALUE_IS_POSITIVE_INTEGER:
    case VALUE_IS_BOOLEAN:
    case VALUE_IS_NEGATIVE_INTEGER:
      return _content.asInteger != 0;
    case VALUE_IS_FLOAT:
      return _content.asFloat != 0;
    case VALUE_IS_NULL:
      return false;
    default:
      return true;
  }
}
template <typename T>
inline T VariantData::asFloat() const {
  switch (type()) {
    case VALUE_IS_POSITIVE_INTEGER:
    case VALUE_IS_BOOLEAN:
      return static_cast<T>(_content.asInteger);
    case VALUE_IS_NEGATIVE_INTEGER:
      return -static_cast<T>(_content.asInteger);
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return parseNumber<T>(_content.asString);
    case VALUE_IS_FLOAT:
      return static_cast<T>(_content.asFloat);
    default:
      return 0;
  }
}
inline const char *VariantData::asString() const {
  switch (type()) {
    case VALUE_IS_LINKED_STRING:
    case VALUE_IS_OWNED_STRING:
      return _content.asString;
    default:
      return 0;
  }
}
template <typename TVariant>
typename enable_if<IsVisitable<TVariant>::value, bool>::type VariantRef::set(
    const TVariant &value) const {
  VariantConstRef v = value;
  return variantCopyFrom(_data, v._data, _pool);
}
template <typename T>
inline typename enable_if<is_same<T, ArrayRef>::value, ArrayRef>::type
VariantRef::to() const {
  return ArrayRef(_pool, variantToArray(_data));
}
template <typename T>
typename enable_if<is_same<T, ObjectRef>::value, ObjectRef>::type
VariantRef::to() const {
  return ObjectRef(_pool, variantToObject(_data));
}
template <typename T>
typename enable_if<is_same<T, VariantRef>::value, VariantRef>::type
VariantRef::to() const {
  variantSetNull(_data);
  return *this;
}
inline VariantConstRef VariantConstRef::getElement(size_t index) const {
  return ArrayConstRef(_data != 0 ? _data->asArray() : 0)[index];
}
inline VariantRef VariantRef::addElement() const {
  return VariantRef(_pool, variantAddElement(_data, _pool));
}
inline VariantRef VariantRef::getElement(size_t index) const {
  return VariantRef(_pool, _data != 0 ? _data->getElement(index) : 0);
}
inline VariantRef VariantRef::getOrAddElement(size_t index) const {
  return VariantRef(_pool, variantGetOrAddElement(_data, index, _pool));
}
template <typename TChar>
inline VariantRef VariantRef::getMember(TChar *key) const {
  return VariantRef(_pool, _data != 0 ? _data->getMember(adaptString(key)) : 0);
}
template <typename TString>
inline typename enable_if<IsString<TString>::value, VariantRef>::type
VariantRef::getMember(const TString &key) const {
  return VariantRef(_pool, _data != 0 ? _data->getMember(adaptString(key)) : 0);
}
template <typename TChar>
inline VariantRef VariantRef::getOrAddMember(TChar *key) const {
  return VariantRef(_pool, variantGetOrAddMember(_data, key, _pool));
}
template <typename TString>
inline VariantRef VariantRef::getOrAddMember(const TString &key) const {
  return VariantRef(_pool, variantGetOrAddMember(_data, key, _pool));
}
inline VariantConstRef operator|(VariantConstRef preferedValue,
                                 VariantConstRef defaultValue) {
  return preferedValue ? preferedValue : defaultValue;
}
}  // namespace ARDUINOJSON_NAMESPACE
#if ARDUINOJSON_ENABLE_STD_STREAM
#include <ostream>
#endif
namespace ARDUINOJSON_NAMESPACE {
class DeserializationError {
  typedef void (DeserializationError::*bool_type)() const;
  void safeBoolHelper() const {}
 public:
  enum Code {
    Ok,
    EmptyInput,
    IncompleteInput,
    InvalidInput,
    NoMemory,
    NotSupported,
    TooDeep
  };
  DeserializationError() {}
  DeserializationError(Code c) : _code(c) {}
  friend bool operator==(const DeserializationError& lhs,
                         const DeserializationError& rhs) {
    return lhs._code == rhs._code;
  }
  friend bool operator!=(const DeserializationError& lhs,
                         const DeserializationError& rhs) {
    return lhs._code != rhs._code;
  }
  friend bool operator==(const DeserializationError& lhs, Code rhs) {
    return lhs._code == rhs;
  }
  friend bool operator==(Code lhs, const DeserializationError& rhs) {
    return lhs == rhs._code;
  }
  friend bool operator!=(const DeserializationError& lhs, Code rhs) {
    return lhs._code != rhs;
  }
  friend bool operator!=(Code lhs, const DeserializationError& rhs) {
    return lhs != rhs._code;
  }
  operator bool_type() const {
    return _code != Ok ? &DeserializationError::safeBoolHelper : 0;
  }
  friend bool operator==(bool value, const DeserializationError& err) {
    return static_cast<bool>(err) == value;
  }
  friend bool operator==(const DeserializationError& err, bool value) {
    return static_cast<bool>(err) == value;
  }
  friend bool operator!=(bool value, const DeserializationError& err) {
    return static_cast<bool>(err) != value;
  }
  friend bool operator!=(const DeserializationError& err, bool value) {
    return static_cast<bool>(err) != value;
  }
  Code code() const {
    return _code;
  }
  const char* c_str() const {
    static const char* messages[] = {
        "Ok",       "EmptyInput",   "IncompleteInput", "InvalidInput",
        "NoMemory", "NotSupported", "TooDeep"};
    ARDUINOJSON_ASSERT(static_cast<size_t>(_code) <
                       sizeof(messages) / sizeof(messages[0]));
    return messages[_code];
  }
#if ARDUINOJSON_ENABLE_PROGMEM
  const __FlashStringHelper* f_str() const {
    ARDUINOJSON_DEFINE_STATIC_ARRAY(char, s0, "Ok");
    ARDUINOJSON_DEFINE_STATIC_ARRAY(char, s1, "EmptyInput");
    ARDUINOJSON_DEFINE_STATIC_ARRAY(char, s2, "IncompleteInput");
    ARDUINOJSON_DEFINE_STATIC_ARRAY(char, s3, "InvalidInput");
    ARDUINOJSON_DEFINE_STATIC_ARRAY(char, s4, "NoMemory");
    ARDUINOJSON_DEFINE_STATIC_ARRAY(char, s5, "NotSupported");
    ARDUINOJSON_DEFINE_STATIC_ARRAY(char, s6, "TooDeep");
    ARDUINOJSON_DEFINE_STATIC_ARRAY(
        const char*, messages,
        ARDUINOJSON_EXPAND7({s0, s1, s2, s3, s4, s5, s6}));
    return ARDUINOJSON_READ_STATIC_ARRAY(const __FlashStringHelper*, messages,
                                         _code);
  }
#endif
 private:
  Code _code;
};
#if ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream& operator<<(std::ostream& s,
                                const DeserializationError& e) {
  s << e.c_str();
  return s;
}
inline std::ostream& operator<<(std::ostream& s, DeserializationError::Code c) {
  s << DeserializationError(c).c_str();
  return s;
}
#endif
class Filter {
 public:
  explicit Filter(VariantConstRef v) : _variant(v) {}
  bool allow() const {
    return _variant;
  }
  bool allowArray() const {
    return _variant == true || _variant.is<ArrayRef>();
  }
  bool allowObject() const {
    return _variant == true || _variant.is<ObjectRef>();
  }
  bool allowValue() const {
    return _variant == true;
  }
  template <typename TKey>
  Filter operator[](const TKey& key) const {
    if (_variant == true)  // "true" means "allow recursively"
      return *this;
    else
      return Filter(_variant[key] | _variant["*"]);
  }
 private:
  VariantConstRef _variant;
};
struct AllowAllFilter {
  bool allow() const {
    return true;
  }
  bool allowArray() const {
    return true;
  }
  bool allowObject() const {
    return true;
  }
  bool allowValue() const {
    return true;
  }
  template <typename TKey>
  AllowAllFilter operator[](const TKey&) const {
    return AllowAllFilter();
  }
};
class NestingLimit {
 public:
  NestingLimit() : _value(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}
  explicit NestingLimit(uint8_t n) : _value(n) {}
  NestingLimit decrement() const {
    ARDUINOJSON_ASSERT(_value > 0);
    return NestingLimit(static_cast<uint8_t>(_value - 1));
  }
  bool reached() const {
    return _value == 0;
  }
 private:
  uint8_t _value;
};
template <typename TSource, typename Enable = void>
struct Reader {
 public:
  Reader(TSource& source) : _source(&source) {}
  int read() {
    return _source->read();
  }
  size_t readBytes(char* buffer, size_t length) {
    return _source->readBytes(buffer, length);
  }
 private:
  TSource* _source;
};
template <typename TSource, typename Enable = void>
struct BoundedReader {
};
template <typename TIterator>
class IteratorReader {
  TIterator _ptr, _end;
 public:
  explicit IteratorReader(TIterator begin, TIterator end)
      : _ptr(begin), _end(end) {}
  int read() {
    if (_ptr < _end)
      return static_cast<unsigned char>(*_ptr++);
    else
      return -1;
  }
  size_t readBytes(char* buffer, size_t length) {
    size_t i = 0;
    while (i < length && _ptr < _end) buffer[i++] = *_ptr++;
    return i;
  }
};
template <typename T>
struct void_ {
  typedef void type;
};
template <typename TSource>
struct Reader<TSource, typename void_<typename TSource::const_iterator>::type>
    : IteratorReader<typename TSource::const_iterator> {
  explicit Reader(const TSource& source)
      : IteratorReader<typename TSource::const_iterator>(source.begin(),
                                                         source.end()) {}
};
template <typename T>
struct IsCharOrVoid {
  static const bool value =
      is_same<T, void>::value || is_same<T, char>::value ||
      is_same<T, unsigned char>::value || is_same<T, signed char>::value;
};
template <typename T>
struct IsCharOrVoid<const T> : IsCharOrVoid<T> {};
template <typename TSource>
struct Reader<TSource*,
              typename enable_if<IsCharOrVoid<TSource>::value>::type> {
  const char* _ptr;
 public:
  explicit Reader(const void* ptr)
      : _ptr(ptr ? reinterpret_cast<const char*>(ptr) : "") {}
  int read() {
    return static_cast<unsigned char>(*_ptr++);
  }
  size_t readBytes(char* buffer, size_t length) {
    for (size_t i = 0; i < length; i++) buffer[i] = *_ptr++;
    return length;
  }
};
template <typename TSource>
struct BoundedReader<TSource*,
                     typename enable_if<IsCharOrVoid<TSource>::value>::type>
    : public IteratorReader<const char*> {
 public:
  explicit BoundedReader(const void* ptr, size_t len)
      : IteratorReader<const char*>(reinterpret_cast<const char*>(ptr),
                                    reinterpret_cast<const char*>(ptr) + len) {}
};
template <typename TArray>
struct Reader<ElementProxy<TArray>, void> : Reader<char*, void> {
  explicit Reader(const ElementProxy<TArray>& x)
      : Reader<char*, void>(x.template as<const char*>()) {}
};
template <typename TObject, typename TStringRef>
struct Reader<MemberProxy<TObject, TStringRef>, void> : Reader<char*, void> {
  explicit Reader(const MemberProxy<TObject, TStringRef>& x)
      : Reader<char*, void>(x.template as<const char*>()) {}
};
template <>
struct Reader<VariantRef, void> : Reader<char*, void> {
  explicit Reader(VariantRef x) : Reader<char*, void>(x.as<const char*>()) {}
};
template <>
struct Reader<VariantConstRef, void> : Reader<char*, void> {
  explicit Reader(VariantConstRef x)
      : Reader<char*, void>(x.as<const char*>()) {}
};
}  // namespace ARDUINOJSON_NAMESPACE
#if ARDUINOJSON_ENABLE_ARDUINO_STREAM
namespace ARDUINOJSON_NAMESPACE {
template <typename TSource>
struct Reader<TSource,
              typename enable_if<is_base_of<Stream, TSource>::value>::type> {
 public:
  explicit Reader(Stream& stream) : _stream(&stream) {}
  int read() {
    char c;
    return _stream->readBytes(&c, 1) ? static_cast<unsigned char>(c) : -1;
  }
  size_t readBytes(char* buffer, size_t length) {
    return _stream->readBytes(buffer, length);
  }
 private:
  Stream* _stream;
};
}  // namespace ARDUINOJSON_NAMESPACE
#endif
#if ARDUINOJSON_ENABLE_ARDUINO_STRING
namespace ARDUINOJSON_NAMESPACE {
template <typename TSource>
struct Reader<TSource,
              typename enable_if<is_base_of< ::String, TSource>::value>::type>
    : BoundedReader<const char*> {
  explicit Reader(const ::String& s)
      : BoundedReader<const char*>(s.c_str(), s.length()) {}
};
}  // namespace ARDUINOJSON_NAMESPACE
#endif
#if ARDUINOJSON_ENABLE_PROGMEM
namespace ARDUINOJSON_NAMESPACE {
template <>
struct Reader<const __FlashStringHelper*, void> {
  const char* _ptr;
 public:
  explicit Reader(const __FlashStringHelper* ptr)
      : _ptr(reinterpret_cast<const char*>(ptr)) {}
  int read() {
    return pgm_read_byte(_ptr++);
  }
  size_t readBytes(char* buffer, size_t length) {
    memcpy_P(buffer, _ptr, length);
    _ptr += length;
    return length;
  }
};
template <>
struct BoundedReader<const __FlashStringHelper*, void> {
  const char* _ptr;
  const char* _end;
 public:
  explicit BoundedReader(const __FlashStringHelper* ptr, size_t size)
      : _ptr(reinterpret_cast<const char*>(ptr)), _end(_ptr + size) {}
  int read() {
    if (_ptr < _end)
      return pgm_read_byte(_ptr++);
    else
      return -1;
  }
  size_t readBytes(char* buffer, size_t length) {
    size_t available = static_cast<size_t>(_end - _ptr);
    if (available < length)
      length = available;
    memcpy_P(buffer, _ptr, length);
    _ptr += length;
    return length;
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
#endif
#if ARDUINOJSON_ENABLE_STD_STREAM
#include <istream>
namespace ARDUINOJSON_NAMESPACE {
template <typename TSource>
struct Reader<TSource, typename enable_if<
                           is_base_of<std::istream, TSource>::value>::type> {
 public:
  explicit Reader(std::istream& stream) : _stream(&stream) {}
  int read() {
    return _stream->get();
  }
  size_t readBytes(char* buffer, size_t length) {
    _stream->read(buffer, static_cast<std::streamsize>(length));
    return static_cast<size_t>(_stream->gcount());
  }
 private:
  std::istream* _stream;
};
}  // namespace ARDUINOJSON_NAMESPACE
#endif
namespace ARDUINOJSON_NAMESPACE {
class StringCopier {
 public:
  StringCopier(MemoryPool& pool) : _pool(&pool) {}
  void startString() {
    _pool->getFreeZone(&_ptr, &_capacity);
    _size = 0;
  }
  const char* save() {
    ARDUINOJSON_ASSERT(_ptr);
    return _pool->saveStringFromFreeZone(_size);
  }
  void append(const char* s) {
    while (*s) append(*s++);
  }
  void append(const char* s, size_t n) {
    while (n-- > 0) append(*s++);
  }
  void append(char c) {
    if (!_ptr)
      return;
    if (_size >= _capacity) {
      _ptr = 0;
      _pool->markAsOverflowed();
      return;
    }
    _ptr[_size++] = c;
  }
  bool isValid() {
    return _ptr != 0;
  }
  const char* c_str() {
    return _ptr;
  }
  typedef storage_policies::store_by_copy storage_policy;
 private:
  MemoryPool* _pool;
  char* _ptr;
  size_t _size;
  size_t _capacity;
};
class StringMover {
 public:
  StringMover(char* ptr) : _writePtr(ptr) {}
  void startString() {
    _startPtr = _writePtr;
  }
  const char* save() const {
    return _startPtr;
  }
  void append(char c) {
    *_writePtr++ = c;
  }
  bool isValid() const {
    return true;
  }
  const char* c_str() const {
    return _startPtr;
  }
  typedef storage_policies::store_by_address storage_policy;
 private:
  char* _writePtr;
  char* _startPtr;
};
template <typename TInput>
StringCopier makeStringStorage(TInput&, MemoryPool& pool) {
  return StringCopier(pool);
}
template <typename TChar>
StringMover makeStringStorage(
    TChar* input, MemoryPool&,
    typename enable_if<!is_const<TChar>::value>::type* = 0) {
  return StringMover(reinterpret_cast<char*>(input));
}
template <template <typename, typename> class TDeserializer, typename TReader,
          typename TWriter>
TDeserializer<TReader, TWriter> makeDeserializer(MemoryPool &pool,
                                                 TReader reader,
                                                 TWriter writer) {
  return TDeserializer<TReader, TWriter>(pool, reader, writer);
}
template <template <typename, typename> class TDeserializer, typename TString,
          typename TFilter>
typename enable_if<!is_array<TString>::value, DeserializationError>::type
deserialize(JsonDocument &doc, const TString &input, NestingLimit nestingLimit,
            TFilter filter) {
  Reader<TString> reader(input);
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), reader,
             makeStringStorage(input, doc.memoryPool()))
      .parse(doc.data(), filter, nestingLimit);
}
template <template <typename, typename> class TDeserializer, typename TChar,
          typename TFilter>
DeserializationError deserialize(JsonDocument &doc, TChar *input,
                                 size_t inputSize, NestingLimit nestingLimit,
                                 TFilter filter) {
  BoundedReader<TChar *> reader(input, inputSize);
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), reader,
             makeStringStorage(input, doc.memoryPool()))
      .parse(doc.data(), filter, nestingLimit);
}
template <template <typename, typename> class TDeserializer, typename TStream,
          typename TFilter>
DeserializationError deserialize(JsonDocument &doc, TStream &input,
                                 NestingLimit nestingLimit, TFilter filter) {
  Reader<TStream> reader(input);
  doc.clear();
  return makeDeserializer<TDeserializer>(
             doc.memoryPool(), reader,
             makeStringStorage(input, doc.memoryPool()))
      .parse(doc.data(), filter, nestingLimit);
}
class EscapeSequence {
 public:
  static char escapeChar(char c) {
    const char *p = escapeTable(true);
    while (p[0] && p[1] != c) {
      p += 2;
    }
    return p[0];
  }
  static char unescapeChar(char c) {
    const char *p = escapeTable(false);
    for (;;) {
      if (p[0] == '\0')
        return 0;
      if (p[0] == c)
        return p[1];
      p += 2;
    }
  }
 private:
  static const char *escapeTable(bool excludeSolidus) {
    return &"//\"\"\\\\b\bf\fn\nr\rt\t"[excludeSolidus ? 2 : 0];
  }
};
template <typename TReader>
class Latch {
 public:
  Latch(TReader reader) : _reader(reader), _loaded(false) {
#if ARDUINOJSON_DEBUG
    _ended = false;
#endif
  }
  void clear() {
    _loaded = false;
  }
  int last() const {
    return _current;
  }
  FORCE_INLINE char current() {
    if (!_loaded) {
      load();
    }
    return _current;
  }
 private:
  void load() {
    ARDUINOJSON_ASSERT(!_ended);
    int c = _reader.read();
#if ARDUINOJSON_DEBUG
    if (c <= 0)
      _ended = true;
#endif
    _current = static_cast<char>(c > 0 ? c : 0);
    _loaded = true;
  }
  TReader _reader;
  char _current;
  bool _loaded;
#if ARDUINOJSON_DEBUG
  bool _ended;
#endif
};
}  // namespace ARDUINOJSON_NAMESPACE
#if defined(__GNUC__)
#if __GNUC__ >= 7
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
#endif
namespace ARDUINOJSON_NAMESPACE {
namespace Utf16 {
inline bool isHighSurrogate(uint16_t codeunit) {
  return codeunit >= 0xD800 && codeunit < 0xDC00;
}
inline bool isLowSurrogate(uint16_t codeunit) {
  return codeunit >= 0xDC00 && codeunit < 0xE000;
}
class Codepoint {
 public:
  Codepoint() : _highSurrogate(0) {}
  bool append(uint16_t codeunit) {
    if (isHighSurrogate(codeunit)) {
      _highSurrogate = codeunit & 0x3FF;
      return false;
    }
    if (isLowSurrogate(codeunit)) {
      _codepoint =
          uint32_t(0x10000 + ((_highSurrogate << 10) | (codeunit & 0x3FF)));
      return true;
    }
    _codepoint = codeunit;
    return true;
  }
  uint32_t value() const {
    return _codepoint;
  }
 private:
  uint16_t _highSurrogate;
  uint32_t _codepoint;
};
}  // namespace Utf16
}  // namespace ARDUINOJSON_NAMESPACE
#if defined(__GNUC__)
#if __GNUC__ >= 8
#pragma GCC diagnostic pop
#endif
#endif
namespace ARDUINOJSON_NAMESPACE {
namespace Utf8 {
template <typename TStringBuilder>
inline void encodeCodepoint(uint32_t codepoint32, TStringBuilder& str) {
  char buf[5];
  char* p = buf;
  *(p++) = 0;
  if (codepoint32 < 0x80) {
    *(p++) = char((codepoint32));
  } else {
    *(p++) = char((codepoint32 | 0x80) & 0xBF);
    uint16_t codepoint16 = uint16_t(codepoint32 >> 6);
    if (codepoint16 < 0x20) {  // 0x800
      *(p++) = char(codepoint16 | 0xC0);
    } else {
      *(p++) = char((codepoint16 | 0x80) & 0xBF);
      codepoint16 = uint16_t(codepoint16 >> 6);
      if (codepoint16 < 0x10) {  // 0x10000
        *(p++) = char(codepoint16 | 0xE0);
      } else {
        *(p++) = char((codepoint16 | 0x80) & 0xBF);
        codepoint16 = uint16_t(codepoint16 >> 6);
        *(p++) = char(codepoint16 | 0xF0);
      }
    }
  }
  while (*(--p)) {
    str.append(*p);
  }
}
}  // namespace Utf8
template <typename TReader, typename TStringStorage>
class JsonDeserializer {
 public:
  JsonDeserializer(MemoryPool &pool, TReader reader,
                   TStringStorage stringStorage)
      : _stringStorage(stringStorage),
        _foundSomething(false),
        _latch(reader),
        _pool(&pool),
        _error(DeserializationError::Ok) {}
  template <typename TFilter>
  DeserializationError parse(VariantData &variant, TFilter filter,
                             NestingLimit nestingLimit) {
    parseVariant(variant, filter, nestingLimit);
    if (!_error && _latch.last() != 0 && !variant.isEnclosed()) {
      return DeserializationError::InvalidInput;
    }
    return _error;
  }
 private:
  char current() {
    return _latch.current();
  }
  void move() {
    _latch.clear();
  }
  bool eat(char charToSkip) {
    if (current() != charToSkip)
      return false;
    move();
    return true;
  }
  template <typename TFilter>
  bool parseVariant(VariantData &variant, TFilter filter,
                    NestingLimit nestingLimit) {
    if (!skipSpacesAndComments())
      return false;
    switch (current()) {
      case '[':
        if (filter.allowArray())
          return parseArray(variant.toArray(), filter, nestingLimit);
        else
          return skipArray(nestingLimit);
      case '{':
        if (filter.allowObject())
          return parseObject(variant.toObject(), filter, nestingLimit);
        else
          return skipObject(nestingLimit);
      case '\"':
      case '\'':
        if (filter.allowValue())
          return parseStringValue(variant);
        else
          return skipString();
      default:
        if (filter.allowValue())
          return parseNumericValue(variant);
        else
          return skipNumericValue();
    }
  }
  bool skipVariant(NestingLimit nestingLimit) {
    if (!skipSpacesAndComments())
      return false;
    switch (current()) {
      case '[':
        return skipArray(nestingLimit);
      case '{':
        return skipObject(nestingLimit);
      case '\"':
      case '\'':
        return skipString();
      default:
        return skipNumericValue();
    }
  }
  template <typename TFilter>
  bool parseArray(CollectionData &array, TFilter filter,
                  NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }
    ARDUINOJSON_ASSERT(current() == '[');
    move();
    if (!skipSpacesAndComments())
      return false;
    if (eat(']'))
      return true;
    TFilter memberFilter = filter[0UL];
    for (;;) {
      if (memberFilter.allow()) {
        VariantData *value = array.addElement(_pool);
        if (!value) {
          _error = DeserializationError::NoMemory;
          return false;
        }
        if (!parseVariant(*value, memberFilter, nestingLimit.decrement()))
          return false;
      } else {
        if (!skipVariant(nestingLimit.decrement()))
          return false;
      }
      if (!skipSpacesAndComments())
        return false;
      if (eat(']'))
        return true;
      if (!eat(',')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
    }
  }
  bool skipArray(NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }
    ARDUINOJSON_ASSERT(current() == '[');
    move();
    for (;;) {
      if (!skipVariant(nestingLimit.decrement()))
        return false;
      if (!skipSpacesAndComments())
        return false;
      if (eat(']'))
        return true;
      if (!eat(',')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
    }
  }
  template <typename TFilter>
  bool parseObject(CollectionData &object, TFilter filter,
                   NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }
    ARDUINOJSON_ASSERT(current() == '{');
    move();
    if (!skipSpacesAndComments())
      return false;
    if (eat('}'))
      return true;
    for (;;) {
      if (!parseKey())
        return false;
      if (!skipSpacesAndComments())
        return false;
      if (!eat(':')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
      const char *key = _stringStorage.c_str();
      TFilter memberFilter = filter[key];
      if (memberFilter.allow()) {
        VariantData *variant = object.getMember(adaptString(key));
        if (!variant) {
          key = _stringStorage.save();
          VariantSlot *slot = object.addSlot(_pool);
          if (!slot) {
            _error = DeserializationError::NoMemory;
            return false;
          }
          slot->setKey(key, typename TStringStorage::storage_policy());
          variant = slot->data();
        }
        if (!parseVariant(*variant, memberFilter, nestingLimit.decrement()))
          return false;
      } else {
        if (!skipVariant(nestingLimit.decrement()))
          return false;
      }
      if (!skipSpacesAndComments())
        return false;
      if (eat('}'))
        return true;
      if (!eat(',')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
      if (!skipSpacesAndComments())
        return false;
    }
  }
  bool skipObject(NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }
    ARDUINOJSON_ASSERT(current() == '{');
    move();
    if (!skipSpacesAndComments())
      return false;
    if (eat('}'))
      return true;
    for (;;) {
      if (!skipVariant(nestingLimit.decrement()))
        return false;
      if (!skipSpacesAndComments())
        return false;
      if (!eat(':')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
      if (!skipVariant(nestingLimit.decrement()))
        return false;
      if (!skipSpacesAndComments())
        return false;
      if (eat('}'))
        return true;
      if (!eat(',')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
    }
  }
  bool parseKey() {
    _stringStorage.startString();
    if (isQuote(current())) {
      return parseQuotedString();
    } else {
      return parseNonQuotedString();
    }
  }
  bool parseStringValue(VariantData &variant) {
    _stringStorage.startString();
    if (!parseQuotedString())
      return false;
    const char *value = _stringStorage.save();
    variant.setStringPointer(value, typename TStringStorage::storage_policy());
    return true;
  }
  bool parseQuotedString() {
#if ARDUINOJSON_DECODE_UNICODE
    Utf16::Codepoint codepoint;
#endif
    const char stopChar = current();
    move();
    for (;;) {
      char c = current();
      move();
      if (c == stopChar)
        break;
      if (c == '\0') {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      if (c == '\\') {
        c = current();
        if (c == '\0') {
          _error = DeserializationError::IncompleteInput;
          return false;
        }
        if (c == 'u') {
#if ARDUINOJSON_DECODE_UNICODE
          move();
          uint16_t codeunit;
          if (!parseHex4(codeunit))
            return false;
          if (codepoint.append(codeunit))
            Utf8::encodeCodepoint(codepoint.value(), _stringStorage);
          continue;
#else
          _error = DeserializationError::NotSupported;
          return false;
#endif
        }
        c = EscapeSequence::unescapeChar(c);
        if (c == '\0') {
          _error = DeserializationError::InvalidInput;
          return false;
        }
        move();
      }
      _stringStorage.append(c);
    }
    _stringStorage.append('\0');
    if (!_stringStorage.isValid()) {
      _error = DeserializationError::NoMemory;
      return false;
    }
    return true;
  }
  bool parseNonQuotedString() {
    char c = current();
    ARDUINOJSON_ASSERT(c);
    if (canBeInNonQuotedString(c)) {  // no quotes
      do {
        move();
        _stringStorage.append(c);
        c = current();
      } while (canBeInNonQuotedString(c));
    } else {
      _error = DeserializationError::InvalidInput;
      return false;
    }
    _stringStorage.append('\0');
    if (!_stringStorage.isValid()) {
      _error = DeserializationError::NoMemory;
      return false;
    }
    return true;
  }
  bool skipString() {
    const char stopChar = current();
    move();
    for (;;) {
      char c = current();
      move();
      if (c == stopChar)
        break;
      if (c == '\0') {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      if (c == '\\') {
        if (current() != '\0')
          move();
      }
    }
    return true;
  }
  bool parseNumericValue(VariantData &result) {
    uint8_t n = 0;
    char c = current();
    while (canBeInNonQuotedString(c) && n < 63) {
      move();
      _buffer[n++] = c;
      c = current();
    }
    _buffer[n] = 0;
    c = _buffer[0];
    if (c == 't') {  // true
      result.setBoolean(true);
      if (n != 4) {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      return true;
    }
    if (c == 'f') {  // false
      result.setBoolean(false);
      if (n != 5) {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      return true;
    }
    if (c == 'n') {  // null
      if (n != 4) {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      return true;
    }
    if (!parseNumber(_buffer, result)) {
      _error = DeserializationError::InvalidInput;
      return false;
    }
    return true;
  }
  bool skipNumericValue() {
    char c = current();
    while (canBeInNonQuotedString(c)) {
      move();
      c = current();
    }
    return true;
  }
  bool parseHex4(uint16_t &result) {
    result = 0;
    for (uint8_t i = 0; i < 4; ++i) {
      char digit = current();
      if (!digit) {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      uint8_t value = decodeHex(digit);
      if (value > 0x0F) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
      result = uint16_t((result << 4) | value);
      move();
    }
    return true;
  }
  static inline bool isBetween(char c, char min, char max) {
    return min <= c && c <= max;
  }
  static inline bool canBeInNonQuotedString(char c) {
    return isBetween(c, '0', '9') || isBetween(c, '_', 'z') ||
           isBetween(c, 'A', 'Z') || c == '+' || c == '-' || c == '.';
  }
  static inline bool isQuote(char c) {
    return c == '\'' || c == '\"';
  }
  static inline uint8_t decodeHex(char c) {
    if (c < 'A')
      return uint8_t(c - '0');
    c = char(c & ~0x20);  // uppercase
    return uint8_t(c - 'A' + 10);
  }
  bool skipSpacesAndComments() {
    for (;;) {
      switch (current()) {
        case '\0':
          _error = _foundSomething ? DeserializationError::IncompleteInput
                                   : DeserializationError::EmptyInput;
          return false;
        case ' ':
        case '\t':
        case '\r':
        case '\n':
          move();
          continue;
#if ARDUINOJSON_ENABLE_COMMENTS
        case '/':
          move();  // skip '/'
          switch (current()) {
            case '*': {
              move();  // skip '*'
              bool wasStar = false;
              for (;;) {
                char c = current();
                if (c == '\0') {
                  _error = DeserializationError::IncompleteInput;
                  return false;
                }
                if (c == '/' && wasStar) {
                  move();
                  break;
                }
                wasStar = c == '*';
                move();
              }
              break;
            }
            case '/':
              for (;;) {
                move();
                char c = current();
                if (c == '\0') {
                  _error = DeserializationError::IncompleteInput;
                  return false;
                }
                if (c == '\n')
                  break;
              }
              break;
            default:
              _error = DeserializationError::InvalidInput;
              return false;
          }
          break;
#endif
        default:
          _foundSomething = true;
          return true;
      }
    }
  }
  TStringStorage _stringStorage;
  bool _foundSomething;
  Latch<TReader> _latch;
  MemoryPool *_pool;
  char _buffer[64];  // using a member instead of a local variable because it
  DeserializationError _error;
};
template <typename TString>
DeserializationError deserializeJson(
    JsonDocument &doc, const TString &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit,
                                       AllowAllFilter());
}
template <typename TString>
DeserializationError deserializeJson(
    JsonDocument &doc, const TString &input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TString>
DeserializationError deserializeJson(JsonDocument &doc, const TString &input,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TStream>
DeserializationError deserializeJson(
    JsonDocument &doc, TStream &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit,
                                       AllowAllFilter());
}
template <typename TStream>
DeserializationError deserializeJson(
    JsonDocument &doc, TStream &input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TStream>
DeserializationError deserializeJson(JsonDocument &doc, TStream &input,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit,
                                       AllowAllFilter());
}
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TChar>
DeserializationError deserializeJson(JsonDocument &doc, TChar *input,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, size_t inputSize,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, inputSize, nestingLimit,
                                       AllowAllFilter());
}
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, size_t inputSize, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, inputSize, nestingLimit,
                                       filter);
}
template <typename TChar>
DeserializationError deserializeJson(JsonDocument &doc, TChar *input,
                                     size_t inputSize,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, inputSize, nestingLimit,
                                       filter);
}
template <typename TFloat>
struct FloatParts {
  uint32_t integral;
  uint32_t decimal;
  int16_t exponent;
  int8_t decimalPlaces;
  FloatParts(TFloat value) {
    uint32_t maxDecimalPart = sizeof(TFloat) >= 8 ? 1000000000 : 1000000;
    decimalPlaces = sizeof(TFloat) >= 8 ? 9 : 6;
    exponent = normalize(value);
    integral = uint32_t(value);
    for (uint32_t tmp = integral; tmp >= 10; tmp /= 10) {
      maxDecimalPart /= 10;
      decimalPlaces--;
    }
    TFloat remainder = (value - TFloat(integral)) * TFloat(maxDecimalPart);
    decimal = uint32_t(remainder);
    remainder = remainder - TFloat(decimal);
    decimal += uint32_t(remainder * 2);
    if (decimal >= maxDecimalPart) {
      decimal = 0;
      integral++;
      if (exponent && integral >= 10) {
        exponent++;
        integral = 1;
      }
    }
    while (decimal % 10 == 0 && decimalPlaces > 0) {
      decimal /= 10;
      decimalPlaces--;
    }
  }
  static int16_t normalize(TFloat& value) {
    typedef FloatTraits<TFloat> traits;
    int16_t powersOf10 = 0;
    int8_t index = sizeof(TFloat) == 8 ? 8 : 5;
    int bit = 1 << index;
    if (value >= ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD) {
      for (; index >= 0; index--) {
        if (value >= traits::positiveBinaryPowerOfTen(index)) {
          value *= traits::negativeBinaryPowerOfTen(index);
          powersOf10 = int16_t(powersOf10 + bit);
        }
        bit >>= 1;
      }
    }
    if (value > 0 && value <= ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD) {
      for (; index >= 0; index--) {
        if (value < traits::negativeBinaryPowerOfTenPlusOne(index)) {
          value *= traits::positiveBinaryPowerOfTen(index);
          powersOf10 = int16_t(powersOf10 - bit);
        }
        bit >>= 1;
      }
    }
    return powersOf10;
  }
};
template <typename TWriter>
class CountingDecorator {
 public:
  explicit CountingDecorator(TWriter& writer) : _writer(writer), _count(0) {}
  void write(uint8_t c) {
    _count += _writer.write(c);
  }
  void write(const uint8_t* s, size_t n) {
    _count += _writer.write(s, n);
  }
  size_t count() const {
    return _count;
  }
 private:
  TWriter _writer;
  size_t _count;
};
template <typename TWriter>
class TextFormatter {
 public:
  explicit TextFormatter(TWriter writer) : _writer(writer) {}
  size_t bytesWritten() const {
    return _writer.count();
  }
  void writeBoolean(bool value) {
    if (value)
      writeRaw("true");
    else
      writeRaw("false");
  }
  void writeString(const char *value) {
    ARDUINOJSON_ASSERT(value != NULL);
    writeRaw('\"');
    while (*value) writeChar(*value++);
    writeRaw('\"');
  }
  void writeChar(char c) {
    char specialChar = EscapeSequence::escapeChar(c);
    if (specialChar) {
      writeRaw('\\');
      writeRaw(specialChar);
    } else {
      writeRaw(c);
    }
  }
  template <typename T>
  void writeFloat(T value) {
    if (isnan(value))
      return writeRaw(ARDUINOJSON_ENABLE_NAN ? "NaN" : "null");
#if ARDUINOJSON_ENABLE_INFINITY
    if (value < 0.0) {
      writeRaw('-');
      value = -value;
    }
    if (isinf(value))
      return writeRaw("Infinity");
#else
    if (isinf(value))
      return writeRaw("null");
    if (value < 0.0) {
      writeRaw('-');
      value = -value;
    }
#endif
    FloatParts<T> parts(value);
    writePositiveInteger(parts.integral);
    if (parts.decimalPlaces)
      writeDecimals(parts.decimal, parts.decimalPlaces);
    if (parts.exponent < 0) {
      writeRaw("e-");
      writePositiveInteger(-parts.exponent);
    }
    if (parts.exponent > 0) {
      writeRaw('e');
      writePositiveInteger(parts.exponent);
    }
  }
  void writeNegativeInteger(UInt value) {
    writeRaw('-');
    writePositiveInteger(value);
  }
  template <typename T>
  void writePositiveInteger(T value) {
    char buffer[22];
    char *end = buffer + sizeof(buffer);
    char *begin = end;
    do {
      *--begin = char(value % 10 + '0');
      value = T(value / 10);
    } while (value);
    writeRaw(begin, end);
  }
  void writeDecimals(uint32_t value, int8_t width) {
    char buffer[16];
    char *end = buffer + sizeof(buffer);
    char *begin = end;
    while (width--) {
      *--begin = char(value % 10 + '0');
      value /= 10;
    }
    *--begin = '.';
    writeRaw(begin, end);
  }
  void writeRaw(const char *s) {
    _writer.write(reinterpret_cast<const uint8_t *>(s), strlen(s));
  }
  void writeRaw(const char *s, size_t n) {
    _writer.write(reinterpret_cast<const uint8_t *>(s), n);
  }
  void writeRaw(const char *begin, const char *end) {
    _writer.write(reinterpret_cast<const uint8_t *>(begin),
                  static_cast<size_t>(end - begin));
  }
  template <size_t N>
  void writeRaw(const char (&s)[N]) {
    _writer.write(reinterpret_cast<const uint8_t *>(s), N - 1);
  }
  void writeRaw(char c) {
    _writer.write(static_cast<uint8_t>(c));
  }
 protected:
  CountingDecorator<TWriter> _writer;
  size_t _length;
 private:
  TextFormatter &operator=(const TextFormatter &);  // cannot be assigned
};
class DummyWriter {
 public:
  size_t write(uint8_t) {
    return 1;
  }
  size_t write(const uint8_t*, size_t n) {
    return n;
  }
};
template <template <typename> class TSerializer, typename TSource>
size_t measure(const TSource &source) {
  DummyWriter dp;
  TSerializer<DummyWriter> serializer(dp);
  return source.accept(serializer);
}
template <typename TDestination, typename Enable = void>
class Writer {
 public:
  explicit Writer(TDestination& dest) : _dest(&dest) {}
  size_t write(uint8_t c) {
    return _dest->write(c);
  }
  size_t write(const uint8_t* s, size_t n) {
    return _dest->write(s, n);
  }
 private:
  TDestination* _dest;
};
class StaticStringWriter {
 public:
  StaticStringWriter(char *buf, size_t size) : end(buf + size - 1), p(buf) {
    *p = '\0';
  }
  size_t write(uint8_t c) {
    if (p >= end)
      return 0;
    *p++ = static_cast<char>(c);
    *p = '\0';
    return 1;
  }
  size_t write(const uint8_t *s, size_t n) {
    char *begin = p;
    while (p < end && n > 0) {
      *p++ = static_cast<char>(*s++);
      n--;
    }
    *p = '\0';
    return size_t(p - begin);
  }
 private:
  char *end;
  char *p;
};
}  // namespace ARDUINOJSON_NAMESPACE
#if ARDUINOJSON_ENABLE_STD_STRING
namespace ARDUINOJSON_NAMESPACE {
template <class T>
struct is_std_string : false_type {};
template <class TCharTraits, class TAllocator>
struct is_std_string<std::basic_string<char, TCharTraits, TAllocator> >
    : true_type {};
template <typename TDestination>
class Writer<TDestination,
             typename enable_if<is_std_string<TDestination>::value>::type> {
 public:
  Writer(TDestination &str) : _str(&str) {}
  size_t write(uint8_t c) {
    _str->operator+=(static_cast<char>(c));
    return 1;
  }
  size_t write(const uint8_t *s, size_t n) {
    _str->append(reinterpret_cast<const char *>(s), n);
    return n;
  }
 private:
  TDestination *_str;
};
}  // namespace ARDUINOJSON_NAMESPACE
#endif
#if ARDUINOJSON_ENABLE_ARDUINO_STRING
namespace ARDUINOJSON_NAMESPACE {
template <>
class Writer< ::String, void> {
  static const size_t bufferCapacity = ARDUINOJSON_STRING_BUFFER_SIZE;
 public:
  explicit Writer(::String &str) : _destination(&str) {
    _size = 0;
  }
  ~Writer() {
    flush();
  }
  size_t write(uint8_t c) {
    ARDUINOJSON_ASSERT(_size < bufferCapacity);
    _buffer[_size++] = static_cast<char>(c);
    if (_size + 1 >= bufferCapacity)
      flush();
    return 1;
  }
  size_t write(const uint8_t *s, size_t n) {
    for (size_t i = 0; i < n; i++) {
      write(s[i]);
    }
    return n;
  }
 private:
  void flush() {
    ARDUINOJSON_ASSERT(_size < bufferCapacity);
    _buffer[_size] = 0;
    *_destination += _buffer;
    _size = 0;
  }
  ::String *_destination;
  char _buffer[bufferCapacity];
  size_t _size;
};
}  // namespace ARDUINOJSON_NAMESPACE
#endif
#if ARDUINOJSON_ENABLE_STD_STREAM
namespace ARDUINOJSON_NAMESPACE {
template <typename TDestination>
class Writer<
    TDestination,
    typename enable_if<is_base_of<std::ostream, TDestination>::value>::type> {
 public:
  explicit Writer(std::ostream& os) : _os(&os) {}
  size_t write(uint8_t c) {
    _os->put(static_cast<char>(c));
    return 1;
  }
  size_t write(const uint8_t* s, size_t n) {
    _os->write(reinterpret_cast<const char*>(s),
               static_cast<std::streamsize>(n));
    return n;
  }
 private:
  std::ostream* _os;
};
}  // namespace ARDUINOJSON_NAMESPACE
#endif
#if ARDUINOJSON_ENABLE_ARDUINO_PRINT
namespace ARDUINOJSON_NAMESPACE {
template <typename TDestination>
class Writer<
    TDestination,
    typename enable_if<is_base_of< ::Print, TDestination>::value>::type> {
 public:
  explicit Writer(::Print& print) : _print(&print) {}
  size_t write(uint8_t c) {
    return _print->write(c);
  }
  size_t write(const uint8_t* s, size_t n) {
    return _print->write(s, n);
  }
 private:
  ::Print* _print;
};
}  // namespace ARDUINOJSON_NAMESPACE
#endif
namespace ARDUINOJSON_NAMESPACE {
template <template <typename> class TSerializer, typename TSource,
          typename TWriter>
size_t doSerialize(const TSource &source, TWriter writer) {
  TSerializer<TWriter> serializer(writer);
  return source.accept(serializer);
}
template <template <typename> class TSerializer, typename TSource,
          typename TDestination>
size_t serialize(const TSource &source, TDestination &destination) {
  Writer<TDestination> writer(destination);
  return doSerialize<TSerializer>(source, writer);
}
template <template <typename> class TSerializer, typename TSource>
size_t serialize(const TSource &source, void *buffer, size_t bufferSize) {
  StaticStringWriter writer(reinterpret_cast<char *>(buffer), bufferSize);
  return doSerialize<TSerializer>(source, writer);
}
template <template <typename> class TSerializer, typename TSource,
          typename TChar, size_t N>
#if defined _MSC_VER && _MSC_VER < 1900
typename enable_if<sizeof(remove_reference<TChar>::type) == 1, size_t>::type
#else
typename enable_if<sizeof(TChar) == 1, size_t>::type
#endif
serialize(const TSource &source, TChar (&buffer)[N]) {
  StaticStringWriter writer(reinterpret_cast<char *>(buffer), N);
  return doSerialize<TSerializer>(source, writer);
}
template <typename TWriter>
class JsonSerializer : public Visitor<size_t> {
 public:
  JsonSerializer(TWriter writer) : _formatter(writer) {}
  FORCE_INLINE size_t visitArray(const CollectionData &array) {
    write('[');
    VariantSlot *slot = array.head();
    while (slot != 0) {
      slot->data()->accept(*this);
      slot = slot->next();
      if (slot == 0)
        break;
      write(',');
    }
    write(']');
    return bytesWritten();
  }
  size_t visitObject(const CollectionData &object) {
    write('{');
    VariantSlot *slot = object.head();
    while (slot != 0) {
      _formatter.writeString(slot->key());
      write(':');
      slot->data()->accept(*this);
      slot = slot->next();
      if (slot == 0)
        break;
      write(',');
    }
    write('}');
    return bytesWritten();
  }
  size_t visitFloat(Float value) {
    _formatter.writeFloat(value);
    return bytesWritten();
  }
  size_t visitString(const char *value) {
    _formatter.writeString(value);
    return bytesWritten();
  }
  size_t visitRawJson(const char *data, size_t n) {
    _formatter.writeRaw(data, n);
    return bytesWritten();
  }
  size_t visitNegativeInteger(UInt value) {
    _formatter.writeNegativeInteger(value);
    return bytesWritten();
  }
  size_t visitPositiveInteger(UInt value) {
    _formatter.writePositiveInteger(value);
    return bytesWritten();
  }
  size_t visitBoolean(bool value) {
    _formatter.writeBoolean(value);
    return bytesWritten();
  }
  size_t visitNull() {
    _formatter.writeRaw("null");
    return bytesWritten();
  }
 protected:
  size_t bytesWritten() const {
    return _formatter.bytesWritten();
  }
  void write(char c) {
    _formatter.writeRaw(c);
  }
  void write(const char *s) {
    _formatter.writeRaw(s);
  }
 private:
  TextFormatter<TWriter> _formatter;
};
template <typename TSource, typename TDestination>
size_t serializeJson(const TSource &source, TDestination &destination) {
  return serialize<JsonSerializer>(source, destination);
}
template <typename TSource>
size_t serializeJson(const TSource &source, void *buffer, size_t bufferSize) {
  return serialize<JsonSerializer>(source, buffer, bufferSize);
}
template <typename TSource>
size_t measureJson(const TSource &source) {
  return measure<JsonSerializer>(source);
}
#if ARDUINOJSON_ENABLE_STD_STREAM
template <typename T>
inline typename enable_if<IsVisitable<T>::value, std::ostream &>::type
operator<<(std::ostream &os, const T &source) {
  serializeJson(source, os);
  return os;
}
#endif
template <typename TWriter>
class PrettyJsonSerializer : public JsonSerializer<TWriter> {
  typedef JsonSerializer<TWriter> base;
 public:
  PrettyJsonSerializer(TWriter &writer) : base(writer), _nesting(0) {}
  size_t visitArray(const CollectionData &array) {
    VariantSlot *slot = array.head();
    if (slot) {
      base::write("[\r\n");
      _nesting++;
      while (slot != 0) {
        indent();
        slot->data()->accept(*this);
        slot = slot->next();
        base::write(slot ? ",\r\n" : "\r\n");
      }
      _nesting--;
      indent();
      base::write("]");
    } else {
      base::write("[]");
    }
    return this->bytesWritten();
  }
  size_t visitObject(const CollectionData &object) {
    VariantSlot *slot = object.head();
    if (slot) {
      base::write("{\r\n");
      _nesting++;
      while (slot != 0) {
        indent();
        base::visitString(slot->key());
        base::write(": ");
        slot->data()->accept(*this);
        slot = slot->next();
        base::write(slot ? ",\r\n" : "\r\n");
      }
      _nesting--;
      indent();
      base::write("}");
    } else {
      base::write("{}");
    }
    return this->bytesWritten();
  }
 private:
  void indent() {
    for (uint8_t i = 0; i < _nesting; i++) base::write(ARDUINOJSON_TAB);
  }
  uint8_t _nesting;
};
template <typename TSource, typename TDestination>
size_t serializeJsonPretty(const TSource &source, TDestination &destination) {
  return serialize<PrettyJsonSerializer>(source, destination);
}
template <typename TSource>
size_t serializeJsonPretty(const TSource &source, void *buffer,
                           size_t bufferSize) {
  return serialize<PrettyJsonSerializer>(source, buffer, bufferSize);
}
template <typename TSource>
size_t measureJsonPretty(const TSource &source) {
  return measure<PrettyJsonSerializer>(source);
}
template <typename T>
inline void swap(T& a, T& b) {
  T t(a);
  a = b;
  b = t;
}
#if ARDUINOJSON_HAS_RVALUE_REFERENCES
template <typename T>
typename remove_reference<T>::type&& move(T&& t) {
  return static_cast<typename remove_reference<T>::type&&>(t);
}
#else
template <typename T>
T& move(T& t) {
  return t;
}
#endif
#if ARDUINOJSON_LITTLE_ENDIAN
inline void fixEndianess(uint8_t *p, integral_constant<size_t, 8>) {
  swap(p[0], p[7]);
  swap(p[1], p[6]);
  swap(p[2], p[5]);
  swap(p[3], p[4]);
}
inline void fixEndianess(uint8_t *p, integral_constant<size_t, 4>) {
  swap(p[0], p[3]);
  swap(p[1], p[2]);
}
inline void fixEndianess(uint8_t *p, integral_constant<size_t, 2>) {
  swap(p[0], p[1]);
}
inline void fixEndianess(uint8_t *, integral_constant<size_t, 1>) {}
template <typename T>
inline void fixEndianess(T &value) {
  fixEndianess(reinterpret_cast<uint8_t *>(&value),
               integral_constant<size_t, sizeof(T)>());
}
#else
template <typename T>
inline void fixEndianess(T &) {}
#endif
inline void doubleToFloat(const uint8_t d[8], uint8_t f[4]) {
  f[0] = uint8_t((d[0] & 0xC0) | (d[0] << 3 & 0x3f) | (d[1] >> 5));
  f[1] = uint8_t((d[1] << 3) | (d[2] >> 5));
  f[2] = uint8_t((d[2] << 3) | (d[3] >> 5));
  f[3] = uint8_t((d[3] << 3) | (d[4] >> 5));
}
template <typename TReader, typename TStringStorage>
class MsgPackDeserializer {
 public:
  MsgPackDeserializer(MemoryPool &pool, TReader reader,
                      TStringStorage stringStorage)
      : _pool(&pool),
        _reader(reader),
        _stringStorage(stringStorage),
        _error(DeserializationError::Ok),
        _foundSomething(false) {}
  template <typename TFilter>
  DeserializationError parse(VariantData &variant, TFilter filter,
                             NestingLimit nestingLimit) {
    parseVariant(variant, filter, nestingLimit);
    return _foundSomething ? _error : DeserializationError::EmptyInput;
  }
 private:
  MsgPackDeserializer &operator=(const MsgPackDeserializer &);
  bool invalidInput() {
    _error = DeserializationError::InvalidInput;
    return false;
  }
  bool notSupported() {
    _error = DeserializationError::NotSupported;
    return false;
  }
  template <typename TFilter>
  bool parseVariant(VariantData &variant, TFilter filter,
                    NestingLimit nestingLimit) {
    uint8_t code = 0;  // TODO: why do we need to initialize this variable?
    if (!readByte(code))
      return false;
    _foundSomething = true;
    bool allowValue = filter.allowValue();
    switch (code) {
      case 0xc0:
        return true;
      case 0xc1:
        return invalidInput();
      case 0xc2:
        if (allowValue)
          variant.setBoolean(false);
        return true;
      case 0xc3:
        if (allowValue)
          variant.setBoolean(true);
        return true;
      case 0xc4:  // bin 8
        if (allowValue)
          return notSupported();
        else
          return skipString<uint8_t>();
      case 0xc5:  // bin 16
        if (allowValue)
          return notSupported();
        else
          return skipString<uint16_t>();
      case 0xc6:  // bin 32
        if (allowValue)
          return notSupported();
        else
          return skipString<uint32_t>();
      case 0xc7:  // ext 8
        if (allowValue)
          return notSupported();
        else
          return skipExt<uint8_t>();
      case 0xc8:  // ext 16
        if (allowValue)
          return notSupported();
        else
          return skipExt<uint16_t>();
      case 0xc9:  // ext 32
        if (allowValue)
          return notSupported();
        else
          return skipExt<uint32_t>();
      case 0xca:
        if (allowValue)
          return readFloat<float>(variant);
        else
          return skipBytes(4);
      case 0xcb:
        if (allowValue)
          return readDouble<double>(variant);
        else
          return skipBytes(8);
      case 0xcc:
        if (allowValue)
          return readInteger<uint8_t>(variant);
        else
          return skipBytes(1);
      case 0xcd:
        if (allowValue)
          return readInteger<uint16_t>(variant);
        else
          return skipBytes(2);
      case 0xce:
        if (allowValue)
          return readInteger<uint32_t>(variant);
        else
          return skipBytes(4);
      case 0xcf:
        if (allowValue)
#if ARDUINOJSON_USE_LONG_LONG
          return readInteger<uint64_t>(variant);
#else
          return notSupported();
#endif
        else
          return skipBytes(8);
      case 0xd0:
        if (allowValue)
          return readInteger<int8_t>(variant);
        else
          return skipBytes(1);
      case 0xd1:
        if (allowValue)
          return readInteger<int16_t>(variant);
        else
          return skipBytes(2);
      case 0xd2:
        if (allowValue)
          return readInteger<int32_t>(variant);
        else
          return skipBytes(4);
      case 0xd3:
        if (allowValue)
#if ARDUINOJSON_USE_LONG_LONG
          return readInteger<int64_t>(variant);
#else
          return notSupported();
#endif
        else
          return skipBytes(8);
      case 0xd4:  // fixext 1
        if (allowValue)
          return notSupported();
        else
          return skipBytes(2);
      case 0xd5:  // fixext 2
        if (allowValue)
          return notSupported();
        else
          return skipBytes(3);
      case 0xd6:  // fixext 4
        if (allowValue)
          return notSupported();
        else
          return skipBytes(5);
      case 0xd7:  // fixext 8
        if (allowValue)
          return notSupported();
        else
          return skipBytes(9);
      case 0xd8:  // fixext 16
        if (allowValue)
          return notSupported();
        else
          return skipBytes(17);
      case 0xd9:
        if (allowValue)
          return readString<uint8_t>(variant);
        else
          return skipString<uint8_t>();
      case 0xda:
        if (allowValue)
          return readString<uint16_t>(variant);
        else
          return skipString<uint16_t>();
      case 0xdb:
        if (allowValue)
          return readString<uint32_t>(variant);
        else
          return skipString<uint32_t>();
      case 0xdc:
        return readArray<uint16_t>(variant, filter, nestingLimit);
      case 0xdd:
        return readArray<uint32_t>(variant, filter, nestingLimit);
      case 0xde:
        return readObject<uint16_t>(variant, filter, nestingLimit);
      case 0xdf:
        return readObject<uint32_t>(variant, filter, nestingLimit);
    }
    switch (code & 0xf0) {
      case 0x80:
        return readObject(variant, code & 0x0F, filter, nestingLimit);
      case 0x90:
        return readArray(variant, code & 0x0F, filter, nestingLimit);
    }
    if ((code & 0xe0) == 0xa0) {
      if (allowValue)
        return readString(variant, code & 0x1f);
      else
        return skipBytes(code & 0x1f);
    }
    if (allowValue)
      variant.setInteger(static_cast<int8_t>(code));
    return true;
  }
  bool readByte(uint8_t &value) {
    int c = _reader.read();
    if (c < 0) {
      _error = DeserializationError::IncompleteInput;
      return false;
    }
    value = static_cast<uint8_t>(c);
    return true;
  }
  bool readBytes(uint8_t *p, size_t n) {
    if (_reader.readBytes(reinterpret_cast<char *>(p), n) == n)
      return true;
    _error = DeserializationError::IncompleteInput;
    return false;
  }
  template <typename T>
  bool readBytes(T &value) {
    return readBytes(reinterpret_cast<uint8_t *>(&value), sizeof(value));
  }
  bool skipBytes(size_t n) {
    for (; n; --n) {
      if (_reader.read() < 0) {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
    }
    return true;
  }
  template <typename T>
  bool readInteger(T &value) {
    if (!readBytes(value))
      return false;
    fixEndianess(value);
    return true;
  }
  template <typename T>
  bool readInteger(VariantData &variant) {
    T value;
    if (!readInteger(value))
      return false;
    variant.setInteger(value);
    return true;
  }
  template <typename T>
  typename enable_if<sizeof(T) == 4, bool>::type readFloat(
      VariantData &variant) {
    T value;
    if (!readBytes(value))
      return false;
    fixEndianess(value);
    variant.setFloat(value);
    return true;
  }
  template <typename T>
  typename enable_if<sizeof(T) == 8, bool>::type readDouble(
      VariantData &variant) {
    T value;
    if (!readBytes(value))
      return false;
    fixEndianess(value);
    variant.setFloat(value);
    return true;
  }
  template <typename T>
  typename enable_if<sizeof(T) == 4, bool>::type readDouble(
      VariantData &variant) {
    uint8_t i[8];  // input is 8 bytes
    T value;       // output is 4 bytes
    uint8_t *o = reinterpret_cast<uint8_t *>(&value);
    if (!readBytes(i, 8))
      return false;
    doubleToFloat(i, o);
    fixEndianess(value);
    variant.setFloat(value);
    return true;
  }
  template <typename T>
  bool readString(VariantData &variant) {
    T size;
    if (!readInteger(size))
      return false;
    return readString(variant, size);
  }
  template <typename T>
  bool readString() {
    T size;
    if (!readInteger(size))
      return false;
    return readString(size);
  }
  template <typename T>
  bool skipString() {
    T size;
    if (!readInteger(size))
      return false;
    return skipBytes(size);
  }
  bool readString(VariantData &variant, size_t n) {
    if (!readString(n))
      return false;
    variant.setStringPointer(_stringStorage.save(),
                             typename TStringStorage::storage_policy());
    return true;
  }
  bool readString(size_t n) {
    _stringStorage.startString();
    for (; n; --n) {
      uint8_t c;
      if (!readBytes(c))
        return false;
      _stringStorage.append(static_cast<char>(c));
    }
    _stringStorage.append('\0');
    if (!_stringStorage.isValid()) {
      _error = DeserializationError::NoMemory;
      return false;
    }
    return true;
  }
  template <typename TSize, typename TFilter>
  bool readArray(VariantData &variant, TFilter filter,
                 NestingLimit nestingLimit) {
    TSize size;
    if (!readInteger(size))
      return false;
    return readArray(variant, size, filter, nestingLimit);
  }
  template <typename TFilter>
  bool readArray(VariantData &variant, size_t n, TFilter filter,
                 NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }
    bool allowArray = filter.allowArray();
    CollectionData *array = allowArray ? &variant.toArray() : 0;
    TFilter memberFilter = filter[0U];
    for (; n; --n) {
      VariantData *value;
      if (memberFilter.allow()) {
        value = array->addElement(_pool);
        if (!value) {
          _error = DeserializationError::NoMemory;
          return false;
        }
      } else {
        value = 0;
      }
      if (!parseVariant(*value, memberFilter, nestingLimit.decrement()))
        return false;
    }
    return true;
  }
  template <typename TSize, typename TFilter>
  bool readObject(VariantData &variant, TFilter filter,
                  NestingLimit nestingLimit) {
    TSize size;
    if (!readInteger(size))
      return false;
    return readObject(variant, size, filter, nestingLimit);
  }
  template <typename TFilter>
  bool readObject(VariantData &variant, size_t n, TFilter filter,
                  NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }
    CollectionData *object = filter.allowObject() ? &variant.toObject() : 0;
    for (; n; --n) {
      if (!readKey())
        return false;
      const char *key = _stringStorage.c_str();
      TFilter memberFilter = filter[key];
      VariantData *member;
      if (memberFilter.allow()) {
        key = _stringStorage.save();
        VariantSlot *slot = object->addSlot(_pool);
        if (!slot) {
          _error = DeserializationError::NoMemory;
          return false;
        }
        slot->setKey(key, typename TStringStorage::storage_policy());
        member = slot->data();
      } else {
        member = 0;
      }
      if (!parseVariant(*member, memberFilter, nestingLimit.decrement()))
        return false;
    }
    return true;
  }
  bool readKey() {
    uint8_t code;
    if (!readByte(code))
      return false;
    if ((code & 0xe0) == 0xa0)
      return readString(code & 0x1f);
    switch (code) {
      case 0xd9:
        return readString<uint8_t>();
      case 0xda:
        return readString<uint16_t>();
      case 0xdb:
        return readString<uint32_t>();
      default:
        return notSupported();
    }
  }
  template <typename T>
  bool skipExt() {
    T size;
    if (!readInteger(size))
      return false;
    return skipBytes(size + 1);
  }
  MemoryPool *_pool;
  TReader _reader;
  TStringStorage _stringStorage;
  DeserializationError _error;
  bool _foundSomething;
};
template <typename TString>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, const TString &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
                                          AllowAllFilter());
}
template <typename TString>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, const TString &input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TString>
DeserializationError deserializeMsgPack(JsonDocument &doc, const TString &input,
                                        NestingLimit nestingLimit,
                                        Filter filter) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TStream>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TStream &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
                                          AllowAllFilter());
}
template <typename TStream>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TStream &input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TStream>
DeserializationError deserializeMsgPack(JsonDocument &doc, TStream &input,
                                        NestingLimit nestingLimit,
                                        Filter filter) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TChar>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TChar *input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
                                          AllowAllFilter());
}
template <typename TChar>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TChar *input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TChar>
DeserializationError deserializeMsgPack(JsonDocument &doc, TChar *input,
                                        NestingLimit nestingLimit,
                                        Filter filter) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TChar>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TChar *input, size_t inputSize,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, inputSize, nestingLimit,
                                          AllowAllFilter());
}
template <typename TChar>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TChar *input, size_t inputSize, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, inputSize, nestingLimit,
                                          filter);
}
template <typename TChar>
DeserializationError deserializeMsgPack(JsonDocument &doc, TChar *input,
                                        size_t inputSize,
                                        NestingLimit nestingLimit,
                                        Filter filter) {
  return deserialize<MsgPackDeserializer>(doc, input, inputSize, nestingLimit,
                                          filter);
}
template <typename TWriter>
class MsgPackSerializer : public Visitor<size_t> {
 public:
  MsgPackSerializer(TWriter writer) : _writer(writer) {}
  template <typename T>
  typename enable_if<sizeof(T) == 4, size_t>::type visitFloat(T value32) {
    writeByte(0xCA);
    writeInteger(value32);
    return bytesWritten();
  }
  template <typename T>
  ARDUINOJSON_NO_SANITIZE("float-cast-overflow")
  typename enable_if<sizeof(T) == 8, size_t>::type visitFloat(T value64) {
    float value32 = float(value64);
    if (value32 == value64) {
      writeByte(0xCA);
      writeInteger(value32);
    } else {
      writeByte(0xCB);
      writeInteger(value64);
    }
    return bytesWritten();
  }
  size_t visitArray(const CollectionData& array) {
    size_t n = array.size();
    if (n < 0x10) {
      writeByte(uint8_t(0x90 + array.size()));
    } else if (n < 0x10000) {
      writeByte(0xDC);
      writeInteger(uint16_t(n));
    } else {
      writeByte(0xDD);
      writeInteger(uint32_t(n));
    }
    for (VariantSlot* slot = array.head(); slot; slot = slot->next()) {
      slot->data()->accept(*this);
    }
    return bytesWritten();
  }
  size_t visitObject(const CollectionData& object) {
    size_t n = object.size();
    if (n < 0x10) {
      writeByte(uint8_t(0x80 + n));
    } else if (n < 0x10000) {
      writeByte(0xDE);
      writeInteger(uint16_t(n));
    } else {
      writeByte(0xDF);
      writeInteger(uint32_t(n));
    }
    for (VariantSlot* slot = object.head(); slot; slot = slot->next()) {
      visitString(slot->key());
      slot->data()->accept(*this);
    }
    return bytesWritten();
  }
  size_t visitString(const char* value) {
    ARDUINOJSON_ASSERT(value != NULL);
    size_t n = strlen(value);
    if (n < 0x20) {
      writeByte(uint8_t(0xA0 + n));
    } else if (n < 0x100) {
      writeByte(0xD9);
      writeInteger(uint8_t(n));
    } else if (n < 0x10000) {
      writeByte(0xDA);
      writeInteger(uint16_t(n));
    } else {
      writeByte(0xDB);
      writeInteger(uint32_t(n));
    }
    writeBytes(reinterpret_cast<const uint8_t*>(value), n);
    return bytesWritten();
  }
  size_t visitRawJson(const char* data, size_t size) {
    writeBytes(reinterpret_cast<const uint8_t*>(data), size);
    return bytesWritten();
  }
  size_t visitNegativeInteger(UInt value) {
    UInt negated = UInt(~value + 1);
    if (value <= 0x20) {
      writeInteger(int8_t(negated));
    } else if (value <= 0x80) {
      writeByte(0xD0);
      writeInteger(int8_t(negated));
    } else if (value <= 0x8000) {
      writeByte(0xD1);
      writeInteger(int16_t(negated));
    } else if (value <= 0x80000000) {
      writeByte(0xD2);
      writeInteger(int32_t(negated));
    }
#if ARDUINOJSON_USE_LONG_LONG
    else {
      writeByte(0xD3);
      writeInteger(int64_t(negated));
    }
#endif
    return bytesWritten();
  }
  size_t visitPositiveInteger(UInt value) {
    if (value <= 0x7F) {
      writeInteger(uint8_t(value));
    } else if (value <= 0xFF) {
      writeByte(0xCC);
      writeInteger(uint8_t(value));
    } else if (value <= 0xFFFF) {
      writeByte(0xCD);
      writeInteger(uint16_t(value));
    }
#if ARDUINOJSON_USE_LONG_LONG
    else if (value <= 0xFFFFFFFF)
#else
    else
#endif
    {
      writeByte(0xCE);
      writeInteger(uint32_t(value));
    }
#if ARDUINOJSON_USE_LONG_LONG
    else {
      writeByte(0xCF);
      writeInteger(uint64_t(value));
    }
#endif
    return bytesWritten();
  }
  size_t visitBoolean(bool value) {
    writeByte(value ? 0xC3 : 0xC2);
    return bytesWritten();
  }
  size_t visitNull() {
    writeByte(0xC0);
    return bytesWritten();
  }
 private:
  size_t bytesWritten() const {
    return _writer.count();
  }
  void writeByte(uint8_t c) {
    _writer.write(c);
  }
  void writeBytes(const uint8_t* p, size_t n) {
    _writer.write(p, n);
  }
  template <typename T>
  void writeInteger(T value) {
    fixEndianess(value);
    writeBytes(reinterpret_cast<uint8_t*>(&value), sizeof(value));
  }
  CountingDecorator<TWriter> _writer;
};
template <typename TSource, typename TDestination>
inline size_t serializeMsgPack(const TSource& source, TDestination& output) {
  return serialize<MsgPackSerializer>(source, output);
}
template <typename TSource>
inline size_t serializeMsgPack(const TSource& source, void* output,
                               size_t size) {
  return serialize<MsgPackSerializer>(source, output, size);
}
template <typename TSource>
inline size_t measureMsgPack(const TSource& source) {
  return measure<MsgPackSerializer>(source);
}
}  // namespace ARDUINOJSON_NAMESPACE
#ifdef __GNUC__
#define ARDUINOJSON_PRAGMA(x) _Pragma(#x)
#define ARDUINOJSON_COMPILE_ERROR(msg) ARDUINOJSON_PRAGMA(GCC error msg)
#define ARDUINOJSON_STRINGIFY(S) #S
#define ARDUINOJSON_DEPRECATION_ERROR(X, Y) \
  ARDUINOJSON_COMPILE_ERROR(ARDUINOJSON_STRINGIFY(X is a Y from ArduinoJson 5. Please see arduinojson.org/upgrade to learn how to upgrade your program to ArduinoJson version 6))
#define StaticJsonBuffer ARDUINOJSON_DEPRECATION_ERROR(StaticJsonBuffer, class)
#define DynamicJsonBuffer ARDUINOJSON_DEPRECATION_ERROR(DynamicJsonBuffer, class)
#define JsonBuffer ARDUINOJSON_DEPRECATION_ERROR(JsonBuffer, class)
#define RawJson ARDUINOJSON_DEPRECATION_ERROR(RawJson, function)
#endif
namespace ArduinoJson {
typedef ARDUINOJSON_NAMESPACE::ArrayConstRef JsonArrayConst;
typedef ARDUINOJSON_NAMESPACE::ArrayRef JsonArray;
typedef ARDUINOJSON_NAMESPACE::Float JsonFloat;
typedef ARDUINOJSON_NAMESPACE::Integer JsonInteger;
typedef ARDUINOJSON_NAMESPACE::ObjectConstRef JsonObjectConst;
typedef ARDUINOJSON_NAMESPACE::ObjectRef JsonObject;
typedef ARDUINOJSON_NAMESPACE::Pair JsonPair;
typedef ARDUINOJSON_NAMESPACE::PairConst JsonPairConst;
typedef ARDUINOJSON_NAMESPACE::String JsonString;
typedef ARDUINOJSON_NAMESPACE::UInt JsonUInt;
typedef ARDUINOJSON_NAMESPACE::VariantConstRef JsonVariantConst;
typedef ARDUINOJSON_NAMESPACE::VariantRef JsonVariant;
using ARDUINOJSON_NAMESPACE::BasicJsonDocument;
using ARDUINOJSON_NAMESPACE::copyArray;
using ARDUINOJSON_NAMESPACE::DeserializationError;
using ARDUINOJSON_NAMESPACE::deserializeJson;
using ARDUINOJSON_NAMESPACE::deserializeMsgPack;
using ARDUINOJSON_NAMESPACE::DynamicJsonDocument;
using ARDUINOJSON_NAMESPACE::JsonDocument;
using ARDUINOJSON_NAMESPACE::measureJson;
using ARDUINOJSON_NAMESPACE::serialized;
using ARDUINOJSON_NAMESPACE::serializeJson;
using ARDUINOJSON_NAMESPACE::serializeJsonPretty;
using ARDUINOJSON_NAMESPACE::serializeMsgPack;
using ARDUINOJSON_NAMESPACE::StaticJsonDocument;
namespace DeserializationOption {
using ARDUINOJSON_NAMESPACE::Filter;
using ARDUINOJSON_NAMESPACE::NestingLimit;
}  // namespace DeserializationOption
}  // namespace ArduinoJson

using namespace ArduinoJson;

#else

#error ArduinoJson requires a C++ compiler, please change file extension to .cc or .cpp

#endif
