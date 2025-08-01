# this is the entry point for animation Framework
# it imports all other modules and register in the "animation" object
#
# launch with "./berry -s -g -m lib/libesp32/berry_animation"

# import in global scope all that is needed
import global
if !global.contains("tasmota")
  import tasmota
end

#@ solidify:animation,weak
var animation = module("animation")
global.animation = animation

# Version information
# Format: 0xAABBCCDD (AA=major, BB=minor, CC=patch, DD=build)
animation.VERSION = 0x00010000

# Convert version number to string format "major.minor.patch"
 def animation_version_string(version_num)
  if version_num == nil version_num = animation.VERSION end
  var major = (version_num >> 24) & 0xFF
  var minor = (version_num >> 16) & 0xFF
  var patch = (version_num >> 8) & 0xFF
  return f"{major}.{minor}.{patch}"
end
animation.version_string = animation_version_string

import sys

# Takes a map returned by "import XXX" and add each key/value to module `animation`
def register_to_animation(m)
  for k: m.keys()
    animation.(k) = m[k]
  end
end

# Import the core classes
import "core/frame_buffer" as frame_buffer
register_to_animation(frame_buffer)
import "core/pattern_base" as pattern_base
register_to_animation(pattern_base)
import "core/animation_base" as animation_base
register_to_animation(animation_base)
import "core/sequence_manager" as sequence_manager
register_to_animation(sequence_manager)

# Import the unified animation engine
import "core/animation_engine" as animation_engine
register_to_animation(animation_engine)

# Import event system
import "core/event_handler" as event_handler
register_to_animation(event_handler)

# Import user functions registry
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

# Global variable resolver with error checking
# First checks animation module, then global scope
def animation_global(name, module_name)
  import global
  import introspect
  import animation
  
  # First try to find in animation module
  if (module_name != nil) && introspect.contains(animation, module_name)
    return animation.(module_name)
  end
  
  # Then try global scope
  if global.contains(name)
    return global.(name)
  else
    raise "syntax_error", f"'{name}' undeclared"
  end
end
animation.global = animation_global

def animation_init(m)
  var animation_new = module("animation")   # create new non-solidified module
  animation_new._ntv = m                    # keep the native module
  animation_new.event_manager = m.EventManager()  # create monad for event manager
  
  # create a member function that looks in current module then in solidified
  animation_new.member = def (k)
    import animation
    import introspect
    if introspect.contains(animation._ntv, k)
      return animation._ntv.(k)
    else
      return module("undefined")
    end
  end

  return animation_new
end
animation.init = animation_init

return animation
