/*
 * OpenTherm Library
 * Original Author: Ihor Melnyk (https://github.com/ihormelnyk/opentherm_library)
 * 
 * Copyright (c) 2019 Ihor Melnyk
 * MIT License
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 *
 * ---------------------------------------------
 * Modifications and improvements by Alex Pavlov
 * Copyright (c) 2025 Alex Pavlov
 *
 * Description of changes:
 * - Added support for ESP32 RMT peripheral for OpenTherm communication in Arduino environment.
 *   (https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/rmt.html)
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
#ifdef OPENTHERM_USE_RMT
    // RMT owns both pins; a GPIO interrupt on the RX pin would race with it
    // and corrupt the state machine, so it is intentionally not attached.
    (void)handleInterruptCallback;
    if (!setupRMT())
    {
        log_e("OpenTherm: RMT init failed (rx=%d tx=%d)", inPin, outPin);
        releaseRMT();
        return; // stays NOT_INITIALIZED
    }
#else
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
#endif

    activateBoiler();
    status = OpenThermStatus::READY;
#ifdef OPENTHERM_USE_RMT
    if (isSlave)
    {
        startReceiveRMT();
    }
#endif
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

// Bus "active" level on the TX pin is LOW (Ihor Melnyk adapter), idle is HIGH
#define OT_TX_ACTIVE_LEVEL 0
#define OT_TX_IDLE_LEVEL (!OT_TX_ACTIVE_LEVEL)

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
#ifndef OPENTHERM_USE_RMT
    setIdleState(); // with RMT the TX channel already holds the idle level
#endif
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
#ifdef OPENTHERM_USE_RMT
    if (!isReady()) {
        return false;
    }

    status = OpenThermStatus::REQUEST_SENDING;
    response = 0;
    responseStatus = OpenThermResponseStatus::NONE;

    rmtGlitches = 0;
    rmtLastCount = 0;
    if (!sendRMT(request))
    {
        status = OpenThermStatus::READY;
        return false;
    }
    // Arm RX only after TX is finished so an adapter echo of our own frame
    // is not captured; the slave answers no earlier than 20 ms later.
    startReceiveRMT();
    responseTimestamp = micros();
    status = OpenThermStatus::RESPONSE_WAITING;
#else
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
#ifdef OPENTHERM_USE_RMT
    stopReceiveRMT();
    const bool sent = sendRMT(request);
    status = OpenThermStatus::READY;
    startReceiveRMT(); // keep listening for the next request
    return sent;
#else
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
#endif
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
#ifdef OPENTHERM_USE_RMT
    const OpenThermStatus st = status;
    if (st == OpenThermStatus::NOT_INITIALIZED)
        return;

    const unsigned long newTs = micros();

    if (isSlave)
    {
        // Slave: listen while READY, hand a received request to the callback,
        // which is expected to call sendResponse().
        if (st == OpenThermStatus::READY && rmtRxDone)
        {
            rmtRxDone = false;
            uint32_t frame = 0;
            if (decodeRMT(rmtRxBuf, rmtRxCount, frame))
            {
                response = frame;
                responseStatus = isValidRequest(frame) ? OpenThermResponseStatus::SUCCESS : OpenThermResponseStatus::INVALID;
                processResponse();
            }
            if (!rmtRxArmed && status == OpenThermStatus::READY)
                startReceiveRMT();
        }
        return;
    }

    if (st == OpenThermStatus::RESPONSE_WAITING)
    {
        if (rmtRxDone)
        {
            rmtRxDone = false;
            const size_t count = rmtRxCount;
            if (count < 8)
            {
                // Too short to be a frame: a glitch on the line. Keep waiting.
                if (rmtGlitches < 0xFFFF)
                    rmtGlitches++;
                startReceiveRMT();
                return;
            }
            // rmtRxBuf stays untouched until the next request, so it can be dumped
            rmtLastCount = count;
            uint32_t frame = 0;
            const bool decoded = decodeRMT(rmtRxBuf, count, frame);
            response = frame;
            responseStatus = (decoded && isValidResponse(frame)) ? OpenThermResponseStatus::SUCCESS : OpenThermResponseStatus::INVALID;
            status = OpenThermStatus::DELAY;
            responseTimestamp = newTs;
            processResponse();
        }
        else if ((newTs - responseTimestamp) > 1000000)
        {
            stopReceiveRMT();
            responseStatus = OpenThermResponseStatus::TIMEOUT;
            status = OpenThermStatus::DELAY;
            responseTimestamp = newTs;
            processResponse();
        }
    }
    else if (st == OpenThermStatus::DELAY)
    {
        // Mandatory pause between master requests (>= 100 ms per spec)
        if ((newTs - responseTimestamp) > 100000)
        {
            status = OpenThermStatus::READY;
        }
    }
#else
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
#endif
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

size_t OpenTherm::dumpLastRx(char *buf, size_t len)
{
    if (!buf || len == 0)
        return 0;
    buf[0] = '\0';
#ifdef OPENTHERM_USE_RMT
    size_t pos = 0;
    for (size_t i = 0; i < rmtLastCount && i < RMT_RX_SYMBOLS; ++i)
    {
        const uint32_t dur[2] = {rmtRxBuf[i].duration0, rmtRxBuf[i].duration1};
        const uint32_t lvl[2] = {rmtRxBuf[i].level0, rmtRxBuf[i].level1};
        for (int k = 0; k < 2; ++k)
        {
            if (dur[k] == 0)
                continue; // end marker
            const int w = snprintf(buf + pos, len - pos, "%s%c%u", pos ? " " : "", lvl[k] ? 'H' : 'L', (unsigned)dur[k]);
            if (w < 0 || (size_t)w >= len - pos)
            {
                buf[pos] = '\0'; // truncated: drop the partial entry
                return pos;
            }
            pos += w;
        }
    }
    return pos;
#else
    return 0;
#endif
}

uint16_t OpenTherm::getRxGlitchCount()
{
#ifdef OPENTHERM_USE_RMT
    return rmtGlitches;
#else
    return 0;
#endif
}

void OpenTherm::end()
{
#ifdef OPENTHERM_USE_RMT
    releaseRMT();
    status = OpenThermStatus::NOT_INITIALIZED;
#else
    detachInterrupt(digitalPinToInterrupt(inPin));
#endif
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

#ifdef OPENTHERM_USE_RMT
// ---------------------------------------------------------------------------
// ESP32 RMT implementation (ESP-IDF 5.x "new" RMT driver: driver/rmt_tx.h, rmt_rx.h)
//
// Timing: channel resolution 1 MHz -> 1 tick = 1 us.
// OpenTherm frame: start bit '1', 32 data bits (MSB first), stop bit '1'.
// Manchester: '1' = active half then idle half, '0' = idle then active.
// ---------------------------------------------------------------------------

static constexpr uint32_t OT_RMT_RESOLUTION_HZ = 1000000;
static constexpr uint16_t OT_HALF_BIT_US = 500;
static constexpr int OT_FRAME_BITS = 34;

// Accepted pulse widths at the receiver (spec allows +-10% bit rate and edge jitter)
static constexpr uint32_t OT_RX_HALF_MIN = 300;
static constexpr uint32_t OT_RX_HALF_MAX = 700;
static constexpr uint32_t OT_RX_FULL_MIN = 750;
static constexpr uint32_t OT_RX_FULL_MAX = 1250;

bool OpenTherm::setupRMT()
{
    rmt_tx_channel_config_t txCfg = {};
    txCfg.gpio_num = (gpio_num_t)outPin;
    txCfg.clk_src = RMT_CLK_SRC_DEFAULT;
    txCfg.resolution_hz = OT_RMT_RESOLUTION_HZ;
    txCfg.mem_block_symbols = SOC_RMT_MEM_WORDS_PER_CHANNEL; // 64 (48 on C3/S3/C6) >= 34 symbols
    txCfg.trans_queue_depth = 2;
    txCfg.flags.init_level = OT_TX_IDLE_LEVEL;
    if (rmt_new_tx_channel(&txCfg, &rmtTx) != ESP_OK)
        return false;

    rmt_copy_encoder_config_t encCfg = {};
    if (rmt_new_copy_encoder(&encCfg, &rmtEncoder) != ESP_OK)
        return false;

    rmt_rx_channel_config_t rxCfg = {};
    rxCfg.gpio_num = (gpio_num_t)inPin;
    rxCfg.clk_src = RMT_CLK_SRC_DEFAULT;
    rxCfg.resolution_hz = OT_RMT_RESOLUTION_HZ;
    rxCfg.mem_block_symbols = SOC_RMT_MEM_WORDS_PER_CHANNEL;
    if (rmt_new_rx_channel(&rxCfg, &rmtRx) != ESP_OK)
        return false;

    rmt_rx_event_callbacks_t cbs = {};
    cbs.on_recv_done = rmtRxDoneCallback;
    if (rmt_rx_register_event_callbacks(rmtRx, &cbs, this) != ESP_OK)
        return false;

    if (rmt_enable(rmtTx) != ESP_OK)
        return false;
    if (rmt_enable(rmtRx) != ESP_OK)
        return false;
    return true;
}

void OpenTherm::releaseRMT()
{
    if (rmtRx)
    {
        rmt_disable(rmtRx);
        rmt_del_channel(rmtRx);
        rmtRx = nullptr;
    }
    if (rmtTx)
    {
        rmt_disable(rmtTx);
        rmt_del_channel(rmtTx);
        rmtTx = nullptr;
    }
    if (rmtEncoder)
    {
        rmt_del_encoder(rmtEncoder);
        rmtEncoder = nullptr;
    }
    rmtRxArmed = false;
    rmtRxDone = false;
}

bool IRAM_ATTR OpenTherm::rmtRxDoneCallback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata, void *ctx)
{
    (void)channel;
    OpenTherm *self = static_cast<OpenTherm *>(ctx);
    // Symbols are already in self->rmtRxBuf (the buffer passed to rmt_receive)
    self->rmtRxCount = edata->num_symbols;
    self->rmtRxArmed = false;
    self->rmtRxDone = true;
    return false;
}

void OpenTherm::startReceiveRMT()
{
    if (!rmtRx || rmtRxArmed)
        return;

    rmt_receive_config_t rxCfg = {};
    // Glitch filter is limited by an 8-bit register in source-clock ticks
    // (255 / 80 MHz ~= 3.2 us), so it can only suppress very short spikes.
    rxCfg.signal_range_min_ns = 3000;
    // A level held longer than the longest valid pulse (1 ms) ends the frame.
    rxCfg.signal_range_max_ns = 1500 * 1000;

    rmtRxDone = false;
    rmtRxCount = 0;
    rmtRxArmed = true;
    if (rmt_receive(rmtRx, rmtRxBuf, sizeof(rmtRxBuf), &rxCfg) != ESP_OK)
    {
        rmtRxArmed = false;
    }
}

void OpenTherm::stopReceiveRMT()
{
    if (!rmtRx || !rmtRxArmed)
        return;
    // Disabling the channel aborts the pending receive transaction
    rmt_disable(rmtRx);
    rmt_enable(rmtRx);
    rmtRxArmed = false;
    rmtRxDone = false;
}

bool OpenTherm::sendRMT(uint32_t data)
{
    if (!rmtTx || !rmtEncoder)
        return false;

    rmt_symbol_word_t symbols[OT_FRAME_BITS];
    for (int i = 0; i < OT_FRAME_BITS; ++i)
    {
        bool bit;
        if (i == 0 || i == OT_FRAME_BITS - 1)
            bit = true; // start and stop bits
        else
            bit = (data >> (OT_FRAME_BITS - 2 - i)) & 1;

        symbols[i].level0 = bit ? OT_TX_ACTIVE_LEVEL : OT_TX_IDLE_LEVEL;
        symbols[i].duration0 = OT_HALF_BIT_US;
        symbols[i].level1 = bit ? OT_TX_IDLE_LEVEL : OT_TX_ACTIVE_LEVEL;
        symbols[i].duration1 = OT_HALF_BIT_US;
    }

    rmt_transmit_config_t txCfg = {};
    txCfg.loop_count = 0;
    txCfg.flags.eot_level = OT_TX_IDLE_LEVEL;
    if (rmt_transmit(rmtTx, rmtEncoder, symbols, sizeof(symbols), &txCfg) != ESP_OK)
        return false;
    // Frame lasts 34 ms; symbols[] lives on the stack, so wait for completion.
    return rmt_tx_wait_all_done(rmtTx, 100) == ESP_OK;
}

// Convert captured RMT symbols into a 32-bit OpenTherm frame.
// On the RX pin the bus "active" level is HIGH (as in the interrupt version), so the
// value of a Manchester bit equals the level of its first half.
bool OpenTherm::decodeRMT(const rmt_symbol_word_t *symbols, size_t count, uint32_t &frame)
{
    constexpr int HALVES = OT_FRAME_BITS * 2;
    uint8_t halves[HALVES];
    int n = 0;
    bool ended = false;

    for (size_t i = 0; i < count && !ended; ++i)
    {
        const uint32_t dur[2] = {symbols[i].duration0, symbols[i].duration1};
        const uint8_t lvl[2] = {(uint8_t)symbols[i].level0, (uint8_t)symbols[i].level1};
        for (int k = 0; k < 2; ++k)
        {
            int units;
            if (n == 0 && (dur[k] > OT_RX_FULL_MAX || lvl[k] == 0))
                continue; // leading idle before the start bit, if captured
            if (dur[k] >= OT_RX_HALF_MIN && dur[k] <= OT_RX_HALF_MAX)
                units = 1;
            else if (dur[k] >= OT_RX_FULL_MIN && dur[k] <= OT_RX_FULL_MAX)
                units = 2;
            else if (n >= HALVES - 1)
            {
                // Zero-length end marker or the trailing idle that ended the
                // capture: the frame is already complete.
                ended = true;
                break;
            }
            else
                return false;

            for (int u = 0; u < units; ++u)
            {
                if (n >= HALVES)
                {
                    ended = true;
                    break;
                }
                halves[n++] = lvl[k];
            }
            if (ended)
                break;
        }
    }

    // The idle half of the stop bit merges with the idle line after the frame
    // and is not captured as a separate pulse; restore it.
    if (n == HALVES - 1)
    {
        halves[n] = !halves[n - 1];
        n++;
    }
    if (n < HALVES)
        return false;

    uint32_t result = 0;
    for (int b = 0; b < OT_FRAME_BITS; ++b)
    {
        const uint8_t first = halves[2 * b];
        const uint8_t second = halves[2 * b + 1];
        if (first == second)
            return false; // no mid-bit transition: not Manchester
        const bool bit = first;
        if (b == 0 || b == OT_FRAME_BITS - 1)
        {
            if (!bit)
                return false; // start/stop bit must be '1'
        }
        else
        {
            result = (result << 1) | bit;
        }
    }
    frame = result;
    return true;
}

#endif
