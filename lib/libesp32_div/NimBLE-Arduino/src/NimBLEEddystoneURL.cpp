/*
 * NimBLEEddystoneURL.cpp
 *
 *  Created: on March 15 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEEddystoneURL.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: pcbreflux
 */
#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLEEddystoneURL.h"
#include "NimBLELog.h"

#include <cstring>

static const char LOG_TAG[] = "NimBLEEddystoneURL";


/**
 * @brief Construct a default EddystoneURL beacon object.
 */
NimBLEEddystoneURL::NimBLEEddystoneURL() {
    beaconUUID = 0xFEAA;
    lengthURL = 0;
    m_eddystoneData.frameType = EDDYSTONE_URL_FRAME_TYPE;
    m_eddystoneData.advertisedTxPower = 0;
    memset(m_eddystoneData.url, 0, sizeof(m_eddystoneData.url));
} // BLEEddystoneURL


/**
 * @brief Retrieve the data that is being advertised.
 * @return The advertised data.
 */
std::string NimBLEEddystoneURL::getData() {
    return std::string((char*) &m_eddystoneData, sizeof(m_eddystoneData));
} // getData


/**
 * @brief Get the UUID being advertised.
 * @return The UUID advertised.
 */
NimBLEUUID NimBLEEddystoneURL::getUUID() {
    return NimBLEUUID(beaconUUID);
} // getUUID


/**
 * @brief Get the transmit power being advertised.
 * @return The transmit power.
 */
int8_t NimBLEEddystoneURL::getPower() {
    return m_eddystoneData.advertisedTxPower;
} // getPower


/**
 * @brief Get the raw URL being advertised.
 * @return The raw URL.
 */
std::string NimBLEEddystoneURL::getURL() {
    return std::string((char*) &m_eddystoneData.url, sizeof(m_eddystoneData.url));
} // getURL


/**
 * @brief Get the full URL being advertised.
 * @return The full URL.
 */
std::string NimBLEEddystoneURL::getDecodedURL() {
    std::string decodedURL = "";

    switch (m_eddystoneData.url[0]) {
        case 0x00:
            decodedURL += "http://www.";
            break;
        case 0x01:
            decodedURL += "https://www.";
            break;
        case 0x02:
            decodedURL += "http://";
            break;
        case 0x03:
            decodedURL += "https://";
            break;
        default:
            decodedURL += m_eddystoneData.url[0];
    }

    for (int i = 1; i < lengthURL; i++) {
        if (m_eddystoneData.url[i] > 33 && m_eddystoneData.url[i] < 127) {
            decodedURL += m_eddystoneData.url[i];
        } else {
            switch (m_eddystoneData.url[i]) {
                case 0x00:
                    decodedURL += ".com/";
                    break;
                case 0x01:
                    decodedURL += ".org/";
                    break;
                case 0x02:
                    decodedURL += ".edu/";
                    break;
                case 0x03:
                    decodedURL += ".net/";
                    break;
                case 0x04:
                    decodedURL += ".info/";
                    break;
                case 0x05:
                    decodedURL += ".biz/";
                    break;
                case 0x06:
                    decodedURL += ".gov/";
                    break;
                case 0x07:
                    decodedURL += ".com";
                    break;
                case 0x08:
                    decodedURL += ".org";
                    break;
                case 0x09:
                    decodedURL += ".edu";
                    break;
                case 0x0A:
                    decodedURL += ".net";
                    break;
                case 0x0B:
                    decodedURL += ".info";
                    break;
                case 0x0C:
                    decodedURL += ".biz";
                    break;
                case 0x0D:
                    decodedURL += ".gov";
                    break;
                default:
                    break;
            }
        }
    }
    return decodedURL;
} // getDecodedURL



/**
 * @brief Set the raw data for the beacon advertisment.
 * @param [in] data The raw data to advertise.
 */
void NimBLEEddystoneURL::setData(const std::string &data) {
    if (data.length() > sizeof(m_eddystoneData)) {
        NIMBLE_LOGE(LOG_TAG, "Unable to set the data ... length passed in was %d and max expected %d",
                                                    data.length(), sizeof(m_eddystoneData));
        return;
    }
    memset(&m_eddystoneData, 0, sizeof(m_eddystoneData));
    memcpy(&m_eddystoneData, data.data(), data.length());
    lengthURL = data.length() - (sizeof(m_eddystoneData) - sizeof(m_eddystoneData.url));
} // setData


/**
 * @brief Set the UUID to advertise.
 * @param [in] l_uuid The UUID.
 */
void NimBLEEddystoneURL::setUUID(const NimBLEUUID &l_uuid) {
    beaconUUID = l_uuid.getNative()->u16.value;
} // setUUID


/**
 * @brief Set the transmit power to advertise.
 * @param [in] advertisedTxPower The transmit power level.
 */
void NimBLEEddystoneURL::setPower(int8_t advertisedTxPower) {
    m_eddystoneData.advertisedTxPower = advertisedTxPower;
} // setPower


/**
 * @brief Set the URL to advertise.
 * @param [in] url The URL.
 */
void NimBLEEddystoneURL::setURL(const std::string &url) {
  if (url.length() > sizeof(m_eddystoneData.url)) {
    NIMBLE_LOGE(LOG_TAG, "Unable to set the url ... length passed in was %d and max expected %d",
                                                    url.length(), sizeof(m_eddystoneData.url));
    return;
  }
  memset(m_eddystoneData.url, 0, sizeof(m_eddystoneData.url));
  memcpy(m_eddystoneData.url, url.data(), url.length());
  lengthURL = url.length();
} // setURL


#endif
