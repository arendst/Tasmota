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
      _writeLimitAfterCalls(0),
      _writeLimitAfterValue(0),
      _writeCallsSinceLimit(0),
      _trickleBytesPerReveal(0),
      _trickleMsPerReveal(0),
      _trickleBaseMs(0),
      _trickleStartReadPos(0),
      _connected(false),
      _connectResult(1),
      _connectResults(),
      _connectResultPos(0),
      _connectCalled(false),
      _connectCount(0),
      _connectionEvents(),
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
    _writeLimitAfterValue = 0;
    _writeCallsSinceLimit = 0;
}

void MockClient::setWriteLimitAfter(size_t successfulWriteCalls, size_t maxPerWrite) {
    _writeLimitAfterCalls = successfulWriteCalls;
    _writeLimitAfterValue = maxPerWrite;
    _writeCallsSinceLimit = 0;
}

void MockClient::setTrickle(size_t bytesPerReveal, unsigned long msPerReveal) {
    _trickleBytesPerReveal = bytesPerReveal;
    _trickleMsPerReveal = msPerReveal;
    // Each schedule starts from the read position that exists now. Otherwise a
    // consumed CONNECT/CONNACK prefix delays all newly queued bytes and a caller
    // must repeatedly enter loop() before the first trickled byte becomes visible.
    _trickleStartReadPos = _readPos;
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

void MockClient::pushConnectResult(int result) {
    _connectResults.push_back(result);
}

const std::vector<MockClient::ConnectionEvent>& MockClient::connectionEvents() const {
    return _connectionEvents;
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

unsigned MockClient::connectCount() const {
    return _connectCount;
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
    ++_connectCount;
    _connectionEvents.push_back(ConnectionEvent::Connect);
    _lastIp = ip;
    _lastPort = port;
    const int result = (_connectResultPos < _connectResults.size())
                           ? _connectResults[_connectResultPos++]
                           : _connectResult;
    // Model a real client: a successful connect brings the socket up. Tests can
    // still override the reported state with setConnected().
    if (result == 1) {
        _connected = true;
    }
    return result;
}

int MockClient::connect(const char* host, uint16_t port) {
    _connectCalled = true;
    ++_connectCount;
    _connectionEvents.push_back(ConnectionEvent::Connect);
    _lastHost = (host != nullptr) ? host : "";
    _lastPort = port;
    const int result = (_connectResultPos < _connectResults.size())
                           ? _connectResults[_connectResultPos++]
                           : _connectResult;
    if (result == 1) {
        _connected = true;
    }
    return result;
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
    size_t limit = _writeLimit;
    if ((_writeLimitAfterValue != 0) &&
        (_writeCallsSinceLimit >= _writeLimitAfterCalls)) {
        limit = _writeLimitAfterValue;
    }
    if (limit != 0 && accepted > limit) {
        accepted = limit;
    }
    ++_writeCallsSinceLimit;
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
    _connectionEvents.push_back(ConnectionEvent::Stop);
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

    // `steps` is measured from the point scheduling began, not byte zero of the
    // historical queue. Bytes consumed before setTrickle() stay consumed.
    const unsigned long long revealedAbsWide =
        static_cast<unsigned long long>(_trickleStartReadPos) +
        steps * static_cast<unsigned long long>(_trickleBytesPerReveal);
    const size_t revealedAbs = (revealedAbsWide >= _inbound.size())
                                   ? _inbound.size()
                                   : static_cast<size_t>(revealedAbsWide);

    // Only bytes past the read cursor are still visible to the caller.
    return (revealedAbs <= _readPos) ? 0 : (revealedAbs - _readPos);
}
