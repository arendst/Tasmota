/*
  vbi_test.cpp - Property tests for the Variable Byte Integer codec
  (PubSubClient::encodeVariableByteInteger / variableByteIntegerSize /
  decodeVariableByteInteger), task 2.3.

  Feature: tasmota-pubsub-mqtt5

  The codec is version-neutral: it is compiled into BOTH selections and the
  3.1.1 build uses it for the Remaining Length exactly as the 5.0 build does.
  This file is therefore deliberately NOT wrapped in a TEST_SUITE, so it runs in
  both binaries. Tagging it `baseline` would exclude it from the v5 run
  (`make test` runs the v5 binary with -tse=baseline); tagging it `mqtt5` would
  exclude it from the v311 run.

  Every vector is cross-checked against the harness mirrors
  MqttPacket::encodeVbi / MqttParser::decodeVbi, which are written from the
  specification's own arithmetic rather than by calling the library, so a
  round-trip failure means two independent implementations disagree
  (Requirements 15.4, 15.5).

  Task 2.4 appends a second TEST_CASE to this file for Property 2 (rejection is
  total and non-emitting); the shared probe helper below is written so that case
  can reuse the sentinel-buffer convention.
*/

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "doctest.h"

#include "MqttPacket.h"
#include "PubSubClient.h"

namespace vbi_prop {

// Fill byte for the encode destination. 0xAA has its continuation bit set, so a
// stale sentinel byte can never be mistaken for a valid terminator: if the
// encoder wrote fewer bytes than claimed, the decode over the sentinel tail
// fails rather than silently succeeding.
static constexpr uint8_t kSentinel = 0xAA;

// Destination size is deliberately larger than the 2 bytes the codec can ever
// write, so the untouched tail is observable.
static constexpr size_t kBufSize = 4;

// One vector's worth of observations. Collecting the results first (instead of
// asserting inside the codec calls) lets the exhaustive sweep evaluate all
// 16,384 values cheaply while the named boundary subcases assert every
// individual field, so a size-class regression names itself.
struct VbiProbe {
    uint32_t n = 0;

    uint8_t expectedSize = 0;   // 1 for 0..127, 2 for 128..16383
    uint8_t reportedSize = 0;   // variableByteIntegerSize(n)
    uint8_t written = 0;        // bytes encodeVariableByteInteger claims to write
    bool    tailUntouched = false;

    // Decode over exactly the emitted bytes.
    bool     decodeOk = false;
    uint32_t decodedValue = 0;
    uint8_t  decodedUsed = 0;

    // Decode over the emitted bytes PLUS the sentinel tail: a well-formed field
    // must terminate on its own, consuming the same byte count and never
    // reaching into the trailing bytes.
    bool     decodeOkWithTail = false;
    uint32_t decodedValueWithTail = 0;
    uint8_t  decodedUsedWithTail = 0;

    std::vector<uint8_t> emitted;         // library bytes
    std::vector<uint8_t> mirrorEmitted;   // MqttPacket::encodeVbi

    bool     mirrorDecodeOk = false;
    uint32_t mirrorValue = 0;
    size_t   mirrorUsed = 0;

    // Property 1 in full, for a single vector.
    bool holds() const {
        return reportedSize == expectedSize
            && (reportedSize == 1 || reportedSize == 2)   // only two size classes
            && written == expectedSize
            && tailUntouched
            && decodeOk && decodedValue == n && decodedUsed == written
            && decodeOkWithTail && decodedValueWithTail == n
            && decodedUsedWithTail == written
            && mirrorEmitted == emitted
            && mirrorDecodeOk && mirrorValue == n
            && mirrorUsed == static_cast<size_t>(written);
    }
};

static VbiProbe probe(uint32_t n) {
    VbiProbe p;
    p.n = n;
    p.expectedSize = (n <= 127) ? 1 : 2;

    p.reportedSize = PubSubClient::variableByteIntegerSize(n);

    uint8_t buf[kBufSize];
    for (size_t i = 0; i < kBufSize; i++) {
        buf[i] = kSentinel;
    }
    p.written = PubSubClient::encodeVariableByteInteger(n, buf);

    p.tailUntouched = true;
    for (size_t i = p.written; i < kBufSize; i++) {
        if (buf[i] != kSentinel) {
            p.tailUntouched = false;
        }
    }
    p.emitted.assign(buf, buf + p.written);

    p.decodeOk = PubSubClient::decodeVariableByteInteger(
        buf, p.written, p.decodedValue, p.decodedUsed);
    p.decodeOkWithTail = PubSubClient::decodeVariableByteInteger(
        buf, kBufSize, p.decodedValueWithTail, p.decodedUsedWithTail);

    p.mirrorEmitted = MqttPacket::encodeVbi(n);
    p.mirrorDecodeOk = MqttParser::decodeVbi(p.mirrorEmitted, 0,
                                             p.mirrorValue, p.mirrorUsed);
    return p;
}

// Assert every field of one probe individually. Used by the named boundary
// subcases so the failing condition, not just the failing value, is reported.
static void checkVector(uint32_t n) {
    const VbiProbe p = probe(n);
    CAPTURE(n);
    CAPTURE(p.expectedSize);

    // Requirements 4.1 / 4.2: the two size classes, and only those two.
    CHECK(p.reportedSize == p.expectedSize);
    CHECK((p.reportedSize == 1 || p.reportedSize == 2));

    // Requirement 4.8: the encoder emits exactly variableByteIntegerSize(n)
    // bytes and touches nothing beyond them.
    CHECK(p.written == p.expectedSize);
    CHECK(p.tailUntouched);
    CHECK(p.emitted.size() == p.expectedSize);

    // Requirement 4.4: decoding the emitted bytes yields n, consuming the same
    // byte count.
    CHECK(p.decodeOk);
    CHECK(p.decodedValue == n);
    CHECK(p.decodedUsed == p.written);

    // The field terminates on its own: extra available bytes change nothing.
    CHECK(p.decodeOkWithTail);
    CHECK(p.decodedValueWithTail == n);
    CHECK(p.decodedUsedWithTail == p.written);

    // Cross-check against the independent harness mirrors.
    CHECK(p.mirrorEmitted == p.emitted);
    CHECK(p.mirrorDecodeOk);
    CHECK(p.mirrorValue == n);
    CHECK(p.mirrorUsed == static_cast<size_t>(p.written));
}

}  // namespace vbi_prop

// --- Property 1: Variable Byte Integer round-trip ---------------------------

// Feature: tasmota-pubsub-mqtt5, Property 1: For all integers n in 0..16383,
// encodeVariableByteInteger(n) emits exactly variableByteIntegerSize(n) bytes
// (1 for 0..127, 2 for 128..16383), and decoding those bytes yields n with the
// same byte count consumed.
//
// The curated table is EXHAUSTIVE over the whole supported range: 16,384
// vectors are cheap, so there is no stride sampling and no partial coverage.
// The boundary values 0, 127, 128 and 16,383 additionally get named subcases so
// a size-class regression identifies itself in the failure output.
TEST_CASE("Property 1: Variable Byte Integer round-trip over 0..16383") {
    using namespace vbi_prop;

    SUBCASE("boundary 0 - smallest 1-byte value") {
        checkVector(0);
        CHECK(MqttPacket::encodeVbi(0) == std::vector<uint8_t>{0x00});
    }

    SUBCASE("boundary 127 - largest 1-byte value") {
        checkVector(127);
        CHECK(MqttPacket::encodeVbi(127) == std::vector<uint8_t>{0x7F});
    }

    SUBCASE("boundary 128 - smallest 2-byte value") {
        checkVector(128);
        CHECK(MqttPacket::encodeVbi(128) == std::vector<uint8_t>{0x80, 0x01});
    }

    SUBCASE("boundary 16383 - largest encodable value") {
        checkVector(PubSubClient::MQTT_VBI_MAX);
        CHECK(PubSubClient::MQTT_VBI_MAX == 16383u);
        CHECK(MqttPacket::encodeVbi(16383) == std::vector<uint8_t>{0xFF, 0x7F});
    }

    SUBCASE("exhaustive sweep over every value in 0..16383") {
        // Evaluated silently and reported in aggregate: one assertion per vector
        // would mean ~200k doctest assertions for no extra diagnostic value,
        // since the first offending value and its full probe are reported here
        // and the boundary subcases above pin the size classes by name.
        uint32_t checked = 0;
        uint32_t mismatches = 0;
        uint32_t firstBad = 0;
        VbiProbe firstBadProbe;

        for (uint32_t n = 0; n <= PubSubClient::MQTT_VBI_MAX; n++) {
            const VbiProbe p = probe(n);
            checked++;
            if (!p.holds()) {
                if (mismatches == 0) {
                    firstBad = n;
                    firstBadProbe = p;
                }
                mismatches++;
            }
        }

        // Widened to unsigned so a byte count is reported as a number rather
        // than as a character.
        const unsigned firstBadExpectedSize = firstBadProbe.expectedSize;
        const unsigned firstBadReportedSize = firstBadProbe.reportedSize;
        const unsigned firstBadWritten = firstBadProbe.written;
        const unsigned firstBadDecodedUsed = firstBadProbe.decodedUsed;

        CAPTURE(firstBad);
        CAPTURE(firstBadExpectedSize);
        CAPTURE(firstBadReportedSize);
        CAPTURE(firstBadWritten);
        CAPTURE(firstBadProbe.decodeOk);
        CAPTURE(firstBadProbe.decodedValue);
        CAPTURE(firstBadDecodedUsed);
        CHECK(mismatches == 0);
        CHECK(checked == 16384u);   // the whole range really was covered
    }
}

// --- Property 2 support ------------------------------------------------------

namespace vbi_prop {

// Observations for one encode-rejection vector. The destination is a heap
// allocation of exactly kBufSize bytes, pre-filled with kSentinel, so a stray
// write is caught twice: inside the buffer by the sentinel comparison, past its
// end by ASan.
struct EncodeRejection {
    uint32_t n = 0;
    uint8_t  reportedSize = 0;   // variableByteIntegerSize(n), must be 0
    uint8_t  written = 0;        // encodeVariableByteInteger(n, out), must be 0
    bool     bufUntouched = false;
    size_t   mirrorSize = 0;     // MqttPacket::encodeVbi(n).size(), must be 0
};

static EncodeRejection encodeReject(uint32_t n) {
    EncodeRejection r;
    r.n = n;

    std::unique_ptr<uint8_t[]> out(new uint8_t[kBufSize]);
    for (size_t i = 0; i < kBufSize; i++) {
        out[i] = kSentinel;
    }

    r.reportedSize = PubSubClient::variableByteIntegerSize(n);
    r.written = PubSubClient::encodeVariableByteInteger(n, out.get());

    // Not one byte of the destination may change: the refusal is non-emitting,
    // which is what keeps a caller that aborts on the 0 return from leaking a
    // truncated or wrapped field onto the wire (Requirement 4.10).
    r.bufUntouched = true;
    for (size_t i = 0; i < kBufSize; i++) {
        if (out[i] != kSentinel) {
            r.bufUntouched = false;
        }
    }

    r.mirrorSize = MqttPacket::encodeVbi(n).size();
    return r;
}

struct DecodeOutcome {
    bool     ok = false;
    uint32_t value = 0;
    uint8_t  used = 0;
};

// Decode with EXACTLY `avail` bytes visible to the decoder. The buffer is a heap
// allocation of that exact size - not an oversized stack array - so any read at
// index >= avail is a heap-buffer-overflow that ASan reports, turning
// Requirement 4.7's "SHALL NOT read any byte beyond the bytes actually received"
// into a mechanically checked claim rather than an unobservable one.
//
// avail == 0 needs different handling: a zero-length `new uint8_t[0]` is not
// reliably poisoned (measured on this toolchain: reading it goes unreported), so
// a 1-byte block is allocated instead and the decoder is handed a pointer just
// past its end. That pointer is non-null, so the library's nullptr guard is NOT
// what makes the case pass - the `avail` check is - and any read at all is out of
// bounds.
static DecodeOutcome decodeExactAvail(const std::vector<uint8_t>& bytes,
                                      uint32_t avail) {
    const uint32_t allocSize = (avail == 0) ? 1u : avail;
    std::unique_ptr<uint8_t[]> heap(new uint8_t[allocSize]);
    for (uint32_t i = 0; i < allocSize; i++) {
        heap[i] = kSentinel;
    }
    for (uint32_t i = 0; i < avail; i++) {
        heap[i] = (i < bytes.size()) ? bytes[i] : kSentinel;
    }
    const uint8_t* buf = (avail == 0) ? (heap.get() + allocSize) : heap.get();

    DecodeOutcome o;
    o.ok = PubSubClient::decodeVariableByteInteger(buf, avail, o.value, o.used);
    return o;
}

// A refusal must not claim progress: no value, no consumed byte count. Otherwise
// a caller that ignores the boolean would advance its read cursor over bytes the
// decoder never validated.
static void checkDecodeRefused(const DecodeOutcome& o) {
    CHECK_FALSE(o.ok);
    CHECK(o.value == 0u);
    CHECK(o.used == 0);
}

}  // namespace vbi_prop

// --- Property 2: Variable Byte Integer rejection ----------------------------

// Feature: tasmota-pubsub-mqtt5, Property 2: For all values above 16383,
// encoding returns a failure indication and writes no byte to the output; and
// for all byte sequences whose second byte still sets the continuation bit,
// decoding returns a failure indication rather than consuming a third byte.
//
// This is Property 28's codec half: every refusal returns a failure indication
// and emits nothing, so no truncated or wrapped VBI can reach the wire. The
// outbound-path half (refusal atomicity across every emitting entry point,
// including streaming beginPublish) is task 14.5's.
TEST_CASE("Property 2: Variable Byte Integer rejection is total and non-emitting") {
    using namespace vbi_prop;

    // Requirement 4.5: above 16,383 the value is refused and no byte is emitted.
    SUBCASE("encode refuses every out-of-range value without emitting a byte") {
        // 16,384 is the first unencodable value; 65,535 and 268,435,455 are the
        // 16-bit and 4-byte-VBI ceilings the MQTT 5.0 spec would allow but this
        // codec deliberately does not; 0xFFFFFFFF is the 32-bit extreme, where a
        // naive septet loop would wrap into a plausible-looking 2-byte field.
        const uint32_t rejects[] = {16384u, 65535u, 268435455u, 0xFFFFFFFFu};

        for (uint32_t n : rejects) {
            const EncodeRejection r = encodeReject(n);
            CAPTURE(n);
            CHECK(r.written == 0);          // failure indication
            CHECK(r.bufUntouched);          // and nothing written
            CHECK(r.reportedSize == 0);     // size query agrees it is unencodable
            CHECK(r.mirrorSize == 0u);      // harness mirror refuses identically
        }
    }

    SUBCASE("16383 encodes but 16384 does not - the boundary is exact") {
        CHECK(PubSubClient::variableByteIntegerSize(PubSubClient::MQTT_VBI_MAX) == 2);
        CHECK(PubSubClient::variableByteIntegerSize(PubSubClient::MQTT_VBI_MAX + 1) == 0);
    }

    // Requirement 4.6: a 2nd byte that still sets the continuation bit implies a
    // 3rd byte, which is outside this codec's 2-byte contract.
    SUBCASE("decode refuses a 2nd byte that implies a 3rd") {
        const std::vector<uint8_t> impliesThird{0x80, 0x80};

        // Only the two bytes of the field are visible, so a decoder that reached
        // for a 3rd byte would read out of bounds instead of finding padding.
        checkDecodeRefused(decodeExactAvail(impliesThird, 2));

        uint32_t mirrorValue = 0;
        size_t mirrorUsed = 0;
        CHECK_FALSE(MqttParser::decodeVbi(impliesThird, 0, mirrorValue, mirrorUsed));

        // A continuation bit on the 2nd byte is refused whatever its payload
        // bits are, and extra following bytes do not rescue it.
        for (uint8_t low : {0x80, 0xFF}) {
            const std::vector<uint8_t> v{0xFF, low, 0x01};
            const DecodeOutcome o = decodeExactAvail(v, 3);
            CAPTURE(low);
            checkDecodeRefused(o);
        }
    }

    // Requirements 4.7 / 4.10: a field truncated within the received bytes is
    // refused, and no byte past `avail` is read.
    SUBCASE("decode refuses a field truncated within the available bytes") {
        const std::vector<uint8_t> continued{0x80};

        // avail == 1: the only available byte sets the continuation bit, so the
        // required 2nd byte was not received.
        checkDecodeRefused(decodeExactAvail(continued, 1));

        // avail == 0: nothing available at all, and nothing may be read - the
        // pointer handed over is non-null but addresses no readable byte.
        checkDecodeRefused(decodeExactAvail(continued, 0));

        // The harness mirror refuses the same two shapes.
        uint32_t mirrorValue = 0;
        size_t mirrorUsed = 0;
        CHECK_FALSE(MqttParser::decodeVbi(continued, 0, mirrorValue, mirrorUsed));
        CHECK_FALSE(MqttParser::decodeVbi(std::vector<uint8_t>{}, 0,
                                          mirrorValue, mirrorUsed));
    }

#if MQTT_VERSION == MQTT_VERSION_5_0
    SUBCASE("decode refuses non-minimal two-byte forms") {
        const std::vector<std::vector<uint8_t>> padded = {
            {0x80, 0x00},  // value 0 must be one byte
            {0x81, 0x00},  // value 1 must be one byte
            {0xFF, 0x00},  // value 127 must be one byte
        };
        for (const std::vector<uint8_t>& bytes : padded) {
            CAPTURE(bytes);
            checkDecodeRefused(decodeExactAvail(bytes, 2));
            uint32_t mirrorValue = 0;
            size_t mirrorUsed = 0;
            CHECK_FALSE(MqttParser::decodeVbi(bytes, 0, mirrorValue, mirrorUsed));
        }
    }
#endif

    // The refusals above are not over-broad: a well-formed 2-byte field whose
    // 2nd byte clears the continuation bit still decodes, with avail set to
    // exactly 2 so the accepting path is bounds-checked the same way.
    SUBCASE("refusal is narrow - a terminating 2nd byte still decodes") {
        const DecodeOutcome o = decodeExactAvail({0x80, 0x01}, 2);
        CHECK(o.ok);
        CHECK(o.value == 128u);
        CHECK(o.used == 2);
    }
}
