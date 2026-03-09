#include <Arduino.h>
#include <stdint.h>
#include "GT911.h"

#undef log_d
#define log_d
#undef log_e
#define log_e

#ifdef ESP8266
#define ESP_OK 0
#define ESP_FAIL -1
#endif

//#define log_d Serial.printf

GT911::GT911() {}

volatile uint8_t gt911_irq_trigger = 0;
void ICACHE_RAM_ATTR ___GT911IRQ___()
{
    noInterrupts();
    gt911_irq_trigger = 1;
    interrupts();
}

int32_t GT911::begin(TwoWire *use_wire, int8_t pin_int, int8_t pin_res, uint16_t xs, uint16_t ys)
{
    log_d("GT911: Initialization");

    if (pin_int >= 0) {
      pinMode(pin_int, INPUT); // Startup sequence PIN part
    }
    if (pin_res >= 0) {
      pinMode(pin_res, OUTPUT); // Startup sequence PIN part
      digitalWrite(pin_res, 0);
      delay(1);
      digitalWrite(pin_res, 1);
    }
    delay(100);
    wire = use_wire;

    wire->beginTransmission(0x14);
    if (wire->endTransmission())
    {
        wire->beginTransmission(0x5D);
        if (wire->endTransmission())
        {
            log_e("Touch screen IIC connection error");
            return ESP_FAIL;
        }
        _iic_addr = 0x5D;
    }

    if (pin_int >= 0) {
      attachInterrupt(pin_int, ___GT911IRQ___, FALLING);
    }

    readBlockData(configBuf, GT911_CONFIG_START, GT911_CONFIG_SIZE);

    uint16_t curx = configBuf[GT911_X_OUTPUT_MAX_LOW - GT911_CONFIG_START] | (configBuf[GT911_X_OUTPUT_MAX_HIGH - GT911_CONFIG_START] << 8);
    uint16_t cury = configBuf[GT911_Y_OUTPUT_MAX_LOW - GT911_CONFIG_START] | (configBuf[GT911_Y_OUTPUT_MAX_HIGH - GT911_CONFIG_START] << 8);

    if (curx != xs || cury != ys) {
      setResolution(xs, ys);
    }

    log_d("GT911: initialized");

    return ESP_OK;
}


void GT911::write(uint16_t addr, uint8_t data)
{
    wire->beginTransmission(_iic_addr);
    wire->write((uint8_t)(addr >> 8));
    wire->write((uint8_t)addr);
    wire->write(data);
    wire->endTransmission(true);
}

void GT911::write(uint16_t addr, const uint8_t *data, uint16_t len)
{
    wire->beginTransmission(_iic_addr);
    wire->write((uint8_t)(addr >> 8));
    wire->write((uint8_t)addr);
    wire->write(data, len);
    wire->endTransmission(true);
}

uint8_t GT911::read(uint16_t addr)
{
    wire->flush();
    wire->beginTransmission(_iic_addr);
    wire->write((uint8_t)(addr >> 8));
    wire->write((uint8_t)addr);
    wire->endTransmission(false);
    wire->requestFrom((uint8_t)_iic_addr, (uint8_t)1);
    return wire->read();
}

void GT911::read(uint16_t addr, uint8_t *buf, uint16_t len)
{
    wire->flush();
    wire->beginTransmission(_iic_addr);
    wire->write((uint8_t)(addr >> 8));
    wire->write((uint8_t)addr);
    wire->endTransmission(false);
    wire->requestFrom((int)_iic_addr, (int)len);
    wire->readBytes(buf, len);
}

void GT911::readBlockData(uint8_t *buf, uint16_t reg, uint8_t size) {
  wire->beginTransmission(_iic_addr);
  wire->write(highByte(reg));
  wire->write(lowByte(reg));
  wire->endTransmission();
  wire->requestFrom(_iic_addr, size);
  for (uint8_t i = 0; i < size; i++) {
    buf[i] = wire->read();
  }
}


void GT911::calculateChecksum() {
  uint8_t checksum = 0;
  for (uint8_t i = 0; i < GT911_CONFIG_SIZE - 1 ; i++) {
    checksum += configBuf[i];
  }
  checksum = (~checksum) + 1;
  configBuf[GT911_CONFIG_CHKSUM - GT911_CONFIG_START] = checksum;
}

void GT911::reflashConfig() {
  calculateChecksum();
  write(GT911_X_OUTPUT_MAX_LOW, configBuf[GT911_X_OUTPUT_MAX_LOW - GT911_CONFIG_START]);
  write(GT911_X_OUTPUT_MAX_HIGH, configBuf[GT911_X_OUTPUT_MAX_HIGH - GT911_CONFIG_START]);
  write(GT911_Y_OUTPUT_MAX_LOW, configBuf[GT911_Y_OUTPUT_MAX_LOW - GT911_CONFIG_START]);
  write(GT911_Y_OUTPUT_MAX_HIGH, configBuf[GT911_Y_OUTPUT_MAX_HIGH - GT911_CONFIG_START]);
  write(GT911_CONFIG_CHKSUM, configBuf[GT911_CONFIG_CHKSUM - GT911_CONFIG_START]);
  write(GT911_CONFIG_FRESH, 1);
}

void GT911::setResolution(uint16_t _width, uint16_t _height) {
  configBuf[GT911_X_OUTPUT_MAX_LOW - GT911_CONFIG_START] = lowByte(_width);
  configBuf[GT911_X_OUTPUT_MAX_HIGH - GT911_CONFIG_START] = highByte(_width);
  configBuf[GT911_Y_OUTPUT_MAX_LOW - GT911_CONFIG_START] = lowByte(_height);
  configBuf[GT911_Y_OUTPUT_MAX_HIGH - GT911_CONFIG_START] = highByte(_height);
  reflashConfig();
}

bool GT911::avaliable()
{
    if(gt911_irq_trigger == 1)
    {
        gt911_irq_trigger = 0;
        return true;
    }
    return false;
}

void GT911::flush(void)
{
    write(0x814E, 0x00);
    gt911_irq_trigger = 0;
    _num = 0;
    _is_finger_up = 0;
}

void GT911::update()
{
    uint8_t r814e = read(0x814E);
    uint8_t num = r814e & 0x0F;
    if(r814e & 0x80)
    {
        if(num != 0)
        {
            _is_finger_up = false;
            _num = num;
            uint8_t data[num * 8];
            read(0x8150, data, num * 8);
            for(int j = 0; j < num; j++)
            {
                uint8_t *buf = data + j * 8;

                if(_rotate == ROTATE_0)
                {
                    _fingers[j].x = (buf[3] << 8) | buf[2];
                    _fingers[j].y = 540 - ((buf[1] << 8) | buf[0]);
                }
                else if(_rotate == ROTATE_180)
                {
                    _fingers[j].x = 960 - ((buf[3] << 8) | buf[2]);
                    _fingers[j].y = (buf[1] << 8) | buf[0];
                }
                else if(_rotate == ROTATE_270)
                {
                    _fingers[j].x = 540 - ((buf[1] << 8) | buf[0]);
                    _fingers[j].y = 960 - ((buf[3] << 8) | buf[2]);
                }
                else
                {
                    _fingers[j].x = (buf[1] << 8) | buf[0];
                    _fingers[j].y = (buf[3] << 8) | buf[2];
                }

                _fingers[j].size = (buf[5] << 8) | buf[4];
                _fingers[j].id = buf[7];
            }
        }
        else
        {
            _is_finger_up = true;
        }
        write(0x814E, 0x00);
    }
    else
    {
        _is_finger_up = 1;
    }
}

bool GT911::isFingerUp(void)
{
    if(_is_finger_up == 1)
    {
        _is_finger_up = 0;
        return true;
    }
    return false;
}

void GT911::SetRotation(uint16_t rotate)
{
    if(rotate < 4)
    {
        this->_rotate = rotate;
    }
    else if(rotate < 90)
    {
        this->_rotate = ROTATE_0;
    }
    else if(rotate < 180)
    {
        this->_rotate = ROTATE_90;
    }
    else if(rotate < 270)
    {
        this->_rotate = ROTATE_180;
    }
    else
    {
        this->_rotate = ROTATE_270;
    }
}

tp_finger_t GT911::readFinger(uint8_t num)
{
    if(num > 2)
    {
        num = 1;
    }
    return this->_fingers[num];
}

uint16_t GT911::readFingerID(uint8_t num)
{
    return this->_fingers[num].id;
}

uint16_t GT911::readFingerSize(uint8_t num)
{
    return this->_fingers[num].size;
}

uint16_t GT911::readFingerX(uint8_t num)
{
    return this->_fingers[num].x;
}

uint16_t GT911::readFingerY(uint8_t num)
{
    return this->_fingers[num].y;
}

uint8_t GT911::getFingerNum(void)
{
    return _num;
}
