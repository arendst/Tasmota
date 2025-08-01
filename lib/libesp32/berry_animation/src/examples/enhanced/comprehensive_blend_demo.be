#
# Comprehensive blending demonstration with multiple animations
# Showcases: Advanced blending, multiple layers, dynamic composition
# Demonstrates: Frame buffer blending, animation layering, blend modes
#

import animation

var leds = 40  # Good size for blending demonstration

# Create LED strip
var strip = global.Leds(leds, gpio.pin(gpio.WS2812, 0))
var engine = animation.create_engine(strip)

# Layer 1: Base gradient background
var gradient_colors = [0x440000, 0x004400, 0x000044, 0x444400]
var gradient_provider = animation.color_cycle_color_provider(gradient_colors, 20000, 1)
var gradient_bg = animation.filled_animation(gradient_provider, 0, 0, true, "gradient_background")
gradient_bg.set_param_value("opacity", 120)  # Semi-transparent base
engine.add_animation(gradient_bg)

# Layer 2: Moving comet with trail
var comet_color = animation.rich_palette_color_provider.rainbow(8000, 1, 255)
var comet = animation.comet(comet_color, 12, 6, 4000, true, true, "rainbow_comet")
comet.priority = 10
comet.set_param_value("opacity", 200)
engine.add_animation(comet)

# Layer 3: Breathing pulse overlay
var pulse_color = animation.color_cycle_color_provider([
  0xFFFFFF, 0xFF8080, 0x80FF80, 0x8080FF
], 6000, 1)
var breathe = animation.breathe_animation(pulse_color, 180, 3000, true, "color_breathe")
breathe.priority = 20
var breathe_opacity = animation.smooth(80, 220, 5000)
breathe.set_param_value("opacity", breathe_opacity)
engine.add_animation(breathe)

# Layer 4: Sparkle effects
var sparkle = animation.twinkle_animation(0xFFFFFF, 6, 200, 0, true, "white_sparkles")
sparkle.priority = 30
sparkle.set_param_value("opacity", 150)
engine.add_animation(sparkle)

# Layer 5: Position-based pulse that moves
var moving_pulse = animation.pulse_position(0xFF00FF, 8, 2, 35, 0, true, "moving_pulse")
var pulse_pos = animation.linear(2, leds-10, 7000)
var pulse_size = animation.smooth(4, 16, 4500)
moving_pulse.set_param_value("pos", pulse_pos)
moving_pulse.set_param_value("pulse_size", pulse_size)
moving_pulse.priority = 25
moving_pulse.set_param_value("opacity", 180)
engine.add_animation(moving_pulse)

# Demonstration of manual frame buffer blending
def demonstrate_manual_blending()
  print("\n--- Manual Frame Buffer Blending Demo ---")
  
  # Create separate frame buffers
  var frame1 = animation.frame_buffer(leds)
  var frame2 = animation.frame_buffer(leds)
  var frame3 = animation.frame_buffer(leds)
  
  # Fill frame1 with red gradient
  for i: 0..leds-1
    var intensity = int(tasmota.scale_uint(i, 0, leds-1, 50, 255))
    frame1.set_pixel(i, intensity, 0, 0, 200)
  end
  
  # Fill frame2 with green wave pattern
  import math
  for i: 0..leds-1
    var wave = int((math.sin(i * 0.3) + 1) * 127)
    frame2.set_pixel(i, 0, wave, 0, 150)
  end
  
  # Fill frame3 with blue sparkles
  for i: 0..leds-1
    if (i % 7) == 0  # Every 7th pixel
      frame3.set_pixel(i, 0, 0, 255, 180)
    end
  end
  
  # Blend all frames together
  frame1.blend(frame2, 255)  # Blend green wave onto red gradient
  frame1.blend(frame3, 255)  # Blend blue sparkles on top
  
  print("Manual blending result:")
  print("Frame1 (red gradient):", frame1.pixels[0..20].tohex())
  print("Combined result shows additive blending of RGB channels")
  
  return frame1
end

# Event handlers for blending control
animation.register_event_handler("toggle_layer",
  def(event_data)
    var layer_name = event_data.find("layer", "rainbow_comet")
    print("Toggling layer:", layer_name)
    
    var found = false
    for anim: engine.get_animations()
      if anim.name == layer_name
        engine.remove_animation(anim)
        found = true
        break
      end
    end
    
    if !found
      # Re-add the animation based on name
      if layer_name == "rainbow_comet"
        engine.add_animation(comet)
      elif layer_name == "color_breathe"
        engine.add_animation(breathe)
      elif layer_name == "white_sparkles"
        engine.add_animation(sparkle)
      elif layer_name == "moving_pulse"
        engine.add_animation(moving_pulse)
      end
    end
  end,
  10, nil, {"description": "Toggle individual layers"}
)

animation.register_event_handler("adjust_opacity",
  def(event_data)
    var layer = event_data.find("layer", "all")
    var opacity = event_data.find("opacity", 128)
    
    print("Adjusting opacity for", layer, "to", opacity)
    
    if layer == "all"
      for anim: engine.get_animations()
        if anim.name != "gradient_background"
          anim.set_param_value("opacity", opacity)
        end
      end
    else
      for anim: engine.get_animations()
        if anim.name == layer
          anim.set_param_value("opacity", opacity)
          break
        end
      end
    end
  end,
  8, nil, {"description": "Adjust layer opacity"}
)

animation.register_event_handler("demo_manual_blend",
  def(event_data)
    demonstrate_manual_blending()
  end,
  5, nil, {"description": "Demonstrate manual frame buffer blending"}
)

animation.register_event_handler("blend_mode_info",
  def(event_data)
    print("\n--- Blending Information ---")
    print("Current layers (bottom to top):")
    var anims = engine.get_animations()
    # Sort by priority for display
    for i: 0..anims.size()-1
      for j: i+1..anims.size()-1
        if anims[i].priority > anims[j].priority
          var temp = anims[i]
          anims[i] = anims[j]
          anims[j] = temp
        end
      end
    end
    
    for anim: anims
      var opacity = anim.get_param("opacity", 255)
      print("- " + anim.name + " (priority: " + str(anim.priority) + ", opacity: " + str(opacity) + ")")
    end
    
    print("\nBlending occurs in priority order (low to high)")
    print("Each layer blends with the accumulated result below it")
    print("Alpha values control transparency and blending intensity")
  end,
  1, nil, {"description": "Show current blending configuration"}
)

# Start the engine
engine.start()

# Show initial manual blending demo
demonstrate_manual_blending()

print("Comprehensive Blend Demo - Advanced layering and blending")
print("Features:")
print("- 5 animation layers with different priorities")
print("- Dynamic opacity control using value providers")
print("- Multiple blend types (gradient, comet, breathe, sparkle, pulse)")
print("- Manual frame buffer blending demonstration")
print("- Interactive layer control")
print("\nLayers (priority order):")
print("0. Gradient background (priority 0)")
print("1. Rainbow comet (priority 10)")
print("2. Color breathe (priority 20)")
print("3. Moving pulse (priority 25)")
print("4. White sparkles (priority 30)")
print("\nInteractive events:")
print("- animation.trigger_event('toggle_layer', {'layer': 'rainbow_comet'})")
print("- animation.trigger_event('adjust_opacity', {'layer': 'all', 'opacity': 100})")
print("- animation.trigger_event('demo_manual_blend', {})")
print("- animation.trigger_event('blend_mode_info', {})")
print("\nPress Ctrl+C to stop")