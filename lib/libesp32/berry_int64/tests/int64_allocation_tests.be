# Native int64 results must release their payloads when collected.
# Run from the repository root:
# lib/libesp32/berry/berry -g lib/libesp32/berry_int64/tests/int64_allocation_tests.be

import gc

def check_result_values()
  var a = int64(42)
  var b = int64(5)
  assert((a + b).tostring() == "47")
  assert(a.add(-1).tostring() == "41")
  assert((a - b).tostring() == "37")
  assert((-a).tostring() == "-42")
  assert((a * b).tostring() == "210")
  assert((a * int64(0)).tostring() == "0")
  assert((a / b).tostring() == "8")
  assert((a % b).tostring() == "2")
  assert(a.tostring() == "42")
  assert(b.tostring() == "5")

  var maximum = int64("9223372036854775807")
  var minimum = int64("-9223372036854775808")
  assert(int64.fromstring("9223372036854775807") == maximum)
  assert(int64.fromstring("-9223372036854775808") == minimum)
  assert(int64.fromu32(0xFFFFFFFF, 0x7FFFFFFF) == maximum)
  assert(int64.fromu32(0, 0x80000000) == minimum)
  assert(int64.frombytes(bytes("FFFFFFFFFFFFFF7F")) == maximum)
  assert(int64.frombytes(bytes("0000000000000080")) == minimum)
  assert(int64.fromfloat(-3.5).tostring() == "-3")
  assert(maximum.add(-1).tostring() == "9223372036854775806")
  assert(minimum.add(1).tostring() == "-9223372036854775807")
  assert((maximum << 1).tostring() == "-2")
  assert((int64(1) << 63) == minimum)
  assert((minimum >> 63).tostring() == "-1")
  assert((a << 64) == a)
  assert((a >> 64) == a)
  assert((int64(1) << -1) == minimum)
  assert((minimum >> -1).tostring() == "-1")
end

def allocate_results(count)
  var a = int64(42)
  var b = int64(5)
  var zero = int64(0)
  var raw = bytes("2A00000000000000")
  for i : 1..count
    # Exercise every caller of the result-allocation helper.
    var result = a + b
    result = a.add(1)
    result = a - b
    result = -a
    result = a * b
    result = a * zero
    result = a / b
    result = a % b
    result = a << 1
    result = a >> 1
    result = int64.fromstring("42")
    result = int64.fromu32(42)
    result = int64.fromfloat(42.5)
    result = int64.frombytes(raw)
    assert(result == a)
  end
end

def check_collected_memory()
  # Warm the same stack paths before measuring. A second collection also
  # releases objects whose finalizers ran during the first collection.
  allocate_results(1000)
  for batch : 1..3
    gc.collect()
    gc.collect()
    var before = gc.allocated()
    allocate_results(1000)
    gc.collect()
    gc.collect()
    var retained = gc.allocated() - before
    print("int64 allocation batch", batch, "retained bytes:", retained)
    assert(retained == 0, "int64 result payloads leaked after garbage collection")
  end
end

check_result_values()
check_collected_memory()
print("int64 allocation tests passed")
