#include "VitoWifi.h"

//create an instance of the VitoWifiClass to be used in the main sketch
VitoWifiClass VitoWifi;


VitoWifiClass::VitoWifiClass():
  _optolink(),
  _enableLed(false),
  _errorString(),
  _logger() {
    _errorString = new char[40];
}


VitoWifiClass::~VitoWifiClass() {
  _logger.println(F("Destructing of VitoWifi is unsupported"));
  abort();
}


//pass serial to Optolink
#ifdef USE_SOFTWARESERIAL
void VitoWifiClass::setup(const uint8_t rxPin, const uint8_t txPin) {
  _optolink.begin(rxPin, txPin);
  _datapoints.shrink_to_fit();
}
#else
void VitoWifiClass::setup(HardwareSerial* serial) {
  _optolink.begin(serial);
  _datapoints.shrink_to_fit();
}
#endif


void VitoWifiClass::setGlobalCallback(GlobalCallbackFunction globalCallback) {
  if (_datapoints.size()) {
    _datapoints.front()->setGlobalCallback(globalCallback);
  }
}


Datapoint& VitoWifiClass::addDatapoint(const char* name, const char* group, const uint16_t address, const DPType type, bool isWriteable) {
  switch (type) {
    case TEMP :
    {
      Datapoint* tempDP = new TempDP(name, group, address, isWriteable);
      if (!tempDP) {  abort(); }  //out of memory
      _datapoints.push_back(tempDP);
      break;
    }
    case STAT :
    {
      Datapoint* statDP = new StatDP(name, group, address, isWriteable);
      if (!statDP) { abort(); }  //out of memory
      _datapoints.push_back(statDP);
      break;
    }
    case COUNTL :
    {
      Datapoint* countLDP = new CountLDP(name, group, address, isWriteable);
      if (!countLDP) { abort(); }  //out of memory
      _datapoints.push_back(countLDP);
      break;
    }
  }
  return *_datapoints.back();
}


Datapoint& VitoWifiClass::addDatapoint(const char* name, const char* group, const uint16_t address, const DPType type) {
  return addDatapoint(name, group, address, type, false);
}


Datapoint* VitoWifiClass::_getDatapoint(const char* name) {
  //find DP and return pointer
  Datapoint* DP = nullptr;
  for(std::vector<Datapoint*>::iterator it = _datapoints.begin(); it != _datapoints.end(); ++it) {
    if (strcmp(name, (*it)->getName()) == 0)
      DP = *it;
  }
  return DP;
}


void VitoWifiClass::readAll() {
  bool foundOne = false;
  for (Datapoint* iDP : _datapoints) {
    _readDatapoint(iDP);
    foundOne = true;
  }
  if (foundOne) return;
  _logger.println(F("No datapoints available, skipping"));
}


void VitoWifiClass::readGroup(const char* group) {
  bool foundOne = false;
  for (Datapoint* iDP : _datapoints) {
    if (strcmp(group, iDP->getGroup()) == 0) {
      _readDatapoint(iDP);
      foundOne = true;
    }
  }
  if(!foundOne) _logger.println(F("Group not found, skipping"));
}


void VitoWifiClass::readDatapoint(const char* name) {
  for (Datapoint* iDP : _datapoints) {
    if (strcmp(name, iDP->getName()) == 0) {
      _readDatapoint(iDP);
      return;
    }
  }
  _logger.println(F("Datapoint not found, skipping"));
}


inline void VitoWifiClass::_readDatapoint(Datapoint* dp) {
  Action action = {dp, false};
  _queue.push(action);
  _logger.print("Datapoint ");
  _logger.print(dp->getName());
  _logger.println(" READ action added");
}


void VitoWifiClass::_writeDatapoint(const char* name, float value, size_t length) {
  Datapoint* DP = _getDatapoint(name);
  if (DP) {
    if (DP->getLength() != length) {
      _logger.println(F("Value type does not match Datapoint type, skipping"));
      return;
    }
    uint8_t transformedValue[2] = {0};
    DP->transform(transformedValue, value);
    Action action;
    action.DP = DP;
    action.write = true;
    memcpy(action.value, transformedValue, 2);
    _queue.push(action);
    return;
  }
  _logger.println(F("Datapoint not found, skipping"));
}


void VitoWifiClass::loop(){
  _optolink.loop();
  if (!_queue.empty() && !_optolink.isBusy()) {
    if (_queue.front().write) _optolink.writeToDP(_queue.front().DP->getAddress(), _queue.front().DP->getLength(), _queue.front().value);
    else _optolink.readFromDP(_queue.front().DP->getAddress(), _queue.front().DP->getLength());
    return;
  }
  if (_optolink.available() > 0) {  //trigger callback when ready and remove element from queue
    _logger.print(F("Datapoint "));
    _logger.print(_queue.front().DP->getName());
    _logger.println(F(" action successful"));
    uint8_t value[4] = {0};
    _optolink.read(value);
    _logger.println(F("Value received"));
    _queue.front().DP->callback(value);
    _queue.pop();
    return;
  }
  if (_optolink.available() < 0) {  //display error message and remove element from queue
    _logger.print(F("Datapoint "));
    _logger.print(_queue.front().DP->getName());
    _logger.print(F(" action unsuccessful - CODE:"));
    uint8_t errorCode = _optolink.readError();
    _logger.println(errorCode, DEC);
    _queue.pop();
    return;
  }
}


/*
void VitoWifiClass::enableLed(uint8_t pin, uint8_t on) {
 _enableLed = true;
 _blinker.setLedPin(pin, on);
}
*/


//Logger stuff, taken from Marvin ROGER's Homie for ESP8266
void VitoWifiClass::setLoggingPrinter(Print* printer) {
  _logger.setPrinter(printer);
  _optolink.setDebugPrinter(&_logger);
}
void VitoWifiClass::enableLogger() {
  _logger.setLogging(true);
}
void VitoWifiClass::disableLogger() {
  _logger.setLogging(false);
}
