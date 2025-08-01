# Troubleshooting Guide

Common issues and solutions for the Tasmota Berry Animation Framework.

## Installation Issues

### Framework Not Found

**Problem:** `import animation` fails with "module not found"

**Solutions:**
1. **Check Module Path:**
   ```berry
   # Verify the animation module exists
   import sys
   print(sys.path())
   ```

2. **Set Module Path:**
   ```bash
   berry -m lib/libesp32/berry_animation
   ```

3. **Verify File Structure:**
   ```
   lib/libesp32/berry_animation/
   ├── animation.be          # Main module file
   ├── core/                 # Core classes
   ├── effects/              # Animation effects
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

**Problem:** Animations created but LEDs don't change

**Diagnostic Steps:**
```berry
import animation

var strip = Leds(30)
var engine = animation.create_engine(strip)
var anim = animation.solid(0xFFFF0000)

# Check each step
print("Engine created:", engine != nil)
print("Animation created:", anim != nil)

engine.add_animation(anim)
print("Animation added, count:", engine.size())

engine.start()
print("Engine started:", engine.is_active())
```

**Common Solutions:**

1. **Forgot to Start Engine:**
   ```berry
   engine.add_animation(anim)
   engine.start()  # Don't forget this!
   ```

2. **Animation Not Added:**
   ```berry
   # Make sure animation is added to engine
   engine.add_animation(anim)
   print("Animation count:", engine.size())  # Should be > 0
   ```

3. **Strip Not Configured:**
   ```berry
   # Check strip configuration
   var strip = Leds(30)  # 30 LEDs
   print("Strip created:", strip != nil)
   ```

### Colors Look Wrong

**Problem:** Colors appear different than expected

**Common Issues:**

1. **Missing Alpha Channel:**
   ```berry
   # Wrong - missing alpha
   var red = 0xFF0000
   
   # Correct - with alpha channel
   var red = 0xFFFF0000  # ARGB format
   ```

2. **Color Format Confusion:**
   ```berry
   # ARGB format: 0xAARRGGBB
   var red = 0xFFFF0000      # Alpha=FF, Red=FF, Green=00, Blue=00
   var green = 0xFF00FF00    # Alpha=FF, Red=00, Green=FF, Blue=00
   var blue = 0xFF0000FF     # Alpha=FF, Red=00, Green=00, Blue=FF
   ```

3. **Brightness Issues:**
   ```berry
   # Check opacity settings
   anim.set_opacity(255)  # Full brightness
   
   # Check pulse brightness ranges
   var pulse = animation.pulse(pattern, 2000, 50, 255)  # Min=50, Max=255
   ```

### Animations Too Fast/Slow

**Problem:** Animation timing doesn't match expectations

**Solutions:**

1. **Check Time Units:**
   ```berry
   # Berry uses milliseconds
   var pulse = animation.pulse(pattern, 2000, 50, 255)  # 2 seconds
   
   # DSL uses time units
   # animation pulse_anim = pulse(pattern, 2s, 20%, 100%)  # 2 seconds
   ```

2. **Adjust Periods:**
   ```berry
   # Too fast - increase period
   var slow_pulse = animation.pulse(pattern, 5000, 50, 255)  # 5 seconds
   
   # Too slow - decrease period
   var fast_pulse = animation.pulse(pattern, 500, 50, 255)   # 0.5 seconds
   ```

3. **Performance Limitations:**
   ```berry
   # Reduce number of simultaneous animations
   engine.clear()  # Remove all animations
   engine.add_animation(single_animation)
   ```

## DSL Issues

### DSL Compilation Errors

**Problem:** DSL code fails to compile

**Diagnostic Approach:**
```berry
try
  var berry_code = animation.compile_dsl(dsl_source)
  print("Compilation successful")
except "dsl_compilation_error" as e, msg
  print("DSL Error:", msg)
end
```

**Common DSL Errors:**

1. **Undefined Colors:**
   ```dsl
   # Wrong - color not defined
   animation red_anim = solid(red)
   
   # Correct - define color first
   color red = #FF0000
   animation red_anim = solid(red)
   ```

2. **Invalid Color Format:**
   ```dsl
   # Wrong - invalid hex format
   color red = FF0000
   
   # Correct - with # prefix
   color red = #FF0000
   ```

3. **Missing Time Units:**
   ```dsl
   # Wrong - no time unit
   animation pulse_anim = pulse(solid(red), 2000, 50%, 100%)
   
   # Correct - with time unit
   animation pulse_anim = pulse(solid(red), 2s, 50%, 100%)
   ```

4. **Reserved Name Conflicts:**
   ```dsl
   # Wrong - 'red' is a predefined color
   color red = #800000
   
   # Correct - use different name
   color dark_red = #800000
   ```

### DSL Runtime Errors

**Problem:** DSL compiles but fails at runtime

**Common Issues:**

1. **Strip Not Initialized:**
   ```dsl
   # Add strip declaration if needed
   strip length 30
   
   color red = #FF0000
   animation red_anim = solid(red)
   run red_anim
   ```

2. **Sequence Issues:**
   ```dsl
   # Make sure animations are defined before sequences
   color red = #FF0000
   animation red_anim = solid(red)  # Define first
   
   sequence demo {
     play red_anim for 3s  # Use after definition
   }
   ```

## Performance Issues

### Choppy Animations

**Problem:** Animations appear jerky or stuttering

**Solutions:**

1. **Reduce Animation Count:**
   ```berry
   # Good - 1-3 animations
   engine.clear()
   engine.add_animation(main_animation)
   
   # Avoid - too many simultaneous animations
   # engine.add_animation(anim1)
   # engine.add_animation(anim2)
   # ... (10+ animations)
   ```

2. **Increase Animation Periods:**
   ```berry
   # Smooth - longer periods
   var smooth_pulse = animation.pulse(pattern, 3000, 50, 255)  # 3 seconds
   
   # Choppy - very short periods
   var choppy_pulse = animation.pulse(pattern, 50, 50, 255)    # 50ms
   ```

3. **Optimize Value Providers:**
   ```berry
   # Efficient - reuse providers
   var breathing = animation.smooth(50, 255, 2000)
   var anim1 = animation.pulse(pattern1, breathing)
   var anim2 = animation.pulse(pattern2, breathing)  # Reuse
   
   # Inefficient - create new providers
   var anim1 = animation.pulse(pattern1, animation.smooth(50, 255, 2000))
   var anim2 = animation.pulse(pattern2, animation.smooth(50, 255, 2000))
   ```

### Memory Issues

**Problem:** Out of memory errors or system crashes

**Solutions:**

1. **Clear Unused Animations:**
   ```berry
   # Clear before adding new animations
   engine.clear()
   engine.add_animation(new_animation)
   ```

2. **Limit Palette Size:**
   ```dsl
   # Good - reasonable palette size
   palette simple_fire = [
     (0, #000000),
     (128, #FF0000),
     (255, #FFFF00)
   ]
   
   # Avoid - very large palettes
   # palette huge_palette = [
   #   (0, color1), (1, color2), ... (255, color256)
   # ]
   ```

3. **Use Sequences Instead of Simultaneous Animations:**
   ```dsl
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

# If this doesn't work, check hardware
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

### Enable Debug Mode

```berry
# Enable debug output
var runtime = animation.DSLRuntime(engine, true)  # Debug mode on

# Check generated code
try
  var berry_code = animation.compile_dsl(dsl_source)
  print("Generated Berry code:")
  print(berry_code)
except "dsl_compilation_error" as e, msg
  print("Compilation error:", msg)
end
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
var anim = animation.solid(0xFFFF0000)
print("Animation created:", anim != nil)

print("4. Adding animation...")
engine.add_animation(anim)
print("Animation count:", engine.size())

print("5. Starting engine...")
engine.start()
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
```dsl
color red = #FF0000
animation red_anim = solid(red)
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

Following these guidelines will help you avoid most common issues and create reliable LED animations.