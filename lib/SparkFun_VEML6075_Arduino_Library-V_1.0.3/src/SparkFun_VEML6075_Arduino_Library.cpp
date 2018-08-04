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

#include <SparkFun_VEML6075_Arduino_Library.h>

#ifdef DEBUG_VEML6075
#define VEML6075_DEBUG(x) if (_debugPort != NULL) { _debugPort->print(x);}
#define VEML6075_DEBUGLN(x) if (_debugPort != NULL) { _debugPort->println(x);}
#define STORAGE(x) (x)
#else
#define VEML6075_DEBUG(x)
#define VEML6075_DEBUGLN(x)
#define STORAGE(x) (x)
#endif

#define VEML6075_REGISTER_LENGTH 2 // 12 bytes per register
#define NUM_INTEGRATION_TIMES 5

#define VEML6075_DEVICE_ID 0x26

#define VEML6075_UV_IT_MASK  0x70
#define VEML6075_UV_IT_SHIFT 4

#define VEML6075_SHUTDOWN_MASK  0x01
#define VEML6075_SHUTDOWN_SHIFT 0

#define VEML6075_HD_MASK  0x08
#define VEML6075_HD_SHIFT 3

#define VEML6075_TRIG_MASK  0x04
#define VEML6075_TRIG_SHIFT 2

#define VEML6075_AF_MASK  0x02
#define VEML6075_AF_SHIFT 1

#define VEML6075_MASK(reg, mask, shift) ((reg & mask) >> shift)

const float HD_SCALAR = 2.0;

const float UV_ALPHA = 1.0;
const float UV_BETA = 1.0;
const float UV_GAMMA = 1.0;
const float UV_DELTA = 1.0;

const float UVA_A_COEF = 2.22;
const float UVA_B_COEF = 1.33;
const float UVA_C_COEF = 2.95;
const float UVA_D_COEF = 1.75;

const float UVA_RESPONSIVITY_100MS_UNCOVERED = 0.001111;
const float UVB_RESPONSIVITY_100MS_UNCOVERED = 0.00125;

const float UVA_RESPONSIVITY[NUM_INTEGRATION_TIMES] = 
{
    UVA_RESPONSIVITY_100MS_UNCOVERED / 0.5016286645, // 50ms
    UVA_RESPONSIVITY_100MS_UNCOVERED,                // 100ms
    UVA_RESPONSIVITY_100MS_UNCOVERED / 2.039087948,  // 200ms
    UVA_RESPONSIVITY_100MS_UNCOVERED / 3.781758958,  // 400ms
    UVA_RESPONSIVITY_100MS_UNCOVERED / 7.371335505   // 800ms
};

const float UVB_RESPONSIVITY[NUM_INTEGRATION_TIMES] = 
{
    UVB_RESPONSIVITY_100MS_UNCOVERED / 0.5016286645, // 50ms
    UVB_RESPONSIVITY_100MS_UNCOVERED,                // 100ms
    UVB_RESPONSIVITY_100MS_UNCOVERED / 2.039087948,  // 200ms
    UVB_RESPONSIVITY_100MS_UNCOVERED / 3.781758958,  // 400ms
    UVB_RESPONSIVITY_100MS_UNCOVERED / 7.371335505   // 800ms
};

VEML6075::VEML6075()
{
    _i2cPort = NULL;
    _debugPort = NULL;
    _deviceAddress = VEML6075_ADDRESS_INVALID;
    _lastReadTime = 0;
    _integrationTime = 0;
    _lastIndex = 0.0;
    _aResponsivity = UVA_RESPONSIVITY_100MS_UNCOVERED;
    _bResponsivity = UVB_RESPONSIVITY_100MS_UNCOVERED;
    _hdEnabled = false;
}

boolean VEML6075::begin(void)
{
    if (begin(Wire) == VEML6075_ERROR_SUCCESS)
    {
        return true;
    }
    return false;
}

VEML6075_error_t VEML6075::begin(TwoWire &wirePort) 
{
    uint8_t systemControl = 0;
    VEML6075_error_t err;

    _deviceAddress = VEML6075_ADDRESS;
    _i2cPort = &wirePort;

    #ifdef WIRE_HAS_END
        _i2cPort->end();
    #endif
    _i2cPort->begin();

    err = _connected();
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return err;
    }

    if (powerOn() == VEML6075_ERROR_SUCCESS)
    {
        // Set intergration time to a default of 100ms
        setIntegrationTime(IT_100MS);
        // Set high-dynamic mode to normal:
        setHighDynamic(DYNAMIC_NORMAL);
        // Disable auto-force mode
        setAutoForce(AF_DISABLE);
    }
    return VEML6075_ERROR_SUCCESS;
}

void VEML6075::setDebugStream(Stream & debugPort)
{
    _debugPort = &debugPort;
}

boolean VEML6075::isConnected(void)
{
    if (_connected() != VEML6075_ERROR_SUCCESS)
    {
        return false;
    }
    return true;
}

VEML6075_error_t VEML6075::setIntegrationTime(VEML6075::veml6075_uv_it_t it)
{
    VEML6075_error_t err;
    veml6075_t conf;
    if (it >= IT_RESERVED_0)
    {
        return VEML6075_ERROR_UNDEFINED;
    }

    err = readI2CRegister(&conf, VEML6075::REG_UV_CONF);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return err;
    }
    
    conf &= ~(VEML6075_UV_IT_MASK);
    conf |= (it<<VEML6075_UV_IT_SHIFT);
    err = writeI2CRegister(conf, VEML6075::REG_UV_CONF);    
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return err;
    }

    _aResponsivity = UVA_RESPONSIVITY[(uint8_t)it];
    _bResponsivity = UVB_RESPONSIVITY[(uint8_t)it];

    switch (it)
    {
    case IT_50MS:
        _integrationTime = 50;
        break;
    case IT_100MS:
        _integrationTime = 100;
        break;
    case IT_200MS:
        _integrationTime = 200;
        break;
    case IT_400MS:
        _integrationTime = 400;
        break;
    case IT_800MS:
        _integrationTime = 800;
        break;
    default:
        _integrationTime = 0;
    }
    return err;
}

VEML6075::veml6075_uv_it_t VEML6075::getIntegrationTime(void)
{
    VEML6075_error_t err;
    veml6075_t conf;

    err = readI2CRegister(&conf, VEML6075::REG_UV_CONF);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return IT_INVALID;
    }
    return static_cast<VEML6075::veml6075_uv_it_t>((conf & VEML6075_UV_IT_MASK) >> VEML6075_UV_IT_SHIFT);
}

VEML6075_error_t VEML6075::setHighDynamic(VEML6075::veml6075_hd_t hd)
{
    VEML6075_error_t err;
    veml6075_t conf;

    err = readI2CRegister(&conf, VEML6075::REG_UV_CONF);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return err;
    }
    
    if (hd == DYNAMIC_HIGH) 
    {
        _hdEnabled = true;
    }
    else
    {
        _hdEnabled = false;        
    }
    conf &= ~(VEML6075_HD_MASK);
    conf |= (hd << VEML6075_HD_SHIFT);
    return writeI2CRegister(conf, VEML6075::REG_UV_CONF);
}

VEML6075::veml6075_hd_t VEML6075::getHighDynamic(void)
{
    VEML6075_error_t err;
    veml6075_t conf;

    err = readI2CRegister(&conf, VEML6075::REG_UV_CONF);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return HD_INVALID;
    }
    return static_cast<VEML6075::veml6075_hd_t>((conf & VEML6075_HD_MASK) >> VEML6075_HD_SHIFT);
}

VEML6075_error_t VEML6075::setTrigger(VEML6075::veml6075_uv_trig_t trig)
{
    VEML6075_error_t err;
    veml6075_t conf;

    err = readI2CRegister(&conf, VEML6075::REG_UV_CONF);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return err;
    }
    
    conf &= ~(VEML6075_TRIG_MASK);
    conf |= (trig << VEML6075_TRIG_SHIFT);
    return writeI2CRegister(conf, VEML6075::REG_UV_CONF);
}

VEML6075::veml6075_uv_trig_t VEML6075::getTrigger(void)
{
    VEML6075_error_t err;
    veml6075_t conf;

    err = readI2CRegister(&conf, VEML6075::REG_UV_CONF);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return TRIGGER_INVALID;
    }
    return static_cast<VEML6075::veml6075_uv_trig_t>((conf & VEML6075_TRIG_MASK) >> VEML6075_TRIG_SHIFT);
}


VEML6075_error_t VEML6075::setAutoForce(VEML6075::veml6075_af_t af)
{
    VEML6075_error_t err;
    veml6075_t conf;

    err = readI2CRegister(&conf, VEML6075::REG_UV_CONF);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return err;
    }
    
    conf &= ~(VEML6075_AF_MASK);
    conf |= (af << VEML6075_AF_SHIFT);
    return writeI2CRegister(conf, VEML6075::REG_UV_CONF);
}

VEML6075::veml6075_af_t VEML6075::getAutoForce(void)
{
    VEML6075_error_t err;
    veml6075_t conf;

    err = readI2CRegister(&conf, VEML6075::REG_UV_CONF);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return AF_INVALID;
    }
    return static_cast<VEML6075::veml6075_af_t>((conf & VEML6075_AF_MASK) >> VEML6075_AF_SHIFT);
}

VEML6075_error_t VEML6075::powerOn(boolean enable)
{
    return shutdown(!enable);
}

VEML6075_error_t VEML6075::shutdown(boolean shutdown)
{
    VEML6075_error_t err;
    veml6075_t conf;
    VEML6075_shutdown_t sd = VEML6075::POWER_ON;

    err = readI2CRegister(&conf, VEML6075::REG_UV_CONF);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return err;
    }
    
    if (shutdown == true)
    {
        sd = VEML6075::SHUT_DOWN;
    }
    conf &= ~(VEML6075_SHUTDOWN_MASK); // Clear shutdown bit
    conf |= sd << VEML6075_SHUTDOWN_SHIFT;//VEML6075_MASK(conf, VEML6075_SHUTDOWN_MASK, VEML6075_SHUTDOWN_SHIFT);
    return writeI2CRegister(conf, VEML6075::REG_UV_CONF);
}

VEML6075::VEML6075_shutdown_t VEML6075::getShutdown(void)
{
}

VEML6075_error_t VEML6075::trigger(void)
{
    return setTrigger(TRIGGER_ONE_OR_UV_TRIG);
}

float VEML6075::a(void)
{
    return uva();
}

float VEML6075::b(void)
{
    return uvb();
}

float VEML6075::i(void)
{
    return index();
}

float VEML6075::uva(void)
{
    return (float)rawUva() - ((UVA_A_COEF * UV_ALPHA * uvComp1()) / UV_GAMMA) 
                           - ((UVA_B_COEF * UV_ALPHA * uvComp2()) / UV_DELTA);
}

float VEML6075::uvb(void)
{
    return (float)rawUvb() - ((UVA_C_COEF * UV_BETA * uvComp1()) / UV_GAMMA)
                           - ((UVA_D_COEF * UV_BETA * uvComp2()) / UV_DELTA);
}

uint16_t VEML6075::rawUva(void)
{
    VEML6075_error_t err;
    uint8_t uva[2] = {0, 0};

    /*if ((_lastReadTime + _integrationTime) > millis())
    {
        return _lastUVA;
    }*/
    err = readI2CBuffer(uva, VEML6075::REG_UVA_DATA, 2);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return err;
    }
    _lastReadTime = millis();
    _lastUVA = (uva[0] & 0x00FF) | ((uva[1] & 0x00FF) << 8);
    return _lastUVA;
}

uint16_t VEML6075::rawUvb(void)
{
    VEML6075_error_t err;
    uint8_t uvb[2] = {0, 0};

    /*if ((_lastReadTime + _integrationTime) > millis())
    {
        return _lastUVB;
    }*/
    err = readI2CBuffer(uvb, VEML6075::REG_UVB_DATA, 2);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return err;
    }
    _lastReadTime = millis();
    _lastUVB = (uvb[0] & 0x00FF) | ((uvb[1] & 0x00FF) << 8);
    return _lastUVB;
}

float VEML6075::index(void)
{
    uint16_t uva;
    uint16_t uvb;
    uint16_t uvComp1;
    uint16_t uvComp2;

    /*if ((_lastReadTime + _integrationTime) > millis())
    {
        return _lastIndex;
    }*/

    float uvaCalc = this->uva();
    float uvbCalc = this->uvb();

    float uvia = uvaCalc * (1.0 / UV_ALPHA) * _aResponsivity;
    float uvib = uvbCalc * (1.0 / UV_BETA) * _bResponsivity;
    _lastIndex = (uvia + uvib) / 2.0;
    if (_hdEnabled)
    {
        _lastIndex *= HD_SCALAR;
    }
    
    _lastReadTime = millis();
    return _lastIndex;
}

uint16_t VEML6075::uvComp1(void)
{
    VEML6075_error_t err;
    uint8_t uvcomp1[2] = {0, 0};
    err = readI2CBuffer(uvcomp1, VEML6075::REG_UVCOMP1_DATA, 2);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return err;
    }
    return (uvcomp1[0] & 0x00FF) | ((uvcomp1[1] & 0x00FF) << 8);
}

uint16_t VEML6075::uvComp2(void)
{
    VEML6075_error_t err;
    uint8_t uvcomp2[2] = {0, 0};
    err = readI2CBuffer(uvcomp2, VEML6075::REG_UVCOMP2_DATA, 2);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return err;
    }
    return (uvcomp2[0] & 0x00FF) | ((uvcomp2[1] & 0x00FF) << 8);
}

uint16_t VEML6075::visibleCompensation(void)
{
    return uvComp1();
}

uint16_t VEML6075::irCompensation(void)
{
    return uvComp2();
}

VEML6075_error_t VEML6075::_connected(void)
{
    uint8_t id;
    VEML6075_error_t err = deviceID(&id);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        VEML6075_DEBUGLN(("Connect err: " + String (err)));
        return err;
    }
    if (id != VEML6075_DEVICE_ID)
    {
        VEML6075_DEBUGLN(("Connect read err: " + String (id)));
        return VEML6075_ERROR_INVALID_ADDRESS;
    }
    VEML6075_DEBUGLN(("Connect success!"));

    return VEML6075_ERROR_SUCCESS;
}

VEML6075_error_t VEML6075::deviceID(uint8_t * id)
{
    VEML6075_error_t err;
    veml6075_t devID = 0;
    
    err = readI2CRegister(&devID, VEML6075::REG_ID);

    if (err != VEML6075_ERROR_SUCCESS)
    {
        return err;
    }
    VEML6075_DEBUGLN(("Device ID: " + String(devID, HEX)));
    *id = (uint8_t) (devID & 0x00FF);
    return err;
}

VEML6075_error_t VEML6075::deviceAddress(uint8_t * address)
{
    veml6075_t ret[2] = {0, 0};
    VEML6075_error_t err;
    err = readI2CRegister(ret, REG_ID);
    if (err != VEML6075_ERROR_SUCCESS)
    {
        return err;
    }
    *address = ret[1];
    VEML6075_DEBUGLN(("Address: " + String(ret[1])));
    return err;
}

VEML6075_error_t VEML6075::readI2CBuffer(uint8_t * dest, VEML6075_REGISTER_t startRegister, uint16_t len)
{
    VEML6075_DEBUGLN((STORAGE("(readI2CBuffer): read ") + String(len) + 
                       STORAGE(" @ 0x") + String(startRegister, HEX)));
    if (_deviceAddress == VEML6075_ADDRESS_INVALID)
    {
        VEML6075_DEBUGLN(STORAGE("    ERR (readI2CBuffer): Invalid address"));
        return VEML6075_ERROR_INVALID_ADDRESS;
    }
    _i2cPort->beginTransmission((uint8_t) _deviceAddress);
    _i2cPort->write(startRegister);
    if (_i2cPort->endTransmission(false) != 0)
    {
        VEML6075_DEBUGLN(STORAGE("    ERR (readI2CBuffer): End transmission"));
        return VEML6075_ERROR_READ;
    }
    
    _i2cPort->requestFrom((uint8_t)_deviceAddress, (uint8_t)len);
    for (int i = 0; i < len; i++)
    {
        dest[i] = _i2cPort->read();
        VEML6075_DEBUGLN((STORAGE("    ") + String(i) + STORAGE(": 0x") + String(dest[i], HEX)));
    }

    return VEML6075_ERROR_SUCCESS;
}

VEML6075_error_t VEML6075::writeI2CBuffer(uint8_t * src, VEML6075_REGISTER_t startRegister, uint16_t len)
{
    if (_deviceAddress == VEML6075_ADDRESS_INVALID)
    {
        VEML6075_DEBUGLN(STORAGE("ERR (readI2CBuffer): Invalid address"));
        return VEML6075_ERROR_INVALID_ADDRESS;
    }
    _i2cPort->beginTransmission((uint8_t) _deviceAddress);
    _i2cPort->write(startRegister);
    for (int i = 0; i < len; i++)
    {
        _i2cPort->write(src[i]);
    }
    if (_i2cPort->endTransmission(true) != 0)
    {
        return VEML6075_ERROR_WRITE;
    }
    return VEML6075_ERROR_SUCCESS;
}

VEML6075_error_t VEML6075::readI2CRegister(veml6075_t * dest, VEML6075_REGISTER_t registerAddress)
{
    VEML6075_error_t err;
    uint8_t tempDest[2];
    err = readI2CBuffer(tempDest, registerAddress, VEML6075_REGISTER_LENGTH);
    if (err == VEML6075_ERROR_SUCCESS)
    {
        *dest = (tempDest[0]) | ((veml6075_t) tempDest[1] << 8);
    }
    return err;
}

VEML6075_error_t VEML6075::writeI2CRegister(veml6075_t data, VEML6075_REGISTER_t registerAddress)
{
    uint8_t d[2];
    // Write LSB first
    d[0] = (uint8_t) (data & 0x00FF);
    d[1] = (uint8_t) ((data & 0xFF00) >> 8);
    return writeI2CBuffer(d, registerAddress, VEML6075_REGISTER_LENGTH);
}