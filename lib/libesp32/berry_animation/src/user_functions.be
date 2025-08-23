# User-defined functions for Animation DSL
# This file demonstrates how to create custom functions that can be used in the DSL

import animation

# Example 1: Simple breathing effect
def breathing_effect(engine, base_color, period)
  # Create a pulse animation with the specified color and period
  var pulse_anim = animation.pulsating_animation(engine)
  pulse_anim.color = base_color
  pulse_anim.min_brightness = 50
  pulse_anim.max_brightness = 255
  pulse_anim.period = period
  return pulse_anim
end

# Example 2: Police lights effect  
def police_lights(engine, flash_speed)
  # Create alternating red/blue flash effect
  var pulse_anim = animation.pulsating_animation(engine)
  pulse_anim.color = 0xFFFF0000  # Red
  pulse_anim.min_brightness = 0
  pulse_anim.max_brightness = 255
  pulse_anim.period = flash_speed
  return pulse_anim
end

# Example 3: Fire effect with customizable intensity
def fire_effect(engine, intensity, speed)
  # Use the fire palette with custom parameters
  var color_provider = animation.rich_palette(engine)
  color_provider.palette = animation.PALETTE_FIRE
  color_provider.cycle_period = speed
  color_provider.easing = 1  # Smooth transitions
  
  var fire_anim = animation.filled(engine)
  fire_anim.color_provider = color_provider
  fire_anim.brightness = intensity
  return fire_anim
end

# Example 4: Sparkle effect
def sparkle_effect(engine, base_color, sparkle_color, density)
  # Create a twinkling sparkle effect
  var sparkle_anim = animation.twinkle_animation(engine)
  sparkle_anim.color = sparkle_color
  sparkle_anim.density = density
  sparkle_anim.speed = 500
  return sparkle_anim
end

# Example 5: Color wheel effect
def color_wheel(engine, speed)
  # Create a rainbow that cycles through colors
  var color_provider = animation.rich_palette(engine)
  color_provider.palette = animation.PALETTE_RAINBOW
  color_provider.cycle_period = speed
  color_provider.easing = 1  # Smooth transitions
  
  var rainbow_anim = animation.filled(engine)
  rainbow_anim.color_provider = color_provider
  rainbow_anim.brightness = 255
  return rainbow_anim
end

# Example 6: Comet effect with custom tail
def comet_effect(engine, color, tail_length, speed)
  # Create a moving comet with customizable tail
  var comet_anim = animation.comet_animation(engine)
  comet_anim.color = color
  comet_anim.tail_length = tail_length
  comet_anim.speed = speed
  return comet_anim
end

# Example 7: Pulsing position effect
def pulse_spot(engine, color, position, width, period)
  # Create a pulsing effect at a specific position
  var pulse_pos_anim = animation.beacon_animation(engine)
  pulse_pos_anim.color = color
  pulse_pos_anim.position = position
  pulse_pos_anim.width = width
  pulse_pos_anim.period = period
  return pulse_pos_anim
end

# Register all user functions with the animation module
animation.register_user_function("breathing", breathing_effect)
animation.register_user_function("police_lights", police_lights)
animation.register_user_function("fire", fire_effect)
animation.register_user_function("sparkle", sparkle_effect)
animation.register_user_function("color_wheel", color_wheel)
animation.register_user_function("comet_effect", comet_effect)
animation.register_user_function("pulse_spot", pulse_spot)

print("User functions registered:")
var functions = animation.list_user_functions()
for func_name : functions
  print(f"  - {func_name}")
end