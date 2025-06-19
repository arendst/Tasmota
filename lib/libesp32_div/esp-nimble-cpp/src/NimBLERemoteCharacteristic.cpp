/*
 * Copyright 2020-2025 Ryan Powell <ryan@nable-embedded.io> and
 * esp-nimble-cpp, NimBLE-Arduino contributors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "NimBLERemoteCharacteristic.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_CENTRAL

# include "NimBLERemoteDescriptor.h"
# include "NimBLERemoteService.h"
# include "NimBLEClient.h"
# include "NimBLEUtils.h"
# include "NimBLELog.h"

# include <climits>

struct NimBLEDescriptorFilter {
    NimBLERemoteDescriptor* dsc;
    const NimBLEUUID*       uuid;
    void*                   taskData;
};

static const char* LOG_TAG = "NimBLERemoteCharacteristic";

/**
 * @brief Constructor.
 * @param [in] svc A pointer to the service this characteristic belongs to.
 * @param [in] chr struct defined as:
 *  struct ble_gatt_chr {
 *      uint16_t def_handle;
 *      uint16_t val_handle;
 *      uint8_t properties;
 *      ble_uuid_any_t uuid;
 *  };
 */
NimBLERemoteCharacteristic::NimBLERemoteCharacteristic(const NimBLERemoteService* svc, const ble_gatt_chr* chr)
    : NimBLERemoteValueAttribute{chr->uuid, chr->val_handle},
      m_pRemoteService{svc},
      m_properties{chr->properties},
      m_notifyCallback{},
      m_vDescriptors{} {} // NimBLERemoteCharacteristic

/**
 *@brief Destructor.
 */
NimBLERemoteCharacteristic::~NimBLERemoteCharacteristic() {
    deleteDescriptors();
} // ~NimBLERemoteCharacteristic

/**
 * @brief Callback used by the API when a descriptor is discovered or search complete.
 */
int NimBLERemoteCharacteristic::descriptorDiscCB(
    uint16_t connHandle, const ble_gatt_error* error, uint16_t chrHandle, const ble_gatt_dsc* dsc, void* arg) {
    int        rc        = error->status;
    auto       filter    = (NimBLEDescriptorFilter*)arg;
    auto       pTaskData = (NimBLETaskData*)filter->taskData;
    const auto pChr      = (NimBLERemoteCharacteristic*)pTaskData->m_pInstance;
    const auto uuid      = filter->uuid; // UUID to filter for
    NIMBLE_LOGD(LOG_TAG, "Descriptor Discovery >> status: %d handle: %d", rc, (rc == 0) ? dsc->handle : -1);

    // Results for chrHandle added until rc != 0
    // Must find specified UUID if filter is used
    if (rc == 0 && pChr->getHandle() == chrHandle && (!uuid || 0 == ble_uuid_cmp(uuid->getBase(), &dsc->uuid.u))) {
        // Return BLE_HS_EDONE if the descriptor was found, stop the search
        pChr->m_vDescriptors.push_back(new NimBLERemoteDescriptor(pChr, dsc));
        rc = !!uuid * BLE_HS_EDONE;
    }

    if (rc != 0) {
        NimBLEUtils::taskRelease(*pTaskData, rc);
        NIMBLE_LOGD(LOG_TAG, "<< Descriptor Discovery");
    }
    return rc;
}

/**
 * @brief Populate the descriptors (if any) for this characteristic.
 * @param [in] pFilter Pointer to a filter containing pointers to descriptor, UUID, and task data.
 * @return True if successfully retrieved, success = BLE_HS_EDONE.
 */
bool NimBLERemoteCharacteristic::retrieveDescriptors(NimBLEDescriptorFilter* pFilter) const {
    NIMBLE_LOGD(LOG_TAG, ">> retrieveDescriptors() for characteristic: %s", getUUID().toString().c_str());

    // If this is the last handle then there are no descriptors
    if (getHandle() == getRemoteService()->getEndHandle()) {
        NIMBLE_LOGD(LOG_TAG, "<< retrieveDescriptors(): found 0 descriptors.");
        return true;
    }

    NimBLETaskData         taskData(const_cast<NimBLERemoteCharacteristic*>(this));
    NimBLEDescriptorFilter defaultFilter{nullptr, nullptr, &taskData};
    if (pFilter == nullptr) {
        pFilter = &defaultFilter;
    }

    int rc = ble_gattc_disc_all_dscs(getClient()->getConnHandle(),
                                     getHandle(),
                                     getRemoteService()->getEndHandle(),
                                     NimBLERemoteCharacteristic::descriptorDiscCB,
                                     pFilter);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gattc_disc_all_dscs: rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    auto prevDscCount = m_vDescriptors.size();
    NimBLEUtils::taskWait(taskData, BLE_NPL_TIME_FOREVER);
    rc = ((NimBLETaskData*)pFilter->taskData)->m_flags;
    if (rc != BLE_HS_EDONE) {
        NIMBLE_LOGE(LOG_TAG, "<< retrieveDescriptors(): failed: rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    if (m_vDescriptors.size() > prevDscCount) {
        pFilter->dsc = m_vDescriptors.back();
    }

    NIMBLE_LOGD(LOG_TAG, "<< retrieveDescriptors(): found %d descriptors.", m_vDescriptors.size() - prevDscCount);
    return true;
} // retrieveDescriptors

/**
 * @brief Get the descriptor instance with the given UUID that belongs to this characteristic.
 * @param [in] uuid The UUID of the descriptor to find.
 * @return The Remote descriptor (if present) or nullptr if not present.
 */
NimBLERemoteDescriptor* NimBLERemoteCharacteristic::getDescriptor(const NimBLEUUID& uuid) const {
    NIMBLE_LOGD(LOG_TAG, ">> getDescriptor: uuid: %s", uuid.toString().c_str());
    NimBLEUUID             uuidTmp{uuid};
    NimBLETaskData         taskData(const_cast<NimBLERemoteCharacteristic*>(this));
    NimBLEDescriptorFilter filter{nullptr, &uuidTmp, &taskData};

    for (const auto& dsc : m_vDescriptors) {
        if (dsc->getUUID() == uuid) {
            filter.dsc = dsc;
            goto Done;
        }
    }

    if (!retrieveDescriptors(&filter) || filter.dsc) {
        goto Done;
    }

    // Try again with 128 bit uuid if request succeeded but no descriptor found.
    if (uuid.bitSize() != BLE_UUID_TYPE_128) {
        uuidTmp.to128();
        retrieveDescriptors(&filter);
        goto Done;
    }

    // If the uuid was 128 bit, try again with 16 bit uuid.
    uuidTmp.to16();
    if (uuidTmp.bitSize() == BLE_UUID_TYPE_16) {
        filter.uuid = &uuidTmp;
        retrieveDescriptors(&filter);
    }

Done:
    NIMBLE_LOGD(LOG_TAG, "<< getDescriptor: %sfound", filter.dsc ? "" : "not ");
    return filter.dsc;
} // getDescriptor

/**
 * @brief Get a pointer to the vector of found descriptors.
 * @param [in] refresh If true the current descriptor vector will be cleared and\n
 * all descriptors for this characteristic retrieved from the peripheral.\n
 * If false the vector will be returned with the currently stored descriptors
 * of this characteristic.
 * @return A pointer to the vector of descriptors for this characteristic.
 */
const std::vector<NimBLERemoteDescriptor*>& NimBLERemoteCharacteristic::getDescriptors(bool refresh) const {
    if (refresh) {
        deleteDescriptors();
        retrieveDescriptors();
    }

    return m_vDescriptors;
} // getDescriptors

/**
 * @brief Get iterator to the beginning of the vector of remote descriptor pointers.
 * @return An iterator to the beginning of the vector of remote descriptor pointers.
 */
std::vector<NimBLERemoteDescriptor*>::iterator NimBLERemoteCharacteristic::begin() const {
    return m_vDescriptors.begin();
}

/**
 * @brief Get iterator to the end of the vector of remote descriptor pointers.
 * @return An iterator to the end of the vector of remote descriptor pointers.
 */
std::vector<NimBLERemoteDescriptor*>::iterator NimBLERemoteCharacteristic::end() const {
    return m_vDescriptors.end();
}

/**
 * @brief Get the remote service associated with this characteristic.
 * @return The remote service associated with this characteristic.
 */
const NimBLERemoteService* NimBLERemoteCharacteristic::getRemoteService() const {
    return m_pRemoteService;
} // getRemoteService

/**
 * @brief Subscribe or unsubscribe for notifications or indications.
 * @param [in] val 0x00 to unsubscribe, 0x01 for notifications, 0x02 for indications.
 * @param [in] notifyCallback A callback to be invoked for a notification.
 * @param [in] response If write response required set this to true.
 * If NULL is provided then no callback is performed.
 * @return false if writing to the descriptor failed.
 */
bool NimBLERemoteCharacteristic::setNotify(uint16_t val, notify_callback notifyCallback, bool response) const {
    NIMBLE_LOGD(LOG_TAG, ">> setNotify()");

    m_notifyCallback             = notifyCallback;
    NimBLERemoteDescriptor* desc = getDescriptor(NimBLEUUID((uint16_t)0x2902));
    if (desc == nullptr) {
        NIMBLE_LOGW(LOG_TAG, "<< setNotify(): Callback set, CCCD not found");
        return true;
    }

    NIMBLE_LOGD(LOG_TAG, "<< setNotify()");
    return desc->writeValue(reinterpret_cast<uint8_t*>(&val), 2, response);
} // setNotify

/**
 * @brief Subscribe for notifications or indications.
 * @param [in] notifications If true, subscribe for notifications, false subscribe for indications.
 * @param [in] notifyCallback A callback to be invoked for a notification.
 * @param [in] response If true, require a write response from the descriptor write operation.
 * If NULL is provided then no callback is performed.
 * @return false if writing to the descriptor failed.
 */
bool NimBLERemoteCharacteristic::subscribe(bool notifications, notify_callback notifyCallback, bool response) const {
    return setNotify(notifications ? 0x01 : 0x02, notifyCallback, response);
} // subscribe

/**
 * @brief Unsubscribe for notifications or indications.
 * @param [in] response bool if true, require a write response from the descriptor write operation.
 * @return false if writing to the descriptor failed.
 */
bool NimBLERemoteCharacteristic::unsubscribe(bool response) const {
    return setNotify(0x00, nullptr, response);
} // unsubscribe

/**
 * @brief Delete the descriptors in the descriptor vector.
 * @details We maintain a vector called m_vDescriptors that contains pointers to NimBLERemoteDescriptors
 * object references. Since we allocated these in this class, we are also responsible for deleting
 * them. This method does just that.
 */
void NimBLERemoteCharacteristic::deleteDescriptors() const {
    NIMBLE_LOGD(LOG_TAG, ">> deleteDescriptors");

    for (const auto& it : m_vDescriptors) {
        delete it;
    }
    std::vector<NimBLERemoteDescriptor*>().swap(m_vDescriptors);

    NIMBLE_LOGD(LOG_TAG, "<< deleteDescriptors");
} // deleteDescriptors

/**
 * @brief Delete descriptor by UUID
 * @param [in] uuid The UUID of the descriptor to be deleted.
 * @return Number of descriptors left in the vector.
 */
size_t NimBLERemoteCharacteristic::deleteDescriptor(const NimBLEUUID& uuid) const {
    NIMBLE_LOGD(LOG_TAG, ">> deleteDescriptor");

    for (auto it = m_vDescriptors.begin(); it != m_vDescriptors.end(); ++it) {
        if ((*it)->getUUID() == uuid) {
            delete (*it);
            m_vDescriptors.erase(it);
            break;
        }
    }

    NIMBLE_LOGD(LOG_TAG, "<< deleteDescriptor");
    return m_vDescriptors.size();
} // deleteDescriptor

/**
 * @brief Does the characteristic support value broadcasting?
 * @return True if supported.
 */
bool NimBLERemoteCharacteristic::canBroadcast() const {
    return (m_properties & BLE_GATT_CHR_PROP_BROADCAST);
};

/**
 * @brief Does the characteristic support reading?
 * @return True if supported.
 */
bool NimBLERemoteCharacteristic::canRead() const {
    return (m_properties & BLE_GATT_CHR_PROP_READ);
};

/**
 * @brief Does the characteristic support writing without a response?
 * @return True if supported.
 */
bool NimBLERemoteCharacteristic::canWriteNoResponse() const {
    return (m_properties & BLE_GATT_CHR_PROP_WRITE_NO_RSP);
};

/**
 * @brief Does the characteristic support writing?
 * @return True if supported.
 */
bool NimBLERemoteCharacteristic::canWrite() const {
    return (m_properties & BLE_GATT_CHR_PROP_WRITE);
};

/**
 * @brief Does the characteristic support reading with encryption?
 * @return True if supported.
 */
bool NimBLERemoteCharacteristic::canNotify() const {
    return (m_properties & BLE_GATT_CHR_PROP_NOTIFY);
};

/**
 * @brief Does the characteristic support indication?
 * @return True if supported.
 */
bool NimBLERemoteCharacteristic::canIndicate() const {
    return (m_properties & BLE_GATT_CHR_PROP_INDICATE);
};

/**
 * @brief Does the characteristic support signed writing?
 * @return True if supported.
 */
bool NimBLERemoteCharacteristic::canWriteSigned() const {
    return (m_properties & BLE_GATT_CHR_PROP_AUTH_SIGN_WRITE);
};

/**
 * @brief Does the characteristic support extended properties?
 * @return True if supported.
 */
bool NimBLERemoteCharacteristic::hasExtendedProps() const {
    return (m_properties & BLE_GATT_CHR_PROP_EXTENDED);
};

/**
 * @brief Convert a NimBLERemoteCharacteristic to a string representation;
 * @return a String representation.
 */
std::string NimBLERemoteCharacteristic::toString() const {
    std::string res = "Characteristic: uuid: " + m_uuid.toString();
    char        val[6];
    res += ", handle: ";
    snprintf(val, sizeof(val), "%d", getHandle());
    res += val;
    res += " 0x";
    snprintf(val, sizeof(val), "%04x", getHandle());
    res += val;
    res += ", props: ";
    res += " 0x";
    snprintf(val, sizeof(val), "%02x", m_properties);
    res += val;

    for (const auto& it : m_vDescriptors) {
        res += "\n" + it->toString();
    }

    return res;
} // toString

NimBLEClient* NimBLERemoteCharacteristic::getClient() const {
    return getRemoteService()->getClient();
} // getClient

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_CENTRAL
