#ifndef _NEO_TRELLIS_H
#define _NEO_TRELLIS_H

#include <Arduino.h>
#include "Adafruit_seesaw.h"
#include "seesaw_neopixel.h"

#define NEO_TRELLIS_ADDR 0x2E

#define NEO_TRELLIS_NEOPIX_PIN 3

#define NEO_TRELLIS_NUM_ROWS 4
#define NEO_TRELLIS_NUM_COLS 4
#define NEO_TRELLIS_NUM_KEYS (NEO_TRELLIS_NUM_ROWS * NEO_TRELLIS_NUM_COLS)

#define NEO_TRELLIS_MAX_CALLBACKS 32

#define NEO_TRELLIS_KEY(x) (((x)/4)*8 + ((x)%4))
#define NEO_TRELLIS_SEESAW_KEY(x) (((x)/8)*4 + ((x)%8))

#define NEO_TRELLIS_X(k) ((k)%4)
#define NEO_TRELLIS_Y(k) ((k)/4)

#define NEO_TRELLIS_XY(x, y) ((y)*NEO_TRELLIS_NUM_ROWS + (x))

typedef void (*TrellisCallback)(keyEvent evt);

/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for interacting with the seesaw keypad module
*/
/**************************************************************************/
class Adafruit_NeoTrellis : public Adafruit_seesaw {
public:
	Adafruit_NeoTrellis(uint8_t addr = NEO_TRELLIS_ADDR);
	~Adafruit_NeoTrellis() {};

    bool begin(uint8_t addr = NEO_TRELLIS_ADDR, int8_t flow=-1);

    void registerCallback(uint8_t key, TrellisCallback (*cb)(keyEvent));
    void unregisterCallback(uint8_t key);

    void activateKey(uint8_t key, uint8_t edge, bool enable=true);

    void read(bool polling = true);

    seesaw_NeoPixel pixels; ///< the onboard neopixel matrix

    friend class Adafruit_MultiTrellis; ///< for allowing use of protected methods by aggregate class
    
protected:
    uint8_t _addr; ///< the I2C address of this board
    TrellisCallback (*_callbacks[NEO_TRELLIS_NUM_KEYS])(keyEvent); ///< the array of callback functions
};

/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for interacting with multiple neotrellis boards
*/
/**************************************************************************/
class Adafruit_MultiTrellis {
public:
    Adafruit_MultiTrellis(Adafruit_NeoTrellis *trelli, uint8_t rows, uint8_t cols);
    ~Adafruit_MultiTrellis() {};

    bool begin();

    void registerCallback(uint16_t num, TrellisCallback (*cb)(keyEvent));
    void registerCallback(uint8_t x, uint8_t y, TrellisCallback (*cb)(keyEvent));
    void unregisterCallback(uint16_t num);
    void unregisterCallback(uint8_t x, uint8_t y);

    void activateKey(uint16_t num, uint8_t edge, bool enable=true);
    void activateKey(uint8_t x, uint8_t y, uint8_t edge, bool enable=true);

    void setPixelColor(uint8_t x, uint8_t y, uint32_t color);
    void setPixelColor(uint16_t num, uint32_t color);
    void show();

    void read();

protected:
    uint8_t _rows; ///< the number of trellis boards in the Y direction
    uint8_t  _cols; ///< the number of trellis boards in the X direction
    Adafruit_NeoTrellis *_trelli; ///< a multidimensional array of neotrellis objects
};

#endif
