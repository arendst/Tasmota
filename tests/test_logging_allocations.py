#!/usr/bin/env python3
"""Fault-inject the actual firmware logging code without running a device.

Run: python3 tests/test_logging_allocations.py [--check-baseline GIT_REF]
The optional baseline check requires both original paths to attempt a NULL write.
Only the platform services are stubbed; the logging code and LList are extracted
from the selected source tree. Generated C++ and binaries stay in a temp folder.
"""

import argparse
import os
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
BERRY = 'tasmota/tasmota_xdrv_driver/xdrv_52_0_berry_struct.ino'
SUPPORT = 'tasmota/tasmota_support/support.ino'

PRELUDE = r'''
#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include "LList.h"

static bool fail_node, fail_berry, fail_log;
static void *node;
static unsigned berry_buffers, log_buffers, format_calls;
void *operator new(std::size_t size, const std::nothrow_t&) noexcept {
  if (fail_node) return nullptr;
  void *ptr = std::malloc(size);
  assert(node == nullptr);
  node = ptr;
  return ptr;
}
void operator delete(void *ptr) noexcept {
  if (ptr == node) node = nullptr;
  std::free(ptr);
}
void *berry_malloc(size_t size) {
  if (fail_berry) return nullptr;
  void *ptr = std::malloc(size);
  if (ptr) ++berry_buffers;
  return ptr;
}
void berry_free(void *ptr) {
  assert(ptr && berry_buffers);
  --berry_buffers;
  std::free(ptr);
}
void *log_malloc(size_t size) {
  if (fail_log) return nullptr;
  void *ptr = std::malloc(size);
  if (ptr) ++log_buffers;
  return ptr;
}
void log_free(void *ptr) {
  assert(ptr && log_buffers);
  --log_buffers;
  std::free(ptr);
}
int checked_snprintf(char *dst, size_t size, const char *fmt, ...) {
  ++format_calls;
  if (!dst && size) {
    std::fputs("NULL destination passed to snprintf\n", stderr);
    std::exit(86);  // Deterministic fault instead of invoking host undefined behavior.
  }
  va_list args;
  va_start(args, fmt);
  int result = std::vsnprintf(dst, size, fmt, args);
  va_end(args);
  return result;
}
#define snprintf_P checked_snprintf
#define strlen_P std::strlen
#define PSTR(s) s
#define D_HOUR_MINUTE_SEPARATOR ":"
#define D_MINUTE_SECOND_SEPARATOR ":"
#define LOG_BUFFER_SIZE 512
#define MAX_LOGSZ (LOG_BUFFER_SIZE - 64)
#define TOPSZ 256
struct {
  unsigned maxlog_level = 2, seriallog_level = 0, masterlog_level = 0;
  char *log_buffer = nullptr;
  unsigned log_buffer_pointer = 1;
} TasmotaGlobal;
struct SettingsMock {
  struct { bool show_heap_with_timestamp = false; } flag5;
  struct { bool json_pretty_print = false; } mbflag2;
} settings;
SettingsMock *Settings = &settings;
struct { int hour = 1, minute = 2, second = 3; } RtcTime;
int RtcMillis() { return 4; }
int ESP_getFreeHeap1024() { return 100; }
int ESP_getHeapFragmentation() { return 0; }
unsigned HighestLogLevel() { return 2; }
struct Console { template<typename... Args> void printf(Args...) {} } TasConsole;
void TasConsoleLDJsonPPCb(const char*, uint32_t) {}
bool LogDataJsonPrettyPrint(const char*, uint32_t, void (*)(const char*, uint32_t)) { return false; }
size_t strchrspn(const char *s, char c) {
  const char *found = std::strchr(s, c);
  return found ? size_t(found - s) : std::strlen(s);
}
#ifndef __APPLE__
size_t strlcat(char *dst, const char *src, size_t size) {
  size_t used = std::strlen(dst), add = std::strlen(src);
  if (used < size) std::strncat(dst, src, size - used - 1);
  return used + add;
}
#endif
'''

TESTS = r'''
int main(int argc, char **argv) {
  assert(argc == 2);
  const char *mode = argv[1];
  if (!std::strcmp(mode, "berry-failure")) {
    BerryLog logs;
    fail_berry = true;
    assert(logs.addString("button event") == nullptr);
    assert(logs.isEmpty() && node == nullptr && berry_buffers == 0);
    assert(format_calls == 0);
  } else if (!std::strcmp(mode, "node-failure")) {
    BerryLog logs;
    fail_node = true;
    assert(logs.addString("button event") == nullptr);
    assert(logs.isEmpty() && node == nullptr && berry_buffers == 0);
    assert(format_calls == 0);
  } else if (!std::strcmp(mode, "berry-success")) {
    BerryLog logs;
    assert(logs.addString(nullptr) == nullptr && logs.isEmpty());
    auto *entry = logs.addString("event", "[", "]");
    assert(entry && !std::strcmp(entry->val().getBuffer(), "[event]"));
    assert(logs.log.length() == 1 && berry_buffers == 1);
    logs.reset();
    assert(logs.isEmpty() && node == nullptr && berry_buffers == 0);
    entry = logs.addString("plain");
    assert(entry && !std::strcmp(entry->val().getBuffer(), "plain"));
    logs.reset();
  } else {
    char buffer[LOG_BUFFER_SIZE] = {};
    TasmotaGlobal.log_buffer = buffer;
    AddLogData(2, "existing", " payload", " retained");
    assert(std::strstr(buffer + 2, "01:02:03.004 existing payload retained\1"));
    char before[LOG_BUFFER_SIZE];
    std::memcpy(before, buffer, sizeof(buffer));
    unsigned index = TasmotaGlobal.log_buffer_pointer;
    char large[1001];
    std::memset(large, 'X', sizeof(large) - 1);
    large[sizeof(large) - 1] = 0;
    if (!std::strcmp(mode, "log-failure")) {
      fail_log = true;
      AddLogData(2, large);
      assert(!std::memcmp(before, buffer, sizeof(buffer)));
      assert(TasmotaGlobal.log_buffer_pointer == index && log_buffers == 0);
    } else if (!std::strcmp(mode, "log-success")) {
      AddLogData(2, large);
      assert(TasmotaGlobal.log_buffer_pointer == index + 1 && log_buffers == 0);
      assert(std::strstr(buffer, "... 1000 truncated\1"));
      const char *prefix = std::strchr(buffer, 'X');
      assert(prefix);
      for (size_t i = 0; i < TOPSZ - 21; ++i) assert(prefix[i] == 'X');
      assert(prefix[TOPSZ - 21] == '.');
    } else {
      assert(false && "unknown test mode");
    }
  }
  assert(node == nullptr && berry_buffers == 0 && log_buffers == 0);
}
'''


def read_source(path, revision=None):
    if revision:
        return subprocess.check_output(['git', 'show', f'{revision}:{path}'], cwd=ROOT, text=True)
    return (ROOT / path).read_text()


def build(directory, revision=None):
    berry = read_source(BERRY, revision)
    berry = berry[berry.index('class Log_line {'):berry.index('class BerrySupport {')]
    support = read_source(SUPPORT, revision)
    support = support[support.index('void AddLogData('):support.index('\nvoid TasConsoleLDJsonPPCb(')]
    # Only the oversized-log path calls malloc/free in AddLogData.
    cpp = directory / 'logging.cpp'
    cpp.write_text(PRELUDE + berry + '\n#define malloc log_malloc\n#define free log_free\n'
                   + support + '\n#undef malloc\n#undef free\n' + TESTS)
    binary = directory / 'logging'
    subprocess.run([os.environ.get('CXX', 'c++'), '-std=c++11', '-Wall', '-Wextra',
                    '-Werror', '-funsigned-char', '-fno-exceptions', '-fsanitize=address,undefined',
                    '-fno-omit-frame-pointer', '-I', str(ROOT / 'lib/default/TasmotaLList/src'),
                    str(cpp), '-o', str(binary)], check=True)
    return binary


def run(binary, mode, expected=0):
    result = subprocess.run([str(binary), mode], capture_output=True, text=True)
    if result.returncode != expected:
        raise AssertionError(f'{mode}: expected {expected}, got {result.returncode}\n{result.stderr}')
    if expected == 86:
        assert 'NULL destination passed to snprintf' in result.stderr


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check-baseline', metavar='GIT_REF')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='tasmota-log-allocation-') as temporary:
        directory = Path(temporary)
        if args.check_baseline:
            baseline = directory / 'baseline'
            baseline.mkdir()
            binary = build(baseline, args.check_baseline)
            for mode in ('berry-failure', 'log-failure'):
                run(binary, mode, 86)
            print('Baseline: both failed allocations reach a NULL snprintf destination.')
        current = directory / 'current'
        current.mkdir()
        binary = build(current)
        for mode in ('berry-failure', 'node-failure', 'berry-success', 'log-failure', 'log-success'):
            run(binary, mode)
        print('PASS: logging allocation failures drop safely; successful logs are preserved (5 cases, ASan/UBSan).')


if __name__ == '__main__':
    main()
