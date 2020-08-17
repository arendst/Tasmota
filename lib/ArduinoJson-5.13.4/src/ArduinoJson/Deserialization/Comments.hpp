// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {
template <typename TInput>
void skipSpacesAndComments(TInput& input) {
  for (;;) {
    switch (input.current()) {
      // spaces
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        input.move();
        continue;

      // comments
      case '/':
        switch (input.next()) {
          // C-style block comment
          case '*':
            input.move();  // skip '/'
            // no need to skip '*'
            for (;;) {
              input.move();
              if (input.current() == '\0') return;
              if (input.current() == '*' && input.next() == '/') {
                input.move();  // skip '*'
                input.move();  // skip '/'
                break;
              }
            }
            break;

          // C++-style line comment
          case '/':
            // not need to skip "//"
            for (;;) {
              input.move();
              if (input.current() == '\0') return;
              if (input.current() == '\n') break;
            }
            break;

          // not a comment, just a '/'
          default:
            return;
        }
        break;

      default:
        return;
    }
  }
}
}
}
