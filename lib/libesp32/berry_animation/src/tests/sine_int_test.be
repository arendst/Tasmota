# Test file for tasmota.sine_int function
#
# This file tests the fixed-point sine implementation
# that is optimized for performance on embedded systems.
#
# Command to run test is:
#    ./berry -s -g lib/libesp32/berry_animation/tests/sine_int_test.be

print("Testing tasmota.sine_int...")

def abs(x)
  return x >= 0 ? x : -x
end

# Test key points in the sine wave
# 0 degrees = 0
assert(tasmota.sine_int(0) == 0, "sine_int(0) should be 0")

# 30 degrees = pi/6 radians = 8192/3 = 2731
var angle_30deg = 2731
var expected_sin_30 = 2048  # sin(30°) = 0.5, so 0.5 * 4096 = 2048
var actual_sin_30 = tasmota.sine_int(angle_30deg)
print(f"sine_int({angle_30deg}) = {actual_sin_30} (expected ~{expected_sin_30})")
assert(abs(actual_sin_30 - expected_sin_30) <= 10, "sine_int(30°) should be approximately 2048")

# 45 degrees = pi/4 radians = 8192/2 = 4096
var angle_45deg = 4096
var expected_sin_45 = 2896  # sin(45°) = 0.7071, so 0.7071 * 4096 = 2896
var actual_sin_45 = tasmota.sine_int(angle_45deg)
print(f"sine_int({angle_45deg}) = {actual_sin_45} (expected ~{expected_sin_45})")
assert(abs(actual_sin_45 - expected_sin_45) <= 10, "sine_int(45°) should be approximately 2896")

# 90 degrees = pi/2 radians = 8192
var angle_90deg = 8192
var expected_sin_90 = 4096  # sin(90°) = 1.0, so 1.0 * 4096 = 4096
var actual_sin_90 = tasmota.sine_int(angle_90deg)
print(f"sine_int({angle_90deg}) = {actual_sin_90} (expected {expected_sin_90})")
assert(abs(actual_sin_90 - expected_sin_90) <= 1, "sine_int(90°) should be 4096")

# 180 degrees = pi radians = 8192*2 = 16384
var angle_180deg = 16384
var expected_sin_180 = 0  # sin(180°) = 0
var actual_sin_180 = tasmota.sine_int(angle_180deg)
print(f"sine_int({angle_180deg}) = {actual_sin_180} (expected {expected_sin_180})")
assert(abs(actual_sin_180 - expected_sin_180) <= 1, "sine_int(180°) should be 0")

# 270 degrees = 3pi/2 radians = 8192*3 = 24576
var angle_270deg = 24576
var expected_sin_270 = -4096  # sin(270°) = -1.0, so -1.0 * 4096 = -4096
var actual_sin_270 = tasmota.sine_int(angle_270deg)
print(f"sine_int({angle_270deg}) = {actual_sin_270} (expected {expected_sin_270})")
assert(abs(actual_sin_270 - expected_sin_270) <= 1, "sine_int(270°) should be -4096")

# 360 degrees = 2pi radians = 8192*4 = 32768
var angle_360deg = 32768
var expected_sin_360 = 0  # sin(360°) = 0
var actual_sin_360 = tasmota.sine_int(angle_360deg)
print(f"sine_int({angle_360deg}) = {actual_sin_360} (expected {expected_sin_360})")
assert(abs(actual_sin_360 - expected_sin_360) <= 1, "sine_int(360°) should be 0")

# Test negative angles
# -90 degrees = -pi/2 radians = -8192
var angle_neg_90deg = -8192
var expected_sin_neg_90 = -4096  # sin(-90°) = -1.0, so -1.0 * 4096 = -4096
var actual_sin_neg_90 = tasmota.sine_int(angle_neg_90deg)
print(f"sine_int({angle_neg_90deg}) = {actual_sin_neg_90} (expected {expected_sin_neg_90})")
assert(abs(actual_sin_neg_90 - expected_sin_neg_90) <= 1, "sine_int(-90°) should be -4096")

print("All tests passed!")
return true