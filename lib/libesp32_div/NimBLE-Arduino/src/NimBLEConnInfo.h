#ifndef NIMBLECONNINFO_H_
#define NIMBLECONNINFO_H_

#include "NimBLEAddress.h"

/**
 * @brief Connection information.
 */
class NimBLEConnInfo {
friend class NimBLEServer;
friend class NimBLEClient;
    ble_gap_conn_desc m_desc;
    NimBLEConnInfo()                       { m_desc = {}; }
    NimBLEConnInfo(ble_gap_conn_desc desc) { m_desc = desc; }
public:
    /** @brief Gets the over-the-air address of the connected peer */
    NimBLEAddress    getAddress()          { return NimBLEAddress(m_desc.peer_ota_addr); }

    /** @brief Gets the ID address of the connected peer */
    NimBLEAddress    getIdAddress()        { return NimBLEAddress(m_desc.peer_id_addr); }

    /** @brief Gets the connection handle of the connected peer */
    uint16_t         getConnHandle()       { return m_desc.conn_handle; }

    /** @brief Gets the connection interval for this connection (in 1.25ms units) */
    uint16_t         getConnInterval()     { return m_desc.conn_itvl; }

    /** @brief Gets the supervision timeout for this connection (in 10ms units) */
    uint16_t         getConnTimeout()      { return m_desc.supervision_timeout; }

    /** @brief Gets the allowable latency for this connection (unit = number of intervals) */
    uint16_t         getConnLatency()      { return m_desc.conn_latency; }

    /** @brief Gets the maximum transmission unit size for this connection (in bytes) */
    uint16_t         getMTU()              { return ble_att_mtu(m_desc.conn_handle); }

    /** @brief Check if we are in the master role in this connection */
    bool             isMaster()            { return (m_desc.role == BLE_GAP_ROLE_MASTER); }

    /** @brief Check if we are in the slave role in this connection */
    bool             isSlave()             { return (m_desc.role == BLE_GAP_ROLE_SLAVE); }

    /** @brief Check if we are connected to a bonded peer */
    bool             isBonded()            { return (m_desc.sec_state.bonded == 1); }

    /** @brief Check if the connection in encrypted */
    bool             isEncrypted()         { return (m_desc.sec_state.encrypted == 1); }

    /** @brief Check if the the connection has been authenticated */
    bool             isAuthenticated()     { return (m_desc.sec_state.authenticated == 1); }

    /** @brief Gets the key size used to encrypt the connection */
    uint8_t          getSecKeySize()       { return m_desc.sec_state.key_size; }
};
#endif
