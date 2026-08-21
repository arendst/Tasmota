/*
  MockClient.h - Host-side scriptable mock transport (Mock_Transport) for the
  TasmotaPubSub host test system.

  MockClient subclasses the abstract Arduino shim `Client` so the *unmodified*
  PubSubClient library drives it exactly as it would a real `WiFiClient`. It
  provides:
    - a scriptable inbound byte queue the library reads from,
    - an outbound record capturing every byte the library writes,
    - controllable connection state / connect() results and close tracking.

  This file implements the task 6.1 "core" only. Fault injection
  (partial-write via setWriteLimit and slow/trickle delivery via setTrickle)
  is added later in task 6.2; the read/available and write paths below are
  intentionally structured with single choke points (revealedCount() and the
  write helpers) so that behavior can be layered in without reshaping the core.
*/

#ifndef TASMOTA_PUBSUB_TEST_MOCK_CLIENT_H
#define TASMOTA_PUBSUB_TEST_MOCK_CLIENT_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "Client.h"
#include "IPAddress.h"

// MqttPacket is only forward-declared here to keep this header light; the
// pushPacket() overload is implemented in MockClient.cpp, which includes the
// full MqttPacket definition (task 7.1).
class MqttPacket;

class MockClient : public Client {
public:
    MockClient();
    ~MockClient() override = default;

    // --- Inbound scripting -------------------------------------------------
    // Append raw bytes to the inbound queue the library reads via read().
    void pushInbound(const std::vector<uint8_t>& bytes);
    // Append a built MQTT packet's exact wire bytes to the inbound queue.
    void pushPacket(const MqttPacket& p);
    // Drop any not-yet-consumed inbound bytes and reset the read cursor.
    void clearInbound();

    // --- Outbound record ---------------------------------------------------
    // Every byte the library writes is appended here for structural assertion.
    const std::vector<uint8_t>& outbound() const;
    void clearOutbound();

    // --- Fault injection (task 6.2) ----------------------------------------
    // Partial-write injection (Requirement 6.4). When maxPerWrite is nonzero, a
    // single write(buf,size) accepts, records and returns at most maxPerWrite
    // bytes (but always >= 1 for a successful write with size >= 1), modelling
    // WiFiClientSecure_light's short writes. Only the accepted prefix is
    // appended to the outbound record; repeated writes keep making progress.
    // A limit of 0 means "no limit" (accept all) - the core behavior. Used by F-05.
    void setWriteLimit(size_t maxPerWrite);

    // Trickle delivery (Requirement 6.5). When bytesPerReveal is nonzero,
    // available() reveals at most bytesPerReveal bytes immediately and
    // bytesPerReveal more for every msPerReveal of virtual time that elapses
    // (measured against TestClock) since this schedule was configured. This
    // models a slow broker and drives the F-03 per-byte-timeout vs
    // packet-deadline distinction. A bytesPerReveal of 0 means "reveal
    // everything" - the core behavior.
    void setTrickle(size_t bytesPerReveal, unsigned long msPerReveal);

    // --- Connection control ------------------------------------------------
    // Force the reported connection state.
    void setConnected(bool connected);
    // Script the value returned by the next connect() call (default 1 = ok).
    void setConnectResult(int result);
    // True once stop() has been called at least once (F-03 / F-06 closes).
    bool stopCalled() const;
    // Introspection helpers for connect()/close/flush bookkeeping.
    unsigned    stopCount() const;
    bool        flushCalled() const;
    unsigned    flushCount() const;
    bool        connectCalled() const;
    const std::string& lastHost() const;   // set by connect(const char*, ...)
    IPAddress   lastIp() const;             // set by connect(IPAddress, ...)
    uint16_t    lastPort() const;

    // --- Client interface (see Client.h) -----------------------------------
    int    connect(IPAddress ip, uint16_t port) override;
    int    connect(const char* host, uint16_t port) override;
    size_t write(uint8_t b) override;
    size_t write(const uint8_t* buf, size_t size) override;
    int    available() override;
    int    read() override;
    int    read(uint8_t* buf, size_t size) override;
    int    peek() override;
    void   flush() override;
    void   stop() override;
    uint8_t connected() override;
    operator bool() override;

private:
    // Number of inbound bytes currently visible to the library. With no fault
    // injection this is simply every unconsumed byte; task 6.2's trickle mode
    // overrides this single choke point to reveal bytes gradually.
    size_t revealedCount() const;

    // Inbound queue and read cursor. Consumed bytes stay in _inbound; _readPos
    // marks the boundary so accessors remain cheap and index math is obvious.
    std::vector<uint8_t> _inbound;
    size_t               _readPos;

    // Outbound capture.
    std::vector<uint8_t> _outbound;

    // Fault injection (task 6.2). All zero == disabled == core behavior.
    size_t        _writeLimit;             // max bytes accepted per write() (0 = unlimited)
    size_t        _trickleBytesPerReveal;  // bytes revealed per step (0 = reveal all)
    unsigned long _trickleMsPerReveal;     // virtual ms between reveal steps
    unsigned long _trickleBaseMs;          // virtual time the trickle schedule started

    // Connection bookkeeping.
    bool        _connected;
    int         _connectResult;
    bool        _connectCalled;
    std::string _lastHost;
    IPAddress   _lastIp;
    uint16_t    _lastPort;

    // Close / flush tracking.
    bool     _stopCalled;
    unsigned _stopCount;
    bool     _flushCalled;
    unsigned _flushCount;
};

#endif  // TASMOTA_PUBSUB_TEST_MOCK_CLIENT_H
