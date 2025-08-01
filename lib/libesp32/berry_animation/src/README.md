# Tasmota Berry Animation Framework

A powerful, lightweight animation framework for controlling addressable LED strips in Tasmota using Berry scripting language.

## ✨ Features

- **🎨 Rich Animation Effects** - Pulse, breathe, fire, comet, twinkle, and more
- **🌈 Advanced Color System** - Palettes, gradients, color cycling with smooth transitions
- **📝 Domain-Specific Language (DSL)** - Write animations in intuitive, declarative syntax
- **⚡ High Performance** - Optimized for embedded systems with minimal memory usage
- **🔧 Extensible** - Create custom animations and user-defined functions
- **🎯 Position-Based Effects** - Precise control over individual LED positions
- **📊 Dynamic Parameters** - Animate colors, positions, sizes with value providers
- **🎭 Event System** - Responsive animations that react to button presses, timers, and sensors

## 🚀 Quick Start

### 1. Basic Berry Animation

```berry
import animation

# Create LED strip (60 LEDs)
var strip = Leds(60)
var engine = animation.create_engine(strip)

# Create a pulsing red animation
var pulse_red = animation.pulse(
  animation.solid(0xFFFF0000),  # Red color
  2000,                         # 2 second period
  50,                          # Min brightness (0-255)
  255                          # Max brightness (0-255)
)

# Start the animation
engine.add_animation(pulse_red)
engine.start()
```

### 2. Using the Animation DSL

Create a file `my_animation.anim`:

```dsl
# Define colors
color red = #FF0000
color blue = #0000FF

# Create animations
animation pulse_red = pulse(solid(red), 2s, 20%, 100%)
animation pulse_blue = pulse(solid(blue), 3s, 30%, 100%)

# Create a sequence
sequence demo {
  play pulse_red for 5s
  wait 1s
  play pulse_blue for 5s
  repeat 3 times:
    play pulse_red for 2s
    play pulse_blue for 2s
}

run demo
```

Load and run the DSL:

```berry
import animation

var strip = Leds(60)
var runtime = animation.DSLRuntime(animation.create_engine(strip))
runtime.load_dsl_file("my_animation.anim")
```

### 3. Palette-Based Animations

```dsl
# Define a fire palette
palette fire_colors = [
  (0, #000000),    # Black
  (64, #800000),   # Dark red
  (128, #FF0000),  # Red
  (192, #FF8000),  # Orange
  (255, #FFFF00)   # Yellow
]

# Create fire animation
animation fire_effect = rich_palette_animation(fire_colors, 3s, smooth, 255)

run fire_effect
```

## 📚 Documentation

### User Guides
- **[Quick Start Guide](docs/QUICK_START.md)** - Get up and running in 5 minutes
- **[API Reference](docs/API_REFERENCE.md)** - Complete Berry API documentation
- **[Examples](docs/EXAMPLES.md)** - Curated examples with explanations
- **[Troubleshooting](docs/TROUBLESHOOTING.md)** - Common issues and solutions

### DSL (Domain-Specific Language)
- **[DSL Reference](.kiro/specs/berry-animation-framework/dsl-specification.md)** - Complete DSL syntax guide
- **[DSL Grammar](.kiro/specs/berry-animation-framework/dsl-grammar.md)** - Formal grammar specification
- **[Palette Guide](.kiro/specs/berry-animation-framework/palette-quick-reference.md)** - Working with color palettes

### Advanced Topics
- **[User Functions](.kiro/specs/berry-animation-framework/USER_FUNCTIONS.md)** - Create custom animation functions
- **[Event System](.kiro/specs/berry-animation-framework/EVENT_SYSTEM.md)** - Responsive, interactive animations
- **[Project Structure](docs/PROJECT_STRUCTURE.md)** - Navigate the codebase

### Framework Design
- **[Requirements](.kiro/specs/berry-animation-framework/requirements.md)** - Project goals and requirements (✅ Complete)
- **[Architecture](.kiro/specs/berry-animation-framework/design.md)** - Framework design and architecture
- **[Future Features](.kiro/specs/berry-animation-framework/future_features.md)** - Planned enhancements

## 🎯 Core Concepts

### Unified Architecture
The framework uses a **unified pattern-animation architecture** where `Animation` extends `Pattern`. This means:
- Animations ARE patterns with temporal behavior
- Infinite composition: animations can use other animations as base patterns
- Consistent API across all visual elements

### Animation Engine
The `AnimationEngine` is the heart of the framework:
- Manages multiple animations with priority-based layering
- Handles timing, blending, and LED output
- Integrates with Tasmota's `fast_loop` for smooth performance

### Value Providers
Dynamic parameters that change over time:
- **Static values**: `solid(red)` 
- **Oscillators**: `pulse(solid(red), smooth(50, 255, 2s))`
- **Color providers**: `rich_palette_animation(fire_palette, 3s)`

## 🎨 Animation Types

### Basic Animations
- **`solid(color)`** - Static color fill
- **`pulse(pattern, period, min, max)`** - Pulsing brightness
- **`breathe(color, period)`** - Smooth breathing effect

### Pattern-Based Animations  
- **`rich_palette_animation(palette, period, easing, brightness)`** - Palette color cycling
- **`gradient(color1, color2, ...)`** - Color gradients
- **`fire_animation(intensity, speed)`** - Realistic fire simulation

### Position-Based Animations
- **`pulse_position_animation(color, pos, size, fade)`** - Localized pulse
- **`comet_animation(color, tail_length, speed)`** - Moving comet effect
- **`twinkle_animation(color, density, speed)`** - Twinkling stars

### Motion Effects
- **`shift_left(pattern, speed)`** - Move pattern left
- **`shift_right(pattern, speed)`** - Move pattern right
- **`bounce(pattern, period)`** - Bouncing motion

## 🔧 Installation

### For Tasmota Development
1. Copy the `lib/libesp32/berry_animation/` directory to your Tasmota build
2. The framework will be available as the `animation` module
3. Use `import animation` in your Berry scripts

### For Testing/Development
1. Install Berry interpreter with Tasmota extensions
2. Set module path: `berry -m lib/libesp32/berry_animation`
3. Run examples: `berry examples/simple_engine_test.be`

## 🧪 Examples

The framework includes comprehensive examples:

### Berry Examples
- **[Basic Engine](lib/libesp32/berry_animation/examples/simple_engine_test.be)** - Simple animation setup
- **[Color Providers](lib/libesp32/berry_animation/examples/color_provider_demo.be)** - Dynamic color effects
- **[Position Effects](lib/libesp32/berry_animation/examples/pulse_position_animation_demo.be)** - Localized animations
- **[Event System](lib/libesp32/berry_animation/examples/event_system_demo.be)** - Interactive animations

### DSL Examples
- **[Aurora Borealis](anim_examples/aurora_borealis.anim)** - Northern lights effect
- **[Breathing Colors](anim_examples/breathing_colors.anim)** - Smooth color breathing
- **[Fire Effect](anim_examples/fire_demo.anim)** - Realistic fire simulation

## 🤝 Contributing

### Running Tests
```bash
# Run all tests
berry lib/libesp32/berry_animation/tests/test_all.be

# Run specific test
berry lib/libesp32/berry_animation/tests/animation_engine_test.be
```

### Code Style
- Follow Berry language conventions
- Use descriptive variable names
- Include comprehensive comments
- Add test coverage for new features

## 📄 License

This project is part of the Tasmota ecosystem and follows the same licensing terms.

## 🙏 Acknowledgments

- **Tasmota Team** - For the excellent IoT platform
- **Berry Language** - For the lightweight scripting language
- **Community Contributors** - For testing, feedback, and improvements

---

**Ready to create amazing LED animations?** Start with the [Quick Start Guide](docs/QUICK_START.md)!