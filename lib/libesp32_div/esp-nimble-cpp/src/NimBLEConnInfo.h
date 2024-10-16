#ifndef NIMBLECONNINFO_H_
#define NIMBLECONNINFO_H_

#include "NimBLEAddress.h"

/**
 * @brief Connection information.
 */
class NimBLEConnInfo {
friend class NimBLEServer;
friend class NimBLEClient;
friend class NimBLECharacteristic;
friend class NimBLEDescriptor;

    ble_gap_conn_desc m_desc{};
    NimBLEConnInfo(){};
    NimBLEConnInfo(ble_gap_conn_desc desc)   { m_desc = desc; }
public:
    /** @brief Gets the over-the-air address of the connected peer */
    NimBLEAddress    getAddress() const      { return NimBLEAddress(m_desc.peer_ota_addr); }

    /** @brief Gets the ID address of the connected peer */
    NimBLEAddress    getIdAddress() const    { return NimBLEAddress(m_desc.peer_id_addr); }

    /** @brief Gets the connection handle (also known as the connection id) of the connected peer */
    uint16_t         getConnHandle() const   { return m_desc.conn_handle; }

    /** @brief Gets the connection interval for this connection (in 1.25ms units) */
    uint16_t         getConnInterval() const { return m_desc.conn_itvl; }

    /** @brief Gets the supervision timeout for this connection (in 10ms units) */
    uint16_t         getConnTimeout() const  { return m_desc.supervision_timeout; }

    /** @brief Gets the allowable latency for this connection (unit = number of intervals) */
    uint16_t         getConnLatency() const  { return m_desc.conn_latency; }

    /** @brief Gets the maximum transmission unit size for this connection (in bytes) */
    uint16_t         getMTU() const          { return ble_att_mtu(m_desc.conn_handle); }

    /** @brief Check if we are in the master role in this connection */
    bool             isMaster() const        { return (m_desc.role == BLE_GAP_ROLE_MASTER); }

    /** @brief Check if we are in the slave role in this connection */
    bool             isSlave() const         { return (m_desc.role == BLE_GAP_ROLE_SLAVE); }

    /** @brief Check if we are connected to a bonded peer */
    bool             isBonded() const        { return (m_desc.sec_state.bonded == 1); }

    /** @brief Check if the connection in encrypted */
    bool             isEncrypted() const     { return (m_desc.sec_state.encrypted == 1); }

    /** @brief Check if the the connection has been authenticated */
    bool             isAuthenticated() const { return (m_desc.sec_state.authenticated == 1); }

    /** @brief Gets the key size used to encrypt the connection */
    uint8_t          getSecKeySize() const   { return m_desc.sec_state.key_size; }
};
#endif
