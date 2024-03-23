#if !defined(_RADIOLIB_MODULE_H)
#define _RADIOLIB_MODULE_H

#include "TypeDef.h"
#include "Hal.h"

#if defined(RADIOLIB_BUILD_ARDUINO)
  #include <SPI.h>
#endif

#if defined(STM32WLxx)
  #include <SubGhz.h>
#endif

/*!
* Value to use as the last element in a mode table to indicate the
* end of the table.
*
* See setRfSwitchTable() for details.
*/
#define END_OF_MODE_TABLE    { Module::MODE_END_OF_TABLE, {} }

// default timeout for SPI transfers
#define RADIOLIB_MODULE_SPI_TIMEOUT                             (1000)

/*!
  \class Module
  \brief Implements all common low-level methods to control the wireless module.
  Every module class contains one private instance of this class.
*/
class Module {
  public:
    /*!
     * \brief The maximum number of pins supported by the RF switch
     * code.
     *
     * Note: It is not recommended to use this constant in your sketch
     * when defining a rfswitch pins array, to prevent issues when this
     * value is ever increased and such an array gets extra zero
     * elements (that will be interpreted as pin 0).
     */
    static const size_t RFSWITCH_MAX_PINS = 3;

    /*!
     * Description of RF switch pin states for a single mode.
     *
     * See setRfSwitchTable() for details.
     */
    struct RfSwitchMode_t {
      uint8_t mode;
      uint32_t values[RFSWITCH_MAX_PINS];
    };

    /*!
     * Constants to use in a mode table set be setRfSwitchTable. These
     * constants work for most radios, but some radios define their own
     * constants to be used instead.
     *
     * See setRfSwitchTable() for details.
     */
    enum OpMode_t {
      /*! End of table marker, use \ref END_OF_MODE_TABLE constant
       * instead. Value is zero to ensure zero-initialized mode ends the
       * table */
      MODE_END_OF_TABLE = 0,
      /*! Idle mode */
      MODE_IDLE,
      /*! Receive mode */
      MODE_RX,
      /*! Transmission mode */
      MODE_TX,
    };

    #if defined(RADIOLIB_BUILD_ARDUINO)
    /*!
      \brief Arduino Module constructor. Will use the default SPI interface and automatically initialize it.
      \param cs Arduino pin to be used as chip select.
      \param irq Arduino pin to be used as interrupt/GPIO.
      \param rst Arduino pin to be used as hardware reset for the module.
      \param gpio Arduino pin to be used as additional interrupt/GPIO.
    */
    Module(uint32_t cs, uint32_t irq, uint32_t rst, uint32_t gpio = RADIOLIB_NC);

    /*!
      \brief Arduino Module constructor. Will not attempt SPI interface initialization.
      \param cs Arduino pin to be used as chip select.
      \param irq Arduino pin to be used as interrupt/GPIO.
      \param rst Arduino pin to be used as hardware reset for the module.
      \param gpio Arduino pin to be used as additional interrupt/GPIO.
      \param spi SPI interface to be used, can also use software SPI implementations.
      \param spiSettings SPI interface settings.
    */
    Module(uint32_t cs, uint32_t irq, uint32_t rst, uint32_t gpio, SPIClass& spi, SPISettings spiSettings = RADIOLIB_DEFAULT_SPI_SETTINGS);
    #endif

    /*!
      \brief Module constructor.
      \param hal A Hardware abstraction layer instance. An ArduinoHal instance for example.
      \param cs Pin to be used as chip select.
      \param irq Pin to be used as interrupt/GPIO.
      \param rst Pin to be used as hardware reset for the module.
      \param gpio Pin to be used as additional interrupt/GPIO.
    */
    Module(RadioLibHal *hal, uint32_t cs, uint32_t irq, uint32_t rst, uint32_t gpio = RADIOLIB_NC);

    /*!
      \brief Copy constructor.
      \param mod Module instance to copy.
    */
    Module(const Module& mod);

    /*!
      \brief Overload for assignment operator.
      \param frame rvalue Module.
    */
    Module& operator=(const Module& mod);

    // public member variables
    /*!
      \brief Hardware abstraction layer to be used.
    */
    RadioLibHal* hal = NULL;

    /*!
      \brief Basic SPI read command. Defaults to 0x00.
    */
    uint8_t SPIreadCommand = 0b00000000;

    /*!
      \brief Basic SPI write command. Defaults to 0x80.
    */
    uint8_t SPIwriteCommand = 0b10000000;

    /*!
      \brief Basic SPI no-operation command. Defaults to 0x00.
    */
    uint8_t SPInopCommand = 0x00;

    /*!
      \brief Basic SPI status read command. Defaults to 0x00.
    */
    uint8_t SPIstatusCommand = 0x00;

    /*!
      \brief SPI address width. Defaults to 8, currently only supports 8 and 16-bit addresses.
    */
    uint8_t SPIaddrWidth = 8;

    /*!
      \brief Whether the SPI interface is stream-type (e.g. SX126x) or register-type (e.g. SX127x).
      Defaults to register-type SPI interfaces.
    */
    bool SPIstreamType = false;

    /*!
      \brief The last recorded SPI stream error.
    */
    int16_t SPIstreamError = RADIOLIB_ERR_UNKNOWN;

    /*!
      \brief SPI status parsing callback typedef.
    */
    typedef int16_t (*SPIparseStatusCb_t)(uint8_t in);

    /*!
      \brief Callback to function that will parse the module-specific status codes to RadioLib status codes.
      Typically used for modules with SPI stream-type interface (e.g. SX126x/SX128x).
    */
    SPIparseStatusCb_t SPIparseStatusCb = nullptr;

    #if RADIOLIB_INTERRUPT_TIMING

    /*!
      \brief Timer interrupt setup callback typedef.
    */
    typedef void (*TimerSetupCb_t)(uint32_t len);

    /*!
      \brief Callback to timer interrupt setup function when running in interrupt timing control mode.
    */
    TimerSetupCb_t TimerSetupCb = nullptr;

    /*!
      \brief Timer flag variable to be controlled by a platform-dependent interrupt.
    */
    volatile bool TimerFlag = false;

    #endif

    // basic methods

    /*!
      \brief Initialize low-level module control.
    */
    void init();

    /*!
      \brief Terminate low-level module control.
    */
    void term();

    // SPI methods

    /*!
      \brief SPI read method that automatically masks unused bits. This method is the preferred SPI read mechanism.
      \param reg Address of SPI register to read.
      \param msb Most significant bit of the register variable. Bits above this one will be masked out.
      \param lsb Least significant bit of the register variable. Bits below this one will be masked out.
      \returns Masked register value or status code.
    */
    int16_t SPIgetRegValue(uint16_t reg, uint8_t msb = 7, uint8_t lsb = 0);

    /*!
      \brief Overwrite-safe SPI write method with verification. This method is the preferred SPI write mechanism.
      \param reg Address of SPI register to write.
      \param value Single byte value that will be written to the SPI register.
      \param msb Most significant bit of the register variable. Bits above this one will not be affected by the write operation.
      \param lsb Least significant bit of the register variable. Bits below this one will not be affected by the write operation.
      \param checkInterval Number of milliseconds between register writing and verification reading. Some registers need up to 10ms to process the change.
      \param checkMask Mask of bits to check, only bits set to 1 will be verified.
      \returns \ref status_codes
    */
    int16_t SPIsetRegValue(uint16_t reg, uint8_t value, uint8_t msb = 7, uint8_t lsb = 0, uint8_t checkInterval = 2, uint8_t checkMask = 0xFF);

    /*!
      \brief SPI burst read method.
      \param reg Address of SPI register to read.
      \param numBytes Number of bytes that will be read.
      \param inBytes Pointer to array that will hold the read data.
    */
    void SPIreadRegisterBurst(uint16_t reg, size_t numBytes, uint8_t* inBytes);

    /*!
      \brief SPI basic read method. Use of this method is reserved for special cases, SPIgetRegValue should be used instead.
      \param reg Address of SPI register to read.
      \returns Value that was read from register.
    */
    uint8_t SPIreadRegister(uint16_t reg);

    /*!
      \brief SPI burst write method.
      \param reg Address of SPI register to write.
      \param data Pointer to array that holds the data that will be written.
      \param numBytes Number of bytes that will be written.
    */
    void SPIwriteRegisterBurst(uint16_t reg, uint8_t* data, size_t numBytes);

    /*!
      \brief SPI basic write method. Use of this method is reserved for special cases, SPIsetRegValue should be used instead.
      \param reg Address of SPI register to write.
      \param data Value that will be written to the register.
    */
    void SPIwriteRegister(uint16_t reg, uint8_t data);

    /*!
      \brief SPI single transfer method.
      \param cmd SPI access command (read/write/burst/...).
      \param reg Address of SPI register to transfer to/from.
      \param dataOut Data that will be transferred from master to slave.
      \param dataIn Data that was transferred from slave to master.
      \param numBytes Number of bytes to transfer.
    */
    void SPItransfer(uint8_t cmd, uint16_t reg, uint8_t* dataOut, uint8_t* dataIn, size_t numBytes);

    /*!
      \brief Method to check the result of last SPI stream transfer.
      \returns \ref status_codes
    */
    int16_t SPIcheckStream();
    
    /*!
      \brief Method to perform a read transaction with SPI stream.
      \param cmd SPI operation command.
      \param data Data that will be transferred from slave to master.
      \param numBytes Number of bytes to transfer.
      \param waitForGpio Whether to wait for some GPIO at the end of transfer (e.g. BUSY line on SX126x/SX128x).
      \param verify Whether to verify the result of the transaction after it is finished.
      \returns \ref status_codes
    */
    int16_t SPIreadStream(uint8_t cmd, uint8_t* data, size_t numBytes, bool waitForGpio = true, bool verify = true);
    
    /*!
      \brief Method to perform a read transaction with SPI stream.
      \param cmd SPI operation command.
      \param cmdLen SPI command length in bytes.
      \param data Data that will be transferred from slave to master.
      \param numBytes Number of bytes to transfer.
      \param waitForGpio Whether to wait for some GPIO at the end of transfer (e.g. BUSY line on SX126x/SX128x).
      \param verify Whether to verify the result of the transaction after it is finished.
      \returns \ref status_codes
    */
    int16_t SPIreadStream(uint8_t* cmd, uint8_t cmdLen, uint8_t* data, size_t numBytes, bool waitForGpio = true, bool verify = true);
    
    /*!
      \brief Method to perform a write transaction with SPI stream.
      \param cmd SPI operation command.
      \param data Data that will be transferred from master to slave.
      \param numBytes Number of bytes to transfer.
      \param waitForGpio Whether to wait for some GPIO at the end of transfer (e.g. BUSY line on SX126x/SX128x).
      \param verify Whether to verify the result of the transaction after it is finished.
      \returns \ref status_codes
    */
    int16_t SPIwriteStream(uint8_t cmd, uint8_t* data, size_t numBytes, bool waitForGpio = true, bool verify = true);

    /*!
      \brief Method to perform a write transaction with SPI stream.
      \param cmd SPI operation command.
      \param cmdLen SPI command length in bytes.
      \param data Data that will be transferred from master to slave.
      \param numBytes Number of bytes to transfer.
      \param waitForGpio Whether to wait for some GPIO at the end of transfer (e.g. BUSY line on SX126x/SX128x).
      \param verify Whether to verify the result of the transaction after it is finished.
      \returns \ref status_codes
    */
    int16_t SPIwriteStream(uint8_t* cmd, uint8_t cmdLen, uint8_t* data, size_t numBytes, bool waitForGpio = true, bool verify = true);
    
    /*!
      \brief SPI single transfer method for modules with stream-type SPI interface (SX126x, SX128x etc.).
      \param cmd SPI operation command.
      \param cmdLen SPI command length in bytes.
      \param write Set to true for write commands, false for read commands.
      \param dataOut Data that will be transferred from master to slave.
      \param dataIn Data that was transferred from slave to master.
      \param numBytes Number of bytes to transfer.
      \param waitForGpio Whether to wait for some GPIO at the end of transfer (e.g. BUSY line on SX126x/SX128x).
      \param timeout GPIO wait period timeout in milliseconds.
      \returns \ref status_codes
    */
    int16_t SPItransferStream(uint8_t* cmd, uint8_t cmdLen, bool write, uint8_t* dataOut, uint8_t* dataIn, size_t numBytes, bool waitForGpio, uint32_t timeout);

    // pin number access methods

    /*!
      \brief Access method to get the pin number of SPI chip select.
      \returns Pin number of SPI chip select configured in the constructor.
    */
    uint32_t getCs() const { return(csPin); }

    /*!
      \brief Access method to get the pin number of interrupt/GPIO.
      \returns Pin number of interrupt/GPIO configured in the constructor.
    */
    uint32_t getIrq() const { return(irqPin); }

    /*!
      \brief Access method to get the pin number of hardware reset pin.
      \returns Pin number of hardware reset pin configured in the constructor.
    */
    uint32_t getRst() const { return(rstPin); }

    /*!
      \brief Access method to get the pin number of second interrupt/GPIO.
      \returns Pin number of second interrupt/GPIO configured in the constructor.
    */
    uint32_t getGpio() const { return(gpioPin); }

    /*!
      \brief Some modules contain external RF switch controlled by pins.
      This function gives RadioLib control over those pins to
      automatically switch between various modes: When idle both pins
      will be LOW, during TX the `txEn` pin will be HIGH, during RX the
      `rxPin` will be HIGH.

      Radiolib will automatically set the pin mode and value of these
      pins, so do not control them from the sketch.

      When more than two pins or more control over the output values are
      needed, use the setRfSwitchTable() function.

      \param rxEn RX enable pin.
      \param txEn TX enable pin.
    */
    void setRfSwitchPins(uint32_t rxEn, uint32_t txEn);

    /*!
      \brief Some modules contain external RF switch controlled by pins.
      This function gives RadioLib control over those pins to
      automatically switch between various modes.

      Radiolib will automatically set the pin mode and value of these
      pins, so do not control them from the sketch.


      \param pins A reference to an array of pins to control. This
      should always be an array of 3 elements. If you need less pins,
      use RADIOLIB_NC for the unused elements.

      \param table A reference to an array of pin values to use for each
      supported mode. Each element is an RfSwitchMode_T struct that
      lists the mode for which it applies and the values for each of the
      pins passed in the pins argument respectively.

      The `pins` array will be copied into the Module object, so the
      original array can be deallocated after this call. However,
      a reference to the `table` array will be stored, so that array
      must remain valid as long RadioLib is being used.

      The `mode` field in each table row should normally use any of the
      `MODE_*` constants from the Module::OpMode_t enum. However, some
      radios support additional modes and will define their own OpMode_t
      enum.

      The length of the table is variable (to support radios that add
      additional modes), so the table must always be terminated with the
      special END_OF_MODE_TABLE value.

      Normally all modes should be listed in the table, but for some
      radios, modes can be omitted to indicate they are not supported
      (e.g. when a radio has a high power and low power TX mode but
      external circuitry only supports low power). If applicable, this
      is documented in the radio class itself.

      #### Example
      For example, on a board that has an RF switch with an enable pin
      connected to PA0 and a TX/RX select pin connected to PA1:

      \code
      // In global scope, define the pin array and mode table
      static const uint32_t rfswitch_pins[] =
                             {PA0,  PA1,  RADIOLIB_NC};
      static const Module::RfSwitchMode_t rfswitch_table[] = {
        {Module::MODE_IDLE,  {LOW,  LOW}},
        {Module::MODE_RX,    {HIGH, LOW}},
        {Module::MODE_TX,    {HIGH, HIGH}},
         Module::END_OF_MODE_TABLE,
      };

      void setup() {
        ...
        // Then somewhere in setup, pass them to radiolib
        radio.setRfSwitchTable(rfswitch_pins, rfswitch_table);
        ...
      }
      \endcode
    */

    void setRfSwitchTable(const uint32_t (&pins)[RFSWITCH_MAX_PINS], const RfSwitchMode_t table[]);

    /*!
      \brief Find a mode in the RfSwitchTable.
      \param The mode to find.
      \returns A pointer to the RfSwitchMode_t struct in the table that
      matches the passed mode. Returns nullptr if no rfswitch pins are
      configured, or the passed mode is not listed in the table.
    */
    const RfSwitchMode_t *findRfSwitchMode(uint8_t mode) const;

    /*!
      \brief Set RF switch state.
      \param mode The mode to set. This must be one of the MODE_ constants, or a radio-specific constant.
    */
    void setRfSwitchState(uint8_t mode);

    /*!
      \brief Wait for time to elapse, either using the microsecond timer, or the TimerFlag.
      Note that in interrupt timing mode, it is up to the user to set up the timing interrupt!

      \param start Waiting start timestamp, in microseconds.
      \param len Waiting duration, in microseconds;
    */
    void waitForMicroseconds(uint32_t start, uint32_t len);

    /*!
      \brief Function to reflect bits within a byte.
      \param in The input to reflect.
      \param bits Number of bits to reflect.
      \return The reflected input.
    */
    static uint32_t reflect(uint32_t in, uint8_t bits);

    #if RADIOLIB_DEBUG
    /*!
      \brief Function to dump data as hex into the debug port.
      \param level RadioLib debug level, set to NULL to not print.
      \param data Data to dump.
      \param len Number of bytes to dump.
      \param width Word width (1 for uint8_t, 2 for uint16_t, 4 for uint32_t).
      \param be Print multi-byte data as big endian. Defaults to false.
    */
    static void hexdump(const char* level, uint8_t* data, size_t len, uint32_t offset = 0, uint8_t width = 1, bool be = false);

    /*!
      \brief Function to dump device registers as hex into the debug port.
      \param level RadioLib debug level, set to NULL to not print.
      \param start First address to dump.
      \param len Number of bytes to dump.
    */
    void regdump(const char* level, uint16_t start, size_t len);
    #endif

    #if RADIOLIB_DEBUG and defined(RADIOLIB_BUILD_ARDUINO)
    static size_t serialPrintf(const char* format, ...);
    #endif

#if !RADIOLIB_GODMODE
  private:
#endif
    uint32_t csPin = RADIOLIB_NC;
    uint32_t irqPin = RADIOLIB_NC;
    uint32_t rstPin = RADIOLIB_NC;
    uint32_t gpioPin = RADIOLIB_NC;

    // RF switch pins and table
    uint32_t rfSwitchPins[RFSWITCH_MAX_PINS] = { RADIOLIB_NC, RADIOLIB_NC, RADIOLIB_NC };
    const RfSwitchMode_t *rfSwitchTable = nullptr;

    #if RADIOLIB_INTERRUPT_TIMING
    uint32_t prevTimingLen = 0;
    #endif
};

#endif
