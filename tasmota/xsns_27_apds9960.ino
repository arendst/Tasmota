/*
  xsns_27_apds9960.ino - Support for I2C APDS9960 Proximity Sensor for Tasmota

  Copyright (C) 2020  Shawn Hymel/Sparkfun and Theo Arends

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef USE_I2C
#ifdef USE_APDS9960
/*********************************************************************************************\
 * APDS9960 - Digital Proximity Ambient Light RGB and Gesture Sensor
 *
 * Source: Shawn Hymel (SparkFun Electronics)
 * Adaption for TASMOTA: Christian Baars
 *
 * I2C Address: 0x39 - standard address
\*********************************************************************************************/

// #if defined(USE_SHT) || defined(USE_VEML6070) || defined(USE_TSL2561)
//   #warning **** Turned off conflicting drivers SHT and VEML6070 ****
//   #ifdef USE_SHT
//   #undef USE_SHT          // SHT-Driver blocks gesture sensor
//   #endif
//   #ifdef USE_VEML6070
//   #undef USE_VEML6070     // address conflict on the I2C-bus
//   #endif
//   #ifdef USE_TSL2561
//   #undef USE_TSL2561     // possible address conflict on the I2C-bus
//   #endif
// #endif

#define XSNS_27                   27
#define XI2C_21                   21              // See I2CDEVICES.md


#define APDS9960_I2C_ADDR         0x39

#define APDS9960_CHIPID_1         0xAB
#define APDS9960_CHIPID_2         0x9C
#define APDS9960_CHIPID_3         0xA8

#define APDS9930_CHIPID_1         0x12  // we will check, if someone got an incorrect sensor
#define APDS9930_CHIPID_2         0x39  // there are case reports about "accidentially bought" 9930's

#define APDS9960_MODE_GESTURE     0
#define APDS9960_MODE_COLOR       1

/* Gesture parameters */
#define GESTURE_THRESHOLD_OUT     10
#define GESTURE_SENSITIVITY_1     50
#define GESTURE_SENSITIVITY_2     20

#define APDS9960_LONG_RECOVERY           50  // long pause after sensor overload in loops
#define APDS9960_MAX_GESTURE_CYCLES      50  // how many FIFO-reads are allowed to prevent crash

/******************************************************************************\
 * Constants
\******************************************************************************/

const char APDS9960_TAG[]         PROGMEM = "APDS9960";  // Only one actualy

#ifdef USE_WEBSERVER

#ifdef USE_APDS9960_GESTURE
const char HTTP_SNS_GESTURE[]     PROGMEM = "{s}%s " D_GESTURE "{m}%s{e}";
#endif  // USE_APDS9960_GESTURE

#ifdef USE_APDS9960_COLOR
const char HTTP_SNS_COLOR_RED[]   PROGMEM = "{s}%s " D_COLOR_RED "{m}%u{e}";
const char HTTP_SNS_COLOR_GREEN[] PROGMEM = "{s}%s " D_COLOR_GREEN "{m}%u{e}";
const char HTTP_SNS_COLOR_BLUE[]  PROGMEM = "{s}%s " D_COLOR_BLUE "{m}%u{e}";
const char HTTP_SNS_CCT[]         PROGMEM = "{s}%s " D_CCT "{m}%u " D_UNIT_KELVIN  "{e}";
#endif  // USE_APDS9960_COLOR

#ifdef USE_APDS9960_PROXIMITY
const char HTTP_SNS_PROXIMITY[]   PROGMEM = "{s}%s " D_PROXIMITY  "{m}%u{e}";
#endif  // USE_APDS9960_PROXIMITY

#endif  // USE_WEBSERVER

/******************************************************************************\
 * APDS9960
 *
 * Programmer : APDS9960 Datasheet and Sparkfun
\******************************************************************************/

/* Misc parameters */
#define FIFO_PAUSE_TIME         30      // Wait period (ms) between FIFO reads

/* APDS-9960 register addresses */
#define APDS9960_ENABLE         0x80
#define APDS9960_ATIME          0x81
#define APDS9960_WTIME          0x83
#define APDS9960_AILTL          0x84
#define APDS9960_AILTH          0x85
#define APDS9960_AIHTL          0x86
#define APDS9960_AIHTH          0x87
#define APDS9960_PILT           0x89
#define APDS9960_PIHT           0x8B
#define APDS9960_PERS           0x8C
#define APDS9960_CONFIG1        0x8D
#define APDS9960_PPULSE         0x8E
#define APDS9960_CONTROL        0x8F
#define APDS9960_CONFIG2        0x90
#define APDS9960_ID             0x92
#define APDS9960_STATUS         0x93
#define APDS9960_CDATAL         0x94
#define APDS9960_CDATAH         0x95
#define APDS9960_RDATAL         0x96
#define APDS9960_RDATAH         0x97
#define APDS9960_GDATAL         0x98
#define APDS9960_GDATAH         0x99
#define APDS9960_BDATAL         0x9A
#define APDS9960_BDATAH         0x9B
#define APDS9960_PDATA          0x9C
#define APDS9960_POFFSET_UR     0x9D
#define APDS9960_POFFSET_DL     0x9E
#define APDS9960_CONFIG3        0x9F
#define APDS9960_GPENTH         0xA0
#define APDS9960_GEXTH          0xA1
#define APDS9960_GCONF1         0xA2
#define APDS9960_GCONF2         0xA3
#define APDS9960_GOFFSET_U      0xA4
#define APDS9960_GOFFSET_D      0xA5
#define APDS9960_GOFFSET_L      0xA7
#define APDS9960_GOFFSET_R      0xA9
#define APDS9960_GPULSE         0xA6
#define APDS9960_GCONF3         0xAA
#define APDS9960_GCONF4         0xAB
#define APDS9960_GFLVL          0xAE
#define APDS9960_GSTATUS        0xAF
#define APDS9960_IFORCE         0xE4
#define APDS9960_PICLEAR        0xE5
#define APDS9960_CICLEAR        0xE6
#define APDS9960_AICLEAR        0xE7
#define APDS9960_GFIFO_U        0xFC
#define APDS9960_GFIFO_D        0xFD
#define APDS9960_GFIFO_L        0xFE
#define APDS9960_GFIFO_R        0xFF

/* Bit fields */
#define APDS9960_PON            0b00000001
#define APDS9960_AEN            0b00000010
#define APDS9960_PEN            0b00000100
#define APDS9960_WEN            0b00001000
#define APDS9960_AIEN           0b00010000
#define APDS9960_PIEN           0b00100000
#define APDS9960_GEN            0b01000000

#define APDS9960_GVALID         0b00000001

/* On/Off definitions */
#define OFF                     0
#define ON                      1

/* Acceptable parameters for setMode */
#define POWER                   0
#define AMBIENT_LIGHT           1
#define PROXIMITY               2
#define WAIT                    3
#define AMBIENT_LIGHT_INT       4
#define PROXIMITY_INT           5
#define GESTURE                 6
#define ALL                     7

/* LED Drive values */
#define LED_DRIVE_100MA         0
#define LED_DRIVE_50MA          1
#define LED_DRIVE_25MA          2
#define LED_DRIVE_12_5MA        3

/* Proximity Gain (PGAIN) values */
#define PGAIN_1X                0
#define PGAIN_2X                1
#define PGAIN_4X                2
#define PGAIN_8X                3

/* ALS Gain (AGAIN) values */
#define AGAIN_1X                0
#define AGAIN_4X                1
#define AGAIN_16X               2
#define AGAIN_64X               3

/* Gesture Gain (GGAIN) values */
#define GGAIN_1X                0
#define GGAIN_2X                1
#define GGAIN_4X                2
#define GGAIN_8X                3

/* LED Boost values */
#define LED_BOOST_100           0
#define LED_BOOST_150           1
#define LED_BOOST_200           2
#define LED_BOOST_300           3

/* Gesture wait time values */
#define GWTIME_0MS              0
#define GWTIME_2_8MS            1
#define GWTIME_5_6MS            2
#define GWTIME_8_4MS            3
#define GWTIME_14_0MS           4
#define GWTIME_22_4MS           5
#define GWTIME_30_8MS           6
#define GWTIME_39_2MS           7



/* Default values */
#define DEFAULT_ATIME           0xdb              // 103ms = 0xdb = 219
#define DEFAULT_WTIME           246               // 27ms
#define DEFAULT_PROX_PPULSE     0x87              // 16us, 8 pulses
#define DEFAULT_GESTURE_PPULSE  0x89              // 16us, 10 pulses  ---89
#define DEFAULT_POFFSET_UR      0                 // 0 offset
#define DEFAULT_POFFSET_DL      0                 // 0 offset
#define DEFAULT_CONFIG1         0x60              // No 12x wait (WTIME) factor
#define DEFAULT_LDRIVE          LED_DRIVE_100MA
#define DEFAULT_PGAIN           PGAIN_4X
#define DEFAULT_AGAIN           AGAIN_4X          // we have to divide by the same facot at the end
#define DEFAULT_PILT            0                 // Low proximity threshold
#define DEFAULT_PIHT            50                // High proximity threshold
#define DEFAULT_AILT            0xFFFF            // Force interrupt for calibration
#define DEFAULT_AIHT            0
#define DEFAULT_PERS            0x11              // 2 consecutive prox or ALS for int.
#define DEFAULT_CONFIG2         0x01              // No saturation interrupts or LED boost
#define DEFAULT_CONFIG3         0                 // Enable all photodiodes, no SAI
#define DEFAULT_GPENTH          40                // Threshold for entering gesture mode
#define DEFAULT_GEXTH           30                // Threshold for exiting gesture mode
#define DEFAULT_GCONF1          0x40              // 4 gesture events for int., 1 for exit
#define DEFAULT_GGAIN           GGAIN_4X
#define DEFAULT_GLDRIVE         LED_DRIVE_100MA   // default 100ma
#define DEFAULT_GWTIME          GWTIME_2_8MS      // default 2_8MS
#define DEFAULT_GOFFSET         0                 // No offset scaling for gesture mode
#define DEFAULT_GPULSE          0xC9              // 32us, 10 pulses
#define DEFAULT_GCONF3          0                 // All photodiodes active during gesture
#define DEFAULT_GIEN            0                 // Disable gesture interrupts

#define APDS9960_ERROR          0xFF

#ifdef USE_APDS9960_GESTURE

/* Direction definitions */
const char GESTURE_UP[]    PROGMEM = "Up";
const char GESTURE_DOWN[]  PROGMEM = "Down";
const char GESTURE_LEFT[]  PROGMEM = "Left";
const char GESTURE_RIGHT[] PROGMEM = "Right";
const char GESTURE_LONG[]  PROGMEM = "Long";
const char GESTURE_NONE[]  PROGMEM = "None";

enum {
  DIR_NONE,
  DIR_LEFT,
  DIR_RIGHT,
  DIR_UP,
  DIR_DOWN,
  DIR_NEAR,
  DIR_FAR,
  DIR_ALL
};

/* State definitions*/
/*
enum {
  APDS9960_NA_STATE,
  APDS9960_NEAR_STATE,
  APDS9960_FAR_STATE,
  APDS9960_ALL_STATE
};
*/

/* Container for gesture data */
typedef struct gesture_data_type {
  uint8_t u_data[32];
  uint8_t d_data[32];
  uint8_t l_data[32];
  uint8_t r_data[32];
  uint8_t index;
  uint8_t total_gestures;
  uint8_t in_threshold;
  uint8_t out_threshold;
} gesture_data_t;

typedef struct gesture_type {
  int16_t ud_delta_ = 0;
  int16_t lr_delta_ = 0;
  int16_t ud_count_ = 0;
  int16_t lr_count_ = 0;
  int16_t state_ = 0;
  int16_t motion_ = DIR_NONE;
} gesture_t;

#endif  // USE_APDS9960_GESTURE

#if defined(USE_APDS9960_COLOR) || defined(USE_APDS9960_PROXIMITY)
typedef struct color_data_type {
  uint16_t a;     // measured ambient
  uint16_t r;     // Red
  uint16_t g;     // Green
  uint16_t b;     // Blue
  uint8_t p;      // proximity

  uint16_t cct;   // calculated color temperature
  uint16_t lux;   // calculated illuminance - atm only from rgb
} color_data_t;
#endif  // USE_APDS9960_COLOR || USE_APDS9960_PROXIMITY

/******************************************************************************\
 * Globals
\******************************************************************************/

#ifdef USE_APDS9960_GESTURE
gesture_data_t gesture_data;
gesture_t gesture;
char currentGesture[6];
#endif  // USE_APDS9960_GESTURE

#if defined(USE_APDS9960_COLOR) || defined(USE_APDS9960_PROXIMITY)
color_data_t color_data;
#endif  // USE_APDS9960_COLOR || USE_APDS9960_PROXIMITY

volatile uint8_t recovery_loop_counter = 0;   // count number of stateloops to switch the sensor off, if needed
bool APDS9960_overload = false;
uint8_t APDS9960_aTime = DEFAULT_ATIME;
uint8_t APDS9960_type = 0;
uint8_t gesture_mode = 1;                     // 1 : Gesture | 2 : Color

/******************************************************************************\
 * Helper functions
\******************************************************************************/

#ifdef USE_APDS9960_COLOR
/**
 *   Taken from the Adafruit-library
 *   @brief  Converts the raw R/G/B values to color temperature in degrees
 *            Kelvin
 */
void calculateColorTemperature(void) {
  float X, Y, Z;      /* RGB to XYZ correlation      */
  float xc, yc;       /* Chromaticity co-ordinates   */
  float n;            /* McCamy's formula            */
  float cct;

  /* 1. Map RGB values to their XYZ counterparts.    */
  /* Based on 6500K fluorescent, 3000K fluorescent   */
  /* and 60W incandescent values for a wide range.   */
  /* Note: Y = Illuminance or lux                    */
  X = (-0.14282F * color_data.r) + (1.54924F * color_data.g) + (-0.95641F * color_data.b);
  Y = (-0.32466F * color_data.r) + (1.57837F * color_data.g) + (-0.73191F * color_data.b);  // this is Lux ... under certain circumstances
  Z = (-0.68202F * color_data.r) + (0.77073F * color_data.g) + (+0.56332F * color_data.b);

  /* 2. Calculate the chromaticity co-ordinates      */
  xc = (X) / (X + Y + Z);
  yc = (Y) / (X + Y + Z);

  /* 3. Use McCamy's formula to determine the CCT    */
  n = (xc - 0.3320F) / (0.1858F - yc);

  /* Calculate the final CCT */
  color_data.cct = (449.0F * FastPrecisePowf(n, 3)) + (3525.0F * FastPrecisePowf(n, 2)) + (6823.3F * n) + 5520.33F;

  return;
}
#endif  // USE_APDS9960_COLOR 

/******************************************************************************\
 * Getters and setters for register values
\******************************************************************************/

/**
 * @brief Returns the lower threshold for proximity detection
 *
 * @return lower threshold
 */
uint8_t getProxIntLowThresh(void) {
  uint8_t val;

  /* Read value from PILT register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_PILT);

  return val;
}

/**
 * @brief Sets the lower threshold for proximity detection
 *
 * @param[in] threshold the lower proximity threshold
 */
inline void setProxIntLowThresh(uint8_t threshold) {
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_PILT, threshold);
}

/**
 * @brief Returns the high threshold for proximity detection
 *
 * @return high threshold
 */
uint8_t getProxIntHighThresh(void) {
  uint8_t val;

  /* Read value from PIHT register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_PIHT);

  return val;
}

/**
 * @brief Sets the high threshold for proximity detection
 *
 * @param[in] threshold the high proximity threshold
 */
inline void setProxIntHighThresh(uint8_t threshold) {
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_PIHT, threshold);
}

/**
 * @brief Returns LED drive strength for proximity and ALS
 *
 * Value    LED Current
 *   0        100 mA
 *   1         50 mA
 *   2         25 mA
 *   3         12.5 mA
 *
 * @return the value of the LED drive strength. 0xFF on failure.
 */
uint8_t getLEDDrive(void) {
  uint8_t val;

  /* Read value from CONTROL register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_CONTROL);

  /* Shift and mask out LED drive bits */
  val = (val >> 6) & 0b00000011;

  return val;
}

/**
 * @brief Sets the LED drive strength for proximity and ALS
 *
 * Value    LED Current
 *   0        100 mA
 *   1         50 mA
 *   2         25 mA
 *   3         12.5 mA
 *
 * @param[in] drive the value (0-3) for the LED drive strength
 */
void setLEDDrive(uint8_t drive) {
  uint8_t val;

  /* Read value from CONTROL register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_CONTROL);

  /* Set bits in register to given value */
  drive &= 0b00000011;
  drive = drive << 6;
  val &= 0b00111111;
  val |= drive;

  /* Write register value back into CONTROL register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_CONTROL, val);
}

/**
 * @brief Returns receiver gain for proximity detection
 *
 * Value    Gain
 *   0       1x
 *   1       2x
 *   2       4x
 *   3       8x
 *
 * @return the value of the proximity gain. 0xFF on failure.
 */
uint8_t getProximityGain(void) {
  uint8_t val;

  /* Read value from CONTROL register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_CONTROL);

  /* Shift and mask out PDRIVE bits */
  val = (val >> 2) & 0b00000011;

  return val;
}

/**
 * @brief Sets the receiver gain for proximity detection
 *
 * Value    Gain
 *   0       1x
 *   1       2x
 *   2       4x
 *   3       8x
 *
 * @param[in] drive the value (0-3) for the gain
 */
void setProximityGain(uint8_t drive) {
  uint8_t val;

  /* Read value from CONTROL register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_CONTROL);

  /* Set bits in register to given value */
  drive &= 0b00000011;
  drive = drive << 2;
  val &= 0b11110011;
  val |= drive;

  /* Write register value back into CONTROL register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_CONTROL, val);
}

/**
 * @brief Returns receiver gain for the ambient light sensor (ALS)
 *
 * Value    Gain
 *   0        1x
 *   1        4x
 *   2       16x
 *   3       64x
 *
 * @return the value of the ALS gain. 0xFF on failure.
 */
uint8_t getAmbientLightGain() {
  uint8_t val;

  /* Read value from CONTROL register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_CONTROL);

  /* Shift and mask out ADRIVE bits */
  val &= 0b00000011;

  return val;
}

/**
 * @brief Sets the receiver gain for the ambient light sensor (ALS)
 *
 * Value    Gain
 *   0        1x
 *   1        4x
 *   2       16x
 *   3       64x
 *
 * @param[in] drive the value (0-3) for the gain
 */
void setAmbientLightGain(uint8_t drive) {
  uint8_t val;

  /* Read value from CONTROL register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_CONTROL);

  /* Set bits in register to given value */
  drive &= 0b00000011;
  val &= 0b11111100;
  val |= drive;

  /* Write register value back into CONTROL register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_CONTROL, val);
}

/**
 * @brief Get the current LED boost value
 *
 * Value  Boost Current
 *   0        100%
 *   1        150%
 *   2        200%
 *   3        300%
 *
 * @return The LED boost value. 0xFF on failure.
 */
uint8_t getLEDBoost(void) {
  uint8_t val;

  /* Read value from CONFIG2 register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_CONFIG2);

  /* Shift and mask out LED_BOOST bits */
  val = (val >> 4) & 0b00000011;

  return val;
}

/**
 * @brief Sets the LED current boost value
 *
 * Value  Boost Current
 *   0        100%
 *   1        150%
 *   2        200%
 *   3        300%
 *
 * @param[in] drive the value (0-3) for current boost (100-300%)
 */
void setLEDBoost(uint8_t boost) {
  uint8_t val;

  /* Read value from CONFIG2 register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_CONFIG2);

  /* Set bits in register to given value */
  boost &= 0b00000011;
  boost = boost << 4;
  val &= 0b11001111;
  val |= boost;

  /* Write register value back into CONFIG2 register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_CONFIG2, val);
}

/**
 * @brief Gets proximity gain compensation enable
 *
 * @return 1 if compensation is enabled. 0 if not. 0xFF on error.
 */
uint8_t getProxGainCompEnable(void) {
  uint8_t val;

  /* Read value from CONFIG3 register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_CONFIG3);

  /* Shift and mask out PCMP bits */
  val = (val >> 5) & 0b00000001;

  return val;
}

/**
 * @brief Sets the proximity gain compensation enable
 *
 * @param[in] enable 1 to enable compensation. 0 to disable compensation.
 */
void setProxGainCompEnable(uint8_t enable) {
    uint8_t val;

    /* Read value from CONFIG3 register */
    val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_CONFIG3);

    /* Set bits in register to given value */
    enable &= 0b00000001;
    enable = enable << 5;
    val &= 0b11011111;
    val |= enable;

    /* Write register value back into CONFIG3 register */
    I2cWrite8(APDS9960_I2C_ADDR, APDS9960_CONFIG3, val);
}

/**
 * @brief Gets the current mask for enabled/disabled proximity photodiodes
 *
 * 1 = disabled, 0 = enabled
 * Bit    Photodiode
 *  3       UP
 *  2       DOWN
 *  1       LEFT
 *  0       RIGHT
 *
 * @return Current proximity mask for photodiodes. 0xFF on error.
 */
uint8_t getProxPhotoMask(void) {
  uint8_t val;

  /* Read value from CONFIG3 register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_CONFIG3);

  /* Mask out photodiode enable mask bits */
  val &= 0b00001111;

  return val;
}

/**
 * @brief Sets the mask for enabling/disabling proximity photodiodes
 *
 * 1 = disabled, 0 = enabled
 * Bit    Photodiode
 *  3       UP
 *  2       DOWN
 *  1       LEFT
 *  0       RIGHT
 *
 * @param[in] mask 4-bit mask value
 */
void setProxPhotoMask(uint8_t mask) {
  uint8_t val;

  /* Read value from CONFIG3 register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_CONFIG3);

  /* Set bits in register to given value */
  mask &= 0b00001111;
  val &= 0b11110000;
  val |= mask;

  /* Write register value back into CONFIG3 register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_CONFIG3, val);
}

#ifdef USE_APDS9960_GESTURE

/**
 * @brief Gets the entry proximity threshold for gesture sensing
 *
 * @return Current entry proximity threshold.
 */
uint8_t getGestureEnterThresh(void) {
    uint8_t val;

    /* Read value from GPENTH register */
    val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GPENTH);

    return val;
}

/**
 * @brief Sets the entry proximity threshold for gesture sensing
 *
 * @param[in] threshold proximity value needed to start gesture mode
 */
inline void setGestureEnterThresh(uint8_t threshold) {
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_GPENTH, threshold);
}

/**
 * @brief Gets the exit proximity threshold for gesture sensing
 *
 * @return Current exit proximity threshold.
 */
uint8_t getGestureExitThresh(void) {
  uint8_t val;

  /* Read value from GEXTH register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GEXTH);

  return val;
}

/**
 * @brief Sets the exit proximity threshold for gesture sensing
 *
 * @param[in] threshold proximity value needed to end gesture mode
 */
inline void setGestureExitThresh(uint8_t threshold) {
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_GEXTH, threshold);
}

/**
 * @brief Gets the gain of the photodiode during gesture mode
 *
 * Value    Gain
 *   0       1x
 *   1       2x
 *   2       4x
 *   3       8x
 *
 * @return the current photodiode gain. 0xFF on error.
 */
uint8_t getGestureGain(void) {
  uint8_t val;

  /* Read value from GCONF2 register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GCONF2);

  /* Shift and mask out GGAIN bits */
  val = (val >> 5) & 0b00000011;

  return val;
}

/**
 * @brief Sets the gain of the photodiode during gesture mode
 *
 * Value    Gain
 *   0       1x
 *   1       2x
 *   2       4x
 *   3       8x
 *
 * @param[in] gain the value for the photodiode gain
 */
void setGestureGain(uint8_t gain) {
  uint8_t val;

  /* Read value from GCONF2 register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GCONF2);

  /* Set bits in register to given value */
  gain &= 0b00000011;
  gain = gain << 5;
  val &= 0b10011111;
  val |= gain;

  /* Write register value back into GCONF2 register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_GCONF2, val);
}

/**
 * @brief Gets the drive current of the LED during gesture mode
 *
 * Value    LED Current
 *   0        100 mA
 *   1         50 mA
 *   2         25 mA
 *   3         12.5 mA
 *
 * @return the LED drive current value. 0xFF on error.
 */
uint8_t getGestureLEDDrive(void) {
  uint8_t val;

  /* Read value from GCONF2 register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GCONF2);

  /* Shift and mask out GLDRIVE bits */
  val = (val >> 3) & 0b00000011;

  return val;
}

/**
 * @brief Sets the LED drive current during gesture mode
 *
 * Value    LED Current
 *   0        100 mA
 *   1         50 mA
 *   2         25 mA
 *   3         12.5 mA
 *
 * @param[in] drive the value for the LED drive current
 */
void setGestureLEDDrive(uint8_t drive) {
  uint8_t val;

  /* Read value from GCONF2 register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GCONF2);

  /* Set bits in register to given value */
  drive &= 0b00000011;
  drive = drive << 3;
  val &= 0b11100111;
  val |= drive;

  /* Write register value back into GCONF2 register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_GCONF2, val);
}

/**
 * @brief Gets the time in low power mode between gesture detections
 *
 * Value    Wait time
 *   0          0 ms
 *   1          2.8 ms
 *   2          5.6 ms
 *   3          8.4 ms
 *   4         14.0 ms
 *   5         22.4 ms
 *   6         30.8 ms
 *   7         39.2 ms
 *
 * @return the current wait time between gestures. 0xFF on error.
 */
uint8_t getGestureWaitTime(void) {
  uint8_t val;

  /* Read value from GCONF2 register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GCONF2);

  /* Mask out GWTIME bits */
  val &= 0b00000111;

  return val;
}

/**
 * @brief Sets the time in low power mode between gesture detections
 *
 * Value    Wait time
 *   0          0 ms
 *   1          2.8 ms
 *   2          5.6 ms
 *   3          8.4 ms
 *   4         14.0 ms
 *   5         22.4 ms
 *   6         30.8 ms
 *   7         39.2 ms
 *
 * @param[in] the value for the wait time
 */
void setGestureWaitTime(uint8_t time) {
  uint8_t val;

  /* Read value from GCONF2 register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GCONF2);

  /* Set bits in register to given value */
  time &= 0b00000111;
  val &= 0b11111000;
  val |= time;

  /* Write register value back into GCONF2 register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_GCONF2, val);
}

#endif  // USE_APDS9960_GESTURE

/**
 * @brief Gets the low threshold for ambient light interrupts
 *
 * @param[out] threshold current low threshold stored on the APDS-9960
 */
void getLightIntLowThreshold(uint16_t &threshold) {
  uint8_t val_byte;
  threshold = 0;

  /* Read value from ambient light low threshold, low byte register */
  val_byte = I2cRead8(APDS9960_I2C_ADDR, APDS9960_AILTL);
  threshold = val_byte;

  /* Read value from ambient light low threshold, high byte register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_AILTH, val_byte);
  threshold = threshold + ((uint16_t)val_byte << 8);
}

/**
 * @brief Sets the low threshold for ambient light interrupts
 *
 * @param[in] threshold low threshold value for interrupt to trigger
 */
void setLightIntLowThreshold(uint16_t threshold) {
  uint8_t val_low;
  uint8_t val_high;

  /* Break 16-bit threshold into 2 8-bit values */
  val_low = threshold & 0x00FF;
  val_high = (threshold & 0xFF00) >> 8;

  /* Write low byte */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_AILTL, val_low);

  /* Write high byte */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_AILTH, val_high);
}

/**
 * @brief Gets the high threshold for ambient light interrupts
 *
 * @param[out] threshold current low threshold stored on the APDS-9960
 */
void getLightIntHighThreshold(uint16_t &threshold) {
  uint8_t val_byte;
  threshold = 0;

  /* Read value from ambient light high threshold, low byte register */
  val_byte = I2cRead8(APDS9960_I2C_ADDR, APDS9960_AIHTL);
  threshold = val_byte;

  /* Read value from ambient light high threshold, high byte register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_AIHTH, val_byte);
  threshold = threshold + ((uint16_t)val_byte << 8);
}

/**
 * @brief Sets the high threshold for ambient light interrupts
 *
 * @param[in] threshold high threshold value for interrupt to trigger
 */
void setLightIntHighThreshold(uint16_t threshold) {
  uint8_t val_low;
  uint8_t val_high;

  /* Break 16-bit threshold into 2 8-bit values */
  val_low = threshold & 0x00FF;
  val_high = (threshold & 0xFF00) >> 8;

  /* Write low byte */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_AIHTL, val_low);

  /* Write high byte */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_AIHTH, val_high);
}

/**
 * @brief Gets the low threshold for proximity interrupts
 *
 * @param[out] threshold current low threshold stored on the APDS-9960
 */
void getProximityIntLowThreshold(uint8_t &threshold) {
  threshold = 0;

  /* Read value from proximity low threshold register */
  threshold = I2cRead8(APDS9960_I2C_ADDR, APDS9960_PILT);
}

/**
 * @brief Sets the low threshold for proximity interrupts
 *
 * @param[in] threshold low threshold value for interrupt to trigger
 */
void setProximityIntLowThreshold(uint8_t threshold) {
  /* Write threshold value to register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_PILT, threshold);
}

/**
 * @brief Gets the high threshold for proximity interrupts
 *
 * @param[out] threshold current low threshold stored on the APDS-9960
 */
void getProximityIntHighThreshold(uint8_t &threshold) {
  threshold = 0;

  /* Read value from proximity low threshold register */
  threshold = I2cRead8(APDS9960_I2C_ADDR, APDS9960_PIHT);
}

/**
 * @brief Sets the high threshold for proximity interrupts
 *
 * @param[in] threshold high threshold value for interrupt to trigger
 */
void setProximityIntHighThreshold(uint8_t threshold) {
  /* Write threshold value to register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_PIHT, threshold);
}

/**
 * @brief Gets if ambient light interrupts are enabled or not
 *
 * @return 1 if interrupts are enabled, 0 if not. 0xFF on error.
 */
uint8_t getAmbientLightIntEnable(void) {
  uint8_t val;

  /* Read value from ENABLE register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_ENABLE);

  /* Shift and mask out AIEN bit */
  val = (val >> 4) & 0b00000001;

  return val;
}

/**
 * @brief Turns ambient light interrupts on or off
 *
 * @param[in] enable 1 to enable interrupts, 0 to turn them off
 */
void setAmbientLightIntEnable(uint8_t enable) {
  uint8_t val;

  /* Read value from ENABLE register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_ENABLE);

  /* Set bits in register to given value */
  enable &= 0b00000001;
  enable = enable << 4;
  val &= 0b11101111;
  val |= enable;

  /* Write register value back into ENABLE register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_ENABLE, val);
}

/**
 * @brief Gets if proximity interrupts are enabled or not
 *
 * @return 1 if interrupts are enabled, 0 if not. 0xFF on error.
 */
uint8_t getProximityIntEnable(void) {
  uint8_t val;

  /* Read value from ENABLE register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_ENABLE);

  /* Shift and mask out PIEN bit */
  val = (val >> 5) & 0b00000001;

  return val;
}

/**
 * @brief Turns proximity interrupts on or off
 *
 * @param[in] enable 1 to enable interrupts, 0 to turn them off
 */
void setProximityIntEnable(uint8_t enable) {
  uint8_t val;

  /* Read value from ENABLE register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_ENABLE);

  /* Set bits in register to given value */
  enable &= 0b00000001;
  enable = enable << 5;
  val &= 0b11011111;
  val |= enable;

  /* Write register value back into ENABLE register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_ENABLE, val);
}

/**
 * @brief Gets if gesture interrupts are enabled or not
 *
 * @return 1 if interrupts are enabled, 0 if not. 0xFF on error.
 */
uint8_t getGestureIntEnable(void) {
    uint8_t val;

    /* Read value from GCONF4 register */
    val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GCONF4);

    /* Shift and mask out GIEN bit */
    val = (val >> 1) & 0b00000001;

    return val;
}

/**
 * @brief Turns gesture-related interrupts on or off
 *
 * @param[in] enable 1 to enable interrupts, 0 to turn them off
 */
void setGestureIntEnable(uint8_t enable) {
  uint8_t val;

  /* Read value from GCONF4 register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GCONF4);

  /* Set bits in register to given value */
  enable &= 0b00000001;
  enable = enable << 1;
  val &= 0b11111101;
  val |= enable;

  /* Write register value back into GCONF4 register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_GCONF4, val);
}

/**
 * @brief Clears the ambient light interrupt
 *
 */
void clearAmbientLightInt(void) {
  uint8_t throwaway;
  throwaway = I2cRead8(APDS9960_I2C_ADDR, APDS9960_AICLEAR);
}

/**
 * @brief Clears the proximity interrupt
 *
 */
void clearProximityInt(void) {
  uint8_t throwaway;
  throwaway = I2cRead8(APDS9960_I2C_ADDR, APDS9960_PICLEAR);
}

/**
 * @brief Tells if the gesture state machine is currently running
 *
 * @return 1 if gesture state machine is running, 0 if not. 0xFF on error.
 */
uint8_t getGestureMode(void) {
  uint8_t val;

  /* Read value from GCONF4 register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GCONF4);

  /* Mask out GMODE bit */
  val &= 0b00000001;

  return val;
}

/**
 * @brief Tells the state machine to either enter or exit gesture state machine
 *
 * @param[in] mode 1 to enter gesture state machine, 0 to exit.
 */
void setGestureMode(uint8_t mode) {
  uint8_t val;

  /* Read value from GCONF4 register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GCONF4);

  /* Set bits in register to given value */
  mode &= 0b00000001;
  val &= 0b11111110;
  val |= mode;

  /* Write register value back into GCONF4 register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_GCONF4, val);
}


bool APDS9960_init(void) {
  setMode(ALL, OFF);

  /* Set default values for ambient light and proximity registers */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_ATIME, DEFAULT_ATIME);
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_WTIME, DEFAULT_WTIME);
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_PPULSE, DEFAULT_PROX_PPULSE);
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_POFFSET_UR, DEFAULT_POFFSET_UR);
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_POFFSET_DL, DEFAULT_POFFSET_DL);
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_CONFIG1, DEFAULT_CONFIG1);

  setLEDDrive(DEFAULT_LDRIVE);
  setProximityGain(DEFAULT_PGAIN);
  setAmbientLightGain(DEFAULT_AGAIN);
  setProxIntLowThresh(DEFAULT_PILT);
  setProxIntHighThresh(DEFAULT_PIHT);
  setLightIntLowThreshold(DEFAULT_AILT);
  setLightIntHighThreshold(DEFAULT_AIHT);

  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_PERS, DEFAULT_PERS);
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_CONFIG2, DEFAULT_CONFIG2);
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_CONFIG3, DEFAULT_CONFIG3);

  /* Set default values for gesture sense registers */
#ifdef USE_APDS9960_GESTURE
  setGestureEnterThresh(DEFAULT_GPENTH);
  setGestureExitThresh(DEFAULT_GEXTH);

  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_GCONF1, DEFAULT_GCONF1);

  setGestureGain(DEFAULT_GGAIN);
  setGestureLEDDrive(DEFAULT_GLDRIVE);
  setGestureWaitTime(DEFAULT_GWTIME);

  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_GOFFSET_U, DEFAULT_GOFFSET);
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_GOFFSET_D, DEFAULT_GOFFSET);
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_GOFFSET_L, DEFAULT_GOFFSET);
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_GOFFSET_R, DEFAULT_GOFFSET);
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_GPULSE, DEFAULT_GPULSE);
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_GCONF3, DEFAULT_GCONF3);

  setGestureIntEnable(DEFAULT_GIEN);
#endif  // USE_APDS9960_GESTURE

  disablePower();  // go to sleep

  return true;
}


/******************************************************************************\
 * Public methods for controlling the APDS-9960
\******************************************************************************/

/**
 * @brief Reads and returns the contents of the ENABLE register
 *
 * @return Contents of the ENABLE register. 0xFF if error.
 */
inline uint8_t getMode(void) {
  uint8_t enable_value;

  /* Read current ENABLE register */
  enable_value = I2cRead8(APDS9960_I2C_ADDR, APDS9960_ENABLE);

  return enable_value;
}

/**
 * @brief Enables or disables a feature in the APDS-9960
 *
 * @param[in] mode which feature to enable
 * @param[in] enable ON (1) or OFF (0)
 */
void setMode(uint8_t mode, uint8_t enable) {
  uint8_t reg_val;

  /* Read current ENABLE register */
  reg_val = getMode();

  /* Change bit(s) in ENABLE register */
  enable = enable & 0x01;
  if (mode <= 6) {
    if (enable) {
      reg_val |= (1 << mode);
    } else {
      reg_val &= ~(1 << mode);
    }
  } else if (mode == ALL) {
    if (enable) {
      reg_val = 0x7F;
    } else {
      reg_val = 0x00;
    }
  }

  /* Write value back to ENABLE register */
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_ENABLE, reg_val);
}

/**
 * @brief Starts the light (R/G/B/Ambient) sensor on the APDS-9960
 *
 * no interrupts
 */
void enableLightSensor(void) {
  /* Set default gain, interrupts, enable power, and enable sensor */
  setAmbientLightGain(DEFAULT_AGAIN);
  setAmbientLightIntEnable(OFF);
  enablePower();
  setMode(AMBIENT_LIGHT, ON);
}

/**
 * @brief Ends the light sensor on the APDS-9960
 *
 */
void disableLightSensor(void) {
  setAmbientLightIntEnable(OFF);
  setMode(AMBIENT_LIGHT, OFF);
}

/**
 * @brief Starts the proximity sensor on the APDS-9960
 *
 * no interrupts
 */
void enableProximitySensor(void) {
  /* Set default gain, LED, interrupts, enable power, and enable sensor */
  setProximityGain(DEFAULT_PGAIN);
  setLEDDrive(DEFAULT_LDRIVE);
  setProximityIntEnable(OFF);
  enablePower();
  setMode(PROXIMITY, ON);
}

/**
 * @brief Ends the proximity sensor on the APDS-9960
 *
 */
void disableProximitySensor(void) {
  setProximityIntEnable(OFF);
  setMode(PROXIMITY, OFF);
}

#ifdef USE_APDS9960_GESTURE
/**
 * @brief Starts the gesture recognition engine on the APDS-9960
 *
 * no interrupts
 */
void enableGestureSensor(void) {
  /* Enable gesture mode
      Set ENABLE to 0 (power off)
      Set WTIME to 0xFF
      Set AUX to LED_BOOST_300
      Enable PON, WEN, PEN, GEN in ENABLE
  */

  resetGestureParameters();
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_WTIME, 0xFF);
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_PPULSE, DEFAULT_GESTURE_PPULSE);
  setLEDBoost(LED_BOOST_100);  // tip from jonn26 - 100 for 300 ---- 200 from Adafruit
  setGestureIntEnable(OFF);
  setGestureMode(ON);
  enablePower();
  setMode(WAIT, ON);
  setMode(PROXIMITY, ON);
  setMode(GESTURE, ON);
}

/**
 * @brief Ends the gesture recognition engine on the APDS-9960
 *
 */
void disableGestureSensor(void) {
  resetGestureParameters();
  setGestureIntEnable(OFF);
  setGestureMode(OFF);
  setMode(GESTURE, OFF);
}

/**
 * @brief Determines if there is a gesture available for reading
 *
 * @return True if gesture available. False otherwise.
 */
bool isGestureAvailable(void) {
  uint8_t val;

  /* Read value from GSTATUS register */
  val = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GSTATUS);

  /* Shift and mask out GVALID bit */
  val &= APDS9960_GVALID;

  /* Return true/false based on GVALID bit */
  return (val == 1);
}

/**
 * @brief Processes a gesture event and returns best guessed gesture
 *
 * @return Number corresponding to gesture. -1 on error.
 */
int16_t readGesture(void) {
  uint8_t fifo_level = 0;
  uint8_t fifo_data[128];
  uint8_t gstatus;
  int16_t motion;
  uint16_t i;
  uint8_t gesture_loop_counter = 0;  // don't loop forever later
  int8_t bytes_read = 0;

  /* Make sure that power and gesture is on and data is valid */
  if (!isGestureAvailable() || !(getMode() & 0b01000001)) {
    return DIR_NONE;
  }

  /* Keep looping as long as gesture data is valid */
  while (1) {
    if (gesture_loop_counter == APDS9960_MAX_GESTURE_CYCLES) {  // We will escape after a few loops
      disableGestureSensor();     // stop the sensor to prevent problems with power consumption/blocking and return to the main loop
      APDS9960_overload = true;   // we report this as "long"-gesture
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("Sensor overload"));
    }
    gesture_loop_counter += 1;

    /* Wait some time to collect next batch of FIFO data */
    delay(FIFO_PAUSE_TIME);

    /* Get the contents of the STATUS register. Is data still valid? */
    gstatus = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GSTATUS);

    /* If we have valid data, read in FIFO */
    if ((gstatus & APDS9960_GVALID) == APDS9960_GVALID) {
      /* Read the current FIFO level */
      fifo_level = I2cRead8(APDS9960_I2C_ADDR, APDS9960_GFLVL);

#ifdef USE_DEBUG_DRIVER
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DRV: FIFO Level : %d"), fifo_level);
#endif  // USE_DEBUG_DRIVER

      /* If there's stuff in the FIFO, read it into our data block */
      if (fifo_level > 0) {
        bytes_read = (fifo_level * 4);

        if (I2cReadBuffer(APDS9960_I2C_ADDR, APDS9960_GFIFO_U, (uint8_t*)fifo_data, bytes_read)) {
          return APDS9960_ERROR;
        }

#ifdef USE_DEBUG_DRIVER
      char output[(bytes_read * 2) + 1];
      char *ptr = &output[0];

      for ( i = 0; i < bytes_read; i++ ) {
          ptr += sprintf(ptr, "%02X", fifo_data[i]);
      }
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DRV: FIFO Dump : %s"), output);
#endif  // USE_DEBUG_DRIVER

        /* If at least 1 set of data, sort the data into U/D/L/R */
        if (bytes_read >= 4) {
          for (i = 0; i < bytes_read; i += 4) {
            gesture_data.u_data[gesture_data.index] = fifo_data[i + 0];
            gesture_data.d_data[gesture_data.index] = fifo_data[i + 1];
            gesture_data.l_data[gesture_data.index] = fifo_data[i + 2];
            gesture_data.r_data[gesture_data.index] = fifo_data[i + 3];
            gesture_data.index++;
            gesture_data.total_gestures++;
          }

          /* Filter and process gesture data. Decode near/far state */
          if (processGestureData()) {
              if (decodeGesture()) {
                // TODO(xx): U-Turn Gestures
              }
          }
          /* Reset data */
          gesture_data.index = 0;
          gesture_data.total_gestures = 0;
        }
      }
    } else {
      /* Determine best guessed gesture and clean up */
      delay(FIFO_PAUSE_TIME);
      decodeGesture();
      motion = gesture.motion_;
      resetGestureParameters();
      return motion;
    }
  }
}

#endif  // USE_APDS9960_GESTURE

/**
 * Turn the APDS-9960 on
 *
 */
inline void enablePower(void) {
  setMode(POWER, ON);
}

/**
 * Turn the APDS-9960 off
 *
 */
inline void disablePower(void) {
  setMode(POWER, OFF);
}


/******************************************************************************\
 * Ambient light and color sensor controls
\******************************************************************************/

#if defined(USE_APDS9960_COLOR) || defined(USE_APDS9960_PROXIMITY)
/**
 * @brief Reads the ARGB-Data and fills color_data
 */
inline void readAllColorAndProximityData(void) {
  if (I2cReadBuffer(APDS9960_I2C_ADDR, APDS9960_CDATAL, (uint8_t *) &color_data, (uint16_t)9)) {
    // not absolutely shure, if this is a correct way to do this, but it is very short
    // we fill the struct byte by byte
  }
}

void APDS9960_adjustATime(void) {  // not really used atm
  // readAllColorAndProximityData();
  I2cValidRead16LE(&color_data.a, APDS9960_I2C_ADDR, APDS9960_CDATAL);
  // disablePower();

  if (color_data.a < (uint16_t)20) {
    APDS9960_aTime = 0x40;
  } else if (color_data.a < (uint16_t)40) {
    APDS9960_aTime = 0x80;
  } else if (color_data.a < (uint16_t)50) {
    APDS9960_aTime = DEFAULT_ATIME;
  } else if (color_data.a < (uint16_t)70) {
    APDS9960_aTime = 0xc0;
  }

  if (color_data.a < 200) {
    APDS9960_aTime = 0xe9;
  }
  /* if (color_data.a < 10000){
    APDS9960_aTime = 0xF0;
  }*/
  else {
    APDS9960_aTime = 0xff;
  }

  // disableLightSensor();
  I2cWrite8(APDS9960_I2C_ADDR, APDS9960_ATIME, APDS9960_aTime);
  enablePower();
  enableLightSensor();
  delay(20);
}
#endif  // USE_APDS9960_COLOR || USE_APDS9960_PROXIMITY

/******************************************************************************\
 * High-level gesture controls
\******************************************************************************/

#ifdef USE_APDS9960_GESTURE

/**
 * @brief Resets all the parameters in the gesture data member
 */
void resetGestureParameters(void) {
  gesture_data.index = 0;
  gesture_data.total_gestures = 0;

  gesture.ud_delta_ = 0;
  gesture.lr_delta_ = 0;

  gesture.ud_count_ = 0;
  gesture.lr_count_ = 0;

  gesture.state_ = 0;
  gesture.motion_ = DIR_NONE;
}

/**
 * @brief Processes the raw gesture data to determine swipe direction
 *
 * @return True if near or far state seen. False otherwise.
 */
bool processGestureData(void) {
  uint8_t u_first = 0;
  uint8_t d_first = 0;
  uint8_t l_first = 0;
  uint8_t r_first = 0;
  uint8_t u_last = 0;
  uint8_t d_last = 0;
  uint8_t l_last = 0;
  uint8_t r_last = 0;
  uint16_t ud_ratio_first;
  uint16_t lr_ratio_first;
  uint16_t ud_ratio_last;
  uint16_t lr_ratio_last;
  uint16_t ud_delta;
  uint16_t lr_delta;
  uint16_t i;

  /* If we have less than 4 total gestures, that's not enough */
  if (gesture_data.total_gestures <= 4) {
    return false;
  }

  /* Check to make sure our data isn't out of bounds */
  if ((gesture_data.total_gestures <= 32) && \
    (gesture_data.total_gestures > 0)) {
    /* Find the first value in U/D/L/R above the threshold */
    for (i = 0; i < gesture_data.total_gestures; i++) {
      if ((gesture_data.u_data[i] > GESTURE_THRESHOLD_OUT) &&
        (gesture_data.d_data[i] > GESTURE_THRESHOLD_OUT) &&
        (gesture_data.l_data[i] > GESTURE_THRESHOLD_OUT) &&
        (gesture_data.r_data[i] > GESTURE_THRESHOLD_OUT) ) {
        u_first = gesture_data.u_data[i];
        d_first = gesture_data.d_data[i];
        l_first = gesture_data.l_data[i];
        r_first = gesture_data.r_data[i];
        break;
      }
    }

    /* If one of the _first values is 0, then there is no good data */
    if ((u_first == 0) || (d_first == 0) || (l_first == 0) || (r_first == 0)) {
      return false;
    }

    /* Find the last value in U/D/L/R above the threshold */
    for (i = gesture_data.total_gestures - 1; i >= 0; i--) {
      if ((gesture_data.u_data[i] > GESTURE_THRESHOLD_OUT) &&
        (gesture_data.d_data[i] > GESTURE_THRESHOLD_OUT) &&
        (gesture_data.l_data[i] > GESTURE_THRESHOLD_OUT) &&
        (gesture_data.r_data[i] > GESTURE_THRESHOLD_OUT)) {
        u_last = gesture_data.u_data[i];
        d_last = gesture_data.d_data[i];
        l_last = gesture_data.l_data[i];
        r_last = gesture_data.r_data[i];
        break;
      }
    }
  }

  /* Calculate the first vs. last ratio of up/down and left/right */
  ud_ratio_first = ((u_first - d_first) * 100) / (u_first + d_first);
  lr_ratio_first = ((l_first - r_first) * 100) / (l_first + r_first);
  ud_ratio_last = ((u_last - d_last) * 100) / (u_last + d_last);
  lr_ratio_last = ((l_last - r_last) * 100) / (l_last + r_last);

  /* Determine the difference between the first and last ratios */
  ud_delta = ud_ratio_last - ud_ratio_first;
  lr_delta = lr_ratio_last - lr_ratio_first;

  /* Accumulate the UD and LR delta values */
  gesture.ud_delta_ += ud_delta;
  gesture.lr_delta_ += lr_delta;

  /* Determine U/D gesture */
  if (gesture.ud_delta_ >= GESTURE_SENSITIVITY_1) {
    gesture.ud_count_ = 1;
  } else if (gesture.ud_delta_ <= -GESTURE_SENSITIVITY_1) {
    gesture.ud_count_ = -1;
  } else {
    gesture.ud_count_ = 0;
  }

  /* Determine L/R gesture */
  if (gesture.lr_delta_ >= GESTURE_SENSITIVITY_1) {
    gesture.lr_count_ = 1;
  } else if (gesture.lr_delta_ <= -GESTURE_SENSITIVITY_1) {
    gesture.lr_count_ = -1;
  } else {
    gesture.lr_count_ = 0;
  }
  return false;
}

/**
 * @brief Determines swipe direction or near/far state
 *
 * @return True if near/far event. False otherwise.
 */
bool decodeGesture(void) {
  /* Determine swipe direction */
  if ((gesture.ud_count_ == -1) && (gesture.lr_count_ == 0)) {
    gesture.motion_ = DIR_UP;
  } else if ((gesture.ud_count_ == 1) && (gesture.lr_count_ == 0)) {
    gesture.motion_ = DIR_DOWN;
  } else if ((gesture.ud_count_ == 0) && (gesture.lr_count_ == 1)) {
     gesture.motion_ = DIR_RIGHT;
  } else if ((gesture.ud_count_ == 0) && (gesture.lr_count_ == -1)) {
     gesture.motion_ = DIR_LEFT;
  } else if ((gesture.ud_count_ == -1) && (gesture.lr_count_ == 1)) {
    if (abs(gesture.ud_delta_) > abs(gesture.lr_delta_)) {
      gesture.motion_ = DIR_UP;
    } else {
      gesture.motion_ = DIR_RIGHT;
    }
  } else if ((gesture.ud_count_ == 1) && (gesture.lr_count_ == -1)) {
    if (abs(gesture.ud_delta_) > abs(gesture.lr_delta_)) {
      gesture.motion_ = DIR_DOWN;
    } else {
      gesture.motion_ = DIR_LEFT;
    }
  } else if ((gesture.ud_count_ == -1) && (gesture.lr_count_ == -1)) {
    if (abs(gesture.ud_delta_) > abs(gesture.lr_delta_)) {
      gesture.motion_ = DIR_UP;
    } else {
        gesture.motion_ = DIR_LEFT;
    }
  } else if ((gesture.ud_count_ == 1) && (gesture.lr_count_ == 1)) {
    if (abs(gesture.ud_delta_) > abs(gesture.lr_delta_)) {
      gesture.motion_ = DIR_DOWN;
    } else {
      gesture.motion_ = DIR_RIGHT;
    }
  } else {
    return false;
  }

  return true;
}

void handleGesture(void) {
  if (isGestureAvailable()) {
    switch (readGesture()) {
      case DIR_UP:
        AddLog_P(LOG_LEVEL_DEBUG, GESTURE_UP);
        snprintf_P(currentGesture, sizeof(currentGesture), GESTURE_UP);
        break;
      case DIR_DOWN:
        AddLog_P(LOG_LEVEL_DEBUG, GESTURE_DOWN);
        snprintf_P(currentGesture, sizeof(currentGesture), GESTURE_DOWN);
        break;
      case DIR_LEFT:
        AddLog_P(LOG_LEVEL_DEBUG, GESTURE_LEFT);
        snprintf_P(currentGesture, sizeof(currentGesture), GESTURE_LEFT);
        break;
      case DIR_RIGHT:
        AddLog_P(LOG_LEVEL_DEBUG, GESTURE_RIGHT);
        snprintf_P(currentGesture, sizeof(currentGesture), GESTURE_RIGHT);
        break;
      default:
        if (APDS9960_overload) {
          AddLog_P(LOG_LEVEL_DEBUG, GESTURE_LONG);
          snprintf_P(currentGesture, sizeof(currentGesture), GESTURE_LONG);
        } else {
          AddLog_P(LOG_LEVEL_DEBUG, GESTURE_NONE);
          snprintf_P(currentGesture, sizeof(currentGesture), GESTURE_NONE);
        }
        break;
    }
    MqttPublishSensor();
  }
  currentGesture[0] = '\0';
}

void APDS9960_loop(void) {
  if (recovery_loop_counter > 0) {
    recovery_loop_counter -= 1;
  }

  if (recovery_loop_counter == 1 && APDS9960_overload) {  // restart sensor just before the end of recovery from long press
    enableGestureSensor();
    APDS9960_overload = false;
    Response_P(PSTR("{\"Gesture\":\"On\"}"));
    MqttPublishPrefixTopic_P(RESULT_OR_TELE, mqtt_data);  // only after the long break we report, that we are online again
    gesture_mode = 1;
  }

  if (gesture_mode) {
    if (recovery_loop_counter == 0) {
      handleGesture();

      if (APDS9960_overload) {
        disableGestureSensor();
        recovery_loop_counter = APDS9960_LONG_RECOVERY;  // long pause after overload/long press - number of stateloops
        Response_P(PSTR("{\"Gesture\":\"Off\"}"));
        MqttPublishPrefixTopic_P(RESULT_OR_TELE, mqtt_data);
        gesture_mode = 0;
      }
    }
  }
}

#endif  // USE_APDS9960_GESTURE

void APDS9960_detect(void) {
  if (APDS9960_type || I2cActive(APDS9960_I2C_ADDR)) { return; }

  APDS9960_type = I2cRead8(APDS9960_I2C_ADDR, APDS9960_ID);

#ifdef USE_DEBUG_DRIVER
  // Debug new chip
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("DRV: %s Chip %X"), APDS9960_TAG, APDS9960_type);
#endif  // USE_DEBUG_DRIVER

  if (APDS9960_type == APDS9960_CHIPID_1 || APDS9960_type == APDS9960_CHIPID_2 || APDS9960_type == APDS9960_CHIPID_3) {
    if (APDS9960_init()) {
      I2cSetActiveFound(APDS9960_I2C_ADDR, APDS9960_TAG);

      enableProximitySensor();

#if defined(USE_APDS9960_GESTURE) && USE_APDS9960_STARTMODE == APDS9960_MODE_GESTURE
      gesture_mode = 1;
      enableGestureSensor();
#endif  // USE_APDS9960_GESTURE

#if ( defined(USE_APDS9960_COLOR) || defined(USE_APDS9960_PROXIMITY) ) && USE_APDS9960_STARTMODE == APDS9960_MODE_COLOR
      gesture_mode = 0;
      enableLightSensor();
      APDS9960_overload = false;
#endif  // USE_APDS9960_GESTURE
    } else {
      APDS9960_type = 0;
    }
  } else {
    APDS9960_type = 0;
  }

#ifdef USE_APDS9960_GESTURE
  currentGesture[0] = '\0';
#endif  // USE_APDS9960_GESTURE
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void APDS9960_show(bool json) {
  if (!APDS9960_type) { return; }

  if (!gesture_mode && !APDS9960_overload) {

#if defined(USE_APDS9960_COLOR) || defined(USE_APDS9960_PROXIMITY)
    uint16_t ambient;

    readAllColorAndProximityData();
    ambient = color_data.a/4;

    /*  disableLightSensor();
    I2cWrite8(APDS9960_I2C_ADDR, APDS9960_ATIME, DEFAULT_ATIME); // reset to default
    enableLightSensor();*/

#ifdef USE_APDS9960_COLOR
    calculateColorTemperature();  // and calculate Lux
#endif  // USE_APDS9960_COLOR
    if (json) {
#if defined(USE_APDS9960_COLOR) && defined(USE_APDS9960_PROXIMITY)
      ResponseAppend_P(PSTR(",\"%s\":{\"Red\":%u,\"Green\":%u,\"Blue\":%u,\"" D_JSON_ILLUMINANCE "\":%u,\"CCT\":%u,\"Proximity\":%u}"),
        APDS9960_TAG,
        color_data.r,
        color_data.g,
        color_data.b,
        ambient,
        color_data.cct,
        color_data.p);
#else

#ifdef USE_APDS9960_COLOR
      ResponseAppend_P(PSTR(",\"%s\":{\"Red\":%u,\"Green\":%u,\"Blue\":%u,\"" D_JSON_ILLUMINANCE "\":%u,\"CCT\":%u}"),
        APDS9960_TAG,
        color_data.r,
        color_data.g,
        color_data.b,
        ambient,
        color_data.cct);
#endif  // USE_APDS9960_COLOR

#ifdef USE_APDS9960_PROXIMITY
      ResponseAppend_P(PSTR(",\"%s\":{\"Proximity\":%u}"),
        APDS9960_TAG,
        color_data.p);
#endif  // USE_APDS9960_PROXIMITY

#endif  // USE_APDS9960_COLOR && USE_APDS9960_PROXIMITY
#ifdef USE_WEBSERVER
    } else {

#ifdef USE_APDS9960_COLOR
      WSContentSend_PD(HTTP_SNS_COLOR_RED,   APDS9960_TAG, color_data.r);
      WSContentSend_PD(HTTP_SNS_COLOR_GREEN, APDS9960_TAG, color_data.g);
      WSContentSend_PD(HTTP_SNS_COLOR_BLUE,  APDS9960_TAG, color_data.b);
      WSContentSend_PD(HTTP_SNS_ILLUMINANCE, APDS9960_TAG, ambient);
      WSContentSend_PD(HTTP_SNS_CCT,         APDS9960_TAG, color_data.cct);
#endif  // USE_APDS9960_COLOR

#ifdef USE_APDS9960_PROXIMITY
      WSContentSend_PD(HTTP_SNS_PROXIMITY,   APDS9960_TAG, color_data.p);
#endif  // USE_APDS9960_PROXIMITY

#endif  // USE_WEBSERVER
    }
#endif  // USE_APDS9960_COLOR || USE_APDS9960_PROXIMITY

#ifdef USE_APDS9960_GESTURE
  } else {
    if (currentGesture[0] != '\0') {
      if (json) {
        ResponseAppend_P(PSTR(",\"%s\":{\"%s\":1}"), APDS9960_TAG, currentGesture);
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(HTTP_SNS_GESTURE, APDS9960_TAG, currentGesture);
#endif  // USE_WEBSERVER
        currentGesture[0] = '\0';
      }
    }
#endif  // USE_APDS9960_GESTURE

  }
}

/*********************************************************************************************\
 * Command Sensor27
 *
 * Command  | Payload | Description
 * ---------|---------|--------------------------
 * Sensor27 |         | Show current gesture mode
 * Sensor27 | 0 / Off | Disable gesture mode
 * Sensor27 | 1 / On  | Enable gesture mode
 * Sensor27 | 2 / On  | Enable gesture mode with half gain
\*********************************************************************************************/

bool APDS9960CommandSensor(void) {
  bool serviced = true;

  switch (XdrvMailbox.payload) {
    case 0:  // Off
#ifdef USE_APDS9960_GESTURE
      disableGestureSensor();
#endif  // USE_APDS9960_GESTURE
      gesture_mode = 0;
      enableLightSensor();
      APDS9960_overload = false;  // prevent unwanted re-enabling
      break;
#ifdef USE_APDS9960_GESTURE
    case 1:  // On with default gain of 4x
      if (APDS9960_type) {
        setGestureGain(DEFAULT_GGAIN);
        setProximityGain(DEFAULT_PGAIN);
        disableLightSensor();
        enableGestureSensor();
        gesture_mode = 1;
      }
      break;
    case 2:  // gain of 2x , needed for some models
      if (APDS9960_type) {
        setGestureGain(GGAIN_2X);
        setProximityGain(PGAIN_2X);
        disableLightSensor();
        enableGestureSensor();
        gesture_mode = 1;
      }
      break;
#endif  // USE_APDS9960_GESTURE
    default:
      int temp_aTime = (uint8_t)XdrvMailbox.payload;
      if (temp_aTime > 2 && temp_aTime < 256) {
        disablePower();
        I2cWrite8(APDS9960_I2C_ADDR, APDS9960_ATIME, temp_aTime);
        enablePower();
        enableLightSensor();
      }
      break;
  }
  Response_P(S_JSON_SENSOR_INDEX_SVALUE, XSNS_27, GetStateText(gesture_mode));

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns27(uint8_t function) {
  if (!I2cEnabled(XI2C_21)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    APDS9960_detect();
  } else if (APDS9960_type) {
    switch (function) {
#ifdef USE_APDS9960_GESTURE
      case FUNC_EVERY_50_MSECOND:
        APDS9960_loop();
        break;
#endif  // USE_APDS9960_GESTURE
      case FUNC_COMMAND_SENSOR:
        if (XSNS_27 == XdrvMailbox.index) {
          result = APDS9960CommandSensor();
        }
        break;
      case FUNC_JSON_APPEND:
        APDS9960_show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        APDS9960_show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_APDS9960
#endif  // USE_I2C
