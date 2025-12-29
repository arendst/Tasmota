# Animation DSL Tutorial

!!! tip "Try it online!"
    You can test all examples in this tutorial directly in your browser using the **[Online Emulator](https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html)**. No hardware required! All tutorial examples are available in the **Examples** dropdown menu.

This tutorial will guide you through the Berry Animation DSL, from your first solid color to complex animated sequences with templates. Each chapter builds on the previous one, introducing new concepts progressively.

## Table of Contents

- [Overview](#overview)
- [Chapter 1: Getting Started](#chapter-1-getting-started)
- [Chapter 2: Color Cycling with Palettes](#chapter-2-color-cycling-with-palettes)
- [Chapter 3: Smooth Color Transitions](#chapter-3-smooth-color-transitions)
- [Chapter 4: Spatial Patterns and Gradients](#chapter-4-spatial-patterns-and-gradients)
- [Chapter 5: Beacons and Moving Effects](#chapter-5-beacons-and-moving-effects)
- [Chapter 6: Shutters and Sequences](#chapter-6-shutters-and-sequences)
- [Chapter 7: Crenel Patterns](#chapter-7-crenel-patterns)
- [Chapter 8: Templates for Reusable Animations](#chapter-8-templates-for-reusable-animations)
- [Quick Reference Card](#quick-reference-card)

---

## Overview

The Animation DSL (Domain-Specific Language) lets you create LED strip animations using simple, declarative syntax. Instead of writing complex Berry code with timing loops and state machines, you describe *what* you want and *how long* it should take.

### Why Use the DSL?

Traditional LED programming requires managing frame buffers, timing loops, and complex state machines. The Animation DSL abstracts all of this away:

- **No timing code** - Just specify durations like `period=2s` and the framework handles the rest
- **No state machines** - Animations automatically manage their internal state
- **Composable** - Layer multiple animations, use one as an opacity mask for another
- **Readable** - Code reads almost like English: "animation pulse = breathe_animation(color=red, period=2s)"

### Key Concepts

Before diving into code, let's understand the building blocks:

| Concept | What It Does | Example |
|---------|--------------|---------|
| **Animation** | A visual effect on the LED strip | `solid`, `twinkle_animation`, `beacon_animation` |
| **Color** | Either a static value or a dynamic provider that changes over time | `red`, `0xFF0000`, `color_cycle(...)` |
| **Palette** | A collection of colors for gradients or cycling | `PALETTE_RAINBOW`, custom arrays |
| **Value Provider** | A number that changes over time (oscillates) | `sine_osc`, `triangle`, `sawtooth` |
| **Sequence** | Orchestrates animations with timing control | `play`, `wait`, `repeat` |
| **Template** | A reusable animation pattern with parameters | `template animation cylon_eye { ... }` |

### How It Works

1. You write DSL code in a `.anim` file
2. The transpiler converts it to Berry code
3. Berry code runs on your ESP32 (or in the browser emulator)
4. The animation engine renders frames to your LED strip

---

## Chapter 1: Getting Started

This chapter covers the basics: creating simple animations, using colors, and layering effects. By the end, you'll understand how to combine multiple animations into a scene.

### 1.1 Your First Animation: Solid Color

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_1_10_plain" target="_blank"><img src="../../_media/berry_animation/chap_1_10.png" alt="Solid Red"></a>

Every animation starts with two steps: **define** it, then **run** it. The simplest animation fills the entire strip with a single color.

```berry
# Solid red background - the simplest animation

animation back = solid(color=red)
run back
```

Let's break this down:

- `animation back` - Creates a new animation and names it "back"
- `solid(color=red)` - Uses the `solid` animation type, which fills all LEDs with one color
- `run back` - Starts the animation running

The DSL provides many predefined colors: `red`, `green`, `blue`, `white`, `yellow`, `orange`, `purple`, `cyan`, and more.

### 1.2 Custom Colors

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_1_20_plain_custom_color" target="_blank"><img src="../../_media/berry_animation/chap_1_20.png" alt="Custom Color"></a>

Predefined colors are convenient, but you'll often want specific shades. Define custom colors using hexadecimal values.

```berry
# Solid dark blue - using a custom color

color space_blue = 0x000066    # Note: opaque 0xFF alpha channel is implicitly added
animation back = solid(color=space_blue)
run back
```

**Color format:** Colors include an alpha channel (transparency) in addition to RGB:

- **6-digit format** `0xRRGGBB` - The alpha is automatically set to `FF` (fully opaque)
- **8-digit format** `0xAARRGGBB` - You specify the alpha explicitly

The alpha channel ranges from `00` (fully transparent) to `FF` (fully opaque). Examples:

- `0xFF0000` = pure red (actually stored as `0xFFFF0000`)
- `0x00FF00` = pure green (actually stored as `0xFF00FF00`)
- `0x000066` = dark blue, fully opaque
- `0x80FF0000` = semi-transparent red (50% opacity)

The DSL also provides `transparent` as a predefined color, equivalent to `0x00000000` (fully transparent black). This is useful for backgrounds or opacity masks where you want parts to be invisible.

### 1.3 Using Predefined Animations

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_1_30_twinkle" target="_blank"><img src="../../_media/berry_animation/chap_1_30.png" alt="Twinkle Stars"></a>

Beyond `solid`, the DSL includes many ready-to-use animation types. Each creates a different visual effect. Let's try `twinkle_animation`, which creates a twinkling stars effect.

```berry
# Twinkle stars - using predefined animations

animation stars = twinkle_animation()
run stars
```

With no parameters, animations use sensible defaults. The twinkle effect will use white sparkles at a moderate density. But you can customize everything!

### 1.4 Animation Parameters

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_1_40_twinke_params" target="_blank"><img src="../../_media/berry_animation/chap_1_40.png" alt="Twinkle Parameters"></a>

Most animations accept **parameters** to customize their behavior. Parameters use the `name=value` syntax inside the parentheses.

```berry
# Twinkle stars with parameters - using animation parameters

# Note: when parameters are in separate lines, you don't need a comma ','
animation stars = twinkle_animation(
  color=0xFFFFAA        # Light yellow sparkles
  density=8             # density (moderate sparkles)
  twinkle_speed=100ms   # twinkle speed
  fade_speed=50         # when no unit, time unit is 'ms'  
)
run stars
```

!!! tip "Parameter Syntax"
    When parameters are on separate lines, commas are optional. This makes the code more readable for complex animations.

**Time values** can use units (internally converted to milliseconds):

- `500ms` - milliseconds (stays as 500)
- `2s` - seconds (converted to 2000)
- `1m` - minutes (converted to 60000)

### 1.5 Layering Animations

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_1_50_twinke_night" target="_blank"><img src="../../_media/berry_animation/chap_1_50.png" alt="Twinkle Night"></a>

One of the most powerful features is **layering** - running multiple animations simultaneously. Each animation has a `priority` that determines its rendering order. Think of it like layers in an image editor: lower numbers are "on top" and can obscure higher numbers.

```berry
# Twinkle night - twinkle stars over a dark night

# Dark blue background
color space_blue = 0x000066
animation background = solid(color=space_blue)
run background

# Twinkle stars on top
animation stars = twinkle_animation(
  color=0xFFFFAA        # Light yellow sparkles
  density=8             # density (moderate sparkles)
  twinkle_speed=100ms   # twinkle speed
  fade_speed=50         # when no unit, time unit is 'ms'
  priority=8            # default priority is 10, so being lower puts it on top
)
run stars
```

Here's what happens:

1. The `background` animation runs with default priority 10
2. The `stars` animation runs with priority 8
3. Since 8 < 10, stars render "on top" of the background
4. Where there's no star, the blue background shows through

This layering technique is fundamental - you'll use it throughout this tutorial to create complex scenes.

---

## Chapter 2: Color Cycling with Palettes

So far, our colors have been static. This chapter introduces **dynamic colors** that change over time, creating smooth color transitions without any manual timing code.

The key concept is the **color provider** - instead of a fixed color value, you use a function that produces different colors as time passes.

### 2.1 Built-in Palette Cycling

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_2_10_palette_rotation" target="_blank"><img src="../../_media/berry_animation/chap_2_10.png" alt="Palette Rotation"></a>

The simplest way to create changing colors is with `color_cycle`, which steps through a palette of colors over time.

```berry
# Rainbow colors cycling through the entire strip over time

# Define a color that cycles over time, cycle is 5 seconds
# PALETTE_RAINBOW_W defines 7 rainbow colors + white
color rainbow_color = color_cycle(colors=PALETTE_RAINBOW_W, period=5s)
animation back = solid(color=rainbow_color)
run back
```

Notice the difference from Chapter 1:

- Before: `color=red` (static color)
- Now: `color=rainbow_color` (dynamic color provider)

The `color_cycle` function creates a color provider that cycles through the palette. The `period=5s` means one complete cycle takes 5 seconds.

**Built-in palettes:**

- `PALETTE_RAINBOW` - 7 rainbow colors
- `PALETTE_RAINBOW_W` - 7 rainbow colors + white
- `PALETTE_RAINBOW2` - Rainbow with first color repeated at end for smooth roll-over
- `PALETTE_RAINBOW_W2` - Rainbow + white with first color repeated at end for smooth roll-over
- `PALETTE_FIRE` - Fire colors (red, orange, yellow)

### 2.2 Custom Palettes

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_2_20_palette_rotation" target="_blank"><img src="../../_media/berry_animation/chap_2_20.png" alt="Custom Palette"></a>

Built-in palettes are convenient, but you'll often want your own color schemes. Define custom palettes as arrays of hex colors.

```berry
# Rainbow colors cycling with custom palette

# Define a palette of rainbow colors including white
palette rainbow_with_white = [
  0xFC0000        # Red
  0xFF8000        # Orange
  0xFFFF00        # Yellow
  0x00FF00        # Green
  0x00FFFF        # Cyan
  0x0080FF        # Blue
  0x8000FF        # Violet
  0xCCCCCC        # White
]

# Define a color that cycles over time
color rainbow_color = color_cycle(colors=rainbow_with_white, period=5s)

# Use it in a solid animation
animation back = solid(color=rainbow_color)
run back
```

The `palette` keyword creates a named color collection. Colors are listed in order - the cycle will go Red → Orange → Yellow → ... → White → Red.

---

## Chapter 3: Smooth Color Transitions

Chapter 2 showed `color_cycle`, which steps discretely between colors. This chapter introduces `rich_palette`, which creates **smooth, interpolated transitions** - the color gradually morphs from one to the next.

### 3.1 Rich Palette Animation

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_3_10_color_transition" target="_blank"><img src="../../_media/berry_animation/chap_3_10.png" alt="Rich Palette"></a>

The `rich_palette_animation` is a complete animation that handles both the color transitions and rendering. It's the easiest way to get smooth rainbow effects.

```berry
# Smooth cycling through rainbow colors

animation back = rich_palette_animation()
# Equivalent to:
# animation back = rich_palette_animation(colors=PALETTE_RAINBOW, period=5s,
#                                         transition_type=SINE, brightness=100%)
run back
```

With no parameters, it uses sensible defaults. The `transition_type=SINE` creates smooth, natural-looking transitions that ease in and out.

### 3.2 Rich Palette with Custom Colors

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_3_20_color_transition_palette" target="_blank"><img src="../../_media/berry_animation/chap_3_20.png" alt="Rich Palette Custom"></a>

For more control, use `rich_palette` as a **color provider** (not an animation). This lets you use smooth color transitions with any animation type.

```berry
# Smooth cycling through rainbow colors with custom palette

palette rainbow_with_white = [
  0xFC0000        # Red
  0xFF8000        # Orange
  0xFFFF00        # Yellow
  0x00FF00        # Green
  0x00FFFF        # Cyan
  0x0080FF        # Blue
  0x8000FF        # Violet
  0xCCCCCC        # White
  0xFC0000        # Red - add first color at end for smooth roll-over
]

# Define a color that cycles over time with smooth transitions
color rainbow_color_rollover = rich_palette(period=10s)

# Use the dynamic color in a solid animation
animation back = solid(color=rainbow_color_rollover)
run back
```

!!! tip "Smooth Roll-over"
    Add the first color at the end of your palette to ensure smooth transitions when the cycle repeats.


---

## Chapter 4: Spatial Patterns and Gradients

Until now, all LEDs displayed the same color at any given moment. This chapter introduces **spatial variation** - different LEDs showing different colors simultaneously, creating gradients and patterns across the strip.

The key insight is that color providers can work in two dimensions:

- **Time**: Colors change as time passes (what we've done so far)
- **Space**: Colors vary by position along the strip (new in this chapter)

### 4.1 Rainbow Gradient

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_4_10_color_pattern" target="_blank"><img src="../../_media/berry_animation/chap_4_10.png" alt="Color Pattern"></a>

A gradient maps colors to positions along the strip. The `palette_gradient_animation` does exactly this.

```berry
# Rainbow pattern across the strip

# Define a palette with period=0 (no time-based change, only spatial)
color rainbow_rich_color = rich_palette(colors=PALETTE_RAINBOW_W, period=0)

# Create a gradient across the whole strip
animation back_pattern = palette_gradient_animation(color_source = rainbow_rich_color)
run back_pattern
```

The magic is `period=0` - this tells the color provider to ignore time and only vary by position. The gradient animation then maps the palette across the strip's length.

### 4.2 Multiple Gradient Repetitions

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_4_12_color_pattern_spatial_2" target="_blank"><img src="../../_media/berry_animation/chap_4_12.png" alt="Gradient 2x"></a>

By default, the gradient spans the entire strip once. Use `spatial_period` to control how many LEDs one complete gradient cycle covers.

```berry
# Rainbow gradient with 2 repetitions across the strip

color rainbow_rich_color = rich_palette(colors=PALETTE_RAINBOW_W, period=0)

# Get the strip length as a variable
set strip_len = strip_length()

# Create gradient with half the strip length as spatial period
animation back_pattern = palette_gradient_animation(
  color_source = rainbow_rich_color
  spatial_period = strip_len / 2
)
run back_pattern
```

With `spatial_period = strip_len / 2`, the gradient repeats twice across the strip. Use smaller values for more repetitions.

!!! note "Using strip_length()"
    The `strip_length()` function returns the current LED count. Since it's a value provider, you must assign it to a variable with `set` before using it in calculations.

### 4.3 Oscillating Spatial Period

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_4_15_color_pattern_spatial_osc" target="_blank"><img src="../../_media/berry_animation/chap_4_15.png" alt="Oscillating Period"></a>

Here's where things get interesting: you can make **any parameter dynamic** by using a value provider instead of a fixed number. This example makes the gradient "breathe" by oscillating its spatial period.

```berry
# Rainbow gradient with oscillating spatial period

color rainbow_rich_color = rich_palette(colors=PALETTE_RAINBOW_W, period=0)

set strip_len = strip_length()

# Oscillate spatial period between 1/2 and 3/2 of strip length
set period = sine_osc(min_value = (strip_len - 1) / 2, max_value = (3 * strip_len) / 2, duration = 5s)

animation back = palette_gradient_animation(color_source = rainbow_rich_color, spatial_period = period)
run back
```

The `sine_osc` function creates a **value provider** - a number that changes smoothly over time. Here it oscillates the spatial period, making the gradient compress and expand.

**Available oscillators:**

| Oscillator | Description |
|------------|-------------|
| `sine_osc` | Smooth sine wave |
| `cosine_osc` | Cosine wave (same as sine, different phase) |
| `triangle` | Linear up and down |
| `sawtooth` | Linear up, instant reset |
| `smooth` | Smooth cosine-based |
| `square` | Instant on/off |

### 4.4 Rotating Gradient

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_4_18_color_pattern_spatial_rotate" target="_blank"><img src="../../_media/berry_animation/chap_4_18.png" alt="Rotating Gradient"></a>

Make the gradient rotate along the strip:

```berry
# Rainbow gradient rotating along the strip over 5 seconds

color rainbow_rich_color = rich_palette(colors=PALETTE_RAINBOW_W, period=0)

animation back = palette_gradient_animation(
  color_source = rainbow_rich_color
  shift_period = 5s    # Complete rotation in 5 seconds
)
run back
```

The `shift_period` parameter makes the entire pattern shift along the strip.

### 4.5 VU-Meter Style Animation

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_4_30_color_pattern_meter" target="_blank"><img src="../../_media/berry_animation/chap_4_30.png" alt="VU Meter"></a>

Create a meter/bar that fills based on a value:

```berry
# VU-meter style animation with green-yellow-red gradient

# Define a VU-meter palette with position-based colors
palette vue_meter_palette = [
  (  0, 0x00FF00)     # Green at 0%
  (143, 0x00FF00)     # Green until ~56%

  (164, 0xFFFF00)     # Yellow transition
  (207, 0xFFFF00)     # Yellow until ~81%

  (228, 0xFF0000)     # Red transition
  (255, 0xFF0000)     # Red at 100%
]

color rainbow_rich_color = rich_palette(colors=vue_meter_palette, period=0, transition_type=LINEAR)

# Sawtooth value from 0% to 100%
set level = sawtooth(min_value = 0%, max_value=100%, duration = 2s)

# Create the meter animation
animation back = palette_meter_animation(color_source = rainbow_rich_color, level = level)
run back
```

The palette uses position-based entries `(position, color)` where position ranges from 0 to 255.

### 4.6 Custom Value Functions

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_4_35_color_pattern_meter_random" target="_blank"><img src="../../_media/berry_animation/chap_4_35.png" alt="Random Meter"></a>

Sometimes the built-in oscillators aren't enough - you need custom logic like random values, sensor readings, or complex calculations. The DSL lets you embed native Berry code and use it in your animations.

**How it works:**

1. **Embed Berry code** using `berry """..."""` blocks (triple quotes) and multi-line content
2. **Declare the function** with `extern function name` so the DSL knows about it
3. **Call the function** in your animation parameters

**Function signature:** Your function receives an `engine` parameter that provides access to timing and animation state. It should return a value (typically 0-255 for levels/opacity).

```berry
# VU-meter with random level using custom Berry function

# Step 1: Embed native Berry code
# The function receives 'engine' which has properties like time_ms
berry """
def rand_meter(engine)
  # Use time to generate pseudo-random values
  # The & 0xFF ensures result is 0-255
  return (engine.time_ms * 2654435761) & 0xFF
end
"""

# Step 2: Declare the function for DSL use
# This tells the transpiler that 'rand_meter' is a valid function
extern function rand_meter

palette vue_meter_palette = [
  (  0, 0x00FF00)     # Green
  (143, 0x00FF00)
  (164, 0xFFFF00)     # Yellow
  (207, 0xFFFF00)
  (228, 0xFF0000)     # Red
  (255, 0xFF0000)
]

color rainbow_rich_color = rich_palette(colors=vue_meter_palette, period=0, transition_type=LINEAR)

# Step 3: Use the custom function as a parameter
# Call it with () - the engine parameter is passed automatically
animation back = palette_meter_animation(color_source = rainbow_rich_color, level = rand_meter())
run back
```

The `berry """..."""` block lets you embed arbitrary Berry code. The `extern function` declaration makes it available to the DSL.

---

## Chapter 5: Beacons and Moving Effects

Previous chapters filled the entire strip with colors or gradients. This chapter introduces **beacons** - localized highlights at specific positions. By animating the position, you create moving effects like the classic "Cylon eye" scanner.

A beacon has five key properties:

| Property | Description | Default |
|----------|-------------|---------|
| `color` | The highlight color | `white` |
| `back_color` | The background color | `transparent` |
| `pos` | Position on the strip (pixel index) | `0` |
| `beacon_size` | Width of the highlight in pixels | `1` |
| `slew_size` | Fade-out width on each edge (soft edges) | `0` |

### 5.1 Static Beacon

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_5_10_beacon" target="_blank"><img src="../../_media/berry_animation/chap_5_10.png" alt="Static Beacon"></a>

Let's start with a stationary beacon - a red highlight on a blue background.

```berry
# Static beacon

animation back = beacon_animation(
  back_color = blue
  color = red
  pos = 5              # Start at pixel 5
  beacon_size = 7      # 7 pixels wide
)
run back
```

### 5.2 Beacon with Slew (Soft Edges)

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_5_15_beacon_slew" target="_blank"><img src="../../_media/berry_animation/chap_5_15.png" alt="Beacon Slew"></a>

Hard edges can look harsh. The `slew_size` parameter adds a gradual fade on each side of the beacon, creating softer, more natural-looking highlights.

**How slew works:**

- The slew creates a transition zone of `slew_size` pixels on each side of the beacon
- Within the slew zone, the color gradually blends from `color` to `back_color`
- The blending is linear: the first slew pixel is mostly `color`, the last is mostly `back_color`
- If `back_color` is `transparent`, the edges become progressively transparent, allowing underlying animations to show through

**Total width:** A beacon with `beacon_size=7` and `slew_size=3` occupies 7 + 3 + 3 = 13 pixels total (7 solid + 3 fade on each side).

```berry
# Static beacon with slew

animation back = beacon_animation(
  back_color = blue
  color = red
  pos = 5
  beacon_size = 7
  slew_size = 3        # 3 pixel fade on each side
)
run back
```

### 5.3 Animated Slew

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_5_20_beacon_slew_osc" target="_blank"><img src="../../_media/berry_animation/chap_5_20.png" alt="Oscillating Slew"></a>

Remember: any numeric parameter can be replaced with a value provider. Here we make the slew size pulse in and out.

```berry
# Beacon with oscillating slew

set slew = cosine_osc(min_value = 0, max_value = 4, duration = 2s)

animation back = beacon_animation(
  back_color = blue
  color = red
  pos = 5
  beacon_size = 7
  slew_size = slew     # Dynamic slew size
)
run back
```

### 5.4 Cylon Eye (Moving Beacon)

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_5_30_cylon" target="_blank"><img src="../../_media/berry_animation/chap_5_30.png" alt="Cylon Eye"></a>

Now for the classic effect: a beacon that moves back and forth across the strip. We use `cosine_osc` on the `pos` parameter to create smooth scanning motion.

```berry
# Moving red beacon - Cylon style

set strip_len = strip_length()

animation back = beacon_animation(
    color = red
    pos = cosine_osc(min_value = -1, max_value = strip_len - 2, duration = 5s)
    beacon_size = 3       # small 3 pixels eye
    slew_size = 2         # with 2 pixel shading around
)
run back
```

The cosine oscillator creates smooth acceleration and deceleration at the ends, just like the original Battlestar Galactica Cylon!

### 5.5 Rainbow Cylon with Stars

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_5_40_cylon_rainbow" target="_blank"><img src="../../_media/berry_animation/chap_5_40.png" alt="Rainbow Cylon"></a>

Let's combine everything we've learned: layered animations, dynamic colors, and moving beacons. This example creates a scene with two layers.

**Layer structure (rendered bottom to top):**

| Priority | Animation | Description |
|----------|-----------|-------------|
| 10 (default) | `stars` | Twinkling background - rendered first |
| 5 | `back` | Moving rainbow beacon - rendered on top |

**How layering works:**

1. The `stars` animation runs with default priority 10
2. The `back` beacon runs with priority 5 (lower = on top)
3. The beacon uses `back_color = transparent` (default), so where there's no beacon, the stars show through
4. The beacon's slew creates a soft edge that gradually reveals the stars underneath

```berry
# Moving rainbow beacon with twinkling stars background

set strip_len = strip_length()

# Twinkling stars background
animation stars = twinkle_animation(
  color=0xFFFFAA
  density=2
  twinkle_speed=100ms
  fade_speed=100
  # priority = 10 (default)
)
run stars

# Moving beacon with dynamic color
# back_color defaults to transparent, so stars show through
animation back = beacon_animation(
    color = rich_palette(colors=PALETTE_RAINBOW_W2, period=5s)
    pos = cosine_osc(min_value = -1, max_value = strip_len - 2, duration = 5s)
    beacon_size = 3
    slew_size = 2
    priority = 5          # Lower priority = rendered on top of stars
)
run back
```

The result: a rainbow-colored eye scans across a field of twinkling stars, with the stars visible everywhere except where the beacon is.

### 5.6 Beacon as Opacity Mask

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_5_50_cylon_opacity" target="_blank"><img src="../../_media/berry_animation/chap_5_50.png" alt="Opacity Mask"></a>

Instead of layering animations with priority, you can use one animation as an **opacity mask** for another. This creates a "window" effect where the mask controls what's visible.

**How opacity masks work:**

1. The mask animation renders to determine opacity values (0-255 per pixel)
2. Where the mask is bright (white/high values), the main animation is fully visible
3. Where the mask is dark (black/low values), the main animation is transparent
4. The mask's actual color doesn't matter - only its brightness/alpha is used

**In this example:**

- `moving_eye` is a beacon that moves back and forth
- `eye_pattern` is a red-blue-red gradient across the entire strip
- The gradient is only visible where the beacon is - creating a "spotlight" effect
- The beacon's slew creates soft edges on the spotlight

```berry
# Moving beacon used as opacity filter on pattern

set strip_len = strip_length()

# Define a red-blue-red gradient palette
palette red_blue_red_palette = [ red, 0x3333FF, red ]
color red_blue_red_color = rich_palette(colors=red_blue_red_palette)

# Moving beacon as opacity mask
# The color is white but it doesn't matter - only brightness counts
animation moving_eye = beacon_animation(
    color = white           # Color doesn't matter, only brightness/alpha
    pos = cosine_osc(min_value = -1, max_value = strip_len - 2, duration = 5s)
    beacon_size = 3
    slew_size = 2
)

# Apply the mask to a gradient
# The gradient exists everywhere, but only shows where the beacon is
animation eye_pattern = palette_gradient_animation(
  color_source = red_blue_red_color
  opacity = moving_eye      # Use beacon as opacity mask
)
run eye_pattern
```

The result: a moving "window" reveals different parts of the underlying gradient as it scans across the strip.


---

## Chapter 6: Shutters and Sequences

This chapter introduces two powerful concepts:

1. **Shutters** - Expanding/contracting effects created by animating a beacon's size
2. **Sequences** - Orchestrating multiple animations with precise timing control

Sequences are essential when you need things to happen in order: play animation A, then wait, then play animation B, then change a color, etc.

### 6.1 Simple Shutter

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_6_10_shutter_lr" target="_blank"><img src="../../_media/berry_animation/chap_6_10.png" alt="Simple Shutter"></a>

A shutter effect is simply a beacon with an animated size. Using `sawtooth` makes it grow linearly then reset.

```berry
# Shutter left to right using beacon

set strip_len = strip_length()

# Sawtooth from 0 to strip_len - grows linearly, then resets
set shutter_size = sawtooth(min_value = 0, max_value = strip_len, duration = 1.5s)

animation shutter_lr_animation = beacon_animation(
    color = red
    back_color = blue
    pos = 0                         # Start from left
    beacon_size = shutter_size      # Expanding size
)
run shutter_lr_animation
```

### 6.2 Shutter with Rotating Colors

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_6_20_shutter_lr_rotate" target="_blank"><img src="../../_media/berry_animation/chap_6_20.png" alt="Rotating Shutter"></a>

Now let's add color changes after each shutter cycle. This requires a **sequence** - a way to run code at specific times.

A sequence contains steps that execute in order:

- `play animation for duration` - Run an animation for a specific time
- `wait duration` - Pause before the next step
- `restart provider` - Reset a value provider's timing
- `repeat N times { ... }` - Loop a block of steps

```berry
# Shutter with rotating colors using sequence

set strip_len = strip_length()
set period = 1.5s

set shutter_size = sawtooth(min_value = 0, max_value = strip_len, duration = period)

# Two color providers cycling through rainbow
# period=0 means they don't auto-cycle - we control them manually
color col1 = color_cycle(colors=PALETTE_RAINBOW_W, period=0)
color col2 = color_cycle(colors=PALETTE_RAINBOW_W, period=0)
col2.next = 1           # Shift col2 by one color at startup

animation shutter_lr_animation = beacon_animation(
    color = col2
    back_color = col1
    pos = 0
    beacon_size = shutter_size
)

# Sequence running forever
sequence shutter_seq repeat forever {
    restart shutter_size        # Sync timing
    play shutter_lr_animation for period
    col1.next = 1               # Advance col1 to next color in palette
    col2.next = 1               # Advance col2 to next color in palette
}
run shutter_seq
```

**Understanding `color_cycle` with `period=0`:**

When `period=0`, the color provider doesn't automatically cycle through colors over time. Instead, you control it manually by writing to its `next` property:

- `col.next = 1` - Advance to the next color in the palette
- `col.next = 2` - Skip ahead by 2 colors
- `col.next = -1` - Go back to the previous color

This gives you precise control over color changes, synchronized with your sequence timing.

**Sequence statements:**

| Statement | Description |
|-----------|-------------|
| `play animation for duration` | Play an animation for a specific time |
| `wait duration` | Pause the sequence |
| `restart value_provider` | Reset a value provider's timing |
| `repeat N times { ... }` | Repeat a block N times |
| `repeat forever { ... }` | Repeat indefinitely |

### 6.3 Central Shutter

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_6_30_shutter_central" target="_blank"><img src="../../_media/berry_animation/chap_6_30.png" alt="Central Shutter"></a>

Instead of expanding from the left edge, this shutter expands from the center outward.

**From left-right to center-out:**

The key difference is the `pos` calculation:

- **Left-right shutter** (6.1): `pos = 0` - beacon starts at left edge and grows rightward
- **Center-out shutter** (6.3): `pos = strip_len_2 - (shutter_size + 1) / 2` - beacon stays centered as it grows

The formula `strip_len_2 - (shutter_size + 1) / 2` keeps the beacon centered by:

1. Starting at the center (`strip_len_2`)
2. Subtracting half the beacon size so it expands equally in both directions
3. As `shutter_size` grows from 0 to `strip_len`, the position moves left to keep the beacon centered

```berry
# Shutter central with rotating colors

set strip_len = strip_length()
set strip_len_2 = (strip_len + 1) / 2       # Half length (center position)

set period = 1.5s
set shutter_size = sawtooth(min_value = 0, max_value = strip_len, duration = period)

color col1 = color_cycle(colors=PALETTE_RAINBOW_W, period=0)
color col2 = color_cycle(colors=PALETTE_RAINBOW_W, period=0)
col2.next = 1

# Position calculated to center the beacon as it grows
animation shutter_inout_animation = beacon_animation(
    color = col2
    back_color = col1
    pos = strip_len_2 - (shutter_size + 1) / 2
    beacon_size = shutter_size
)

sequence shutter_seq repeat forever {
    restart shutter_size
    play shutter_inout_animation for period
    col1.next = 1
    col2.next = 1
}
run shutter_seq
```

### 6.4 Bidirectional Shutter

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_6_40_shutter_central_bidir" target="_blank"><img src="../../_media/berry_animation/chap_6_40.png" alt="Bidirectional Shutter"></a>

This example combines both directions: first the shutter expands from center (in-out), then contracts back to center (out-in). The sequence cycles through all palette colors in each direction before switching.

**Sequence structure:**

The sequence contains two nested `repeat` blocks:

1. **First repeat**: Runs the in-out animation once for each color in the palette
2. **Second repeat**: Runs the out-in animation once for each color in the palette

The repeat count `col1.palette_size` dynamically gets the number of colors in the palette (8 for `PALETTE_RAINBOW_W`), so the animation automatically adapts if you change palettes.

**Why `restart` is important:**

The `shutter_size` sawtooth oscillator runs continuously based on elapsed time. Without `restart`, each iteration would start wherever the sawtooth happened to be, causing visual glitches. By calling `restart shutter_size` at the beginning of each iteration:

- The sawtooth resets to its starting value (0)
- The shutter animation starts cleanly from the beginning
- Everything stays perfectly synchronized with the sequence timing

```berry
# Shutter central in-out and out-in with rotating colors

set strip_len = strip_length()
set strip_len_2 = (strip_len + 1) / 2

set period = 1.5s
set shutter_size = sawtooth(min_value = 0, max_value = strip_len, duration = period)

color col1 = color_cycle(colors=PALETTE_RAINBOW_W, period=0)
color col2 = color_cycle(colors=PALETTE_RAINBOW_W, period=0)
col2.next = 1

# In-out animation: beacon grows from center outward
animation shutter_inout_animation = beacon_animation(
    color = col2
    back_color = col1
    pos = strip_len_2 - (shutter_size + 1) / 2
    beacon_size = shutter_size
)

# Out-in animation: beacon shrinks from edges toward center
# Uses inverted size (strip_len - shutter_size) and swapped colors
animation shutter_outin_animation = beacon_animation(
    color = col1
    back_color = col2
    pos = strip_len_2 - (strip_len - shutter_size + 1) / 2
    beacon_size = strip_len - shutter_size
)

sequence shutter_seq repeat forever {
    # First: cycle through all colors in-out (8 iterations for PALETTE_RAINBOW_W)
    repeat col1.palette_size times {
        restart shutter_size        # Reset sawtooth to 0 for clean start
        play shutter_inout_animation for period
        col1.next = 1               # Advance both colors
        col2.next = 1
    }
    # Then: cycle through all colors out-in (8 more iterations)
    repeat col1.palette_size times {
        restart shutter_size        # Reset sawtooth again
        play shutter_outin_animation for period
        col1.next = 1
        col2.next = 1
    }
}
run shutter_seq
```

---

## Chapter 7: Crenel Patterns

Crenels create repeating square wave patterns - alternating blocks of two colors, like the battlements on a castle wall. They're useful for:

- Creating striped patterns
- Building opacity masks with regular gaps
- Marquee-style effects when animated

### 7.1 Static Crenel

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_7_10_crenel" target="_blank"><img src="../../_media/berry_animation/chap_7_10.png" alt="Static Crenel"></a>

A basic crenel alternates between two colors with configurable block sizes.

**Crenel parameters:**

| Parameter | Description | Default |
|-----------|-------------|---------|
| `color` | Color of the "high" pulses | `white` |
| `back_color` | Color of the "low" gaps | `transparent` (0x00000000) |
| `pos` | Starting position on the strip | `0` |
| `pulse_size` | Width of each pulse in pixels | `1` |
| `low_size` | Width of each gap in pixels | `3` |
| `nb_pulse` | Number of pulses (`-1` = infinite) | `-1` |

The pattern repeats with a period of `pulse_size + low_size` pixels.

```berry
# Static crenel pattern

animation back = crenel_animation(
    color = red
    back_color = blue
    pulse_size = 2        # 2 pixels of 'color'
    low_size = 2          # 2 pixels of 'back_color'
)
run back
```

This creates a pattern: 🔴🔴🔵🔵🔴🔴🔵🔵... across the entire strip.

### 7.2 Variable Number of Pulses

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_7_20_crenel_nb_pulse" target="_blank"><img src="../../_media/berry_animation/chap_7_20.png" alt="Variable Pulses"></a>

Instead of showing all pulses (`nb_pulse = -1`), you can animate the number of visible pulses using a value provider. The `triangle` oscillator smoothly varies `nb_pulse` from 0 (no pulses) to the maximum that fits on the strip.

To compute the maximum number of pulses, use the formula: `max_pulses = (strip_len + period - 1) / period` where `period = pulse_size + low_size`. This ceiling division ensures we count partial pulses at the end of the strip.

```berry
# Crenel with variable number of pulses

set strip_len = strip_length()
set period = 4                    # pulse_size (2) + low_size (2)
set max_pulses = (strip_len + period - 1) / period

set nb_pulse = triangle(min_value = 0, max_value = max_pulses, duration = 2s)

animation back = crenel_animation(
    color = red
    back_color = blue
    pulse_size = 2
    low_size = 2
    nb_pulse = nb_pulse
)
run back
```

### 7.3 Variable Pulse Size

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_7_30_crenel_pulse_size" target="_blank"><img src="../../_media/berry_animation/chap_7_30.png" alt="Variable Size"></a>

Instead of a fixed `pulse_size`, you can use a value provider to animate the pulse width over time. Here, a `triangle` oscillator smoothly varies the pulse size between 0 and 4 pixels over 2 seconds, creating a breathing effect on the crenel pattern:

```berry
# Crenel with variable pulse size

set pulse_size = triangle(min_value = 0, max_value = 4, duration = 2s)

animation back = crenel_animation(
    color = red
    back_color = blue
    pulse_size = pulse_size
    low_size = 2
)
run back
```

### 7.4 Dynamic Colors

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_7_40_crenel_color" target="_blank"><img src="../../_media/berry_animation/chap_7_40.png" alt="Dynamic Colors"></a>

The `color` parameter also accepts a color provider instead of a static color. This example uses `rich_palette` to cycle through rainbow colors over 5 seconds, making the crenel pulses continuously change color while the blue background remains fixed:

```berry
# Crenel with dynamic color

color rainbow_color = rich_palette(colors=PALETTE_RAINBOW_W2, period=5s)

animation back = crenel_animation(
    color = rainbow_color
    back_color = blue
    pulse_size = 2
    low_size = 2
)
run back
```

### 7.5 Crenel as Opacity Mask

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_7_50_crenel_opacity" target="_blank"><img src="../../_media/berry_animation/chap_7_50.png" alt="Crenel Mask"></a>

Instead of using a crenel directly as a visible animation, you can use it as an **opacity mask** for another animation. This creates a "window" effect where the crenel pattern controls what's visible.

**How opacity masks work:**

1. The mask animation renders to determine opacity values (0-255 per pixel)
2. Where the mask is bright (white/high values), the main animation is fully visible
3. Where the mask is dark or transparent, the main animation is hidden
4. The mask's actual color doesn't matter - only its brightness/alpha is used

**In this example:**

- `back` is a solid blue background at priority 20
- `mask` is a crenel pattern with white pulses and transparent gaps
- `pattern` is a rotating rainbow gradient that uses the crenel as its `opacity` parameter
- The rainbow gradient is only visible where the crenel pulses are white - creating alternating "windows" of rainbow color on a blue background

```berry
# Crenel used as opacity mask

# Blue background
animation back = solid(color = blue, priority = 20)
run back

# Crenel mask (white = visible, transparent = hidden)
animation mask = crenel_animation(
    color = white
    back_color = transparent
    pulse_size = 2
    low_size = 2
)

# Rainbow gradient masked by crenel
color rainbow_rich_color = rich_palette(colors=PALETTE_RAINBOW_W, period=0)
animation pattern = palette_gradient_animation(
    color_source = rainbow_rich_color
    shift_period = 2s           # Rotating gradient
    opacity = mask              # Apply crenel mask
)
run pattern
```


---

## Chapter 8: Templates for Reusable Animations

As your animations grow more complex, you'll want to reuse patterns with different parameters. **Templates** solve this by letting you define animation "blueprints" with configurable parameters.

Think of templates like functions in programming: define once, use many times with different inputs.

### 8.1 Simple Template: Cylon Eye

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_8_10_template_cylon" target="_blank"><img src="../../_media/berry_animation/chap_8_10.png" alt="Template Cylon"></a>

The `template animation` keyword creates a new animation type that can be instantiated just like built-in animations (`solid`, `beacon_animation`, etc.). Once defined, you use it by calling `animation my_anim = template_name(param1=value1, ...)` - exactly like native animations.

**Defining parameters:**

Each `param` declaration creates a configurable input for your template:

```berry
param parameter_name type type_name default default_value
```

- `parameter_name`: The name you'll use inside the template and when instantiating
- `type type_name`: Optional type constraint (see table below)
- `default value`: Optional default value if not provided at instantiation

**Available parameter types:**

| Type | Description | Example |
|------|-------------|---------|
| `color` | Color value (hex or named) | `param fg type color default red` |
| `palette` | Palette definition (bytes) | `param colors type palette` |
| `time` | Time value with unit (converted to ms) | `param period type time default 5s` |
| `int` | Integer value | `param count type int default 10` |
| `bool` | Boolean (true/false) | `param enabled type bool default true` |
| `string` | String value | `param name type string default "anim"` |
| `percentage` | Percentage (0-255) | `param brightness type percentage default 255` |
| `number` | Generic numeric | `param value type number` |
| `any` | Any type (no validation) | `param data type any` |

Let's package the Cylon eye effect from Chapter 5 as a reusable template. Users can customize the color, speed, and priority without rewriting the animation logic:

```berry
# Template for Cylon-style scanning eye

template animation cylon_eye {
  param eye_color type color default red
  param back_color type color default transparent
  param period type time default 5s
  param priority default 5

  set strip_len = strip_length()

  animation eye_animation = beacon_animation(
    color = eye_color
    back_color = back_color
    pos = cosine_osc(min_value = -1, max_value = strip_len - 2, duration = period)
    beacon_size = 3
    slew_size = 2
    priority = priority
  )

  run eye_animation
}

# Use the template with defaults
animation eye = cylon_eye()
run eye
```

### 8.2 Template with Palette Parameter

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_8_20_template_color_cycle" target="_blank"><img src="../../_media/berry_animation/chap_8_20.png" alt="Template Color Cycle"></a>

Templates can accept complex types like palettes. Here we create a reusable color cycling animation where the user provides their own palette and cycle period:

```berry
# Template for color cycling animation

template animation color_cycle2 {
    param colors type palette
    param period default 5s

    color rainbow_color = color_cycle(colors=colors, period=period)
    animation back = solid(color=rainbow_color)
    run back
}

# Define a custom palette
palette rgb = [
  0xFC0000        # Red
  0x00FF00        # Green
  0x0080FF        # Blue
]

# Use the template with custom parameters
animation main = color_cycle2(colors = rgb, period = 2s)
run main
```

### 8.3 Advanced Template with Conditional Flags

<a href="https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html?example=chap_8_30_template_shutter" target="_blank"><img src="../../_media/berry_animation/chap_8_30.png" alt="Template Shutter"></a>

Templates support `bool` parameters that can be used with `if` statements inside sequences. This allows users to enable or disable parts of the animation at instantiation time. Here we create a bidirectional shutter that can optionally run in-out, out-in, or both directions.

**Dynamic parameter changes:**

You can also modify template parameters at runtime using Berry code. Note that DSL variable names get an underscore suffix in Berry to avoid collisions with reserved words (e.g., `main` becomes `main_`). For example: `main_.inout = false` disables the in-out animation while it's running.

```berry
# Template with conditional flags for bidirectional shutter

template animation shutter_bidir {
  param colors type palette
  param period default 2s
  param inout type bool default true    # Enable in-out animation
  param outin type bool default true    # Enable out-in animation

  set strip_len = strip_length()
  set shutter_size = sawtooth(min_value = 0, max_value = strip_len, duration = period)

  # Two rotating color providers
  color col1 = color_cycle(colors=colors, period=0)
  color col2 = color_cycle(colors=colors, period=0)
  col2.next = 1

  # In-out shutter
  animation shutter_inout_animation = beacon_animation(
    color = col2
    back_color = col1
    pos = 0
    beacon_size = shutter_size
    slew_size = 0
    priority = 5
  )

  # Out-in shutter
  animation shutter_outin_animation = beacon_animation(
    color = col1
    back_color = col2
    pos = 0
    beacon_size = strip_len - shutter_size
    slew_size = 0
    priority = 5
  )

  # Sequence with conditional blocks
  sequence shutter_seq repeat forever {
    if inout {                              # Only if inout is true
      repeat col1.palette_size times {
        restart shutter_size
        play shutter_inout_animation for period
        col1.next = 1
        col2.next = 1
      }
    }
    if outin {                              # Only if outin is true
      repeat col1.palette_size times {
        restart shutter_size
        play shutter_outin_animation for period
        col1.next = 1
        col2.next = 1
      }
    }
  }
  run shutter_seq
}

# Define palette
palette rainbow_with_white = [
  0xFC0000        # Red
  0xFF8000        # Orange
  0xFFFF00        # Yellow
  0x00FF00        # Green
  0x00FFFF        # Cyan
  0x0080FF        # Blue
  0x8000FF        # Violet
  0xCCCCCC        # White
]

# Use the template
animation main = shutter_bidir(colors = rainbow_with_white, period = 1.5s)
run main
```

## Quick Reference Card

### Animation Types

| Animation | Description | Key Parameters |
|-----------|-------------|----------------|
| `solid` | Solid color fill | `color` |
| `twinkle_animation` | Twinkling stars effect | `color`, `density`, `twinkle_speed`, `fade_speed` |
| `beacon_animation` | Positioned pulse/highlight | `color`, `pos`, `beacon_size`, `slew_size` |
| `crenel_animation` | Square wave pattern | `color`, `back_color`, `pulse_size`, `low_size` |
| `rich_palette_animation` | Smooth palette cycling | `colors`, `period`, `transition_type` |
| `palette_gradient_animation` | Gradient across strip | `color_source`, `spatial_period`, `shift_period` |
| `palette_meter_animation` | VU-meter style bar | `color_source`, `level` |
| `breathe_animation` | Breathing/pulsing effect | `color`, `period` |
| `comet_animation` | Moving comet with tail | `color`, `tail_length`, `speed` |

### Colors

```berry
# Predefined
red, green, blue, white, yellow, orange, purple, cyan, transparent

# Custom hex
color my_color = 0xRRGGBB
```

### Palettes

```berry
# Built-in
PALETTE_RAINBOW, PALETTE_RAINBOW_W, PALETTE_FIRE

# Custom
palette my_palette = [ 0xFF0000, 0x00FF00, 0x0000FF ]

# Position-based
palette gradient = [ (0, red), (128, yellow), (255, green) ]
```

### Value Providers

```berry
sine_osc(min_value=0, max_value=255, duration=2s)
cosine_osc(min_value=0, max_value=255, duration=2s)
triangle(min_value=0, max_value=255, duration=2s)
sawtooth(min_value=0, max_value=255, duration=2s)
smooth(min_value=0, max_value=255, duration=2s)
square(min_value=0, max_value=255, duration=2s)
strip_length()
```

### Time Units

```berry
500ms    # milliseconds
2s       # seconds
1m       # minutes
```

### Percentages

```berry
0%       # 0
50%      # 128
100%     # 255
```

### Sequence Statements

```berry
play animation for 5s
wait 1s
restart value_provider
repeat 3 times { ... }
repeat forever { ... }
if condition { ... }
```

### Template Definition

```berry
template animation name {
  param param_name type type_name default value
  # ... body ...
}
```

### Resources

- **[DSL Reference](Dsl_Reference.md)** - Complete syntax documentation
- **[Animation Classes](Animation_Class_Hierarchy.md)** - All available animations
- **[Oscillation Patterns](Oscillation_Patterns.md)** - Value provider waveforms
- **[Examples](Examples.md)** - More animation examples
- **[Online Emulator](https://tasmota.github.io/docs/Tasmota-Berry-emulator/index.html)** - Test animations in your browser
