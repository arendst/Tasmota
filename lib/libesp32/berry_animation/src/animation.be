# Berry Animation Framework - Main Entry Point
# 
# This is the central module that imports and registers all animation framework components
# into a unified "animation" object for use in Tasmota LED strip control.
#
# The framework provides:
# - DSL (Domain Specific Language) for declarative animation definitions  
# - Value providers for dynamic parameters (oscillators, color providers)
# - Event system for interactive animations
# - Optimized performance for embedded ESP32 systems
#
# Usage in Tasmota:
#   import animation
#   var engine = animation.create_engine(strip)
#   var pulse_anim = animation.pulse(animation.solid(0xFF0000), 2000, 50, 255)
#   engine.add(pulse_anim).start()
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

# Parameter constraint encoder for PARAMS definitions
import "core/param_encoder" as param_encoder
register_to_animation(param_encoder)

# Mathematical functions for use in closures and throughout the framework
import "core/math_functions" as math_functions
register_to_animation(math_functions)

# Base class for parameter management and playable behavior - shared by Animation and ValueProvider
import "core/parameterized_object" as parameterized_object
register_to_animation(parameterized_object)

# Frame buffer management for LED strip pixel data
import "core/frame_buffer" as frame_buffer
register_to_animation(frame_buffer)

# Base Animation class - unified foundation for all visual elements
import "core/animation_base" as animation_base
register_to_animation(animation_base)

# Sequence manager for complex animation choreography
import "core/sequence_manager" as sequence_manager
register_to_animation(sequence_manager)

# Engine proxy - combines rendering and orchestration
import "core/engine_proxy" as engine_proxy
register_to_animation(engine_proxy)

# Unified animation engine - central engine for all animations
# Provides priority-based layering, automatic blending, and performance optimization
import "core/animation_engine" as animation_engine
register_to_animation(animation_engine)

# Event system for interactive animations (button presses, timers, etc.)
import "core/event_handler" as event_handler
register_to_animation(event_handler)

# User-defined function registry for DSL extensibility
import "core/user_functions" as user_functions
register_to_animation(user_functions)

# Import and register actual user functions
# try
#   import "user_functions" as user_funcs  # This registers the actual user functions
# except .. as e, msg
#   # User functions are optional - continue without them if not available
#   print(f"Note: User functions not loaded: {msg}")
# end

# Import value providers
import "providers/value_provider.be" as value_provider
register_to_animation(value_provider)
import "providers/static_value_provider.be" as static_value_provider
register_to_animation(static_value_provider)
import "providers/oscillator_value_provider.be" as oscillator_value_provider
register_to_animation(oscillator_value_provider)
import "providers/strip_length_provider.be" as strip_length_provider
register_to_animation(strip_length_provider)
import "providers/iteration_number_provider.be" as iteration_number_provider
register_to_animation(iteration_number_provider)
import "providers/closure_value_provider.be" as closure_value_provider
register_to_animation(closure_value_provider)

# Import color providers
import "providers/color_provider.be" as color_provider
register_to_animation(color_provider)
import "providers/color_cycle_color_provider.be" as color_cycle_color_provider
register_to_animation(color_cycle_color_provider)
# import "providers/composite_color_provider.be" as composite_color_provider
# register_to_animation(composite_color_provider)
import "providers/rich_palette_color_provider.be" as rich_palette_color_provider
register_to_animation(rich_palette_color_provider)
import "providers/breathe_color_provider.be" as breathe_color_provider
register_to_animation(breathe_color_provider)

# Import animations
import "animations/solid" as solid_impl
register_to_animation(solid_impl)
import "animations/beacon" as beacon
register_to_animation(beacon)
import "animations/crenel" as crenel
register_to_animation(crenel)
import "animations/breathe" as breathe
register_to_animation(breathe)
import "animations/palette_gradient" as palette_pattern_animation
register_to_animation(palette_pattern_animation)
import "animations/comet" as comet
register_to_animation(comet)
# import "animations/fire" as fire
# register_to_animation(fire)
import "animations/twinkle" as twinkle
register_to_animation(twinkle)
import "animations/gradient" as gradient
register_to_animation(gradient)
import "animations/palette_meter" as palette_meter
register_to_animation(palette_meter)
# import "animations/plasma" as plasma_animation
# register_to_animation(plasma_animation)
# import "animations/sparkle" as sparkle_animation
# register_to_animation(sparkle_animation)
# import "animations/wave" as wave
# register_to_animation(wave)
# import "animations/shift" as shift_animation
# register_to_animation(shift_animation)
# import "animations/bounce" as bounce_animation
# register_to_animation(bounce_animation)
# import "animations/scale" as scale_animation
# register_to_animation(scale_animation)
# import "animations/jitter" as jitter_animation
# register_to_animation(jitter_animation)

# Import palette examples
import "animations/palettes" as palettes
register_to_animation(palettes)

# Import specialized animation classes
import "animations/rich_palette" as rich_palette
register_to_animation(rich_palette)

# DSL components are now in separate animation_dsl module

# Function called to initialize the `Leds` and `engine` objects
#
# It keeps track of previously created engines and strips to reuse
# when called with the same arguments
#
# Parameters:
#   l - list of arguments (vararg)
#
# Returns:
#   An instance of `AnimationEngine` managing the strip
def animation_init_strip(*l)
  import global
  import animation
  import introspect
  # we keep a hash of strip configurations to reuse existing engines
  if !introspect.contains(animation, "_engines")
    animation._engines = {}
  end

  var l_as_string = str(l)
  var engine = animation._engines.find(l_as_string)
  if (engine != nil)
    # we reuse it
    engine.stop()
    engine.clear()
  else
    var strip = call(global.Leds, l)    # call global.Leds() with vararg
    engine = animation.create_engine(strip)
    animation._engines[l_as_string] = engine
  end

  return engine
end
animation.init_strip = animation_init_strip

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

  # Create an empty map for user_functions
  animation_new._user_functions = {}

  return animation_new
end
animation.init = animation_init

return animation
