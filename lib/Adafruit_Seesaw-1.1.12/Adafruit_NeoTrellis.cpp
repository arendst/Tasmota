#include "Adafruit_NeoTrellis.h"

/**************************************************************************/
/*! 
    @brief  Class constructor
    @param  addr the I2C address this neotrellis object uses
*/
/**************************************************************************/
Adafruit_NeoTrellis::Adafruit_NeoTrellis(uint8_t addr) : pixels(NEO_TRELLIS_NUM_KEYS, NEO_TRELLIS_NEOPIX_PIN, NEO_GRB + NEO_KHZ800)
{
    for(int i=0; i<NEO_TRELLIS_NUM_KEYS; i++){
        _callbacks[i] = NULL;
    }
    this->_addr = addr;
}

/**************************************************************************/
/*! 
    @brief  Begin communication with the RGB trellis.
    @param  addr optional i2c address where the device can be found. Defaults to NEO_TRELLIS_ADDR
    @param  flow optional flow control pin
    @returns true on success, false on error.
*/
/**************************************************************************/
bool Adafruit_NeoTrellis::begin(uint8_t addr, int8_t flow)
{
    _addr = addr;
    
    bool ret = pixels.begin(addr, flow);
    
    ret = Adafruit_seesaw::begin(addr, flow, false);
    if(!ret) return ret;

    enableKeypadInterrupt();

    return ret;
}

/**************************************************************************/
/*! 
    @brief  register a callback function on the passed key.
    @param  key the key number to register the callback on
    @param  cb the callback function that should be called when an event on that key happens
*/
/**************************************************************************/
void Adafruit_NeoTrellis::registerCallback(uint8_t key, TrellisCallback (*cb)(keyEvent))
{
    _callbacks[key] = cb;
}

/**************************************************************************/
/*! 
    @brief  unregister a callback on a given key
    @param  key the key number the callback is currently mapped to.
*/
/**************************************************************************/
void Adafruit_NeoTrellis::unregisterCallback(uint8_t key)
{
    _callbacks[key] = NULL;
}

/**************************************************************************/
/*! 
    @brief  activate or deactivate a given key event
    @param  key the key number to map the event to
    @param  edge the edge sensitivity of the event
    @param  enable pass true to enable the passed event, false to disable it.
*/
/**************************************************************************/
void Adafruit_NeoTrellis::activateKey(uint8_t key, uint8_t edge, bool enable)
{
    setKeypadEvent(NEO_TRELLIS_KEY(key), edge, enable);
}

/**************************************************************************/
/*! 
    @brief  read all events currently stored in the seesaw fifo and call any callbacks.
    @param  polling pass true if the interrupt pin is not being used, false if it is. Defaults to true.
*/
/**************************************************************************/
void Adafruit_NeoTrellis::read(bool polling)
{
    uint8_t count = getKeypadCount();
    delayMicroseconds(500);
    if(count > 0){
        if(polling) count = count + 2;
        keyEventRaw e[count];
        readKeypad(e, count);
        for(int i=0; i<count; i++){
            //call any callbacks associated with the key
            e[i].bit.NUM = NEO_TRELLIS_SEESAW_KEY(e[i].bit.NUM);
            if(e[i].bit.NUM < NEO_TRELLIS_NUM_KEYS && _callbacks[e[i].bit.NUM] != NULL){
                keyEvent evt = { e[i].bit.EDGE, e[i].bit.NUM };
                _callbacks[e[i].bit.NUM](evt);
            }
        }
    }
}

/**************************************************************************/
/*! 
    @brief  class constructor
    @param  trelli pointer to a multidimensional array of neotrellis objects.
            these object must have their I2C addresses specified in the class
            constructors.
    @param  rows the number of individual neotrellis boards in the Y direction
            of your matrix.
    @param  cols the number of individual neotrellis boards in the X direction
            of your matrix.
*/
/**************************************************************************/
Adafruit_MultiTrellis::Adafruit_MultiTrellis(Adafruit_NeoTrellis *trelli, uint8_t rows, uint8_t cols)
{
    this->_rows = rows;
    this->_cols = cols;
    this->_trelli = trelli;
}

/**************************************************************************/
/*! 
    @brief  begin communication with the matrix of neotrellis boards.
    @returns true on success, false otherwise.
*/
/**************************************************************************/
bool Adafruit_MultiTrellis::begin()
{
    Adafruit_NeoTrellis *t;
    for(int n=0; n<_rows; n++){
        for(int m=0; m<_cols; m++){
            t = (_trelli+n*_cols) + m;
            if(!t->begin(t->_addr))
                return false;
        }
    }

    return true;
}

/**************************************************************************/
/*! 
    @brief  register a callback for a key addressed by key index. 
    @param  x the column index of the key. column 0 is on the lefthand side of the matix.
    @param  y the row index of the key. row 0 is at the top of the matrix and the numbers increase
            downwards.
    @param  cb the function to be called when an event from the specified key is
            detected.
*/
/**************************************************************************/
void Adafruit_MultiTrellis::registerCallback(uint8_t x, uint8_t y, TrellisCallback (*cb)(keyEvent))
{
    Adafruit_NeoTrellis *t = (_trelli+y/NEO_TRELLIS_NUM_ROWS*_cols) + x/NEO_TRELLIS_NUM_COLS;
    int xkey = NEO_TRELLIS_X(x);
    int ykey = NEO_TRELLIS_Y(y%NEO_TRELLIS_NUM_ROWS*NEO_TRELLIS_NUM_COLS);

    t->registerCallback(NEO_TRELLIS_XY(xkey, ykey), cb);
}

/**************************************************************************/
/*! 
    @brief  register a callback for a key addressed by key number. 
    @param  num the keynumber to set the color of. Key 0 is in the top left
            corner of the trellis matrix, key 1 is directly to the right of it,
            and the last key number is in the bottom righthand corner.
    @param  cb the function to be called when an event from the specified key is
            detected.
*/
/**************************************************************************/
void Adafruit_MultiTrellis::registerCallback(uint16_t num, TrellisCallback (*cb)(keyEvent)){
    uint8_t x = num % (NEO_TRELLIS_NUM_COLS*_cols);
    uint8_t y = num / (NEO_TRELLIS_NUM_COLS*_cols);

    registerCallback(x, y, cb);
}

/**************************************************************************/
/*! 
    @brief  Unregister a callback for a key addressed by key index. 
    @param  x the column index of the key. column 0 is on the lefthand side of the matix.
    @param  y the row index of the key. row 0 is at the top of the matrix and the numbers increase
            downwards.
*/
/**************************************************************************/
void Adafruit_MultiTrellis::unregisterCallback(uint8_t x, uint8_t y)
{
    Adafruit_NeoTrellis *t = (_trelli+y/NEO_TRELLIS_NUM_ROWS*_cols) + x/NEO_TRELLIS_NUM_COLS;
    int xkey = NEO_TRELLIS_X(x);
    int ykey = NEO_TRELLIS_Y(y%NEO_TRELLIS_NUM_ROWS*NEO_TRELLIS_NUM_COLS);

    t->unregisterCallback(NEO_TRELLIS_XY(xkey, ykey));
}

/**************************************************************************/
/*! 
    @brief  Unregister a callback for a key addressed by key number. 
    @param  num the keynumber to set the color of. Key 0 is in the top left
            corner of the trellis matrix, key 1 is directly to the right of it,
            and the last key number is in the bottom righthand corner.
*/
/**************************************************************************/
void Adafruit_MultiTrellis::unregisterCallback(uint16_t num){
    uint8_t x = num % (NEO_TRELLIS_NUM_COLS*_cols);
    uint8_t y = num / (NEO_TRELLIS_NUM_COLS*_cols);

    unregisterCallback(x, y);
}

/**************************************************************************/
/*! 
    @brief  Activate or deactivate a key by number. 
    @param  x the column index of the key. column 0 is on the lefthand side of the matix.
    @param  y the row index of the key. row 0 is at the top of the matrix and the numbers increase
            downwards.
    @param  edge the edge that the key triggers an event on. Use SEESAW_KEYPAD_EDGE_FALLING
            or SEESAW_KEYPAD_EDGE_RISING.
    @param  enable pass true to enable the key on the passed edge, false to disable.
*/
/**************************************************************************/
void Adafruit_MultiTrellis::activateKey(uint8_t x, uint8_t y, uint8_t edge, bool enable)
{
    Adafruit_NeoTrellis *t = (_trelli+y/NEO_TRELLIS_NUM_ROWS*_cols) + x/NEO_TRELLIS_NUM_COLS;
    int xkey = NEO_TRELLIS_X(x);
    int ykey = NEO_TRELLIS_Y(y%NEO_TRELLIS_NUM_ROWS*NEO_TRELLIS_NUM_COLS);

    t->activateKey(NEO_TRELLIS_XY(xkey, ykey), edge, enable);
}

/**************************************************************************/
/*! 
    @brief  Activate or deactivate a key by number. 
    @param  num the keynumber to set the color of. Key 0 is in the top left
            corner of the trellis matrix, key 1 is directly to the right of it,
            and the last key number is in the bottom righthand corner.
    @param  edge the edge that the key triggers an event on. Use SEESAW_KEYPAD_EDGE_FALLING
            or SEESAW_KEYPAD_EDGE_RISING.
    @param  enable pass true to enable the key on the passed edge, false to disable.
*/
/**************************************************************************/
void Adafruit_MultiTrellis::activateKey(uint16_t num, uint8_t edge, bool enable)
{
    uint8_t x = num % (NEO_TRELLIS_NUM_COLS*_cols);
    uint8_t y = num / (NEO_TRELLIS_NUM_COLS*_cols);

    activateKey(x, y, edge, enable);
}

/**************************************************************************/
/*! 
    @brief  set the color of a neopixel at a key index.
    @param  x the column index of the key. column 0 is on the lefthand side of the matix.
    @param  y the row index of the key. row 0 is at the top of the matrix and the numbers increase
            downwards.
    @param  color the color to set the pixel to. This is a 24 bit RGB value.
            for example, full brightness red would be 0xFF0000, and full brightness
            blue would be 0x0000FF.
*/
/**************************************************************************/
void Adafruit_MultiTrellis::setPixelColor(uint8_t x, uint8_t y, uint32_t color)
{
    Adafruit_NeoTrellis *t = (_trelli+y/NEO_TRELLIS_NUM_ROWS*_cols) + x/NEO_TRELLIS_NUM_COLS;
    int xkey = NEO_TRELLIS_X(x);
    int ykey = NEO_TRELLIS_Y(y%NEO_TRELLIS_NUM_ROWS*NEO_TRELLIS_NUM_COLS);

    t->pixels.setPixelColor(NEO_TRELLIS_XY(xkey, ykey), color);
}

/**************************************************************************/
/*! 
    @brief  set the color of a neopixel at a key number.
    @param  num the keynumber to set the color of. Key 0 is in the top left
            corner of the trellis matrix, key 1 is directly to the right of it,
            and the last key number is in the bottom righthand corner.
    @param  color the color to set the pixel to. This is a 24 bit RGB value.
            for example, full brightness red would be 0xFF0000, and full brightness
            blue would be 0x0000FF.
*/
/**************************************************************************/
void Adafruit_MultiTrellis::setPixelColor(uint16_t num, uint32_t color)
{
    uint8_t x = num % (NEO_TRELLIS_NUM_COLS*_cols);
    uint8_t y = num / (NEO_TRELLIS_NUM_COLS*_cols);

    setPixelColor(x, y, color);
}

/**************************************************************************/
/*! 
    @brief  call show for all connected neotrellis boards to show all neopixels
*/
/**************************************************************************/
void Adafruit_MultiTrellis::show()
{
    Adafruit_NeoTrellis *t;
    for(int n=0; n<_rows; n++){
        for(int m=0; m<_cols; m++){
            t = (_trelli+n*_cols) + m;
            t->pixels.show();
        }
    }
}

/**************************************************************************/
/*! 
    @brief  read all events currently stored in the seesaw fifo and call any callbacks.
*/
/**************************************************************************/
void Adafruit_MultiTrellis::read()
{
    Adafruit_NeoTrellis *t;
    for(int n=0; n<_rows; n++){
        for(int m=0; m<_cols; m++){
            t = (_trelli+n*_cols) + m;

            uint8_t count = t->getKeypadCount();
            delayMicroseconds(500);
            if(count > 0){
                count = count + 2;
                keyEventRaw e[count];
                t->readKeypad(e, count);
                for(int i=0; i<count; i++){
                    //call any callbacks associated with the key
                    e[i].bit.NUM = NEO_TRELLIS_SEESAW_KEY(e[i].bit.NUM);
                    if(e[i].bit.NUM < NEO_TRELLIS_NUM_KEYS && t->_callbacks[e[i].bit.NUM] != NULL){
                        //update the event with the multitrellis number
                        keyEvent evt = { e[i].bit.EDGE, e[i].bit.NUM };
                        int x = NEO_TRELLIS_X(e[i].bit.NUM);
                        int y = NEO_TRELLIS_Y(e[i].bit.NUM);

                        x = x + m*NEO_TRELLIS_NUM_COLS;
                        y = y + n*NEO_TRELLIS_NUM_ROWS;

                        evt.bit.NUM = y*NEO_TRELLIS_NUM_COLS*_cols + x;

                        t->_callbacks[e[i].bit.NUM](evt);
                    }
                }
            }
        }
    }
}