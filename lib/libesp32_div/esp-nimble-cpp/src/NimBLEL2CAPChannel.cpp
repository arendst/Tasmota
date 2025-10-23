//
// (C) Dr. Michael 'Mickey' Lauer <mickey@vanille-media.de>
//

#include "NimBLEL2CAPChannel.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM

# include "NimBLEClient.h"
# include "NimBLELog.h"
# include "NimBLEUtils.h"

# if defined(CONFIG_NIMBLE_CPP_IDF)
#  include "host/ble_gap.h"
# else
#  include "nimble/nimble/host/include/host/ble_gap.h"
# endif

// L2CAP buffer block size
# define L2CAP_BUF_BLOCK_SIZE            (250)
# define L2CAP_BUF_SIZE_MTUS_PER_CHANNEL (3)
// Round-up integer division
# define CEIL_DIVIDE(a, b)               (((a) + (b) - 1) / (b))
# define ROUND_DIVIDE(a, b)              (((a) + (b) / 2) / (b))
// Retry
constexpr uint32_t RetryTimeout = 50;
constexpr int      RetryCounter = 3;

NimBLEL2CAPChannel::NimBLEL2CAPChannel(uint16_t psm, uint16_t mtu, NimBLEL2CAPChannelCallbacks* callbacks)
    : psm(psm), mtu(mtu), callbacks(callbacks) {
    assert(mtu);            // fail here, if MTU is too little
    assert(callbacks);      // fail here, if no callbacks are given
    assert(setupMemPool()); // fail here, if the memory pool could not be setup

    NIMBLE_LOGI(LOG_TAG, "L2CAP COC 0x%04X initialized w/ L2CAP MTU %i", this->psm, this->mtu);
};

NimBLEL2CAPChannel::~NimBLEL2CAPChannel() {
    teardownMemPool();

    NIMBLE_LOGI(LOG_TAG, "L2CAP COC 0x%04X shutdown and freed.", this->psm);
}

bool NimBLEL2CAPChannel::setupMemPool() {
    const size_t buf_blocks = CEIL_DIVIDE(mtu, L2CAP_BUF_BLOCK_SIZE) * L2CAP_BUF_SIZE_MTUS_PER_CHANNEL;
    NIMBLE_LOGD(LOG_TAG, "Computed number of buf_blocks = %d", buf_blocks);

    _coc_memory = malloc(OS_MEMPOOL_SIZE(buf_blocks, L2CAP_BUF_BLOCK_SIZE) * sizeof(os_membuf_t));
    if (_coc_memory == 0) {
        NIMBLE_LOGE(LOG_TAG, "Can't allocate _coc_memory: %d", errno);
        return false;
    }

    auto rc = os_mempool_init(&_coc_mempool, buf_blocks, L2CAP_BUF_BLOCK_SIZE, _coc_memory, "appbuf");
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Can't os_mempool_init: %d", rc);
        return false;
    }

    auto rc2 = os_mbuf_pool_init(&_coc_mbuf_pool, &_coc_mempool, L2CAP_BUF_BLOCK_SIZE, buf_blocks);
    if (rc2 != 0) {
        NIMBLE_LOGE(LOG_TAG, "Can't os_mbuf_pool_init: %d", rc);
        return false;
    }

    this->receiveBuffer = (uint8_t*)malloc(mtu);
    if (!this->receiveBuffer) {
        NIMBLE_LOGE(LOG_TAG, "Can't malloc receive buffer: %d, %s", errno, strerror(errno));
        return false;
    }

    return true;
}

void NimBLEL2CAPChannel::teardownMemPool() {
    if (this->callbacks) {
        delete this->callbacks;
    }
    if (this->receiveBuffer) {
        free(this->receiveBuffer);
    }
    if (_coc_memory) {
        free(_coc_memory);
    }
}

int NimBLEL2CAPChannel::writeFragment(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end) {
    auto toSend = end - begin;

    if (stalled) {
        NIMBLE_LOGD(LOG_TAG, "L2CAP Channel waiting for unstall...");
        NimBLETaskData taskData;
        m_pTaskData = &taskData;
        NimBLEUtils::taskWait(taskData, BLE_NPL_TIME_FOREVER);
        m_pTaskData = nullptr;
        stalled     = false;
        NIMBLE_LOGD(LOG_TAG, "L2CAP Channel unstalled!");
    }

    struct ble_l2cap_chan_info info;
    ble_l2cap_get_chan_info(channel, &info);
    // Take the minimum of our and peer MTU
    auto mtu = info.peer_coc_mtu < info.our_coc_mtu ? info.peer_coc_mtu : info.our_coc_mtu;

    if (toSend > mtu) {
        return -BLE_HS_EBADDATA;
    }

    auto retries = RetryCounter;

    while (retries--) {
        auto txd = os_mbuf_get_pkthdr(&_coc_mbuf_pool, 0);
        if (!txd) {
            NIMBLE_LOGE(LOG_TAG, "Can't os_mbuf_get_pkthdr.");
            return -BLE_HS_ENOMEM;
        }
        auto append = os_mbuf_append(txd, &(*begin), toSend);
        if (append != 0) {
            NIMBLE_LOGE(LOG_TAG, "Can't os_mbuf_append: %d", append);
            return append;
        }

        auto res = ble_l2cap_send(channel, txd);
        switch (res) {
            case 0:
                NIMBLE_LOGD(LOG_TAG, "L2CAP COC 0x%04X sent %d bytes.", this->psm, toSend);
                return 0;

            case BLE_HS_ESTALLED:
                stalled = true;
                NIMBLE_LOGD(LOG_TAG, "L2CAP COC 0x%04X sent %d bytes.", this->psm, toSend);
                NIMBLE_LOGW(LOG_TAG,
                            "ble_l2cap_send returned BLE_HS_ESTALLED. Next send will wait for unstalled event...");
                return 0;

            case BLE_HS_ENOMEM:
            case BLE_HS_EAGAIN:
            case BLE_HS_EBUSY:
                NIMBLE_LOGD(LOG_TAG, "ble_l2cap_send returned %d. Retrying shortly...", res);
                os_mbuf_free_chain(txd);
                ble_npl_time_delay(ble_npl_time_ms_to_ticks32(RetryTimeout));
                continue;

            default:
                NIMBLE_LOGE(LOG_TAG, "ble_l2cap_send failed: %d", res);
                return res;
        }
    }
    NIMBLE_LOGE(LOG_TAG, "Retries exhausted, dropping %d bytes to send.", toSend);
    return -BLE_HS_EREJECT;
}

# if CONFIG_BT_NIMBLE_ROLE_CENTRAL
NimBLEL2CAPChannel* NimBLEL2CAPChannel::connect(NimBLEClient*                client,
                                                uint16_t                     psm,
                                                uint16_t                     mtu,
                                                NimBLEL2CAPChannelCallbacks* callbacks) {
    if (!client->isConnected()) {
        NIMBLE_LOGE(
            LOG_TAG,
            "Client is not connected. Before connecting via L2CAP, a GAP connection must have been established");
        return nullptr;
    };

    auto channel = new NimBLEL2CAPChannel(psm, mtu, callbacks);

    auto sdu_rx = os_mbuf_get_pkthdr(&channel->_coc_mbuf_pool, 0);
    if (!sdu_rx) {
        NIMBLE_LOGE(LOG_TAG, "Can't allocate SDU buffer: %d, %s", errno, strerror(errno));
        return nullptr;
    }
    auto rc = ble_l2cap_connect(client->getConnHandle(), psm, mtu, sdu_rx, NimBLEL2CAPChannel::handleL2capEvent, channel);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_l2cap_connect failed: %d", rc);
    }
    return channel;
}
# endif // CONFIG_BT_NIMBLE_ROLE_CENTRAL

bool NimBLEL2CAPChannel::write(const std::vector<uint8_t>& bytes) {
    if (!this->channel) {
        NIMBLE_LOGW(LOG_TAG, "L2CAP Channel not open");
        return false;
    }

    struct ble_l2cap_chan_info info;
    ble_l2cap_get_chan_info(channel, &info);
    auto mtu = info.peer_coc_mtu < info.our_coc_mtu ? info.peer_coc_mtu : info.our_coc_mtu;

    auto start = bytes.begin();
    while (start != bytes.end()) {
        auto end = start + mtu < bytes.end() ? start + mtu : bytes.end();
        if (writeFragment(start, end) < 0) {
            return false;
        }
        start = end;
    }
    return true;
}

// private
int NimBLEL2CAPChannel::handleConnectionEvent(struct ble_l2cap_event* event) {
    channel = event->connect.chan;
    struct ble_l2cap_chan_info info;
    ble_l2cap_get_chan_info(channel, &info);
    NIMBLE_LOGI(LOG_TAG,
                "L2CAP COC 0x%04X connected. Local MTU = %d [%d], remote MTU = %d [%d].",
                psm,
                info.our_coc_mtu,
                info.our_l2cap_mtu,
                info.peer_coc_mtu,
                info.peer_l2cap_mtu);
    if (info.our_coc_mtu > info.peer_coc_mtu) {
        NIMBLE_LOGW(LOG_TAG, "L2CAP COC 0x%04X connected, but local MTU is bigger than remote MTU.", psm);
    }
    auto mtu = info.peer_coc_mtu < info.our_coc_mtu ? info.peer_coc_mtu : info.our_coc_mtu;
    callbacks->onConnect(this, mtu);
    return 0;
}

int NimBLEL2CAPChannel::handleAcceptEvent(struct ble_l2cap_event* event) {
    NIMBLE_LOGI(LOG_TAG, "L2CAP COC 0x%04X accept.", psm);
    if (!callbacks->shouldAcceptConnection(this)) {
        NIMBLE_LOGI(LOG_TAG, "L2CAP COC 0x%04X refused by delegate.", psm);
        return -1;
    }

    struct os_mbuf* sdu_rx = os_mbuf_get_pkthdr(&_coc_mbuf_pool, 0);
    assert(sdu_rx != NULL);
    ble_l2cap_recv_ready(event->accept.chan, sdu_rx);
    return 0;
}

int NimBLEL2CAPChannel::handleDataReceivedEvent(struct ble_l2cap_event* event) {
    NIMBLE_LOGD(LOG_TAG, "L2CAP COC 0x%04X data received.", psm);

    struct os_mbuf* rxd = event->receive.sdu_rx;
    assert(rxd != NULL);

    int rx_len = (int)OS_MBUF_PKTLEN(rxd);
    assert(rx_len <= (int)mtu);

    int res = os_mbuf_copydata(rxd, 0, rx_len, receiveBuffer);
    assert(res == 0);

    NIMBLE_LOGD(LOG_TAG, "L2CAP COC 0x%04X received %d bytes.", psm, rx_len);

    res = os_mbuf_free_chain(rxd);
    assert(res == 0);

    std::vector<uint8_t> incomingData(receiveBuffer, receiveBuffer + rx_len);
    callbacks->onRead(this, incomingData);

    struct os_mbuf* next = os_mbuf_get_pkthdr(&_coc_mbuf_pool, 0);
    assert(next != NULL);

    res = ble_l2cap_recv_ready(channel, next);
    assert(res == 0);

    return 0;
}

int NimBLEL2CAPChannel::handleTxUnstalledEvent(struct ble_l2cap_event* event) {
    if (m_pTaskData != nullptr) {
        NimBLEUtils::taskRelease(*m_pTaskData, event->tx_unstalled.status);
    }

    NIMBLE_LOGI(LOG_TAG, "L2CAP COC 0x%04X transmit unstalled.", psm);
    return 0;
}

int NimBLEL2CAPChannel::handleDisconnectionEvent(struct ble_l2cap_event* event) {
    NIMBLE_LOGI(LOG_TAG, "L2CAP COC 0x%04X disconnected.", psm);
    channel = NULL;
    callbacks->onDisconnect(this);
    return 0;
}

/* STATIC */
int NimBLEL2CAPChannel::handleL2capEvent(struct ble_l2cap_event* event, void* arg) {
    NIMBLE_LOGD(LOG_TAG, "handleL2capEvent: handling l2cap event %d", event->type);
    NimBLEL2CAPChannel* self = reinterpret_cast<NimBLEL2CAPChannel*>(arg);

    int returnValue = 0;

    switch (event->type) {
        case BLE_L2CAP_EVENT_COC_CONNECTED:
            returnValue = self->handleConnectionEvent(event);
            break;

        case BLE_L2CAP_EVENT_COC_DISCONNECTED:
            returnValue = self->handleDisconnectionEvent(event);
            break;

        case BLE_L2CAP_EVENT_COC_ACCEPT:
            returnValue = self->handleAcceptEvent(event);
            break;

        case BLE_L2CAP_EVENT_COC_DATA_RECEIVED:
            returnValue = self->handleDataReceivedEvent(event);
            break;

        case BLE_L2CAP_EVENT_COC_TX_UNSTALLED:
            returnValue = self->handleTxUnstalledEvent(event);
            break;

        default:
            NIMBLE_LOGW(LOG_TAG, "Unhandled l2cap event %d", event->type);
            break;
    }

    return returnValue;
}

#endif // #if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM
