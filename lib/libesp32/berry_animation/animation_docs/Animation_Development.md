# Animation Development Guide

Guide for developers creating custom animation classes in the Berry Animation Framework.

## Overview

**Note**: This guide is for developers who want to extend the framework by creating new animation classes. For using existing animations, see the [DSL Reference](Dsl_Reference.md) which provides a declarative way to create animations without programming.

The Berry Animation Framework uses a unified architecture where all visual elements inherit from the base `Animation` class. This guide explains how to create custom animation classes that integrate seamlessly with the framework's parameter system, value providers, and rendering pipeline.

## Animation Class Structure

### Basic Class Template

```berry
#@ solidify:MyAnimation,weak
class MyAnimation : animation.animation
  # NO instance variables for parameters - they are handled by the virtual parameter system
  
  # Parameter definitions following the new specification
  static var PARAMS = {
    "my_param1": {"default": "default_value", "type": "string"},
    "my_param2": {"min": 0, "max": 255, "default": 100, "type": "int"}
    # Do NOT include inherited Animation parameters here
  }
  
  def init(engine)
    # Engine parameter is MANDATORY and cannot be nil
    super(self).init(engine)
    
    # Only initialize non-parameter instance variables (none in this example)
    # Parameters are handled by the virtual parameter system
  end
  
  # Handle parameter changes (optional)
  def on_param_changed(name, value)
    # Add custom logic for parameter changes if needed
    # Parameter validation is handled automatically by the framework
  end
  
  # Update animation state (no return value needed)
  def update(time_ms)
    super(self).update(time_ms)
    # Your update logic here
  end
  
  def render(frame, time_ms, strip_length)
    if !self.is_running || frame == nil
      return false
    end
    
    # Use virtual parameter access - automatically resolves ValueProviders
    var param1 = self.my_param1
    var param2 = self.my_param2
    
    # Use strip_length parameter instead of self.engine.strip_length for performance
    # Your rendering logic here
    # ...
    
    return true
  end
  
  # NO setter methods needed - use direct virtual parameter assignment:
  # obj.my_param1 = value
  # obj.my_param2 = value
  
  def tostring()
    return f"MyAnimation(param1={self.my_param1}, param2={self.my_param2}, running={self.is_running})"
  end
end
```

## PARAMS System

### Static Parameter Definition

The `PARAMS` static variable defines all parameters specific to your animation class. This system provides:

- **Parameter validation** with min/max constraints and type checking
- **Default value handling** for initialization
- **Virtual parameter access** through getmember/setmember
- **Automatic ValueProvider resolution**

#### Parameter Definition Format

```berry
static var PARAMS = {
  "parameter_name": {
    "default": default_value,    # Default value (optional)
    "min": minimum_value,        # Minimum value for integers (optional)
    "max": maximum_value,        # Maximum value for integers (optional)
    "enum": [val1, val2, val3],  # Valid enum values (optional)
    "type": "parameter_type",    # Expected type (optional)
    "nillable": true             # Whether nil values are allowed (optional)
  }
}
```

#### Supported Types

- **`"int"`** - Integer values (default if not specified)
- **`"string"`** - String values
- **`"bool"`** - Boolean values (true/false)
- **`"bytes"`** - Bytes objects (validated using isinstance())
- **`"instance"`** - Object instances
- **`"any"`** - Any type (no type validation)

#### Important Rules

- **Do NOT include inherited parameters** - Animation base class parameters are handled automatically
- **Only define class-specific parameters** in your PARAMS
- **No constructor parameter mapping** - the new system uses engine-only constructors
- **Parameters are accessed via virtual members**: `obj.param_name`

## Constructor Implementation

### Engine-Only Constructor Pattern

```berry
def init(engine)
  # 1. ALWAYS call super with engine (engine is the ONLY parameter)
  super(self).init(engine)
  
  # 2. Initialize non-parameter instance variables only
  self.internal_state = initial_value
  self.buffer = nil
  # Do NOT initialize parameters here - they are handled by the virtual system
end
```

### Parameter Change Handling

```berry
def on_param_changed(name, value)
  # Optional method to handle parameter changes
  if name == "scale"
    # Recalculate internal state when scale changes
    self._update_internal_buffers()
  elif name == "color"
    # Handle color changes
    self._invalidate_color_cache()
  end
end
```

### Key Changes from Old System

- **Engine-only constructor**: Constructor takes ONLY the engine parameter
- **No parameter initialization**: Parameters are set by caller using virtual member assignment
- **No instance variables for parameters**: Parameters are handled by the virtual system
- **Automatic validation**: Parameter validation happens automatically based on PARAMS constraints

## Value Provider Integration

### Automatic ValueProvider Resolution

The virtual parameter system automatically resolves ValueProviders when you access parameters:

```berry
def render(frame, time_ms, strip_length)
  # Virtual parameter access automatically resolves ValueProviders
  var color = self.color      # Returns current color value, not the provider
  var position = self.pos     # Returns current position value
  var size = self.size        # Returns current size value
  
  # Use strip_length parameter (computed once by engine_proxy) instead of self.engine.strip_length
  # Use resolved values in rendering logic
  for i: position..(position + size - 1)
    if i >= 0 && i < strip_length
      frame.set_pixel_color(i, color)
    end
  end
  
  return true
end
```

### Setting Dynamic Parameters

Users can set both static values and ValueProviders using the same syntax:

```berry
# Create animation
var anim = animation.my_animation(engine)

# Static values
anim.color = 0xFFFF0000
anim.pos = 5
anim.size = 3

# Dynamic values
anim.color = animation.smooth(0xFF000000, 0xFFFFFFFF, 2000)
anim.pos = animation.triangle(0, 29, 3000)
```

### Performance Optimization

For performance-critical code, cache parameter values:

```berry
def render(frame, time_ms, strip_length)
  # Cache parameter values to avoid multiple virtual member access
  var current_color = self.color
  var current_pos = self.pos
  var current_size = self.size
  
  # Use cached values in loops
  for i: current_pos..(current_pos + current_size - 1)
    if i >= 0 && i < strip_length
      frame.set_pixel_color(i, current_color)
    end
  end
  
  return true
end
```

### Color Provider LUT Optimization

For color providers that perform expensive color calculations (like palette interpolation), the base `ColorProvider` class provides a Lookup Table (LUT) mechanism for caching pre-computed colors:

```berry
#@ solidify:MyColorProvider,weak
class MyColorProvider : animation.color_provider
  # Instance variables (all should start with underscore)
  var _cached_data    # Your custom cached data
  
  def init(engine)
    super(self).init(engine)  # Initializes _color_lut and _lut_dirty
    self._cached_data = nil
  end
  
  # Mark LUT as dirty when parameters change
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    if name == "colors" || name == "transition_type"
      self._lut_dirty = true  # Inherited from ColorProvider
    end
  end
  
  # Rebuild LUT when needed
  def _rebuild_color_lut()
    # Allocate LUT (e.g., 129 entries * 4 bytes = 516 bytes)
    if self._color_lut == nil
      self._color_lut = bytes()
      self._color_lut.resize(129 * 4)
    end
    
    # Pre-compute colors for values 0, 2, 4, ..., 254, 255
    var i = 0
    while i < 128
      var value = i * 2
      var color = self._compute_color_expensive(value)
      self._color_lut.set(i * 4, color, 4)
      i += 1
    end
    
    # Add final entry for value 255
    var color_255 = self._compute_color_expensive(255)
    self._color_lut.set(128 * 4, color_255, 4)
    
    self._lut_dirty = false
  end
  
  # Update method checks if LUT needs rebuilding
  def update(time_ms)
    if self._lut_dirty || self._color_lut == nil
      self._rebuild_color_lut()
    end
    return self.is_running
  end
  
  # Fast color lookup using LUT
  def get_color_for_value(value, time_ms)
    # Build LUT if needed (lazy initialization)
    if self._lut_dirty || self._color_lut == nil
      self._rebuild_color_lut()
    end
    
    # Map value to LUT index (divide by 2, special case for 255)
    var lut_index = value >> 1
    if value >= 255
      lut_index = 128
    end
    
    # Retrieve pre-computed color from LUT
    var color = self._color_lut.get(lut_index * 4, 4)
    
    # Apply brightness scaling using static method (only if not 255)
    var brightness = self.brightness
    if brightness != 255
      return animation.color_provider.apply_brightness(color, brightness)
    end
    
    return color
  end
  
  # Access LUT from outside (returns bytes() or nil)
  # Inherited from ColorProvider: get_lut()
end
```

**LUT Benefits:**
- **5-10x speedup** for expensive color calculations
- **Reduced CPU usage** during rendering
- **Smooth animations** even with complex color logic
- **Memory efficient** (typically 516 bytes for 129 entries)

**When to use LUT:**
- Palette interpolation with binary search
- Complex color transformations
- Brightness calculations
- Any expensive per-pixel color computation

**LUT Guidelines:**
- Store colors at maximum brightness, apply scaling after lookup
- Use 2-step resolution (0, 2, 4, ..., 254, 255) to save memory
- Invalidate LUT when parameters affecting color calculation change
- Don't invalidate for brightness changes if brightness is applied post-lookup

**Brightness Handling:**

The `ColorProvider` base class includes a `brightness` parameter (0-255, default 255) and a static method for applying brightness scaling:

```berry
# Static method for brightness scaling (only scales if brightness != 255)
animation.color_provider.apply_brightness(color, brightness)
```

**Best Practices:**
- Store LUT colors at maximum brightness (255)
- Apply brightness scaling after LUT lookup using the static method
- Only call the static method if `brightness != 255` to avoid unnecessary overhead
- For inline performance-critical code, you can inline the brightness calculation instead of calling the static method
- Brightness changes do NOT invalidate the LUT since brightness is applied after lookup

## Parameter Access

### Direct Virtual Member Assignment

The new system uses direct parameter assignment instead of setter methods:

```berry
# Create animation
var anim = animation.my_animation(engine)

# Direct parameter assignment (recommended)
anim.color = 0xFF00FF00
anim.pos = 10
anim.size = 5

# Method chaining is not needed - just set parameters directly
```

### Parameter Validation

The parameter system handles validation automatically based on PARAMS constraints:

```berry
# This will raise an exception due to min: 0 constraint
anim.size = -1  # Raises value_error

# This will be accepted
anim.size = 5   # Parameter updated successfully

# Method-based setting returns true/false for validation
var success = anim.set_param("size", -1)  # Returns false, no exception
```

### Accessing Raw Parameters

```berry
# Get current parameter value (resolved if ValueProvider)
var current_color = anim.color

# Get raw parameter (returns ValueProvider if set)
var raw_color = anim.get_param("color")

# Check if parameter is a ValueProvider
if animation.is_value_provider(raw_color)
  print("Color is dynamic")
else
  print("Color is static")
end
```

## Rendering Implementation

### Frame Buffer Operations

```berry
def render(frame, time_ms, strip_length)
  if !self.is_running || frame == nil
    return false
  end
  
  # Resolve dynamic parameters
  var color = self.resolve_value(self.color, "color", time_ms)
  var opacity = self.resolve_value(self.opacity, "opacity", time_ms)
  
  # Render your effect using strip_length parameter
  for i: 0..(strip_length-1)
    var pixel_color = calculate_pixel_color(i, time_ms)
    frame.set_pixel_color(i, pixel_color)
  end
  
  # Apply opacity if not full (supports numbers, animations)
  if opacity < 255
    frame.apply_opacity(opacity)
  end
  
  return true  # Frame was modified
end
```

### Common Rendering Patterns

#### Fill Pattern
```berry
# Fill entire frame with color
frame.fill_pixels(color)
```

#### Position-Based Effects
```berry
# Render at specific positions
var start_pos = self.resolve_value(self.pos, "pos", time_ms)
var size = self.resolve_value(self.size, "size", time_ms)

for i: 0..(size-1)
  var pixel_pos = start_pos + i
  if pixel_pos >= 0 && pixel_pos < frame.width
    frame.set_pixel_color(pixel_pos, color)
  end
end
```

#### Gradient Effects
```berry
# Create gradient across frame
for i: 0..(frame.width-1)
  var progress = i / (frame.width - 1.0)  # 0.0 to 1.0
  var interpolated_color = interpolate_color(start_color, end_color, progress)
  frame.set_pixel_color(i, interpolated_color)
end
```

## Complete Example: BeaconAnimation

Here's a complete example showing all concepts:

```berry
#@ solidify:BeaconAnimation,weak
class BeaconAnimation : animation.animation
  # NO instance variables for parameters - they are handled by the virtual parameter system
  
  # Parameter definitions following the new specification
  static var PARAMS = {
    "color": {"default": 0xFFFFFFFF},
    "back_color": {"default": 0xFF000000},
    "pos": {"default": 0},
    "beacon_size": {"min": 0, "default": 1},
    "slew_size": {"min": 0, "default": 0}
  }
  
  # Initialize a new Pulse Position animation
  # Engine parameter is MANDATORY and cannot be nil
  def init(engine)
    # Call parent constructor with engine (engine is the ONLY parameter)
    super(self).init(engine)
    
    # Only initialize non-parameter instance variables (none in this case)
    # Parameters are handled by the virtual parameter system
  end
  
  # Handle parameter changes (optional - can be removed if no special handling needed)
  def on_param_changed(name, value)
    # No special handling needed for this animation
    # Parameter validation is handled automatically by the framework
  end
  
  # Render the pulse to the provided frame buffer
  def render(frame, time_ms, strip_length)
    if frame == nil
      return false
    end
    
    var pixel_size = strip_length
    # Use virtual parameter access - automatically resolves ValueProviders
    var back_color = self.back_color
    var pos = self.pos
    var slew_size = self.slew_size
    var beacon_size = self.beacon_size
    var color = self.color
    
    # Fill background if not transparent
    if back_color != 0xFF000000
      frame.fill_pixels(back_color)
    end
    
    # Calculate pulse boundaries
    var pulse_min = pos
    var pulse_max = pos + beacon_size
    
    # Clamp to frame boundaries
    if pulse_min < 0
      pulse_min = 0
    end
    if pulse_max >= pixel_size
      pulse_max = pixel_size
    end
    
    # Draw the main pulse
    var i = pulse_min
    while i < pulse_max
      frame.set_pixel_color(i, color)
      i += 1
    end
    
    # Draw slew regions if slew_size > 0
    if slew_size > 0
      # Left slew (fade from background to pulse color)
      var left_slew_min = pos - slew_size
      var left_slew_max = pos
      
      if left_slew_min < 0
        left_slew_min = 0
      end
      if left_slew_max >= pixel_size
        left_slew_max = pixel_size
      end
      
      i = left_slew_min
      while i < left_slew_max
        # Calculate blend factor
        var blend_factor = tasmota.scale_uint(i, pos - slew_size, pos - 1, 255, 0)
        var alpha = 255 - blend_factor
        var blend_color = (alpha << 24) | (color & 0x00FFFFFF)
        var blended_color = frame.blend(back_color, blend_color)
        frame.set_pixel_color(i, blended_color)
        i += 1
      end
      
      # Right slew (fade from pulse color to background)
      var right_slew_min = pos + beacon_size
      var right_slew_max = pos + beacon_size + slew_size
      
      if right_slew_min < 0
        right_slew_min = 0
      end
      if right_slew_max >= pixel_size
        right_slew_max = pixel_size
      end
      
      i = right_slew_min
      while i < right_slew_max
        # Calculate blend factor
        var blend_factor = tasmota.scale_uint(i, pos + beacon_size, pos + beacon_size + slew_size - 1, 0, 255)
        var alpha = 255 - blend_factor
        var blend_color = (alpha << 24) | (color & 0x00FFFFFF)
        var blended_color = frame.blend(back_color, blend_color)
        frame.set_pixel_color(i, blended_color)
        i += 1
      end
    end
    
    return true
  end
  
  # NO setter methods - use direct virtual parameter assignment instead:
  # obj.color = value
  # obj.pos = value  
  # obj.beacon_size = value
  # obj.slew_size = value
  
  # String representation of the animation
  def tostring()
    return f"BeaconAnimation(color=0x{self.color :08x}, pos={self.pos}, beacon_size={self.beacon_size}, slew_size={self.slew_size})"
  end
end

# Export class directly - no redundant factory function needed
return {'beacon_animation': BeaconAnimation}
```

## Testing Your Animation

### Unit Tests

Create comprehensive tests for your animation:

```berry
import animation

def test_my_animation()
  # Create LED strip and engine for testing
  var strip = global.Leds(10)  # Use built-in LED strip for testing
  var engine = animation.create_engine(strip)
  
  # Test basic construction
  var anim = animation.my_animation(engine)
  assert(anim != nil, "Animation should be created")
  
  # Test parameter setting
  anim.color = 0xFFFF0000
  assert(anim.color == 0xFFFF0000, "Color should be set")
  
  # Test parameter updates
  anim.color = 0xFF00FF00
  assert(anim.color == 0xFF00FF00, "Color should be updated")
  
  # Test value providers
  var dynamic_color = animation.smooth(engine)
  dynamic_color.min_value = 0xFF000000
  dynamic_color.max_value = 0xFFFFFFFF
  dynamic_color.duration = 2000
  
  anim.color = dynamic_color
  var raw_color = anim.get_param("color")
  assert(animation.is_value_provider(raw_color), "Should accept value provider")
  
  # Test rendering
  var frame = animation.frame_buffer(10)
  anim.start()
  var result = anim.render(frame, 1000, engine.strip_length)
  assert(result == true, "Should render successfully")
  
  print("✓ All tests passed")
end

test_my_animation()
```

### Integration Testing

Test with the animation engine:

```berry
var strip = global.Leds(30)  # Use built-in LED strip
var engine = animation.create_engine(strip)
var anim = animation.my_animation(engine)

# Set parameters
anim.color = 0xFFFF0000
anim.pos = 5
anim.beacon_size = 3

engine.add(anim)  # Unified method for animations and sequence managers
engine.run()

# Let it run for a few seconds
tasmota.delay(3000)

engine.stop()
print("Integration test completed")
```

## Best Practices

### Performance
- **Minimize calculations** in render() method
- **Cache resolved values** when possible
- **Use integer math** instead of floating point
- **Avoid memory allocation** in render loops

### Memory Management
- **Reuse objects** when possible
- **Clear references** to large objects when done
- **Use static variables** for constants

### Code Organization
- **Group related parameters** together
- **Use descriptive variable names**
- **Comment complex algorithms**
- **Follow Berry naming conventions**

### Error Handling
- **Validate parameters** in constructor
- **Handle edge cases** gracefully
- **Return false** from render() on errors
- **Use meaningful error messages**

## Publishing Your Animation Class

Once you've created a new animation class:

1. **Add it to the animation module** by importing it in `animation.be`
2. **Create a factory function** following the engine-first pattern
3. **Add DSL support** by ensuring the transpiler recognizes your factory function
4. **Document parameters** in the class hierarchy documentation
5. **Test with DSL** to ensure users can access your animation declaratively

**Remember**: Users should primarily interact with animations through the DSL. The programmatic API is mainly for framework development and advanced integrations.

This guide provides everything needed to create professional-quality animation classes that integrate seamlessly with the Berry Animation Framework's parameter system and rendering pipeline.