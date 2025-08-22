# Berry Animation Framework

A powerful, lightweight animation framework for controlling addressable LED strips in Tasmota using a simple Domain-Specific Language (DSL).

## ✨ Features

- **🎨 Rich Animation Effects** - Pulse, breathe, fire, comet, sparkle, wave, and more
- **🌈 Advanced Color System** - Predefined palettes, custom gradients, smooth color cycling
- **📝 Simple DSL Syntax** - Write animations in intuitive, declarative language
- **⚡ High Performance** - Optimized for embedded systems with minimal memory usage
- **🔧 Extensible** - Create custom animations and effects
- **🎯 Position-Based Effects** - Precise control over individual LED positions
- **📊 Dynamic Parameters** - Animate colors, positions, sizes with oscillating values
- **🎭 Sequences** - Create complex shows with timing and loops

## 🚀 Quick Start

### Simple Pulsing Animation

```dsl
# Define colors
color red = #FF0000

# Create pulsing animation
animation pulse_red = pulsating_animation(color=red, period=3s)

# Run it
run pulse_red
```

### Rainbow Color Cycling

```dsl
# Use predefined rainbow palette
animation rainbow_cycle = rich_palette(
  palette=PALETTE_RAINBOW,
  cycle_period=5s,
  transition_type=1
)

run rainbow_cycle
```

### Custom Color Palette

```dsl
# Define a sunset palette
palette sunset = [
  (0, #191970),    # Midnight blue
  (64, purple),    # Purple
  (128, #FF69B4),  # Hot pink
  (192, orange),   # Orange
  (255, yellow)    # Yellow
]

# Create palette animation
animation sunset_glow = rich_palette(
  palette=sunset,
  cycle_period=8s,
  transition_type=1
)

run sunset_glow
```

### Animation Sequences

```dsl
animation red_pulse = pulsating_animation(color=red, period=2s)
animation green_pulse = pulsating_animation(color=green, period=2s)
animation blue_pulse = pulsating_animation(color=blue, period=2s)

sequence rgb_show {
  play red_pulse for 3s
  wait 500ms
  play green_pulse for 3s
  wait 500ms
  play blue_pulse for 3s
  
  repeat 2 times:
    play red_pulse for 1s
    play green_pulse for 1s
    play blue_pulse for 1s
}

run rgb_show
```

## 📚 Documentation

### Getting Started
- **[Quick Start Guide](docs/QUICK_START.md)** - Get up and running in 5 minutes
- **[DSL Reference](docs/DSL_REFERENCE.md)** - Complete DSL syntax and features
- **[Examples](docs/EXAMPLES.md)** - Comprehensive examples and tutorials

### Reference
- **[Animation Class Hierarchy](docs/ANIMATION_CLASS_HIERARCHY.md)** - All available animations and parameters
- **[Oscillation Patterns](docs/OSCILLATION_PATTERNS.md)** - Dynamic value patterns and waveforms
- **[Troubleshooting](docs/TROUBLESHOOTING.md)** - Common issues and solutions

### Advanced
- **[User Functions](docs/USER_FUNCTIONS.md)** - Create custom animation functions
- **[Animation Development](docs/ANIMATION_DEVELOPMENT.md)** - Create custom animations

## 🎯 Core Concepts

### DSL-First Design
Write animations using simple, declarative syntax:
```dsl
animation fire_effect = fire_animation(intensity=200, cooling=55, sparking=120)
run fire_effect
```

### Dynamic Parameters
Use oscillating values to create complex effects:
```dsl
animation pulsing_comet = comet_animation(
  color=red,
  tail_length=smooth_oscillator(5, 15, 3s),
  speed=2
)
```

### Color Palettes
Rich color transitions with predefined or custom palettes:
```dsl
palette custom = [(0, blue), (128, purple), (255, pink)]
animation palette_cycle = rich_palette(palette=custom, cycle_period=4s)
```

## 🎨 Animation Types

### Basic Effects
- **Pulse** - Breathing/pulsing effects with smooth transitions
- **Sparkle** - Random twinkling and starfield effects
- **Fire** - Realistic fire simulation with warm colors
- **Comet** - Moving comet with customizable tail

### Color Animations  
- **Rich Palette** - Smooth color transitions using predefined palettes
- **Color Cycling** - Custom color sequences with smooth blending
- **Gradient** - Linear and radial color gradients
- **Plasma** - Classic plasma effects with sine wave interference

### Pattern Effects
- **Wave** - Mathematical waveforms (sine, triangle, square, sawtooth)
- **Noise** - Organic patterns using Perlin noise
- **Position-Based** - Precise control over individual LED positions

### Motion Effects
- **Bounce** - Physics-based bouncing with gravity and damping
- **Shift** - Scrolling and translation effects
- **Scale** - Size transformation and breathing effects
- **Jitter** - Add random variations to any animation

## 🔧 Installation

### Prerequisites
- Tasmota firmware with Berry support
- Addressable LED strip (WS2812, SK6812, etc.)

### Setup
1. **Enable Berry** in Tasmota configuration
2. **Configure LED strip** using Tasmota's LED configuration
3. **Import the framework**:
   ```berry
   import animation
   ```
4. **Create your first animation** using the DSL

## 🌈 Predefined Palettes

The framework includes several built-in color palettes:

- **PALETTE_RAINBOW** - Standard 7-color rainbow (Red → Orange → Yellow → Green → Blue → Indigo → Violet)
- **PALETTE_RGB** - Simple RGB cycle (Red → Green → Blue)
- **PALETTE_FIRE** - Warm fire colors (Black → Dark Red → Red → Orange → Yellow)
- **PALETTE_SUNSET_TICKS** - Sunset colors (Orange Red → Dark Orange → Gold → Hot Pink → Purple → Midnight Blue)
- **PALETTE_OCEAN** - Blue and green ocean tones (Navy → Blue → Cyan → Spring Green → Green)
- **PALETTE_FOREST** - Various green forest tones (Dark Green → Forest Green → Lime Green → Mint Green → Light Green)

```dsl
# Use any predefined palette
animation ocean_waves = rich_palette(
  palette=PALETTE_OCEAN,
  cycle_period=8s,
  transition_type=1
)
run ocean_waves
```

## ⚡ Performance

Optimized for embedded systems:

- **Memory Efficient** - Minimal RAM usage
- **CPU Optimized** - Efficient rendering algorithms  
- **Scalable** - Handles strips from 10 to 1000+ LEDs
- **Real-time** - Smooth 30+ FPS animation

### Performance Tips
- Use longer animation periods (2-5 seconds) for smoother performance
- Limit simultaneous animations (3-5 max recommended)
- Consider strip length when designing complex effects

## 🤝 Contributing

Contributions are welcome! Areas for contribution:
- New animation effects
- Performance optimizations  
- Documentation improvements
- Example animations
- Bug fixes and testing

## 📄 License

This project is licensed under the MIT License.

---

**Happy Animating!** 🎨✨