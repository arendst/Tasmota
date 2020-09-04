/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * @file RF24.h
 *
 * Class declaration for RF24 and helper enums
 */

#ifndef __RF24_H__
#define __RF24_H__

#include "RF24_config.h"

#if defined (RF24_LINUX) || defined (LITTLEWIRE)
  #include "utility/includes.h"
#elif defined SOFTSPI
  #include <DigitalIO.h>
#endif

/**
 * Power Amplifier level.
 *
 * For use with setPALevel()
 */
typedef enum { RF24_PA_MIN = 0,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX, RF24_PA_ERROR } rf24_pa_dbm_e ;

/**
 * Data rate.  How fast data moves through the air.
 *
 * For use with setDataRate()
 */
typedef enum { RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS } rf24_datarate_e;

/**
 * CRC Length.  How big (if any) of a CRC is included.
 *
 * For use with setCRCLength()
 */
typedef enum { RF24_CRC_DISABLED = 0, RF24_CRC_8, RF24_CRC_16 } rf24_crclength_e;

/**
 * Driver for nRF24L01(+) 2.4GHz Wireless Transceiver
 */

class RF24
{
private:
#ifdef SOFTSPI
  SoftSPI<SOFT_SPI_MISO_PIN, SOFT_SPI_MOSI_PIN, SOFT_SPI_SCK_PIN, SPI_MODE> spi;
#elif defined (SPI_UART)
  SPIUARTClass uspi;
#endif

#if defined (RF24_LINUX) || defined (XMEGA_D3) /* XMEGA can use SPI class */
  SPI spi;
#endif
#if defined (MRAA)
  GPIO gpio;
#endif

  uint16_t ce_pin; /**< "Chip Enable" pin, activates the RX or TX role */
  uint16_t csn_pin; /**< SPI Chip select */
  uint16_t spi_speed; /**< SPI Bus Speed */
#if defined (RF24_LINUX) || defined (XMEGA_D3)
  uint8_t spi_rxbuff[32+1] ; //SPI receive buffer (payload max 32 bytes)
  uint8_t spi_txbuff[32+1] ; //SPI transmit buffer (payload max 32 bytes + 1 byte for the command)
#endif  
  bool p_variant; /* False for RF24L01 and true for RF24L01P */
  uint8_t payload_size; /**< Fixed size of payloads */
  bool dynamic_payloads_enabled; /**< Whether dynamic payloads are enabled. */
  uint8_t pipe0_reading_address[5]; /**< Last address set on pipe 0 for reading. */
  uint8_t addr_width; /**< The address width to use - 3,4 or 5 bytes. */
  

protected:
  /**
   * SPI transactions
   *
   * Common code for SPI transactions including CSN toggle
   *
   */
  inline void beginTransaction();

  inline void endTransaction();

public:

  /**
   * @name Primary public interface
   *
   *  These are the main methods you need to operate the chip
   */
  /**@{*/

  /**
   * Arduino Constructor
   *
   * Creates a new instance of this driver.  Before using, you create an instance
   * and send in the unique pins that this chip is connected to.
   *
   * @param _cepin The pin attached to Chip Enable on the RF module
   * @param _cspin The pin attached to Chip Select
   */
  RF24(void);
  //#if defined (RF24_LINUX)
  
    /**
  * Optional Linux Constructor
  *
  * Creates a new instance of this driver.  Before using, you create an instance
  * and send in the unique pins that this chip is connected to.
  *
  * @param _cepin The pin attached to Chip Enable on the RF module
  * @param _cspin The pin attached to Chip Select
  * @param spispeed For RPi, the SPI speed in MHZ ie: BCM2835_SPI_SPEED_8MHZ
  */
  
  RF24(uint16_t _cepin, uint16_t _cspin, uint32_t spispeed );
  //#endif

  #if defined (RF24_LINUX)
  virtual ~RF24() {};
  #endif

  /**
   * Begin operation of the chip
   * 
   * Call this in setup(), before calling any other methods.
   * @code radio.begin() @endcode
   */
  bool begin(uint16_t _cepin, uint16_t _cspin);

  /**
   * Checks if the chip is connected to the SPI bus
   */
  bool isChipConnected();

  /**
   * Start listening on the pipes opened for reading.
   *
   * 1. Be sure to call openReadingPipe() first.  
   * 2. Do not call write() while in this mode, without first calling stopListening().
   * 3. Call available() to check for incoming traffic, and read() to get it. 
   *  
   * @code
   * Open reading pipe 1 using address CCCECCCECC
   *  
   * byte address[] = { 0xCC,0xCE,0xCC,0xCE,0xCC };
   * radio.openReadingPipe(1,address);
   * radio.startListening();
   * @endcode
   */
  void startListening(void);

  /**
   * Stop listening for incoming messages, and switch to transmit mode.
   *
   * Do this before calling write().
   * @code
   * radio.stopListening();
   * radio.write(&data,sizeof(data));
   * @endcode
   */
  void stopListening(void);

  /**
   * Check whether there are bytes available to be read
   * @code
   * if(radio.available()){
   *   radio.read(&data,sizeof(data));
   * }
   * @endcode
   * @return True if there is a payload available, false if none is
   */
  bool available(void);

  /**
   * Read the available payload
   *
   * The size of data read is the fixed payload size, see getPayloadSize()
   *
   * @note I specifically chose 'void*' as a data type to make it easier
   * for beginners to use.  No casting needed.
   *
   * @note No longer boolean. Use available to determine if packets are
   * available. Interrupt flags are now cleared during reads instead of
   * when calling available().
   *
   * @param buf Pointer to a buffer where the data should be written
   * @param len Maximum number of bytes to read into the buffer
   *
   * @code
   * if(radio.available()){
   *   radio.read(&data,sizeof(data));
   * }
   * @endcode
   * @return No return value. Use available().
   */
  void read( void* buf, uint8_t len );

  /**
   * Be sure to call openWritingPipe() first to set the destination
   * of where to write to.
   *
   * This blocks until the message is successfully acknowledged by
   * the receiver or the timeout/retransmit maxima are reached.  In
   * the current configuration, the max delay here is 60-70ms.
   *
   * The maximum size of data written is the fixed payload size, see
   * getPayloadSize().  However, you can write less, and the remainder
   * will just be filled with zeroes.
   *
   * TX/RX/RT interrupt flags will be cleared every time write is called
   *
   * @param buf Pointer to the data to be sent
   * @param len Number of bytes to be sent
   *
   * @code
   * radio.stopListening();
   * radio.write(&data,sizeof(data));
   * @endcode
   * @return True if the payload was delivered successfully and an ACK was received, or upon successfull transmission if auto-ack is disabled.
   */
  bool write( const void* buf, uint8_t len );

  /**
   * New: Open a pipe for writing via byte array. Old addressing format retained
   * for compatibility.
   *
   * Only one writing pipe can be open at once, but you can change the address
   * you'll write to. Call stopListening() first.
   *
   * Addresses are assigned via a byte array, default is 5 byte address length
s   *
   * @code
   *   uint8_t addresses[][6] = {"1Node","2Node"};
   *   radio.openWritingPipe(addresses[0]);
   * @endcode
   * @code
   *  uint8_t address[] = { 0xCC,0xCE,0xCC,0xCE,0xCC };
   *  radio.openWritingPipe(address);
   *  address[0] = 0x33;
   *  radio.openReadingPipe(1,address);
   * @endcode
   * @see setAddressWidth
   *
   * @param address The address of the pipe to open. Coordinate these pipe
   * addresses amongst nodes on the network.
   */

  void openWritingPipe(const uint8_t *address);

  /**
   * Open a pipe for reading
   *
   * Up to 6 pipes can be open for reading at once.  Open all the required
   * reading pipes, and then call startListening().
   *
   * @see openWritingPipe
   * @see setAddressWidth
   *
   * @note Pipes 0 and 1 will store a full 5-byte address. Pipes 2-5 will technically 
   * only store a single byte, borrowing up to 4 additional bytes from pipe #1 per the
   * assigned address width.
   * @warning Pipes 1-5 should share the same address, except the first byte.
   * Only the first byte in the array should be unique, e.g.
   * @code
   *   uint8_t addresses[][6] = {"1Node","2Node"};
   *   openReadingPipe(1,addresses[0]);
   *   openReadingPipe(2,addresses[1]);
   * @endcode
   *
   * @warning Pipe 0 is also used by the writing pipe.  So if you open
   * pipe 0 for reading, and then startListening(), it will overwrite the
   * writing pipe.  Ergo, do an openWritingPipe() again before write().
   *
   * @param number Which pipe# to open, 0-5.
   * @param address The 24, 32 or 40 bit address of the pipe to open.
   */

  void openReadingPipe(uint8_t number, const uint8_t *address);

   /**@}*/
  /**
   * @name Advanced Operation
   *
   *  Methods you can use to drive the chip in more advanced ways
   */
  /**@{*/

  /**
   * Print a giant block of debugging information to stdout
   *
   * @warning Does nothing if stdout is not defined.  See fdevopen in stdio.h
   * The printf.h file is included with the library for Arduino.
   * @code
   * #include <printf.h>
   * setup(){
   *  Serial.begin(115200);
   *  printf_begin();
   *  ...
   * }
   * @endcode
   */
  void printDetails(void);

  /**
   * Test whether there are bytes available to be read in the
   * FIFO buffers. 
   *
   * @param[out] pipe_num Which pipe has the payload available
   *  
   * @code
   * uint8_t pipeNum;
   * if(radio.available(&pipeNum)){
   *   radio.read(&data,sizeof(data));
   *   Serial.print("Got data on pipe");
   *   Serial.println(pipeNum);
   * }
   * @endcode
   * @return True if there is a payload available, false if none is
   */
  bool available(uint8_t* pipe_num);

  /**
   * Check if the radio needs to be read. Can be used to prevent data loss
   * @return True if all three 32-byte radio buffers are full
   */
  bool rxFifoFull();

  /**
   * Enter low-power mode
   *
   * To return to normal power mode, call powerUp().
   *
   * @note After calling startListening(), a basic radio will consume about 13.5mA
   * at max PA level.
   * During active transmission, the radio will consume about 11.5mA, but this will
   * be reduced to 26uA (.026mA) between sending.
   * In full powerDown mode, the radio will consume approximately 900nA (.0009mA)   
   *
   * @code
   * radio.powerDown();
   * avr_enter_sleep_mode(); // Custom function to sleep the device
   * radio.powerUp();
   * @endcode
   */
  void powerDown(void);

  /**
   * Leave low-power mode - required for normal radio operation after calling powerDown()
   * 
   * To return to low power mode, call powerDown().
   * @note This will take up to 5ms for maximum compatibility 
   */
  void powerUp(void) ;

  /**
  * Write for single NOACK writes. Optionally disables acknowledgements/autoretries for a single write.
  *
  * @note enableDynamicAck() must be called to enable this feature
  *
  * Can be used with enableAckPayload() to request a response
  * @see enableDynamicAck()
  * @see setAutoAck()
  * @see write()
  *
  * @param buf Pointer to the data to be sent
  * @param len Number of bytes to be sent
  * @param multicast Request ACK (0), NOACK (1)
  */
  bool write( const void* buf, uint8_t len, const bool multicast );

  /**
   * This will not block until the 3 FIFO buffers are filled with data.
   * Once the FIFOs are full, writeFast will simply wait for success or
   * timeout, and return 1 or 0 respectively. From a user perspective, just
   * keep trying to send the same data. The library will keep auto retrying
   * the current payload using the built in functionality.
   * @warning It is important to never keep the nRF24L01 in TX mode and FIFO full for more than 4ms at a time. If the auto
   * retransmit is enabled, the nRF24L01 is never in TX mode long enough to disobey this rule. Allow the FIFO
   * to clear by issuing txStandBy() or ensure appropriate time between transmissions.
   *
   * @code
   * Example (Partial blocking):
   *
   *			radio.writeFast(&buf,32);  // Writes 1 payload to the buffers
   *			txStandBy();     		   // Returns 0 if failed. 1 if success. Blocks only until MAX_RT timeout or success. Data flushed on fail.
   *
   *			radio.writeFast(&buf,32);  // Writes 1 payload to the buffers
   *			txStandBy(1000);		   // Using extended timeouts, returns 1 if success. Retries failed payloads for 1 seconds before returning 0.
   * @endcode
   *
   * @see txStandBy()
   * @see write()
   * @see writeBlocking()
   *
   * @param buf Pointer to the data to be sent
   * @param len Number of bytes to be sent
   * @return True if the payload was delivered successfully false if not
   */
  bool writeFast( const void* buf, uint8_t len );

  /**
  * WriteFast for single NOACK writes. Disables acknowledgements/autoretries for a single write.
  *
  * @note enableDynamicAck() must be called to enable this feature
  * @see enableDynamicAck()
  * @see setAutoAck()
  *
  * @param buf Pointer to the data to be sent
  * @param len Number of bytes to be sent
  * @param multicast Request ACK (0) or NOACK (1)
  */
  bool writeFast( const void* buf, uint8_t len, const bool multicast );

  /**
   * This function extends the auto-retry mechanism to any specified duration.
   * It will not block until the 3 FIFO buffers are filled with data.
   * If so the library will auto retry until a new payload is written
   * or the user specified timeout period is reached.
   * @warning It is important to never keep the nRF24L01 in TX mode and FIFO full for more than 4ms at a time. If the auto
   * retransmit is enabled, the nRF24L01 is never in TX mode long enough to disobey this rule. Allow the FIFO
   * to clear by issuing txStandBy() or ensure appropriate time between transmissions.
   *
   * @code
   * Example (Full blocking):
   *
   *			radio.writeBlocking(&buf,32,1000); //Wait up to 1 second to write 1 payload to the buffers
   *			txStandBy(1000);     			   //Wait up to 1 second for the payload to send. Return 1 if ok, 0 if failed.
   *					  				   		   //Blocks only until user timeout or success. Data flushed on fail.
   * @endcode
   * @note If used from within an interrupt, the interrupt should be disabled until completion, and sei(); called to enable millis().
   * @see txStandBy()
   * @see write()
   * @see writeFast()
   *
   * @param buf Pointer to the data to be sent
   * @param len Number of bytes to be sent
   * @param timeout User defined timeout in milliseconds.
   * @return True if the payload was loaded into the buffer successfully false if not
   */
  bool writeBlocking( const void* buf, uint8_t len, uint32_t timeout );

  /**
   * This function should be called as soon as transmission is finished to
   * drop the radio back to STANDBY-I mode. If not issued, the radio will
   * remain in STANDBY-II mode which, per the data sheet, is not a recommended
   * operating mode.
   *
   * @note When transmitting data in rapid succession, it is still recommended by
   * the manufacturer to drop the radio out of TX or STANDBY-II mode if there is
   * time enough between sends for the FIFOs to empty. This is not required if auto-ack
   * is enabled.
   *
   * Relies on built-in auto retry functionality.
   *
   * @code
   * Example (Partial blocking):
   *
   *			radio.writeFast(&buf,32);
   *			radio.writeFast(&buf,32);
   *			radio.writeFast(&buf,32);  //Fills the FIFO buffers up
   *			bool ok = txStandBy();     //Returns 0 if failed. 1 if success.
   *					  				   //Blocks only until MAX_RT timeout or success. Data flushed on fail.
   * @endcode
   * @see txStandBy(unsigned long timeout)
   * @return True if transmission is successful
   *
   */
   bool txStandBy();

  /**
   * This function allows extended blocking and auto-retries per a user defined timeout
   * @code
   *	Fully Blocking Example:
   *
   *			radio.writeFast(&buf,32);
   *			radio.writeFast(&buf,32);
   *			radio.writeFast(&buf,32);   //Fills the FIFO buffers up
   *			bool ok = txStandBy(1000);  //Returns 0 if failed after 1 second of retries. 1 if success.
   *					  				    //Blocks only until user defined timeout or success. Data flushed on fail.
   * @endcode
   * @note If used from within an interrupt, the interrupt should be disabled until completion, and sei(); called to enable millis().
   * @param timeout Number of milliseconds to retry failed payloads
   * @return True if transmission is successful
   *
   */
   bool txStandBy(uint32_t timeout, bool startTx = 0);

  /**
   * Write an ack payload for the specified pipe
   *
   * The next time a message is received on @p pipe, the data in @p buf will
   * be sent back in the acknowledgement.
   * @see enableAckPayload()
   * @see enableDynamicPayloads()
   * @warning Only three of these can be pending at any time as there are only 3 FIFO buffers.<br> Dynamic payloads must be enabled.
   * @note Ack payloads are handled automatically by the radio chip when a payload is received. Users should generally
   * write an ack payload as soon as startListening() is called, so one is available when a regular payload is received.
   * @note Ack payloads are dynamic payloads. This only works on pipes 0&1 by default. Call 
   * enableDynamicPayloads() to enable on all pipes.
   *
   * @param pipe Which pipe# (typically 1-5) will get this response.
   * @param buf Pointer to data that is sent
   * @param len Length of the data to send, up to 32 bytes max.  Not affected
   * by the static payload set by setPayloadSize().
   */
  void writeAckPayload(uint8_t pipe, const void* buf, uint8_t len);

  /**
   * Determine if an ack payload was received in the most recent call to
   * write(). The regular available() can also be used.
   *
   * Call read() to retrieve the ack payload.
   *
   * @return True if an ack payload is available.
   */
  bool isAckPayloadAvailable(void);

  /**
   * Call this when you get an interrupt to find out why
   *
   * Tells you what caused the interrupt, and clears the state of
   * interrupts.
   *
   * @param[out] tx_ok The send was successful (TX_DS)
   * @param[out] tx_fail The send failed, too many retries (MAX_RT)
   * @param[out] rx_ready There is a message waiting to be read (RX_DS)
   */
  void whatHappened(bool& tx_ok,bool& tx_fail,bool& rx_ready);

  /**
   * Non-blocking write to the open writing pipe used for buffered writes
   *
   * @note Optimization: This function now leaves the CE pin high, so the radio
   * will remain in TX or STANDBY-II Mode until a txStandBy() command is issued. Can be used as an alternative to startWrite()
   * if writing multiple payloads at once.
   * @warning It is important to never keep the nRF24L01 in TX mode with FIFO full for more than 4ms at a time. If the auto
   * retransmit/autoAck is enabled, the nRF24L01 is never in TX mode long enough to disobey this rule. Allow the FIFO
   * to clear by issuing txStandBy() or ensure appropriate time between transmissions.
   *
   * @see write()
   * @see writeFast()
   * @see startWrite()
   * @see writeBlocking()
   *
   * For single noAck writes see:
   * @see enableDynamicAck()
   * @see setAutoAck()
   *
   * @param buf Pointer to the data to be sent
   * @param len Number of bytes to be sent
   * @param multicast Request ACK (0) or NOACK (1)
   * @return True if the payload was delivered successfully false if not
   */
  void startFastWrite( const void* buf, uint8_t len, const bool multicast, bool startTx = 1 );

  /**
   * Non-blocking write to the open writing pipe
   *
   * Just like write(), but it returns immediately. To find out what happened
   * to the send, catch the IRQ and then call whatHappened().
   *
   * @see write()
   * @see writeFast()
   * @see startFastWrite()
   * @see whatHappened()
   *
   * For single noAck writes see:
   * @see enableDynamicAck()
   * @see setAutoAck()
   *
   * @param buf Pointer to the data to be sent
   * @param len Number of bytes to be sent
   * @param multicast Request ACK (0) or NOACK (1)
   *
   */
  void startWrite( const void* buf, uint8_t len, const bool multicast );
  
  /**
   * This function is mainly used internally to take advantage of the auto payload
   * re-use functionality of the chip, but can be beneficial to users as well.
   *
   * The function will instruct the radio to re-use the data in the FIFO buffers,
   * and instructs the radio to re-send once the timeout limit has been reached.
   * Used by writeFast and writeBlocking to initiate retries when a TX failure
   * occurs. Retries are automatically initiated except with the standard write().
   * This way, data is not flushed from the buffer until switching between modes.
   *
   * @note This is to be used AFTER auto-retry fails if wanting to resend
   * using the built-in payload reuse features.
   * After issuing reUseTX(), it will keep reending the same payload forever or until
   * a payload is written to the FIFO, or a flush_tx command is given.
   */
   void reUseTX();

  /**
   * Empty the transmit buffer. This is generally not required in standard operation.
   * May be required in specific cases after stopListening() , if operating at 250KBPS data rate.
   *
   * @return Current value of status register
   */
  uint8_t flush_tx(void);

  /**
   * Test whether there was a carrier on the line for the
   * previous listening period.
   *
   * Useful to check for interference on the current channel.
   *
   * @return true if was carrier, false if not
   */
  bool testCarrier(void);

  /**
   * Test whether a signal (carrier or otherwise) greater than
   * or equal to -64dBm is present on the channel. Valid only
   * on nRF24L01P (+) hardware. On nRF24L01, use testCarrier().
   *
   * Useful to check for interference on the current channel and
   * channel hopping strategies.
   *
   * @code
   * bool goodSignal = radio.testRPD();
   * if(radio.available()){
   *    Serial.println(goodSignal ? "Strong signal > 64dBm" : "Weak signal < 64dBm" );
   *    radio.read(0,0);
   * }
   * @endcode
   * @return true if signal => -64dBm, false if not
   */
  bool testRPD(void) ;

  /**
   * Test whether this is a real radio, or a mock shim for
   * debugging.  Setting either pin to 0xff is the way to
   * indicate that this is not a real radio.
   *
   * @return true if this is a legitimate radio
   */
  bool isValid() { return ce_pin != 0xff && csn_pin != 0xff; }
  
   /**
   * Close a pipe after it has been previously opened.
   * Can be safely called without having previously opened a pipe.
   * @param pipe Which pipe # to close, 0-5.
   */
  void closeReadingPipe( uint8_t pipe ) ;

   /**
   * 
   * If a failure has been detected, it usually indicates a hardware issue. By default the library
   * will cease operation when a failure is detected.  
   * This should allow advanced users to detect and resolve intermittent hardware issues.  
   *   
   * In most cases, the radio must be re-enabled via radio.begin(); and the appropriate settings
   * applied after a failure occurs, if wanting to re-enable the device immediately.
   *
   * The three main failure modes of the radio include:
   *
   * Writing to radio: Radio unresponsive - Fixed internally by adding a timeout to the internal write functions in RF24 (failure handling)
   *
   * Reading from radio: Available returns true always - Fixed by adding a timeout to available functions by the user. This is implemented internally in  RF24Network.
   *
   * Radio configuration settings are lost - Fixed by monitoring a value that is different from the default, and re-configuring the radio if this setting reverts to the default.
   * 
   * See the included example, GettingStarted_HandlingFailures
   *    
   *  @code
   *  if(radio.failureDetected){ 
   *    radio.begin();                       // Attempt to re-configure the radio with defaults
   *    radio.failureDetected = 0;           // Reset the detection value
   *	radio.openWritingPipe(addresses[1]); // Re-configure pipe addresses
   *    radio.openReadingPipe(1,addresses[0]);
   *    report_failure();                    // Blink leds, send a message, etc. to indicate failure
   *  }
   * @endcode
  */
  //#if defined (FAILURE_HANDLING)
    bool failureDetected; 
  //#endif
    
  /**@}*/

  /**@}*/
  /**
   * @name Optional Configurators
   *
   *  Methods you can use to get or set the configuration of the chip.
   *  None are required.  Calling begin() sets up a reasonable set of
   *  defaults.
   */
  /**@{*/

  /**
  * Set the address width from 3 to 5 bytes (24, 32 or 40 bit)
  *
  * @param a_width The address width to use: 3,4 or 5
  */

  void setAddressWidth(uint8_t a_width);
  
  /**
   * Set the number and delay of retries upon failed submit
   *
   * @param delay How long to wait between each retry, in multiples of 250us,
   * max is 15.  0 means 250us, 15 means 4000us.
   * @param count How many retries before giving up, max 15
   */
  void setRetries(uint8_t delay, uint8_t count);

  /**
   * Set RF communication channel
   *
   * @param channel Which RF channel to communicate on, 0-125
   */
  void setChannel(uint8_t channel);
  
    /**
   * Get RF communication channel
   *
   * @return The currently configured RF Channel
   */
  uint8_t getChannel(void);

  /**
   * Set Static Payload Size
   *
   * This implementation uses a pre-stablished fixed payload size for all
   * transmissions.  If this method is never called, the driver will always
   * transmit the maximum payload size (32 bytes), no matter how much
   * was sent to write().
   *
   * @todo Implement variable-sized payloads feature
   *
   * @param size The number of bytes in the payload
   */
  void setPayloadSize(uint8_t size);

  /**
   * Get Static Payload Size
   *
   * @see setPayloadSize()
   *
   * @return The number of bytes in the payload
   */
  uint8_t getPayloadSize(void);

  /**
   * Get Dynamic Payload Size
   *
   * For dynamic payloads, this pulls the size of the payload off
   * the chip
   *
   * @note Corrupt packets are now detected and flushed per the
   * manufacturer.
   * @code
   * if(radio.available()){
   *   if(radio.getDynamicPayloadSize() < 1){
   *     // Corrupt payload has been flushed
   *     return; 
   *   }
   *   radio.read(&data,sizeof(data));
   * }
   * @endcode
   *
   * @return Payload length of last-received dynamic payload
   */
  uint8_t getDynamicPayloadSize(void);

  /**
   * Enable custom payloads on the acknowledge packets
   *
   * Ack payloads are a handy way to return data back to senders without
   * manually changing the radio modes on both units.
   *
   * @note Ack payloads are dynamic payloads. This only works on pipes 0&1 by default. Call 
   * enableDynamicPayloads() to enable on all pipes.
   */
  void enableAckPayload(void);

  /**
   * Enable dynamically-sized payloads
   *
   * This way you don't always have to send large packets just to send them
   * once in a while.  This enables dynamic payloads on ALL pipes.
   *
   */
  void enableDynamicPayloads(void);
  
  /**
   * Disable dynamically-sized payloads
   *
   * This disables dynamic payloads on ALL pipes. Since Ack Payloads
   * requires Dynamic Payloads, Ack Payloads are also disabled.
   * If dynamic payloads are later re-enabled and ack payloads are desired
   * then enableAckPayload() must be called again as well.
   *
   */
  void disableDynamicPayloads(void);
  
  /**
   * Enable dynamic ACKs (single write multicast or unicast) for chosen messages
   *
   * @note To enable full multicast or per-pipe multicast, use setAutoAck()
   *
   * @warning This MUST be called prior to attempting single write NOACK calls
   * @code
   * radio.enableDynamicAck();
   * radio.write(&data,32,1);  // Sends a payload with no acknowledgement requested
   * radio.write(&data,32,0);  // Sends a payload using auto-retry/autoACK
   * @endcode
   */
  void enableDynamicAck();
  
  /**
   * Determine whether the hardware is an nRF24L01+ or not.
   *
   * @return true if the hardware is nRF24L01+ (or compatible) and false
   * if its not.
   */
  bool isPVariant(void) ;

  /**
   * Enable or disable auto-acknowlede packets
   *
   * This is enabled by default, so it's only needed if you want to turn
   * it off for some reason.
   *
   * @param enable Whether to enable (true) or disable (false) auto-acks
   */
  void setAutoAck(bool enable);

  /**
   * Enable or disable auto-acknowlede packets on a per pipeline basis.
   *
   * AA is enabled by default, so it's only needed if you want to turn
   * it off/on for some reason on a per pipeline basis.
   *
   * @param pipe Which pipeline to modify
   * @param enable Whether to enable (true) or disable (false) auto-acks
   */
  void setAutoAck( uint8_t pipe, bool enable ) ;

  /**
   * Set Power Amplifier (PA) level to one of four levels:
   * RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
   *
   * The power levels correspond to the following output levels respectively:
   * NRF24L01: -18dBm, -12dBm,-6dBM, and 0dBm
   *
   * SI24R1: -6dBm, 0dBm, 3dBM, and 7dBm.
   *
   * @param level Desired PA level.
   */
  void setPALevel ( uint8_t level );

  /**
   * Fetches the current PA level.
   *
   * NRF24L01: -18dBm, -12dBm, -6dBm and 0dBm
   * SI24R1:   -6dBm, 0dBm, 3dBm, 7dBm
   *
   * @return Returns values 0 to 3 representing the PA Level.
   */
   uint8_t getPALevel( void );

  /**
   * Set the transmission data rate
   *
   * @warning setting RF24_250KBPS will fail for non-plus units
   *
   * @param speed RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
   * @return true if the change was successful
   */
  bool setDataRate(rf24_datarate_e speed);

  /**
   * Fetches the transmission data rate
   *
   * @return Returns the hardware's currently configured datarate. The value
   * is one of 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS, as defined in the
   * rf24_datarate_e enum.
   */
  rf24_datarate_e getDataRate( void ) ;

  /**
   * Set the CRC length
   * <br>CRC checking cannot be disabled if auto-ack is enabled
   * @param length RF24_CRC_8 for 8-bit or RF24_CRC_16 for 16-bit
   */
  void setCRCLength(rf24_crclength_e length);

  /**
   * Get the CRC length
   * <br>CRC checking cannot be disabled if auto-ack is enabled
   * @return RF24_CRC_DISABLED if disabled or RF24_CRC_8 for 8-bit or RF24_CRC_16 for 16-bit
   */
  rf24_crclength_e getCRCLength(void);

  /**
   * Disable CRC validation
   * 
   * @warning CRC cannot be disabled if auto-ack/ESB is enabled.
   */
  void disableCRC( void ) ;

  /**
  * The radio will generate interrupt signals when a transmission is complete,
  * a transmission fails, or a payload is received. This allows users to mask
  * those interrupts to prevent them from generating a signal on the interrupt
  * pin. Interrupts are enabled on the radio chip by default.
  *
  * @code
  * 	Mask all interrupts except the receive interrupt:
  *
  *		radio.maskIRQ(1,1,0);
  * @endcode
  *
  * @param tx_ok  Mask transmission complete interrupts
  * @param tx_fail  Mask transmit failure interrupts
  * @param rx_ready Mask payload received interrupts
  */
  void maskIRQ(bool tx_ok,bool tx_fail,bool rx_ready);
  
  /**
  * 
  * The driver will delay for this duration when stopListening() is called
  * 
  * When responding to payloads, faster devices like ARM(RPi) are much faster than Arduino:
  * 1. Arduino sends data to RPi, switches to RX mode
  * 2. The RPi receives the data, switches to TX mode and sends before the Arduino radio is in RX mode
  * 3. If AutoACK is disabled, this can be set as low as 0. If AA/ESB enabled, set to 100uS minimum on RPi
  *
  * @warning If set to 0, ensure 130uS delay after stopListening() and before any sends
  */
  
  uint32_t txDelay;

  /**
  * 
  * On all devices but Linux and ATTiny, a small delay is added to the CSN toggling function
  * 
  * This is intended to minimise the speed of SPI polling due to radio commands
  *
  * If using interrupts or timed requests, this can be set to 0 Default:5
  */
  
  uint32_t csDelay;
  
  /**@}*/
  /**
   * @name Deprecated
   *
   *  Methods provided for backwards compabibility.
   */
  /**@{*/


  /**
   * Open a pipe for reading
   * @note For compatibility with old code only, see new function
   *
   * @warning Pipes 1-5 should share the first 32 bits.
   * Only the least significant byte should be unique, e.g.
   * @code
   *   openReadingPipe(1,0xF0F0F0F0AA);
   *   openReadingPipe(2,0xF0F0F0F066);
   * @endcode
   *
   * @warning Pipe 0 is also used by the writing pipe.  So if you open
   * pipe 0 for reading, and then startListening(), it will overwrite the
   * writing pipe.  Ergo, do an openWritingPipe() again before write().
   *
   * @param number Which pipe# to open, 0-5.
   * @param address The 40-bit address of the pipe to open.
   */
  void openReadingPipe(uint8_t number, uint64_t address);

  /**
   * Open a pipe for writing
   * @note For compatibility with old code only, see new function
   *
   * Addresses are 40-bit hex values, e.g.:
   *
   * @code
   *   openWritingPipe(0xF0F0F0F0F0);
   * @endcode
   *
   * @param address The 40-bit address of the pipe to open.
   */
  void openWritingPipe(uint64_t address);

  /**
   * Empty the receive buffer
   *
   * @return Current value of status register
   */
  uint8_t flush_rx(void);

private:

  /**
   * @name Low-level internal interface.
   *
   *  Protected methods that address the chip directly.  Regular users cannot
   *  ever call these.  They are documented for completeness and for developers who
   *  may want to extend this class.
   */
  /**@{*/

  /**
   * Set chip select pin
   *
   * Running SPI bus at PI_CLOCK_DIV2 so we don't waste time transferring data
   * and best of all, we make use of the radio's FIFO buffers. A lower speed
   * means we're less likely to effectively leverage our FIFOs and pay a higher
   * AVR runtime cost as toll.
   *
   * @param mode HIGH to take this unit off the SPI bus, LOW to put it on
   */
  void csn(bool mode);

  /**
   * Set chip enable
   *
   * @param level HIGH to actively begin transmission or LOW to put in standby.  Please see data sheet
   * for a much more detailed description of this pin.
   */
  void ce(bool level);

  /**
   * Read a chunk of data in from a register
   *
   * @param reg Which register. Use constants from nRF24L01.h
   * @param buf Where to put the data
   * @param len How many bytes of data to transfer
   * @return Current value of status register
   */
  uint8_t read_register(uint8_t reg, uint8_t* buf, uint8_t len);

  /**
   * Read single byte from a register
   *
   * @param reg Which register. Use constants from nRF24L01.h
   * @return Current value of register @p reg
   */
  uint8_t read_register(uint8_t reg);

  /**
   * Write a chunk of data to a register
   *
   * @param reg Which register. Use constants from nRF24L01.h
   * @param buf Where to get the data
   * @param len How many bytes of data to transfer
   * @return Current value of status register
   */
  uint8_t write_register(uint8_t reg, const uint8_t* buf, uint8_t len);

  /**
   * Write a single byte to a register
   *
   * @param reg Which register. Use constants from nRF24L01.h
   * @param value The new value to write
   * @return Current value of status register
   */
  uint8_t write_register(uint8_t reg, uint8_t value);

  /**
   * Write the transmit payload
   *
   * The size of data written is the fixed payload size, see getPayloadSize()
   *
   * @param buf Where to get the data
   * @param len Number of bytes to be sent
   * @return Current value of status register
   */
  uint8_t write_payload(const void* buf, uint8_t len, const uint8_t writeType);

  /**
   * Read the receive payload
   *
   * The size of data read is the fixed payload size, see getPayloadSize()
   *
   * @param buf Where to put the data
   * @param len Maximum number of bytes to read
   * @return Current value of status register
   */
  uint8_t read_payload(void* buf, uint8_t len);

  /**
   * Retrieve the current status of the chip
   *
   * @return Current value of status register
   */
  uint8_t get_status(void);

  #if !defined (MINIMAL)
  /**
   * Decode and print the given status to stdout
   *
   * @param status Status value to print
   *
   * @warning Does nothing if stdout is not defined.  See fdevopen in stdio.h
   */
  void print_status(uint8_t status);

  /**
   * Decode and print the given 'observe_tx' value to stdout
   *
   * @param value The observe_tx value to print
   *
   * @warning Does nothing if stdout is not defined.  See fdevopen in stdio.h
   */
  void print_observe_tx(uint8_t value);

  /**
   * Print the name and value of an 8-bit register to stdout
   *
   * Optionally it can print some quantity of successive
   * registers on the same line.  This is useful for printing a group
   * of related registers on one line.
   *
   * @param name Name of the register
   * @param reg Which register. Use constants from nRF24L01.h
   * @param qty How many successive registers to print
   */
  void print_byte_register(const char* name, uint8_t reg, uint8_t qty = 1);

  /**
   * Print the name and value of a 40-bit address register to stdout
   *
   * Optionally it can print some quantity of successive
   * registers on the same line.  This is useful for printing a group
   * of related registers on one line.
   *
   * @param name Name of the register
   * @param reg Which register. Use constants from nRF24L01.h
   * @param qty How many successive registers to print
   */
  void print_address_register(const char* name, uint8_t reg, uint8_t qty = 1);
#endif
  /**
   * Turn on or off the special features of the chip
   *
   * The chip has certain 'features' which are only available when the 'features'
   * are enabled.  See the datasheet for details.
   */
  void toggle_features(void);

  /**
   * Built in spi transfer function to simplify repeating code repeating code
   */

  uint8_t spiTrans(uint8_t cmd);
  
  #if defined (FAILURE_HANDLING) || defined (RF24_LINUX)
	void errNotify(void);
  #endif
  
  /**@}*/

};


/**
 * @example GettingStarted.ino
 * <b>For Arduino</b><br>
 * <b>Updated: TMRh20 2014 </b><br>
 *
 * This is an example of how to use the RF24 class to communicate on a basic level. Configure and write this sketch to two
 * different nodes. Put one of the nodes into 'transmit' mode by connecting with the serial monitor and <br>
 * sending a 'T'. The ping node sends the current time to the pong node, which responds by sending the value
 * back. The ping node can then see how long the whole cycle took. <br>
 * @note For a more efficient call-response scenario see the GettingStarted_CallResponse.ino example.
 * @note When switching between sketches, the radio may need to be powered down to clear settings that are not "un-set" otherwise
 */

 /**
 * @example gettingstarted.cpp
 * <b>For Linux</b><br>
 * <b>Updated: TMRh20 2014 </b><br>
 *
 * This is an example of how to use the RF24 class to communicate on a basic level. Configure and write this sketch to two
 * different nodes. Put one of the nodes into 'transmit' mode by connecting with the serial monitor and <br>
 * sending a 'T'. The ping node sends the current time to the pong node, which responds by sending the value
 * back. The ping node can then see how long the whole cycle took. <br>
 * @note For a more efficient call-response scenario see the GettingStarted_CallResponse.ino example.
 */
 
/**
 * @example GettingStarted_CallResponse.ino
 * <b>For Arduino</b><br>
 * <b>New: TMRh20 2014</b><br>
 *
 * This example continues to make use of all the normal functionality of the radios including
 * the auto-ack and auto-retry features, but allows ack-payloads to be written optionlly as well. <br>
 * This allows very fast call-response communication, with the responding radio never having to
 * switch out of Primary Receiver mode to send back a payload, but having the option to switch to <br>
 * primary transmitter if wanting to initiate communication instead of respond to a commmunication.
 */
 
 /**
 * @example gettingstarted_call_response.cpp
 * <b>For Linux</b><br>
 * <b>New: TMRh20 2014</b><br>
 *
 * This example continues to make use of all the normal functionality of the radios including
 * the auto-ack and auto-retry features, but allows ack-payloads to be written optionlly as well. <br>
 * This allows very fast call-response communication, with the responding radio never having to
 * switch out of Primary Receiver mode to send back a payload, but having the option to switch to <br>
 * primary transmitter if wanting to initiate communication instead of respond to a commmunication.
 */

 /**
 * @example GettingStarted_HandlingData.ino
 * <b>Dec 2014 - TMRh20</b><br>
 *
 * This example demonstrates how to send multiple variables in a single payload and work with data. As usual, it is
 * generally important to include an incrementing value like millis() in the payloads to prevent errors.
 */
 
 /**
 * @example GettingStarted_HandlingFailures.ino
 *
 * This example demonstrates the basic getting started functionality, but with failure handling for the radio chip.
 * Addresses random radio failures etc, potentially due to loose wiring on breadboards etc.
 */
 
 
/**
 * @example Transfer.ino
 * <b>For Arduino</b><br>
 * This example demonstrates half-rate transfer using the FIFO buffers<br>
 *
 * It is an example of how to use the RF24 class.  Write this sketch to two
 * different nodes.  Put one of the nodes into 'transmit' mode by connecting <br>
 * with the serial monitor and sending a 'T'.  The data transfer will begin,
 * with the receiver displaying the payload count. (32Byte Payloads) <br>
 */
 
 /**
 * @example transfer.cpp
 * <b>For Linux</b><br>
 * This example demonstrates half-rate transfer using the FIFO buffers<br>
 *
 * It is an example of how to use the RF24 class.  Write this sketch to two
 * different nodes.  Put one of the nodes into 'transmit' mode by connecting <br>
 * with the serial monitor and sending a 'T'.  The data transfer will begin,
 * with the receiver displaying the payload count. (32Byte Payloads) <br>
 */

/**
 * @example TransferTimeouts.ino
 * <b>New: TMRh20 </b><br>
 * This example demonstrates the use of and extended timeout period and
 * auto-retries/auto-reUse to increase reliability in noisy or low signal scenarios. <br>
 *
 * Write this sketch to two different nodes.  Put one of the nodes into 'transmit'
 * mode by connecting with the serial monitor and sending a 'T'.  The data <br>
 * transfer will begin, with the receiver displaying the payload count and the
 * data transfer rate.
 */

/**
 * @example starping.pde
 *
 * This sketch is a more complex example of using the RF24 library for Arduino.
 * Deploy this on up to six nodes.  Set one as the 'pong receiver' by tying the
 * role_pin low, and the others will be 'ping transmit' units.  The ping units
 * unit will send out the value of millis() once a second.  The pong unit will
 * respond back with a copy of the value.  Each ping unit can get that response
 * back, and determine how long the whole cycle took.
 *
 * This example requires a bit more complexity to determine which unit is which.
 * The pong receiver is identified by having its role_pin tied to ground.
 * The ping senders are further differentiated by a byte in eeprom.
 */

/**
 * @example pingpair_ack.ino
 * <b>Update: TMRh20</b><br>
 * This example continues to make use of all the normal functionality of the radios including
 * the auto-ack and auto-retry features, but allows ack-payloads to be written optionlly as well.<br>
 * This allows very fast call-response communication, with the responding radio never having to
 * switch out of Primary Receiver mode to send back a payload, but having the option to if wanting<br>
 * to initiate communication instead of respond to a commmunication.
 */

/**
 * @example pingpair_irq.ino
 * <b>Update: TMRh20</b><br>
 * This is an example of how to user interrupts to interact with the radio, and a demonstration
 * of how to use them to sleep when receiving, and not miss any payloads.<br>
 * The pingpair_sleepy example expands on sleep functionality with a timed sleep option for the transmitter.
 * Sleep functionality is built directly into my fork of the RF24Network library<br>
 */

 /**
 * @example pingpair_irq_simple.ino
 * <b>Dec 2014 - TMRh20</b><br>
 * This is an example of how to user interrupts to interact with the radio, with bidirectional communication.
 */
 
/**
 * @example pingpair_sleepy.ino
 * <b>Update: TMRh20</b><br>
 * This is an example of how to use the RF24 class to create a battery-
 * efficient system.  It is just like the GettingStarted_CallResponse example, but the<br>
 * ping node powers down the radio and sleeps the MCU after every
 * ping/pong cycle, and the receiver sleeps between payloads. <br>
 */

 /**
 * @example rf24ping85.ino
 * <b>New: Contributed by https://github.com/tong67</b><br>
 * This is an example of how to use the RF24 class to communicate with ATtiny85 and other node. <br>
 */
 
 /**
 * @example timingSearch3pin.ino
 * <b>New: Contributed by https://github.com/tong67</b><br>
 * This is an example of how to determine the correct timing for ATtiny when using only 3-pins
 */
  
/**
 * @example pingpair_dyn.ino
 *
 * This is an example of how to use payloads of a varying (dynamic) size on Arduino.
 */
 
 /**
 * @example pingpair_dyn.cpp
 *
 * This is an example of how to use payloads of a varying (dynamic) size on Linux.
 */

/**
 * @example pingpair_dyn.py
 *
 * This is a python example for RPi of how to use payloads of a varying (dynamic) size.
 */ 
 
/**
 * @example scanner.ino
 *
 * Example to detect interference on the various channels available.
 * This is a good diagnostic tool to check whether you're picking a
 * good channel for your application.
 *
 * Inspired by cpixip.
 * See http://arduino.cc/forum/index.php/topic,54795.0.html
 */

/**
 * @mainpage Optimized High Speed Driver for nRF24L01(+) 2.4GHz Wireless Transceiver
 *
 * @section Goals Design Goals
 *
 * This library fork is designed to be...
 * @li More compliant with the manufacturer specified operation of the chip, while allowing advanced users
 * to work outside the recommended operation.
 * @li Utilize the capabilities of the radio to their full potential via Arduino
 * @li More reliable, responsive, bug-free and feature rich
 * @li Easy for beginners to use, with well documented examples and features
 * @li Consumed with a public interface that's similar to other Arduino standard libraries
 *
 * @section News News
 *
 * **Dec 2015**<br>
 * - ESP8266 support via Arduino IDE
 * - <a href="https://github.com/stewarthou/Particle-RF24">Particle Photon/Core</a> fork available
 * - ATTiny2313/4313 support added
 * - Python 3 support added
 * - RF24 added to Arduino library manager
 * - RF24 added to PlatformIO library manager
 *
 * **March 2015**<br>
 * - Uses SPI transactions on Arduino
 * - New layout for <a href="Portability.html">easier portability:</a> Break out defines & includes for individual platforms to RF24/utility
 * - <a href="MRAA.html">MRAA</a> support added ( Galileo, Edison, etc)
 * - <a href="Linux.html">Generic Linux support (SPIDEV)</a> support
 * - Support for RPi 2 added
 * - Major Documentation cleanup & update (Move all docs to github.io)
 *
 *
 * If issues are discovered with the documentation, please report them <a href="https://github.com/TMRh20/tmrh20.github.io/issues"> here</a>
 *
 * <br>
 * @section Useful Useful References
 *
 *
 * @li <a href="http://tmrh20.github.io/RF24/classRF24.html"><b>RF24</b> Class Documentation</a>
 * @li <a href="https://github.com/TMRh20/RF24/archive/master.zip"><b>Download</b></a>
 * @li <a href="https://github.com/tmrh20/RF24/"><b>Source Code</b></a>
 * @li <a href="http://tmrh20.blogspot.com/2014/03/high-speed-data-transfers-and-wireless.html"><b>My Blog:</b> RF24 Optimization Overview</a> 
 * @li <a href="http://tmrh20.blogspot.com/2016/08/raspberry-pilinux-with-nrf24l01.html"><b>My Blog:</b> RPi/Linux w/RF24Gateway</a> 
 * @li <a href="http://www.nordicsemi.com/files/Product/data_sheet/nRF24L01_Product_Specification_v2_0.pdf">Chip Datasheet</a>
 *
 * **Additional Information and Add-ons**
 *
 * @li <a href="http://tmrh20.github.io/RF24Network"> <b>RF24Network:</b> OSI Network Layer for multi-device communication. Create a home sensor network.</a>
 * @li <a href="http://tmrh20.github.io/RF24Mesh"> <b>RF24Mesh:</b> Dynamic Mesh Layer for RF24Network</a>
 * @li <a href="http://tmrh20.github.io/RF24Ethernet"> <b>RF24Ethernet:</b> TCP/IP Radio Mesh Networking (shares Arduino Ethernet API)</a>
 * @li <a href="http://tmrh20.github.io/RF24Audio"> <b>RF24Audio:</b> Realtime Wireless Audio streaming</a>
 * @li <a href="http://tmrh20.github.io/">All TMRh20 Documentation Main Page</a>
 *
 * **More Information and RF24 Based Projects**
 *
 * @li <a href="http://TMRh20.blogspot.com"> Project Blog: TMRh20.blogspot.com </a>
 * @li <a href="http://maniacalbits.blogspot.ca/"> Maniacal Bits Blog</a>
 * @li <a href="http://www.mysensors.org/">MySensors.org (User friendly sensor networks/IoT)</a>
 * @li <a href="https://github.com/mannkind/RF24Node_MsgProto"> RF24Node_MsgProto (MQTT)</a>
 * @li <a href="https://bitbucket.org/pjhardy/rf24sensornet/"> RF24SensorNet </a>
 * @li <a href="http://www.homeautomationforgeeks.com/rf24software.shtml">Home Automation for Geeks</a>
 * @li <a href="https://maniacbug.wordpress.com/2012/03/30/rf24network/"> Original Maniacbug RF24Network Blog Post</a>
 * @li <a href="https://github.com/maniacbug/RF24"> ManiacBug on GitHub (Original Library Author)</a>
 * 
 *
 * <br>
 *
 * @section Platform_Support Platform Support Pages
 *
 * @li <a href="Arduino.html"><b>Arduino</b></a> (Uno, Nano, Mega, Due, Galileo, etc)
 * @li <a href="ATTiny.html"><b>ATTiny</b></a>
 * @li <a href="Linux.html"><b>Linux devices</b></a>( <a href="RPi.html"><b>RPi</b></a> , <a href="Linux.html"><b>Linux SPI userspace device</b></a>, <a href="MRAA.html"><b>MRAA</b></a> supported boards ( Galileo, Edison, etc), <a href="LittleWire.html"><b>LittleWire</b></a>)
 * @li <a href="CrossCompile.html"><b>Cross-compilation</b></a> for linux devices
 * @li <a href="Python.html"><b>Python</b></a> wrapper available for Linux devices
 *
 * <br>
 * **General µC Pin layout** (See the individual board support pages for more info)
 *
 * The table below shows how to connect the the pins of the NRF24L01(+) to different boards.
 * CE and CSN are configurable.
 *
 * | PIN | NRF24L01 | Arduino UNO | ATtiny25/45/85 [0] | ATtiny44/84 [1] | LittleWire [2]          |    RPI     | RPi -P1 Connector |
 * |-----|----------|-------------|--------------------|-----------------|-------------------------|------------|-------------------|
 * |  1  |   GND    |   GND       |     pin 4          |    pin 14       | GND                     | rpi-gnd    |     (25)          |
 * |  2  |   VCC    |   3.3V      |     pin 8          |    pin  1       | regulator 3.3V required | rpi-3v3    |     (17)          |
 * |  3  |   CE     |   digIO 7   |     pin 2          |    pin 12       | pin to 3.3V             | rpi-gpio22 |     (15)          |
 * |  4  |   CSN    |   digIO 8   |     pin 3          |    pin 11       | RESET                   | rpi-gpio8  |     (24)          |
 * |  5  |   SCK    |   digIO 13  |     pin 7          |    pin  9       | SCK                     | rpi-sckl   |     (23)          |
 * |  6  |   MOSI   |   digIO 11  |     pin 6          |    pin  7       | MOSI                    | rpi-mosi   |     (19)          |
 * |  7  |   MISO   |   digIO 12  |     pin 5          |    pin  8       | MISO                    | rpi-miso   |     (21)          |
 * |  8  |   IRQ    |      -      |        -           |         -       | -                       |    -       |       -           |
 *
 * @li [0] https://learn.sparkfun.com/tutorials/tiny-avr-programmer-hookup-guide/attiny85-use-hints
 * @li [1] http://highlowtech.org/?p=1695
 * @li [2] http://littlewire.cc/   
 * <br><br><br>
 *
 *
 *
 *
 * @page Arduino Arduino
 * 
 * RF24 is fully compatible with Arduino boards <br>
 * See <b> http://www.arduino.cc/en/Reference/Board </b> and <b> http://arduino.cc/en/Reference/SPI </b> for more information
 * 
 * RF24 makes use of the standard hardware SPI pins (MISO,MOSI,SCK) and requires two additional pins, to control
 * the chip-select and chip-enable functions.<br>
 * These pins must be chosen and designated by the user, in RF24 radio(ce_pin,cs_pin); and can use any 
 * available pins.
 * 
 * <br>
 * @section ARD_DUE Arduino Due
 * 
 * RF24 makes use of the extended SPI functionality available on the Arduino Due, and requires one of the
 * defined hardware SS/CS pins to be designated in RF24 radio(ce_pin,cs_pin);<br>
 * See http://arduino.cc/en/Reference/DueExtendedSPI for more information
 *
 * Initial Due support taken from https://github.com/mcrosson/RF24/tree/due
 *
 * <br>
 * @section Alternate_SPI Alternate SPI Support
 *
 * RF24 supports alternate SPI methods, in case the standard hardware SPI pins are otherwise unavailable.
 * 
 * <br>
 * **Software Driven SPI**
 *
 * Software driven SPI is provided by the <a href=https://github.com/greiman/DigitalIO>DigitalIO</a> library
 *
 * Setup:<br>
 * 1. Install the digitalIO library<br>
 * 2. Open RF24_config.h in a text editor. 
      Uncomment the line 
      @code
      #define SOFTSPI
      @endcode
      or add the build flag/option
      @code
      -DSOFTSPI
      @endcode
 * 3. In your sketch, add
 *     @code
 *     #include DigitalIO.h
 *     @endcode
 *
 * @note Note: Pins are listed as follows and can be modified by editing the RF24_config.h file<br>
 *
 *     #define SOFT_SPI_MISO_PIN 16
 *     #define SOFT_SPI_MOSI_PIN 15
 *     #define SOFT_SPI_SCK_PIN 14
 * Or add the build flag/option
 *
 *     -DSOFT_SPI_MISO_PIN=16 -DSOFT_SPI_MOSI_PIN=15 -DSOFT_SPI_SCK_PIN=14
 *
 * <br>
 * **Alternate Hardware (UART) Driven  SPI**
 *
 * The Serial Port (UART) on Arduino can also function in SPI mode, and can double-buffer data, while the 
 * default SPI hardware cannot.
 *
 * The SPI_UART library is available at https://github.com/TMRh20/Sketches/tree/master/SPI_UART
 * 
 * Enabling:
 * 1. Install the SPI_UART library
 * 2. Edit RF24_config.h and uncomment #define SPI_UART
 * 3. In your sketch, add @code #include <SPI_UART.h> @endcode
 *
 * SPI_UART SPI Pin Connections:
 * | NRF |Arduino Uno Pin|
 * |-----|---------------|
 * | MOSI| TX(0)         |
 * | MISO| RX(1)         |
 * | SCK | XCK(4)        |
 * | CE  | User Specified|
 * | CSN | User Specified|
 *
 *
 * @note SPI_UART on Mega boards requires soldering to an unused pin on the chip. <br>See
 * https://github.com/TMRh20/RF24/issues/24 for more information on SPI_UART.
 * 
 * @page ATTiny ATTiny
 *
 * ATTiny support is built into the library, so users are not required to include SPI.h in their sketches<br>
 * See the included rf24ping85 example for pin info and usage
 * 
 * Some versions of Arduino IDE may require a patch to allow use of the full program space on ATTiny<br>
 * See https://github.com/TCWORLD/ATTinyCore/tree/master/PCREL%20Patch%20for%20GCC for ATTiny patch
 *
 * ATTiny board support initially added from https://github.com/jscrane/RF24
 *
 * @section Hardware Hardware Configuration
 * By tong67 ( https://github.com/tong67 )
 * 
 *    **ATtiny25/45/85 Pin map with CE_PIN 3 and CSN_PIN 4**
 * @code
 *                                 +-\/-+
 *                   NC      PB5  1|o   |8  Vcc --- nRF24L01  VCC, pin2 --- LED --- 5V
 *    nRF24L01  CE, pin3 --- PB3  2|    |7  PB2 --- nRF24L01  SCK, pin5
 *    nRF24L01 CSN, pin4 --- PB4  3|    |6  PB1 --- nRF24L01 MOSI, pin6
 *    nRF24L01 GND, pin1 --- GND  4|    |5  PB0 --- nRF24L01 MISO, pin7
 *                                 +----+ 
 * @endcode
 *
 * <br>
 *    **ATtiny25/45/85 Pin map with CE_PIN 3 and CSN_PIN 3** => PB3 and PB4 are free to use for application <br>
 *    Circuit idea from http://nerdralph.blogspot.ca/2014/01/nrf24l01-control-with-3-attiny85-pins.html <br>
 *   Original RC combination was 1K/100nF. 22K/10nF combination worked better.                          <br>
 *	For best settletime delay value in RF24::csn() the timingSearch3pin.ino sketch can be used.         <br>
 *    This configuration is enabled when CE_PIN and CSN_PIN are equal, e.g. both 3                      <br>
 *    Because CE is always high the power consumption is higher than for 5 pins solution                <br>
 * @code
 *                                                                                           ^^         
 *                                 +-\/-+           nRF24L01   CE, pin3 ------|              //         
 *                           PB5  1|o   |8  Vcc --- nRF24L01  VCC, pin2 ------x----------x--|<|-- 5V    
 *                   NC      PB3  2|    |7  PB2 --- nRF24L01  SCK, pin5 --|<|---x-[22k]--|  LED         
 *                   NC      PB4  3|    |6  PB1 --- nRF24L01 MOSI, pin6  1n4148 |                       
 *    nRF24L01 GND, pin1 -x- GND  4|    |5  PB0 --- nRF24L01 MISO, pin7         |                       
 *                        |        +----+                                       |                       
 *                        |-----------------------------------------------||----x-- nRF24L01 CSN, pin4  
 *                                                                      10nF                            
 * @endcode
 *
 * <br>
 *    **ATtiny24/44/84 Pin map with CE_PIN 8 and CSN_PIN 7** <br>
 *	Schematic provided and successfully tested by Carmine Pastore (https://github.com/Carminepz) <br>
 * @code
 *                                  +-\/-+                                                              
 *    nRF24L01  VCC, pin2 --- VCC  1|o   |14 GND --- nRF24L01  GND, pin1
 *                            PB0  2|    |13 AREF
 *                            PB1  3|    |12 PA1
 *                            PB3  4|    |11 PA2 --- nRF24L01   CE, pin3
 *                            PB2  5|    |10 PA3 --- nRF24L01  CSN, pin4
 *                            PA7  6|    |9  PA4 --- nRF24L01  SCK, pin5
 *    nRF24L01 MISO, pin7 --- PA6  7|    |8  PA5 --- nRF24L01 MOSI, pin6
 *                                  +----+
 *	@endcode					 
 *	
 * <br>
 *    **ATtiny2313/4313 Pin map with CE_PIN 12 and CSN_PIN 13** <br>
 * @code
 *                                  +-\/-+                                                              
 *                            PA2  1|o   |20 VCC --- nRF24L01  VCC, pin2
 *                            PD0  2|    |19 PB7 --- nRF24L01  SCK, pin5
 *                            PD1  3|    |18 PB6 --- nRF24L01 MOSI, pin6
 *                            PA1  4|    |17 PB5 --- nRF24L01 MISO, pin7
 *                            PA0  5|    |16 PB4 --- nRF24L01  CSN, pin4
 *                            PD2  6|    |15 PB3 --- nRF24L01   CE, pin3
 *                            PD3  7|    |14 PB2
 *                            PD4  8|    |13 PB1
 *                            PD5  9|    |12 PB0
 *    nRF24L01  GND, pin1 --- GND 10|    |11 PD6
 *                                  +----+
 *	@endcode					 
 *
 * <br><br><br>
 *
 *
 * 
 * 
 *
 *
 * @page Linux Linux devices
 *
 * Generic Linux devices are supported via SPIDEV, MRAA, RPi native via BCM2835, or using LittleWire.
 *
 *  @note The SPIDEV option should work with most Linux systems supporting spi userspace device. <br>
 *
 * <br>
 * @section AutoInstall Automated Install 
 *(**Designed & Tested on RPi** - Defaults to SPIDEV on devices supporting it)
 *
 * 
 * 1. Install prerequisites if there are any (MRAA, LittleWire libraries, setup SPI device etc)
 * 2. Download the install.sh file from http://tmrh20.github.io/RF24Installer/RPi/install.sh
 * @code wget http://tmrh20.github.io/RF24Installer/RPi/install.sh @endcode
 * 3. Make it executable
 * @code chmod +x install.sh @endcode
 * 4. Run it and choose your options
 * @code ./install.sh @endcode
 * 5. Run an example from one of the libraries
 * @code 
 * cd rf24libs/RF24/examples_linux  
 * @endcode
 * Edit the gettingstarted example, to set your pin configuration
 * @code nano gettingstarted.cpp
 * make  
 * sudo ./gettingstarted  
 * @endcode
 *
 * <br>
 * @section ManInstall Manual Install
 * 1. Install prerequisites if there are any (MRAA, LittleWire libraries, setup SPI device etc)
 * @note See the <a href="http://iotdk.intel.com/docs/master/mraa/index.html">MRAA </a> documentation for more info on installing MRAA <br>
 * 2. Make a directory to contain the RF24 and possibly RF24Network lib and enter it
 * @code
 *  mkdir ~/rf24libs 
 *  cd ~/rf24libs
*  @endcode
 * 3. Clone the RF24 repo
 *    @code git clone https://github.com/tmrh20/RF24.git RF24 @endcode
 * 4. Change to the new RF24 directory
 *    @code cd RF24 @endcode
 * 5. Configure build environment using @code ./configure @endcode script. It auto detectes device and build environment. For overriding autodetections, use command-line switches, see @code ./configure --help @endcode for description.
 * 6. Build the library, and run an example file
 * @code sudo make install @endcode
 * @code
 * cd examples_linux  
 * @endcode
 * Edit the gettingstarted example, to set your pin configuration
 * @code nano gettingstarted.cpp 
 * make 
 * sudo ./gettingstarted
 * @endcode
 *
 * <br><br>
 *   
 * @page MRAA MRAA
 *  
 * MRAA is a Low Level Skeleton Library for Communication on GNU/Linux platforms <br>
 * See http://iotdk.intel.com/docs/master/mraa/index.html for more information
 *
 * RF24 supports all MRAA supported platforms, but might not be tested on each individual platform due to the wide range of hardware support:<br>
 * <a href="https://github.com/TMRh20/RF24/issues">Report an RF24 bug or issue </a>
 *
 * @section Setup Setup and installation
 * 1. Install the MRAA lib
 * 2. As per your device, SPI may need to be enabled
 * 3. Follow <a href="Linux.html">Linux installation steps</a> to install RF24 libraries
 * 
 *
 * <br><br><br>
 *
 * 
 *
 *
 * @page RPi Raspberry Pi
 *
 * RF24 supports a variety of Linux based devices via various drivers. Some boards like RPi can utilize multiple methods
 * to drive the GPIO and SPI functionality.
 *
 * <br>
 * @section PreConfig Potential PreConfiguration
 *
 * If SPI is not already enabled, load it on boot:
 * @code sudo raspi-config  @endcode
 * A. Update the tool via the menu as required<br>
 * B. Select **Advanced** and **enable the SPI kernel module** <br>
 * C. Update other software and libraries
 * @code sudo apt-get update @endcode
 * @code sudo apt-get upgrade @endcode 
 * <br><br>
 *
 * @section Build Build Options
 * The default build on Raspberry Pi utilizes the included **BCM2835** driver from http://www.airspayce.com/mikem/bcm2835
 * 1. @code sudo make install -B @endcode
 *
 * Build using the **MRAA** library from http://iotdk.intel.com/docs/master/mraa/index.html <br>
 * MRAA is not included. See the <a href="MRAA.html">MRAA</a> platform page for more information.
 *
 * 1. Install, and build MRAA
 * @code
 * git clone https://github.com/intel-iot-devkit/mraa.git
 * cd mraa
 * mkdir build
 * cd build
 * cmake .. -DBUILDSWIGNODE=OFF
 * sudo make install
 * @endcode
 *
 * 2. Complete the install <br>
 * @code nano /etc/ld.so.conf @endcode
 * Add the line @code /usr/local/lib/arm-linux-gnueabihf @endcode
 * Run @code sudo ldconfig @endcode
 *
 * 3. Install RF24, using MRAA
 * @code
 * ./configure --driver=MRAA
 * sudo make install -B
 * @endcode
 * See the gettingstarted example for an example of pin configuration
 *
 * Build using **SPIDEV**
 *
 * 1. Make sure that spi device support is enabled and /dev/spidev\<a\>.\<b\> is present
 * 2. Install RF24, using SPIDEV
 * @code
 * ./configure --driver=SPIDEV
 * sudo make install -B
 * @endcode
 * 3. See the gettingstarted example for an example of pin configuration
 *
 * <br>
 * @section Pins Connections and Pin Configuration
 *
 *
 * Using pin 15/GPIO 22 for CE, pin 24/GPIO8 (CE0) for CSN
 *
 * Can use either RPi CE0 or CE1 pins for radio CSN.<br>
 * Choose any RPi output pin for radio CE pin.
 *
 * **BCM2835 Constructor:**
 * @code
 *  RF24 radio(RPI_V2_GPIO_P1_15,BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
 *   or
 *  RF24 radio(RPI_V2_GPIO_P1_15,BCM2835_SPI_CS1, BCM2835_SPI_SPEED_8MHZ);
 *	
 *  RPi B+:
 *  RF24 radio(RPI_BPLUS_GPIO_J8_15,RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ);
 *  or
 *  RF24 radio(RPI_BPLUS_GPIO_J8_15,RPI_BPLUS_GPIO_J8_26, BCM2835_SPI_SPEED_8MHZ);
 *
 *  General:
 *  RF24 radio(22,0);
 *  or
 *  RF24 radio(22,1);
 *
 * @endcode
 * See the gettingstarted example for an example of pin configuration
 *
 * See http://www.airspayce.com/mikem/bcm2835/index.html for BCM2835 class documentation.
 * <br><br>
 * **MRAA Constructor:**
 *
 * @code RF24 radio(15,0); @endcode
 *
 * See http://iotdk.intel.com/docs/master/mraa/rasppi.html
 * <br><br>
 * **SPI_DEV Constructor**
 *
 * @code RF24 radio(22,0); @endcode
 * In general, use @code RF24 radio(<ce_pin>, <a>*10+<b>); @endcode for proper SPIDEV constructor to address correct spi device at /dev/spidev\<a\>.\<b\>
 *
 * See http://pi.gadgetoid.com/pinout
 *
 * **Pins:**  
 *
 * | PIN | NRF24L01 |    RPI     | RPi -P1 Connector |
 * |-----|----------|------------|-------------------|
 * |  1  |   GND    | rpi-gnd    |     (25)          |
 * |  2  |   VCC    | rpi-3v3    |     (17)          |
 * |  3  |   CE     | rpi-gpio22 |     (15)          |
 * |  4  |   CSN    | rpi-gpio8  |     (24)          |
 * |  5  |   SCK    | rpi-sckl   |     (23)          |
 * |  6  |   MOSI   | rpi-mosi   |     (19)          |
 * |  7  |   MISO   | rpi-miso   |     (21)          |
 * |  8  |   IRQ    |    -       |       -           |
 *   
 *   
 *  
 *  
 * <br><br>
 ****************
 *   
 * Based on the arduino lib from J. Coliz <maniacbug@ymail.com>  <br>
 * the library was berryfied by Purinda Gunasekara <purinda@gmail.com> <br>  
 * then forked from github stanleyseow/RF24 to https://github.com/jscrane/RF24-rpi  <br>
 * Network lib also based on https://github.com/farconada/RF24Network
 *
 * 
 *
 * 
 * <br><br><br>
 * 
 *
 *  
 * @page Python Python Wrapper (by https://github.com/mz-fuzzy)
 *
 * @note Both python2 and python3 are supported.
 *
 * @section Install Installation:  
 *
 * 1. Install the python-dev (or python3-dev) and boost libraries
 * @code sudo apt-get install python-dev libboost-python-dev @endcode
 * @note For python3 in Raspbian, it's needed to manually link python boost library, like this:
 * @code sudo ln -s /usr/lib/arm-linux-gnueabihf/libboost_python-py34.so /usr/lib/arm-linux-gnueabihf/libboost_python3.so @endcode
 *
 * 2. Install python-setuptools (or python3-setuptools)
 * @code sudo apt-get install python-setuptools @endcode
 *
 * 3. Build the library
 * @code ./setup.py build   @endcode
 * @note Build takes several minutes on arm-based machines. Machines with RAM <1GB may need to increase amount of swap for build.
 *
 * 4. Install the library
 * @code sudo ./setup.py install  @endcode
 * See the additional <a href="pages.html">Platform Support</a> pages for information on connecting your hardware  <br>
 * See the included <a href="pingpair_dyn_8py-example.html">example </a> for usage information.   
 * 
 * 5. Running the Example
 * Edit the pingpair_dyn.py example to configure the appropriate pins per the above documentation:  
 * @code nano pingpair_dyn.py   @endcode
 * Configure another device, Arduino or RPi with the <a href="pingpair_dyn_8py-example.html">pingpair_dyn</a> example <br>
 * Run the example  
 * @code sudo ./pingpair_dyn.py  @endcode
 *
 * <br><br><br>
 *
 * @page CrossCompile Linux cross-compilation
 * 
 * RF24 library supports cross-compilation. Advantages of cross-compilation:
 *  - development tools don't have to be installed on target machine
 *  - resources of target machine don't have to be sufficient for compilation
 *  - compilation time can be reduced for large projects
 *
 *  Following prerequisites need to be assured:
 *  - ssh passwordless access to target machine (https://linuxconfig.org/passwordless-ssh)
 *  - sudo of a remote user without password (http://askubuntu.com/questions/334318/sudoers-file-enable-nopasswd-for-user-all-commands)
 *  - cross-compilation toolchain for your target machine; for RPi
 *  @code git clone https://github.com/raspberrypi/tools rpi_tools @endcode
 *  and cross-compilation tools must be in PATH, for example
 *  @code export PATH=$PATH:/your/dir/rpi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin @endcode
 *
 * @section CxSteps Cross compilation steps:
 * 1. clone RF24 to a machine for cross-compilation
 * @code
 * git clone https://github.com/TMRh20/RF24
 * cd RF24
 * @endcode
 * 2. configure for cross compilation
 * @code ./configure --remote=pi@target_linux_host @endcode
 * eventually
 * @code ./configure --remote=pi@target_linux_host --driver=<driver> @endcode
 * 3. build
 * @code make @endcode
 * 4. (opt) install library to cross-compilation machine into cross-exvironment - important for compilation of examples
 * @code sudo make install @endcode
 * 5. upload library to target machine
 * @code make upload @endcode
 * 6. (opt) compile examples
 * @code
 * cd examples_linux
 * make
 * @endcode
 * 7. (opt) upload examples to target machine
 * @code make upload @endcode
 *
 * @section CxStepsPython Cross comilation steps for python wrapper
 *
 * Prerequisites:
 * - Python setuptools must be installed on both target and cross-compilation machines
 *   @code sudo pip install setuptools @endcode
 *   or
 *   @code sudo apt-get install python-setuptools @endcode
 *
 * Installation steps:
 * 1. Assure having libboost-python-dev library in your cross-compilation environment. Alternatively, you can install it into your target machine and copy /usr and /lib directories to the cross-compilation machine.
 * For example
 * @code
 * mkdir -p rpi_root && rsync -a pi@target_linux_host:/usr :/lib rpi_root
 * export CFLAGS="--sysroot=/your/dir/rpi_root -I/your/dir/rpi_root/usr/include/python2.7/"
 * @endcode
 *
 * 2. Build the python wrapper
 * @code
 * cd pyRF24
 * ./setup.py build --compiler=crossunix
 * @endcode
 *
 * 3. Make the egg package
 * @code ./setup.py bdist_egg --plat-name=cross @endcode
 * dist/RF24-<version>-cross.egg should be created.
 *
 * 4. Upload it to the target machine and install there:
 * @code
 * scp dist/RF24-*-cross.egg pi@target_linux_host:
 * ssh pi@target_linux_host 'sudo easy_install RF24-*-cross.egg'
 * @endcode
 *
 * <br><br><br>
 *
 * @page ATXMEGA ATXMEGA
 * 
 * The RF24 driver can be build as a static library with Atmel Studio 7 in order to be included as any other library in another program for the XMEGA family.
 *
 * Currently only the <b>ATXMEGA D3</b> family is implemented.
 * 
 * @section Preparation 
 * 
 * Create an empty GCC Static Library project in AS7.<br>
 * As not all files are required, copy the following directory structure in the project:
 * 
 * @code
 * utility\
 *   ATXMegaD3\
 *     compatibility.c
 *     compatibility.h
 *     gpio.cpp
 *     gpio.h
 *     gpio_helper.c
 *     gpio_helper.h
 *     includes.h
 *     RF24_arch_config.h
 *     spi.cpp
 *     spi.h
 * nRF24L01.h
 * printf.h
 * RF24.cpp
 * RF24.h
 * RF24_config.h
 * @endcode
 * 
 * @section Usage
 * 
 * Add the library to your project!<br>
 * In the file where the **main()** is put the following in order to update the millisecond functionality:
 * 
 * @code
 * ISR(TCE0_OVF_vect)
 * {
 * 	update_milisec();
 * }
 * @endcode
 * 
 * Declare the rf24 radio with **RF24 radio(XMEGA_PORTC_PIN3, XMEGA_SPI_PORT_C);**
 * 
 * First parameter is the CE pin which can be any available pin on the uC.
 * 
 * Second parameter is the CS which can be on port C (**XMEGA_SPI_PORT_C**) or on port D (**XMEGA_SPI_PORT_D**). 
 * 
 * Call the **__start_timer()** to start the millisecond timer.
 * 
 * @note Note about the millisecond functionality:<br>
 * 
 * 	The millisecond functionality is based on the TCE0 so don't use these pins as IO.<br>
 * 	The operating frequency of the uC is 32MHz. If you have other frequency change the TCE0 registers appropriatly in function **__start_timer()** in **compatibility.c** file for your frequency. 
 *
 * @page Portability RF24 Portability
 *
 * The RF24 radio driver mainly utilizes the <a href="http://arduino.cc/en/reference/homePage">Arduino API</a> for GPIO, SPI, and timing functions, which are easily replicated
 * on various platforms. <br>Support files for these platforms are stored under RF24/utility, and can be modified to provide 
 * the required functionality.
 * 
 * <br>
 * @section Hardware_Templates Basic Hardware Template
 *
 * **RF24/utility**
 *
 * The RF24 library now includes a basic hardware template to assist in porting to various platforms. <br> The following files can be included
 * to replicate standard Arduino functions as needed, allowing devices from ATTiny to Raspberry Pi to utilize the same core RF24 driver.
 *
 * | File               |                   Purpose                                                    | 
 * |--------------------|------------------------------------------------------------------------------| 
 * | RF24_arch_config.h | Basic Arduino/AVR compatibility, includes for remaining support files, etc   | 
 * | includes.h         | Linux only. Defines specific platform, include correct RF24_arch_config file | 
 * | spi.h              | Provides standardized SPI ( transfer() ) methods                         | 
 * | gpio.h             | Provides standardized GPIO ( digitalWrite() ) methods                        | 
 * | compatibility.h    | Provides standardized timing (millis(), delay()) methods                     | 
 * | your_custom_file.h | Provides access to custom drivers for spi,gpio, etc                          | 
 *
 * <br>
 * Examples are provided via the included hardware support templates in **RF24/utility** <br>
 * See the <a href="modules.html">modules</a> page for examples of class declarations 
 *
 *<br>
 * @section Device_Detection Device Detection
 *
 * 1. The main detection for Linux devices is done in the configure script, with the includes.h from the proper hardware directory copied to RF24/utility/includes.h <br>
 * 2. Secondary detection is completed in RF24_config.h, causing the include.h file to be included for all supported Linux devices <br>
 * 3. RF24.h contains the declaration for SPI and GPIO objects 'spi' and 'gpio' to be used for porting-in related functions.
 *
 * <br>
 * @section Ported_Code Code
 * To have your ported code included in this library, or for assistance in porting, create a pull request or open an issue at https://github.com/TMRh20/RF24
 * 
 *
 *<br><br><br>
 */

#endif // __RF24_H__
