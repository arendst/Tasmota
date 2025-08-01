# Clean API Demo
# Demonstrates the simplified, unified animation API

import animation
import tasmota

print("=== Clean Animation API Demo ===")

# Mock LED strip for demonstration
class MockStrip
  var length_val
  var pixels
  
  def init(length)
    self.length_val = length
    self.pixels = []
    for i : 0..length-1
      self.pixels.push(0x00000000)
    end
  end
  
  def length()
    return self.length_val
  end
  
  def set_pixel_color(index, color)
    if index >= 0 && index < self.length_val
      self.pixels[index] = color
    end
  end
  
  def show()
    print(f"✓ Strip updated with {self.length_val} pixels")
  end
  
  def can_show()
    return true
  end
end

print("\n--- Simple Setup ---")

# Create LED strip
var strip = MockStrip(30)
print(f"Created strip with {strip.length()} pixels")

# Create animation engine - ONE LINE SETUP!
var engine = animation.create_engine(strip)
print(f"Created engine: {engine}")

print("\n--- Add Animations ---")

# Create some animations
var red_fill = animation.solid(0xFFFF0000, 10)      # Solid red, priority 10
var blue_pulse = animation.pulse_animation(0xFF0000FF, 2000, 5) # Blue pulse, 2s, priority 5
var rainbow = animation.rich_palette_animation(animation.PALETTE_RAINBOW, 5000, 1, 255, 15) # Rainbow, priority 15

print("Created animations:")
print(f"  Red fill: priority {red_fill.priority}")
print(f"  Blue pulse: priority {blue_pulse.priority}")  
print(f"  Rainbow: priority {rainbow.priority}")

# Add animations to engine
engine.add_animation(red_fill)
engine.add_animation(blue_pulse)
engine.add_animation(rainbow)

print(f"Engine now managing {engine.size()} animations")

print("\n--- Start Animation System ---")

# Start the engine
engine.start()
print(f"Engine started: {engine.is_active()}")

print("\n--- Simulate Animation Loop ---")

# Simulate some animation frames
var current_time = tasmota.millis()
for i : 0..5
  print(f"\nFrame {i+1}:")
  engine.on_tick(current_time + (i * 50))  # 50ms intervals
  
  var active_animations = engine.get_animations()
  print(f"  Active animations: {size(active_animations)}")
  for anim : active_animations
    if anim.is_running
      print(f"    ✓ {anim.name} (priority: {anim.priority})")
    end
  end
end

print("\n--- Sequence Management ---")

# Create and run a sequence
var seq_manager = animation.SequenceManager(engine)
var steps = []

# Build sequence: red for 1s, wait 0.5s, rainbow for 2s
steps.push(animation.create_play_step(red_fill, 1000))
steps.push(animation.create_wait_step(500))
steps.push(animation.create_play_step(rainbow, 2000))

print(f"Created sequence with {size(steps)} steps")

# Add sequence to engine and start
engine.add_sequence_manager(seq_manager)
seq_manager.start_sequence(steps)

print("Sequence started - simulating execution...")

# Simulate sequence execution
for i : 0..8
  print(f"Sequence frame {i+1}:")
  engine.on_tick(current_time + 1000 + (i * 250))  # 250ms intervals
  
  if seq_manager.is_sequence_running()
    var step_info = seq_manager.get_current_step_info()
    print(f"  Current step: {step_info}")
  else
    print("  ✓ Sequence completed")
    break
  end
end

print("\n--- Cleanup ---")

# Stop the engine
engine.stop()
print(f"Engine stopped: {engine.is_active()}")

# Clear all animations
engine.clear()
print(f"Animations cleared: {engine.size()} remaining")

print("\n=== API Summary ===")
print("The unified animation API provides:")
print("✓ Single object creation: animation.create_engine(strip)")
print("✓ Simple animation management: engine.add_animation()")
print("✓ Easy lifecycle control: engine.start() / engine.stop()")
print("✓ Integrated sequence support: engine.add_sequence_manager()")
print("✓ Clean, intuitive interface with no deprecated components")
print("✓ High performance through unified architecture")

print("\n=== Demo Complete ===")
print("This demonstrates the clean, simplified animation framework")
print("that replaces the previous complex multi-object architecture")