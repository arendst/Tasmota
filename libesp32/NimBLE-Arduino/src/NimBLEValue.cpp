/*
 * NimNimBLEValue.cpp
 *
 *  Created: on March 6, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEValue.cpp
 *
 *  Created on: Jul 17, 2017
 *      Author: kolban
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLEValue.h"
#include "NimBLELog.h"

static const char* LOG_TAG="NimBLEValue";

NimBLEValue::NimBLEValue() {
    m_accumulation = "";
    m_value        = "";
    m_readOffset   = 0;
} // NimBLEValue


/**
 * @brief Add a message part to the accumulation.
 * The accumulation is a growing set of data that is added to until a commit or cancel.
 * @param [in] part A message part being added.
 */
void NimBLEValue::addPart(const std::string &part) {
    NIMBLE_LOGD(LOG_TAG, ">> addPart: length=%d", part.length());
    m_accumulation += part;
} // addPart


/**
 * @brief Add a message part to the accumulation.
 * The accumulation is a growing set of data that is added to until a commit or cancel.
 * @param [in] pData A message part being added.
 * @param [in] length The number of bytes being added.
 */
void NimBLEValue::addPart(const uint8_t* pData, size_t length) {
    NIMBLE_LOGD(LOG_TAG, ">> addPart: length=%d", length);
    m_accumulation += std::string((char*) pData, length);
} // addPart


/**
 * @brief Cancel the current accumulation.
 */
void NimBLEValue::cancel() {
    NIMBLE_LOGD(LOG_TAG, ">> cancel");
    m_accumulation = "";
    m_readOffset   = 0;
} // cancel


/**
 * @brief Commit the current accumulation.
 * When writing a value, we may find that we write it in "parts" meaning that the writes come in in pieces
 * of the overall message.  After the last part has been received, we may perform a commit which means that
 * we now have the complete message and commit the change as a unit.
 */
void NimBLEValue::commit() {
    NIMBLE_LOGD(LOG_TAG, ">> commit");
    // If there is nothing to commit, do nothing.
    if (m_accumulation.length() == 0) return;
    setValue(m_accumulation);
    m_accumulation = "";
    m_readOffset   = 0;
} // commit


/**
 * @brief Get a pointer to the data.
 * @return A pointer to the data.
 */
uint8_t* NimBLEValue::getData() {
    return (uint8_t*) m_value.data();
}


/**
 * @brief Get the length of the data in bytes.
 * @return The length of the data in bytes.
 */
size_t NimBLEValue::getLength() {
    return m_value.length();
} // getLength


/**
 * @brief Get the read offset.
 * @return The read offset into the read.
 */
uint16_t NimBLEValue::getReadOffset() {
    return m_readOffset;
} // getReadOffset


/**
 * @brief Get the current value.
 */
std::string NimBLEValue::getValue() {
    return m_value;
} // getValue


/**
 * @brief Set the read offset
 * @param [in] readOffset The offset into the read.
 */
void NimBLEValue::setReadOffset(uint16_t readOffset) {
    m_readOffset = readOffset;
} // setReadOffset


/**
 * @brief Set the current value.
 */
void NimBLEValue::setValue(const std::string &value) {
    m_value = value;
} // setValue


/**
 * @brief Set the current value.
 * @param [in] pData The data for the current value.
 * @param [in] The length of the new current value.
 */
void NimBLEValue::setValue(const uint8_t* pData, size_t length) {
    m_value = std::string((char*) pData, length);
} // setValue

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif // CONFIG_BT_ENABLED
