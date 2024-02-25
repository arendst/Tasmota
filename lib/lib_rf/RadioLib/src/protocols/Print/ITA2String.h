#if !defined(_RADIOLIB_ITA2_STRING_H)
#define _RADIOLIB_ITA2_STRING_H

#include "../../TypeDef.h"

#define RADIOLIB_ITA2_FIGS                                      0x1B
#define RADIOLIB_ITA2_LTRS                                      0x1F
#define RADIOLIB_ITA2_LENGTH                                    32

// ITA2 character table: - position in array corresponds to 5-bit ITA2 code
//                       - characters to the left are in letters shift, characters to the right in figures shift
//                       - characters marked 0x7F do not have ASCII equivalent
static const char ITA2Table[RADIOLIB_ITA2_LENGTH][2] RADIOLIB_NONVOLATILE = {
  {'\0', '\0'}, {'E', '3'}, {'\n', '\n'}, {'A', '-'}, {' ', ' '}, {'S', '\''}, {'I', '8'}, {'U', '7'},
  {'\r', '\r'}, {'D', 0x05}, {'R', '4'}, {'J', '\a'}, {'N', ','}, {'F', '!'}, {'C', ':'}, {'K', '('},
  {'T', '5'}, {'Z', '+'}, {'L', ')'}, {'W', '2'}, {'H', 0x7F}, {'Y', '6'}, {'P', '0'}, {'Q', '1'},
  {'O', '9'}, {'B', '?'}, {'G', '&'}, {0x7F, 0x7F}, {'M', '.'}, {'X', '/'}, {'V', ';'}, {0x7F, 0x7F}
};

/*!
  \class ITA2String
  \brief ITA2-encoded string.
*/
class ITA2String {
  public:
    /*!
      \brief Default single-character constructor.
      \param c ASCII-encoded character to encode as ITA2.
    */
    ITA2String(char c);

    /*!
      \brief Default string constructor.
      \param str ASCII-encoded string to encode as ITA2.
    */
    ITA2String(const char* str);

    /*!
      \brief Default destructor.
    */
    ~ITA2String();

    /*!
      \brief Gets the length of the ITA2 string. This number is not the same as the length of ASCII-encoded string!
      \returns Length of ITA2-encoded string.
    */
    size_t length();

    /*!
      \brief Gets the ITA2 representation of the ASCII string set in constructor.
      \returns Pointer to dynamically allocated array, which contains ITA2-encoded bytes.
      It is the caller's responsibility to deallocate this memory!
    */
    uint8_t* byteArr();

#if !RADIOLIB_GODMODE
  private:
#endif
    #if RADIOLIB_STATIC_ONLY
      char strAscii[RADIOLIB_STATIC_ARRAY_SIZE];
    #else
      char* strAscii;
    #endif
    size_t asciiLen;
    size_t ita2Len;

    static uint16_t getBits(char c);
};

#endif
