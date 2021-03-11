#include "i2c_bus.h"
#include "Wire.h"
#include <Arduino.h>

void I2CBus::scan(void)
{
    uint8_t err, addr;
    int nDevices = 0;
    for (addr = 1; addr < 127; addr++) {
        _port->beginTransmission(addr);
        err = _port->endTransmission();
        if (err == 0) {
            Serial.print("I2C device found at address 0x");
            if (addr < 16)
                Serial.print("0");
            Serial.print(addr, HEX);
            Serial.println(" !");
            nDevices++;
        } else if (err == 4) {
            Serial.print("Unknow error at address 0x");
            if (addr < 16)
                Serial.print("0");
            Serial.println(addr, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
}


uint16_t I2CBus::readBytes(uint8_t addr, uint8_t *data, uint16_t len, uint16_t delay_ms)
{
    uint16_t ret = 0;
    xSemaphoreTakeRecursive(_i2c_mux, portMAX_DELAY);
    uint8_t cnt = _port->requestFrom(addr, (uint8_t)len, (uint8_t)1);
    if (!cnt) {
        ret =  1 << 13;
    }
    uint16_t index = 0;
    while (_port->available()) {
        if (index > len)return 1 << 14;
        if (delay_ms)delay(delay_ms);
        data[index++] = _port->read();
    }
    xSemaphoreGiveRecursive(_i2c_mux);
    return ret;
}


uint16_t I2CBus::readBytes(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len)
{
    uint16_t ret = 0;
    xSemaphoreTakeRecursive(_i2c_mux, portMAX_DELAY);
    _port->beginTransmission(addr);
    _port->write(reg);
    _port->endTransmission(false);
    uint8_t cnt = _port->requestFrom(addr, (uint8_t)len, (uint8_t)1);
    if (!cnt) {
        ret =  1 << 13;
    }
    uint16_t index = 0;
    while (_port->available()) {
        if (index > len)return 1 << 14;
        data[index++] = _port->read();
    }
    xSemaphoreGiveRecursive(_i2c_mux);
    return ret;
}

uint16_t I2CBus::writeBytes(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len)
{
    uint16_t ret = 0;
    xSemaphoreTakeRecursive(_i2c_mux, portMAX_DELAY);
    _port->beginTransmission(addr);
    _port->write(reg);
    for (uint16_t i = 0; i < len; i++) {
        _port->write(data[i]);
    }
    ret =  _port->endTransmission();
    xSemaphoreGiveRecursive(_i2c_mux);
    return ret ? 1 << 12 : ret;
}

bool I2CBus::deviceProbe(uint8_t addr)
{
    uint16_t ret = 0;
    xSemaphoreTakeRecursive(_i2c_mux, portMAX_DELAY);
    _port->beginTransmission(addr);
    ret = _port->endTransmission();
    xSemaphoreGiveRecursive(_i2c_mux);
    return (ret == 0);
}