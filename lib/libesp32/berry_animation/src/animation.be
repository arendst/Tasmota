# Berry Animation Framework - Main Entry Point
# 
# This is the central module that imports and registers all animation framework components
# into a unified "animation" object for use in Tasmota LED strip control.
#
# The framework provides:
# - Unified Pattern-Animation architecture (Animation extends Pattern)
# - DSL (Domain Specific Language) for declarative animation definitions  
# - Value providers for dynamic parameters (oscillators, color providers)
# - Event system for interactive animations
# - Optimized performance for embedded ESP32 systems
#
# Usage in Tasmota:
#   import animation
#   var engine = animation.create_engine(strip)
#   var pulse_anim = animation.pulse(animation.solid(0xFF0000), 2000, 50, 255)
#   engine.add_animation(pulse_anim).start()
#
# Launch standalone with: "./berry -s -g -m lib/libesp32/berry_animation"

# Import Tasmota integration if available (for embedded use)
import global
if !global.contains("tasmota")
  import tasmota
end

# Create the main animation module and make it globally accessible
# The @solidify directive enables compilation to C++ for performance
#@ solidify:animation,weak
var animation = module("animation")
global.animation = animation

# Version information for compatibility tracking
# Format: 0xAABBCCDD (AA=major, BB=minor, CC=patch, DD=build)
animation.VERSION = 0x00010000

# Convert version number to human-readable string format "major.minor.patch"
def animation_version_string(version_num)
  if version_num == nil version_num = animation.VERSION end
  var major = (version_num >> 24) & 0xFF
  var minor = (version_num >> 16) & 0xFF
  var patch = (version_num >> 8) & 0xFF
  return f"{major}.{minor}.{patch}"
end
animation.version_string = animation_version_string

import sys

# Helper function to register all exports from imported modules into the main animation object
# This creates a flat namespace where all animation functions are accessible as animation.function_name()
# Takes a map returned by "import XXX" and adds each key/value to module `animation`
def register_to_animation(m)
  for k: m.keys()
    animation.(k) = m[k]
  end
end

# Import core framework components
# These provide the fundamental architecture for the animation system

# Frame buffer management for LED strip pixel data
import "core/frame_buffer" as frame_buffer
register_to_animation(frame_buffer)

# Base Pattern class - foundation for all visual elements
import "core/pattern_base" as pattern_base
register_to_animation(pattern_base)

# Base Animation class - extends Pattern with temporal behavior
import "core/animation_base" as animation_base
register_to_animation(animation_base)

# Sequence manager for complex animation choreography
import "core/sequence_manager" as sequence_manager
register_to_animation(sequence_manager)

# Unified animation engine - central controller for all animations
# Provides priority-based layering, automatic blending, and performance optimization
import "core/animation_engine" as animation_engine
register_to_animation(animation_engine)

# Event system for interactive animations (button presses, timers, etc.)
import "core/event_handler" as event_handler
register_to_animation(event_handler)

# User-defined function registry for DSL extensibility
import "core/user_functions" as user_functions
register_to_animation(user_functions)

# Import effects
import "effects/filled" as filled_animation
register_to_animation(filled_animation)
import "effects/pulse" as pulse_animation
register_to_animation(pulse_animation)
import "effects/pulse_position" as pulse_position_animation
register_to_animation(pulse_position_animation)
import "effects/crenel_position" as crenel_position_animation
register_to_animation(crenel_position_animation)
import "effects/breathe" as breathe_animation
register_to_animation(breathe_animation)
import "effects/palette_pattern" as palette_pattern_animation
register_to_animation(palette_pattern_animation)
import "effects/comet" as comet_animation
register_to_animation(comet_animation)
import "effects/fire" as fire_animation
register_to_animation(fire_animation)
import "effects/twinkle" as twinkle_animation
register_to_animation(twinkle_animation)
import "effects/gradient" as gradient_animation
register_to_animation(gradient_animation)
import "effects/noise" as noise_animation
register_to_animation(noise_animation)
import "effects/plasma" as plasma_animation
register_to_animation(plasma_animation)
import "effects/sparkle" as sparkle_animation
register_to_animation(sparkle_animation)
import "effects/wave" as wave_animation
register_to_animation(wave_animation)
import "effects/shift" as shift_animation
register_to_animation(shift_animation)
import "effects/bounce" as bounce_animation
register_to_animation(bounce_animation)
import "effects/scale" as scale_animation
register_to_animation(scale_animation)
import "effects/jitter" as jitter_animation
register_to_animation(jitter_animation)

# Import palette examples
import "effects/palettes" as palettes
register_to_animation(palettes)

# Import pattern implementations
import "patterns/solid_pattern" as solid_pattern_impl
register_to_animation(solid_pattern_impl)

# Import animation implementations
# Note: pulse_animation is already imported from effects/pulse.be
import "effects/pattern_animation" as pattern_animation_impl
register_to_animation(pattern_animation_impl)

# Import value providers
import "providers/value_provider.be" as value_provider
register_to_animation(value_provider)
import "providers/static_value_provider.be" as static_value_provider
register_to_animation(static_value_provider)
import "providers/oscillator_value_provider.be" as oscillator_value_provider
register_to_animation(oscillator_value_provider)

# Import color providers
import "providers/color_provider.be" as color_provider
register_to_animation(color_provider)
import "providers/color_cycle_color_provider.be" as color_cycle_color_provider
register_to_animation(color_cycle_color_provider)
import "providers/composite_color_provider.be" as composite_color_provider
register_to_animation(composite_color_provider)
import "providers/solid_color_provider.be" as solid_color_provider
register_to_animation(solid_color_provider)
import "providers/rich_palette_color_provider.be" as rich_palette_color_provider
register_to_animation(rich_palette_color_provider)

# Import DSL components
import "dsl/token.be" as dsl_token
register_to_animation(dsl_token)
import "dsl/lexer.be" as dsl_lexer
register_to_animation(dsl_lexer)
import "dsl/transpiler.be" as dsl_transpiler
register_to_animation(dsl_transpiler)
import "dsl/runtime.be" as dsl_runtime
register_to_animation(dsl_runtime)

# Function called to initialize the `Leds` and `engine` objects
#
# Parameters:
#   l - list of arguments (vararg)
#
# Returns:
#   An instance of `AnimationEngine` managing the strip
def animation_init_strip(*l)
  import global
  import animation
  var strip = call(global.Leds, l)    # call global.Leds() with vararg
  var engine = animation.create_engine(strip)
  return engine
end
animation.init_strip = animation_init_strip

# Global variable resolver with error checking
# Used by DSL transpiler to resolve variable names during compilation
# First checks animation module, then global scope for user-defined variables
def animation_global(name, module_name)
  import global
  import introspect
  import animation
  
  # First try to find in animation module (built-in functions/classes)
  if (module_name != nil) && introspect.contains(animation, module_name)
    return animation.(module_name)
  end
  
  # Then try global scope (user-defined variables)
  if global.contains(name)
    return global.(name)
  else
    raise "syntax_error", f"'{name}' undeclared"
  end
end
animation.global = animation_global

# This function is called from C++ code to set up the Berry animation environment
# It creates a mutable 'animation' module on top of the immutable solidified
#
# Parameters:
#   m - Solidified immutable module
#
# Returns:
#   A new animation module instance that is return for `import animation`
def animation_init(m)
  var animation_new = module("animation")         # Create new non-solidified module for runtime use
  animation_new._ntv = m                          # Keep reference to native solidified module
  animation_new.event_manager = m.EventManager()  # Create event manager instance for handling triggers
  
  # Create dynamic member lookup function for extensibility
  # This allows the module to find members in both Berry and solidified components
  #
  # Note: if the module already contained the member, then `member()` would not be called in the first place
  animation_new.member = def (k)
    import animation
    import introspect
    if introspect.contains(animation._ntv, k)
      return animation._ntv.(k)              # Return native solidified member if available
    else
      return module("undefined")             # Return undefined module for missing members
    end
  end

  return animation_new
end
animation.init = animation_init

return animation
