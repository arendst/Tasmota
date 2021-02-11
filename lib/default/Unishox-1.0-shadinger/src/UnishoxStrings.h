/*
  UnishoxStrings.c - support library for compressed strings in Flash

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __UNISHOX_STRINGS__
#define __UNISHOX_STRINGS__

#include <cstddef>
#include <cstdint>
#include "WString.h"


/*******************************************************************************************/
/*
 * UnishoxStrings compressed literal strings
 *
 * To be typically used for web UI to have multiple strings like JS, CSS, HTML
 * compressed in a single list of small strings. This is where Unishox shines.
 *
 * Strings before compression are separated with NULL chars, which makes them ready to use
 * once you know the offset in the global array.
 *
 * The list of string is marked by an empty stings, i.e. two consecutive NULL chars
 *
 * To distinguish from uncompressed templates, and to indicate the global size
 * the compressed array is prefixed with NULL and the uncompressed size * 16 bytes.
 *
 * Compressed format:
 * - Byte 00 : \00 - if non-null, then it is not compressed
 * - Byte 01 : <size16> - uncompressed size is <size16> * 16 bytes (always rounded up)
 * - Byte [] : compressed data (does not contain NULL)
 * - Byte n  : \00 - final NULL
 *
/*******************************************************************************************/


/*******************************************************************************************/
/*
 * This class is initialzed with either a PMEM uncompressed list of strings
 * or a PMEM unishox-compressed list of strings; in this case RAM is allocated
 * to hold the uncompressed data, until the class gets out of scope.
 *
 * To encode, use https://tasmota.hadinger.fr/util
 * and use the "Compress strings template with unishox"
 * past the C code for the strings, and copy/paste the result
 * (don't foget to rename the variable).
 *
 * Input:
 * Each string must be terminated with an explicit NULL char "\0". The list is hence
 * terminated with a double-NULL.
 *
 * Each string is then indexed as its byte offset in the whole template.
 * The offsets are computed at the same time as the compressed version.
 * You need to use the online compressor even if you don't use the comrpessed version.
 *
 * Indexes are marked as C++ comments starting with "//="
 *
 * Example input:
 *    con
 *    // start of strings
 *    //=STRING_1
 *    "This is string 1"
 *    "\0"
 *    //=STRING_2
 *    "This is string 2"
 *    "continued here"
 *    "\0"
 *    ;
 *
/*******************************************************************************************/

class UnishoxStrings {
public:
  UnishoxStrings(const char *k_msg);

  inline const char * operator[](uint16_t i) const { return _msg + i; }

  const char *  _msg = nullptr;
  String        _decompressed;
};


#endif // __UNISHOX_STRINGS__