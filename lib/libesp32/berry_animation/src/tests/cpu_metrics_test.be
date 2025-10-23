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

# Test 2: Profiling API Initialization
print("\n--- Test 2: Profiling API Initialization ---")
assert_not_nil(engine.profile_points, "Profile points map should exist")
assert_not_nil(engine.profile_start_times, "Profile start times map should exist")
assert_equals(size(engine.profile_points), 0, "Profile points should start empty")
assert_equals(size(engine.profile_start_times), 0, "Profile start times should start empty")

# Test 3: Basic Profiling API
print("\n--- Test 3: Basic Profiling API ---")

# Test profile_start
engine.profile_start("test_section")
assert_equals(size(engine.profile_start_times), 1, "Should have 1 start time after profile_start")
assert_not_nil(engine.profile_start_times.find("test_section"), "Start time should be recorded")

# Test profile_end
engine.profile_end("test_section")
assert_equals(size(engine.profile_start_times), 0, "Start time should be cleared after profile_end")
assert_equals(size(engine.profile_points), 1, "Should have 1 profile point after profile_end")

var stats = engine.profile_points.find("test_section")
assert_not_nil(stats, "Profile stats should exist")
assert_equals(stats['count'], 1, "Profile count should be 1")
assert_greater_than(stats['sum'], -1, "Profile sum should be non-negative")
assert_greater_than(stats['min'], -1, "Profile min should be non-negative")
assert_greater_than(stats['max'], -1, "Profile max should be non-negative")

# Test 4: Multiple Profiling Points
print("\n--- Test 4: Multiple Profiling Points ---")

engine.profile_start("section_a")
# Simulate some work
var x = 0
while x < 10
  x += 1
end
engine.profile_end("section_a")

engine.profile_start("section_b")
# Simulate different work
var y = 0
while y < 5
  y += 1
end
engine.profile_end("section_b")

assert_equals(size(engine.profile_points), 3, "Should have 3 profile points (test_section + section_a + section_b)")
assert_not_nil(engine.profile_points.find("section_a"), "section_a stats should exist")
assert_not_nil(engine.profile_points.find("section_b"), "section_b stats should exist")

# Test 5: Repeated Profiling
print("\n--- Test 5: Repeated Profiling ---")

# Profile the same section multiple times
for i : 0..9
  engine.profile_start("repeated_section")
  var z = 0
  while z < i
    z += 1
  end
  engine.profile_end("repeated_section")
end

var repeated_stats = engine.profile_points.find("repeated_section")
assert_not_nil(repeated_stats, "Repeated section stats should exist")
assert_equals(repeated_stats['count'], 10, "Repeated section should have count of 10")
assert_greater_than(repeated_stats['sum'], -1, "Repeated section sum should be non-negative")

# Test 6: Profile End Without Start
print("\n--- Test 6: Profile End Without Start ---")

var points_before = size(engine.profile_points)
engine.profile_end("nonexistent_section")
var points_after = size(engine.profile_points)

assert_equals(points_after, points_before, "Profile end without start should not create new point")

# Test 7: CPU Metrics During Ticks
print("\n--- Test 7: CPU Metrics During Ticks ---")

# Create a fresh engine for tick testing
var tick_strip = global.Leds(20)
var tick_engine = animation.create_engine(tick_strip)

# Add a simple animation
var test_anim = animation.solid(tick_engine)
test_anim.color = 0xFFFF0000
tick_engine.add(test_anim)
tick_engine.run()

# Simulate several ticks
var current_time = tasmota.millis()
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
reset_engine.run()

# Simulate ticks for just under 5 seconds
var start_time = 0
var current_time = start_time
while current_time < 4900
  reset_engine.on_tick(current_time)
  current_time += 5
end

var tick_count_before = reset_engine.tick_count
assert_greater_than(tick_count_before, 0, "Should have ticks before stats period")

# Record the last_stats_time before crossing threshold
var last_stats_before = reset_engine.last_stats_time

# Simulate more ticks to cross the 5 second threshold
while current_time < 5100
  reset_engine.on_tick(current_time)
  current_time += 5
end

# Check that stats were printed (last_stats_time updated)
assert_test(reset_engine.last_stats_time > last_stats_before, "Stats should have been printed (last_stats_time updated)")

# Tick count should have been reset and is now accumulating again
# It won't be exactly 1, but should be much smaller than before
assert_less_than(reset_engine.tick_count, 50, "Tick count should be small after reset (< 50)")
assert_less_than(reset_engine.tick_time_sum, 100, "Tick time sum should be small after reset")

# Test 9: Profiling with Ticks
print("\n--- Test 9: Profiling with Ticks ---")

var profile_strip = global.Leds(25)
var profile_engine = animation.create_engine(profile_strip)
profile_engine.run()

# Simulate ticks with profiling
var tick_time = 0
for i : 0..19
  profile_engine.profile_start("tick_work")
  
  # Simulate some work
  var work = 0
  while work < 5
    work += 1
  end
  
  profile_engine.profile_end("tick_work")
  
  profile_engine.on_tick(tick_time)
  tick_time += 5
end

var tick_work_stats = profile_engine.profile_points.find("tick_work")
assert_not_nil(tick_work_stats, "Tick work profiling should exist")
assert_equals(tick_work_stats['count'], 20, "Should have 20 profiled sections")

# Test 10: Min/Max Tracking
print("\n--- Test 10: Min/Max Tracking ---")

var minmax_strip = global.Leds(10)
var minmax_engine = animation.create_engine(minmax_strip)

# Profile sections with varying durations
for i : 0..4
  minmax_engine.profile_start("varying_work")
  
  # Variable amount of work
  var work = 0
  while work < i * 10
    work += 1
  end
  
  minmax_engine.profile_end("varying_work")
end

var varying_stats = minmax_engine.profile_points.find("varying_work")
assert_not_nil(varying_stats, "Varying work stats should exist")
assert_test(varying_stats['min'] <= varying_stats['max'], "Min should be <= max")
assert_test(varying_stats['min'] >= 0, "Min should be non-negative")
assert_test(varying_stats['max'] >= 0, "Max should be non-negative")

# Test 11: Streaming Statistics Accuracy
print("\n--- Test 11: Streaming Statistics Accuracy ---")

var stats_strip = global.Leds(15)
var stats_engine = animation.create_engine(stats_strip)
stats_engine.run()

# Run exactly 10 ticks
var stats_time = 0
for i : 0..9
  stats_engine.on_tick(stats_time)
  stats_time += 5
end

assert_equals(stats_engine.tick_count, 10, "Should have exactly 10 ticks")
assert_test(stats_engine.tick_time_sum >= 0, "Tick time sum should be non-negative")
assert_test(stats_engine.anim_time_sum >= 0, "Animation time sum should be non-negative")
assert_test(stats_engine.hw_time_sum >= 0, "Hardware time sum should be non-negative")

# Test 12: Profile Points Cleared After Stats
print("\n--- Test 12: Profile Points Cleared After Stats ---")

var clear_strip = global.Leds(20)
var clear_engine = animation.create_engine(clear_strip)
clear_engine.run()

# Add some profile points
clear_engine.profile_start("temp_section")
clear_engine.profile_end("temp_section")

assert_equals(size(clear_engine.profile_points), 1, "Should have 1 profile point")

# Simulate ticks to cross stats period
var clear_time = 0
while clear_time < 5100
  clear_engine.on_tick(clear_time)
  clear_time += 5
end

# Profile points should be cleared after stats are printed
assert_equals(size(clear_engine.profile_points), 0, "Profile points should be cleared after stats period")

# Test 13: Multiple Engines Independence
print("\n--- Test 13: Multiple Engines Independence ---")

var strip1 = global.Leds(10)
var engine1 = animation.create_engine(strip1)

var strip2 = global.Leds(20)
var engine2 = animation.create_engine(strip2)

# Profile in engine1
engine1.profile_start("engine1_work")
engine1.profile_end("engine1_work")

# Profile in engine2
engine2.profile_start("engine2_work")
engine2.profile_end("engine2_work")

assert_equals(size(engine1.profile_points), 1, "Engine1 should have 1 profile point")
assert_equals(size(engine2.profile_points), 1, "Engine2 should have 1 profile point")
assert_not_nil(engine1.profile_points.find("engine1_work"), "Engine1 should have engine1_work")
assert_not_nil(engine2.profile_points.find("engine2_work"), "Engine2 should have engine2_work")
assert_test(engine1.profile_points.find("engine2_work") == nil, "Engine1 should not have engine2_work")
assert_test(engine2.profile_points.find("engine1_work") == nil, "Engine2 should not have engine1_work")

# Test 14: Nested Profiling (Same Name)
print("\n--- Test 14: Nested Profiling (Same Name) ---")

var nested_strip = global.Leds(15)
var nested_engine = animation.create_engine(nested_strip)

# Start outer profiling
nested_engine.profile_start("nested_section")

# Start inner profiling (overwrites start time)
nested_engine.profile_start("nested_section")

# End profiling (uses most recent start time)
nested_engine.profile_end("nested_section")

var nested_stats = nested_engine.profile_points.find("nested_section")
assert_not_nil(nested_stats, "Nested section stats should exist")
assert_equals(nested_stats['count'], 1, "Should have 1 count (inner timing)")

# Test 15: Performance of Metrics Collection
print("\n--- Test 15: Performance of Metrics Collection ---")

var perf_strip = global.Leds(30)
var perf_engine = animation.create_engine(perf_strip)
perf_engine.run()

# Measure overhead of metrics collection
var perf_start = tasmota.millis()
for i : 0..99
  perf_engine.on_tick(perf_start + i * 5)
end
var perf_duration = tasmota.millis() - perf_start

assert_less_than(perf_duration, 200, f"100 ticks with metrics should be fast (took {perf_duration}ms)")

# Measure overhead of profiling
perf_start = tasmota.millis()
for i : 0..99
  perf_engine.profile_start("perf_test")
  perf_engine.profile_end("perf_test")
end
var profile_duration = tasmota.millis() - perf_start

assert_less_than(profile_duration, 100, f"100 profile calls should be fast (took {profile_duration}ms)")

# Cleanup
engine.stop()
tick_engine.stop()
reset_engine.stop()
profile_engine.stop()
clear_engine.stop()

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
print("CPU Metrics and Profiling features:")
print("- Automatic performance tracking every 5 seconds")
print("- Separate animation vs hardware timing")
print("- Custom profiling API for any code section")
print("- Streaming statistics (no array storage)")
print("- Memory-efficient for ESP32 embedded systems")
print("- Helps identify performance bottlenecks")
print("- Min/max/mean timing statistics")
