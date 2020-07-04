/*
 * NimBLEValue.h
 *
 *  Created: on March 6, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEValue.h
 *
 *  Created on: Jul 17, 2017
 *      Author: kolban
 */

#ifndef MAIN_BLEVALUE_H_
#define MAIN_BLEVALUE_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include <string>

/**
 * @brief The model of a %BLE value.
 */
class NimBLEValue {
public:
    NimBLEValue();
    void        addPart(const std::string &part);
    void        addPart(const uint8_t* pData, size_t length);
    void        cancel();
    void        commit();
    uint8_t*    getData();
    size_t      getLength();
    uint16_t    getReadOffset();
    std::string getValue();
    void        setReadOffset(uint16_t readOffset);
    void        setValue(const std::string &value);
    void        setValue(const uint8_t* pData, size_t length);

private:
    std::string m_accumulation;
    uint16_t    m_readOffset;
    std::string m_value;

};

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif // CONFIG_BT_ENABLED
#endif /* MAIN_BLEVALUE_H_ */
