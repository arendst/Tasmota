#
# Enhanced modern version of the breathing effect
# Showcases: Multiple colors, layering, value providers, events
# Demonstrates: Advanced color providers, dynamic parameters, event system
#

import animation

var leds = 30  # More LEDs for better effect

# Create LED strip
var strip = global.Leds(leds, gpio.pin(gpio.WS2812, 0))
var engine = animation.create_engine(strip)

# Create multiple breathing layers with different colors and speeds
var colors = [0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF]
var durations = [3000, 4000, 5000, 3500, 4500]

# Create breathing animations with different phases
for i: 0..4
  var color = colors[i]
  var duration = durations[i]
  
  # Create oscillator with phase offset to create wave effect
  var phase_offset = int(tasmota.scale_uint(i, 0, 4, 0, 100))
  var brightness_osc = animation.smooth(30, 200, duration)
  brightness_osc.set_phase(phase_offset)
  
  # Create position oscillator for moving breathe effect
  var pos_osc = animation.linear(0, leds-5, duration * 2)
  pos_osc.set_phase(phase_offset)
  
  # Create breathe animation
  var breathe_anim = animation.breathe_animation(color, 150, duration, true, "breathe_" + str(i))
  breathe_anim.set_param_value("opacity", brightness_osc)
  
  # Add slight priority differences for layering
  breathe_anim.priority = i * 2
  engine.add_animation(breathe_anim)
end

# Add a background rainbow that slowly cycles
var rainbow_bg = animation.filled_animation(
  animation.rich_palette_color_provider.rainbow(20000, 1, 50),  # Slow, dim rainbow
  0, 0, true, "rainbow_background"
)
engine.add_animation(rainbow_bg)

# Register event handlers for interactive control
animation.register_event_handler("button_press", 
  def(event_data)
    print("Button pressed - changing breathing speed")
    # Speed up all animations
    for anim: engine.get_animations()
      if anim.name[0..7] == "breathe_"
        # Double the breathing speed
        var current_duration = anim.get_param("duration", 3000)
        anim.set_param("duration", current_duration / 2)
      end
    end
  end,
  10, nil, {"description": "Speed up breathing on button press"}
)

animation.register_event_handler("timer", 
  def(event_data)
    if event_data.find("interval") == 10000  # Every 10 seconds
      print("Timer event - adding sparkle effect")
      # Add temporary sparkle overlay
      var sparkle = animation.twinkle_animation(0xFFFFFF, 5, 500, 2000, false, "sparkle_overlay")
      sparkle.priority = 100  # High priority overlay
      engine.add_animation(sparkle)
    end
  end,
  5, nil, {"description": "Add sparkles every 10 seconds"}
)

engine.start()

print("Modern Breathe Showcase - Enhanced breathing with multiple layers")
print("Features:")
print("- 5 breathing layers with different colors and phases")
print("- Background rainbow effect")
print("- Event system for interactive control")
print("- Dynamic parameter updates")
print("- Layered composition")
print("\nTrigger events:")
print("- animation.trigger_event('button_press', {})")
print("- animation.trigger_event('timer', {'interval': 10000})")
print("\nPress Ctrl+C to stop")