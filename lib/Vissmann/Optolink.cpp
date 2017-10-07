#include "Optolink.h"


Optolink::Optolink():
  _serialptr(nullptr),
  _address(0),
  _length(0),
  _value{0},
  _writeMessageType(false),
  _rcvBuffer{0},
  _rcvBufferLen(0),
  _rcvLen(0),
  _debugMessage(true),
  _state(RESET),
  _action(PROCESS),
  _lastMillis(0),
  _numberOfTries(5),
  _errorCode(0),
  _debugPrinter(nullptr)
  {}


//begin serial @ 4800 baud, 8 bits, even parity, 2 stop bits
#ifdef USE_SOFTWARESERIAL
void Optolink::begin(const uint8_t rx, const uint8_t tx) {
  _serialptr = new SoftwareSerial((int)rx, (int)tx, false, 64);
  _serialptr->begin(4800);
  _serialptr->setParity(PARITY_MODE_EVEN);
  _serialptr->setStopBits(STOP_BITS_TWO);
  //serial->flush();
  //serial->swap();  //use GPIO15 and GPIO13 for TX/RX
}
#else
void Optolink::begin(HardwareSerial* serial) {
  _serialptr = serial;
  _serialptr->begin(4800, SERIAL_8E2);
  //serial->flush();
  //serial->swap();  //use GPIO15 and GPIO13 for TX/RX
}
#endif


void Optolink::loop() {
  if (_numberOfTries < 1) {
    _state = IDLE;
    _action = RETURN_ERROR;
  }
  switch (_state) {
    case RESET:
      _resetHandler();
      break;
    case RESET_ACK:
      _resetAckHandler();
      break;
    case INIT:
      _initHandler();
      break;
    case INIT_ACK:
      _initAckHandler();
      break;
    case IDLE:
      _idleHandler();
      break;
    case SYNC:
      _syncHandler();
      break;
    case SYNC_ACK:
      _syncAckHandler();
      break;
    case SEND:
      _sendHandler();
      break;
    case SEND_ACK:
      _sendAckHandler();
      break;
    case RECEIVE:
      _receiveHandler();
      break;
  }
}


//Set communication with Vitotronic to defined state = reset to KW protocol
void Optolink::_resetHandler() {
  const uint8_t buff[] = {0x04};
  _serialptr->write(buff, sizeof(buff));
  _lastMillis = millis();
  _state = RESET_ACK;
  if (_debugMessage) {
    _debugPrinter->println(F("Resetting Optolink..."));
    _debugMessage = false;
  }
}


void Optolink::_resetAckHandler() {
  if (_serialptr->available()) {
    if (_serialptr->peek() == 0x05) {  //use peek so connection can be made immediately in next state
      //received 0x05/enquiry: optolink has been reset
      _state = INIT;
      _debugMessage = true;
      _debugPrinter->println(F("Optolink reset."));
    }
    else {
      _clearInputBuffer();
    }
  }
  else {
    if (millis() - _lastMillis > 500) {  //try again every 0,5sec
      _state = RESET;
    }
  }
}


//send initiator to Vitotronic to establish connection
void Optolink::_initHandler() {
  if (_debugMessage) {
    _debugPrinter->println(F("Establishing Optolink connection..."));
    _debugMessage = false;
  }
  if (_serialptr->available()) {
    if (_serialptr->read() == 0x05) {
      //0x05/ENQ received, sending initiator
      const uint8_t buff[] = {0x16, 0x00, 0x00};
      _serialptr->write(buff, sizeof(buff));
      _lastMillis = millis();
      _state = INIT_ACK;
    }
  }
}


void Optolink::_initAckHandler() {
  if (_serialptr->available()) {
    if (_serialptr->read() == 0x06) {
      //ACK received, moving to next state
      _state = IDLE;
      _action = WAIT;
      _debugMessage = true;
      //debug: done
      _debugPrinter->println(F("Optolink connection established."));
    }
    else {
      //return to previous state
      _clearInputBuffer();
      _state = INIT;
    }
  }
  if (millis() - _lastMillis > 10 * 1000UL) {  //if no ACK is coming, reset connection
    _state = RESET;
  }
}


//idle state, waiting for user action
void Optolink::_idleHandler() {
  if (millis() - _lastMillis > 2 * 60 * 1000UL) {  //send SYNC every 2 minutes to keep communication alive
    _state = SYNC;
  }
  _clearInputBuffer(); //keep input clean
  if (_action == PROCESS) _state = SYNC;
}


//send SYNC (= initiator)
void Optolink::_syncHandler() {
  const uint8_t buff[] = {0x16, 0x00, 0x00};
  _serialptr->write(buff, sizeof(buff));
  _lastMillis = millis();
  _state = SYNC_ACK;
}


void Optolink::_syncAckHandler() {
  if (_serialptr->available()) {
    if (_serialptr->read() == 0x06) {
      if(_action == PROCESS) _state = SEND;
      else _state = IDLE;
    }
  }
  if (millis() - _lastMillis > 2 * 1000UL) {  //if no ACK is coming, reset connection
    _state = RESET;
    if (_action = PROCESS) --_numberOfTries;
  }
}


void Optolink::_sendHandler() {
  uint8_t buff[12];
  if (_writeMessageType) {
    //type is WRITE
    //has length of 8 chars + length of value
    buff[0] = 0x41;
    buff[1] = 5 + _length;
    buff[2] = 0x00;
    buff[3] = 0x02;
    buff[4] = (_address >> 8) & 0xFF;
    buff[5] = _address & 0xFF;
    buff[6] = _length;
    //add value to message
    memcpy(&buff[7], _value, 1);
    buff[7 + _length] = _calcChecksum(buff, 7 + _length);
    _serialptr->write(buff, 8 + _length);
  }
  else {
    //type is READ
    //has fixed length of 8 chars
    buff[0] = 0x41;
    buff[1] = 0x05;
    buff[2] = 0x00;
    buff[3] = 0x01;
    buff[4] = (_address >> 8) & 0xFF;
    buff[5] = _address & 0xFF;
    buff[6] = _length;
    buff[7] = _calcChecksum(buff, 8);
    //set length of expected answer
    _rcvLen = 8 + _length;
    _serialptr->write(buff, 8);
  }
  _rcvBufferLen = 0;
  _lastMillis = millis();
  --_numberOfTries;
  _state = SEND_ACK;
  if (_writeMessageType) _debugPrinter->print(F("WRITE "));
  else _debugPrinter->print(F("READ"));
  _debugPrinter->print(F(" request on address "));
  _debugPrinter->print(_address, HEX);
  _debugPrinter->print(F(", length "));
  _debugPrinter->println(_length);
}


void Optolink::_sendAckHandler() {
  if (_serialptr->available()) {
    uint8_t buff = _serialptr->read();
    if (buff == 0x06) {  //transmit succesful, moving to next state
      _debugPrinter->println(F("req: ack"));
      _state = RECEIVE;
    }
    else if (buff == 0x15) {  //transmit negatively acknowledged, return to SYNC and try again
      _debugPrinter->println(F("req: nack"));
      _state = SYNC;
      _clearInputBuffer();
    }
  }
  if (millis() - _lastMillis > 2 * 1000UL) {  //if no ACK is coming, return to SYNC and try again
    _debugPrinter->println(F("req: t/o"));
    _state = SYNC;
    _clearInputBuffer();
  }
}


void Optolink::_receiveHandler() {
  while (_serialptr->available() > 0) {  //while instead of if: read complete RX buffer
    _rcvBuffer[_rcvBufferLen] = _serialptr->read();
    if (_rcvBuffer[0] != 0x41) return; //find out why this is needed! I'd expect the rx-buffer to be empty.
    ++_rcvBufferLen;
  }
  if (_rcvBufferLen == _rcvLen) {  //message complete, check message
    if (_rcvBuffer[1] != (_rcvLen - 3)) {  //check for message length
      _numberOfTries = 0;
      _errorCode = 4;
      return;
    }
    if (_rcvBuffer[2] != 0x01) {  //Vitotronic returns an error message, skipping DP
      _numberOfTries = 0;
      _errorCode = 3;  //Vitotronic error
      return;
    }
    if (!_checkChecksum(_rcvBuffer, _rcvLen)) {  //checksum is wrong, trying again
      _rcvBufferLen = 0;
      _errorCode = 2;  //checksum error
      memset(_rcvBuffer, 0, 12);
      _state = SYNC;
      return;
    }
    if (_rcvBuffer[3] == 0x01) {
      //message is from READ command, so returning read value
    }
    _state = IDLE;
    _action = RETURN;
    _errorCode = 0;  //succes
    _debugPrinter->println(F("succes"));
    return;
  }
  if (millis() - _lastMillis > 10 * 1000UL) {  //Vitotronic isn't answering, try again
    _rcvBufferLen = 0;
    _errorCode = 1;  //Connection error
    memset(_rcvBuffer, 0, 12);
    _state = SYNC;
  }
}


//set properties for datapoint and move state to SEND
bool Optolink::readFromDP(uint16_t address, uint8_t length) {
  if (_action != WAIT) {
    _debugPrinter->println(F("Optolink not available, skipping action."));
    return false;
  }
  //setup properties for next state in communicationHandler
  _address = address;
  _length = length;
  _writeMessageType = false;
  _rcvBufferLen = 0;
  _numberOfTries = 5;
  memset(_rcvBuffer, 0, 12);
  _state = SYNC;  //avoid an extra loop to pass by idleHandler
  _action = PROCESS;
  return true;
}


//set properties datapoint and move state to SEND
bool Optolink::writeToDP(uint16_t address, uint8_t length, uint8_t value[]) {
  if (_action != WAIT) {
    _debugPrinter->println(F("Optolink not available, skipping action."));
    return false;
  }
  //setup variables for next state
  _address = address;
  _length = length;
  memcpy(_value, value, _length);
  _writeMessageType = true;
  _rcvBufferLen = 0;
  _numberOfTries = 5;
  memset(_rcvBuffer, 0, 12);
  _state = SYNC;  //avoid an extra loop to pass by idleHandler
  _action = PROCESS;
  return true;
}


const int8_t Optolink::available() const {
  if (_action == RETURN_ERROR) return -1;
  else if (_action == RETURN) return 1;
  else return 0;
}

const bool Optolink::isBusy() const {
  if (_action == WAIT) return false;
  else return true;
}


//return value and reset comunication to IDLE
void Optolink::read(uint8_t value[]) {
  if (_action != RETURN) {
    _debugPrinter->println(F("No reading available"));
    return;
  }
  if (_writeMessageType) {  //return original value in case of WRITE command
    memcpy(value, &_value, _length);
    _action = WAIT;
    return;
  }
  else {
    memcpy(value, &_rcvBuffer[7], _length);
    _action = WAIT;
    return;  //added for clarity
  }
}


const uint8_t Optolink::readError() {
  _action = WAIT;
  return _errorCode;
}


//calculate Checksum
inline uint8_t Optolink::_calcChecksum(uint8_t array[], uint8_t length) {
  uint8_t sum = 0;
  for (uint8_t i = 1; i < length - 1 ; ++i){  //start with second byte and en before checksum
    sum += array[i];
  }
  return sum;
}
inline bool Optolink::_checkChecksum(uint8_t array[], uint8_t length) {
  uint8_t sum = 0;
  for (uint8_t i = 1; i < length - 1; ++i) { //start with second byte and en before checksum
    sum += array[i];
  }
  return (array[length - 1] == sum);
}


//clear serial input buffer
inline void Optolink::_clearInputBuffer() {
  while(_serialptr->available() > 0) {
    _serialptr->read();
  }
}

void Optolink::setDebugPrinter(Print* printer) {
  _debugPrinter = printer;
}


//Copied from Arduino.cc forum --> (C) robtillaart
inline void Optolink::_printHex83(uint8_t array[], uint8_t length) {
  char tmp[length * 2 + 1];
  byte first;
  uint8_t j = 0;
  for (uint8_t i = 0; i < length; ++i) {
    first = (array[i] >> 4) | 48;
    if (first > 57) tmp[j] = first + (byte)39;
    else tmp[j] = first ;
    ++j;

    first = (array[i] & 0x0F) | 48;
    if (first > 57) tmp[j] = first + (byte)39;
    else tmp[j] = first;
    ++j;
  }
  tmp[length * 2] = 0;
  _debugPrinter->print(tmp);
}
