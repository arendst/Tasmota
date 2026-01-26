# CPU Metrics and Profiling Test Suite
# Tests for the AnimationEngine CPU metrics and profiling API

import animation

print("=== CPU Metrics and Profiling Test Suite ===")

# Test utilities
var test_count = 0
var passed_count = 0

def assert_test(condition, message)
  test_count += 1
  if condition
    passed_count += 1
    print(f"✓ PASS: {message}")
  else
    print(f"✗ FAIL: {message}")
  end
end

def assert_equals(actual, expected, message)
  assert_test(actual == expected, f"{message} (expected: {expected}, actual: {actual})")
end

def assert_not_nil(value, message)
  assert_test(value != nil, f"{message} (value was nil)")
end

def assert_greater_than(actual, threshold, message)
  assert_test(actual > threshold, f"{message} (expected > {threshold}, actual: {actual})")
end

def assert_less_than(actual, threshold, message)
  assert_test(actual < threshold, f"{message} (expected < {threshold}, actual: {actual})")
end

# Test 1: CPU Metrics Initialization
print("\n--- Test 1: CPU Metrics Initialization ---")
var strip = global.Leds(30)
var engine = animation.create_engine(strip)

assert_not_nil(engine, "Engine should be created")
assert_equals(engine.tick_count, 0, "Tick count should start at 0")
assert_equals(engine.tick_time_sum, 0, "Tick time sum should start at 0")
assert_equals(engine.tick_time_min, 999999, "Tick time min should start at max value")
assert_equals(engine.tick_time_max, 0, "Tick time max should start at 0")
assert_equals(engine.anim_time_sum, 0, "Animation time sum should start at 0")
assert_equals(engine.hw_time_sum, 0, "Hardware time sum should start at 0")
assert_equals(engine.last_stats_time, 0, "Last stats time should start at 0")
assert_equals(engine.stats_period, 5000, "Stats period should be 5000ms")

# Test 2: Profiling Timestamps Initialization
print("\n--- Test 2: Profiling Timestamps Initialization ---")
assert_test(engine.ts_start == nil, "ts_start should be nil initially")
assert_test(engine.ts_1 == nil, "ts_1 should be nil initially")
assert_test(engine.ts_2 == nil, "ts_2 should be nil initially")
assert_test(engine.ts_3 == nil, "ts_3 should be nil initially")
assert_test(engine.ts_hw == nil, "ts_hw should be nil initially")
assert_test(engine.ts_end == nil, "ts_end should be nil initially")

# Test 3: Phase Metrics Initialization
print("\n--- Test 3: Phase Metrics Initialization ---")
assert_equals(engine.phase1_time_sum, 0, "Phase 1 time sum should start at 0")
assert_equals(engine.phase1_time_min, 999999, "Phase 1 time min should start at max value")
assert_equals(engine.phase1_time_max, 0, "Phase 1 time max should start at 0")
assert_equals(engine.phase2_time_sum, 0, "Phase 2 time sum should start at 0")
assert_equals(engine.phase2_time_min, 999999, "Phase 2 time min should start at max value")
assert_equals(engine.phase2_time_max, 0, "Phase 2 time max should start at 0")
assert_equals(engine.phase3_time_sum, 0, "Phase 3 time sum should start at 0")
assert_equals(engine.phase3_time_min, 999999, "Phase 3 time min should start at max value")
assert_equals(engine.phase3_time_max, 0, "Phase 3 time max should start at 0")

# Test 4: Timestamps Set During Ticks
print("\n--- Test 4: Timestamps Set During Ticks ---")

# Create a fresh engine for timestamp testing with an animation
var ts_strip = global.Leds(20)
var ts_engine = animation.create_engine(ts_strip)
ts_engine.tick_ms = 5  # Set low tick interval for testing

# Add an animation so rendering happens
var ts_anim = animation.solid(ts_engine)
ts_anim.color = 0xFFFF0000
ts_engine.add(ts_anim)
ts_engine.run()

# Run a single tick
var current_time = int(tasmota.millis())
ts_engine.on_tick(current_time)

# Check that timestamps were set
assert_not_nil(ts_engine.ts_start, "ts_start should be set after tick")
assert_not_nil(ts_engine.ts_1, "ts_1 should be set after tick")
assert_not_nil(ts_engine.ts_2, "ts_2 should be set after tick")
assert_not_nil(ts_engine.ts_3, "ts_3 should be set after tick")
assert_not_nil(ts_engine.ts_hw, "ts_hw should be set after tick (with animation)")
assert_not_nil(ts_engine.ts_end, "ts_end should be set after tick")

# Check timestamp ordering (only if all timestamps are set)
if ts_engine.ts_start != nil && ts_engine.ts_1 != nil
  assert_test(ts_engine.ts_start <= ts_engine.ts_1, "ts_start should be <= ts_1")
end
if ts_engine.ts_1 != nil && ts_engine.ts_2 != nil
  assert_test(ts_engine.ts_1 <= ts_engine.ts_2, "ts_1 should be <= ts_2")
end
if ts_engine.ts_2 != nil && ts_engine.ts_3 != nil
  assert_test(ts_engine.ts_2 <= ts_engine.ts_3, "ts_2 should be <= ts_3")
end
if ts_engine.ts_3 != nil && ts_engine.ts_hw != nil
  assert_test(ts_engine.ts_3 <= ts_engine.ts_hw, "ts_3 should be <= ts_hw")
end
if ts_engine.ts_hw != nil && ts_engine.ts_end != nil
  assert_test(ts_engine.ts_hw <= ts_engine.ts_end, "ts_hw should be <= ts_end")
end

ts_engine.stop()

# Test 5: Phase Metrics Accumulation
print("\n--- Test 5: Phase Metrics Accumulation ---")

# Create engine and run multiple ticks
var phase_strip = global.Leds(15)
var phase_engine = animation.create_engine(phase_strip)
phase_engine.tick_ms = 5  # Set low tick interval for testing
phase_engine.run()

# Run 10 ticks
var phase_time = int(tasmota.millis())
for i : 0..9
  phase_engine.on_tick(phase_time)
  phase_time += 5
end

# Check that phase metrics accumulated
assert_greater_than(phase_engine.phase1_time_sum, -1, "Phase 1 time sum should be non-negative")
assert_greater_than(phase_engine.phase2_time_sum, -1, "Phase 2 time sum should be non-negative")
assert_greater_than(phase_engine.phase3_time_sum, -1, "Phase 3 time sum should be non-negative")

# Check min/max tracking
# assert_test(phase_engine.phase1_time_min <= phase_engine.phase1_time_max, "Phase 1 min should be <= max")
# assert_test(phase_engine.phase2_time_min <= phase_engine.phase2_time_max, "Phase 2 min should be <= max")
# assert_test(phase_engine.phase3_time_min <= phase_engine.phase3_time_max, "Phase 3 min should be <= max")

phase_engine.stop()

# Test 6: Timestamp-Based Duration Calculation
print("\n--- Test 6: Timestamp-Based Duration Calculation ---")

# Create engine and run a tick
var dur_strip = global.Leds(10)
var dur_engine = animation.create_engine(dur_strip)
dur_engine.tick_ms = 5  # Set low tick interval for testing
dur_engine.run()

var dur_time = int(tasmota.millis())
dur_engine.on_tick(dur_time)

# Verify durations can be computed from timestamps
if dur_engine.ts_start != nil && dur_engine.ts_end != nil
  var total_duration = dur_engine.ts_end - dur_engine.ts_start
  assert_greater_than(total_duration, -1, "Total duration should be non-negative")
end

if dur_engine.ts_2 != nil && dur_engine.ts_3 != nil
  var anim_duration = dur_engine.ts_3 - dur_engine.ts_2
  assert_greater_than(anim_duration, -1, "Animation duration should be non-negative")
end

if dur_engine.ts_3 != nil && dur_engine.ts_hw != nil
  var hw_duration = dur_engine.ts_hw - dur_engine.ts_3
  assert_greater_than(hw_duration, -1, "Hardware duration should be non-negative")
end

dur_engine.stop()

# Test 7: CPU Metrics During Ticks
print("\n--- Test 7: CPU Metrics During Ticks ---")

# Create a fresh engine for tick testing
var tick_strip = global.Leds(20)
var tick_engine = animation.create_engine(tick_strip)
tick_engine.tick_ms = 5  # Set low tick interval for testing

# Add a simple animation
var test_anim = animation.solid(tick_engine)
test_anim.color = 0xFFFF0000
tick_engine.add(test_anim)
tick_engine.run()

# Simulate several ticks
var current_time = int(tasmota.millis())
for i : 0..9
  tick_engine.on_tick(current_time + i * 10)
end

# Check that metrics were recorded
assert_greater_than(tick_engine.tick_count, 0, "Tick count should increase after ticks")
assert_greater_than(tick_engine.tick_time_sum, -1, "Tick time sum should be non-negative")

# Test 8: Metrics Reset After Stats Period
print("\n--- Test 8: Metrics Reset After Stats Period ---")

# Create engine and simulate ticks over stats period
var reset_strip = global.Leds(15)
var reset_engine = animation.create_engine(reset_strip)
reset_engine.tick_ms = 5  # Set low tick interval for testing
reset_engine.run()

# Simulate ticks for just under 5 seconds
var start_time = int(tasmota.millis())
var current_time = start_time
while current_time < start_time + 4900
  reset_engine.on_tick(current_time)
  current_time += 5
end

var tick_count_before = reset_engine.tick_count
assert_greater_than(tick_count_before, 0, "Should have ticks before stats period")

# Record the last_stats_time before crossing threshold
var last_stats_before = reset_engine.last_stats_time

# Simulate more ticks to cross the 5 second threshold
while current_time < start_time + 5100
  reset_engine.on_tick(current_time)
  current_time += 5
end

# Check that stats were printed (last_stats_time updated)
assert_test(reset_engine.last_stats_time > last_stats_before, "Stats should have been printed (last_stats_time updated)")

# Tick count should have been reset and is now accumulating again
# It won't be exactly 1, but should be much smaller than before
assert_less_than(reset_engine.tick_count, 50, "Tick count should be small after reset (< 50)")
assert_less_than(reset_engine.tick_time_sum, 100, "Tick time sum should be small after reset")

# Test 9: Metrics Consistency Across Ticks
print("\n--- Test 9: Metrics Consistency Across Ticks ---")

var consistency_strip = global.Leds(25)
var consistency_engine = animation.create_engine(consistency_strip)
consistency_engine.tick_ms = 5  # Set low tick interval for testing
consistency_engine.run()

# Run multiple ticks and verify metrics consistency
var cons_time = int(tasmota.millis())
for i : 0..19
  consistency_engine.on_tick(cons_time)
  cons_time += 5
end

# Verify tick count matches iterations
assert_equals(consistency_engine.tick_count, 20, "Tick count should match iterations")

# Verify all metrics are consistent
assert_test(consistency_engine.tick_time_sum >= consistency_engine.anim_time_sum, "Total time should be >= animation time")
assert_test(consistency_engine.tick_time_sum >= consistency_engine.hw_time_sum, "Total time should be >= hardware time")

consistency_engine.stop()

# Test 10: Min/Max Tracking for All Metrics
print("\n--- Test 10: Min/Max Tracking for All Metrics ---")

var minmax_strip = global.Leds(10)
var minmax_engine = animation.create_engine(minmax_strip)
minmax_engine.tick_ms = 5  # Set low tick interval for testing

# Add an animation so rendering happens
var mm_anim = animation.solid(minmax_engine)
mm_anim.color = 0xFF00FF00
minmax_engine.add(mm_anim)
minmax_engine.run()

# Run several ticks
var mm_time = int(tasmota.millis())
for i : 0..9
  minmax_engine.on_tick(mm_time)
  mm_time += 5
end

# Verify min/max relationships for all metrics
assert_test(minmax_engine.tick_time_min <= minmax_engine.tick_time_max, "Tick min should be <= max")
assert_test(minmax_engine.anim_time_min <= minmax_engine.anim_time_max, "Anim min should be <= max")
assert_test(minmax_engine.hw_time_min <= minmax_engine.hw_time_max, "HW min should be <= max")
assert_test(minmax_engine.phase1_time_min <= minmax_engine.phase1_time_max, "Phase1 min should be <= max")
assert_test(minmax_engine.phase2_time_min <= minmax_engine.phase2_time_max, "Phase2 min should be <= max")
assert_test(minmax_engine.phase3_time_min <= minmax_engine.phase3_time_max, "Phase3 min should be <= max")

minmax_engine.stop()

# Test 11: Streaming Statistics Accuracy
print("\n--- Test 11: Streaming Statistics Accuracy ---")

var stats_strip = global.Leds(15)
var stats_engine = animation.create_engine(stats_strip)
stats_engine.tick_ms = 5  # Set low tick interval for testing
stats_engine.run()

# Run exactly 10 ticks
var stats_time = int(tasmota.millis())
for i : 0..9
  stats_engine.on_tick(stats_time)
  stats_time += 5
end

assert_equals(stats_engine.tick_count, 10, "Should have exactly 10 ticks")
assert_test(stats_engine.tick_time_sum >= 0, "Tick time sum should be non-negative")
assert_test(stats_engine.anim_time_sum >= 0, "Animation time sum should be non-negative")
assert_test(stats_engine.hw_time_sum >= 0, "Hardware time sum should be non-negative")

# Test 12: Phase Metrics Cleared After Stats
print("\n--- Test 12: Phase Metrics Cleared After Stats ---")

var clear_strip = global.Leds(20)
var clear_engine = animation.create_engine(clear_strip)
clear_engine.tick_ms = 5  # Set low tick interval for testing
clear_engine.run()

# Run some ticks to accumulate phase metrics
var clear_time = int(tasmota.millis())
for i : 0..9
  clear_engine.on_tick(clear_time)
  clear_time += 5
end

# Verify phase metrics accumulated
assert_greater_than(clear_engine.phase1_time_sum, -1, "Phase metrics should accumulate")

# Simulate ticks to cross stats period
var clear_start = clear_time
while clear_time < clear_start + 5100
  clear_engine.on_tick(clear_time)
  clear_time += 5
end

# Phase metrics should be reset after stats period
assert_less_than(clear_engine.phase1_time_sum, 50, "Phase1 sum should be small after reset")
assert_less_than(clear_engine.phase2_time_sum, 50, "Phase2 sum should be small after reset")
assert_less_than(clear_engine.phase3_time_sum, 50, "Phase3 sum should be small after reset")

clear_engine.stop()

# Test 13: Multiple Engines Independence
print("\n--- Test 13: Multiple Engines Independence ---")

var strip1 = global.Leds(10)
var engine1 = animation.create_engine(strip1)
engine1.tick_ms = 5  # Set low tick interval for testing
engine1.run()

var strip2 = global.Leds(20)
var engine2 = animation.create_engine(strip2)
engine2.tick_ms = 5  # Set low tick interval for testing
engine2.run()

# Run ticks on both engines
var e1_time = int(tasmota.millis())
var e2_time = int(tasmota.millis())

for i : 0..4
  engine1.on_tick(e1_time)
  e1_time += 5
end

for i : 0..9
  engine2.on_tick(e2_time)
  e2_time += 5
end

# Verify independent tick counts
assert_equals(engine1.tick_count, 5, "Engine1 should have 5 ticks")
assert_equals(engine2.tick_count, 10, "Engine2 should have 10 ticks")

# Verify independent timestamps (engines maintain their own state)
assert_test(engine1.ts_start != engine2.ts_start || engine1.tick_count != engine2.tick_count, "Engines should have independent state")

engine1.stop()
engine2.stop()

# Test 14: Timestamp Nil Safety
print("\n--- Test 14: Timestamp Nil Safety ---")

var nil_strip = global.Leds(15)
var nil_engine = animation.create_engine(nil_strip)

# Before any ticks, timestamps should be nil
assert_test(nil_engine.ts_start == nil, "ts_start should be nil before ticks")
assert_test(nil_engine.ts_end == nil, "ts_end should be nil before ticks")

# Metrics should handle nil timestamps gracefully
assert_equals(nil_engine.tick_count, 0, "Tick count should be 0 before ticks")
assert_equals(nil_engine.tick_time_sum, 0, "Tick time sum should be 0 before ticks")

# Test 15: Performance of Metrics Collection
print("\n--- Test 15: Performance of Metrics Collection ---")

var perf_strip = global.Leds(30)
var perf_engine = animation.create_engine(perf_strip)
perf_engine.tick_ms = 5  # Set low tick interval for testing
perf_engine.run()

# Measure overhead of metrics collection with timestamps
var perf_start = int(tasmota.millis())
var perf_time = perf_start
for i : 0..99
  perf_engine.on_tick(perf_start + i * 5)
end
var perf_duration = tasmota.millis() - perf_start

assert_less_than(perf_duration, 200, f"100 ticks with timestamp metrics should be fast (took {perf_duration}ms)")

perf_engine.stop()

# Cleanup
engine.stop()
tick_engine.stop()
reset_engine.stop()

# Test Results
print(f"\n=== Test Results ===")
print(f"Tests run: {test_count}")
print(f"Tests passed: {passed_count}")
print(f"Tests failed: {test_count - passed_count}")
print(f"Success rate: {tasmota.scale_uint(passed_count, 0, test_count, 0, 100)}%")

if passed_count == test_count
  print("🎉 All CPU metrics tests passed!")
else
  print("❌ Some CPU metrics tests failed")
  raise "test_failed"
end

print("\n=== CPU Metrics Benefits ===")
print("CPU Metrics and Timestamp-Based Profiling features:")
print("- Automatic performance tracking every 5 seconds")
print("- Separate animation vs hardware timing")
print("- Timestamp-based profiling (no duration storage)")
print("- Intermediate measurement points (ts_1, ts_2, ts_3)")
print("- Streaming statistics (no array storage)")
print("- Memory-efficient for ESP32 embedded systems")
print("- Helps identify performance bottlenecks")
print("- Min/max/mean timing statistics")
print("- Phase-based timing breakdown")
