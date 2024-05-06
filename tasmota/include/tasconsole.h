/*
  tasconsole.h - ESP32 tasconsole driver for Tasmota

  SPDX-FileCopyrightText: 2023 Christian Baars (@staars)

  SPDX-License-Identifier: GPL-3.0-only
*/

#include <memory>

class TASCONSOLE {

  struct Concept {
    virtual ~Concept() = default;
    virtual int available() = 0;
    virtual void begin(uint32_t) = 0;
    virtual void flush() = 0;
    virtual size_t println() = 0;
    virtual size_t print(char *) = 0;
    virtual size_t printf(const char*, char *, const char*&, const char*&, const char*&) = 0;
    virtual size_t printf(char *) = 0;
    virtual size_t read() = 0;
    virtual size_t setRxBufferSize(size_t) = 0;
//    virtual size_t setTxBufferSize(size_t) = 0;
  };

public:
  template <typename T>
  TASCONSOLE(T&& obj){
    object = std::make_unique<Model<T>>(std::forward<T>(obj));
  }

  int available() {
    return object->available(); 
  }

  void begin(uint32_t baud) {
    object->begin(baud); 
  }

  void flush() {
    object->flush(); 
  }

  size_t println() {
    return object->println(); 
  }

  size_t print(char * string) {
    return object->print(string); 
  }

  size_t printf(char *format) {
    return object->printf(format); 
  }

  size_t printf(const char* a, char * b, const char*& c, const char*& d, const char*& f){
    return object->printf(a,b,c,d,f);
  }

  size_t read() {
    return object->read();
  }

  size_t setRxBufferSize(size_t rx_queue_len) {
    return object->setRxBufferSize(rx_queue_len); 
  }

//  size_t setTxBufferSize(size_t rx_queue_len) {
//    return object->setTxBufferSize(rx_queue_len); 
//  }

  template< typename T >
  struct Model : Concept {
    Model(T const& t) : object(t) {}
    int available() override {
      return object.available();
    }

    void begin(uint32_t baud) override {
      object.begin(baud);
    }

     void flush() override {
		  object.flush();
    }

     size_t println() override {
      return object.println();
    }
     size_t print(char * string) override {
      return object.print(string);
    }

    size_t printf(char *format) override {
      return object.printf(format);
    }

    size_t printf(const char* a, char * b, const char*& c, const char*& d, const char*& f) override {
      return object.printf(a,b,c,d,f);
    }

    size_t read() override {
      return object.read();
    }

    size_t setRxBufferSize(size_t size) override {
		  return object.setRxBufferSize(size);
    }

//    size_t setTxBufferSize(size_t size) override {
//		  return object.setTxBufferSize(size);
//    }

  private:
    T object;
  };

  std::unique_ptr<Concept> object;
};
