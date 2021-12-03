#ifndef OneWire_h
#define OneWire_h

#include <inttypes.h>

#if defined(__AVR__)
#include <util/crc16.h>
#endif

#if ARDUINO >= 100
#include "Arduino.h"       // for delayMicroseconds, digitalPinToBitMask, etc
#else
#include "WProgram.h"      // for delayMicroseconds
#include "pins_arduino.h"  // for digitalPinToBitMask, etc
#endif

// You can exclude certain features from OneWire.  In theory, this
// might save some space.  In practice, the compiler automatically
// removes unused code (technically, the linker, using -fdata-sections
// and -ffunction-sections when compiling, and Wl,--gc-sections
// when linking), so most of these will not result in any code size
// reduction.  Well, unless you try to use the missing features
// and redesign your program to not need them!  ONEWIRE_CRC8_TABLE
// is the exception, because it selects a fast but large algorithm
// or a small but slow algorithm.

// you can exclude onewire_search by defining that to 0
#ifndef ONEWIRE_SEARCH
#define ONEWIRE_SEARCH 1
#endif

// You can exclude CRC checks altogether by defining this to 0
#ifndef ONEWIRE_CRC
#define ONEWIRE_CRC 1
#endif

// Select the table-lookup method of computing the 8-bit CRC
// by setting this to 1.  The lookup table enlarges code size by
// about 250 bytes.  It does NOT consume RAM (but did in very
// old versions of OneWire).  If you disable this, a slower
// but very compact algorithm is used.
#ifndef ONEWIRE_CRC8_TABLE
#define ONEWIRE_CRC8_TABLE 0
#endif

// You can allow 16-bit CRC checks by defining this to 1
// (Note that ONEWIRE_CRC must also be 1.)
#ifndef ONEWIRE_CRC16
#define ONEWIRE_CRC16 1
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE  1
#endif

// Platform specific I/O definitions

#if defined(__AVR__)
#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint8_t
#define IO_REG_BASE_ATTR asm("r30")
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, mask)         (((*(base)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   ((*((base)+1)) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask)  ((*((base)+1)) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)    ((*((base)+2)) &= ~(mask))
#define DIRECT_WRITE_HIGH(base, mask)   ((*((base)+2)) |= (mask))

#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK66FX1M0__) || defined(__MK64FX512__)
#define PIN_TO_BASEREG(pin)             (portOutputRegister(pin))
#define PIN_TO_BITMASK(pin)             (1)
#define IO_REG_TYPE uint8_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR __attribute__ ((unused))
#define DIRECT_READ(base, mask)         (*((base)+512))
#define DIRECT_MODE_INPUT(base, mask)   (*((base)+640) = 0)
#define DIRECT_MODE_OUTPUT(base, mask)  (*((base)+640) = 1)
#define DIRECT_WRITE_LOW(base, mask)    (*((base)+256) = 1)
#define DIRECT_WRITE_HIGH(base, mask)   (*((base)+128) = 1)

#elif defined(__MKL26Z64__)
#define PIN_TO_BASEREG(pin)             (portOutputRegister(pin))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint8_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, mask)         ((*((base)+16) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   (*((base)+20) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask)  (*((base)+20) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)    (*((base)+8) = (mask))
#define DIRECT_WRITE_HIGH(base, mask)   (*((base)+4) = (mask))

#elif defined(__SAM3X8E__) || defined(__SAM3A8C__) || defined(__SAM3A4C__)
// Arduino 1.5.1 may have a bug in delayMicroseconds() on Arduino Due.
// http://arduino.cc/forum/index.php/topic,141030.msg1076268.html#msg1076268
// If you have trouble with OneWire on Arduino Due, please check the
// status of delayMicroseconds() before reporting a bug in OneWire!
#define PIN_TO_BASEREG(pin)             (&(digitalPinToPort(pin)->PIO_PER))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, mask)         (((*((base)+15)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   ((*((base)+5)) = (mask))
#define DIRECT_MODE_OUTPUT(base, mask)  ((*((base)+4)) = (mask))
#define DIRECT_WRITE_LOW(base, mask)    ((*((base)+13)) = (mask))
#define DIRECT_WRITE_HIGH(base, mask)   ((*((base)+12)) = (mask))
#ifndef PROGMEM
#define PROGMEM
#endif
#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const uint8_t *)(addr))
#endif

#elif defined(__PIC32MX__)
#define PIN_TO_BASEREG(pin)             (portModeRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, mask)         (((*(base+4)) & (mask)) ? 1 : 0)  //PORTX + 0x10
#define DIRECT_MODE_INPUT(base, mask)   ((*(base+2)) = (mask))            //TRISXSET + 0x08
#define DIRECT_MODE_OUTPUT(base, mask)  ((*(base+1)) = (mask))            //TRISXCLR + 0x04
#define DIRECT_WRITE_LOW(base, mask)    ((*(base+8+1)) = (mask))          //LATXCLR  + 0x24
#define DIRECT_WRITE_HIGH(base, mask)   ((*(base+8+2)) = (mask))          //LATXSET + 0x28

#elif defined(ARDUINO_ARCH_ESP8266)
// Special note: I depend on the ESP community to maintain these definitions and
// submit good pull requests.  I can not answer any ESP questions or help you
// resolve any problems related to ESP chips.  Please do not contact me and please
// DO NOT CREATE GITHUB ISSUES for ESP support.  All ESP questions must be asked
// on ESP community forums.
#define PIN_TO_BASEREG(pin)             ((volatile uint32_t*) GPO)
#define PIN_TO_BITMASK(pin)             (1 << pin)
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, mask)         ((GPI & (mask)) ? 1 : 0)    //GPIO_IN_ADDRESS
#define DIRECT_MODE_INPUT(base, mask)   (GPE &= ~(mask))            //GPIO_ENABLE_W1TC_ADDRESS
#define DIRECT_MODE_OUTPUT(base, mask)  (GPE |= (mask))             //GPIO_ENABLE_W1TS_ADDRESS
#define DIRECT_WRITE_LOW(base, mask)    (GPOC = (mask))             //GPIO_OUT_W1TC_ADDRESS
#define DIRECT_WRITE_HIGH(base, mask)   (GPOS = (mask))             //GPIO_OUT_W1TS_ADDRESS

#elif defined(ARDUINO_ARCH_ESP32)
#include <driver/rtc_io.h>
#define PIN_TO_BASEREG(pin)             (0)
#define PIN_TO_BITMASK(pin)             (pin)
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR

static inline __attribute__((always_inline))
IO_REG_TYPE directRead(IO_REG_TYPE pin)
{
#if CONFIG_IDF_TARGET_ESP32C3
    return (GPIO.in.val >> pin) & 0x1;
#else // plain ESP32
    if ( pin < 32 )
        return (GPIO.in >> pin) & 0x1;
    else if ( pin < 40 )
        return (GPIO.in1.val >> (pin - 32)) & 0x1;
#endif

    return 0;
}

static inline __attribute__((always_inline))
void directWriteLow(IO_REG_TYPE pin)
{
#if CONFIG_IDF_TARGET_ESP32C3
    GPIO.out_w1tc.val = ((uint32_t)1 << pin);
#else // plain ESP32
    if ( pin < 32 )
        GPIO.out_w1tc = ((uint32_t)1 << pin);
    else if ( pin < 34 )
        GPIO.out1_w1tc.val = ((uint32_t)1 << (pin - 32));
#endif
}

static inline __attribute__((always_inline))
void directWriteHigh(IO_REG_TYPE pin)
{
#if CONFIG_IDF_TARGET_ESP32C3
    GPIO.out_w1ts.val = ((uint32_t)1 << pin);
#else // plain ESP32
    if ( pin < 32 )
        GPIO.out_w1ts = ((uint32_t)1 << pin);
    else if ( pin < 34 )
        GPIO.out1_w1ts.val = ((uint32_t)1 << (pin - 32));
#endif
}

static inline __attribute__((always_inline))
void directModeInput(IO_REG_TYPE pin)
{
#if CONFIG_IDF_TARGET_ESP32C3
    GPIO.enable_w1tc.val = ((uint32_t)1 << (pin));
#else
    if ( digitalPinIsValid(pin) )
    {
#if ESP_IDF_VERSION_MAJOR < 4      // IDF 3.x ESP32/PICO-D4
        uint32_t rtc_reg(rtc_gpio_desc[pin].reg);

        if ( rtc_reg ) // RTC pins PULL settings
        {
            ESP_REG(rtc_reg) = ESP_REG(rtc_reg) & ~(rtc_gpio_desc[pin].mux);
            ESP_REG(rtc_reg) = ESP_REG(rtc_reg) & ~(rtc_gpio_desc[pin].pullup | rtc_gpio_desc[pin].pulldown);
        }
#elif ESP_IDF_VERSION_MAJOR > 3  // ESP32-S2 needs IDF 4.2 or later
        int rtcio_num = rtc_io_number_get((gpio_num_t)pin);

        if (rtcio_num >= 0) {
            uint32_t rtc_reg(rtc_io_desc[rtcio_num].reg);

            if ( rtc_reg ) // RTC pins PULL settings
            {
                ESP_REG(rtc_reg) = ESP_REG(rtc_reg) & ~(rtc_io_desc[rtcio_num].mux);
                ESP_REG(rtc_reg) = ESP_REG(rtc_reg) & ~(rtc_io_desc[rtcio_num].pullup | rtc_io_desc[rtcio_num].pulldown);
            }
        }
#endif

        if ( pin < 32 )
            GPIO.enable_w1tc = ((uint32_t)1 << pin);
        else
            GPIO.enable1_w1tc.val = ((uint32_t)1 << (pin - 32));

        uint32_t pinFunction((uint32_t)2 << FUN_DRV_S); // what are the drivers?
        pinFunction |= FUN_IE; // input enable but required for output as well?
        pinFunction |= ((uint32_t)2 << MCU_SEL_S);

        ESP_REG(DR_REG_IO_MUX_BASE + esp32_gpioMux[pin].reg) = pinFunction;

        GPIO.pin[pin].val = 0;
    }
#endif
}

static inline __attribute__((always_inline))
void directModeOutput(IO_REG_TYPE pin)
{
#if CONFIG_IDF_TARGET_ESP32C3
    GPIO.enable_w1ts.val = ((uint32_t)1 << (pin));
#else
    if ( digitalPinIsValid(pin) && pin <= 33 ) // pins above 33 can be only inputs
    {
#if ESP_IDF_VERSION_MAJOR < 4      // IDF 3.x ESP32/PICO-D4
        uint32_t rtc_reg(rtc_gpio_desc[pin].reg);

        if ( rtc_reg ) // RTC pins PULL settings
        {
            ESP_REG(rtc_reg) = ESP_REG(rtc_reg) & ~(rtc_gpio_desc[pin].mux);
            ESP_REG(rtc_reg) = ESP_REG(rtc_reg) & ~(rtc_gpio_desc[pin].pullup | rtc_gpio_desc[pin].pulldown);
        }
#elif ESP_IDF_VERSION_MAJOR > 3  // ESP32-S2 needs IDF 4.2 or later
        int rtcio_num = rtc_io_number_get((gpio_num_t)pin);

        if (rtcio_num >= 0) {
            uint32_t rtc_reg(rtc_io_desc[rtcio_num].reg);

            if ( rtc_reg ) // RTC pins PULL settings
            {
                ESP_REG(rtc_reg) = ESP_REG(rtc_reg) & ~(rtc_io_desc[rtcio_num].mux);
                ESP_REG(rtc_reg) = ESP_REG(rtc_reg) & ~(rtc_io_desc[rtcio_num].pullup | rtc_io_desc[rtcio_num].pulldown);
            }
        }
#endif

        if ( pin < 32 )
            GPIO.enable_w1ts = ((uint32_t)1 << pin);
        else // already validated to pins <= 33
            GPIO.enable1_w1ts.val = ((uint32_t)1 << (pin - 32));

        uint32_t pinFunction((uint32_t)2 << FUN_DRV_S); // what are the drivers?
        pinFunction |= FUN_IE; // input enable but required for output as well?
        pinFunction |= ((uint32_t)2 << MCU_SEL_S);

        ESP_REG(DR_REG_IO_MUX_BASE + esp32_gpioMux[pin].reg) = pinFunction;

        GPIO.pin[pin].val = 0;
    }
#endif
}

#define DIRECT_READ(base, pin)          directRead(pin)
#define DIRECT_WRITE_LOW(base, pin)     directWriteLow(pin)
#define DIRECT_WRITE_HIGH(base, pin)    directWriteHigh(pin)
#define DIRECT_MODE_INPUT(base, pin)    directModeInput(pin)
#define DIRECT_MODE_OUTPUT(base, pin)   directModeOutput(pin)
//#warning "ESP32 OneWire testing"

#elif defined(__SAMD21G18A__)
#define PIN_TO_BASEREG(pin)             portModeRegister(digitalPinToPort(pin))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, mask)         (((*((base)+8)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   ((*((base)+1)) = (mask))
#define DIRECT_MODE_OUTPUT(base, mask)  ((*((base)+2)) = (mask))
#define DIRECT_WRITE_LOW(base, mask)    ((*((base)+5)) = (mask))
#define DIRECT_WRITE_HIGH(base, mask)   ((*((base)+6)) = (mask))

#elif defined(RBL_NRF51822)
#define PIN_TO_BASEREG(pin)             (0)
#define PIN_TO_BITMASK(pin)             (pin)
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, pin)          nrf_gpio_pin_read(pin)
#define DIRECT_WRITE_LOW(base, pin)     nrf_gpio_pin_clear(pin)
#define DIRECT_WRITE_HIGH(base, pin)    nrf_gpio_pin_set(pin)
#define DIRECT_MODE_INPUT(base, pin)    nrf_gpio_cfg_input(pin, NRF_GPIO_PIN_NOPULL)
#define DIRECT_MODE_OUTPUT(base, pin)   nrf_gpio_cfg_output(pin)

#elif defined(__arc__) /* Arduino101/Genuino101 specifics */

#include "scss_registers.h"
#include "portable.h"
#include "avr/pgmspace.h"

#define GPIO_ID(pin)      (g_APinDescription[pin].ulGPIOId)
#define GPIO_TYPE(pin)      (g_APinDescription[pin].ulGPIOType)
#define GPIO_BASE(pin)      (g_APinDescription[pin].ulGPIOBase)
#define DIR_OFFSET_SS     0x01
#define DIR_OFFSET_SOC      0x04
#define EXT_PORT_OFFSET_SS    0x0A
#define EXT_PORT_OFFSET_SOC   0x50

/* GPIO registers base address */
#define PIN_TO_BASEREG(pin)   ((volatile uint32_t *)g_APinDescription[pin].ulGPIOBase)
#define PIN_TO_BITMASK(pin)   pin
#define IO_REG_TYPE     uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR

static inline __attribute__((always_inline))
IO_REG_TYPE directRead(volatile IO_REG_TYPE *base, IO_REG_TYPE pin)
{
    IO_REG_TYPE ret;
    if (SS_GPIO == GPIO_TYPE(pin)) {
        ret = READ_ARC_REG(((IO_REG_TYPE)base + EXT_PORT_OFFSET_SS));
    } else {
        ret = MMIO_REG_VAL_FROM_BASE((IO_REG_TYPE)base, EXT_PORT_OFFSET_SOC);
    }
    return ((ret >> GPIO_ID(pin)) & 0x01);
}

static inline __attribute__((always_inline))
void directModeInput(volatile IO_REG_TYPE *base, IO_REG_TYPE pin)
{
    if (SS_GPIO == GPIO_TYPE(pin)) {
        WRITE_ARC_REG(READ_ARC_REG((((IO_REG_TYPE)base) + DIR_OFFSET_SS)) & ~(0x01 << GPIO_ID(pin)),
      ((IO_REG_TYPE)(base) + DIR_OFFSET_SS));
    } else {
        MMIO_REG_VAL_FROM_BASE((IO_REG_TYPE)base, DIR_OFFSET_SOC) &= ~(0x01 << GPIO_ID(pin));
    }
}

static inline __attribute__((always_inline))
void directModeOutput(volatile IO_REG_TYPE *base, IO_REG_TYPE pin)
{
    if (SS_GPIO == GPIO_TYPE(pin)) {
        WRITE_ARC_REG(READ_ARC_REG(((IO_REG_TYPE)(base) + DIR_OFFSET_SS)) | (0x01 << GPIO_ID(pin)),
      ((IO_REG_TYPE)(base) + DIR_OFFSET_SS));
    } else {
        MMIO_REG_VAL_FROM_BASE((IO_REG_TYPE)base, DIR_OFFSET_SOC) |= (0x01 << GPIO_ID(pin));
    }
}

static inline __attribute__((always_inline))
void directWriteLow(volatile IO_REG_TYPE *base, IO_REG_TYPE pin)
{
    if (SS_GPIO == GPIO_TYPE(pin)) {
        WRITE_ARC_REG(READ_ARC_REG(base) & ~(0x01 << GPIO_ID(pin)), base);
    } else {
        MMIO_REG_VAL(base) &= ~(0x01 << GPIO_ID(pin));
    }
}

static inline __attribute__((always_inline))
void directWriteHigh(volatile IO_REG_TYPE *base, IO_REG_TYPE pin)
{
    if (SS_GPIO == GPIO_TYPE(pin)) {
        WRITE_ARC_REG(READ_ARC_REG(base) | (0x01 << GPIO_ID(pin)), base);
    } else {
        MMIO_REG_VAL(base) |= (0x01 << GPIO_ID(pin));
    }
}

#define DIRECT_READ(base, pin)    directRead(base, pin)
#define DIRECT_MODE_INPUT(base, pin)  directModeInput(base, pin)
#define DIRECT_MODE_OUTPUT(base, pin) directModeOutput(base, pin)
#define DIRECT_WRITE_LOW(base, pin) directWriteLow(base, pin)
#define DIRECT_WRITE_HIGH(base, pin)  directWriteHigh(base, pin)

#elif defined(__riscv)

/*
 * Tested on highfive1
 *
 * Stable results are achieved operating in the
 * two high speed modes of the highfive1.  It
 * seems to be less reliable in slow mode.
 */
#define PIN_TO_BASEREG(pin)             (0)
#define PIN_TO_BITMASK(pin)             digitalPinToBitMask(pin)
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR

static inline __attribute__((always_inline))
IO_REG_TYPE directRead(IO_REG_TYPE mask)
{
    return ((GPIO_REG(GPIO_INPUT_VAL) & mask) != 0) ? 1 : 0;
}

static inline __attribute__((always_inline))
void directModeInput(IO_REG_TYPE mask)
{
    GPIO_REG(GPIO_OUTPUT_XOR)  &= ~mask;
    GPIO_REG(GPIO_IOF_EN)      &= ~mask;

    GPIO_REG(GPIO_INPUT_EN)  |=  mask;
    GPIO_REG(GPIO_OUTPUT_EN) &= ~mask;
}

static inline __attribute__((always_inline))
void directModeOutput(IO_REG_TYPE mask)
{
    GPIO_REG(GPIO_OUTPUT_XOR)  &= ~mask;
    GPIO_REG(GPIO_IOF_EN)      &= ~mask;

    GPIO_REG(GPIO_INPUT_EN)  &= ~mask;
    GPIO_REG(GPIO_OUTPUT_EN) |=  mask;
}

static inline __attribute__((always_inline))
void directWriteLow(IO_REG_TYPE mask)
{
    GPIO_REG(GPIO_OUTPUT_VAL) &= ~mask;
}

static inline __attribute__((always_inline))
void directWriteHigh(IO_REG_TYPE mask)
{
    GPIO_REG(GPIO_OUTPUT_VAL) |= mask;
}

#define DIRECT_READ(base, mask)          directRead(mask)
#define DIRECT_WRITE_LOW(base, mask)     directWriteLow(mask)
#define DIRECT_WRITE_HIGH(base, mask)    directWriteHigh(mask)
#define DIRECT_MODE_INPUT(base, mask)    directModeInput(mask)
#define DIRECT_MODE_OUTPUT(base, mask)   directModeOutput(mask)

#else
#define PIN_TO_BASEREG(pin)             (0)
#define PIN_TO_BITMASK(pin)             (pin)
#define IO_REG_TYPE unsigned int
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, pin)          digitalRead(pin)
#define DIRECT_WRITE_LOW(base, pin)     digitalWrite(pin, LOW)
#define DIRECT_WRITE_HIGH(base, pin)    digitalWrite(pin, HIGH)
#define DIRECT_MODE_INPUT(base, pin)    pinMode(pin,INPUT)
#define DIRECT_MODE_OUTPUT(base, pin)   pinMode(pin,OUTPUT)
#warning "OneWire. Fallback mode. Using API calls for pinMode,digitalRead and digitalWrite. Operation of this library is not guaranteed on this architecture."

#endif


class OneWire
{
  private:
    IO_REG_TYPE bitmask;
    volatile IO_REG_TYPE *baseReg;

#if ONEWIRE_SEARCH
    // global search state
    unsigned char ROM_NO[8];
    uint8_t LastDiscrepancy;
    uint8_t LastFamilyDiscrepancy;
    uint8_t LastDeviceFlag;
#endif

  public:
    OneWire( uint8_t pin);

    // Perform a 1-Wire reset cycle. Returns 1 if a device responds
    // with a presence pulse.  Returns 0 if there is no device or the
    // bus is shorted or otherwise held low for more than 250uS
    uint8_t reset(void);

    // Issue a 1-Wire rom select command, you do the reset first.
    void select(const uint8_t rom[8]);

    // Issue a 1-Wire rom skip command, to address all on bus.
    void skip(void);

    // Write a byte. If 'power' is one then the wire is held high at
    // the end for parasitically powered devices. You are responsible
    // for eventually depowering it by calling depower() or doing
    // another read or write.
    void write(uint8_t v, uint8_t power = 0);

    void write_bytes(const uint8_t *buf, uint16_t count, bool power = 0);

    // Read a byte.
    uint8_t read(void);

    void read_bytes(uint8_t *buf, uint16_t count);

    // Write a bit. The bus is always left powered at the end, see
    // note in write() about that.
    void write_bit(uint8_t v);

    // Read a bit.
    uint8_t read_bit(void);

    // Stop forcing power onto the bus. You only need to do this if
    // you used the 'power' flag to write() or used a write_bit() call
    // and aren't about to do another read or write. You would rather
    // not leave this powered if you don't have to, just in case
    // someone shorts your bus.
    void depower(void);

#if ONEWIRE_SEARCH
    // Clear the search state so that if will start from the beginning again.
    void reset_search();

    // Setup the search to find the device type 'family_code' on the next call
    // to search(*newAddr) if it is present.
    void target_search(uint8_t family_code);

    // Look for the next device. Returns 1 if a new address has been
    // returned. A zero might mean that the bus is shorted, there are
    // no devices, or you have already retrieved all of them.  It
    // might be a good idea to check the CRC to make sure you didn't
    // get garbage.  The order is deterministic. You will always get
    // the same devices in the same order.
    uint8_t search(uint8_t *newAddr, bool search_mode = true);
#endif

#if ONEWIRE_CRC
    // Compute a Dallas Semiconductor 8 bit CRC, these are used in the
    // ROM and scratchpad registers.
    static uint8_t crc8(const uint8_t *addr, uint8_t len);

#if ONEWIRE_CRC16
    // Compute the 1-Wire CRC16 and compare it against the received CRC.
    // Example usage (reading a DS2408):
    //    // Put everything in a buffer so we can compute the CRC easily.
    //    uint8_t buf[13];
    //    buf[0] = 0xF0;    // Read PIO Registers
    //    buf[1] = 0x88;    // LSB address
    //    buf[2] = 0x00;    // MSB address
    //    WriteBytes(net, buf, 3);    // Write 3 cmd bytes
    //    ReadBytes(net, buf+3, 10);  // Read 6 data bytes, 2 0xFF, 2 CRC16
    //    if (!CheckCRC16(buf, 11, &buf[11])) {
    //        // Handle error.
    //    }
    //
    // @param input - Array of bytes to checksum.
    // @param len - How many bytes to use.
    // @param inverted_crc - The two CRC16 bytes in the received data.
    //                       This should just point into the received data,
    //                       *not* at a 16-bit integer.
    // @param crc - The crc starting value (optional)
    // @return True, iff the CRC matches.
    static bool check_crc16(const uint8_t* input, uint16_t len, const uint8_t* inverted_crc, uint16_t crc = 0);

    // Compute a Dallas Semiconductor 16 bit CRC.  This is required to check
    // the integrity of data received from many 1-Wire devices.  Note that the
    // CRC computed here is *not* what you'll get from the 1-Wire network,
    // for two reasons:
    //   1) The CRC is transmitted bitwise inverted.
    //   2) Depending on the endian-ness of your processor, the binary
    //      representation of the two-byte return value may have a different
    //      byte order than the two bytes you get from 1-Wire.
    // @param input - Array of bytes to checksum.
    // @param len - How many bytes to use.
    // @param crc - The crc starting value (optional)
    // @return The CRC16, as defined by Dallas Semiconductor.
    static uint16_t crc16(const uint8_t* input, uint16_t len, uint16_t crc = 0);
#endif
#endif
};

#endif
