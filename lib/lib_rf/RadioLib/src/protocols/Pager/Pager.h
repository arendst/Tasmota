#if !defined(_RADIOLIB_PAGER_H) && !RADIOLIB_EXCLUDE_PAGER
#define _RADIOLIB_PAGER_H

#include "../../TypeDef.h"
#include "../PhysicalLayer/PhysicalLayer.h"
#include "../../utils/FEC.h"

// frequency shift in Hz
#define RADIOLIB_PAGER_FREQ_SHIFT_HZ                            (4500)

// supported encoding schemes
#define RADIOLIB_PAGER_ASCII                                    (0)
#define RADIOLIB_PAGER_BCD                                      (1)

// preamble length in 32-bit code words
#define RADIOLIB_PAGER_PREAMBLE_LENGTH                          (18)

// protocol-specified code words
#define RADIOLIB_PAGER_PREAMBLE_CODE_WORD                       (0xAAAAAAAA)
#define RADIOLIB_PAGER_FRAME_SYNC_CODE_WORD                     (0x7CD215D8)
#define RADIOLIB_PAGER_IDLE_CODE_WORD                           (0x7A89C197)

// code word type identification flags
#define RADIOLIB_PAGER_ADDRESS_CODE_WORD                        (0UL)
#define RADIOLIB_PAGER_MESSAGE_CODE_WORD                        (1UL)

// length of code word in bits
#define RADIOLIB_PAGER_CODE_WORD_LEN                            (32)

// number of message bits in a single code block
#define RADIOLIB_PAGER_ADDRESS_POS                              (13)
#define RADIOLIB_PAGER_FUNC_BITS_POS                            (11)
#define RADIOLIB_PAGER_MESSAGE_BITS_LENGTH                      (20)
#define RADIOLIB_PAGER_MESSAGE_END_POS                          (11)

// number of code words in a batch
#define RADIOLIB_PAGER_BATCH_LEN                                (16)

// mask for address bits in a single code word
#define RADIOLIB_PAGER_ADDRESS_BITS_MASK                        (0x7FFFE000UL)

// mask for function bits in a single code word
#define RADIOLIB_PAGER_FUNCTION_BITS_MASK                       (0x00001800UL)

// mask for BCH bits in a single code word
#define RADIOLIB_PAGER_BCH_BITS_MASK                            (0x000007FFUL)

// message type functional bits
#define RADIOLIB_PAGER_FUNC_BITS_NUMERIC                        (0b00)
#define RADIOLIB_PAGER_FUNC_BITS_TONE                           (0b01)
#define RADIOLIB_PAGER_FUNC_BITS_ACTIVATION                     (0b10)
#define RADIOLIB_PAGER_FUNC_BITS_ALPHA                          (0b11)
#define RADIOLIB_PAGER_FUNC_AUTO                                0xFF

// the maximum allowed address (2^22 - 1)
#define RADIOLIB_PAGER_ADDRESS_MAX                              (2097151)

/*!
  \class PagerClient
  \brief Client for Pager communication.
*/
class PagerClient {
  public:
    /*!
      \brief Default constructor.
      \param phy Pointer to the wireless module providing PhysicalLayer communication.
    */
    explicit PagerClient(PhysicalLayer* phy);

    // basic methods

    /*!
      \brief Initialization method.
      \param base Base (center) frequency to be used in MHz.
      \param speed Bit rate to use in bps. Common POCSAG decoders can receive 512, 1200 and 2400 bps.
      \param invert Enable frequency inversion. Disabled by default (high frequency is digital 0).
      \param shift Set custom frequency shift, defaults to 4500 Hz.
      \returns \ref status_codes
    */
    int16_t begin(float base, uint16_t speed, bool invert = false, uint16_t shift = RADIOLIB_PAGER_FREQ_SHIFT_HZ);

    /*!
      \brief Method to send a tone-only alert to a destination pager.
      \param addr Address of the destination pager. Allowed values are 0 to 2097151 - values above 2000000 are reserved.
      \returns \ref status_codes
    */
    int16_t sendTone(uint32_t addr);

    #if defined(RADIOLIB_BUILD_ARDUINO)
    /*!
      \brief Arduino String transmit method.
      \param str Address of Arduino string that will be transmitted.
      \param addr Address of the destination pager. Allowed values are 0 to 2097151 - values above 2000000 are reserved.
      \param encoding Encoding to be used (BCD or ASCII). Defaults to RADIOLIB_PAGER_BCD.
      \param function bits (NUMERIC, TONE, ACTIVATION, ALPHANUMERIC). Allowed values 0 to 3. Defaults to auto select by specified encoding
      \returns \ref status_codes
    */
    int16_t transmit(String& str, uint32_t addr, uint8_t encoding = RADIOLIB_PAGER_BCD, uint8_t function = RADIOLIB_PAGER_FUNC_AUTO);
    #endif

    /*!
      \brief C-string transmit method.
      \param str C-string that will be transmitted.
      \param addr Address of the destination pager. Allowed values are 0 to 2097151 - values above 2000000 are reserved.
      \param encoding Encoding to be used (BCD or ASCII). Defaults to RADIOLIB_PAGER_BCD.
      \param function bits (NUMERIC, TONE, ACTIVATION, ALPHANUMERIC). Allowed values 0 to 3. Defaults to auto select by specified encoding
      \returns \ref status_codes
    */
    int16_t transmit(const char* str, uint32_t addr, uint8_t encoding = RADIOLIB_PAGER_BCD, uint8_t function = RADIOLIB_PAGER_FUNC_AUTO);

    /*!
      \brief Binary transmit method. Will transmit arbitrary binary data.
      \param data Binary data that will be transmitted.
      \param len Length of binary data to transmit (in bytes).
      \param addr Address of the destination pager. Allowed values are 0 to 2097151 - values above 2000000 are reserved.
      \param encoding Encoding to be used (BCD or ASCII). Defaults to RADIOLIB_PAGER_BCD.
      \param function bits (NUMERIC, TONE, ACTIVATION, ALPHANUMERIC). Allowed values 0 to 3. Defaults to auto select by specified encoding
      \returns \ref status_codes
    */
    int16_t transmit(uint8_t* data, size_t len, uint32_t addr, uint8_t encoding = RADIOLIB_PAGER_BCD, uint8_t function = RADIOLIB_PAGER_FUNC_AUTO);

    #if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
    /*!
      \brief Start reception of POCSAG packets.
      \param pin Pin to receive digital data on (e.g., DIO2 for SX127x).
      \param addr Address of this "pager". Allowed values are 0 to 2097151 - values above 2000000 are reserved.
      \param mask Address filter mask - set individual bits to enable or disable match on that bit of the address.
      Set to 0xFFFFF (all bits checked) by default.
      \returns \ref status_codes
    */
    int16_t startReceive(uint32_t pin, uint32_t addr, uint32_t mask = 0xFFFFF);

    /*!
      \brief Start reception of POCSAG packets for multiple addresses and masks.
      \param pin Pin to receive digital data on (e.g., DIO2 for SX127x).
      \param addrs Array of addresses to receive.
      \param masks Array of address masks to use for filtering. Masks will be applied to corresponding addresses in addr array.
      \param numAddress Number of addresses/masks to match.
      \returns \ref status_codes
    */
    int16_t startReceive(uint32_t pin, uint32_t *addrs, uint32_t *masks, size_t numAddress);

    /*!
      \brief Get the number of POCSAG batches available in buffer. Limited by the size of direct mode buffer!
      \returns Number of available batches.
    */
    size_t available();

    #if defined(RADIOLIB_BUILD_ARDUINO)
    /*!
      \brief Reads data that was received after calling startReceive method.
      \param str Address of Arduino String to save the received data.
      \param len Expected number of characters in the message. When set to 0, the message length will be retrieved
      automatically. When more bytes than received are requested, only the number of bytes requested will be returned.
      \param addr Pointer to variable holding the address of the received pager message.
      Set to NULL to not retrieve address.
      \returns \ref status_codes
    */
    int16_t readData(String& str, size_t len = 0, uint32_t* addr = NULL);
    #endif

    /*!
      \brief Reads data that was received after calling startReceive method.
      \param data Pointer to array to save the received message.
      \param len Pointer to variable holding the number of bytes that will be read. When set to 0, the packet length
      will be retrieved automatically. When more bytes than received are requested, only the number of bytes
      requested will be returned. Upon completion, the number of bytes received will be written to this variable.
      \param addr Pointer to variable holding the address of the received pager message.
      Set to NULL to not retrieve address.
      \returns \ref status_codes
    */
    int16_t readData(uint8_t* data, size_t* len, uint32_t* addr = NULL);
#endif

#if !RADIOLIB_GODMODE
  private:
#endif
    PhysicalLayer* phyLayer;

    float baseFreq;
    float dataRate;
    uint32_t baseFreqRaw;
    uint16_t shiftFreq;
    uint16_t shiftFreqHz;
    uint16_t bitDuration;
    uint32_t filterAddr;
    uint32_t filterMask;
    uint32_t *filterAddresses;
    uint32_t *filterMasks;
    size_t filterNumAddresses;
    bool inv = false;

    void write(uint32_t* data, size_t len);
    void write(uint32_t codeWord);
    int16_t startReceiveCommon();
    bool addressMatched(uint32_t addr);

#if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
    uint32_t read();
#endif

    uint8_t encodeBCD(char c);
    char decodeBCD(uint8_t b);
};

#endif
