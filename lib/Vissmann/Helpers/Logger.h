#pragma once
#include <Arduino.h>

class Logger : public Print {
  public:
    Logger();
    virtual size_t write(uint8_t character);
    virtual size_t write(const uint8_t* buffer, size_t size);
    void setPrinter(Print* printer);
    void setLogging(bool enable);

  private:
    bool _loggingEnabled;
    Print* _printer;
};
