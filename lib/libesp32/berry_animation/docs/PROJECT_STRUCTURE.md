# Project Structure

This document explains the organization of the Tasmota Berry Animation Framework project.

## Root Directory

```
├── README.md                    # Main project overview and quick start
├── docs/                        # User documentation
├── lib/libesp32/berry_animation/ # Framework source code
├── anim_examples/               # DSL animation examples (.anim files)
├── anim_examples/compiled/      # Compiled Berry code from DSL examples
├── .kiro/                       # Project specifications and design docs
└── .docs_archive/               # Archived technical implementation docs
```

## Documentation (`docs/`)

User-focused documentation for learning and using the framework:

```
docs/
├── QUICK_START.md              # 5-minute getting started guide
├── API_REFERENCE.md            # Complete Berry API documentation
├── EXAMPLES.md                 # Curated examples with explanations
├── TROUBLESHOOTING.md          # Common issues and solutions
└── PROJECT_STRUCTURE.md        # This file
```

## Framework Source Code (`lib/libesp32/berry_animation/`)

The complete framework implementation:

```
lib/libesp32/berry_animation/
├── animation.be                # Main module entry point
├── README.md                   # Framework-specific readme
├── user_functions.be           # Example user-defined functions
│
├── core/                       # Core framework classes
│   ├── animation_base.be       # Animation base class
│   ├── animation_engine.be     # Unified animation engine
│   ├── event_handler.be        # Event system
│   ├── frame_buffer.be         # Frame buffer management
│   ├── pattern_base.be         # Pattern base class
│   ├── sequence_manager.be     # Sequence orchestration
│   └── user_functions.be       # User function registry
│
├── effects/                    # Animation implementations
│   ├── breathe.be             # Breathing animation
│   ├── comet.be               # Comet effect
│   ├── crenel_position.be     # Rectangular pulse patterns
│   ├── filled.be              # Filled animations
│   ├── fire.be                # Fire simulation
│   ├── palette_pattern.be     # Palette-based patterns
│   ├── palettes.be            # Predefined palettes
│   ├── pattern_animation.be   # Pattern wrapper animation
│   ├── pulse.be               # Pulse animation
│   ├── pulse_position.be      # Position-based pulse
│   └── twinkle.be             # Twinkling stars
│
├── patterns/                   # Pattern implementations
│   └── solid_pattern.be       # Solid color pattern
│
├── providers/                  # Value and color providers
│   ├── color_cycle_color_provider.be    # Color cycling
│   ├── color_provider.be               # Base color provider
│   ├── composite_color_provider.be     # Blended colors
│   ├── oscillator_value_provider.be    # Waveform generators
│   ├── rich_palette_color_provider.be  # Palette-based colors
│   ├── solid_color_provider.be         # Static colors
│   ├── static_value_provider.be        # Static value wrapper
│   └── value_provider.be               # Base value provider
│
├── dsl/                        # Domain-Specific Language
│   ├── lexer.be               # DSL tokenizer
│   ├── runtime.be             # DSL execution runtime
│   ├── token.be               # Token definitions
│   └── transpiler.be          # DSL to Berry transpiler
│
├── tests/                      # Comprehensive test suite
│   ├── test_all.be            # Run all tests
│   ├── animation_engine_test.be
│   ├── dsl_transpiler_test.be
│   ├── event_system_test.be
│   └── ... (50+ test files)
│
├── examples/                   # Berry code examples
│   ├── run_all_demos.be       # Run all examples
│   ├── simple_engine_test.be  # Basic usage
│   ├── color_provider_demo.be # Color system demo
│   ├── event_system_demo.be   # Interactive animations
│   └── ... (60+ example files)
│
└── docs/                       # Framework-specific documentation
    ├── architecture_simplification.md
    ├── class_hierarchy_reference.md
    ├── migration_guide.md
    └── ... (technical documentation)
```

## DSL Examples (`anim_examples/`)

Ready-to-use animation files in DSL format:

```
anim_examples/
├── aurora_borealis.anim        # Northern lights effect
├── breathing_colors.anim       # Smooth color breathing
├── fire_demo.anim             # Realistic fire simulation
├── palette_demo.anim          # Palette showcase
├── rainbow_cycle.anim         # Rainbow color cycling
└── simple_pulse.anim          # Basic pulsing effect
```

## Compiled Examples (`anim_examples/compiled/`)

Berry code generated from DSL examples (for reference):

```
anim_examples/compiled/
├── aurora_borealis.be         # Compiled from aurora_borealis.anim
├── breathing_colors.be        # Compiled from breathing_colors.anim
└── ... (compiled versions of .anim files)
```

## Project Specifications (`.kiro/specs/berry-animation-framework/`)

Design documents and specifications:

```
.kiro/specs/berry-animation-framework/
├── design.md                  # Architecture overview
├── requirements.md            # Project requirements (✅ complete)
├── dsl-specification.md       # DSL syntax reference
├── dsl-grammar.md            # DSL grammar specification
├── EVENT_SYSTEM.md           # Event system documentation
├── USER_FUNCTIONS.md         # User-defined functions guide
├── palette-quick-reference.md # Palette usage guide
└── future_features.md        # Planned enhancements
```

## Archived Documentation (`.docs_archive/`)

Technical implementation documents moved from active documentation:

```
.docs_archive/
├── dsl-transpiler-architecture.md    # Detailed transpiler design
├── dsl-implementation.md             # Implementation details
├── color_provider_system.md          # Color system internals
├── unified-architecture-summary.md   # Architecture migration notes
└── ... (50+ archived technical docs)
```

## Key Files for Different Use Cases

### Getting Started
1. **`README.md`** - Project overview and quick examples
2. **`docs/QUICK_START.md`** - 5-minute tutorial
3. **`anim_examples/simple_pulse.anim`** - Basic DSL example

### Learning the API
1. **`docs/API_REFERENCE.md`** - Complete API documentation
2. **`docs/EXAMPLES.md`** - Curated examples with explanations
3. **`lib/libesp32/berry_animation/examples/simple_engine_test.be`** - Basic Berry usage

### Using the DSL
1. **`.kiro/specs/berry-animation-framework/dsl-specification.md`** - Complete DSL syntax
2. **`.kiro/specs/berry-animation-framework/palette-quick-reference.md`** - Palette guide
3. **`anim_examples/`** - Working DSL examples

### Advanced Features
1. **`.kiro/specs/berry-animation-framework/USER_FUNCTIONS.md`** - Custom functions
2. **`.kiro/specs/berry-animation-framework/EVENT_SYSTEM.md`** - Interactive animations
3. **`lib/libesp32/berry_animation/user_functions.be`** - Example custom functions

### Troubleshooting
1. **`docs/TROUBLESHOOTING.md`** - Common issues and solutions
2. **`lib/libesp32/berry_animation/tests/test_all.be`** - Run all tests
3. **`lib/libesp32/berry_animation/examples/run_all_demos.be`** - Test examples

### Framework Development
1. **`.kiro/specs/berry-animation-framework/design.md`** - Architecture overview
2. **`.kiro/specs/berry-animation-framework/requirements.md`** - Project requirements
3. **`lib/libesp32/berry_animation/tests/`** - Test suite for development

## File Naming Conventions

### Source Code
- **`*.be`** - Berry source files
- **`*_test.be`** - Test files
- **`*_demo.be`** - Example/demonstration files

### Documentation
- **`*.md`** - Markdown documentation
- **`README.md`** - Overview documents
- **`QUICK_START.md`** - Getting started guides
- **`API_REFERENCE.md`** - API documentation

### DSL Files
- **`*.anim`** - DSL animation files
- **`*.be`** (in anim_examples/compiled/) - Compiled Berry code from DSL

## Navigation Tips

### For New Users
1. Start with `README.md` for project overview
2. Follow `docs/QUICK_START.md` for hands-on tutorial
3. Browse `anim_examples/` for inspiration
4. Reference `docs/API_REFERENCE.md` when needed

### For DSL Users
1. Learn syntax from `.kiro/specs/berry-animation-framework/dsl-specification.md`
2. Study examples in `anim_examples/`
3. Use palette guide: `.kiro/specs/berry-animation-framework/palette-quick-reference.md`
4. Create custom functions: `.kiro/specs/berry-animation-framework/USER_FUNCTIONS.md`

### For Berry Developers
1. Study `lib/libesp32/berry_animation/examples/simple_engine_test.be`
2. Reference `docs/API_REFERENCE.md` for complete API
3. Run tests: `lib/libesp32/berry_animation/tests/test_all.be`
4. Explore advanced examples in `lib/libesp32/berry_animation/examples/`

### For Framework Contributors
1. Understand architecture: `.kiro/specs/berry-animation-framework/design.md`
2. Review requirements: `.kiro/specs/berry-animation-framework/requirements.md`
3. Study source code in `lib/libesp32/berry_animation/core/`
4. Run comprehensive tests: `lib/libesp32/berry_animation/tests/test_all.be`

This structure provides clear separation between user documentation, source code, examples, and technical specifications, making it easy to find relevant information for any use case.