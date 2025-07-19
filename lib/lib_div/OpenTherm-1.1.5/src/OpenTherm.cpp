/*
OpenTherm.cpp - OpenTherm Communication Library For Arduino, ESP8266, ESP32
Copyright 2023, Ihor Melnyk
*/

#include "OpenTherm.h"
#if !defined(__AVR__)
#include "FunctionalInterrupt.h"
#endif

OpenTherm::OpenTherm(int inPin, int outPin, bool isSlave) :
    status(OpenThermStatus::NOT_INITIALIZED),
    inPin(inPin),
    outPin(outPin),
    isSlave(isSlave),
    response(0),
    responseStatus(OpenThermResponseStatus::NONE),
    responseTimestamp(0),
    processResponseCallback(NULL)
{
}

void OpenTherm::begin(void (*handleInterruptCallback)(void))
{
    pinMode(inPin, INPUT);
    pinMode(outPin, OUTPUT);
    if (handleInterruptCallback != NULL)
    {
        attachInterrupt(digitalPinToInterrupt(inPin), handleInterruptCallback, CHANGE);
    }
    else
    {
#if !defined(__AVR__)
        attachInterruptArg(
            digitalPinToInterrupt(inPin),
            OpenTherm::handleInterruptHelper,
            this,
            CHANGE
        );
#endif
    }
    activateBoiler();
    status = OpenThermStatus::READY;
}

void OpenTherm::begin(void (*handleInterruptCallback)(void), void (*processResponseCallback)(unsigned long, int))
{
    begin(handleInterruptCallback);
    this->processResponseCallback = processResponseCallback;
}

#if !defined(__AVR__)
void OpenTherm::begin()
{
    begin(NULL);
}

void OpenTherm::begin(std::function<void(unsigned long, OpenThermResponseStatus)> processResponseFunction)
{
    begin();
    this->processResponseFunction = processResponseFunction;
}
#endif

bool IRAM_ATTR OpenTherm::isReady()
{
    return status == OpenThermStatus::READY;
}

int IRAM_ATTR OpenTherm::readState()
{
    return digitalRead(inPin);
}

void OpenTherm::setActiveState()
{
    digitalWrite(outPin, LOW);
}

void OpenTherm::setIdleState()
{
    digitalWrite(outPin, HIGH);
}

void OpenTherm::activateBoiler()
{
    setIdleState();
    delay(1000);
}

void OpenTherm::sendBit(bool high)
{
    if (high)
        setActiveState();
    else
        setIdleState();
    delayMicroseconds(500);
    if (high)
        setIdleState();
    else
        setActiveState();
    delayMicroseconds(500);
}

bool OpenTherm::sendRequestAsync(unsigned long request)
{
    noInterrupts();
    const bool ready = isReady();

    if (!ready)
    {
        interrupts();
        return false;
    }

    status = OpenThermStatus::REQUEST_SENDING;
    response = 0;
    responseStatus = OpenThermResponseStatus::NONE;

#ifdef INC_FREERTOS_H
    BaseType_t schedulerState = xTaskGetSchedulerState();
    if (schedulerState == taskSCHEDULER_RUNNING)
    {
        vTaskSuspendAll();
    }
#endif

    interrupts();

    sendBit(HIGH); // start bit
    for (int i = 31; i >= 0; i--)
    {
        sendBit(bitRead(request, i));
    }
    sendBit(HIGH); // stop bit
    setIdleState();

    responseTimestamp = micros();
    status = OpenThermStatus::RESPONSE_WAITING;

#ifdef INC_FREERTOS_H
    if (schedulerState == taskSCHEDULER_RUNNING) {
        xTaskResumeAll();
    }
#endif

    return true;
}

unsigned long OpenTherm::sendRequest(unsigned long request)
{
    if (!sendRequestAsync(request))
    {
        return 0;
    }

    while (!isReady())
    {
        process();
        yield();
    }
    return response;
}

bool OpenTherm::sendResponse(unsigned long request)
{
    noInterrupts();
    const bool ready = isReady();

    if (!ready)
    {
        interrupts();
        return false;
    }

    status = OpenThermStatus::REQUEST_SENDING;
    response = 0;
    responseStatus = OpenThermResponseStatus::NONE;

#ifdef INC_FREERTOS_H
    BaseType_t schedulerState = xTaskGetSchedulerState();
    if (schedulerState == taskSCHEDULER_RUNNING)
    {
        vTaskSuspendAll();
    }
#endif

    interrupts();

    sendBit(HIGH); // start bit
    for (int i = 31; i >= 0; i--)
    {
        sendBit(bitRead(request, i));
    }
    sendBit(HIGH); // stop bit
    setIdleState();
    status = OpenThermStatus::READY;

#ifdef INC_FREERTOS_H
    if (schedulerState == taskSCHEDULER_RUNNING) {
        xTaskResumeAll();
    }
#endif

    return true;
}

unsigned long OpenTherm::getLastResponse()
{
    return response;
}

OpenThermResponseStatus OpenTherm::getLastResponseStatus()
{
    return responseStatus;
}

void IRAM_ATTR OpenTherm::handleInterrupt()
{
    if (isReady())
    {
        if (isSlave && readState() == HIGH)
        {
            status = OpenThermStatus::RESPONSE_WAITING;
        }
        else
        {
            return;
        }
    }

    unsigned long newTs = micros();
    if (status == OpenThermStatus::RESPONSE_WAITING)
    {
        if (readState() == HIGH)
        {
            status = OpenThermStatus::RESPONSE_START_BIT;
            responseTimestamp = newTs;
        }
        else
        {
            status = OpenThermStatus::RESPONSE_INVALID;
            responseTimestamp = newTs;
        }
    }
    else if (status == OpenThermStatus::RESPONSE_START_BIT)
    {
        if ((newTs - responseTimestamp < 750) && readState() == LOW)
        {
            status = OpenThermStatus::RESPONSE_RECEIVING;
            responseTimestamp = newTs;
            responseBitIndex = 0;
        }
        else
        {
            status = OpenThermStatus::RESPONSE_INVALID;
            responseTimestamp = newTs;
        }
    }
    else if (status == OpenThermStatus::RESPONSE_RECEIVING)
    {
        if ((newTs - responseTimestamp) > 750)
        {
            if (responseBitIndex < 32)
            {
                response = (response << 1) | !readState();
                responseTimestamp = newTs;
                responseBitIndex = responseBitIndex + 1;
            }
            else
            { // stop bit
                status = OpenThermStatus::RESPONSE_READY;
                responseTimestamp = newTs;
            }
        }
    }
}

#if !defined(__AVR__)
void IRAM_ATTR OpenTherm::handleInterruptHelper(void* ptr)
{
    static_cast<OpenTherm*>(ptr)->handleInterrupt();
}
#endif

void OpenTherm::processResponse()
{
    if (processResponseCallback != NULL)
    {
        processResponseCallback(response, (int)responseStatus);
    }
#if !defined(__AVR__)
    if (this->processResponseFunction != NULL)
    {
        processResponseFunction(response, responseStatus);
    }
#endif
}

void OpenTherm::process()
{
    noInterrupts();
    OpenThermStatus st = status;
    unsigned long ts = responseTimestamp;
    interrupts();

    if (st == OpenThermStatus::READY)
        return;
    unsigned long newTs = micros();
    if (st != OpenThermStatus::NOT_INITIALIZED && st != OpenThermStatus::DELAY && (newTs - ts) > 1000000)
    {
        status = OpenThermStatus::READY;
        responseStatus = OpenThermResponseStatus::TIMEOUT;
        processResponse();
    }
    else if (st == OpenThermStatus::RESPONSE_INVALID)
    {
        status = OpenThermStatus::DELAY;
        responseStatus = OpenThermResponseStatus::INVALID;
        processResponse();
    }
    else if (st == OpenThermStatus::RESPONSE_READY)
    {
        status = OpenThermStatus::DELAY;
        responseStatus = (isSlave ? isValidRequest(response) : isValidResponse(response)) ? OpenThermResponseStatus::SUCCESS : OpenThermResponseStatus::INVALID;
        processResponse();
    }
    else if (st == OpenThermStatus::DELAY)
    {
        if ((newTs - ts) > (isSlave ? 20000 : 100000))
        {
            status = OpenThermStatus::READY;
        }
    }
}

bool OpenTherm::parity(unsigned long frame) // odd parity
{
    byte p = 0;
    while (frame > 0)
    {
        if (frame & 1)
            p++;
        frame = frame >> 1;
    }
    return (p & 1);
}

OpenThermMessageType OpenTherm::getMessageType(unsigned long message)
{
    OpenThermMessageType msg_type = static_cast<OpenThermMessageType>((message >> 28) & 7);
    return msg_type;
}

OpenThermMessageID OpenTherm::getDataID(unsigned long frame)
{
    return (OpenThermMessageID)((frame >> 16) & 0xFF);
}

unsigned long OpenTherm::buildRequest(OpenThermMessageType type, OpenThermMessageID id, unsigned int data)
{
    unsigned long request = data;
    if (type == OpenThermMessageType::WRITE_DATA)
    {
        request |= 1ul << 28;
    }
    request |= ((unsigned long)id) << 16;
    if (parity(request))
        request |= (1ul << 31);
    return request;
}

unsigned long OpenTherm::buildResponse(OpenThermMessageType type, OpenThermMessageID id, unsigned int data)
{
    unsigned long response = data;
    response |= ((unsigned long)type) << 28;
    response |= ((unsigned long)id) << 16;
    if (parity(response))
        response |= (1ul << 31);
    return response;
}

bool OpenTherm::isValidResponse(unsigned long response)
{
    if (parity(response))
        return false;
    byte msgType = (response << 1) >> 29;
    return msgType == (byte)OpenThermMessageType::READ_ACK || msgType == (byte)OpenThermMessageType::WRITE_ACK;
}

bool OpenTherm::isValidRequest(unsigned long request)
{
    if (parity(request))
        return false;
    byte msgType = (request << 1) >> 29;
    return msgType == (byte)OpenThermMessageType::READ_DATA || msgType == (byte)OpenThermMessageType::WRITE_DATA;
}

void OpenTherm::end()
{
    detachInterrupt(digitalPinToInterrupt(inPin));
}

OpenTherm::~OpenTherm()
{
    end();
}

const char *OpenTherm::statusToString(OpenThermResponseStatus status)
{
    switch (status)
    {
    case OpenThermResponseStatus::NONE:
        return "NONE";
    case OpenThermResponseStatus::SUCCESS:
        return "SUCCESS";
    case OpenThermResponseStatus::INVALID:
        return "INVALID";
    case OpenThermResponseStatus::TIMEOUT:
        return "TIMEOUT";
    default:
        return "UNKNOWN";
    }
}

const char *OpenTherm::messageTypeToString(OpenThermMessageType message_type)
{
    switch (message_type)
    {
    case OpenThermMessageType::READ_DATA:
        return "READ_DATA";
    case OpenThermMessageType::WRITE_DATA:
        return "WRITE_DATA";
    case OpenThermMessageType::INVALID_DATA:
        return "INVALID_DATA";
    case OpenThermMessageType::RESERVED:
        return "RESERVED";
    case OpenThermMessageType::READ_ACK:
        return "READ_ACK";
    case OpenThermMessageType::WRITE_ACK:
        return "WRITE_ACK";
    case OpenThermMessageType::DATA_INVALID:
        return "DATA_INVALID";
    case OpenThermMessageType::UNKNOWN_DATA_ID:
        return "UNKNOWN_DATA_ID";
    default:
        return "UNKNOWN";
    }
}

// building requests

unsigned long OpenTherm::buildSetBoilerStatusRequest(bool enableCentralHeating, bool enableHotWater, bool enableCooling, bool enableOutsideTemperatureCompensation, bool enableCentralHeating2)
{
    unsigned int data = enableCentralHeating | (enableHotWater << 1) | (enableCooling << 2) | (enableOutsideTemperatureCompensation << 3) | (enableCentralHeating2 << 4);
    data <<= 8;
    return buildRequest(OpenThermMessageType::READ_DATA, OpenThermMessageID::Status, data);
}

unsigned long OpenTherm::buildSetBoilerTemperatureRequest(float temperature)
{
    unsigned int data = temperatureToData(temperature);
    return buildRequest(OpenThermMessageType::WRITE_DATA, OpenThermMessageID::TSet, data);
}

unsigned long OpenTherm::buildGetBoilerTemperatureRequest()
{
    return buildRequest(OpenThermMessageType::READ_DATA, OpenThermMessageID::Tboiler, 0);
}

// parsing responses
bool OpenTherm::isFault(unsigned long response)
{
    return response & 0x1;
}

bool OpenTherm::isCentralHeatingActive(unsigned long response)
{
    return response & 0x2;
}

bool OpenTherm::isHotWaterActive(unsigned long response)
{
    return response & 0x4;
}

bool OpenTherm::isFlameOn(unsigned long response)
{
    return response & 0x8;
}

bool OpenTherm::isCoolingActive(unsigned long response)
{
    return response & 0x10;
}

bool OpenTherm::isDiagnostic(unsigned long response)
{
    return response & 0x40;
}

uint16_t OpenTherm::getUInt(const unsigned long response)
{
    const uint16_t u88 = response & 0xffff;
    return u88;
}

float OpenTherm::getFloat(const unsigned long response)
{
    const uint16_t u88 = getUInt(response);
    const float f = (u88 & 0x8000) ? -(0x10000L - u88) / 256.0f : u88 / 256.0f;
    return f;
}

unsigned int OpenTherm::temperatureToData(float temperature)
{
    if (temperature < 0)
        temperature = 0;
    if (temperature > 100)
        temperature = 100;
    unsigned int data = (unsigned int)(temperature * 256);
    return data;
}

// basic requests

unsigned long OpenTherm::setBoilerStatus(bool enableCentralHeating, bool enableHotWater, bool enableCooling, bool enableOutsideTemperatureCompensation, bool enableCentralHeating2)
{
    return sendRequest(buildSetBoilerStatusRequest(enableCentralHeating, enableHotWater, enableCooling, enableOutsideTemperatureCompensation, enableCentralHeating2));
}

bool OpenTherm::setBoilerTemperature(float temperature)
{
    unsigned long response = sendRequest(buildSetBoilerTemperatureRequest(temperature));
    return isValidResponse(response);
}

float OpenTherm::getBoilerTemperature()
{
    unsigned long response = sendRequest(buildGetBoilerTemperatureRequest());
    return isValidResponse(response) ? getFloat(response) : 0;
}

float OpenTherm::getReturnTemperature()
{
    unsigned long response = sendRequest(buildRequest(OpenThermRequestType::READ, OpenThermMessageID::Tret, 0));
    return isValidResponse(response) ? getFloat(response) : 0;
}

bool OpenTherm::setDHWSetpoint(float temperature)
{
    unsigned int data = temperatureToData(temperature);
    unsigned long response = sendRequest(buildRequest(OpenThermMessageType::WRITE_DATA, OpenThermMessageID::TdhwSet, data));
    return isValidResponse(response);
}

float OpenTherm::getDHWTemperature()
{
    unsigned long response = sendRequest(buildRequest(OpenThermMessageType::READ_DATA, OpenThermMessageID::Tdhw, 0));
    return isValidResponse(response) ? getFloat(response) : 0;
}

float OpenTherm::getModulation()
{
    unsigned long response = sendRequest(buildRequest(OpenThermRequestType::READ, OpenThermMessageID::RelModLevel, 0));
    return isValidResponse(response) ? getFloat(response) : 0;
}

float OpenTherm::getPressure()
{
    unsigned long response = sendRequest(buildRequest(OpenThermRequestType::READ, OpenThermMessageID::CHPressure, 0));
    return isValidResponse(response) ? getFloat(response) : 0;
}

unsigned char OpenTherm::getFault()
{
    return ((sendRequest(buildRequest(OpenThermRequestType::READ, OpenThermMessageID::ASFflags, 0)) >> 8) & 0xff);
}
