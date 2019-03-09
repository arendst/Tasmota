#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <Arduino.h>
#include <Client.h>
#include <Stream.h>

extern "C" {
#include "lwmqtt/lwmqtt.h"
};

typedef struct {
  uint32_t end;
} lwmqtt_arduino_timer_t;

void lwmqtt_arduino_timer_set(void *ref, uint32_t timeout);

int32_t lwmqtt_arduino_timer_get(void *ref);

typedef struct {
  Client *client;
} lwmqtt_arduino_network_t;

void lwmqtt_arduino_timer_set(void *ref, uint32_t timeout) {
  // cast timer reference
  auto t = (lwmqtt_arduino_timer_t *)ref;

  // set future end time
  t->end = (uint32_t)(millis() + timeout);
}

int32_t lwmqtt_arduino_timer_get(void *ref) {
  // cast timer reference
  auto t = (lwmqtt_arduino_timer_t *)ref;

  // get difference to end time
  return (int32_t)t->end - (int32_t)millis();
}

lwmqtt_err_t lwmqtt_arduino_network_read(void *ref, uint8_t *buffer, size_t len, size_t *read, uint32_t timeout) {
  // cast network reference
  auto n = (lwmqtt_arduino_network_t *)ref;

  // set timeout
  n->client->setTimeout(timeout);

  // read bytes
  *read = n->client->readBytes(buffer, len);
  if (*read <= 0) {
    return LWMQTT_NETWORK_FAILED_READ;
  }

  return LWMQTT_SUCCESS;
}

lwmqtt_err_t lwmqtt_arduino_network_write(void *ref, uint8_t *buffer, size_t len, size_t *sent, uint32_t /*timeout*/) {
  // cast network reference
  auto n = (lwmqtt_arduino_network_t *)ref;

  // write bytes
  *sent = n->client->write(buffer, len);
  if (*sent <= 0) {
    return LWMQTT_NETWORK_FAILED_WRITE;
  };

  return LWMQTT_SUCCESS;
}

class MQTTClient;

typedef void (*MQTTClientCallbackSimple)(String &topic, String &payload);
typedef void (*MQTTClientCallbackAdvanced)(MQTTClient *client, char topic[], char bytes[], int length);

typedef struct {
  MQTTClient *client = nullptr;
  MQTTClientCallbackSimple simple = nullptr;
  MQTTClientCallbackAdvanced advanced = nullptr;
} MQTTClientCallback;

static void MQTTClientHandler(lwmqtt_client_t * /*client*/, void *ref, lwmqtt_string_t topic,
                              lwmqtt_message_t message) {
  // get callback
  auto cb = (MQTTClientCallback *)ref;

  // null terminate topic
  char terminated_topic[topic.len + 1];
  memcpy(terminated_topic, topic.data, topic.len);
  terminated_topic[topic.len] = '\0';

  // null terminate payload if available
  if (message.payload != nullptr) {
    message.payload[message.payload_len] = '\0';
  }

  // call the advanced callback and return if available
  if (cb->advanced != nullptr) {
    cb->advanced(cb->client, terminated_topic, (char *)message.payload, (int)message.payload_len);
    return;
  }

  // return if simple callback is not set
  if (cb->simple == nullptr) {
    return;
  }

  // create topic string
  String str_topic = String(terminated_topic);

  // create payload string
  String str_payload;
  if (message.payload != nullptr) {
    str_payload = String((const char *)message.payload);
  }

  // call simple callback
  cb->simple(str_topic, str_payload);
}

class MQTTClient {
 private:
  size_t bufSize = 0;
  uint8_t *readBuf = nullptr;
  uint8_t *writeBuf = nullptr;

  uint16_t keepAlive = 10;
  bool cleanSession = true;
  uint32_t timeout = 1000;

  Client *netClient = nullptr;
  const char *hostname = nullptr;
  int port = 0;
  lwmqtt_will_t *will = nullptr;
  MQTTClientCallback callback;

  lwmqtt_arduino_network_t network = {nullptr};
  lwmqtt_arduino_timer_t timer1 = {0};
  lwmqtt_arduino_timer_t timer2 = {0};
  lwmqtt_client_t client = {0};

  bool _connected = false;
  lwmqtt_return_code_t _returnCode = (lwmqtt_return_code_t)0;
  lwmqtt_err_t _lastError = (lwmqtt_err_t)0;

 public:
  explicit MQTTClient(int bufSize = 128) {
    // reset client
    memset(&this->client, 0, sizeof(lwmqtt_client_t));

    // allocate buffers
    this->bufSize = (size_t)bufSize;
    this->readBuf = (uint8_t *)malloc((size_t)bufSize + 1);
    this->writeBuf = (uint8_t *)malloc((size_t)bufSize);
  }

  ~MQTTClient() {
    // free will
    this->clearWill();

    // free hostname
    if (this->hostname != nullptr) {
      free((void *)this->hostname);
    }

    // free buffers
    free(this->readBuf);
    free(this->writeBuf);
  }

  void begin(const char hostname[], Client &client) { this->begin(hostname, 1883, client); }

  void begin(const char hostname[], int port, Client &client) {
    // set hostname and port
    this->setHost(hostname, port);

    // set client
    this->netClient = &client;

    // initialize client
    lwmqtt_init(&this->client, this->writeBuf, this->bufSize, this->readBuf, this->bufSize);

    // set timers
    lwmqtt_set_timers(&this->client, &this->timer1, &this->timer2, lwmqtt_arduino_timer_set, lwmqtt_arduino_timer_get);

    // set network
    lwmqtt_set_network(&this->client, &this->network, lwmqtt_arduino_network_read, lwmqtt_arduino_network_write);

    // set callback
    lwmqtt_set_callback(&this->client, (void *)&this->callback, MQTTClientHandler);
  }

  void onMessage(MQTTClientCallbackSimple cb) {
    // set callback
    this->callback.client = this;
    this->callback.simple = cb;
    this->callback.advanced = nullptr;
  }

  void onMessageAdvanced(MQTTClientCallbackAdvanced cb) {
    // set callback
    this->callback.client = this;
    this->callback.simple = nullptr;
    this->callback.advanced = cb;
  }

  void setHost(const char hostname[]) { this->setHost(hostname, 1883); }

  void setHost(const char hostname[], int port) {
    // free hostname if set
    if (this->hostname != nullptr) {
      free((void *)this->hostname);
    }

    // set hostname and port
    this->hostname = strdup(hostname);
    this->port = port;
  }

  void setWill(const char topic[]) { this->setWill(topic, ""); }

  void setWill(const char topic[], const char payload[]) { this->setWill(topic, payload, false, 0); }

  void setWill(const char topic[], const char payload[], bool retained, int qos) {
    // return if topic is missing
    if (topic == nullptr || strlen(topic) == 0) {
      return;
    }

    // clear existing will
    this->clearWill();

    // allocate will
    this->will = (lwmqtt_will_t *)malloc(sizeof(lwmqtt_will_t));
    memset(this->will, 0, sizeof(lwmqtt_will_t));

    // set topic
    this->will->topic = lwmqtt_string(strdup(topic));

    // set payload if available
    if (payload != nullptr && strlen(payload) > 0) {
      this->will->payload = lwmqtt_string(strdup(payload));
    }

    // set flags
    this->will->retained = retained;
    this->will->qos = (lwmqtt_qos_t)qos;
  }

  void clearWill() {
    // return if not set
    if (this->will == nullptr) {
      return;
    }

    // free payload if set
    if (this->will->payload.len > 0) {
      free(this->will->payload.data);
    }

    // free topic if set
    if (this->will->topic.len > 0) {
      free(this->will->topic.data);
    }

    // free will
    free(this->will);
    this->will = nullptr;
  }

  void setOptions(int keepAlive, bool cleanSession, int timeout) {
    // set new options
    this->keepAlive = (uint16_t)keepAlive;
    this->cleanSession = cleanSession;
    this->timeout = (uint32_t)timeout;
  }

  bool connect(const char clientId[], bool skip = false) { return this->connect(clientId, nullptr, nullptr); }

  bool connect(const char clientId[], const char username[], bool skip = false) { return this->connect(clientId, username, nullptr); }

  bool connect(const char clientId[], const char username[], const char password[], bool skip = false) {
    // close left open connection if still connected
    if (!skip && this->connected()) {
      this->close();
    }

    // save client
    this->network.client = this->netClient;

    // connect to hostg
    if(!skip) {
      int ret = this->netClient->connect(this->hostname, (uint16_t)this->port);
      if (ret <= 0) {
        return false;
      }
    }

    // prepare options
    lwmqtt_options_t options = lwmqtt_default_options;
    options.keep_alive = this->keepAlive;
    options.clean_session = this->cleanSession;
    options.client_id = lwmqtt_string(clientId);

    // set username and password if available
    if (username != nullptr) {
      options.username = lwmqtt_string(username);

      if (password != nullptr) {
        options.password = lwmqtt_string(password);
      }
    }

    // connect to broker
    this->_lastError = lwmqtt_connect(&this->client, options, this->will, &this->_returnCode, this->timeout);
    if (this->_lastError != LWMQTT_SUCCESS) {
      // close connection
      this->close();

      return false;
    }

    // set flag
    this->_connected = true;

    return true;
  }

  bool publish(const String &topic) { return this->publish(topic.c_str(), ""); }

  bool publish(const char topic[]) { return this->publish(topic, ""); }

  bool publish(const String &topic, const String &payload) { return this->publish(topic.c_str(), payload.c_str()); }

  bool publish(const String &topic, const String &payload, bool retained, int qos) {
    return this->publish(topic.c_str(), payload.c_str(), retained, qos);
  }

  bool publish(const char topic[], const String &payload) { return this->publish(topic, payload.c_str()); }

  bool publish(const char topic[], const String &payload, bool retained, int qos) {
    return this->publish(topic, payload.c_str(), retained, qos);
  }

  bool publish(const char topic[], const char payload[]) {
    return this->publish(topic, (char *)payload, (int)strlen(payload));
  }

  bool publish(const char topic[], const char payload[], bool retained, int qos) {
    return this->publish(topic, (char *)payload, (int)strlen(payload), retained, qos);
  }

  bool publish(const char topic[], const char payload[], int length) {
    return this->publish(topic, payload, length, false, 0);
  }

  bool publish(const char topic[], const char payload[], int length, bool retained, int qos) {
    // return immediately if not connected
    if (!this->connected()) {
      return false;
    }

    // prepare message
    lwmqtt_message_t message = lwmqtt_default_message;
    message.payload = (uint8_t *)payload;
    message.payload_len = (size_t)length;
    message.retained = retained;
    message.qos = lwmqtt_qos_t(qos);

    // publish message
    this->_lastError = lwmqtt_publish(&this->client, lwmqtt_string(topic), message, this->timeout);
    if (this->_lastError != LWMQTT_SUCCESS) {
      // close connection
      this->close();

      return false;
    }

    return true;
  }

  bool subscribe(const String &topic) { return this->subscribe(topic.c_str()); }

  bool subscribe(const String &topic, int qos) { return this->subscribe(topic.c_str(), qos); }

  bool subscribe(const char topic[]) { return this->subscribe(topic, 0); }

  bool subscribe(const char topic[], int qos) {
    // return immediately if not connected
    if (!this->connected()) {
      return false;
    }

    // subscribe to topic
    this->_lastError = lwmqtt_subscribe_one(&this->client, lwmqtt_string(topic), (lwmqtt_qos_t)qos, this->timeout);
    if (this->_lastError != LWMQTT_SUCCESS) {
      // close connection
      this->close();

      return false;
    }

    return true;
  }

  bool unsubscribe(const String &topic) { return this->unsubscribe(topic.c_str()); }

  bool unsubscribe(const char topic[]) {
    // return immediately if not connected
    if (!this->connected()) {
      return false;
    }

    // unsubscribe from topic
    this->_lastError = lwmqtt_unsubscribe_one(&this->client, lwmqtt_string(topic), this->timeout);
    if (this->_lastError != LWMQTT_SUCCESS) {
      // close connection
      this->close();

      return false;
    }

    return true;
  }

  bool loop() {
    // return immediately if not connected
    if (!this->connected()) {
      return false;
    }

    // get available bytes on the network
    auto available = (size_t)this->netClient->available();

    // yield if data is available
    if (available > 0) {
      this->_lastError = lwmqtt_yield(&this->client, available, this->timeout);
      if (this->_lastError != LWMQTT_SUCCESS) {
        // close connection
        this->close();

        return false;
      }
    }

    // keep the connection alive
    this->_lastError = lwmqtt_keep_alive(&this->client, this->timeout);
    if (this->_lastError != LWMQTT_SUCCESS) {
      // close connection
      this->close();

      return false;
    }

    return true;
  }

  bool connected() {
    // a client is connected if the network is connected, a client is available and
    // the connection has been properly initiated
    return this->netClient != nullptr && this->netClient->connected() == 1 && this->_connected;
  }

  lwmqtt_err_t lastError() { return this->_lastError; }

  lwmqtt_return_code_t returnCode() { return this->_returnCode; }

  bool disconnect() {
    // return immediately if not connected anymore
    if (!this->connected()) {
      return false;
    }

    // cleanly disconnect
    this->_lastError = lwmqtt_disconnect(&this->client, this->timeout);

    // close
    this->close();

    return this->_lastError == LWMQTT_SUCCESS;
  }

 private:
  void close() {
    // set flag
    this->_connected = false;

    // close network
    this->netClient->stop();
  }
};

#endif
