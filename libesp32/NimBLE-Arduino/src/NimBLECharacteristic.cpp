/*
 * NimBLECharacteristic.cpp
 *
 *  Created: on March 3, 2020
 *      Author H2zero
 * 
 * BLECharacteristic.cpp
 *
 *  Created on: Jun 22, 2017
 *      Author: kolban
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLECharacteristic.h"
#include "NimBLE2902.h"
#include "NimBLE2904.h"
#include "NimBLEDevice.h"
#include "NimBLEUtils.h"
#include "NimBLELog.h"

#include <string>

#define NULL_HANDLE (0xffff)

static NimBLECharacteristicCallbacks defaultCallback;

static const char* LOG_TAG = "NimBLECharacteristic";

/**
 * @brief Construct a characteristic
 * @param [in] uuid - UUID (const char*) for the characteristic.
 * @param [in] properties - Properties for the characteristic.
 */
NimBLECharacteristic::NimBLECharacteristic(const char* uuid, uint16_t properties, NimBLEService* pService) 
: NimBLECharacteristic(NimBLEUUID(uuid), properties, pService) {
}

/**
 * @brief Construct a characteristic
 * @param [in] uuid - UUID for the characteristic.
 * @param [in] properties - Properties for the characteristic.
 */
NimBLECharacteristic::NimBLECharacteristic(NimBLEUUID uuid, uint16_t properties, NimBLEService* pService) {
	m_uuid       = uuid;
	m_handle     = NULL_HANDLE;
	m_properties = properties;
	m_pCallbacks = &defaultCallback;
    m_pService   = pService;
//  Backward Compatibility - to be removed
/*	setBroadcastProperty((properties & PROPERTY_BROADCAST) != 0);
	setReadProperty((properties & PROPERTY_READ) != 0);
	setWriteProperty((properties & PROPERTY_WRITE) != 0);
	setNotifyProperty((properties & PROPERTY_NOTIFY) != 0);
	setIndicateProperty((properties & PROPERTY_INDICATE) != 0);
	setWriteNoResponseProperty((properties & PROPERTY_WRITE_NR) != 0);
*/
///////////////////////////////////////////
} // NimBLECharacteristic

/**
 * @brief Destructor.
 */
NimBLECharacteristic::~NimBLECharacteristic() {
} // ~NimBLECharacteristic


/**
 * @brief Associate a descriptor with this characteristic.
 * @param [in] pDescriptor
 * @return N/A.
 */
void NimBLECharacteristic::addDescriptor(NimBLEDescriptor* pDescriptor) {
	NIMBLE_LOGD(LOG_TAG, ">> addDescriptor(): Adding %s to %s", pDescriptor->toString().c_str(), toString().c_str());
	// Check that we don't add the same descriptor twice.
	if (m_descriptorMap.getByUUID(pDescriptor->getUUID()) != nullptr) {
		NIMBLE_LOGW(LOG_TAG, "<< Adding a new descriptor with the same UUID as a previous one");
		//return;
	}
	m_descriptorMap.setByUUID(pDescriptor->getUUID(), pDescriptor);
	NIMBLE_LOGD(LOG_TAG, "<< addDescriptor()");
} // addDescriptor


/**
 * @brief Create a new BLE Descriptor associated with this characteristic.
 * @param [in] uuid - The UUID of the descriptor.
 * @param [in] properties - The properties of the descriptor.
 * @return The new BLE descriptor.
 */
NimBLEDescriptor* NimBLECharacteristic::createDescriptor(const char* uuid, uint32_t properties, uint16_t max_len) {
	return createDescriptor(NimBLEUUID(uuid), properties, max_len);
}


/**
 * @brief Create a new BLE Descriptor associated with this characteristic.
 * @param [in] uuid - The UUID of the descriptor.
 * @param [in] properties - The properties of the descriptor.
 * @return The new BLE descriptor.
 */
NimBLEDescriptor* NimBLECharacteristic::createDescriptor(NimBLEUUID uuid, uint32_t properties, uint16_t max_len) {
	NimBLEDescriptor* pDescriptor = nullptr;
	if(uuid.equals(NimBLEUUID((uint16_t)0x2902))) {
        if(!(m_properties & BLE_GATT_CHR_F_NOTIFY) && !(m_properties & BLE_GATT_CHR_F_INDICATE)) {
            assert(0 && "Cannot create 2902 descriptior without characteristic notification or indication property set");
        }
        // We cannot have more than one 2902 descriptor, if it's already been created just return a pointer to it.
        pDescriptor = m_descriptorMap.getByUUID(uuid);
        if(pDescriptor == nullptr) {
            pDescriptor = new NimBLE2902(this);
        } else {
            return pDescriptor;
        }
		
	} else if (uuid.equals(NimBLEUUID((uint16_t)0x2904))) {
		pDescriptor = new NimBLE2904(this);
		
	} else {
		pDescriptor = new NimBLEDescriptor(uuid, properties, max_len, this);
	}
	addDescriptor(pDescriptor);
	return pDescriptor;
} // createCharacteristic


/**
 * @brief Return the BLE Descriptor for the given UUID if associated with this characteristic.
 * @param [in] descriptorUUID The UUID of the descriptor that we wish to retrieve.
 * @return The BLE Descriptor.  If no such descriptor is associated with the characteristic, nullptr is returned.
 */
NimBLEDescriptor* NimBLECharacteristic::getDescriptorByUUID(const char* descriptorUUID) {
	return m_descriptorMap.getByUUID(NimBLEUUID(descriptorUUID));
} // getDescriptorByUUID


/**
 * @brief Return the BLE Descriptor for the given UUID if associated with this characteristic.
 * @param [in] descriptorUUID The UUID of the descriptor that we wish to retrieve.
 * @return The BLE Descriptor.  If no such descriptor is associated with the characteristic, nullptr is returned.
 */
NimBLEDescriptor* NimBLECharacteristic::getDescriptorByUUID(NimBLEUUID descriptorUUID) {
	return m_descriptorMap.getByUUID(descriptorUUID);
} // getDescriptorByUUID


/**
 * @brief Get the handle of the characteristic.
 * @return The handle of the characteristic.
 */
uint16_t NimBLECharacteristic::getHandle() {
	return m_handle;
} // getHandle

/*
void NimBLECharacteristic::setAccessPermissions(uint16_t perm) {
	m_permissions = perm;
}
*/

uint8_t NimBLECharacteristic::getProperties() {
	return m_properties;
} // getProperties


/**
 * @brief Get the service associated with this characteristic.
 */
NimBLEService* NimBLECharacteristic::getService() {
	return m_pService;
} // getService


/**
 * @brief Get the UUID of the characteristic.
 * @return The UUID of the characteristic.
 */
NimBLEUUID NimBLECharacteristic::getUUID() {
	return m_uuid;
} // getUUID


/**
 * @brief Retrieve the current value of the characteristic.
 * @return A pointer to storage containing the current characteristic value.
 */
std::string NimBLECharacteristic::getValue() {
	return m_value.getValue();
} // getValue


/**
 * @brief Retrieve the current raw data of the characteristic.
 * @return A pointer to storage containing the current characteristic data.
 */
uint8_t* NimBLECharacteristic::getData() {
	return m_value.getData();
} // getData


int NimBLECharacteristic::handleGapEvent(uint16_t conn_handle, uint16_t attr_handle,
                             struct ble_gatt_access_ctxt *ctxt,
                             void *arg)
{
	const ble_uuid_t *uuid;
    int rc;
	NimBLECharacteristic* pCharacteristic = (NimBLECharacteristic*)arg;
	
	NIMBLE_LOGD(LOG_TAG, "Characteristic %s %s event", pCharacteristic->getUUID().toString().c_str(),
                                    ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR ? "Read" : "Write");
                                    
	uuid = ctxt->chr->uuid;
	if(ble_uuid_cmp(uuid, &pCharacteristic->getUUID().getNative()->u) == 0){
        switch(ctxt->op) {
            case BLE_GATT_ACCESS_OP_READ_CHR: {
                //NIMBLE_LOGD(LOG_TAG, "read char pkthdr len:%d flags:%d", ctxt->om->om_pkthdr_len, ctxt->om->om_flags);
				// If the packet header is only 8 bytes this is a follow up of a long read
				// so we don't want to call the onRead() callback again.
				if(ctxt->om->om_pkthdr_len > 8) {
					pCharacteristic->m_pCallbacks->onRead(pCharacteristic);
				}
                rc = os_mbuf_append(ctxt->om, pCharacteristic->getData(), pCharacteristic->m_value.getLength());
                return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
            }
            
            case BLE_GATT_ACCESS_OP_WRITE_CHR: {
                //NIMBLE_LOGD(LOG_TAG, "write char pkthdr len:%d datalen:%d", ctxt->om->om_pkthdr_len, ctxt->om->om_len);
                if (ctxt->om->om_len > BLE_ATT_ATTR_MAX_LEN) {
                    return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
                }
                
                //pCharacteristic->setValue(ctxt->om->om_data, ctxt->om->om_len);
				pCharacteristic->m_value.addPart(ctxt->om->om_data, ctxt->om->om_len);
				os_mbuf *next;
				next = SLIST_NEXT(ctxt->om, om_next);
				while(next != NULL){
					//NIMBLE_LOGD(LOG_TAG, "Found long write data, len:%d", next->om_len);
					pCharacteristic->m_value.addPart(next->om_data, next->om_len);
					next = SLIST_NEXT(next, om_next);
				}
				pCharacteristic->m_value.commit();
                pCharacteristic->m_pCallbacks->onWrite(pCharacteristic);

                return 0;
            }
            default:
                break;
        }
	}
    
    return BLE_ATT_ERR_UNLIKELY;
}
							

/**
 * @brief Set the subscribe status for this characteristic.
 * This will maintain a map of subscribed clients and their indicate/notify status.
 * @return N/A
 */
void NimBLECharacteristic::setSubscribe(struct ble_gap_event *event) {
    uint16_t subVal = 0;
    if(event->subscribe.cur_notify) {
        subVal |= NIMBLE_DESC_FLAG_NOTIFY;
    }
    if(event->subscribe.cur_indicate) {
        subVal |= NIMBLE_DESC_FLAG_INDICATE;
    }
    
    m_semaphoreConfEvt.give((subVal | NIMBLE_DESC_FLAG_INDICATE) ? 0 :
            NimBLECharacteristicCallbacks::Status::ERROR_INDICATE_DISABLED);
    
    NIMBLE_LOGI(LOG_TAG, "New subscribe value for conn: %d val: %d", event->subscribe.conn_handle, subVal);
    
    NimBLE2902* p2902 = (NimBLE2902*)getDescriptorByUUID((uint16_t)0x2902);
	if(p2902 == nullptr){
		ESP_LOGE(LOG_TAG, "No 2902 descriptor found for %s", getUUID().toString().c_str());
        return;
	}
    
    p2902->setNotifications(subVal & NIMBLE_DESC_FLAG_NOTIFY);
    p2902->setIndications(subVal & NIMBLE_DESC_FLAG_INDICATE);
    p2902->m_pCallbacks->onWrite(p2902);
    
    
    auto it = p2902->m_subscribedMap.find(event->subscribe.conn_handle);
    if(subVal > 0 && it == p2902->m_subscribedMap.cend()) {
        p2902->m_subscribedMap.insert(std::pair<uint16_t, uint16_t>(event->subscribe.conn_handle, subVal));
        return;
    } else if(it != p2902->m_subscribedMap.cend()) {
        p2902->m_subscribedMap.erase(event->subscribe.conn_handle);
        return;
    }
/*    
    if(event->subscribe.reason == BLE_GAP_SUBSCRIBE_REASON_TERM) {
        p2902->m_subscribedMap.erase(event->subscribe.conn_handle);
        return;
    }
*/    
    (*it).second = subVal;
}


/**
 * @brief Send an indication.
 * An indication is a transmission of up to the first 20 bytes of the characteristic value.  An indication
 * will block waiting a positive confirmation from the client.
 * @return N/A
 */
void NimBLECharacteristic::indicate() {
	NIMBLE_LOGD(LOG_TAG, ">> indicate: length: %d", m_value.getValue().length());
	notify(false);
	NIMBLE_LOGD(LOG_TAG, "<< indicate");
} // indicate
    
/**
 * @brief Send a notify.
 * A notification is a transmission of up to the first 20 bytes of the characteristic value.  An notification
 * will not block; it is a fire and forget.
 * @return N/A.
 */
void NimBLECharacteristic::notify(bool is_notification) {
	NIMBLE_LOGD(LOG_TAG, ">> notify: length: %d", m_value.getValue().length());

	assert(getService() != nullptr);
	assert(getService()->getServer() != nullptr);


    if (getService()->getServer()->getConnectedCount() == 0) {
		NIMBLE_LOGD(LOG_TAG, "<< notify: No connected clients.");
		return;
	}
    
    m_pCallbacks->onNotify(this);
    
    int rc = 0;
    NimBLE2902* p2902 = (NimBLE2902*)getDescriptorByUUID((uint16_t)0x2902);
    
    for (auto it = p2902->m_subscribedMap.cbegin(); it != p2902->m_subscribedMap.cend(); ++it) {
        uint16_t _mtu = getService()->getServer()->getPeerMTU((*it).first);
        // Must rebuild the data on each loop iteration as NimBLE will release it. 
        size_t length = m_value.getValue().length();
        uint8_t* data = (uint8_t*)m_value.getValue().data();
        os_mbuf *om;
        
        if(_mtu == 0) {
            //NIMBLE_LOGD(LOG_TAG, "peer not connected, removing from map");
            p2902->m_subscribedMap.erase((*it).first);
            it = p2902->m_subscribedMap.cbegin();
            if(it == p2902->m_subscribedMap.cend()) {
                return;
            }
            continue;
        }
        
        if (length > _mtu - 3) {
			NIMBLE_LOGW(LOG_TAG, "- Truncating to %d bytes (maximum notify size)", _mtu - 3);
		}
        
        if((*it).second == 0) {
            //NIMBLE_LOGI(LOG_TAG, "Skipping unsubscribed client");
            continue;
        }
        
        if(is_notification && (!((*it).second & NIMBLE_DESC_FLAG_NOTIFY))) {
            NIMBLE_LOGW(LOG_TAG, 
            "Sending notification to client subscribed to indications, sending indication instead");
            is_notification = false;
        }
        
        if(!is_notification && (!((*it).second & NIMBLE_DESC_FLAG_INDICATE))) {
            NIMBLE_LOGW(LOG_TAG,
            "Sending indication to client subscribed to notifications, sending notifications instead");
            is_notification = true;
        }
        
        // don't create the m_buf until we are sure to send the data or else
        // we could be allocating a buffer that doesn't get released.
        // We also must create it in each loop iteration because it is consumed with each host call.
        om = ble_hs_mbuf_from_flat(data, length);
        
        if(!is_notification) {
            m_semaphoreConfEvt.take("indicate");
            rc = ble_gattc_indicate_custom((*it).first, m_handle, om);
            if(rc != 0){
                m_semaphoreConfEvt.give();
                m_pCallbacks->onStatus(this, NimBLECharacteristicCallbacks::Status::ERROR_GATT, rc);
                return;
            }
            
            rc = m_semaphoreConfEvt.wait();
            
            if(rc == BLE_HS_ETIMEOUT) {
                m_pCallbacks->onStatus(this, NimBLECharacteristicCallbacks::Status::ERROR_INDICATE_TIMEOUT, rc);
            } else if(rc == BLE_HS_EDONE) {
                m_pCallbacks->onStatus(this, NimBLECharacteristicCallbacks::Status::SUCCESS_INDICATE, rc);
            } else {
                m_pCallbacks->onStatus(this, NimBLECharacteristicCallbacks::Status::ERROR_INDICATE_FAILURE, rc);
            }
        } else {
            rc = ble_gattc_notify_custom((*it).first, m_handle, om);
            if(rc == 0) {
                m_pCallbacks->onStatus(this, NimBLECharacteristicCallbacks::Status::SUCCESS_NOTIFY, 0);
            } else {
                m_pCallbacks->onStatus(this, NimBLECharacteristicCallbacks::Status::ERROR_GATT, rc); 
            }
        }
	}

	NIMBLE_LOGD(LOG_TAG, "<< notify");
} // Notify


/**
 * @brief Set the callback handlers for this characteristic.
 * @param [in] pCallbacks An instance of a callbacks structure used to define any callbacks for the characteristic.
 */
void NimBLECharacteristic::setCallbacks(NimBLECharacteristicCallbacks* pCallbacks) {
	if (pCallbacks != nullptr){
		m_pCallbacks = pCallbacks;
	} else {
		m_pCallbacks = &defaultCallback;
	}
} // setCallbacks

// Backward compatibility - to be removed ////////////////////////////////
/**
 * @brief Set the permission to broadcast.
 * A characteristics has properties associated with it which define what it is capable of doing.
 * One of these is the broadcast flag.
 * @param [in] value The flag value of the property.
 * @return N/A
 */
void NimBLECharacteristic::setBroadcastProperty(bool value) {
	if (value) {
		m_properties = (m_properties | BLE_GATT_CHR_F_BROADCAST);
	} else {
		m_properties = (m_properties & ~BLE_GATT_CHR_F_BROADCAST);
	}
} // setBroadcastProperty


/**
 * @brief Set the Indicate property value.
 * @param [in] value Set to true if we are to allow indicate messages.
 */
void NimBLECharacteristic::setIndicateProperty(bool value) {
	if (value) {
		m_properties = (m_properties | BLE_GATT_CHR_F_INDICATE);
	} else {
		m_properties = (m_properties & ~BLE_GATT_CHR_F_INDICATE);
	}
} // setIndicateProperty


/**
 * @brief Set the Notify property value.
 * @param [in] value Set to true if we are to allow notification messages.
 */
void NimBLECharacteristic::setNotifyProperty(bool value) {
	if (value) {
		m_properties = (m_properties | BLE_GATT_CHR_F_NOTIFY);
	} else {
		m_properties = (m_properties & ~BLE_GATT_CHR_F_NOTIFY);
	}
} // setNotifyProperty


/**
 * @brief Set the Read property value.
 * @param [in] value Set to true if we are to allow reads.
 */
void NimBLECharacteristic::setReadProperty(bool value) {
	if (value) {
		m_properties = (m_properties | BLE_GATT_CHR_F_READ);
	} else {
		m_properties = (m_properties & ~BLE_GATT_CHR_F_READ);
	}
} // setReadProperty


/**
 * @brief Set the Write No Response property value.
 * @param [in] value Set to true if we are to allow writes with no response.
 */
void NimBLECharacteristic::setWriteNoResponseProperty(bool value) {
	if (value) {
		m_properties = (m_properties | BLE_GATT_CHR_F_WRITE_NO_RSP);
	} else {
		m_properties = (m_properties & ~BLE_GATT_CHR_F_WRITE_NO_RSP);
	}
} // setWriteNoResponseProperty


/**
 * @brief Set the Write property value.
 * @param [in] value Set to true if we are to allow writes.
 */
void NimBLECharacteristic::setWriteProperty(bool value) {
	if (value) {
		m_properties = (m_properties | BLE_GATT_CHR_F_WRITE );
	} else {
		m_properties = (m_properties & ~BLE_GATT_CHR_F_WRITE );
	}
} // setWriteProperty
//////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Set the value of the characteristic.
 * @param [in] data The data to set for the characteristic.
 * @param [in] length The length of the data in bytes.
 */
void NimBLECharacteristic::setValue(uint8_t* data, size_t length) {
	char* pHex = NimBLEUtils::buildHexData(nullptr, data, length);
	NIMBLE_LOGD(LOG_TAG, ">> setValue: length=%d, data=%s, characteristic UUID=%s", length, pHex, getUUID().toString().c_str());
	free(pHex);

	if (length > BLE_ATT_ATTR_MAX_LEN) {
		NIMBLE_LOGE(LOG_TAG, "Size %d too large, must be no bigger than %d", length, BLE_ATT_ATTR_MAX_LEN);
		return;
	}
    
	m_value.setValue(data, length);
    
  //  if(m_handle != NULL_HANDLE) {
        //ble_gatts_chr_updated(m_handle);
       // ble_gattc_notify(getService()->getServer()->m_connId, m_handle);
  //  }
    
	NIMBLE_LOGD(LOG_TAG, "<< setValue");
} // setValue


/**
 * @brief Set the value of the characteristic from string data.
 * We set the value of the characteristic from the bytes contained in the
 * string.
 * @param [in] Set the value of the characteristic.
 * @return N/A.
 */
void NimBLECharacteristic::setValue(std::string value) {
	setValue((uint8_t*)(value.data()), value.length());
} // setValue

void NimBLECharacteristic::setValue(uint16_t& data16) {
	uint8_t temp[2];
	temp[0] = data16;
	temp[1] = data16 >> 8;
	setValue(temp, 2);
} // setValue

void NimBLECharacteristic::setValue(uint32_t& data32) {
	uint8_t temp[4];
	temp[0] = data32;
	temp[1] = data32 >> 8;
	temp[2] = data32 >> 16;
	temp[3] = data32 >> 24;
	setValue(temp, 4);
} // setValue

void NimBLECharacteristic::setValue(int& data32) {
	uint8_t temp[4];
	temp[0] = data32;
	temp[1] = data32 >> 8;
	temp[2] = data32 >> 16;
	temp[3] = data32 >> 24;
	setValue(temp, 4);
} // setValue

void NimBLECharacteristic::setValue(float& data32) {
	float temp = data32;
	setValue((uint8_t*)&temp, 4);
} // setValue

void NimBLECharacteristic::setValue(double& data64) {
	double temp = data64;
	setValue((uint8_t*)&temp, 8);
} // setValue


/**
 * @brief Return a string representation of the characteristic.
 * @return A string representation of the characteristic.
 */
std::string NimBLECharacteristic::toString() {
	std::string res = "UUID: " + m_uuid.toString() + ", handle : 0x";
	char hex[5];
	snprintf(hex, sizeof(hex), "%04x", m_handle);
	res += hex;
	res += " ";
	if (m_properties & BLE_GATT_CHR_PROP_READ ) res += "Read ";
	if (m_properties & BLE_GATT_CHR_PROP_WRITE) res += "Write ";
	if (m_properties & BLE_GATT_CHR_PROP_WRITE_NO_RSP) res += "WriteNoResponse ";
	if (m_properties & BLE_GATT_CHR_PROP_BROADCAST) res += "Broadcast ";
	if (m_properties & BLE_GATT_CHR_PROP_NOTIFY) res += "Notify ";
	if (m_properties & BLE_GATT_CHR_PROP_INDICATE) res += "Indicate ";
	return res;
} // toString


NimBLECharacteristicCallbacks::~NimBLECharacteristicCallbacks() {}


/**
 * @brief Callback function to support a read request.
 * @param [in] pCharacteristic The characteristic that is the source of the event.
 */
void NimBLECharacteristicCallbacks::onRead(NimBLECharacteristic* pCharacteristic) {
	NIMBLE_LOGD("NimBLECharacteristicCallbacks", "onRead: default");
} // onRead


/**
 * @brief Callback function to support a write request.
 * @param [in] pCharacteristic The characteristic that is the source of the event.
 */
void NimBLECharacteristicCallbacks::onWrite(NimBLECharacteristic* pCharacteristic) {
	NIMBLE_LOGD("NimBLECharacteristicCallbacks", "onWrite: default");
} // onWrite


/**
 * @brief Callback function to support a Notify request.
 * @param [in] pCharacteristic The characteristic that is the source of the event.
 */
void NimBLECharacteristicCallbacks::onNotify(NimBLECharacteristic* pCharacteristic) {
	NIMBLE_LOGD("NimBLECharacteristicCallbacks", "onNotify: default");
} // onNotify


/**
 * @brief Callback function to support a Notify/Indicate Status report.
 * @param [in] pCharacteristic The characteristic that is the source of the event.
 * @param [in] s Status of the notification/indication
 * @param [in] code Additional code of underlying errors
 */
void NimBLECharacteristicCallbacks::onStatus(NimBLECharacteristic* pCharacteristic, Status s, int code) {
	NIMBLE_LOGD("NimBLECharacteristicCallbacks", "onStatus: default");
} // onStatus

#endif /* CONFIG_BT_ENABLED */