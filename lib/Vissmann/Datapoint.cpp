#include "Datapoint.h"

//declare static global callback function as part of the Datapoint base class
GlobalCallbackFunction Datapoint::_globalCallback = nullptr;


Datapoint::Datapoint(const char* name, const char* group, const uint16_t address, bool isWriteable):
  _name(name),
  _group(group),
  _address(address),
  _writeable(isWriteable)
  {}


Datapoint::~Datapoint() {
  abort();  //destruction is not supported
}


const char* Datapoint::getName() const {
  return _name;
}


const char* Datapoint::getGroup() const {
  return _group;
}


const uint16_t Datapoint::getAddress() const {
  return _address;
}


const bool Datapoint::isWriteable() const {
  return _writeable;
}


Datapoint& Datapoint::setWriteable() {
  _writeable = true;
  return *this;
}


void Datapoint::setGlobalCallback(GlobalCallbackFunction globalCallback) {
  _globalCallback = globalCallback;
}


TempDP::TempDP(const char* name, const char* group, const uint16_t address, bool isWriteable):
  Datapoint(name, group, address, isWriteable),
  _callback(nullptr)
  {}


Datapoint& TempDP::setCallback(TempCallbackFunction callback) {
  _callback = callback;
  return *this;
}


void TempDP::callback(uint8_t value[]) {
  int16_t tmp = value[1] << 8 | value[0];
  float floatValue = (float)(tmp) / 10.0;
  if (_callback) {
    _callback(_name, _group, floatValue);
  }
  else if (Datapoint::_globalCallback) {
    char str[6] = {'\0'};
    //snprintf(str, sizeof(str), "%f", floatValue);
    dtostrf(floatValue, 3, 1, str);
    _globalCallback(_name, _group, str);
  }
  return;
}


void TempDP::transform(uint8_t transformedValue[], float value) {
  int16_t tmp = floor((value * 10) + 0.5);
  transformedValue[1] = tmp >> 8;
  transformedValue[0] = tmp & 0xFF;
  return;
}


StatDP::StatDP(const char* name, const char* group, const uint16_t address, bool isWriteable):
  Datapoint(name, group, address, isWriteable),
  _callback(nullptr)
  {}


Datapoint& StatDP::setCallback(StatCallbackFunction callback) {
  _callback = callback;
  return *this;
}


void StatDP::callback(uint8_t value[]) {
  bool boolValue = (value[0]) ? true : false;
  if (_callback) {
    _callback(_name, _group, boolValue);
  }
  else if (_globalCallback) {
    const char* str = (boolValue) ? "1" : "0";  //or "true/false"?
    _globalCallback(_name, _group, str);
  }
}


void StatDP::transform(uint8_t transformedValue[], float value) {
  transformedValue[0] = (value) ? 0x01 : 0x00;
  return;
}


CountLDP::CountLDP(const char* name, const char* group, const uint16_t address, bool isWriteable):
  Datapoint(name, group, address, isWriteable),
  _callback(nullptr)
  {}


Datapoint& CountLDP::setCallback(CountLCallbackFunction callback) {
  _callback = callback;
  return *this;
}


void CountLDP::callback(uint8_t value[]) {
  uint32_t ui32 = value[0] | (value[1] << 8) | (value[2] << 16) | (value[3] << 24);
  if (_callback) {
    _callback(_name, _group, ui32);
  }
  else if (_globalCallback) {
    char str[11] = {'\0'};
    snprintf(str, sizeof(str), "%" PRIu32, ui32);
    _globalCallback(_name, _group, str);
  }
}


void CountLDP::transform(uint8_t transformedValue[], float value) {
  uint32_t _value = (uint32_t)ceil(value);
  transformedValue[3] = _value >> 24;
  transformedValue[2] = _value >> 16;
  transformedValue[1] = _value >> 8;
  transformedValue[0] = _value & 0xFF;
  return;
}


/*
ModeDP::ModeDP(const char* name, const char* group, const uint16_t address, bool isWriteable):
  Datapoint(name, group, address, isWriteable)
  {}


Datapoint& ModeDP::setCallback(ModeCallbackFunction callback) {
  _callback = callback;
  return *this;
}


void ModeDP::callback(DPValue value) {
  uint8_t intValue = (value.byte1Value);
  if (_callback) {
    _callback(this->getName(), this->getGroup(), intValue);
  }
  else if (_globalCallback) {
    _globalCallback(getName(), getGroup(), intValue);
  }
  else {
    _debugPrinter->println(F("no callback found"));
  }
}


HoursDP::HoursDP(const char* name, const char* group, const uint16_t address, bool isWriteable):
  Datapoint(name, group, address, isWriteable)
  {}


Datapoint& HoursDP::setCallback(HoursCallbackFunction callback) {
  _callback = callback;
  return *this;
}


HoursLDP::HoursLDP(const char* name, const char* group, const uint16_t address, bool isWriteable):
  Datapoint(name, group, address, isWriteable)
  {}


Datapoint& HoursLDP::setCallback(HoursLCallbackFunction callback) {
  _callback = callback;
  return *this;
}

*/
