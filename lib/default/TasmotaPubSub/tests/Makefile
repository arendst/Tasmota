# Makefile for the TasmotaPubSub host-based doctest test system.
#
# Builds both guarded library implementation sources under test together with
# the Arduino environment shim, the test support library (src/lib/*.cpp), and
# every test translation unit (src/*.cpp -> test_main.cpp + *_test.cpp).
#
# The build has two compile-time dimensions - the protocol selector and the
# USE_MQTT_QOS feature - built into three independent binaries:
#
#   build/pubsub_tests_v311     -DMQTT_VERSION=MQTT_VERSION_3_1_1
#   build/pubsub_tests_v5       -DMQTT_VERSION=MQTT_VERSION_5_0 -DUSE_MQTT_QOS
#   build/pubsub_tests_v5noqos  -DMQTT_VERSION=MQTT_VERSION_5_0   (no USE_MQTT_QOS)
#
# USE_MQTT_QOS gates OUTBOUND QoS 1/2 acknowledged delivery: the Packet Identifier
# lifecycle for publishes, the Retained_Packet_Body replay buffer with its
# MQTT_INFLIGHT_COPY_BUDGET accounting, PUBLISH retransmission, the
# PUBACK/PUBREC/PUBCOMP handshakes, and the Receive Maximum back-pressure gate. The
# v5 binary compiles it in and runs the whole MQTT 5 corpus; the v5noqos binary
# leaves it out - the QoS-0-only production configuration - and runs every suite
# that does not depend on outbound QoS 1/2 (its QoS cases are excluded, see
# QOS_ONLY_TEST_SRCS and the #ifdef USE_MQTT_QOS guards in the mqtt5_* tests).
# Within the v5 binary, MQTT_INFLIGHT_COPY_BUDGET stays a byte ceiling rather than a
# selector, left at its 2048-byte default on purpose - the retention cases reach
# every budget state by varying body sizes.
#
# Objects live in build/obj/<selection>/{lib,shim,test}/ so the variants never
# collide.
#
# Usage:
#   make              # or `make all` - build both binaries
#   make v311         # build build/pubsub_tests_v311
#   make v5           # build build/pubsub_tests_v5
#   make test         # build + run both binaries (full run)
#   make baseline     # v311 binary, -ts=baseline  (byte-identity gate)
#   make mqtt5        # v5 binary,   -ts=mqtt5
#   make hardening    # alias for hardening-v5
#   make hardening-v5 # v5 binary,   -ts=hardening
#   make selector-check  # both invalid MQTT_VERSION values must fail to compile
#   make check        # authoritative host gate: selector + all required suites
#   make stdlib-scan     # reject forbidden std string/container types in library sources
#   make clean        # remove the build/ directory
#
#   make SANITIZE=0 ...   # build without ASan/UBSan
#   make test TS=-ts=baseline ARGS=--no-colors   # extra pass-through flags

# --- Toolchain -------------------------------------------------------------
# CXX defaults to `c++` so this works with both Apple clang and GNU g++.
CXX ?= c++

# --- Layout ----------------------------------------------------------------
BUILDDIR := build
OBJDIR   := $(BUILDDIR)/obj

# Selections. Each one gets its own binary, its own object tree and its own
# compile flags: v311 and v5 span the protocol dimension, and v5noqos adds the
# USE_MQTT_QOS dimension (MQTT 5.0 without outbound QoS 1/2). The retention, entry
# survival and retransmission cases run in the v5 binary, which defines USE_MQTT_QOS.
SELECTIONS := v311 v5 v5noqos

SELDEF_v311   := -DMQTT_VERSION=MQTT_VERSION_3_1_1
SELDEF_v5     := -DMQTT_VERSION=MQTT_VERSION_5_0 -DUSE_MQTT_QOS
# The v5noqos selection is the same MQTT 5.0 sources built WITHOUT USE_MQTT_QOS: the
# QoS-0-only production configuration. It exists so the reduced build is compiled, linked
# and run rather than only assumed to work.
SELDEF_v5noqos := -DMQTT_VERSION=MQTT_VERSION_5_0

BIN_v311    := $(BUILDDIR)/pubsub_tests_v311
BIN_v5      := $(BUILDDIR)/pubsub_tests_v5
BIN_v5noqos := $(BUILDDIR)/pubsub_tests_v5noqos

# Both implementation files are compiled for every selection, matching Arduino and
# PlatformIO source discovery. Whole-file selector guards make the inactive object empty.
LIB_SRCS := ../src/PubSubClient_311.cpp ../src/PubSubClient_5.cpp

# Source-level standard-library guard (Requirement 13.9). It scans only the
# device library sources, never the C++17 host fixtures under tests/src/.
PYTHON ?= python3
STDLIB_SCAN := tools/stdlib_scan.py
STDLIB_SCAN_SRCS_v311 := ../src/PubSubClient.h \
                         ../src/PubSubClient_311.h ../src/PubSubClient_311.cpp
STDLIB_SCAN_SRCS_v5 := ../src/PubSubClient.h \
                       ../src/PubSubClient_5.h ../src/PubSubClient_5.cpp

# Arduino shim + test support library. Wildcard so future .cpp files under
# src/lib are picked up automatically.
SHIM_SRCS := $(wildcard src/lib/*.cpp)

# Test translation units: test_main.cpp + all *_test.cpp.
# Wildcard so the build does not fail before any test sources exist and so
# future *_test.cpp files are compiled automatically.
TEST_SRCS := $(wildcard src/*.cpp)

# The compact MQTT 3.1.1 header intentionally does not declare MQTT 5 types or
# extension APIs. Keep MQTT 5-only corpus/support and API-contract tests in the
# MQTT 5 binary; the remaining tests exercise the restored level-4 surface.
SHIM_SRCS_v311    := $(filter-out src/lib/MqttCorpus.cpp,$(SHIM_SRCS))
SHIM_SRCS_v5      := $(SHIM_SRCS)
SHIM_SRCS_v5noqos := $(SHIM_SRCS)
TEST_SRCS_v311 := $(filter-out src/mqtt5_%.cpp src/api_shape_test.cpp \
                    src/connect_buffer_safety_test.cpp src/error_model_test.cpp \
                    src/mqtt311_parity_test.cpp src/vbi_test.cpp,$(TEST_SRCS))
TEST_SRCS_v5   := $(TEST_SRCS)

# Test TUs that exercise OUTBOUND QoS 1/2 acknowledged delivery (publish at QoS 1/2, the
# PUBACK/PUBREC/PUBCOMP handshakes, the Retained_Packet_Body replay buffer and its
# retransmission, and the Receive Maximum back-pressure gate). They run in the QoS-enabled
# v5 binary; the v5noqos binary omits them because that build refuses QoS 1/2 publishes
# before framing, so their assertions would not hold. Everything else - QoS 0 publishing,
# subscriptions, properties, inbound delivery - is built and run in BOTH v5 and v5noqos.
QOS_ONLY_TEST_SRCS := src/mqtt5_qos_test.cpp \
                      src/mqtt5_ack_test.cpp \
                      src/mqtt5_retention_admission_test.cpp \
                      src/mqtt5_retransmit_test.cpp \
                      src/mqtt5_receive_test.cpp
TEST_SRCS_v5noqos := $(filter-out $(QOS_ONLY_TEST_SRCS),$(TEST_SRCS_v5))

# --- Flags -----------------------------------------------------------------
# Sanitizers ON by default; toggle off with `make SANITIZE=0`.
SANITIZE ?= 1
ifeq ($(SANITIZE),0)
  SAN_FLAGS :=
else
  SAN_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all
endif

# Include order matters: src/lib FIRST so the Arduino shim headers shadow any
# platform headers, then the library headers in ../src and the real header-only
# Tasmota linked-list implementation shared with device builds.
INCLUDES := -I src/lib -I ../src -I ../../TasmotaLList/src

# BASE_CXXFLAGS is also used by selector-check, which must not depend on the
# sanitizer configuration.
# The production default keeps aliases disabled. Enable a small table only in the
# host harness so Property 21 can exercise establishment, reuse, and saturation.
BASE_CXXFLAGS := -std=c++17 -Wall -Wextra -DESP32 -DMQTT_MAX_TOPIC_ALIASES=3 $(INCLUDES)
CXXFLAGS      := $(BASE_CXXFLAGS) $(SAN_FLAGS)
DEPFLAGS      := -MMD -MP
LDFLAGS       := $(SAN_FLAGS)

# Suite selection / extra doctest flag pass-through.
#   TS   - suite selector, e.g. TS=-ts=baseline
#   ARGS - any additional doctest flags
TS   ?=
ARGS ?=

# Cases and subcases excluded from the v5 selection because their FIXTURE
# ARITHMETIC is specific to the MQTT 3.1.1 wire format - the same reason the
# whole `baseline` suite is excluded from v5, applied at case and subcase
# granularity. Every one of them still runs in the v311 selection, where it is
# the byte-identity net it was written to be. The hardening sources are frozen
# and must not be edited, so the exclusions live here instead.
#
# 1. INBOUND (task 7.1). Both F-01 exact-buffer cases size their payload with
#    the literal 3.1.1 overhead of an inbound PUBLISH:
#
#      payloadLen = total - 5;                    // 1 + RL + 2 + topicLen(1)
#      REQUIRE(pkt.size() == total);              // "fixture is exact"
#
#    An MQTT 5.0 PUBLISH always carries a Properties field, so the same packet
#    is 6 + payloadLen bytes and that REQUIRE cannot hold in the v5 selection.
#    Their 5.0 equivalent - the exact-buffer boundary window delivered through
#    CallbackContractAdapter under ASan - is Property 9 (task 7.4) in the
#    `mqtt5` suite. The pattern matches exactly these two case names:
#      "F-01 exact-buffer inbound PUBLISH boundaries stay in-bounds under ASan"
#      "F-01 integration: exact-buffer inbound PUBLISH keeps the adapter NUL
#       write in-bounds"
#
# 2. OUTBOUND (task 8.1). The same mismatch on the emitting side, now that an
#    outbound PUBLISH carries a Properties field in the v5 selection:
#
#    - The two F-07 alloc-failure cases (`buffer_test.cpp`, `findings_test.cpp`,
#      identical name) prove the working buffer survived a failed realloc by
#      publishing and decoding the result with `MqttParser::decodePublish`, the
#      3.1.1 decoder. Against a 5.0 PUBLISH that decoder reads the Properties
#      length byte as the first payload byte, so `payload == "online"` cannot
#      hold. The 5.0 equivalent of the alloc-failure contract is Property 26
#      (task 15) and the emitted-framing coverage is the `mqtt5` publish suite.
#
#    - The boundary SUBCASE of the two rewritten F-04 cases (`streaming_test.cpp`,
#      `findings_test.cpp`) derives its declared plength from
#      `Remaining Length = plength + 2 + topicLen` and then asserts
#      `out.size() == 1 + rlBytes + 2 + topicLen` - the 3.1.1 outbound PUBLISH
#      with no Properties field. In 5.0 the same call frames one byte more, so
#      the Remaining Length lands at 16,384 and is correctly refused. Only that
#      subcase is excluded: the refusal subcase above the limit is version-neutral
#      and keeps running in both selections, so Property 28's coverage is intact.
#
# 3. SUBSCRIBE (task 9.2). The same mismatch again, now that a 5.0 SUBSCRIBE
#    carries a Properties field between the Packet Identifier and the first
#    Topic Filter. Both F-08 exact-buffer cases (`subscribe_test.cpp`,
#    `findings_test.cpp`) shrink the working buffer to the literal 3.1.1 bound:
#
#      setBufferSize(10 + topicLen);  // 5 + msgId(2) + topiclen(2) + topic + qos(1)
#      REQUIRE(psc.subscribe(topic, qos));
#
#    A 5.0 SUBSCRIBE needs 11 + topicLen, so at that capacity the call is
#    correctly refused and the REQUIRE cannot hold; both cases then also decode
#    the result with `MqttParser::decodeSubscribe`, the 3.1.1 decoder, which
#    would read the Properties length byte as part of the filter length. Both
#    still run in the v311 selection, where they are the off-by-one net they
#    were written to be. The 5.0 equivalents live in `mqtt5_subscribe_test.cpp`:
#    the exact-buffer boundary at the 5.0 bound under ASan and - since the
#    findings_test case bundles null-topic rejection into the same case - the
#    Requirement 9.7 null-topic examples. The separate `subscribe_test.cpp` case
#    "subscribe / unsubscribe reject a null topic pointer" is NOT excluded and
#    keeps running in both selections.
V5_EXCLUDE := -tce='*exact-buffer inbound PUBLISH*,*failed setBufferSize leaves no nonzero capacity*,*exact-buffer SUBSCRIBE*,*F-09 inbound PUBLISH with QoS 2*' \
              -sce='*at and just below the limit*'

# --- Per-selection objects and rules ---------------------------------------
# One template instantiated once per selection. $(1) is the selection name, so
# the only thing that varies between the two instantiations is the object
# directory and $(SELDEF_<selection>).
define SELECTION_RULES

OBJ_LIB_$(1)  := $$(patsubst ../src/%.cpp,$$(OBJDIR)/$(1)/lib/%.o,$$(LIB_SRCS))
OBJ_SHIM_$(1) := $$(patsubst src/lib/%.cpp,$$(OBJDIR)/$(1)/shim/%.o,$$(SHIM_SRCS_$(1)))
OBJ_TEST_$(1) := $$(patsubst src/%.cpp,$$(OBJDIR)/$(1)/test/%.o,$$(TEST_SRCS_$(1)))
OBJS_$(1)     := $$(OBJ_LIB_$(1)) $$(OBJ_SHIM_$(1)) $$(OBJ_TEST_$(1))

OBJDIRS += $$(OBJDIR)/$(1)/lib $$(OBJDIR)/$(1)/shim $$(OBJDIR)/$(1)/test

$$(BIN_$(1)): $$(OBJS_$(1)) | $$(BUILDDIR)
	$$(CXX) $$(LDFLAGS) $$(OBJS_$(1)) -o $$@

# Each library source is compiled with AllocShim.h force-included so its
# malloc()/realloc() calls route through the test allocation interposer
# (F-07 / Requirement 13.2). This force-include is scoped to library objects only,
# so no other TU (shim, doctest, tests) is affected.
$$(OBJDIR)/$(1)/lib/%.o: ../src/%.cpp | $$(OBJDIR)/$(1)/lib
	$$(CXX) $$(CXXFLAGS) $$(DEPFLAGS) $$(SELDEF_$(1)) -include src/lib/AllocShim.h -c $$< -o $$@

$$(OBJDIR)/$(1)/shim/%.o: src/lib/%.cpp | $$(OBJDIR)/$(1)/shim
	$$(CXX) $$(CXXFLAGS) $$(DEPFLAGS) $$(SELDEF_$(1)) -c $$< -o $$@

$$(OBJDIR)/$(1)/test/%.o: src/%.cpp | $$(OBJDIR)/$(1)/test
	$$(CXX) $$(CXXFLAGS) $$(DEPFLAGS) $$(SELDEF_$(1)) -c $$< -o $$@

endef

$(foreach s,$(SELECTIONS),$(eval $(call SELECTION_RULES,$(s))))

ALL_OBJS := $(foreach s,$(SELECTIONS),$(OBJS_$(s)))
ALL_DEPS := $(ALL_OBJS:.o=.d)

# Order-only prerequisites: every output directory has a rule, so a fresh
# `make` right after `make clean` creates what it needs.
$(BUILDDIR) $(OBJDIRS):
	@mkdir -p $@

# --- Targets ---------------------------------------------------------------
.PHONY: all v311 v5 v5noqos test noqos baseline mqtt5 hardening hardening-v5 selector-check runtime-selector-check check stdlib-scan clean
.DEFAULT_GOAL := all

all: v311 v5 v5noqos

v311:    $(BIN_v311)
v5:      $(BIN_v5)
v5noqos: $(BIN_v5noqos)

# Both selections build and pass their applicable suites.
#
# The restored v311 implementation runs the frozen `baseline` characterization
# suite. Tests coupled to MQTT 5 extension APIs and post-migration hardening are
# compiled only into the v5 binary. The v5 binary runs everything except the
# MQTT 3.1.1 wire-format baseline.
#
# $(V5_EXCLUDE) applies the same reasoning at case granularity to the two frozen
# F-01 cases whose fixture arithmetic is hard-coded to the 3.1.1 inbound PUBLISH
# size - see the V5_EXCLUDE definition above for why, and for where the 5.0
# equivalent of that coverage lives.
#
test: stdlib-scan
	./$(BIN_v311) -ts=baseline $(TS) $(ARGS)
	./$(BIN_v5) -tse=baseline $(V5_EXCLUDE) $(TS) $(ARGS)

# The QoS-0-only production configuration: the same MQTT 5.0 sources built without
# USE_MQTT_QOS, excluding the outbound-QoS test TUs (see QOS_ONLY_TEST_SRCS). Proves the
# reduced build compiles, links and passes every suite that does not depend on outbound
# QoS 1/2 - QoS 0 publishing, subscriptions, properties and inbound delivery.
noqos: $(BIN_v5noqos)
	./$(BIN_v5noqos) -tse=baseline $(V5_EXCLUDE) $(TS) $(ARGS)

# Requirement 13.9: source-level enforcement of the library's Arduino String /
# LList rule. The scanner ignores comments and literals, rejects std::string,
# std::vector, std::array and std::map, and permits exactly one std::function:
# the MQTT_CALLBACK_SIGNATURE callback declaration. It runs after both protocol
# selections are built, so each selection receives the same guard.
stdlib-scan: $(BIN_v311) $(BIN_v5) $(STDLIB_SCAN)
	$(PYTHON) $(STDLIB_SCAN) --selection v311 $(STDLIB_SCAN_SRCS_v311)
	$(PYTHON) $(STDLIB_SCAN) --selection v5 $(STDLIB_SCAN_SRCS_v5)

# Requirement 15.3 byte-identity gate: the unmodified baseline sources must
# pass against the 3.1.1 selection.
baseline: $(BIN_v311)
	./$(BIN_v311) -ts=baseline $(ARGS)

# The MQTT 5 suite is mandatory for the final host gate. Unlike doctest's
# default behavior for an unmatched filter, this target rejects a zero-case run
# so a missing or mis-tagged MQTT 5 corpus cannot make validation look green.
mqtt5: $(BIN_v5)
	@log="$(BUILDDIR)/mqtt5.log"; \
	./$(BIN_v5) -ts=mqtt5 $(ARGS) >"$$log" 2>&1; rc=$$?; \
	cat "$$log"; \
	if [ $$rc -ne 0 ]; then exit $$rc; fi; \
	if ! grep -Eq 'test cases:[[:space:]]*[1-9][0-9]*[[:space:]]*\|' "$$log"; then \
	  echo "mqtt5: FAIL - suite selected zero test cases"; \
	  exit 1; \
	fi

hardening: hardening-v5

hardening-v5: $(BIN_v5)
	./$(BIN_v5) -ts=hardening $(V5_EXCLUDE) $(ARGS)

# Requirement 2.5: an out-of-range MQTT_VERSION must be rejected at compile
# time. MQTT_VERSION=3 is the retired MQIsdp selection, 7 is nonsense; both
# must trip the #error in PubSubClient.h. Compile-only (-fsyntax-only), and the
# target succeeds only when BOTH attempts fail. Compiler output is kept in
# build/selector-check-<value>.log.
selector-check: | $(BUILDDIR)
	@rc=0; \
	for v in 3 7; do \
	  log=$(BUILDDIR)/selector-check-$$v.log; \
	  if $(CXX) $(BASE_CXXFLAGS) -DMQTT_VERSION=$$v -fsyntax-only $(LIB_SRCS) >$$log 2>&1; then \
	    echo "selector-check: FAIL - MQTT_VERSION=$$v was accepted, expected #error"; \
	    rc=1; \
	  else \
	    echo "selector-check: ok   - MQTT_VERSION=$$v rejected (see $$log)"; \
	  fi; \
	done; \
	if [ $$rc -eq 0 ]; then echo "selector-check: PASS - both invalid selections rejected"; fi; \
	exit $$rc

# Requirement 24.1/24.2: fallback-only state and classifiers must not survive
# preprocessing in the dedicated MQTT 3.1.1 build. The v5-capable build must retain
# all sentinels so this check cannot pass merely because the feature was deleted.
runtime-selector-check: $(OBJ_LIB_v311) $(OBJ_LIB_v5) | $(BUILDDIR)
	@v311=$(BUILDDIR)/runtime-selector-v311.ii; \
	v5=$(BUILDDIR)/runtime-selector-v5.ii; \
	$(CXX) $(BASE_CXXFLAGS) $(SELDEF_v311) -E -P $(LIB_SRCS) >$$v311 && \
	$(CXX) $(BASE_CXXFLAGS) $(SELDEF_v5) -E -P $(LIB_SRCS) >$$v5 || exit 1; \
	rc=0; \
	for symbol in selectMqtt311Fallback isMqtt311FallbackResponse; do \
		if grep -q $$symbol $$v311; then \
			echo "runtime-selector-check: FAIL - $$symbol present in v311"; rc=1; \
		fi; \
		if ! grep -q $$symbol $$v5; then \
			echo "runtime-selector-check: FAIL - $$symbol absent from v5"; rc=1; \
		fi; \
	done; \
	if nm -C $(OBJ_LIB_v311) | grep -q 'PubSubClient::selectMqtt311Fallback'; then \
		echo "runtime-selector-check: FAIL - fallback code present in v311 object"; rc=1; \
	fi; \
	if ! nm -C $(OBJ_LIB_v5) | grep -q 'PubSubClient::selectMqtt311Fallback'; then \
		echo "runtime-selector-check: FAIL - fallback code absent from v5 object"; rc=1; \
	fi; \
	if [ $$rc -eq 0 ]; then \
		echo "runtime-selector-check: PASS - fallback state and object code exist only in v5"; \
	fi; \
	exit $$rc

# The authoritative host gate. The restored MQTT 3.1.1 implementation is
# covered by the baseline suite; MQTT 5 keeps its complete and hardening suites.
check: selector-check runtime-selector-check test mqtt5 baseline hardening-v5 noqos

clean:
	rm -rf $(BUILDDIR)

-include $(ALL_DEPS)
