# OscillatorValueProvider with Animations Example
#
# This example shows how to use OscillatorValueProvider with actual animations
# to create dynamic, time-varying effects.

import global
import animation
import tasmota

print("=== OscillatorValueProvider with Animations ===")

# Create LED strip using global.Leds
var strip = global.Leds(20, 1)  # 20 LEDs, GPIO 1
var engine = animation.create_engine(strip)

print("\n1. Pulse Position with Oscillating Size")
print("Creating a pulse that changes size over time...")

# Create oscillators for dynamic parameters
var size_oscillator = animation.linear(1, 5, 2000)      # Size varies 1-5 pixels over 2 seconds
var brightness_oscillator = animation.smooth(100, 255, 3000)  # Brightness varies smoothly over 3 seconds

# Create a pulse position animation with static parameters first
var pulse_anim = animation.pulse_position(0xFF00FF00, 5, 1, 10, 0, true, "dynamic_pulse")

# Set dynamic parameters using the oscillators
pulse_anim.set_param_value("pulse_size", size_oscillator)
pulse_anim.set_param_value("opacity", brightness_oscillator)

# Add to engine
engine.add_animation(pulse_anim)
engine.start()

# Simulate animation over time
var start_time = tasmota.millis()
print("Animation frames (size and brightness changing):")

for frame: 0..10
  var current_time = start_time + frame * 300  # Every 300ms
  
  # Update animations
  engine.on_tick(current_time)
  
  # Show current state
  var pulse_size = size_oscillator.get_value(current_time)
  var brightness = brightness_oscillator.get_value(current_time)
  print(f"Frame {frame}: size={pulse_size}, brightness={brightness}")
  strip.show()
end

print("\n2. Multiple Oscillating Animations")
print("Creating multiple animations with different oscillating parameters...")

# Clear previous animation
engine.clear()
strip.clear()

# Create different oscillators
var pos1_osc = animation.ramp(2, 8, 2500)        # Position oscillates 2-8
var pos2_osc = animation.ramp(12, 18, 3000)      # Position oscillates 12-18
var color_osc = animation.linear(0, 255, 1500)         # Color component oscillates

# Create multiple pulse animations with static parameters first
var pulse1 = animation.pulse_position(0xFF0000FF, 2, 1, 10, 0, true, "pulse1")  # Blue
var pulse2 = animation.pulse_position(0xFFFF0000, 2, 1, 20, 0, true, "pulse2")  # Red

# Set dynamic positions using oscillators
pulse1.set_param_value("pos", pos1_osc)
pulse2.set_param_value("pos", pos2_osc)

# Add animations
engine.add_animation(pulse1)
engine.add_animation(pulse2)

print("Two pulses with oscillating positions:")
for frame: 0..8
  var current_time = start_time + frame * 400  # Every 400ms
  
  # Update animations
  engine.on_tick(current_time)
  
  # Show current positions
  var pos1 = pos1_osc.get_value(current_time)
  var pos2 = pos2_osc.get_value(current_time)
  print(f"Frame {frame}: pos1={pos1}, pos2={pos2}")
  strip.show()
end

print("\n3. Square Wave Strobe Effect")
print("Creating a strobe effect using square wave oscillator...")

# Clear previous animations
engine.clear()
strip.clear()

# Create a square wave oscillator for strobe effect
var strobe_osc = animation.square(0, 255, 500, 20)  # Fast strobe, 20% duty cycle

# Create a filled animation with oscillating brightness
var strobe_anim = animation.filled_animation.solid(0xFFFFFFFF, 30, 255, 0, true, "strobe")
strobe_anim.set_param_value("opacity", strobe_osc)  # Use oscillator for opacity

engine.add_animation(strobe_anim)

print("Strobe effect (square wave oscillator):")
for frame: 0..12
  var current_time = start_time + frame * 100  # Every 100ms (fast)
  
  # Update animations
  engine.on_tick(current_time)
  
  # Show strobe state
  var opacity = strobe_osc.get_value(current_time)
  var state = opacity > 128 ? "ON " : "OFF"
  print(f"Frame {frame}: opacity={opacity} ({state})")
  strip.show()
end

print("\n4. Breathing Effect with Color Shift")
print("Creating a breathing effect with color temperature shift...")

# Clear previous animations
engine.clear()
strip.clear()

# Create oscillators for breathing and color shift
var breathing_osc = animation.smooth(50, 255, 4000)     # 4-second breathing cycle
var color_temp_osc = animation.ramp(0, 255, 8000) # 8-second color shift

# Create a custom color provider that uses the oscillators
class DynamicColorProvider : animation.color_provider
  var brightness_osc
  var color_osc
  
  def init(brightness_osc, color_osc)
    self.brightness_osc = brightness_osc
    self.color_osc = color_osc
  end
  
  def get_color(time_ms)
    var brightness = self.brightness_osc.get_value(time_ms)
    var color_shift = self.color_osc.get_value(time_ms)
    
    # Create a color that shifts from warm (red) to cool (blue)
    var red = tasmota.scale_uint(255 - color_shift, 0, 255, 0, brightness)
    var green = tasmota.scale_uint(128, 0, 255, 0, brightness)
    var blue = tasmota.scale_uint(color_shift, 0, 255, 0, brightness)
    
    return (0xFF << 24) | (red << 16) | (green << 8) | blue
  end
  
  def update(time_ms)
    var changed1 = self.brightness_osc.update(time_ms)
    var changed2 = self.color_osc.update(time_ms)
    return changed1 || changed2
  end
end

var dynamic_color = DynamicColorProvider(breathing_osc, color_temp_osc)
var breathing_anim = animation.filled_animation(dynamic_color, 40, 255, 0, true, "breathing")

engine.add_animation(breathing_anim)

print("Breathing effect with color temperature shift:")
for frame: 0..10
  var current_time = start_time + frame * 500  # Every 500ms
  
  # Update animations
  engine.on_tick(current_time)
  
  # Show current state
  var brightness = breathing_osc.get_value(current_time)
  var color_temp = color_temp_osc.get_value(current_time)
  var temp_desc = color_temp < 85 ? "warm" : (color_temp < 170 ? "neutral" : "cool")
  print(f"Frame {frame}: brightness={brightness}, color_temp={color_temp} ({temp_desc})")
  strip.show()
end

print("\n=== Integration Demo Complete ===")

# Stop the engine
engine.stop()

print("\nThis example demonstrates how OscillatorValueProvider can be used to create")
print("dynamic, time-varying effects in animations. The oscillators provide smooth,")
print("predictable parameter changes that can be applied to any animation property.")

return "OscillatorValueProvider integration demo completed"