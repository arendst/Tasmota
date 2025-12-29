# Troubleshooting Guide

Common issues and solutions for the Tasmota Berry Animation Framework.

**Note**: This guide focuses on DSL usage, which is the recommended way to create animations. For programmatic API issues, see the [Animation Development Guide](Animation_Development.md).

## Installation Issues

### Framework Not Found

**Problem:** `import animation` or `import animation_dsl` fails with "module not found"

**Solutions:**
1. **Check Module Import:**
   ```berry
   import animation      # Core framework
   import animation_dsl  # DSL compiler
   ```

2. **Set Module Path:**
   ```bash
   berry -m lib/libesp32/berry_animation
   ```

3. **Verify File Structure:**
   ```
   lib/libesp32/berry_animation/
   ├── animation.be          # Main module file
   ├── dsl/                  # DSL components
   ├── core/                 # Core classes
   ├── animations/           # Animation effects
   └── ...
   ```

### Missing Dependencies

**Problem:** Errors about missing `tasmota` or `Leds` classes

**Solutions:**
1. **For Tasmota Environment:**
   - Ensure you're running on actual Tasmota firmware
   - Check that Berry support is enabled

2. **For Development Environment:**
   ```berry
   # Mock Tasmota for testing
   if !global.contains("tasmota")
     global.tasmota = {
       "millis": def() return 1000 end,
       "scale_uint": def(val, from_min, from_max, to_min, to_max)
         return int((val - from_min) * (to_max - to_min) / (from_max - from_min) + to_min)
       end
     }
   end
   ```

## Animation Issues

### Animations Not Starting

**Problem:** DSL animations compile but LEDs don't change

**Diagnostic Steps:**
```berry
import animation
import animation_dsl

# Test basic DSL execution
var dsl_code = "color red = 0xFF0000\n" +
               "animation red_anim = solid(color=red)\n" +
               "run red_anim"

try
  animation_dsl.execute(dsl_code)
  print("DSL executed successfully")
except .. as e, msg
  print("DSL Error:", msg)
end
```

**Timing Behavior Note:**
The framework has updated timing behavior where:
- The `start()` method only resets the time origin if the animation/value provider was already started previously
- The first actual rendering tick occurs in `update()`, `render()`, or `produce_value()` methods
- This ensures proper timing initialization and prevents premature time reference setting

**Common Solutions:**

1. **Missing Strip Declaration:**
   ```berry
   # Add explicit strip length if needed
   strip length 30
   
   color red = 0xFF0000
   animation red_anim = solid(color=red)
   run red_anim
   ```

2. **Animation Not Executed:**
   ```berry
   # Make sure you have a 'run' statement
   color red = 0xFF0000
   animation red_anim = solid(color=red)
   run red_anim  # Don't forget this!
   ```

3. **Strip Auto-Detection Issues:**
   ```berry
   # Force strip length if auto-detection fails
   strip length 30  # Must be first statement
   
   color red = 0xFF0000
   animation red_anim = solid(color=red)
   run red_anim
   ```

### Colors Look Wrong

**Problem:** Colors appear different than expected

**Common Issues:**

1. **Missing Alpha Channel:**
   ```berry
   # Note: 0xFF0000 is valid RGB format (alpha defaults to 0xFF)
   color red = 0xFF0000      # RGB format (alpha=255 assumed)
   
   # Explicit alpha channel (ARGB format)
   color red = 0xFFFF0000    # ARGB format (alpha=255, red=255)
   color semi_red = 0x80FF0000  # ARGB format (alpha=128, red=255)
   ```

2. **Color Format Confusion:**
   ```berry
   # ARGB format: 0xAARRGGBB
   color red = 0xFFFF0000      # Alpha=FF, Red=FF, Green=00, Blue=00
   color green = 0xFF00FF00    # Alpha=FF, Red=00, Green=FF, Blue=00
   color blue = 0xFF0000FF     # Alpha=FF, Red=00, Green=00, Blue=FF
   ```

3. **Brightness Issues:**
   ```berry
   # Use opacity parameter or property assignment
   animation red_anim = solid(color=red, opacity=255)  # Full brightness
   
   # Or assign after creation
   animation pulse_red = pulsating_animation(color=red, period=2s)
   pulse_red.opacity = 200  # Adjust brightness
   
   # Use value providers for dynamic brightness
   set brightness = smooth(min_value=50, max_value=255, period=3s)
   animation breathing = solid(color=red)
   breathing.opacity = brightness
   ```

### Animations Too Fast/Slow

**Problem:** Animation timing doesn't match expectations

**Solutions:**

1. **Check Time Units:**
   ```berry
   # DSL uses time units (converted to milliseconds)
   animation pulse_anim = pulsating_animation(color=red, period=2s)    # 2 seconds
   animation fast_pulse = pulsating_animation(color=blue, period=500ms) # 0.5 seconds
   ```

2. **Adjust Periods:**
   ```berry
   # Too fast - increase period
   animation slow_pulse = pulsating_animation(color=red, period=5s)   # 5 seconds
   
   # Too slow - decrease period
   animation fast_pulse = pulsating_animation(color=red, period=500ms) # 0.5 seconds
   ```

3. **Performance Limitations:**
   ```berry
   # Use sequences instead of multiple simultaneous animations
   sequence optimized_show {
     play animation1 for 3s
     play animation2 for 3s
     play animation3 for 3s
   }
   run optimized_show
   
   # Instead of:
   # run animation1
   # run animation2  
   # run animation3
   ```

## DSL Issues

### DSL Compilation Errors

**Problem:** DSL code fails to compile

**Diagnostic Approach:**
```berry
try
  var berry_code = animation_dsl.compile(dsl_source)
  print("Compilation successful")
except "dsl_compilation_error" as e, msg
  print("DSL Error:", msg)
end
```

**Common DSL Errors:**

1. **Undefined Colors:**
   ```berry
   # Wrong - color not defined
   animation red_anim = solid(color=red)
   
   # Correct - define color first
   color red = 0xFF0000
   animation red_anim = solid(color=red)
   ```

2. **Invalid Color Format:**
   ```berry
   # Wrong - # prefix not supported (conflicts with comments)
   color red = #FF0000
   
   # Correct - use 0x prefix
   color red = 0xFF0000
   ```

3. **Missing Time Units:**
   ```berry
   # Wrong - no time unit
   animation pulse_anim = pulsating_animation(color=red, period=2000)
   
   # Correct - with time unit
   animation pulse_anim = pulsating_animation(color=red, period=2s)
   ```

4. **Reserved Name Conflicts:**
   ```berry
   # Wrong - 'red' is a predefined color
   color red = 0x800000
   
   # Correct - use different name
   color dark_red = 0x800000
   ```

5. **Invalid Parameter Names:**
   ```berry
   # Wrong - invalid parameter name
   animation pulse_anim = pulsating_animation(color=red, invalid_param=123)
   # Error: "Parameter 'invalid_param' is not valid for pulsating_animation"
   
   # Correct - use valid parameters (see Dsl_Reference.md for complete list)
   animation pulse_anim = pulsating_animation(color=red, period=2s)
   ```

6. **Variable Duration Support:**
   ```berry
   # Now supported - variables in play/wait durations
   set eye_duration = 5s
   
   sequence cylon_eye {
     play red_eye for eye_duration    # ✓ Variables now work
     wait eye_duration                # ✓ Variables work in wait too
   }
   
   # Also supported - value providers for dynamic duration
   set dynamic_time = triangle(min_value=1000, max_value=3000, period=10s)
   
   sequence demo {
     play animation for dynamic_time  # ✓ Dynamic duration
   }
   ```

7. **Template Definition Errors:**
   ```berry
   # Wrong - missing braces
   template pulse_effect
     param color type color
     param speed
   # Error: Expected '{' after template name
   
   # Wrong - invalid parameter syntax
   template pulse_effect {
     param color as color  # Error: Use 'type' instead of 'as'
     param speed
   }
   
   # Wrong - missing template body
   template pulse_effect {
     param color type color
   }
   # Error: Template body cannot be empty
   
   # Correct - proper template syntax
   template pulse_effect {
     param color type color
     param speed
     
     animation pulse = pulsating_animation(
       color=color
       period=speed
     )
     
     run pulse
   }
   ```

8. **Template Call Errors:**
   ```berry
   # Wrong - template not defined
   pulse_effect(red, 2s)
   # Error: "Undefined reference: 'pulse_effect'"
   
   # Wrong - incorrect parameter count
   template pulse_effect {
     param color type color
     param speed
     # ... template body ...
   }
   
   pulse_effect(red)  # Error: Expected 2 parameters, got 1
   
   # Correct - define template first, call with correct parameters
   template pulse_effect {
     param color type color
     param speed
     
     animation pulse = pulsating_animation(color=color, period=speed)
     run pulse
   }
   
   pulse_effect(red, 2s)  # ✓ Correct usage
   ```

6. **Parameter Constraint Violations:**
   ```berry
   # Wrong - negative period not allowed
   animation bad_pulse = pulsating_animation(color=red, period=-2s)
   # Error: "Parameter 'period' value -2000 violates constraint: min=1"
   
   # Wrong - invalid enum value
   animation bad_comet = comet_animation(color=red, direction=5)
   # Error: "Parameter 'direction' value 5 not in allowed values: [-1, 1]"
   
   # Correct - valid parameters within constraints
   animation good_pulse = pulsating_animation(color=red, period=2s)
   animation good_comet = comet_animation(color=red, direction=1)
   ```

7. **Repeat Syntax Errors:**
   ```berry
   # Wrong - old colon syntax no longer supported
   sequence bad_demo {
     repeat 3 times:  # Error: Expected '{' after 'times'
       play anim for 1s
   }
   
   # Wrong - missing braces
   sequence bad_demo2 {
     repeat 3 times
       play anim for 1s  # Error: Expected '{' after 'times'
   }
   
   # Correct - use braces for repeat blocks
   sequence good_demo {
     repeat 3 times {
       play anim for 1s
     }
   }
   
   # Also correct - alternative syntax
   sequence good_demo_alt repeat 3 times {
     play anim for 1s
   }
   
   # Correct - forever syntax
   sequence infinite_demo {
     repeat forever {
       play anim for 1s
       wait 500ms
     }
   }
   ```

### Template Issues

### Template Definition Problems

**Problem:** Template definitions fail to compile

**Common Template Errors:**

1. **Missing Template Body:**
   ```berry
   # Wrong - empty template
   template empty_template {
     param color type color
   }
   # Error: "Template body cannot be empty"
   
   # Correct - template must have content
   template pulse_effect {
     param color type color
     param speed
     
     animation pulse = pulsating_animation(color=color, period=speed)
     run pulse
   }
   ```

2. **Invalid Parameter Syntax:**
   ```berry
   # Wrong - old 'as' syntax
   template pulse_effect {
     param color as color
   }
   # Error: Expected 'type' keyword, got 'as'
   
   # Correct - use 'type' keyword
   template pulse_effect {
     param color type color
     param speed  # Type annotation is optional
   }
   ```

3. **Template Name Conflicts:**
   ```berry
   # Wrong - template name conflicts with built-in function
   template solid {  # 'solid' is a built-in animation function
     param color type color
     # ...
   }
   # Error: "Template name 'solid' conflicts with built-in function"
   
   # Correct - use unique template names
   template solid_effect {
     param color type color
     # ...
   }
   ```

### Template Usage Problems

**Problem:** Template calls fail or behave unexpectedly

**Common Issues:**

1. **Undefined Template:**
   ```berry
   # Wrong - calling undefined template
   my_effect(red, 2s)
   # Error: "Undefined reference: 'my_effect'"
   
   # Correct - define template first
   template my_effect {
     param color type color
     param speed
     # ... template body ...
   }
   
   my_effect(red, 2s)  # Now works
   ```

2. **Parameter Count Mismatch:**
   ```berry
   template pulse_effect {
     param color type color
     param speed
     param brightness
   }
   
   # Wrong - missing parameters
   pulse_effect(red, 2s)  # Error: Expected 3 parameters, got 2
   
   # Correct - provide all parameters
   pulse_effect(red, 2s, 200)
   ```

3. **Parameter Type Issues:**
   ```berry
   template pulse_effect {
     param color type color
     param speed
   }
   
   # Wrong - invalid color parameter
   pulse_effect("not_a_color", 2s)
   # Runtime error: Invalid color value
   
   # Correct - use valid color
   pulse_effect(red, 2s)      # Named color
   pulse_effect(0xFF0000, 2s) # Hex color
   ```

### Template vs User Function Confusion

**Problem:** Mixing template and user function concepts

**Key Differences:**

```berry
# Template (DSL-native) - Recommended for most cases
template pulse_effect {
  param color type color
  param speed
  
  animation pulse = pulsating_animation(color=color, period=speed)
  run pulse
}

# User Function (Berry-native) - For complex logic
def create_pulse_effect(engine, color, speed)
  var pulse = animation.pulsating_animation(engine)
  pulse.color = color
  pulse.period = speed
  return pulse
end
animation.register_user_function("pulse_effect", create_pulse_effect)
```

**When to Use Each:**
- **Templates**: Simple to moderate effects, DSL syntax, type safety
- **User Functions**: Complex logic, Berry features, return values

## DSL Runtime Errors

**Problem:** DSL compiles but fails at runtime

**Common Issues:**

1. **Strip Not Initialized:**
   ```berry
   # Add strip declaration if needed
   strip length 30
   
   color red = 0xFF0000
   animation red_anim = solid(color=red)
   run red_anim
   ```

2. **Repeat Performance Issues:**
   ```berry
   # Efficient - runtime repeats don't expand at compile time
   sequence efficient {
     repeat 1000 times {  # No memory overhead for large counts
       play anim for 100ms
       wait 50ms
     }
   }
   
   # Nested repeats work efficiently
   sequence nested {
     repeat 100 times {
       repeat 50 times {  # Total: 5000 iterations, but efficient
         play quick_flash for 10ms
       }
       wait 100ms
     }
   }
   ```

3. **Sequence Issues:**
   ```berry
   # Make sure animations are defined before sequences
   color red = 0xFF0000
   animation red_anim = solid(color=red)  # Define first
   
   sequence demo {
     play red_anim for 3s  # Use after definition
     wait 1s               # Optional pause between animations
   }
   run demo
   ```

4. **Undefined References:**
   ```berry
   # Wrong - using undefined animation in sequence
   sequence bad_demo {
     play undefined_animation for 3s
   }
   # Error: "Undefined reference: 'undefined_animation'"
   
   # Correct - define all references first
   color blue = 0x0000FF
   animation blue_anim = solid(color=blue)
   
   sequence good_demo {
     play blue_anim for 3s
   }
   run good_demo
   ```

## Performance Issues

### CPU Metrics and Profiling

**Feature:** Built-in CPU metrics tracking to monitor animation performance

The AnimationEngine automatically tracks CPU usage and provides detailed statistics every 5 seconds. This helps identify performance bottlenecks and optimize animations for ESP32 embedded systems.

**Automatic Metrics:**

When the engine is running, it automatically logs performance statistics:

```
AnimEngine: ticks=1000/1000 missed=0 total=0.50ms(0-2) anim=0.30ms(0-1) hw=0.20ms(0-1) cpu=10.0%
  Phase1(checks): mean=0.05ms(0-0)
  Phase2(events): mean=0.05ms(0-0)
  Phase3(anim): mean=0.20ms(0-1)
```

**Metrics Explained:**
- **ticks**: Actual ticks executed vs expected (at 5ms intervals)
- **missed**: Hint of missed ticks (negative means extra ticks, positive means missed)
- **total**: Mean total tick time with (min-max) range in milliseconds
- **anim**: Mean animation calculation time with (min-max) range - everything before hardware output
- **hw**: Mean hardware output time with (min-max) range - just the LED strip update
- **cpu**: Overall CPU usage percentage over the 5-second period

**Phase Metrics (Optional):**
When intermediate measurement points are available, the engine also reports phase-based timing:
- **Phase1(checks)**: Initial checks (strip length, throttling, can_show)
- **Phase2(events)**: Event processing time
- **Phase3(anim)**: Animation update and render time (before hardware output)

**Timestamp-Based Profiling:**

The engine uses a timestamp-based profiling system that stores only timestamps (not durations) in instance variables:

- `ts_start` - Tick start timestamp
- `ts_1` - After initial checks (optional)
- `ts_2` - After event processing (optional)
- `ts_3` - After animation update/render (optional)
- `ts_hw` - After hardware output
- `ts_end` - Tick end timestamp

Durations are computed from these timestamps in `_record_tick_metrics()` with nil checks to ensure values are valid.

**Accessing Profiling Data:**

```berry
import animation

var strip = Leds(30)
var engine = animation.create_engine(strip)

# Add an animation
var anim = animation.solid(engine)
anim.color = 0xFFFF0000
engine.add(anim)
engine.run()

# Run for a while to collect metrics
# After 5 seconds, metrics are automatically logged

# Access current metrics programmatically
print("Tick count:", engine.tick_count)
print("Total time sum:", engine.tick_time_sum)
print("Animation time sum:", engine.anim_time_sum)
print("Hardware time sum:", engine.hw_time_sum)

# Access phase metrics if available
if engine.phase1_time_sum > 0
  print("Phase 1 time sum:", engine.phase1_time_sum)
end
```

**Profiling Benefits:**

1. **Memory Efficient:**
   - Only stores timestamps (6 instance variables)
   - No duration storage or arrays
   - Streaming statistics with no memory overhead

2. **Automatic Tracking:**
   - No manual instrumentation needed
   - Runs continuously in background
   - Reports every 5 seconds

3. **Detailed Breakdown:**
   - Separates animation calculation from hardware output
   - Optional phase-based timing for deeper analysis
   - Min/max/mean statistics for all metrics

**Interpreting Performance Metrics:**

1. **High Animation Time:**
   - Too many simultaneous animations
   - Complex value provider calculations
   - Inefficient custom effects
   
   **Solution:** Simplify animations or use sequences

2. **High Hardware Time:**
   - Large LED strip (many pixels)
   - Slow SPI/I2C communication
   - Hardware limitations
   
   **Solution:** Reduce update frequency or strip length

3. **Missed Ticks:**
   - CPU overload (total time > 5ms per tick)
   - Other Tasmota tasks interfering
   
   **Solution:** Optimize animations or reduce complexity

4. **High CPU Percentage:**
   - Animations consuming too much CPU
   - May affect other Tasmota functions
   
   **Solution:** Increase animation periods or reduce effects

**Example Performance Optimization:**

```berry
import animation

var strip = Leds(60)
var engine = animation.create_engine(strip)

# Before optimization - complex animation
var complex_anim = animation.rainbow_animation(engine)
complex_anim.period = 100  # Very fast, high CPU

engine.add(complex_anim)
engine.run()

# Check metrics after 5 seconds:
# AnimEngine: ticks=950/1000 missed=50 total=5.2ms(4-8) cpu=104.0%
# ^ Too slow! Missing ticks and over 100% CPU

# After optimization - slower period
complex_anim.period = 2000  # 2 seconds instead of 100ms

# Check metrics after 5 seconds:
# AnimEngine: ticks=1000/1000 missed=0 total=0.8ms(0-2) cpu=16.0%
# ^ Much better! All ticks processed, reasonable CPU usage
```

### Choppy Animations

**Problem:** Animations appear jerky or stuttering

**Solutions:**

1. **Use Sequences Instead of Multiple Animations:**
   ```berry
   # Good - sequential playback
   sequence smooth_show {
     play animation1 for 3s
     play animation2 for 3s
     play animation3 for 3s
   }
   run smooth_show
   
   # Avoid - too many simultaneous animations
   # run animation1
   # run animation2
   # run animation3
   ```

2. **Increase Animation Periods:**
   ```berry
   # Smooth - longer periods
   animation smooth_pulse = pulsating_animation(color=red, period=3s)
   
   # Choppy - very short periods
   animation choppy_pulse = pulsating_animation(color=red, period=50ms)
   ```

3. **Optimize Value Providers:**
   ```berry
   # Efficient - reuse providers
   set breathing = smooth(min_value=50, max_value=255, period=2s)
   
   color red = 0xFF0000
   color blue = 0x0000FF
   
   animation anim1 = pulsating_animation(color=red, period=2s)
   anim1.opacity = breathing
   
   animation anim2 = pulsating_animation(color=blue, period=2s)
   anim2.opacity = breathing  # Reuse same provider
   ```

4. **Monitor CPU Metrics:**
   ```berry
   # Check if CPU is overloaded
   # Look for missed ticks or high CPU percentage in metrics
   # AnimEngine: ticks=950/1000 missed=50 ... cpu=95.0%
   # ^ This indicates performance issues
   
   # Use profiling to find bottlenecks
   engine.profile_start("suspect_code")
   # ... code that might be slow ...
   engine.profile_end("suspect_code")
   ```

### Memory Issues

**Problem:** Out of memory errors or system crashes

**Solutions:**

1. **Clear Unused Animations:**
   ```berry
   # Clear before adding new animations
   engine.clear()
   engine.add(new_animation)
   ```

2. **Limit Palette Size:**
   ```berry
   # Good - reasonable palette size
   palette simple_fire = [
     (0, 0x000000),
     (128, 0xFF0000),
     (255, 0xFFFF00)
   ]
   
   # Avoid - very large palettes
   # palette huge_palette = [
   #   (0, color1), (1, color2), ... (255, color256)
   # ]
   ```

3. **Use Sequences Instead of Simultaneous Animations:**
   ```berry
   # Memory efficient - sequential playback
   sequence show {
     play animation1 for 5s
     play animation2 for 5s
     play animation3 for 5s
   }
   
   # Memory intensive - all at once
   # run animation1
   # run animation2
   # run animation3
   ```

## Event System Issues

### Events Not Triggering

**Problem:** Event handlers don't execute

**Diagnostic Steps:**
```berry
# Check if handler is registered
var handlers = animation.get_event_handlers("button_press")
print("Handler count:", size(handlers))

# Test event triggering
animation.trigger_event("test_event", {"debug": true})
```

**Solutions:**

1. **Verify Handler Registration:**
   ```berry
   def test_handler(event_data)
     print("Event triggered:", event_data)
   end
   
   var handler = animation.register_event_handler("test", test_handler, 0)
   print("Handler registered:", handler != nil)
   ```

2. **Check Event Names:**
   ```berry
   # Event names are case-sensitive
   animation.register_event_handler("button_press", handler)  # Correct
   animation.trigger_event("button_press", {})               # Must match exactly
   ```

3. **Verify Conditions:**
   ```berry
   def condition_func(event_data)
     return event_data.contains("required_field")
   end
   
   animation.register_event_handler("event", handler, 0, condition_func)
   
   # Event data must satisfy condition
   animation.trigger_event("event", {"required_field": "value"})
   ```

## Hardware Issues

### LEDs Not Responding

**Problem:** Framework runs but LEDs don't light up

**Hardware Checks:**

1. **Power Supply:**
   - Ensure adequate power for LED count
   - Check voltage (5V for WS2812)
   - Verify ground connections

2. **Wiring:**
   - Data line connected to correct GPIO
   - Ground connected between controller and LEDs
   - Check for loose connections

3. **LED Strip:**
   - Test with known working code
   - Check for damaged LEDs
   - Verify strip type (WS2812, SK6812, etc.)

**Software Checks:**
```berry
# Test basic LED functionality
var strip = Leds(30)  # 30 LEDs
strip.set_pixel_color(0, 0xFFFF0000)  # Set first pixel red
strip.show()  # Update LEDs

# Test with animation framework
import animation
var engine = animation.create_engine(strip)
var red_anim = animation.solid(engine)
red_anim.color = 0xFFFF0000
engine.add(red_anim)
engine.run()

# If basic strip works but animation doesn't, check framework setup
```

### Wrong Colors on Hardware

**Problem:** Colors look different on actual LEDs vs. expected

**Solutions:**

1. **Color Order:**
   ```berry
   # Some strips use different color orders
   # Try different strip types in Tasmota configuration
   # WS2812: RGB order
   # SK6812: GRBW order
   ```

2. **Gamma Correction:**
   ```berry
   # Enable gamma correction in Tasmota
   # SetOption37 128  # Enable gamma correction
   ```

3. **Power Supply Issues:**
   - Voltage drop causes color shifts
   - Use adequate power supply
   - Add power injection for long strips

## Debugging Techniques

### DSL vs Berry API Debugging

**For DSL Issues (Recommended):**
```berry
# Enable DSL debug output
import animation_dsl

var dsl_code = "color red = 0xFF0000\nanimation test = solid(color=red)\nrun test"

# Check compilation
try
  var berry_code = animation_dsl.compile(dsl_code)
  print("DSL compilation successful")
  print("Generated Berry code:")
  print(berry_code)
except .. as e, msg
  print("DSL compilation error:", msg)
end

# Execute with debug
try
  animation_dsl.execute(dsl_code, true)  # debug=true
except .. as e, msg
  print("DSL execution error:", msg)
end
```

**For Framework Issues (Advanced):**
```berry
# Direct Berry API debugging (for framework developers)
import animation

var strip = Leds(30)
var engine = animation.create_engine(strip, true)  # debug=true

var anim = animation.solid(engine)
anim.color = 0xFFFF0000
engine.add(anim)
engine.run()
```

### Step-by-Step Testing

```berry
# Test each component individually
print("1. Creating strip...")
var strip = Leds(30)
print("Strip created:", strip != nil)

print("2. Creating engine...")
var engine = animation.create_engine(strip)
print("Engine created:", engine != nil)

print("3. Creating animation...")
var anim = animation.solid(engine)
anim.color = 0xFFFF0000
print("Animation created:", anim != nil)

print("4. Adding animation...")
engine.add(anim)
print("Animation count:", engine.size())

print("5. Starting engine...")
engine.run()
print("Engine active:", engine.is_active())
```

### Monitor Performance

```berry
# Check timing
var start_time = tasmota.millis()
# ... run animation code ...
var end_time = tasmota.millis()
print("Execution time:", end_time - start_time, "ms")

# Monitor memory (if available)
import gc
print("Memory before:", gc.allocated())
# ... create animations ...
print("Memory after:", gc.allocated())
```

## Getting Help

### Information to Provide

When asking for help, include:

1. **Hardware Setup:**
   - LED strip type and count
   - GPIO pin used
   - Power supply specifications

2. **Software Environment:**
   - Tasmota version
   - Berry version
   - Framework version

3. **Code:**
   - Complete minimal example that reproduces the issue
   - Error messages (exact text)
   - Expected vs. actual behavior

4. **Debugging Output:**
   - Debug mode output
   - Generated Berry code (for DSL issues)
   - Console output

### Example Bug Report

```
**Problem:** DSL animation compiles but LEDs don't change

**Hardware:**
- 30x WS2812 LEDs on GPIO 1
- ESP32 with 5V/2A power supply

**Code:**
```berry
color red = 0xFF0000
animation red_anim = solid(color=red)
run red_anim
```

**Error Output:**
```
DSL compilation successful
Engine created: true
Animation count: 1
Engine active: true
```

**Expected:** LEDs turn red
**Actual:** LEDs remain off

**Additional Info:**
- Basic `strip.set_pixel_color(0, 0xFFFF0000); strip.show()` works
- Tasmota 13.2.0, Berry enabled
```

This format helps identify issues quickly and provide targeted solutions.

## Prevention Tips

### Code Quality

1. **Use Try-Catch Blocks:**
   ```berry
   try
     runtime.load_dsl(dsl_code)
   except .. as e, msg
     print("Error:", msg)
   end
   ```

2. **Validate Inputs:**
   ```berry
   if type(color) == "int" && color >= 0
     var anim = animation.solid(color)
   else
     print("Invalid color:", color)
   end
   ```

3. **Test Incrementally:**
   - Start with simple solid colors
   - Add one effect at a time
   - Test each change before proceeding

### Performance Best Practices

1. **Limit Complexity:**
   - 1-3 simultaneous animations
   - Reasonable animation periods (>1 second)
   - Moderate palette sizes

2. **Resource Management:**
   - Clear unused animations
   - Reuse value providers
   - Use sequences for complex shows

3. **Hardware Considerations:**
   - Adequate power supply
   - Proper wiring and connections
   - Appropriate LED strip for application

## Quick Reference: Common DSL Patterns

### Basic Animation
```berry
color red = 0xFF0000
animation red_solid = solid(color=red)
run red_solid
```

### Templates
```berry
# Define reusable template
template pulse_effect {
  param base_color type color    # Use descriptive names
  param speed type time          # Add type annotations for clarity
  
  animation pulse = pulsating_animation(color=base_color, period=speed)
  run pulse
}

# Use template multiple times
pulse_effect(red, 2s)
pulse_effect(blue, 1s)
```

**Common Template Parameter Issues:**

```berry
# ❌ AVOID: Parameter name conflicts
template bad_example {
  param color type color      # Error: conflicts with built-in color name
  param animation type number # Error: conflicts with reserved keyword
}

# ✅ CORRECT: Use descriptive, non-conflicting names
template good_example {
  param base_color type color    # Clear, non-conflicting name
  param anim_speed type time     # Descriptive parameter name
}

# ⚠️ WARNING: Unused parameters generate warnings
template unused_param_example {
  param used_color type color
  param unused_value type number  # Warning: never used in template body
  
  animation test = solid(color=used_color)
  run test
}
```

### Animation with Parameters
```berry
color blue = 0x0000FF
animation blue_pulse = pulsating_animation(color=blue, period=2s, opacity=200)
run blue_pulse
```

### Using Value Providers
```berry
set breathing = smooth(min_value=50, max_value=255, period=3s)
color green = 0x00FF00
animation breathing_green = solid(color=green)
breathing_green.opacity = breathing
run breathing_green
```

### Sequences
```berry
color red = 0xFF0000
color blue = 0x0000FF

animation red_anim = solid(color=red)
animation blue_anim = solid(color=blue)

sequence demo {
  play red_anim for 2s
  wait 500ms
  play blue_anim for 2s
}
run demo
```

### Multiple Strip Lengths
```berry
strip length 60  # Must be first statement

color rainbow = rainbow_color_provider(period=5s)
animation rainbow_anim = solid(color=rainbow)
run rainbow_anim
```

Following these guidelines will help you avoid most common issues and create reliable LED animations.