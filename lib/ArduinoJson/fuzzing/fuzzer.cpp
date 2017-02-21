#include <ArduinoJson.h>

class memstream : public std::istream {
  struct membuf : std::streambuf {
    membuf(const uint8_t *p, size_t l) {
      setg((char *)p, (char *)p, (char *)p + l);
    }
  };
  membuf _buffer;

 public:
  memstream(const uint8_t *p, size_t l)
      : std::istream(&_buffer), _buffer(p, l) {
    rdbuf(&_buffer);
  }
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  DynamicJsonBuffer jsonBuffer;
  memstream json(data, size);
  jsonBuffer.parse(json);
  return 0;
}
