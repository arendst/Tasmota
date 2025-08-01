# Comet Animation Demo
# Demonstrates the comet animation effect with different configurations

import tasmota
import animation

print("=== Comet Animation Demo ===")

# Test 1: Basic solid color comet
print("\n--- Test 1: Basic Solid Color Comet ---")
var comet1 = animation.comet.solid(0xFFFF0000, 5, 2560, 30, 10)  # Red comet, 5-pixel tail, 10 pixels/sec, 30-pixel strip
print("Created:", comet1)

# Test the parameter system
print("Setting tail length to 8...")
comet1.set_tail_length(8)
print("Tail length:", comet1.get_param("tail_length"))

print("Setting speed to 15.0...")
comet1.set_speed(3840)
print("Speed:", comet1.get_param("speed"))

# Test 2: Color cycling comet
print("\n--- Test 2: Color Cycling Comet ---")
var rainbow_colors = [0xFFFF0000, 0xFFFF8000, 0xFFFFFF00, 0xFF00FF00, 0xFF0000FF, 0xFF8000FF]
var comet2 = animation.comet.color_cycle(rainbow_colors, 3000, 7, 2048, 30, 15)
print("Created:", comet2)

# Test 3: Rich palette comet
print("\n--- Test 3: Rich Palette Comet ---")
var comet3 = animation.comet.rich_palette(animation.PALETTE_FIRE, 4000, 6, 3072, 30, 20)
print("Created:", comet3)

# Test 4: Custom configuration
print("\n--- Test 4: Custom Configuration ---")
var comet4 = animation.comet(
  0xFF00FFFF,  # Cyan color
  10,          # 10-pixel tail
  1280,        # 5 pixels/sec (5 * 256)
  -1,          # Backward direction
  false,       # No wrap around (bounce)
  204,         # Slower fade (80% = 204/255)
  25,          # 25-pixel strip
  25,          # Priority 25
  0,           # Infinite duration
  true,        # Loop
  "custom_comet"
)
print("Created:", comet4)

# Test parameter validation
print("\n--- Test 5: Parameter Validation ---")
print("Testing invalid tail length...")
var result = comet1.set_param("tail_length", -1)
print("Set tail_length to -1:", result ? "SUCCESS" : "FAILED (expected)")

print("Testing invalid speed...")
result = comet1.set_param("speed", 0)
print("Set speed to 0:", result ? "SUCCESS" : "FAILED (expected)")

print("Testing invalid direction...")
result = comet1.set_param("direction", 2)
print("Set direction to 2:", result ? "SUCCESS" : "FAILED (expected)")

print("Testing valid direction...")
result = comet1.set_param("direction", -1)
print("Set direction to -1:", result ? "SUCCESS (expected)" : "FAILED")

# Test 6: Animation lifecycle
print("\n--- Test 6: Animation Lifecycle ---")
print("Starting comet1...")
comet1.start()
print("Is running:", comet1.is_running)

print("Stopping comet1...")
comet1.stop()
print("Is running:", comet1.is_running)

# Test 7: Frame buffer rendering
print("\n--- Test 7: Frame Buffer Rendering ---")
var frame = animation.frame_buffer(30)
print("Created frame buffer:", frame)

# Start the animation and simulate a few updates
comet1.start()
var start_time = tasmota.millis()

print("Simulating animation updates...")
var i = 0
while i < 5
  var current_time = start_time + (i * 100)  # 100ms intervals
  comet1.update(current_time)
  comet1.render(frame, current_time)
  print(f"Frame {i}: head_pos={comet1.head_position:.2f}")
  i += 1
end

print("\n=== Comet Animation Demo Complete ===")