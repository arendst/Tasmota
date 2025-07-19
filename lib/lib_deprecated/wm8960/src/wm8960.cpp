
#ifdef ESP32

#include <Wire.h>
#include <string.h>
#include "esp_log.h"
#include "wm8960.h"

static TwoWire *ws8960wire;


void W8960_Write(uint8_t reg_addr, uint16_t data) {
  reg_addr <<= 1;
  reg_addr |=  ((data >> 8) & 1);
  data &= 0xff;
  ws8960wire->beginTransmission(W8960_ADDR);
  ws8960wire->write(reg_addr);
  ws8960wire->write(data);
  ws8960wire->endTransmission();
}

void W8960_Init(TwoWire *tw) {

ws8960wire = tw;

    // reset
    W8960_Write(0x0f, 0x0000);
    delay(10);

    // enable dac and adc
    W8960_Write(0x19, (1<<8)|(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1));
    // left speaker not used
    W8960_Write(0x1A, (1<<8)|(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3));
    W8960_Write(0x2F, (1<<5)|(1<<4)|(1<<3)|(1<<2));

    // Configure clock
    W8960_Write(0x04, 0x0000);

    // Configure ADC/DAC
    W8960_Write(0x05, 0x0000);

    // Configure audio interface
    // I2S format 16 bits word length
    //W8960_Write(0x07 0x0002)
    W8960_Write(0x07, 0x0022);

    // Configure HP_L and HP_R OUTPUTS
    W8960_Write(0x02, 0x017f);
    W8960_Write(0x03, 0x017f);

    // Configure SPK_RP and SPK_RN
    W8960_Write(0x28, 0x0177);
    W8960_Write(0x29, 0x0177);

    // Enable the OUTPUTS, only right speaker is wired
    W8960_Write(0x31, 0x0080);

    // Configure DAC volume
    W8960_Write(0x0a, 0x01FF);
    W8960_Write(0x0b, 0x01FF);

    // Configure MIXER
    W8960_Write(0x22, (1<<8)|(1<<7));
    W8960_Write(0x25, (1<<8)|(1<<7));

    // Jack Detect
    //W8960_Write(0x18, (1<<6)|(0<<5));
    //W8960_Write(0x17, 0x01C3);
    //W8960_Write(0x30, 0x0009);

    //  input volume
    W8960_Write(0x00, 0x0127);
    W8960_Write(0x01, 0x0127);

    //  set ADC Volume
    W8960_Write(0x15, 0x01c3);
    W8960_Write(0x16, 0x01c3);

    // disable bypass switch
    W8960_Write(0x2d, 0x0000);
    W8960_Write(0x2e, 0x0000);

    // connect LINPUT1 to PGA and set PGA Boost Gain.
    W8960_Write(0x20, 0x0020|(1<<8)|(1<<3));
    W8960_Write(0x21, 0x0020|(1<<8)|(1<<3));

}

void W8960_SetGain(uint8_t sel, uint16_t value) {
  switch (sel) {
    case 0:
      // output dac in 0.5 db steps
      value &= 0x00ff;
      value |= 0x0100;
      W8960_Write(0x0a, value);
      W8960_Write(0x0b, value);
      break;
    case 1:
      // input pga in 0.75 db steps
      value &= 0x001f;
      value |= 0x0120;
      W8960_Write(0x00, value);
      W8960_Write(0x01, value);
      break;
    case 2:
      // adc in 0.5 db steps
      value &= 0x00ff;
      value |= 0x0100;
      W8960_Write(0x15, value);
      W8960_Write(0x16, value);
      break;
    case 3:
      // audio interface
      W8960_Write(0x07, value);
      break;
  }
}
#endif // ESP32
