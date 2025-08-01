# Ported Examples - Legacy to Modern Animation Framework

This directory contains examples that demonstrate the evolution from the legacy `animate` module to the modern Berry Animation Framework. Each example is provided in three versions to show different approaches and capabilities.

## Overview

The examples are organized into three categories:

- **Option A: Direct 1:1 Ports** (`ported/`) - Exact functionality using new framework
- **Option B: Enhanced Modern Versions** (`enhanced/`) - Improved versions showcasing new capabilities  
- **Option C: DSL-Based Ports** (`dsl/`) - Declarative syntax versions

## Example Categories

### 1. Breathing Effect Examples

**Original**: `berry_animate_examples/animate_demo_breathe.be`
- Used old `animate.core()`, `animate.pulse()`, `animate.palette()`, `animate.oscillator()`
- Complex callback-based architecture
- ~25 lines of setup code

**Ported Versions**:
- **A**: `ported/breathe_demo_ported.be` - Direct port using `BreatheAnimation` + `RichPaletteColorProvider`
- **B**: `enhanced/modern_breathe_showcase.be` - Multi-layer breathing with events and dynamic parameters
- **C**: `dsl/breathe_demo.anim` - Clean DSL syntax (~15 lines)

### 2. Palette Background Examples

**Original**: `berry_animate_examples/animate_demo_palette_background.be`
- Used old `animate.core()` with `add_background_animator()`
- Simple rainbow cycling background

**Ported Versions**:
- **A**: `ported/palette_background_demo_ported.be` - Direct port using `FilledAnimation` + rainbow palette
- **B**: `enhanced/advanced_palette_demo.be` - Multiple palettes with layering and mode switching
- **C**: `dsl/palette_background_demo.anim` - Minimal DSL syntax

### 3. Pulse Effect Examples

**Original**: `berry_animate_examples/animate_demo_pulse.be`
- Used old `animate.core()`, `animate.pulse()`, `animate.oscillator()`, `animate.palette()`
- Moving pulse with oscillating position and cycling color

**Ported Versions**:
- **A**: `ported/pulse_demo_ported.be` - Direct port using `PulsePositionAnimation` + `OscillatorValueProvider`
- **B**: `enhanced/dynamic_pulse_demo.be` - Multiple pulse types with interactive control
- **C**: `dsl/pulse_demo.anim` - Declarative pulse definition

### 4. Blending Examples

**Original**: `berry_animate_examples/leds_blend_demo.be`
- Used old `animate.frame()` with manual blending
- Low-level frame buffer operations

**Ported Versions**:
- **A**: `ported/blend_demo_ported.be` - Direct port using new `FrameBuffer` blending
- **B**: `enhanced/comprehensive_blend_demo.be` - Advanced multi-layer blending with interactive control
- **C**: DSL blending is handled automatically by the animation engine

## Key Improvements in New Framework

### Code Simplicity
- **Legacy**: Complex callback chains and manual object management
- **Modern**: Clean object-oriented design with value providers
- **DSL**: Declarative syntax that's easy to read and modify

### Performance
- **Legacy**: Multiple separate objects with manual coordination
- **Modern**: Unified engine with optimized rendering pipeline
- **DSL**: Compiled to optimized Berry code

### Capabilities
- **Legacy**: Basic effects with manual parameter control
- **Modern**: Advanced layering, blending, dynamic parameters, event system
- **DSL**: Full framework capabilities with simple syntax

### Maintainability
- **Legacy**: Tightly coupled code, hard to modify
- **Modern**: Modular design, easy to extend
- **DSL**: Configuration-based, non-programmers can create animations

## Running the Examples

### Direct Ports (Option A)
```berry
# Run individual ported examples
load("lib/libesp32/berry_animation/examples/ported/breathe_demo_ported.be")
load("lib/libesp32/berry_animation/examples/ported/palette_background_demo_ported.be")
load("lib/libesp32/berry_animation/examples/ported/pulse_demo_ported.be")
load("lib/libesp32/berry_animation/examples/ported/blend_demo_ported.be")
```

### Enhanced Versions (Option B)
```berry
# Run enhanced examples with advanced features
load("lib/libesp32/berry_animation/examples/enhanced/modern_breathe_showcase.be")
load("lib/libesp32/berry_animation/examples/enhanced/advanced_palette_demo.be")
load("lib/libesp32/berry_animation/examples/enhanced/dynamic_pulse_demo.be")
load("lib/libesp32/berry_animation/examples/enhanced/comprehensive_blend_demo.be")
```

### DSL Versions (Option C)
```berry
# Run DSL examples using the demo runner
load("lib/libesp32/berry_animation/examples/dsl/run_dsl_demo.be")
# Edit the demo_file variable in run_dsl_demo.be to try different DSL files
```

## Interactive Features

Many enhanced examples include interactive event systems. You can trigger events from the Berry console:

```berry
# Speed up animations
animation.trigger_event("speed_up", {})

# Toggle layers
animation.trigger_event("toggle_layer", {"layer": "rainbow_comet"})

# Switch modes
animation.trigger_event("mode_switch", {})

# Adjust opacity
animation.trigger_event("adjust_opacity", {"layer": "all", "opacity": 100})
```

## Educational Value

These examples demonstrate:

1. **Migration Path**: How to convert legacy code to the new framework
2. **Architecture Evolution**: From callback-based to object-oriented to declarative
3. **Feature Progression**: Basic effects → Advanced layering → Simple DSL syntax
4. **Best Practices**: Modern Berry coding patterns and animation techniques
5. **Framework Capabilities**: Full range of new framework features

## File Structure

```
examples/
├── ported/                    # Direct 1:1 ports (Option A)
│   ├── breathe_demo_ported.be
│   ├── palette_background_demo_ported.be
│   ├── pulse_demo_ported.be
│   └── blend_demo_ported.be
├── enhanced/                  # Enhanced modern versions (Option B)
│   ├── modern_breathe_showcase.be
│   ├── advanced_palette_demo.be
│   ├── dynamic_pulse_demo.be
│   └── comprehensive_blend_demo.be
├── dsl/                       # DSL-based ports (Option C)
│   ├── breathe_demo.anim
│   ├── palette_background_demo.anim
│   ├── pulse_demo.anim
│   ├── enhanced_breathe_showcase.anim
│   ├── advanced_palette_showcase.anim
│   ├── dynamic_pulse_showcase.anim
│   └── run_dsl_demo.be       # DSL runner utility
└── PORTED_EXAMPLES_README.md  # This file
```

## Next Steps

1. Try running each version to see the visual differences
2. Compare the code complexity between versions
3. Experiment with the interactive features in enhanced versions
4. Create your own DSL animations using the examples as templates
5. Use the ported examples as a reference for migrating your own legacy animations

The progression from legacy → modern → DSL demonstrates the evolution of the animation framework and provides multiple approaches for different use cases and skill levels.