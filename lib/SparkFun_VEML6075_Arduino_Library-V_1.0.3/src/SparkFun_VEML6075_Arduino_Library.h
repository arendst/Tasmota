/*
  This is a library written for the VEML6075 UVA/UVB/UV index Sensopr
  SparkFun sells these at its website: www.sparkfun.com
  Do you like this library? Help support SparkFun. Buy a board!
  https://www.sparkfun.com/products/14748
  Written by Jim Lindblom @ SparkFun Electronics, May 23, 2018

  The VEML6075 senses UVA and UVB light, which allows for a calculation
  of the UV index.
  
  This library handles the initialization, configuration and monitoring of the 
  UVA and UVB intensity, and calculation of the UV index.
  
  https://github.com/sparkfunX/SparkFun_VEML6075_Arduino_Library

  Development environment specifics:
  Arduino IDE 1.8.5
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define DEBUG_VEML6075

#include <Wire.h>

typedef uint16_t veml6075_t;

//  Valid VEML6075 addresses
typedef enum {
    VEML6075_ADDRESS = 0x10,
    VEML6075_ADDRESS_INVALID = 0xFF
} VEML6075_Address_t;

// VEML6075 error code returns:
typedef enum {
    VEML6075_ERROR_READ            = -4,
    VEML6075_ERROR_WRITE           = -3,
    VEML6075_ERROR_INVALID_ADDRESS = -2,
    VEML6075_ERROR_UNDEFINED       = -1,
    VEML6075_ERROR_SUCCESS         = 1
} VEML6075_error_t;
const VEML6075_error_t VEML6075_SUCCESS = VEML6075_ERROR_SUCCESS;

struct test {
    float a;
    float b;
};

class VEML6075 {
public:
    typedef enum {
        IT_50MS,
        IT_100MS,
        IT_200MS,
        IT_400MS,
        IT_800MS,
        IT_RESERVED_0,
        IT_RESERVED_1,
        IT_RESERVED_2,
        IT_INVALID
    } veml6075_uv_it_t;

    typedef enum {
        DYNAMIC_NORMAL,
        DYNAMIC_HIGH,
        HD_INVALID
    } veml6075_hd_t;

    typedef enum {
        NO_TRIGGER,
        TRIGGER_ONE_OR_UV_TRIG,
        TRIGGER_INVALID
    } veml6075_uv_trig_t;

    typedef enum {
        AF_DISABLE,
        AF_ENABLE,
        AF_INVALID
    } veml6075_af_t;

    typedef enum {
        POWER_ON,
        SHUT_DOWN,
        SD_INVALID
    } VEML6075_shutdown_t;

    VEML6075();

    // begin initializes the Wire port and I/O expander
    boolean begin(void);
    // give begin a TwoWire port to specify the I2C port
    VEML6075_error_t begin(TwoWire &wirePort);

    // setDebugStream to enable library debug statements
    void setDebugStream(Stream &debugPort = Serial);

    boolean isConnected(void);

    // Configuration controls
    VEML6075_error_t setIntegrationTime(veml6075_uv_it_t it);
    veml6075_uv_it_t getIntegrationTime(void);

    VEML6075_error_t setHighDynamic(veml6075_hd_t hd);
    veml6075_hd_t getHighDynamic(void);

    VEML6075_error_t setTrigger(veml6075_uv_trig_t trig);
    veml6075_uv_trig_t getTrigger(void);
    VEML6075_error_t trigger(void);

    VEML6075_error_t setAutoForce(veml6075_af_t af);
    veml6075_af_t getAutoForce(void);

    VEML6075_error_t powerOn(boolean enable = true);
    VEML6075_error_t shutdown(boolean shutdown = true);
    VEML6075_shutdown_t getShutdown(void);

    uint16_t rawUva(void);
    uint16_t rawUvb(void);
    float uva(void);
    float uvb(void);
    float index(void);
    float a(void);
    float b(void);
    float i(void);

    uint16_t uvComp1(void);
    uint16_t uvComp2(void);
    uint16_t visibleCompensation(void);
    uint16_t irCompensation(void);

    VEML6075_error_t deviceID(uint8_t * id);
    VEML6075_error_t deviceAddress(uint8_t * address);

private:

    // VEML6075 registers:
    typedef enum {
        REG_UV_CONF = 0x00,
        REG_UVA_DATA = 0x07,
        REG_UVB_DATA = 0x09,
        REG_UVCOMP1_DATA = 0x0A,
        REG_UVCOMP2_DATA = 0x0B,
        REG_ID = 0x0C    
    } VEML6075_REGISTER_t;

    TwoWire *_i2cPort; //The generic connection to user's chosen I2C hardware
    Stream * _debugPort;
    VEML6075_Address_t _deviceAddress;

    unsigned int _integrationTime;
    unsigned long _lastReadTime;
    float _lastIndex, _lastUVA, _lastUVB;
    float _aResponsivity, _bResponsivity;
    bool _hdEnabled;

    VEML6075_error_t _connected(void);

// I2C Read/Write
    VEML6075_error_t readI2CBuffer(uint8_t * dest, VEML6075_REGISTER_t startRegister, uint16_t len);
    VEML6075_error_t writeI2CBuffer(uint8_t * src, VEML6075_REGISTER_t startRegister, uint16_t len);
    VEML6075_error_t readI2CRegister(veml6075_t * dest, VEML6075_REGISTER_t registerAddress);
    VEML6075_error_t writeI2CRegister(veml6075_t data, VEML6075_REGISTER_t registerAddress);
};