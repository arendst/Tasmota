# Unified Animation Engine Demo
# Demonstrates the simplified API using the new AnimationEngine

import animation
import global
import tasmota

print("=== Unified Animation Engine Demo ===")

# Create a strip with 10 pixels
var strip = global.Leds(10)

print(f"Created strip with {strip.length()} pixels")

# Create the unified animation engine
var engine = animation.create_engine(strip)
print(f"Created animation engine: {engine}")

# Create some animations
var red_solid = animation.solid(0xFFFF0000, 10)  # Red with priority 10
var blue_pulse = animation.pulse_animation(0xFF0000FF, 2000, 5)  # Blue pulse, 2s period, priority 5

print("Created animations:")
print(f"  Red solid: {red_solid}")
print(f"  Blue pulse: {blue_pulse}")

# Add animations to engine
engine.add_animation(red_solid)
engine.add_animation(blue_pulse)

print(f"Engine now has {engine.size()} animations")

# Start the engine
engine.start()
print("Engine started")

# Simulate some ticks
print("\nSimulating animation ticks...")
var current_time = tasmota.millis()

for i : 0..4
  print(f"Tick {i+1}:")
  engine.on_tick(current_time + (i * 100))  # 100ms intervals
  
  # Show current animations
  var animations = engine.get_animations()
  print(f"  Active animations: {size(animations)}")
  for anim : animations
    print(f"    - {anim.name} (priority: {anim.priority}, running: {anim.is_running})")
  end
end

# Test sequence management
print("\n=== Testing Sequence Management ===")

# Create a sequence manager
var seq_manager = animation.SequenceManager(engine)

# Create sequence steps
var steps = []
steps.push(animation.create_play_step(red_solid, 1000))  # Play red for 1s
steps.push(animation.create_wait_step(500))              # Wait 0.5s
steps.push(animation.create_play_step(blue_pulse, 2000)) # Play blue pulse for 2s

print(f"Created sequence with {size(steps)} steps")

# Add sequence manager to engine
engine.add_sequence_manager(seq_manager)

# Start the sequence
seq_manager.start_sequence(steps)
print("Sequence started")

# Simulate sequence execution
for i : 0..10
  print(f"Sequence tick {i+1}:")
  engine.on_tick(current_time + (i * 200))  # 200ms intervals
  
  if seq_manager.is_sequence_running()
    var step_info = seq_manager.get_current_step_info()
    print(f"  Current step: {step_info}")
  else
    print("  Sequence completed")
    break
  end
end

# Clean up
engine.stop()
print("\nEngine stopped")

print("\n=== API Consistency ===")

print("Main API:")
var main_engine = animation.create_engine(strip)
print(f"Main engine: {main_engine}")

print("\nDirect class access:")
var direct_engine = animation.animation_engine(strip)
print(f"Direct engine: {direct_engine}")

print("\n=== Demo Complete ===")
print("The unified AnimationEngine provides:")
print("- Single class replacing multiple components")
print("- Simplified API with same functionality")
print("- Better performance through reduced object overhead")
print("- Cleaner code organization")
print("- Streamlined development experience")