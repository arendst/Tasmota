// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// (c) ladyada / adafruit
// Code under MIT License

#ifndef _ADAFRUIT_TOUCHSCREEN_H_
#define _ADAFRUIT_TOUCHSCREEN_H_
#include <stdint.h>

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__) ||              \
    defined(TEENSYDUINO) || defined(__AVR_ATmega2560__) ||                     \
    defined(__AVR_ATmega4809__)
typedef volatile uint8_t RwReg;
#elif defined(ARDUINO_STM32_FEATHER)
typedef volatile uint32 RwReg;
#elif defined(NRF52_SERIES) || defined(ESP32) || defined(ESP8266) ||           \
    defined(ARDUINO_ARCH_STM32)
typedef volatile uint32_t RwReg;
#else
typedef volatile uint32_t RwReg;
#endif

#if defined(__AVR__) || defined(TEENSYDUINO) || defined(ARDUINO_ARCH_SAMD)
#define USE_FAST_PINIO
#endif

/** Object that encapsulates the X,Y, and Z/pressure measurements for a touch
 * event. */
class TSPoint {
public:
  TSPoint(void);
  TSPoint(int16_t x, int16_t y, int16_t z);

  bool operator==(TSPoint);
  bool operator!=(TSPoint);

  int16_t x, ///< state variable for the x value
      y,     ///< state variable for the y value
      z;     ///< state variable for the z value
};
/** Object that controls and keeps state for a touch screen. */

class TouchScreen {
public:
  /**
   * @brief Construct a new Touch Screen object
   *
   * @param xp X+ pin. Must be an analog pin
   * @param yp Y+ pin. Must be an analog pin
   * @param xm X- pin. Can be a digital pin
   * @param ym Y- pin. Can be a digital pin
   * @param rx The resistance in ohms between X+ and X- to calibrate pressure
   * sensing
   */
  TouchScreen(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym, uint16_t rx);

  /**
   * @brief **NOT IMPLEMENTED** Test if the screen has been touched
   *
   * @return true : touch detected false: no touch detected
   */
  bool isTouching(void);
  uint16_t pressure(void);
  int readTouchY();
  int readTouchX();
  TSPoint getPoint();
  int16_t pressureThreshhold; ///< Pressure threshold for `isTouching`

private:
  uint8_t _yp, _ym, _xm, _xp;
  uint16_t _rxplate;

#if defined(USE_FAST_PINIO)
  volatile RwReg *xp_port, *yp_port, *xm_port, *ym_port;
  RwReg xp_pin, xm_pin, yp_pin, ym_pin;
#endif
};

#endif
