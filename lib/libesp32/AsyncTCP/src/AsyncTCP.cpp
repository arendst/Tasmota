/*
  Asynchronous TCP library for Espressif MCUs

  Copyright (c) 2016 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Arduino.h"

#include "AsyncTCP.h"
extern "C"{
#include "lwip/opt.h"
#include "lwip/tcp.h"
#include "lwip/inet.h"
#include "lwip/dns.h"
#include "lwip/err.h"
}
#include "esp_task_wdt.h"

/*
 * TCP/IP Event Task
 * */

typedef enum {
    LWIP_TCP_SENT, LWIP_TCP_RECV, LWIP_TCP_FIN, LWIP_TCP_ERROR, LWIP_TCP_POLL, LWIP_TCP_CLEAR, LWIP_TCP_ACCEPT, LWIP_TCP_CONNECTED, LWIP_TCP_DNS
} lwip_event_t;

typedef struct {
        lwip_event_t event;
        void *arg;
        union {
                struct {
                        void * pcb;
                        int8_t err;
                } connected;
                struct {
                        int8_t err;
                } error;
                struct {
                        tcp_pcb * pcb;
                        uint16_t len;
                } sent;
                struct {
                        tcp_pcb * pcb;
                        pbuf * pb;
                        int8_t err;
                } recv;
                struct {
                        tcp_pcb * pcb;
                        int8_t err;
                } fin;
                struct {
                        tcp_pcb * pcb;
                } poll;
                struct {
                        AsyncClient * client;
                } accept;
                struct {
                        const char * name;
                        ip_addr_t addr;
                } dns;
        };
} lwip_event_packet_t;

static xQueueHandle _async_queue;
static TaskHandle_t _async_service_task_handle = NULL;


SemaphoreHandle_t _slots_lock;
const int _number_of_closed_slots = CONFIG_LWIP_MAX_ACTIVE_TCP;
static uint32_t _closed_slots[_number_of_closed_slots];
static uint32_t _closed_index = []() {
    _slots_lock = xSemaphoreCreateBinary();
    xSemaphoreGive(_slots_lock);
    for (int i = 0; i < _number_of_closed_slots; ++ i) {
        _closed_slots[i] = 1;
    }
    return 1;
}();


static inline bool _init_async_event_queue(){
    if(!_async_queue){
        _async_queue = xQueueCreate(32, sizeof(lwip_event_packet_t *));
        if(!_async_queue){
            return false;
        }
    }
    return true;
}

static inline bool _send_async_event(lwip_event_packet_t ** e){
    return _async_queue && xQueueSend(_async_queue, e, portMAX_DELAY) == pdPASS;
}

static inline bool _prepend_async_event(lwip_event_packet_t ** e){
    return _async_queue && xQueueSendToFront(_async_queue, e, portMAX_DELAY) == pdPASS;
}

static inline bool _get_async_event(lwip_event_packet_t ** e){
    return _async_queue && xQueueReceive(_async_queue, e, portMAX_DELAY) == pdPASS;
}

static bool _remove_events_with_arg(void * arg){
    lwip_event_packet_t * first_packet = NULL;
    lwip_event_packet_t * packet = NULL;

    if(!_async_queue){
        return false;
    }
    //figure out which is the first packet so we can keep the order
    while(!first_packet){
        if(xQueueReceive(_async_queue, &first_packet, 0) != pdPASS){
            return false;
        }
        //discard packet if matching
        if((int)first_packet->arg == (int)arg){
            free(first_packet);
            first_packet = NULL;
        //return first packet to the back of the queue
        } else if(xQueueSend(_async_queue, &first_packet, portMAX_DELAY) != pdPASS){
            return false;
        }
    }

    while(xQueuePeek(_async_queue, &packet, 0) == pdPASS && packet != first_packet){
        if(xQueueReceive(_async_queue, &packet, 0) != pdPASS){
            return false;
        }
        if((int)packet->arg == (int)arg){
            free(packet);
            packet = NULL;
        } else if(xQueueSend(_async_queue, &packet, portMAX_DELAY) != pdPASS){
            return false;
        }
    }
    return true;
}

static void _handle_async_event(lwip_event_packet_t * e){
    if(e->arg == NULL){
        // do nothing when arg is NULL
        //ets_printf("event arg == NULL: 0x%08x\n", e->recv.pcb);
    } else if(e->event == LWIP_TCP_CLEAR){
        _remove_events_with_arg(e->arg);
    } else if(e->event == LWIP_TCP_RECV){
        //ets_printf("-R: 0x%08x\n", e->recv.pcb);
        AsyncClient::_s_recv(e->arg, e->recv.pcb, e->recv.pb, e->recv.err);
    } else if(e->event == LWIP_TCP_FIN){
        //ets_printf("-F: 0x%08x\n", e->fin.pcb);
        AsyncClient::_s_fin(e->arg, e->fin.pcb, e->fin.err);
    } else if(e->event == LWIP_TCP_SENT){
        //ets_printf("-S: 0x%08x\n", e->sent.pcb);
        AsyncClient::_s_sent(e->arg, e->sent.pcb, e->sent.len);
    } else if(e->event == LWIP_TCP_POLL){
        //ets_printf("-P: 0x%08x\n", e->poll.pcb);
        AsyncClient::_s_poll(e->arg, e->poll.pcb);
    } else if(e->event == LWIP_TCP_ERROR){
        //ets_printf("-E: 0x%08x %d\n", e->arg, e->error.err);
        AsyncClient::_s_error(e->arg, e->error.err);
    } else if(e->event == LWIP_TCP_CONNECTED){
        //ets_printf("C: 0x%08x 0x%08x %d\n", e->arg, e->connected.pcb, e->connected.err);
        AsyncClient::_s_connected(e->arg, e->connected.pcb, e->connected.err);
    } else if(e->event == LWIP_TCP_ACCEPT){
        //ets_printf("A: 0x%08x 0x%08x\n", e->arg, e->accept.client);
        AsyncServer::_s_accepted(e->arg, e->accept.client);
    } else if(e->event == LWIP_TCP_DNS){
        //ets_printf("D: 0x%08x %s = %s\n", e->arg, e->dns.name, ipaddr_ntoa(&e->dns.addr));
        AsyncClient::_s_dns_found(e->dns.name, &e->dns.addr, e->arg);
    }
    free((void*)(e));
}

static void _async_service_task(void *pvParameters){
    lwip_event_packet_t * packet = NULL;
    for (;;) {
        if(_get_async_event(&packet)){
#if CONFIG_ASYNC_TCP_USE_WDT
            if(esp_task_wdt_add(NULL) != ESP_OK){
                log_e("Failed to add async task to WDT");
            }
#endif
            _handle_async_event(packet);
#if CONFIG_ASYNC_TCP_USE_WDT
            if(esp_task_wdt_delete(NULL) != ESP_OK){
                log_e("Failed to remove loop task from WDT");
            }
#endif
        }
    }
    vTaskDelete(NULL);
    _async_service_task_handle = NULL;
}
/*
static void _stop_async_task(){
    if(_async_service_task_handle){
        vTaskDelete(_async_service_task_handle);
        _async_service_task_handle = NULL;
    }
}
*/
static bool _start_async_task(){
    if(!_init_async_event_queue()){
        return false;
    }
    if(!_async_service_task_handle){
        xTaskCreateUniversal(_async_service_task, "async_tcp", 8192 * 2, NULL, 3, &_async_service_task_handle, CONFIG_ASYNC_TCP_RUNNING_CORE);
        if(!_async_service_task_handle){
            return false;
        }
    }
    return true;
}

/*
 * LwIP Callbacks
 * */

static int8_t _tcp_clear_events(void * arg) {
    lwip_event_packet_t * e = (lwip_event_packet_t *)malloc(sizeof(lwip_event_packet_t));
    e->event = LWIP_TCP_CLEAR;
    e->arg = arg;
    if (!_prepend_async_event(&e)) {
        free((void*)(e));
    }
    return ERR_OK;
}

static int8_t _tcp_connected(void * arg, tcp_pcb * pcb, int8_t err) {
    //ets_printf("+C: 0x%08x\n", pcb);
    lwip_event_packet_t * e = (lwip_event_packet_t *)malloc(sizeof(lwip_event_packet_t));
    e->event = LWIP_TCP_CONNECTED;
    e->arg = arg;
    e->connected.pcb = pcb;
    e->connected.err = err;
    if (!_prepend_async_event(&e)) {
        free((void*)(e));
    }
    return ERR_OK;
}

static int8_t _tcp_poll(void * arg, struct tcp_pcb * pcb) {
    //ets_printf("+P: 0x%08x\n", pcb);
    lwip_event_packet_t * e = (lwip_event_packet_t *)malloc(sizeof(lwip_event_packet_t));
    e->event = LWIP_TCP_POLL;
    e->arg = arg;
    e->poll.pcb = pcb;
    if (!_send_async_event(&e)) {
        free((void*)(e));
    }
    return ERR_OK;
}

static int8_t _tcp_recv(void * arg, struct tcp_pcb * pcb, struct pbuf *pb, int8_t err) {
    lwip_event_packet_t * e = (lwip_event_packet_t *)malloc(sizeof(lwip_event_packet_t));
    e->arg = arg;
    if(pb){
        //ets_printf("+R: 0x%08x\n", pcb);
        e->event = LWIP_TCP_RECV;
        e->recv.pcb = pcb;
        e->recv.pb = pb;
        e->recv.err = err;
    } else {
        //ets_printf("+F: 0x%08x\n", pcb);
        e->event = LWIP_TCP_FIN;
        e->fin.pcb = pcb;
        e->fin.err = err;
        //close the PCB in LwIP thread
        AsyncClient::_s_lwip_fin(e->arg, e->fin.pcb, e->fin.err);
    }
    if (!_send_async_event(&e)) {
        free((void*)(e));
    }
    return ERR_OK;
}

static int8_t _tcp_sent(void * arg, struct tcp_pcb * pcb, uint16_t len) {
    //ets_printf("+S: 0x%08x\n", pcb);
    lwip_event_packet_t * e = (lwip_event_packet_t *)malloc(sizeof(lwip_event_packet_t));
    e->event = LWIP_TCP_SENT;
    e->arg = arg;
    e->sent.pcb = pcb;
    e->sent.len = len;
    if (!_send_async_event(&e)) {
        free((void*)(e));
    }
    return ERR_OK;
}

static void _tcp_error(void * arg, int8_t err) {
    //ets_printf("+E: 0x%08x\n", arg);
    lwip_event_packet_t * e = (lwip_event_packet_t *)malloc(sizeof(lwip_event_packet_t));
    e->event = LWIP_TCP_ERROR;
    e->arg = arg;
    e->error.err = err;
    if (!_send_async_event(&e)) {
        free((void*)(e));
    }
}

static void _tcp_dns_found(const char * name, struct ip_addr * ipaddr, void * arg) {
    lwip_event_packet_t * e = (lwip_event_packet_t *)malloc(sizeof(lwip_event_packet_t));
    //ets_printf("+DNS: name=%s ipaddr=0x%08x arg=%x\n", name, ipaddr, arg);
    e->event = LWIP_TCP_DNS;
    e->arg = arg;
    e->dns.name = name;
    if (ipaddr) {
        memcpy(&e->dns.addr, ipaddr, sizeof(struct ip_addr));
    } else {
        memset(&e->dns.addr, 0, sizeof(e->dns.addr));
    }
    if (!_send_async_event(&e)) {
        free((void*)(e));
    }
}

//Used to switch out from LwIP thread
static int8_t _tcp_accept(void * arg, AsyncClient * client) {
    lwip_event_packet_t * e = (lwip_event_packet_t *)malloc(sizeof(lwip_event_packet_t));
    e->event = LWIP_TCP_ACCEPT;
    e->arg = arg;
    e->accept.client = client;
    if (!_prepend_async_event(&e)) {
        free((void*)(e));
    }
    return ERR_OK;
}

/*
 * TCP/IP API Calls
 * */

#include "lwip/priv/tcpip_priv.h"

typedef struct {
    struct tcpip_api_call_data call;
    tcp_pcb * pcb;
    int8_t closed_slot;
    int8_t err;
    union {
            struct {
                    const char* data;
                    size_t size;
                    uint8_t apiflags;
            } write;
            size_t received;
            struct {
                    ip_addr_t * addr;
                    uint16_t port;
                    tcp_connected_fn cb;
            } connect;
            struct {
                    ip_addr_t * addr;
                    uint16_t port;
            } bind;
            uint8_t backlog;
    };
} tcp_api_call_t;

static err_t _tcp_output_api(struct tcpip_api_call_data *api_call_msg){
    tcp_api_call_t * msg = (tcp_api_call_t *)api_call_msg;
    msg->err = ERR_CONN;
    if(msg->closed_slot == -1 || !_closed_slots[msg->closed_slot]) {
        msg->err = tcp_output(msg->pcb);
    }
    return msg->err;
}

static esp_err_t _tcp_output(tcp_pcb * pcb, int8_t closed_slot) {
    if(!pcb){
        return ERR_CONN;
    }
    tcp_api_call_t msg;
    msg.pcb = pcb;
    msg.closed_slot = closed_slot;
    tcpip_api_call(_tcp_output_api, (struct tcpip_api_call_data*)&msg);
    return msg.err;
}

static err_t _tcp_write_api(struct tcpip_api_call_data *api_call_msg){
    tcp_api_call_t * msg = (tcp_api_call_t *)api_call_msg;
    msg->err = ERR_CONN;
    if(msg->closed_slot == -1 || !_closed_slots[msg->closed_slot]) {
        msg->err = tcp_write(msg->pcb, msg->write.data, msg->write.size, msg->write.apiflags);
    }
    return msg->err;
}

static esp_err_t _tcp_write(tcp_pcb * pcb, int8_t closed_slot, const char* data, size_t size, uint8_t apiflags) {
    if(!pcb){
        return ERR_CONN;
    }
    tcp_api_call_t msg;
    msg.pcb = pcb;
    msg.closed_slot = closed_slot;
    msg.write.data = data;
    msg.write.size = size;
    msg.write.apiflags = apiflags;
    tcpip_api_call(_tcp_write_api, (struct tcpip_api_call_data*)&msg);
    return msg.err;
}

static err_t _tcp_recved_api(struct tcpip_api_call_data *api_call_msg){
    tcp_api_call_t * msg = (tcp_api_call_t *)api_call_msg;
    msg->err = ERR_CONN;
    if(msg->closed_slot == -1 || !_closed_slots[msg->closed_slot]) {
        msg->err = 0;
        tcp_recved(msg->pcb, msg->received);
    }
    return msg->err;
}

static esp_err_t _tcp_recved(tcp_pcb * pcb, int8_t closed_slot, size_t len) {
    if(!pcb){
        return ERR_CONN;
    }
    tcp_api_call_t msg;
    msg.pcb = pcb;
    msg.closed_slot = closed_slot;
    msg.received = len;
    tcpip_api_call(_tcp_recved_api, (struct tcpip_api_call_data*)&msg);
    return msg.err;
}

static err_t _tcp_close_api(struct tcpip_api_call_data *api_call_msg){
    tcp_api_call_t * msg = (tcp_api_call_t *)api_call_msg;
    msg->err = ERR_CONN;
    if(msg->closed_slot == -1 || !_closed_slots[msg->closed_slot]) {
        msg->err = tcp_close(msg->pcb);
    }
    return msg->err;
}

static esp_err_t _tcp_close(tcp_pcb * pcb, int8_t closed_slot) {
    if(!pcb){
        return ERR_CONN;
    }
    tcp_api_call_t msg;
    msg.pcb = pcb;
    msg.closed_slot = closed_slot;
    tcpip_api_call(_tcp_close_api, (struct tcpip_api_call_data*)&msg);
    return msg.err;
}

static err_t _tcp_abort_api(struct tcpip_api_call_data *api_call_msg){
    tcp_api_call_t * msg = (tcp_api_call_t *)api_call_msg;
    msg->err = ERR_CONN;
    if(msg->closed_slot == -1 || !_closed_slots[msg->closed_slot]) {
        tcp_abort(msg->pcb);
    }
    return msg->err;
}

static esp_err_t _tcp_abort(tcp_pcb * pcb, int8_t closed_slot) {
    if(!pcb){
        return ERR_CONN;
    }
    tcp_api_call_t msg;
    msg.pcb = pcb;
    msg.closed_slot = closed_slot;
    tcpip_api_call(_tcp_abort_api, (struct tcpip_api_call_data*)&msg);
    return msg.err;
}

static err_t _tcp_connect_api(struct tcpip_api_call_data *api_call_msg){
    tcp_api_call_t * msg = (tcp_api_call_t *)api_call_msg;
    msg->err = tcp_connect(msg->pcb, msg->connect.addr, msg->connect.port, msg->connect.cb);
    return msg->err;
}

static esp_err_t _tcp_connect(tcp_pcb * pcb, int8_t closed_slot, ip_addr_t * addr, uint16_t port, tcp_connected_fn cb) {
    if(!pcb){
        return ESP_FAIL;
    }
    tcp_api_call_t msg;
    msg.pcb = pcb;
    msg.closed_slot = closed_slot;
    msg.connect.addr = addr;
    msg.connect.port = port;
    msg.connect.cb = cb;
    tcpip_api_call(_tcp_connect_api, (struct tcpip_api_call_data*)&msg);
    return msg.err;
}

static err_t _tcp_bind_api(struct tcpip_api_call_data *api_call_msg){
    tcp_api_call_t * msg = (tcp_api_call_t *)api_call_msg;
    msg->err = tcp_bind(msg->pcb, msg->bind.addr, msg->bind.port);
    return msg->err;
}

static esp_err_t _tcp_bind(tcp_pcb * pcb, ip_addr_t * addr, uint16_t port) {
    if(!pcb){
        return ESP_FAIL;
    }
    tcp_api_call_t msg;
    msg.pcb = pcb;
    msg.closed_slot = -1;
    msg.bind.addr = addr;
    msg.bind.port = port;
    tcpip_api_call(_tcp_bind_api, (struct tcpip_api_call_data*)&msg);
    return msg.err;
}

static err_t _tcp_listen_api(struct tcpip_api_call_data *api_call_msg){
    tcp_api_call_t * msg = (tcp_api_call_t *)api_call_msg;
    msg->err = 0;
    msg->pcb = tcp_listen_with_backlog(msg->pcb, msg->backlog);
    return msg->err;
}

static tcp_pcb * _tcp_listen_with_backlog(tcp_pcb * pcb, uint8_t backlog) {
    if(!pcb){
        return NULL;
    }
    tcp_api_call_t msg;
    msg.pcb = pcb;
    msg.closed_slot = -1;
    msg.backlog = backlog?backlog:0xFF;
    tcpip_api_call(_tcp_listen_api, (struct tcpip_api_call_data*)&msg);
    return msg.pcb;
}



/*
  Async TCP Client
 */

AsyncClient::AsyncClient(tcp_pcb* pcb)
: _connect_cb(0)
, _connect_cb_arg(0)
, _discard_cb(0)
, _discard_cb_arg(0)
, _sent_cb(0)
, _sent_cb_arg(0)
, _error_cb(0)
, _error_cb_arg(0)
, _recv_cb(0)
, _recv_cb_arg(0)
, _pb_cb(0)
, _pb_cb_arg(0)
, _timeout_cb(0)
, _timeout_cb_arg(0)
, _pcb_busy(false)
, _pcb_sent_at(0)
, _ack_pcb(true)
, _rx_last_packet(0)
, _rx_since_timeout(0)
, _ack_timeout(ASYNC_MAX_ACK_TIME)
, _connect_port(0)
, prev(NULL)
, next(NULL)
{
    _pcb = pcb;
    _closed_slot = -1;
    if(_pcb){
        _allocate_closed_slot();
        _rx_last_packet = millis();
        tcp_arg(_pcb, this);
        tcp_recv(_pcb, &_tcp_recv);
        tcp_sent(_pcb, &_tcp_sent);
        tcp_err(_pcb, &_tcp_error);
        tcp_poll(_pcb, &_tcp_poll, 1);
    }
}

AsyncClient::~AsyncClient(){
    if(_pcb) {
        _close();
    }
    _free_closed_slot();
}

/*
 * Operators
 * */

AsyncClient& AsyncClient::operator=(const AsyncClient& other){
    if (_pcb) {
        _close();
    }

    _pcb = other._pcb;
    _closed_slot = other._closed_slot;
    if (_pcb) {
        _rx_last_packet = millis();
        tcp_arg(_pcb, this);
        tcp_recv(_pcb, &_tcp_recv);
        tcp_sent(_pcb, &_tcp_sent);
        tcp_err(_pcb, &_tcp_error);
        tcp_poll(_pcb, &_tcp_poll, 1);
    }
    return *this;
}

bool AsyncClient::operator==(const AsyncClient &other) {
    return _pcb == other._pcb;
}

AsyncClient & AsyncClient::operator+=(const AsyncClient &other) {
    if(next == NULL){
        next = (AsyncClient*)(&other);
        next->prev = this;
    } else {
        AsyncClient *c = next;
        while(c->next != NULL) {
            c = c->next;
        }
        c->next =(AsyncClient*)(&other);
        c->next->prev = c;
    }
    return *this;
}

/*
 * Callback Setters
 * */

void AsyncClient::onConnect(AcConnectHandler cb, void* arg){
    _connect_cb = cb;
    _connect_cb_arg = arg;
}

void AsyncClient::onDisconnect(AcConnectHandler cb, void* arg){
    _discard_cb = cb;
    _discard_cb_arg = arg;
}

void AsyncClient::onAck(AcAckHandler cb, void* arg){
    _sent_cb = cb;
    _sent_cb_arg = arg;
}

void AsyncClient::onError(AcErrorHandler cb, void* arg){
    _error_cb = cb;
    _error_cb_arg = arg;
}

void AsyncClient::onData(AcDataHandler cb, void* arg){
    _recv_cb = cb;
    _recv_cb_arg = arg;
}

void AsyncClient::onPacket(AcPacketHandler cb, void* arg){
  _pb_cb = cb;
  _pb_cb_arg = arg;
}

void AsyncClient::onTimeout(AcTimeoutHandler cb, void* arg){
    _timeout_cb = cb;
    _timeout_cb_arg = arg;
}

void AsyncClient::onPoll(AcConnectHandler cb, void* arg){
    _poll_cb = cb;
    _poll_cb_arg = arg;
}

/*
 * Main Public Methods
 * */

bool AsyncClient::connect(IPAddress ip, uint16_t port){
    if (_pcb){
        log_w("already connected, state %d", _pcb->state);
        return false;
    }
    if(!_start_async_task()){
        log_e("failed to start task");
        return false;
    }

    ip_addr_t addr;
    addr.type = IPADDR_TYPE_V4;
    addr.u_addr.ip4.addr = ip;

    tcp_pcb* pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
    if (!pcb){
        log_e("pcb == NULL");
        return false;
    }

    tcp_arg(pcb, this);
    tcp_err(pcb, &_tcp_error);
    tcp_recv(pcb, &_tcp_recv);
    tcp_sent(pcb, &_tcp_sent);
    tcp_poll(pcb, &_tcp_poll, 1);
    //_tcp_connect(pcb, &addr, port,(tcp_connected_fn)&_s_connected);
    _tcp_connect(pcb, _closed_slot, &addr, port,(tcp_connected_fn)&_tcp_connected);
    return true;
}

bool AsyncClient::connect(const char* host, uint16_t port){
    ip_addr_t addr;
    
    if(!_start_async_task()){
      log_e("failed to start task");
      return false;
    }
    
    err_t err = dns_gethostbyname(host, &addr, (dns_found_callback)&_tcp_dns_found, this);
    if(err == ERR_OK) {
        return connect(IPAddress(addr.u_addr.ip4.addr), port);
    } else if(err == ERR_INPROGRESS) {
        _connect_port = port;
        return true;
    }
    log_e("error: %d", err);
    return false;
}

void AsyncClient::close(bool now){
    if(_pcb){
        _tcp_recved(_pcb, _closed_slot, _rx_ack_len);
    }
    _close();
}

int8_t AsyncClient::abort(){
    if(_pcb) {
        _tcp_abort(_pcb, _closed_slot );
        _pcb = NULL;
    }
    return ERR_ABRT;
}

size_t AsyncClient::space(){
    if((_pcb != NULL) && (_pcb->state == 4)){
        return tcp_sndbuf(_pcb);
    }
    return 0;
}

size_t AsyncClient::add(const char* data, size_t size, uint8_t apiflags) {
    if(!_pcb || size == 0 || data == NULL) {
        return 0;
    }
    size_t room = space();
    if(!room) {
        return 0;
    }
    size_t will_send = (room < size) ? room : size;
    int8_t err = ERR_OK;
    err = _tcp_write(_pcb, _closed_slot, data, will_send, apiflags);
    if(err != ERR_OK) {
        return 0;
    }
    return will_send;
}

bool AsyncClient::send(){
    int8_t err = ERR_OK;
    err = _tcp_output(_pcb, _closed_slot);
    if(err == ERR_OK){
        _pcb_busy = true;
        _pcb_sent_at = millis();
        return true;
    }
    return false;
}

size_t AsyncClient::ack(size_t len){
    if(len > _rx_ack_len)
        len = _rx_ack_len;
    if(len){
        _tcp_recved(_pcb, _closed_slot, len);
    }
    _rx_ack_len -= len;
    return len;
}

void AsyncClient::ackPacket(struct pbuf * pb){
  if(!pb){
    return;
  }
  _tcp_recved(_pcb, _closed_slot, pb->len);
  pbuf_free(pb);
}

/*
 * Main Private Methods
 * */

int8_t AsyncClient::_close(){
    //ets_printf("X: 0x%08x\n", (uint32_t)this);
    int8_t err = ERR_OK;
    if(_pcb) {
        //log_i("");
        tcp_arg(_pcb, NULL);
        tcp_sent(_pcb, NULL);
        tcp_recv(_pcb, NULL);
        tcp_err(_pcb, NULL);
        tcp_poll(_pcb, NULL, 0);
        _tcp_clear_events(this);
        err = _tcp_close(_pcb, _closed_slot);
        if(err != ERR_OK) {
            err = abort();
        }
        _pcb = NULL;
        if(_discard_cb) {
            _discard_cb(_discard_cb_arg, this);
        }
    }
    return err;
}

void AsyncClient::_allocate_closed_slot(){
    xSemaphoreTake(_slots_lock, portMAX_DELAY);
    uint32_t closed_slot_min_index = 0;
    for (int i = 0; i < _number_of_closed_slots; ++ i) {
        if ((_closed_slot == -1 || _closed_slots[i] <= closed_slot_min_index) && _closed_slots[i] != 0) {
            closed_slot_min_index = _closed_slots[i];
            _closed_slot = i;
        }
    }
    if (_closed_slot != -1) {
        _closed_slots[_closed_slot] = 0;
    }
    xSemaphoreGive(_slots_lock);
}

void AsyncClient::_free_closed_slot(){
    if (_closed_slot != -1) {
        _closed_slots[_closed_slot] = _closed_index;
        _closed_slot = -1;
        ++ _closed_index;
    }
}

/*
 * Private Callbacks
 * */

int8_t AsyncClient::_connected(void* pcb, int8_t err){
    _pcb = reinterpret_cast<tcp_pcb*>(pcb);
    if(_pcb){
        _rx_last_packet = millis();
        _pcb_busy = false;
//        tcp_recv(_pcb, &_tcp_recv);
//        tcp_sent(_pcb, &_tcp_sent);
//        tcp_poll(_pcb, &_tcp_poll, 1);
    }
    if(_connect_cb) {
        _connect_cb(_connect_cb_arg, this);
    }
    return ERR_OK;
}

void AsyncClient::_error(int8_t err) {
    if(_pcb){
        tcp_arg(_pcb, NULL);
        if(_pcb->state == LISTEN) {
            tcp_sent(_pcb, NULL);
            tcp_recv(_pcb, NULL);
            tcp_err(_pcb, NULL);
            tcp_poll(_pcb, NULL, 0);
        }
        _pcb = NULL;
    }
    if(_error_cb) {
        _error_cb(_error_cb_arg, this, err);
    }
    if(_discard_cb) {
        _discard_cb(_discard_cb_arg, this);
    }
}

//In LwIP Thread
int8_t AsyncClient::_lwip_fin(tcp_pcb* pcb, int8_t err) {
    if(!_pcb || pcb != _pcb){
        log_e("0x%08x != 0x%08x", (uint32_t)pcb, (uint32_t)_pcb);
        return ERR_OK;
    }
    tcp_arg(_pcb, NULL);
    if(_pcb->state == LISTEN) {
        tcp_sent(_pcb, NULL);
        tcp_recv(_pcb, NULL);
        tcp_err(_pcb, NULL);
        tcp_poll(_pcb, NULL, 0);
    }
    if(tcp_close(_pcb) != ERR_OK) {
        tcp_abort(_pcb);
    }
    _free_closed_slot();
    _pcb = NULL;
    return ERR_OK;
}

//In Async Thread
int8_t AsyncClient::_fin(tcp_pcb* pcb, int8_t err) {
    _tcp_clear_events(this);
    if(_discard_cb) {
        _discard_cb(_discard_cb_arg, this);
    }
    return ERR_OK;
}

int8_t AsyncClient::_sent(tcp_pcb* pcb, uint16_t len) {
    _rx_last_packet = millis();
    //log_i("%u", len);
    _pcb_busy = false;
    if(_sent_cb) {
        _sent_cb(_sent_cb_arg, this, len, (millis() - _pcb_sent_at));
    }
    return ERR_OK;
}

int8_t AsyncClient::_recv(tcp_pcb* pcb, pbuf* pb, int8_t err) {
    while(pb != NULL) {
        _rx_last_packet = millis();
        //we should not ack before we assimilate the data
        _ack_pcb = true;
        pbuf *b = pb;
        pb = b->next;
        b->next = NULL;
        if(_pb_cb){
            _pb_cb(_pb_cb_arg, this, b);
        } else {
            if(_recv_cb) {
                _recv_cb(_recv_cb_arg, this, b->payload, b->len);
            }
            if(!_ack_pcb) {
                _rx_ack_len += b->len;
            } else if(_pcb) {
                _tcp_recved(_pcb, _closed_slot, b->len);
            }
            pbuf_free(b);
        }
    }
    return ERR_OK;
}

int8_t AsyncClient::_poll(tcp_pcb* pcb){
    if(!_pcb){
        log_w("pcb is NULL");
        return ERR_OK;
    }
    if(pcb != _pcb){
        log_e("0x%08x != 0x%08x", (uint32_t)pcb, (uint32_t)_pcb);
        return ERR_OK;
    }

    uint32_t now = millis();

    // ACK Timeout
    if(_pcb_busy && _ack_timeout && (now - _pcb_sent_at) >= _ack_timeout){
        _pcb_busy = false;
        log_w("ack timeout %d", pcb->state);
        if(_timeout_cb)
            _timeout_cb(_timeout_cb_arg, this, (now - _pcb_sent_at));
        return ERR_OK;
    }
    // RX Timeout
    if(_rx_since_timeout && (now - _rx_last_packet) >= (_rx_since_timeout * 1000)){
        log_w("rx timeout %d", pcb->state);
        _close();
        return ERR_OK;
    }
    // Everything is fine
    if(_poll_cb) {
        _poll_cb(_poll_cb_arg, this);
    }
    return ERR_OK;
}

void AsyncClient::_dns_found(struct ip_addr *ipaddr){
    if(ipaddr && ipaddr->u_addr.ip4.addr){
        connect(IPAddress(ipaddr->u_addr.ip4.addr), _connect_port);
    } else {
        if(_error_cb) {
            _error_cb(_error_cb_arg, this, -55);
        }
        if(_discard_cb) {
            _discard_cb(_discard_cb_arg, this);
        }
    }
}

/*
 * Public Helper Methods
 * */

void AsyncClient::stop() {
    close(false);
}

bool AsyncClient::free(){
    if(!_pcb) {
        return true;
    }
    if(_pcb->state == 0 || _pcb->state > 4) {
        return true;
    }
    return false;
}

size_t AsyncClient::write(const char* data) {
    if(data == NULL) {
        return 0;
    }
    return write(data, strlen(data));
}

size_t AsyncClient::write(const char* data, size_t size, uint8_t apiflags) {
    size_t will_send = add(data, size, apiflags);
    if(!will_send || !send()) {
        return 0;
    }
    return will_send;
}

void AsyncClient::setRxTimeout(uint32_t timeout){
    _rx_since_timeout = timeout;
}

uint32_t AsyncClient::getRxTimeout(){
    return _rx_since_timeout;
}

uint32_t AsyncClient::getAckTimeout(){
    return _ack_timeout;
}

void AsyncClient::setAckTimeout(uint32_t timeout){
    _ack_timeout = timeout;
}

void AsyncClient::setNoDelay(bool nodelay){
    if(!_pcb) {
        return;
    }
    if(nodelay) {
        tcp_nagle_disable(_pcb);
    } else {
        tcp_nagle_enable(_pcb);
    }
}

bool AsyncClient::getNoDelay(){
    if(!_pcb) {
        return false;
    }
    return tcp_nagle_disabled(_pcb);
}

uint16_t AsyncClient::getMss(){
    if(!_pcb) {
        return 0;
    }
    return tcp_mss(_pcb);
}

uint32_t AsyncClient::getRemoteAddress() {
    if(!_pcb) {
        return 0;
    }
    return _pcb->remote_ip.u_addr.ip4.addr;
}

uint16_t AsyncClient::getRemotePort() {
    if(!_pcb) {
        return 0;
    }
    return _pcb->remote_port;
}

uint32_t AsyncClient::getLocalAddress() {
    if(!_pcb) {
        return 0;
    }
    return _pcb->local_ip.u_addr.ip4.addr;
}

uint16_t AsyncClient::getLocalPort() {
    if(!_pcb) {
        return 0;
    }
    return _pcb->local_port;
}

IPAddress AsyncClient::remoteIP() {
    return IPAddress(getRemoteAddress());
}

uint16_t AsyncClient::remotePort() {
    return getRemotePort();
}

IPAddress AsyncClient::localIP() {
    return IPAddress(getLocalAddress());
}

uint16_t AsyncClient::localPort() {
    return getLocalPort();
}

uint8_t AsyncClient::state() {
    if(!_pcb) {
        return 0;
    }
    return _pcb->state;
}

bool AsyncClient::connected(){
    if (!_pcb) {
        return false;
    }
    return _pcb->state == 4;
}

bool AsyncClient::connecting(){
    if (!_pcb) {
        return false;
    }
    return _pcb->state > 0 && _pcb->state < 4;
}

bool AsyncClient::disconnecting(){
    if (!_pcb) {
        return false;
    }
    return _pcb->state > 4 && _pcb->state < 10;
}

bool AsyncClient::disconnected(){
    if (!_pcb) {
        return true;
    }
    return _pcb->state == 0 || _pcb->state == 10;
}

bool AsyncClient::freeable(){
    if (!_pcb) {
        return true;
    }
    return _pcb->state == 0 || _pcb->state > 4;
}

bool AsyncClient::canSend(){
    return space() > 0;
}

const char * AsyncClient::errorToString(int8_t error){
    switch(error){
        case ERR_OK: return "OK";
        case ERR_MEM: return "Out of memory error";
        case ERR_BUF: return "Buffer error";
        case ERR_TIMEOUT: return "Timeout";
        case ERR_RTE: return "Routing problem";
        case ERR_INPROGRESS: return "Operation in progress";
        case ERR_VAL: return "Illegal value";
        case ERR_WOULDBLOCK: return "Operation would block";
        case ERR_USE: return "Address in use";
        case ERR_ALREADY: return "Already connected";
        case ERR_CONN: return "Not connected";
        case ERR_IF: return "Low-level netif error";
        case ERR_ABRT: return "Connection aborted";
        case ERR_RST: return "Connection reset";
        case ERR_CLSD: return "Connection closed";
        case ERR_ARG: return "Illegal argument";
        case -55: return "DNS failed";
        default: return "UNKNOWN";
    }
}

const char * AsyncClient::stateToString(){
    switch(state()){
        case 0: return "Closed";
        case 1: return "Listen";
        case 2: return "SYN Sent";
        case 3: return "SYN Received";
        case 4: return "Established";
        case 5: return "FIN Wait 1";
        case 6: return "FIN Wait 2";
        case 7: return "Close Wait";
        case 8: return "Closing";
        case 9: return "Last ACK";
        case 10: return "Time Wait";
        default: return "UNKNOWN";
    }
}

/*
 * Static Callbacks (LwIP C2C++ interconnect)
 * */

void AsyncClient::_s_dns_found(const char * name, struct ip_addr * ipaddr, void * arg){
    reinterpret_cast<AsyncClient*>(arg)->_dns_found(ipaddr);
}

int8_t AsyncClient::_s_poll(void * arg, struct tcp_pcb * pcb) {
    return reinterpret_cast<AsyncClient*>(arg)->_poll(pcb);
}

int8_t AsyncClient::_s_recv(void * arg, struct tcp_pcb * pcb, struct pbuf *pb, int8_t err) {
    return reinterpret_cast<AsyncClient*>(arg)->_recv(pcb, pb, err);
}

int8_t AsyncClient::_s_fin(void * arg, struct tcp_pcb * pcb, int8_t err) {
    return reinterpret_cast<AsyncClient*>(arg)->_fin(pcb, err);
}

int8_t AsyncClient::_s_lwip_fin(void * arg, struct tcp_pcb * pcb, int8_t err) {
    return reinterpret_cast<AsyncClient*>(arg)->_lwip_fin(pcb, err);
}

int8_t AsyncClient::_s_sent(void * arg, struct tcp_pcb * pcb, uint16_t len) {
    return reinterpret_cast<AsyncClient*>(arg)->_sent(pcb, len);
}

void AsyncClient::_s_error(void * arg, int8_t err) {
    reinterpret_cast<AsyncClient*>(arg)->_error(err);
}

int8_t AsyncClient::_s_connected(void * arg, void * pcb, int8_t err){
    return reinterpret_cast<AsyncClient*>(arg)->_connected(pcb, err);
}

/*
  Async TCP Server
 */

AsyncServer::AsyncServer(IPAddress addr, uint16_t port)
: _port(port)
, _addr(addr)
, _noDelay(false)
, _pcb(0)
, _connect_cb(0)
, _connect_cb_arg(0)
{}

AsyncServer::AsyncServer(uint16_t port)
: _port(port)
, _addr((uint32_t) IPADDR_ANY)
, _noDelay(false)
, _pcb(0)
, _connect_cb(0)
, _connect_cb_arg(0)
{}

AsyncServer::~AsyncServer(){
    end();
}

void AsyncServer::onClient(AcConnectHandler cb, void* arg){
    _connect_cb = cb;
    _connect_cb_arg = arg;
}

void AsyncServer::begin(){
    if(_pcb) {
        return;
    }

    if(!_start_async_task()){
        log_e("failed to start task");
        return;
    }
    int8_t err;
    _pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
    if (!_pcb){
        log_e("_pcb == NULL");
        return;
    }

    ip_addr_t local_addr;
    local_addr.type = IPADDR_TYPE_V4;
    local_addr.u_addr.ip4.addr = (uint32_t) _addr;
    err = _tcp_bind(_pcb, &local_addr, _port);

    if (err != ERR_OK) {
        _tcp_close(_pcb, -1);
        log_e("bind error: %d", err);
        return;
    }

    static uint8_t backlog = 5;
    _pcb = _tcp_listen_with_backlog(_pcb, backlog);
    if (!_pcb) {
        log_e("listen_pcb == NULL");
        return;
    }
    tcp_arg(_pcb, (void*) this);
    tcp_accept(_pcb, &_s_accept);
}

void AsyncServer::end(){
    if(_pcb){
        tcp_arg(_pcb, NULL);
        tcp_accept(_pcb, NULL);
        if(tcp_close(_pcb) != ERR_OK){
            _tcp_abort(_pcb, -1);
        }
        _pcb = NULL;
    }
}

//runs on LwIP thread
int8_t AsyncServer::_accept(tcp_pcb* pcb, int8_t err){
    //ets_printf("+A: 0x%08x\n", pcb);
    if(_connect_cb){
        AsyncClient *c = new AsyncClient(pcb);
        if(c){
            c->setNoDelay(_noDelay);
            return _tcp_accept(this, c);
        }
    }
    if(tcp_close(pcb) != ERR_OK){
        tcp_abort(pcb);
    }
    log_e("FAIL");
    return ERR_OK;
}

int8_t AsyncServer::_accepted(AsyncClient* client){
    if(_connect_cb){
        _connect_cb(_connect_cb_arg, client);
    }
    return ERR_OK;
}

void AsyncServer::setNoDelay(bool nodelay){
    _noDelay = nodelay;
}

bool AsyncServer::getNoDelay(){
    return _noDelay;
}

uint8_t AsyncServer::status(){
    if (!_pcb) {
        return 0;
    }
    return _pcb->state;
}

int8_t AsyncServer::_s_accept(void * arg, tcp_pcb * pcb, int8_t err){
    return reinterpret_cast<AsyncServer*>(arg)->_accept(pcb, err);
}

int8_t AsyncServer::_s_accepted(void *arg, AsyncClient* client){
    return reinterpret_cast<AsyncServer*>(arg)->_accepted(client);
}
