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

#include "NimBLEUtils.h"
#if CONFIG_BT_ENABLED

# include "NimBLEAddress.h"
# include "NimBLELog.h"

# if defined(CONFIG_NIMBLE_CPP_IDF)
#  include "host/ble_hs.h"
# else
#  include "nimble/nimble/host/include/host/ble_hs.h"
# endif

/****  FIX COMPILATION ****/
# undef min
# undef max
/**************************/

# include <stdlib.h>
# include <climits>

# if defined INC_FREERTOS_H
#  ifndef CONFIG_NIMBLE_CPP_FREERTOS_TASK_BLOCK_BIT
#   define CONFIG_NIMBLE_CPP_FREERTOS_TASK_BLOCK_BIT 31
#  endif
constexpr uint32_t TASK_BLOCK_BIT = (1 << CONFIG_NIMBLE_CPP_FREERTOS_TASK_BLOCK_BIT);
# endif

static const char* LOG_TAG = "NimBLEUtils";

/**
 * @brief Construct a NimBLETaskData instance.
 * @param [in] pInstance An instance of the class that will be waiting.
 * @param [in] flags General purpose flags for the caller.
 * @param [in] buf A buffer for data.
 */
NimBLETaskData::NimBLETaskData(void* pInstance, int flags, void* buf)
    : m_pInstance{pInstance},
      m_flags{flags},
      m_pBuf{buf}
# if defined INC_FREERTOS_H
      ,
      m_pHandle{xTaskGetCurrentTaskHandle()} {
}
# else
{
    ble_npl_sem* sem = new ble_npl_sem;
    if (ble_npl_sem_init(sem, 0) != BLE_NPL_OK) {
        NIMBLE_LOGE(LOG_TAG, "Failed to init semaphore");
        delete sem;
        m_pHandle = nullptr;
    } else {
        m_pHandle = sem;
    }
}
# endif

/**
 * @brief Destructor.
 */
NimBLETaskData::~NimBLETaskData() {
# if !defined INC_FREERTOS_H
    if (m_pHandle != nullptr) {
        ble_npl_sem_deinit(static_cast<ble_npl_sem*>(m_pHandle));
        delete static_cast<ble_npl_sem*>(m_pHandle);
    }
# endif
}

/**
 * @brief Blocks the calling task until released or timeout.
 * @param [in] taskData A pointer to the task data structure.
 * @param [in] timeout The time to wait in milliseconds.
 * @return True if the task completed, false if the timeout was reached.
 */
bool NimBLEUtils::taskWait(const NimBLETaskData& taskData, uint32_t timeout) {
    ble_npl_time_t ticks;
    if (timeout == BLE_NPL_TIME_FOREVER) {
        ticks = BLE_NPL_TIME_FOREVER;
    } else {
        ble_npl_time_ms_to_ticks(timeout, &ticks);
    }

# if defined INC_FREERTOS_H
    uint32_t notificationValue;
    xTaskNotifyWait(0, TASK_BLOCK_BIT, &notificationValue, 0);
    if (notificationValue & TASK_BLOCK_BIT) {
        return true;
    }

    return xTaskNotifyWait(0, TASK_BLOCK_BIT, nullptr, ticks) == pdTRUE;

# else
    return ble_npl_sem_pend(static_cast<ble_npl_sem*>(taskData.m_pHandle), ticks) == BLE_NPL_OK;
# endif
} // taskWait

/**
 * @brief Release a task.
 * @param [in] taskData A pointer to the task data structure.
 * @param [in] flags A return value to set in the task data structure.
 */
void NimBLEUtils::taskRelease(const NimBLETaskData& taskData, int flags) {
    taskData.m_flags = flags;
    if (taskData.m_pHandle != nullptr) {
# if defined INC_FREERTOS_H
        xTaskNotify(static_cast<TaskHandle_t>(taskData.m_pHandle), TASK_BLOCK_BIT, eSetBits);
# else
        ble_npl_sem_release(static_cast<ble_npl_sem*>(taskData.m_pHandle));
# endif
    }
} // taskRelease

/**
 * @brief Converts a return code from the NimBLE stack to a text string.
 * @param [in] rc The return code to convert.
 * @return A string representation of the return code.
 */
const char* NimBLEUtils::returnCodeToString(int rc) {
# if defined(CONFIG_NIMBLE_CPP_ENABLE_RETURN_CODE_TEXT)
    switch (rc) {
        case 0:
            return "SUCCESS";
        case BLE_HS_EAGAIN:
            return "Temporary failure; try again";
        case BLE_HS_EALREADY:
            return "Operation already in progress or complete";
        case BLE_HS_EINVAL:
            return "One or more arguments are invalid";
        case BLE_HS_EMSGSIZE:
            return "Buffer too small";
        case BLE_HS_ENOENT:
            return "No matching entry found";
        case BLE_HS_ENOMEM:
            return "Not enough memory";
        case BLE_HS_ENOTCONN:
            return "No open connection with handle";
        case BLE_HS_ENOTSUP:
            return "Operation disabled at compile time";
        case BLE_HS_EAPP:
            return "Operation canceled by app";
        case BLE_HS_EBADDATA:
            return "Invalid command from peer";
        case BLE_HS_EOS:
            return "OS error";
        case BLE_HS_ECONTROLLER:
            return "Controller error";
        case BLE_HS_ETIMEOUT:
            return "Operation timed out";
        case BLE_HS_EDONE:
            return "Operation completed successfully";
        case BLE_HS_EBUSY:
            return "Busy";
        case BLE_HS_EREJECT:
            return "Peer rejected request";
        case BLE_HS_EUNKNOWN:
            return "Unknown failure";
        case BLE_HS_EROLE:
            return "Operation requires different role";
        case BLE_HS_ETIMEOUT_HCI:
            return "HCI request timed out";
        case BLE_HS_ENOMEM_EVT:
            return "Controller error; not enough memory";
        case BLE_HS_ENOADDR:
            return "No identity address";
        case BLE_HS_ENOTSYNCED:
            return "Host not synced with controller";
        case BLE_HS_EAUTHEN:
            return "Insufficient authentication";
        case BLE_HS_EAUTHOR:
            return "Insufficient authorization";
        case BLE_HS_EENCRYPT:
            return "Insufficient encryption level";
        case BLE_HS_EENCRYPT_KEY_SZ:
            return "Insufficient key size";
        case BLE_HS_ESTORE_CAP:
            return "Storage at capacity";
        case BLE_HS_ESTORE_FAIL:
            return "Storage IO error";
        case BLE_HS_EPREEMPTED:
            return "Host preempted";
        case BLE_HS_EDISABLED:
            return "Host disabled";
        case BLE_HS_ESTALLED:
            return "CoC module is stalled";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_INVALID_HANDLE):
            return "Invalid attribute handle";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_READ_NOT_PERMITTED):
            return "The attribute cannot be read";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_WRITE_NOT_PERMITTED):
            return "The attribute cannot be written";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_INVALID_PDU):
            return "Invalid attribute PDU";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_INSUFFICIENT_AUTHEN):
            return "Insufficient authentication";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_REQ_NOT_SUPPORTED):
            return "Unsupported request";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_INVALID_OFFSET):
            return "Invalid offset";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_INSUFFICIENT_AUTHOR):
            return "Insufficient authorization";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_PREPARE_QUEUE_FULL):
            return "Prepare write queue full";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_ATTR_NOT_FOUND):
            return "Attribute not found";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_ATTR_NOT_LONG):
            return "Long read not supported";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_INSUFFICIENT_KEY_SZ):
            return "Insufficient encryption key size";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN):
            return "Invalid attribute value length";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_UNLIKELY):
            return "Unlikely attribute error";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_INSUFFICIENT_ENC):
            return "Insufficient encryption";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_UNSUPPORTED_GROUP):
            return "Not a supported grouping attribute type";
        case (BLE_HS_ERR_ATT_BASE + BLE_ATT_ERR_INSUFFICIENT_RES):
            return "Insufficient Resources";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_UNKNOWN_HCI_CMD):
            return "Unknown HCI Command";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_UNK_CONN_ID):
            return "Unknown Connection Identifier";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_HW_FAIL):
            return "Hardware Failure";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_PAGE_TMO):
            return "Page Timeout";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_AUTH_FAIL):
            return "Authentication Failure";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_PINKEY_MISSING):
            return "PIN or Key Missing";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_MEM_CAPACITY):
            return "Memory Capacity Exceeded";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_CONN_SPVN_TMO):
            return "Connection Timeout";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_CONN_LIMIT):
            return "Connection Limit Exceeded";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_SYNCH_CONN_LIMIT):
            return "Synchronous Connection Limit To A Device Exceeded";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_ACL_CONN_EXISTS):
            return "ACL Connection Already Exists";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_CMD_DISALLOWED):
            return "Command Disallowed";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_CONN_REJ_RESOURCES):
            return "Connection Rejected due to Limited Resources";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_CONN_REJ_SECURITY):
            return "Connection Rejected Due To Security Reasons";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_CONN_REJ_BD_ADDR):
            return "Connection Rejected due to Unacceptable BD_ADDR";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_CONN_ACCEPT_TMO):
            return "Connection Accept Timeout Exceeded";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_UNSUPPORTED):
            return "Unsupported Feature or Parameter Value";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_INV_HCI_CMD_PARMS):
            return "Invalid HCI Command Parameters";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_REM_USER_CONN_TERM):
            return "Remote User Terminated Connection";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_RD_CONN_TERM_RESRCS):
            return "Remote Device Terminated Connection; Low Resources";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_RD_CONN_TERM_PWROFF):
            return "Remote Device Terminated Connection; Power Off";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_CONN_TERM_LOCAL):
            return "Connection Terminated By Local Host";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_REPEATED_ATTEMPTS):
            return "Repeated Pairing Attempts";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_NO_PAIRING):
            return "Pairing Not Allowed";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_UNK_LMP):
            return "Unknown LMP PDU";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_UNSUPP_REM_FEATURE):
            return "Unsupported Remote Feature";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_SCO_OFFSET):
            return "SCO Offset Rejected";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_SCO_ITVL):
            return "SCO Interval Rejected";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_SCO_AIR_MODE):
            return "SCO Air Mode Rejected";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_INV_LMP_LL_PARM):
            return "Invalid LL Parameters";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_UNSPECIFIED):
            return "Unspecified Error";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_UNSUPP_LMP_LL_PARM):
            return "Unsupported LL Parameter Value";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_NO_ROLE_CHANGE):
            return "Role Change Not Allowed";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_LMP_LL_RSP_TMO):
            return "LL Response Timeout";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_LMP_COLLISION):
            return "LMP Error Transaction Collision";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_LMP_PDU):
            return "LMP PDU Not Allowed";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_ENCRYPTION_MODE):
            return "Encryption Mode Not Acceptable";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_LINK_KEY_CHANGE):
            return "Link Key cannot be Changed";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_UNSUPP_QOS):
            return "Requested QoS Not Supported";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_INSTANT_PASSED):
            return "Instant Passed";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_UNIT_KEY_PAIRING):
            return "Pairing With Unit Key Not Supported";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_DIFF_TRANS_COLL):
            return "Different Transaction Collision";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_QOS_PARM):
            return "QoS Unacceptable Parameter";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_QOS_REJECTED):
            return "QoS Rejected";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_CHAN_CLASS):
            return "Channel Classification Not Supported";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_INSUFFICIENT_SEC):
            return "Insufficient Security";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_PARM_OUT_OF_RANGE):
            return "Parameter Out Of Mandatory Range";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_PENDING_ROLE_SW):
            return "Role Switch Pending";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_RESERVED_SLOT):
            return "Reserved Slot Violation";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_ROLE_SW_FAIL):
            return "Role Switch Failed";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_INQ_RSP_TOO_BIG):
            return "Extended Inquiry Response Too Large";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_SEC_SIMPLE_PAIR):
            return "Secure Simple Pairing Not Supported By Host";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_HOST_BUSY_PAIR):
            return "Host Busy - Pairing";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_CONN_REJ_CHANNEL):
            return "Connection Rejected, No Suitable Channel Found";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_CTLR_BUSY):
            return "Controller Busy";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_CONN_PARMS):
            return "Unacceptable Connection Parameters";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_DIR_ADV_TMO):
            return "Directed Advertising Timeout";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_CONN_TERM_MIC):
            return "Connection Terminated; MIC Failure";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_CONN_ESTABLISHMENT):
            return "Connection Failed to be Established";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_MAC_CONN_FAIL):
            return "MAC Connection Failed";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_COARSE_CLK_ADJ):
            return "Coarse Clock Adjustment Rejected";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_TYPE0_SUBMAP_NDEF):
            return "Type0 Submap Not Defined";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_UNK_ADV_INDENT):
            return "Unknown Advertising Identifier";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_LIMIT_REACHED):
            return "Limit Reached";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_OPERATION_CANCELLED):
            return "Operation Cancelled by Host";
        case (BLE_HS_ERR_HCI_BASE + BLE_ERR_PACKET_TOO_LONG):
            return "Packet Too Long";
        case (BLE_HS_ERR_L2C_BASE + BLE_L2CAP_SIG_ERR_CMD_NOT_UNDERSTOOD):
            return "Invalid or unsupported incoming L2CAP sig command";
        case (BLE_HS_ERR_L2C_BASE + BLE_L2CAP_SIG_ERR_MTU_EXCEEDED):
            return "Incoming packet too large";
        case (BLE_HS_ERR_L2C_BASE + BLE_L2CAP_SIG_ERR_INVALID_CID):
            return "No channel with specified ID";
        case (BLE_HS_ERR_SM_US_BASE + BLE_SM_ERR_PASSKEY):
        case (BLE_HS_ERR_SM_PEER_BASE + BLE_SM_ERR_PASSKEY):
            return "Incorrect passkey or the user cancelled";
        case (BLE_HS_ERR_SM_US_BASE + BLE_SM_ERR_OOB):
        case (BLE_HS_ERR_SM_PEER_BASE + BLE_SM_ERR_OOB):
            return "The OOB data is not available";
        case (BLE_HS_ERR_SM_US_BASE + BLE_SM_ERR_AUTHREQ):
        case (BLE_HS_ERR_SM_PEER_BASE + BLE_SM_ERR_AUTHREQ):
            return "Authentication requirements cannot be met due to IO capabilities";
        case (BLE_HS_ERR_SM_US_BASE + BLE_SM_ERR_CONFIRM_MISMATCH):
        case (BLE_HS_ERR_SM_PEER_BASE + BLE_SM_ERR_CONFIRM_MISMATCH):
            return "The confirm value does not match the calculated compare value";
        case (BLE_HS_ERR_SM_US_BASE + BLE_SM_ERR_PAIR_NOT_SUPP):
        case (BLE_HS_ERR_SM_PEER_BASE + BLE_SM_ERR_PAIR_NOT_SUPP):
            return "Pairing is not supported by the device";
        case (BLE_HS_ERR_SM_US_BASE + BLE_SM_ERR_ENC_KEY_SZ):
        case (BLE_HS_ERR_SM_PEER_BASE + BLE_SM_ERR_ENC_KEY_SZ):
            return "Insufficient encryption key size for this device";
        case (BLE_HS_ERR_SM_US_BASE + BLE_SM_ERR_CMD_NOT_SUPP):
        case (BLE_HS_ERR_SM_PEER_BASE + BLE_SM_ERR_CMD_NOT_SUPP):
            return "The SMP command received is not supported on this device";
        case (BLE_HS_ERR_SM_US_BASE + BLE_SM_ERR_UNSPECIFIED):
        case (BLE_HS_ERR_SM_PEER_BASE + BLE_SM_ERR_UNSPECIFIED):
            return "Pairing failed; unspecified reason";
        case (BLE_HS_ERR_SM_US_BASE + BLE_SM_ERR_REPEATED):
        case (BLE_HS_ERR_SM_PEER_BASE + BLE_SM_ERR_REPEATED):
            return "Repeated pairing attempt";
        case (BLE_HS_ERR_SM_US_BASE + BLE_SM_ERR_INVAL):
        case (BLE_HS_ERR_SM_PEER_BASE + BLE_SM_ERR_INVAL):
            return "Invalid command length or parameter";
        case (BLE_HS_ERR_SM_US_BASE + BLE_SM_ERR_DHKEY):
        case (BLE_HS_ERR_SM_PEER_BASE + BLE_SM_ERR_DHKEY):
            return "DHKey check value received doesn't match";
        case (BLE_HS_ERR_SM_US_BASE + BLE_SM_ERR_NUMCMP):
        case (BLE_HS_ERR_SM_PEER_BASE + BLE_SM_ERR_NUMCMP):
            return "Confirm values do not match";
        case (BLE_HS_ERR_SM_US_BASE + BLE_SM_ERR_ALREADY):
        case (BLE_HS_ERR_SM_PEER_BASE + BLE_SM_ERR_ALREADY):
            return "Pairing already in process";
        case (BLE_HS_ERR_SM_US_BASE + BLE_SM_ERR_CROSS_TRANS):
        case (BLE_HS_ERR_SM_PEER_BASE + BLE_SM_ERR_CROSS_TRANS):
            return "Invalid link key for the LE transport";
        default:
            return "Unknown";
    }
# else  // #if defined(CONFIG_NIMBLE_CPP_ENABLE_RETURN_CODE_TEXT)
    (void)rc;
    return "";
# endif // #if defined(CONFIG_NIMBLE_CPP_ENABLE_RETURN_CODE_TEXT)
}

/**
 * @brief Convert the advertising type flag to a string.
 * @param advType The type to convert.
 * @return A string representation of the advertising flags.
 */
const char* NimBLEUtils::advTypeToString(uint8_t advType) {
# if defined(CONFIG_NIMBLE_CPP_ENABLE_ADVERTISEMENT_TYPE_TEXT)
    switch (advType) {
        case BLE_HCI_ADV_TYPE_ADV_IND: // 0
            return "Undirected - Connectable / Scannable";
        case BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_HD: // 1
            return "Directed High Duty - Connectable";
        case BLE_HCI_ADV_TYPE_ADV_SCAN_IND: // 2
            return "Non-Connectable - Scan Response Available";
        case BLE_HCI_ADV_TYPE_ADV_NONCONN_IND: // 3
            return "Non-Connectable - No Scan Response";
        case BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_LD: // 4
            return "Directed Low Duty - Connectable";
        default:
            return "Unknown flag";
    }
# else  // #if defined(CONFIG_NIMBLE_CPP_ENABLE_ADVERTISEMENT_TYPE_TEXT)
    (void)advType;
    return "";
# endif // #if defined(CONFIG_NIMBLE_CPP_ENABLE_ADVERTISEMENT_TYPE_TEXT)
} // adFlagsToString

/**
 * @brief Convert a GAP event type to a string representation.
 * @param [in] eventType The type of event.
 * @return A string representation of the event type.
 */
const char* NimBLEUtils::gapEventToString(uint8_t eventType) {
# if defined(CONFIG_NIMBLE_CPP_ENABLE_GAP_EVENT_CODE_TEXT)
    switch (eventType) {
        case BLE_GAP_EVENT_CONNECT: // 0
            return "BLE_GAP_EVENT_CONNECT ";
        case BLE_GAP_EVENT_DISCONNECT: // 1
            return "BLE_GAP_EVENT_DISCONNECT";
        case BLE_GAP_EVENT_CONN_UPDATE: // 3
            return "BLE_GAP_EVENT_CONN_UPDATE";
        case BLE_GAP_EVENT_CONN_UPDATE_REQ: // 4
            return "BLE_GAP_EVENT_CONN_UPDATE_REQ";
        case BLE_GAP_EVENT_L2CAP_UPDATE_REQ: // 5
            return "BLE_GAP_EVENT_L2CAP_UPDATE_REQ";
        case BLE_GAP_EVENT_TERM_FAILURE: // 6
            return "BLE_GAP_EVENT_TERM_FAILURE";
        case BLE_GAP_EVENT_DISC: // 7
            return "BLE_GAP_EVENT_DISC";
        case BLE_GAP_EVENT_DISC_COMPLETE: // 8
            return "BLE_GAP_EVENT_DISC_COMPLETE";
        case BLE_GAP_EVENT_ADV_COMPLETE: // 9
            return "BLE_GAP_EVENT_ADV_COMPLETE";
        case BLE_GAP_EVENT_ENC_CHANGE: // 10
            return "BLE_GAP_EVENT_ENC_CHANGE";
        case BLE_GAP_EVENT_PASSKEY_ACTION: // 11
            return "BLE_GAP_EVENT_PASSKEY_ACTION";
        case BLE_GAP_EVENT_NOTIFY_RX: // 12
            return "BLE_GAP_EVENT_NOTIFY_RX";
        case BLE_GAP_EVENT_NOTIFY_TX: // 13
            return "BLE_GAP_EVENT_NOTIFY_TX";
        case BLE_GAP_EVENT_SUBSCRIBE: // 14
            return "BLE_GAP_EVENT_SUBSCRIBE";
        case BLE_GAP_EVENT_MTU: // 15
            return "BLE_GAP_EVENT_MTU";
        case BLE_GAP_EVENT_IDENTITY_RESOLVED: // 16
            return "BLE_GAP_EVENT_IDENTITY_RESOLVED";
        case BLE_GAP_EVENT_REPEAT_PAIRING: // 17
            return "BLE_GAP_EVENT_REPEAT_PAIRING";
        case BLE_GAP_EVENT_PHY_UPDATE_COMPLETE: // 18
            return "BLE_GAP_EVENT_PHY_UPDATE_COMPLETE";
        case BLE_GAP_EVENT_EXT_DISC: // 19
            return "BLE_GAP_EVENT_EXT_DISC";
#  ifdef BLE_GAP_EVENT_PERIODIC_SYNC      // IDF 4.0 does not support these
        case BLE_GAP_EVENT_PERIODIC_SYNC: // 20
            return "BLE_GAP_EVENT_PERIODIC_SYNC";
        case BLE_GAP_EVENT_PERIODIC_REPORT: // 21
            return "BLE_GAP_EVENT_PERIODIC_REPORT";
        case BLE_GAP_EVENT_PERIODIC_SYNC_LOST: // 22
            return "BLE_GAP_EVENT_PERIODIC_SYNC_LOST";
        case BLE_GAP_EVENT_SCAN_REQ_RCVD: // 23
            return "BLE_GAP_EVENT_SCAN_REQ_RCVD";
        case BLE_GAP_EVENT_PERIODIC_TRANSFER: // 24
            return "BLE_GAP_EVENT_PERIODIC_TRANSFER";
        case BLE_GAP_EVENT_PATHLOSS_THRESHOLD: // 25
            return "BLE_GAP_EVENT_PATHLOSS_THRESHOLD";
        case BLE_GAP_EVENT_TRANSMIT_POWER: // 26
            return "BLE_GAP_EVENT_TRANSMIT_POWER";
        case BLE_GAP_EVENT_PARING_COMPLETE: // 27
            return "BLE_GAP_EVENT_PARING_COMPLETE";
        case BLE_GAP_EVENT_SUBRATE_CHANGE: // 28
            return "BLE_GAP_EVENT_SUBRATE_CHANGE";
        case BLE_GAP_EVENT_VS_HCI: // 29
            return "BLE_GAP_EVENT_VS_HCI";
        case BLE_GAP_EVENT_REATTEMPT_COUNT: // 31
            return "BLE_GAP_EVENT_REATTEMPT_COUNT";
        case BLE_GAP_EVENT_AUTHORIZE: // 32
            return "BLE_GAP_EVENT_AUTHORIZE";
        case BLE_GAP_EVENT_TEST_UPDATE: // 33
            return "BLE_GAP_EVENT_TEST_UPDATE";
#   ifdef BLE_GAP_EVENT_DATA_LEN_CHG
        case BLE_GAP_EVENT_DATA_LEN_CHG: // 34
            return "BLE_GAP_EVENT_DATA_LEN_CHG";
#   endif
#   ifdef BLE_GAP_EVENT_LINK_ESTAB
        case BLE_GAP_EVENT_LINK_ESTAB: // 38
            return "BLE_GAP_EVENT_LINK_ESTAB";
#   endif
#  endif
        default:
            NIMBLE_LOGD(LOG_TAG, "Unknown event type %d 0x%.2x", eventType, eventType);
            return "Unknown event type";
    }
# else  // #if defined(CONFIG_NIMBLE_CPP_ENABLE_GAP_EVENT_CODE_TEXT)
    (void)eventType;
    return "";
# endif // #if defined(CONFIG_NIMBLE_CPP_ENABLE_GAP_EVENT_CODE_TEXT)
} // gapEventToString

/**
 * @brief Create a hexadecimal string representation of the input data.
 * @param [in] source The start of the binary data.
 * @param [in] length The length of the data to convert.
 * @return A string representation of the data.
 */
std::string NimBLEUtils::dataToHexString(const uint8_t* source, uint8_t length) {
    constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    std::string    str{};
    str.resize(length << 1);

    for (uint8_t i = 0; i < length; i++) {
        str[2 * i]     = hexmap[(source[i] & 0xF0) >> 4];
        str[2 * i + 1] = hexmap[source[i] & 0x0F];
    }

    return str;
} // dataToHexString

/**
 * @brief Generate a random BLE address.
 * @param [in] nrpa True to generate a non-resolvable private address,
 * false to generate a random static address
 * @return The generated address or a NULL address if there was an error.
 */
NimBLEAddress NimBLEUtils::generateAddr(bool nrpa) {
    ble_addr_t addr{};
    int        rc = ble_hs_id_gen_rnd(nrpa, &addr);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Generate address failed, rc=%d", rc);
    }

    return NimBLEAddress{addr};
} // generateAddr

#endif // CONFIG_BT_ENABLED
