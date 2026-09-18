/*
  MockClient.cpp - Implementation of the host-side scriptable mock transport.

  Task 6.1 core: inbound queue, outbound record, connection state and close
  tracking. Fault injection (setWriteLimit / setTrickle) is deliberately absent
  here and is added in task 6.2 at the revealedCount() / write() choke points.
*/

#include "MockClient.h"

#include "MqttPacket.h"
#include "TestClock.h"

MockClient::MockClient()
    : _inbound(),
      _readPos(0),
      _outbound(),
      _writeLimit(0),
      _trickleBytesPerReveal(0),
      _trickleMsPerReveal(0),
      _trickleBaseMs(0),
      _connected(false),
      _connectResult(1),
      _connectCalled(false),
      _lastHost(),
      _lastIp(),
      _lastPort(0),
      _stopCalled(false),
      _stopCount(0),
      _flushCalled(false),
      _flushCount(0) {}

// --- Inbound scripting -----------------------------------------------------

void MockClient::pushInbound(const std::vector<uint8_t>& bytes) {
    _inbound.insert(_inbound.end(), bytes.begin(), bytes.end());
}

void MockClient::pushPacket(const MqttPacket& p) {
    // Append the built packet's exact wire bytes to the inbound queue so the
    // library reads them via read()/available() just like any scripted bytes.
    pushInbound(p.bytes());
}

void MockClient::clearInbound() {
    _inbound.clear();
    _readPos = 0;
}

// --- Outbound record -------------------------------------------------------

const std::vector<uint8_t>& MockClient::outbound() const {
    return _outbound;
}

void MockClient::clearOutbound() {
    _outbound.clear();
}

// --- Fault injection -------------------------------------------------------

void MockClient::setWriteLimit(size_t maxPerWrite) {
    _writeLimit = maxPerWrite;
}

void MockClient::setTrickle(size_t bytesPerReveal, unsigned long msPerReveal) {
    _trickleBytesPerReveal = bytesPerReveal;
    _trickleMsPerReveal = msPerReveal;
    // Anchor the reveal schedule at "now" so bytesPerReveal are visible
    // immediately and additional bytes appear as virtual time advances.
    _trickleBaseMs = TestClock::instance().millis();
}

// --- Connection control ----------------------------------------------------

void MockClient::setConnected(bool connected) {
    _connected = connected;
}

void MockClient::setConnectResult(int result) {
    _connectResult = result;
}

bool MockClient::stopCalled() const {
    return _stopCalled;
}

unsigned MockClient::stopCount() const {
    return _stopCount;
}

bool MockClient::flushCalled() const {
    return _flushCalled;
}

unsigned MockClient::flushCount() const {
    return _flushCount;
}

bool MockClient::connectCalled() const {
    return _connectCalled;
}

const std::string& MockClient::lastHost() const {
    return _lastHost;
}

IPAddress MockClient::lastIp() const {
    return _lastIp;
}

uint16_t MockClient::lastPort() const {
    return _lastPort;
}

// --- Client interface ------------------------------------------------------

int MockClient::connect(IPAddress ip, uint16_t port) {
    _connectCalled = true;
    _lastIp = ip;
    _lastPort = port;
    // Model a real client: a successful connect brings the socket up. Tests can
    // still override the reported state with setConnected().
    if (_connectResult == 1) {
        _connected = true;
    }
    return _connectResult;
}

int MockClient::connect(const char* host, uint16_t port) {
    _connectCalled = true;
    _lastHost = (host != nullptr) ? host : "";
    _lastPort = port;
    if (_connectResult == 1) {
        _connected = true;
    }
    return _connectResult;
}

size_t MockClient::write(uint8_t b) {
    _outbound.push_back(b);
    return 1;
}

size_t MockClient::write(const uint8_t* buf, size_t size) {
    if (buf == nullptr) {
        return 0;
    }
    // Partial-write injection (task 6.2): when a write limit is configured,
    // accept at most _writeLimit bytes of this call. Because _writeLimit >= 1
    // whenever it is set, a nonempty write always makes >= 1 byte of progress
    // (matching WiFiClientSecure_light). Only the accepted prefix is recorded,
    // so repeated writes continue advancing through the caller's buffer. A
    // limit of 0 preserves the core "accept everything" behavior.
    size_t accepted = size;
    if (_writeLimit != 0 && accepted > _writeLimit) {
        accepted = _writeLimit;
    }
    _outbound.insert(_outbound.end(), buf, buf + accepted);
    return accepted;
}

int MockClient::available() {
    return static_cast<int>(revealedCount());
}

int MockClient::read() {
    if (revealedCount() == 0) {
        return -1;
    }
    return static_cast<int>(_inbound[_readPos++]);
}

int MockClient::read(uint8_t* buf, size_t size) {
    if (buf == nullptr) {
        return 0;
    }
    size_t n = 0;
    while (n < size && revealedCount() > 0) {
        buf[n++] = _inbound[_readPos++];
    }
    return static_cast<int>(n);
}

int MockClient::peek() {
    if (revealedCount() == 0) {
        return -1;
    }
    return static_cast<int>(_inbound[_readPos]);
}

void MockClient::flush() {
    _flushCalled = true;
    ++_flushCount;
}

void MockClient::stop() {
    _connected = false;
    _stopCalled = true;
    ++_stopCount;
}

uint8_t MockClient::connected() {
    return _connected ? 1 : 0;
}

MockClient::operator bool() {
    return _connected;
}

// --- Internals -------------------------------------------------------------

size_t MockClient::revealedCount() const {
    const size_t unconsumed = _inbound.size() - _readPos;

    // Core / trickle-disabled behavior: every unconsumed byte is visible.
    // msPerReveal == 0 would make the schedule ill-defined, so it also means
    // "reveal everything".
    if (_trickleBytesPerReveal == 0 || _trickleMsPerReveal == 0) {
        return unconsumed;
    }

    // Trickle mode: bytesPerReveal are revealed immediately (step 1), and
    // bytesPerReveal more become visible for every msPerReveal of virtual time
    // elapsed since the schedule was configured.
    const unsigned long now = TestClock::instance().millis();
    const unsigned long elapsed = (now >= _trickleBaseMs) ? (now - _trickleBaseMs) : 0UL;
    const unsigned long long steps = 1ULL + (elapsed / _trickleMsPerReveal);

    // Absolute count of revealed bytes, saturated at the total scripted size.
    const unsigned long long revealedAbsWide =
        steps * static_cast<unsigned long long>(_trickleBytesPerReveal);
    const size_t revealedAbs = (revealedAbsWide >= _inbound.size())
                                   ? _inbound.size()
                                   : static_cast<size_t>(revealedAbsWide);

    // Only bytes past the read cursor are still visible to the caller.
    return (revealedAbs <= _readPos) ? 0 : (revealedAbs - _readPos);
}
