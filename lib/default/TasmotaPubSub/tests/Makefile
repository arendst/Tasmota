# Makefile for the TasmotaPubSub host-based doctest test system.
#
# Builds the *unmodified* library under test (../src/PubSubClient.cpp) together
# with the Arduino environment shim, the test support library (src/lib/*.cpp),
# and every test translation unit (src/*.cpp -> test_main.cpp + *_test.cpp)
# into a single doctest binary: build/pubsub_tests.
#
# Usage:
#   make            # or `make all` - build build/pubsub_tests
#   make test       # build + run the whole binary
#   make baseline   # build + run only the baseline suite (-ts=baseline)
#   make hardening  # build + run only the hardening suite (-ts=hardening)
#   make clean      # remove the build/ directory
#
#   make SANITIZE=0 ...   # build without ASan/UBSan
#   make test TS=-ts=baseline ARGS=--no-colors   # extra pass-through flags

# --- Toolchain -------------------------------------------------------------
# CXX defaults to `c++` so this works with both Apple clang and GNU g++.
CXX ?= c++

# --- Layout ----------------------------------------------------------------
BUILDDIR := build
OBJDIR   := $(BUILDDIR)/obj
BIN      := $(BUILDDIR)/pubsub_tests

# Library under test (never modified).
LIB_SRC := ../src/PubSubClient.cpp

# Arduino shim + test support library. Wildcard so future .cpp files under
# src/lib are picked up automatically.
SHIM_SRCS := $(wildcard src/lib/*.cpp)

# Test translation units: test_main.cpp (task 4.2) + all *_test.cpp.
# Wildcard so the build does not fail before any test sources exist and so
# future *_test.cpp files are compiled automatically.
TEST_SRCS := $(wildcard src/*.cpp)

# --- Object files ----------------------------------------------------------
# Objects live under build/obj (mirrored subtrees) to keep the source tree clean.
LIB_OBJ   := $(OBJDIR)/lib/PubSubClient.o
SHIM_OBJS := $(patsubst src/lib/%.cpp,$(OBJDIR)/shim/%.o,$(SHIM_SRCS))
TEST_OBJS := $(patsubst src/%.cpp,$(OBJDIR)/test/%.o,$(TEST_SRCS))
OBJS      := $(LIB_OBJ) $(SHIM_OBJS) $(TEST_OBJS)

# --- Flags -----------------------------------------------------------------
# Sanitizers ON by default; toggle off with `make SANITIZE=0`.
SANITIZE ?= 1
ifeq ($(SANITIZE),0)
  SAN_FLAGS :=
else
  SAN_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all
endif

# Include order matters: src/lib FIRST so the Arduino shim headers shadow any
# platform headers, then the library headers in ../src.
INCLUDES := -I src/lib -I ../src

CXXFLAGS := -std=c++17 -Wall -Wextra -DESP32 $(INCLUDES) $(SAN_FLAGS)
LDFLAGS  := $(SAN_FLAGS)

# Suite selection / extra doctest flag pass-through.
#   TS   - suite selector, e.g. TS=-ts=baseline
#   ARGS - any additional doctest flags
TS   ?=
ARGS ?=

# --- Targets ---------------------------------------------------------------
.PHONY: all test baseline hardening clean
.DEFAULT_GOAL := all

all: $(BIN)

$(BIN): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

# The library under test is compiled with AllocShim.h force-included so its
# malloc()/realloc() calls route through the test allocation interposer
# (F-07 / Requirement 13.2). This force-include is scoped to THIS object only,
# so no other TU (shim, doctest, tests) is affected. The library source itself
# is never modified.
$(LIB_OBJ): $(LIB_SRC)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -include src/lib/AllocShim.h -c $< -o $@

$(OBJDIR)/shim/%.o: src/lib/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/test/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(BIN)
	./$(BIN) $(TS) $(ARGS)

baseline: $(BIN)
	./$(BIN) -ts=baseline $(ARGS)

hardening: $(BIN)
	./$(BIN) -ts=hardening $(ARGS)

clean:
	rm -rf $(BUILDDIR)
