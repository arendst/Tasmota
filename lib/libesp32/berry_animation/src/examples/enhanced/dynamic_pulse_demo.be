#
# Dynamic pulse demonstration with value providers and events
# Showcases: Multiple pulse types, dynamic parameters, interactive control
# Demonstrates: Value provider system, event-driven parameter changes
#

import animation

var leds = 50  # More LEDs for better pulse effects

# Create LED strip
var strip = global.Leds(leds, gpio.pin(gpio.WS2812, 0))
var engine = animation.create_engine(strip)

# Create background with subtle color cycling
var bg_provider = animation.color_cycle_color_provider([
  0x001122, 0x002211, 0x112200, 0x220011
], 30000, 1)
var background = animation.filled_animation(bg_provider, 0, 0, true, "cycling_background")
engine.add_animation(background)

# Create multiple pulse types with different characteristics

# 1. Breathing pulse with oscillating size
var breathe_pulse = animation.pulse_position(0xFF0000, 5, 1, 10, 0, true, "breathe_pulse")
var size_osc = animation.smooth(2, 12, 4000)
var pos_osc = animation.linear(5, leds-15, 8000)
breathe_pulse.set_param_value("pulse_size", size_osc)
breathe_pulse.set_param_value("pos", pos_osc)
breathe_pulse.priority = 10
engine.add_animation(breathe_pulse)

# 2. Strobe pulse with square wave intensity
var strobe_pulse = animation.pulse_position(0x00FF00, 3, 1, 15, 0, true, "strobe_pulse")
var strobe_intensity = animation.square(50, 255, 800, 25)  # Fast strobe, 25% duty cycle
var strobe_pos = animation.linear(leds-10, 5, 6000)  # Opposite direction
strobe_pulse.set_param_value("opacity", strobe_intensity)
strobe_pulse.set_param_value("pos", strobe_pos)
strobe_pulse.priority = 15
engine.add_animation(strobe_pulse)

# 3. Color-cycling pulse with dynamic color
var rainbow_pulse = animation.pulse_position(0x0000FF, 8, 2, 20, 0, true, "rainbow_pulse")
var rainbow_color = animation.rich_palette_color_provider.rainbow(5000, 1, 255)
var center_pos = animation.smooth(leds/4, 3*leds/4, 10000)
rainbow_pulse.set_param_value("color", rainbow_color)
rainbow_pulse.set_param_value("pos", center_pos)
rainbow_pulse.priority = 20
engine.add_animation(rainbow_pulse)

# 4. Fire pulse with fire palette
var fire_palette = animation.PALETTE_FIRE
var fire_color = animation.rich_palette_color_provider(fire_palette, 3000, 0, 255)
var fire_pulse = animation.pulse_position(0xFF4400, 6, 3, 25, 0, true, "fire_pulse")
var fire_size = animation.linear(4, 15, 7000)
fire_pulse.set_param_value("color", fire_color)
fire_pulse.set_param_value("pulse_size", fire_size)
fire_pulse.set_param_value("pos", leds/2)  # Fixed center position
fire_pulse.priority = 25
engine.add_animation(fire_pulse)

# Interactive event handlers for dynamic control
animation.register_event_handler("speed_up",
  def(event_data)
    print("Speed up event - doubling all animation speeds")
    for anim: engine.get_animations()
      if anim.name != "cycling_background"
        # Speed up oscillators by halving their duration
        for param_name: ["pulse_size", "pos", "opacity", "color"]
          var param_value = anim.get_param(param_name, nil)
          if animation.is_value_provider(param_value)
            if param_value.find("duration_ms") != nil
              param_value.duration_ms = param_value.duration_ms / 2
            end
          end
        end
      end
    end
  end,
  10, nil, {"description": "Double animation speeds"}
)

animation.register_event_handler("slow_down", 
  def(event_data)
    print("Slow down event - halving all animation speeds")
    for anim: engine.get_animations()
      if anim.name != "cycling_background"
        # Slow down oscillators by doubling their duration
        for param_name: ["pulse_size", "pos", "opacity", "color"]
          var param_value = anim.get_param(param_name, nil)
          if animation.is_value_provider(param_value)
            if param_value.find("duration_ms") != nil
              param_value.duration_ms = param_value.duration_ms * 2
            end
          end
        end
      end
    end
  end,
  10, nil, {"description": "Halve animation speeds"}
)

animation.register_event_handler("randomize_positions",
  def(event_data)
    print("Randomize positions event - setting random pulse positions")
    import math
    for anim: engine.get_animations()
      if anim.name[0..4] == "pulse" || anim.name[-5..-1] == "pulse"
        var random_pos = int(math.rand() % (leds - 10)) + 5
        anim.set_param_value("pos", random_pos)
      end
    end
  end,
  5, nil, {"description": "Randomize pulse positions"}
)

animation.register_event_handler("toggle_pulse",
  def(event_data)
    var pulse_name = event_data.find("pulse_name", "breathe_pulse")
    print("Toggle pulse event - toggling", pulse_name)
    
    var found = false
    for anim: engine.get_animations()
      if anim.name == pulse_name
        engine.remove_animation(anim)
        found = true
        break
      end
    end
    
    if !found
      # Re-add the pulse based on name
      if pulse_name == "breathe_pulse"
        engine.add_animation(breathe_pulse)
      elif pulse_name == "strobe_pulse"
        engine.add_animation(strobe_pulse)
      elif pulse_name == "rainbow_pulse"
        engine.add_animation(rainbow_pulse)
      elif pulse_name == "fire_pulse"
        engine.add_animation(fire_pulse)
      end
    end
  end,
  8, nil, {"description": "Toggle individual pulses on/off"}
)

engine.start()

print("Dynamic Pulse Demo - Multiple pulse types with interactive control")
print("Features:")
print("- 4 different pulse types with unique characteristics")
print("- Dynamic parameters using value providers")
print("- Interactive event system for real-time control")
print("- Background color cycling")
print("- Layered composition with priorities")
print("\nPulse types:")
print("- Breathe: Red pulse with oscillating size and position")
print("- Strobe: Green pulse with square wave intensity")
print("- Rainbow: Color-cycling pulse with smooth movement")
print("- Fire: Fire-colored pulse with growing size")
print("\nInteractive events:")
print("- animation.trigger_event('speed_up', {})")
print("- animation.trigger_event('slow_down', {})")
print("- animation.trigger_event('randomize_positions', {})")
print("- animation.trigger_event('toggle_pulse', {'pulse_name': 'breathe_pulse'})")
print("\nPress Ctrl+C to stop")