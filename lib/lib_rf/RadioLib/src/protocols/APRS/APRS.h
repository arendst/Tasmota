#if !defined(_RADIOLIB_APRS_H)
#define _RADIOLIB_APRS_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_APRS

#include "../PhysicalLayer/PhysicalLayer.h"
#include "../AX25/AX25.h"

// APRS data type identifiers
#define RADIOLIB_APRS_DATA_TYPE_POSITION_NO_TIME_NO_MSG         "!"
#define RADIOLIB_APRS_DATA_TYPE_GPS_RAW                         "$"
#define RADIOLIB_APRS_DATA_TYPE_ITEM                            ")"
#define RADIOLIB_APRS_DATA_TYPE_TEST                            ","
#define RADIOLIB_APRS_DATA_TYPE_POSITION_TIME_NO_MSG            "/"
#define RADIOLIB_APRS_DATA_TYPE_MSG                             ":"
#define RADIOLIB_APRS_DATA_TYPE_OBJECT                          ";"
#define RADIOLIB_APRS_DATA_TYPE_STATION_CAPABILITES             "<"
#define RADIOLIB_APRS_DATA_TYPE_POSITION_NO_TIME_MSG            "="
#define RADIOLIB_APRS_DATA_TYPE_STATUS                          ">"
#define RADIOLIB_APRS_DATA_TYPE_QUERY                           "?"
#define RADIOLIB_APRS_DATA_TYPE_POSITION_TIME_MSG               "@"
#define RADIOLIB_APRS_DATA_TYPE_TELEMETRY                       "T"
#define RADIOLIB_APRS_DATA_TYPE_MAIDENHEAD_BEACON               "["
#define RADIOLIB_APRS_DATA_TYPE_WEATHER_REPORT                  "_"
#define RADIOLIB_APRS_DATA_TYPE_USER_DEFINED                    "{"
#define RADIOLIB_APRS_DATA_TYPE_THIRD_PARTY                     "}"

/*!
  \defgroup mic_e_message_types Mic-E message types.

  \{
*/
#define RADIOLIB_APRS_MIC_E_TYPE_OFF_DUTY                       0b00000111
#define RADIOLIB_APRS_MIC_E_TYPE_EN_ROUTE                       0b00000110
#define RADIOLIB_APRS_MIC_E_TYPE_IN_SERVICE                     0b00000101
#define RADIOLIB_APRS_MIC_E_TYPE_RETURNING                      0b00000100
#define RADIOLIB_APRS_MIC_E_TYPE_COMMITTED                      0b00000011
#define RADIOLIB_APRS_MIC_E_TYPE_SPECIAL                        0b00000010
#define RADIOLIB_APRS_MIC_E_TYPE_PRIORITY                       0b00000001
#define RADIOLIB_APRS_MIC_E_TYPE_EMERGENCY                      0b00000000
/*!
  \}
*/

// magic offset applied to encode extra bits in the Mic-E destination field
#define RADIOLIB_APRS_MIC_E_DEST_BIT_OFFSET                     25

// Mic-E data types
#define RADIOLIB_APRS_MIC_E_GPS_DATA_CURRENT                    '`'
#define RADIOLIB_APRS_MIC_E_GPS_DATA_OLD                        '\''

// Mic-E telemetry flags
#define RADIOLIB_APRS_MIC_E_TELEMETRY_LEN_2                     '`'
#define RADIOLIB_APRS_MIC_E_TELEMETRY_LEN_5                     '\''

// alias for unused altitude in Mic-E
#define RADIOLIB_APRS_MIC_E_ALTITUDE_UNUSED                     -1000000

// special header applied for APRS over LoRa
#define RADIOLIB_APRS_LORA_HEADER                               "<\xff\x01"
#define RADIOLIB_APRS_LORA_HEADER_LEN                           (3)

/*!
  \class APRSClient
  \brief Client for APRS communication.
*/
class APRSClient {
  public:
    /*!
      \brief Constructor for "classic" mode using AX.25/AFSK.
      \param ax Pointer to the instance of AX25Client to be used for APRS.
    */
    explicit APRSClient(AX25Client* ax);

    /*!
      \brief Constructor for LoRa mode.
      \param phy Pointer to the wireless module providing PhysicalLayer communication.
    */
    explicit APRSClient(PhysicalLayer* phy);

    // basic methods

    /*!
      \brief Initialization method.
      \param sym APRS symbol to be displayed.
      \param callsign Source callsign. Required and only used for APRS over LoRa, ignored in classic mode.
      \param ssid Source SSID. Only used for APRS over LoRa, ignored in classic mode, defaults to 0.
      \param alt Whether to use the primary (false) or alternate (true) symbol table. Defaults to primary table.
      \returns \ref status_codes
    */
    int16_t begin(char sym, char* callsign = NULL, uint8_t ssid = 0, bool alt = false);

    /*!
      \brief Transmit position.
      \param destCallsign Destination station callsign.
      \param destSSID Destination station SSID.
      \param lat Latitude as a null-terminated string.
      \param long Longitude as a null-terminated string.
      \param msg Message to be transmitted. Defaults to NULL (no message).
      \param msg Position timestamp. Defaults to NULL (no timestamp).
      \returns \ref status_codes
    */
    int16_t sendPosition(char* destCallsign, uint8_t destSSID, char* lat, char* lon, char* msg = NULL, char* time = NULL);

    /*!
      \brief Transmit position using Mic-E encoding.
      \param lat Geographical latitude, positive for north, negative for south.
      \param lon Geographical longitude, positive for east, negative for west.
      \param heading Heading in degrees.
      \param speed Speed in knots.
      \param type Mic-E message type - see \ref mic_e_message_types.
      \param telem Pointer to telemetry array (either 2 or 5 bytes long). NULL when telemetry is not used.
      \param telemLen Telemetry length, 2 or 5. 0 when telemetry is not used.
      \param grid Maidenhead grid locator. NULL when not used.
      \param status Status message to send. NULL when not used.
      \param alt Altitude to send. RADIOLIB_APRS_MIC_E_ALTITUDE_UNUSED when not used.
    */
    int16_t sendMicE(float lat, float lon, uint16_t heading, uint16_t speed, uint8_t type, uint8_t* telem = NULL, size_t telemLen = 0, char* grid = NULL, char* status = NULL, int32_t alt = RADIOLIB_APRS_MIC_E_ALTITUDE_UNUSED);

    /*!
      \brief Transmit generic APRS frame.
      \param destCallsign Destination station callsign.
      \param destSSID Destination station SSID.
      \param info AX.25 info field contents.
      \returns \ref status_codes
    */
    int16_t sendFrame(char* destCallsign, uint8_t destSSID, char* info);

#if !RADIOLIB_GODMODE
  private:
#endif
    AX25Client* axClient;
    PhysicalLayer* phyLayer;

    // default APRS symbol (car)
    char symbol = '>';
    char table = '/';
    
    // source callsign when using APRS over LoRa
    char src[RADIOLIB_AX25_MAX_CALLSIGN_LEN + 1] = { 0 };
    uint8_t id = 0;
};

#endif

#endif
