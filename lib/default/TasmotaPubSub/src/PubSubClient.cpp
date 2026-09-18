/*
  PubSubClient.cpp - A simple client for MQTT.

  TasmotaPubSub - Tasmota fork of the PubSubClient MQTT library.

  Original author:
    Nick O'Leary - http://knolleary.net
  Tasmota fork maintained by Theo Arends and the Tasmota contributors.

  SPDX-FileCopyrightText: 2008-2020 Nicholas O'Leary
  SPDX-FileCopyrightText: 2020-2025 Theo Arends and Tasmota contributors

  SPDX-License-Identifier: MIT
*/

#include "PubSubClient.h"
#include "Arduino.h"

PubSubClient::PubSubClient() {
    this->_state = MQTT_DISCONNECTED;
    this->_client = NULL;
    this->stream = NULL;
    setCallback(NULL);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}

PubSubClient::PubSubClient(Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}

PubSubClient::PubSubClient(IPAddress addr, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr, port);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(IPAddress addr, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr,port);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(IPAddress addr, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr, port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(IPAddress addr, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(addr,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}

PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip, port);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip,port);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip, port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(uint8_t *ip, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(ip,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}

PubSubClient::PubSubClient(const char* domain, uint16_t port, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(const char* domain, uint16_t port, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(const char* domain, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setCallback(callback);
    setClient(client);
    this->stream = NULL;
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}
PubSubClient::PubSubClient(const char* domain, uint16_t port, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream) {
    this->_state = MQTT_DISCONNECTED;
    setServer(domain,port);
    setCallback(callback);
    setClient(client);
    setStream(stream);
    this->bufferSize = 0;
    setBufferSize(MQTT_MAX_PACKET_SIZE);
    setKeepAlive(MQTT_KEEPALIVE);
    setSocketTimeout(MQTT_SOCKET_TIMEOUT);
}

PubSubClient::~PubSubClient() {
  free(this->buffer);
}

boolean PubSubClient::connect(const char *id) {
    return connect(id,NULL,NULL,0,0,0,0,1);
}

boolean PubSubClient::connect(const char *id, const char *user, const char *pass) {
    return connect(id,user,pass,0,0,0,0,1);
}

boolean PubSubClient::connect(const char *id, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage) {
    return connect(id,NULL,NULL,willTopic,willQos,willRetain,willMessage,1);
}

boolean PubSubClient::connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage) {
    return connect(id,user,pass,willTopic,willQos,willRetain,willMessage,1);
}

boolean PubSubClient::connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage, boolean cleanSession) {
    if (!connected()) {
        int result = 0;

        if (_client == nullptr) {
            return false;
        }
        // Without a working buffer the CONNECT packet below would be assembled through
        // a null pointer. bufferSize == 0 means the allocation failed.
        if ((this->buffer == nullptr) || (this->bufferSize == 0)) {
            return false;
        }

        // A null client id would crash the later strnlen()/writeString(). MQTT allows a
        // zero-length id (with clean session) but not a null pointer.
        if (id == nullptr) {
            return false;
        }
        // Reject a Will with an out-of-range QoS before it is shifted into the CONNECT
        // flags byte, and a Will topic without a message.
        if (willTopic != nullptr) {
            if (willQos > 2 || willMessage == nullptr) {
                return false;
            }
        }

        if (_client->connected()) {
            result = 1;

// port == 0 means no server was configured, or setServer() failed to keep the host
// name (out of memory). Connecting anyway would target an unintended endpoint.
        } else if (this->port == 0) {
            _state = MQTT_CONNECT_FAILED;
            return false;

        } else {
            if (domain.length() != 0) {
                result = _client->connect(this->domain.c_str(), this->port);

            } else {
                result = _client->connect(this->ip, this->port);
            }
        }

        if (result == 1) {
            nextMsgId = 1;
            // Leave room in the buffer for header and variable length field
            uint16_t length = MQTT_MAX_HEADER_SIZE;
            unsigned int j;

#if MQTT_VERSION == MQTT_VERSION_3_1
            uint8_t d[9] = {0x00,0x06,'M','Q','I','s','d','p', MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 9
#elif MQTT_VERSION == MQTT_VERSION_3_1_1
            uint8_t d[7] = {0x00,0x04,'M','Q','T','T',MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 7
#endif
            for (j = 0;j<MQTT_HEADER_VERSION_LENGTH;j++) {
                this->buffer[length++] = d[j];
            }

            uint8_t v;
            if (willTopic) {
                v = 0x04|(willQos<<3)|(willRetain<<5);
            } else {
                v = 0x00;
            }
            if (cleanSession) {
                v = v|0x02;
            }

            if (user != NULL) {
                v = v|0x80;

                if (pass != NULL) {
                    v = v|(0x80>>1);
                }
            }
            this->buffer[length++] = v;

            this->buffer[length++] = ((this->keepAlive) >> 8);
            this->buffer[length++] = ((this->keepAlive) & 0xFF);

            CHECK_STRING_LENGTH(length,id)
            length = writeString(id,this->buffer,length);
            if (willTopic) {
                CHECK_STRING_LENGTH(length,willTopic)
                length = writeString(willTopic,this->buffer,length);
                CHECK_STRING_LENGTH(length,willMessage)
                length = writeString(willMessage,this->buffer,length);
            }

            if (user != NULL) {
                CHECK_STRING_LENGTH(length,user)
                length = writeString(user,this->buffer,length);
                if (pass != NULL) {
                    CHECK_STRING_LENGTH(length,pass)
                    length = writeString(pass,this->buffer,length);
                }
            }

            write(MQTTCONNECT,this->buffer,length-MQTT_MAX_HEADER_SIZE);

            lastInActivity = lastOutActivity = millis();

            while (!_client->available()) {
                delay(0);  // Prevent watchdog crashes

                unsigned long t = millis();
                if (t-lastInActivity >= ((int32_t) this->socketTimeout*1000UL)) {
                    _state = MQTT_CONNECTION_TIMEOUT;
                    _client->stop();
                    return false;
                }
            }
            uint8_t llen;
            uint32_t len = readPacket(&llen);

// Only accept a well-formed CONNACK: exact packet type (0x20), Remaining Length 2.
// Previously any 4-byte frame ending in 0 was treated as a successful connection.
            if (len == 4 && (buffer[0] == MQTTCONNACK) && (buffer[1] == 2)) {
                if (buffer[3] == 0) {
                    lastInActivity = millis();
                    pingOutstanding = false;
                    _state = MQTT_CONNECTED;
                    return true;
                } else {
                    _state = buffer[3];
                }
            }
            _client->stop();
        } else {
            _state = MQTT_CONNECT_FAILED;
        }
        return false;
    }
    return true;
}

// reads a byte into result
boolean PubSubClient::readByte(uint8_t * result) {
   if (_client == nullptr) {
     return false;
   }

   uint32_t previousMillis = millis();
   while (!_client->available()) {
     delay(1);  // Prevent watchdog crashes

     uint32_t currentMillis = millis();
     if (currentMillis - previousMillis >= ((int32_t) this->socketTimeout * 1000)) {
       return false;
     }
   }

// read() returns -1 when no byte could be retrieved even though available() was
// non-zero (reachable with TLS clients at record boundaries). Storing that as 0xFF
// would silently inject a bogus byte into the packet being parsed.
   int rc = _client->read();
   if (rc < 0) {
     return false;
   }
   *result = (uint8_t)rc;

   return true;
}

// reads a byte into result[*index] and increments index
boolean PubSubClient::readByte(uint8_t * result, uint16_t * index) {
  uint16_t current_index = *index;
  uint8_t * write_address = &(result[current_index]);
  if (readByte(write_address)) {
    *index = current_index + 1;
    return true;
  }
  return false;
}

uint32_t PubSubClient::readPacket(uint8_t* lengthLength) {
    uint16_t len = 0;

// The fixed header plus the Remaining Length field occupy up to MQTT_MAX_HEADER_SIZE
// bytes, which are always written to the front of the buffer. A buffer smaller than
// that cannot hold any packet header, so refuse instead of overrunning it.
    if ((this->buffer == nullptr) || (this->bufferSize < MQTT_MAX_HEADER_SIZE)) {
        _state = MQTT_DISCONNECTED;
        if (_client != nullptr) { _client->stop(); }
        return 0;
    }

    if (!readByte(this->buffer, &len)) { return 0; }
    bool isPublish = (this->buffer[0]&0xF0) == MQTTPUBLISH;
    uint32_t multiplier = 1;
    uint32_t length = 0;
    uint8_t digit = 0;
    uint16_t skip = 0;
    uint32_t start = 0;

    do {
        if (len == MQTT_MAX_HEADER_SIZE) {
            // Invalid remaining length encoding - kill the connection
            _state = MQTT_DISCONNECTED;
            _client->stop();
            return 0;
        }
        if (!readByte(&digit)) { return 0; }
        this->buffer[len++] = digit;
        length += (digit & 127) * multiplier;
        multiplier <<=7; //multiplier *= 128

// Remaining Length is 1-4 bytes; the `len == MQTT_MAX_HEADER_SIZE` check above is the
// bound. The previous extra `len < bufferSize - 2` condition truncated the decode of a
// legal multi-byte length on small buffers (and underflowed for bufferSize < 2).
    } while ((digit & 128) != 0);

    *lengthLength = len-1;

// DoS mitigation + sentinel byte:
// In non-stream mode, if the declared packet cannot fit the buffer while leaving
// at least one spare byte (needed by consumers that NUL-terminate at buffer[len]),
// close the connection immediately instead of draining the whole Remaining Length
// byte-by-byte. Draining a large or trickle-fed body would block the event loop.
    // total wire packet size = fixed header (1) + length bytes (llen) + Remaining Length
    uint32_t total_packet = (uint32_t)1 + (uint32_t)(*lengthLength) + length;

    // Configurable hard cap, independent of buffer allocation (0 = disabled).
    // Applies to both stream and non-stream mode: a larger packet is refused and the
    // connection closed rather than draining/streaming an unbounded body.
    if (this->maxIncomingPacketSize != 0 && total_packet > this->maxIncomingPacketSize) {
        _state = MQTT_DISCONNECTED;
        _client->stop();
        return 0;
    }

    // Non-stream: the packet must also fit the buffer, leaving one spare byte for a
    // downstream NUL terminator. Draining an oversized body would block the
    // event loop, so close immediately instead.
    if (!this->stream && total_packet >= (uint32_t)this->bufferSize) {
        _state = MQTT_DISCONNECTED;
        _client->stop();
        return 0;
    }

    if (isPublish) {
        // Read in topic length to calculate bytes to skip over for Stream writing
        if (!readByte(this->buffer, &len)) { return 0; }
        if (!readByte(this->buffer, &len)) { return 0; }
        skip = (this->buffer[*lengthLength+1]<<8)+this->buffer[*lengthLength+2];
        start = 2;
        if (this->buffer[0]&MQTTQOS1) {
            // skip message id
            skip += 2;
        }
    }
    uint32_t idx = len;

    for (uint32_t i = start;i<length;i++) {
        if (!readByte(&digit)) { return 0; }
        if (this->stream) {
            if (isPublish && idx-*lengthLength-2>skip) {
                this->stream->write(digit);
            }
        }

        if (len < this->bufferSize) {
            this->buffer[len] = digit;
            len++;
        }
        idx++;

// Periodically yield while consuming a large (typically streamed) body so the
// watchdog is not starved. Non-stream packets are < bufferSize so this rarely fires.
        if ((i & 0x3FF) == 0) { delay(0); }
    }

// Sentinel byte: use >= so an exact-buffer packet is also ignored, guaranteeing a spare byte for
// downstream NUL-termination at buffer[len]. Non-stream oversized packets are
// normally already rejected above; this is defense-in-depth.
    if (!this->stream && idx >= this->bufferSize) {
        len = 0; // This will cause the packet to be ignored.
    }
    return len;
}

boolean PubSubClient::loop() {
    if (connected()) {
        unsigned long t = millis();
// keepAlive == 0 disables the keepalive mechanism (as advertised by MQTT itself).
// Without the guard the interval collapses to 0 ms, so every loop() sent a PINGREQ and
// the next one tore the connection down with MQTT_CONNECTION_TIMEOUT.
        if (this->keepAlive &&
            ((t - lastInActivity > this->keepAlive*1000UL) || (t - lastOutActivity > this->keepAlive*1000UL))) {
            if (pingOutstanding) {
                this->_state = MQTT_CONNECTION_TIMEOUT;
                _client->stop();
                return false;
            } else {
                this->buffer[0] = MQTTPINGREQ;
                this->buffer[1] = 0;

                if (_client->write(this->buffer,2) != 0) {
                  lastOutActivity = t;
                  lastInActivity = t;
                }

                pingOutstanding = true;
            }
        }
        if (_client->available()) {
            uint8_t llen;
            uint16_t len = readPacket(&llen);
            uint16_t msgId = 0;
            uint8_t *payload;
            if (len > 0) {
                lastInActivity = t;
                uint8_t type = this->buffer[0]&0xF0;
                if (type == MQTTPUBLISH) {
                    if (callback) {
                        uint16_t tl = (this->buffer[llen+1]<<8)+this->buffer[llen+2]; /* topic length in bytes */

// Observed heap corruption in some cases since v10.0.0
// Also see https://github.com/knolleary/pubsubclient/pull/843
//
// Decode QoS from bits 1-2 and reject unsupported/illegal values
//       (2 = unsupported, 3 = protocol violation) before parsing.
// Validate the topic and (for QoS 1) the message-id bytes against the
//       number of bytes actually received (len), not just the buffer capacity.
//       This prevents out-of-bounds reads and payload-length underflow when a
//       broker sends a topic length that overruns the packet.
                        uint8_t qos = (this->buffer[0] & 0x06) >> 1;
                        if (qos > 1) {
                          _state = MQTT_DISCONNECTED;
                          _client->stop();
                          return false;
                        }

                        // Bytes required before the payload starts:
                        //   fixed header (1) + length bytes (llen) + topic length field (2)
                        //   + topic (tl) + message id for QoS 1 (2)
                        uint32_t header_len = (uint32_t)llen + 3 + tl + (qos ? 2 : 0);
                        // Topic must be non-empty, the header must fit within the received
                        // bytes, and (redundantly) within the buffer with room for the NUL.
                        if ((tl == 0) ||
                            (header_len > (uint32_t)len) ||
                            ((uint32_t)llen + 3 + tl > (uint32_t)this->bufferSize)) {
                          _state = MQTT_DISCONNECTED;
                          _client->stop();
                          return false;
                        }

                        memmove(this->buffer+llen+2,this->buffer+llen+3,tl); /* move topic inside buffer 1 byte to front */
                        this->buffer[llen+2+tl] = 0; /* end the topic as a 'C' string with \x00 */
                        char *topic = (char*) this->buffer+llen+2;
                        // msgId only present for QOS>0
                        if (qos == 1) {
                            msgId = (this->buffer[llen+3+tl]<<8)+this->buffer[llen+3+tl+1];
                            payload = this->buffer+llen+3+tl+2;
                            callback(topic,payload,len-llen-3-tl-2);

                            this->buffer[0] = MQTTPUBACK;
                            this->buffer[1] = 2;
                            this->buffer[2] = (msgId >> 8);
                            this->buffer[3] = (msgId & 0xFF);

                            if (_client->write(this->buffer,4) != 0) {
                              lastOutActivity = t;
                            }

                        } else {
                            payload = this->buffer+llen+3+tl;
                            callback(topic,payload,len-llen-3-tl);
                        }
                    }
                } else if (type == MQTTPINGREQ) {
                    this->buffer[0] = MQTTPINGRESP;
                    this->buffer[1] = 0;
                    _client->write(this->buffer,2);
                } else if (type == MQTTPINGRESP) {
                    pingOutstanding = false;
                }
            } else if (!connected()) {
                // readPacket has closed the connection
                return false;
            }
        }
        return true;
    }
    return false;
}

boolean PubSubClient::publish(const char* topic, const char* payload) {
    return publish(topic,(const uint8_t*)payload, payload ? strnlen(payload, this->bufferSize) : 0,false);
}

boolean PubSubClient::publish(const char* topic, const char* payload, boolean retained) {
    return publish(topic,(const uint8_t*)payload, payload ? strnlen(payload, this->bufferSize) : 0,retained);
}

boolean PubSubClient::publish(const char* topic, const uint8_t* payload, unsigned int plength) {
    return publish(topic, payload, plength, false);
}

boolean PubSubClient::publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained) {
    if (connected()) {
        if ((topic == nullptr) || ((payload == nullptr) && (plength != 0))) {
            return false;
        }
        if (this->bufferSize < MQTT_MAX_HEADER_SIZE + 2+strnlen(topic, this->bufferSize) + plength) {
            // Too long
            return false;
        }
        // Leave room in the buffer for header and variable length field
        uint16_t length = MQTT_MAX_HEADER_SIZE;
        length = writeString(topic,this->buffer,length);

        // Add payload
        uint16_t i;
        for (i=0;i<plength;i++) {
            this->buffer[length++] = payload[i];
        }

        // Write the header
        uint8_t header = MQTTPUBLISH;
        if (retained) {
            header |= 1;
        }
        return write(header,this->buffer,length-MQTT_MAX_HEADER_SIZE);
    }
    return false;
}

boolean PubSubClient::publish_P(const char* topic, const char* payload, boolean retained) {
    return publish_P(topic, (const uint8_t*)payload, payload ? strnlen(payload, this->bufferSize) : 0, retained);
}

boolean PubSubClient::publish_P(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained) {
    unsigned int rc = 0;

    if (!connected()) {
        return false;
    }

    if ((topic == nullptr) || ((payload == nullptr) && (plength != 0))) {
        return false;
    }
    uint16_t tlen = strnlen(topic, this->bufferSize);
    // Header + topic must fit the working buffer (payload itself is streamed, not buffered).
    if ((size_t)MQTT_MAX_HEADER_SIZE + 2 + tlen > this->bufferSize) {
        return false;
    }

    uint8_t header = MQTTPUBLISH;
    if (retained) {
        header |= 1;
    }

    // Assemble the topic behind the reserved header space, then let buildHeader() emit the
    // Remaining Length in front of it. Sharing the single encoder keeps the framing
    // identical to publish()/beginPublish() and enforces its 4-byte bound in one place.
    uint16_t length = writeString(topic, this->buffer, MQTT_MAX_HEADER_SIZE);
    size_t hlen = buildHeader(header, this->buffer, (uint32_t)plength + 2 + tlen);
    if (hlen == 0) {
        return false;
    }
    uint16_t headerLength = length - (MQTT_MAX_HEADER_SIZE - hlen);

    rc += _client->write(this->buffer + (MQTT_MAX_HEADER_SIZE - hlen), headerLength);

    for (unsigned int i = 0; i < plength; i++) {
        rc += _client->write((char)pgm_read_byte_near(payload + i));
    }

    if (rc > 0) {
      lastOutActivity = millis();
    }

    return (rc == (unsigned int)headerLength + plength);
}

boolean PubSubClient::beginPublish(const char* topic, unsigned int plength, boolean retained) {
    if (connected()) {
        if (topic == nullptr) {
            return false;
        }
        // Header + topic must fit the working buffer (writeString starts at MQTT_MAX_HEADER_SIZE).
        size_t tlen = strnlen(topic, this->bufferSize);
        if ((size_t)MQTT_MAX_HEADER_SIZE + 2 + tlen > this->bufferSize) {
            return false;
        }
        // Remaining Length now uses 32-bit accounting (buildHeader emits up to 4 bytes),
        // so payloads above 65535 bytes are framed correctly instead of being truncated.
        uint32_t remaining_length = (uint32_t)plength + 2 + tlen;

        // Send the header and variable length field
        uint16_t length = MQTT_MAX_HEADER_SIZE;
        length = writeString(topic,this->buffer,length);
        uint8_t header = MQTTPUBLISH;
        if (retained) {
            header |= 1;
        }
        size_t hlen = buildHeader(header, this->buffer, remaining_length);

        if (hlen == 0) {   // Remaining Length above 268435455, nothing has been sent yet
            return false;
        }

        uint16_t rc = _client->write(this->buffer+(MQTT_MAX_HEADER_SIZE-hlen),length-(MQTT_MAX_HEADER_SIZE-hlen));

        if (rc > 0) {
           lastOutActivity = millis();
        }

        return (rc == (length-(MQTT_MAX_HEADER_SIZE-hlen)));
    }
    return false;
}

int PubSubClient::endPublish() {
 return 1;
}

size_t PubSubClient::write(uint8_t data) {
    if (_client == nullptr) {
        lastOutActivity = millis();
        return 0;
    }
    size_t rc = _client->write(data);
    if (rc != 0) {
        lastOutActivity = millis();
    }
    return rc;
}

size_t PubSubClient::write(const uint8_t *buffer, size_t size) {
    if (_client == nullptr) {
        lastOutActivity = millis();
        return 0;
    }
    size_t rc = _client->write(buffer,size);
    if (rc != 0) {
        lastOutActivity = millis();
    }
    return rc;
}

// length is the MQTT Remaining Length and must be 32-bit: encoded as 1-4 bytes
// (max 268435455). A uint16_t parameter previously truncated payloads > 65535,
// desynchronising the connection.
size_t PubSubClient::buildHeader(uint8_t header, uint8_t* buf, uint32_t length) {
    uint8_t lenBuf[4];
    uint8_t llen = 0;
    uint8_t digit;
    uint8_t pos = 0;
    uint32_t len = length;
    do {
        digit = len  & 127; //digit = len %128
        len >>= 7; //len = len / 128
        if (len > 0) {
            digit |= 0x80;
        }
        lenBuf[pos++] = digit;
        llen++;
// Remaining Length is encoded in at most 4 bytes; bound the loop to the lenBuf[4]
// size so an out-of-range length can never overflow lenBuf[] or underflow buf[4-llen].
    } while (len>0 && llen<4);

// A length above 268435455 does not fit the 4-byte encoding. Emitting the truncated
// value would desynchronise the connection, so report failure and let the caller
// abort instead.
    if (len > 0) {
        return 0;
    }

    buf[4-llen] = header;
    for (int i=0;i<llen;i++) {
        buf[MQTT_MAX_HEADER_SIZE-llen+i] = lenBuf[i];
    }
    return llen+1; // Full header size is variable length bit plus the 1-byte fixed header
}

boolean PubSubClient::write(uint8_t header, uint8_t* buf, uint16_t length) {
    uint16_t rc;
    uint8_t hlen = buildHeader(header, buf, length);

    if (hlen == 0) {   // Remaining Length could not be encoded, send nothing
        return false;
    }

#ifdef MQTT_MAX_TRANSFER_SIZE
    uint8_t* writeBuf = buf+(MQTT_MAX_HEADER_SIZE-hlen);
    uint16_t bytesRemaining = length+hlen;  //Match the length type
    uint8_t bytesToWrite;
    boolean result = true;
    while ((bytesRemaining > 0) && result) {
        bytesToWrite = (bytesRemaining > MQTT_MAX_TRANSFER_SIZE)?MQTT_MAX_TRANSFER_SIZE:bytesRemaining;
        rc = _client->write(writeBuf,bytesToWrite);
        result = (rc == bytesToWrite);
        bytesRemaining -= rc;
        writeBuf += rc;
    }
    return result;
#else
    rc = _client->write(buf+(MQTT_MAX_HEADER_SIZE-hlen),length+hlen);

    if (rc != 0) {
        lastOutActivity = millis();
    }

    return (rc == hlen+length);
#endif
}

boolean PubSubClient::subscribe(const char* topic) {
    return subscribe(topic, 0);
}

boolean PubSubClient::subscribe(const char* topic, uint8_t qos) {
// Check for null before calling strnlen(). The SUBSCRIBE packet also writes a
// trailing QoS byte, so it needs one more byte than UNSUBSCRIBE: the buffer must
// hold header(5) + msgId(2) + topic-length(2) + topic + qos(1) = 10 + topicLength.
    if (topic == nullptr) {
        return false;
    }
    size_t topicLength = strnlen(topic, this->bufferSize);
    if (qos > 1) {
        return false;
    }
    if (this->bufferSize < 10 + topicLength) {
        // Too long
        return false;
    }
    if (connected()) {
        // Leave room in the buffer for header and variable length field
        uint16_t length = MQTT_MAX_HEADER_SIZE;
        nextMsgId++;
        if (nextMsgId == 0) {
            nextMsgId = 1;
        }
        this->buffer[length++] = (nextMsgId >> 8);
        this->buffer[length++] = (nextMsgId & 0xFF);
        length = writeString((char*)topic, this->buffer,length);
        this->buffer[length++] = qos;
        return write(MQTTSUBSCRIBE|MQTTQOS1,this->buffer,length-MQTT_MAX_HEADER_SIZE);
    }
    return false;
}

boolean PubSubClient::unsubscribe(const char* topic) {
// Check for null before calling strnlen().
    if (topic == nullptr) {
        return false;
    }
    size_t topicLength = strnlen(topic, this->bufferSize);
    if (this->bufferSize < 9 + topicLength) {
        // Too long
        return false;
    }
    if (connected()) {
        uint16_t length = MQTT_MAX_HEADER_SIZE;
        nextMsgId++;
        if (nextMsgId == 0) {
            nextMsgId = 1;
        }
        this->buffer[length++] = (nextMsgId >> 8);
        this->buffer[length++] = (nextMsgId & 0xFF);
        length = writeString(topic, this->buffer,length);
        return write(MQTTUNSUBSCRIBE|MQTTQOS1,this->buffer,length-MQTT_MAX_HEADER_SIZE);
    }
    return false;
}

void PubSubClient::disconnect(bool disconnect_package) {
    if (_client != nullptr) {
      if (disconnect_package && (this->buffer != nullptr) && (this->bufferSize >= 2)) {
        this->buffer[0] = MQTTDISCONNECT;
        this->buffer[1] = 0;
        _client->write(this->buffer,2);
      }
      _client->flush();
      _client->stop();
    }
    _state = MQTT_DISCONNECTED;
    // A stale outstanding ping would otherwise time out the next connection immediately
    pingOutstanding = false;

    lastInActivity = lastOutActivity = millis();
}

uint16_t PubSubClient::writeString(const char* string, uint8_t* buf, uint16_t pos) {
// Defense in depth: every caller already validates that the string fits, but this
// function used to walk the string to its NUL with no bound at all. Verify the 2-byte
// length prefix plus the payload fit the buffer and leave `pos` untouched otherwise,
// so a mistake in a caller can no longer corrupt the heap.
    if (string == nullptr) {
        return pos;
    }
    size_t slen = strnlen(string, this->bufferSize);
    if ((slen > 0xFFFF) || ((size_t)pos + 2 + slen > (size_t)this->bufferSize)) {
        return pos;
    }
    buf[pos++] = (uint8_t)(slen >> 8);
    buf[pos++] = (uint8_t)(slen & 0xFF);
    memcpy(buf + pos, string, slen);
    return pos + slen;
}

boolean PubSubClient::connected() {
    boolean rc;

// A client without a working buffer cannot be considered connected: every packet
// exchange goes through `buffer`, so treat a failed allocation as disconnected.
    if ((_client == NULL) || (this->buffer == NULL)) {
        this->_state = MQTT_DISCONNECTED;

        rc = false;
    } else {
        rc = (int)_client->connected();
        if (!rc) {
            if (this->_state == MQTT_CONNECTED) {
                this->_state = MQTT_CONNECTION_LOST;
                _client->flush();
                _client->stop();

                pingOutstanding = false;
            }
        } else {
            return this->_state == MQTT_CONNECTED;
        }
    }
    return rc;
}

PubSubClient& PubSubClient::setServer(uint8_t * ip, uint16_t port) {
    IPAddress addr(ip[0],ip[1],ip[2],ip[3]);
    return setServer(addr,port);
}

PubSubClient& PubSubClient::setServer(IPAddress ip, uint16_t port) {
    this->ip = ip;
    this->port = port;

    this->domain = "";

    return *this;
}

PubSubClient& PubSubClient::setServer(const char * domain, uint16_t port) {
// A null host, or a String assignment that ran out of memory, used to leave an empty
// domain behind - connect() would then silently fall back to the (unrelated) IP
// address. Clear the port instead so connect() fails cleanly.
    this->domain = "";
    if (domain != nullptr) {
        this->domain = domain;
        if (this->domain.length() != strlen(domain)) {   // out of memory
            this->domain = "";
            this->port = 0;
            return *this;
        }
    }
    this->port = port;

    return *this;
}

PubSubClient& PubSubClient::setCallback(MQTT_CALLBACK_SIGNATURE) {
    this->callback = callback;
    return *this;
}

PubSubClient& PubSubClient::setClient(Client& client) {
    this->_client = &client;
    return *this;
}

PubSubClient& PubSubClient::setStream(Stream& stream) {
    this->stream = &stream;
    return *this;
}

int PubSubClient::state() const {
    return this->_state;
}

boolean PubSubClient::setBufferSize(uint16_t size) {
    if (size == 0) {
        // Cannot set it back to 0
        return false;
    }
// Commit bufferSize only after a successful (re)allocation. Previously bufferSize
// was set even when malloc() failed, leaving the object reporting a non-zero
// capacity while buffer == nullptr, which later operations would dereference.
    if (this->bufferSize == 0) {
        uint8_t* newBuffer = (uint8_t*)malloc(size);
        if (newBuffer == NULL) {
            return false;
        }
        this->buffer = newBuffer;
    } else {
        uint8_t* newBuffer = (uint8_t*)realloc(this->buffer, size);
        if (newBuffer == NULL) {
            return false;
        }
        this->buffer = newBuffer;
    }
    this->bufferSize = size;
    return true;
}

uint16_t PubSubClient::getBufferSize() const {
    return this->bufferSize;
}

PubSubClient& PubSubClient::setKeepAlive(uint16_t keepAlive) {
    this->keepAlive = keepAlive;
    return *this;
}
PubSubClient& PubSubClient::setSocketTimeout(uint16_t timeout) {
    this->socketTimeout = timeout;
    return *this;
}
