# Change Log

All notable changes to the Animation DSL extension will be documented in this file.

## [1.2.0] - 2025-01-24

### Added
- Support for `if` keyword for conditional execution in sequences
- Conditional execution allows boolean-based gating (runs 0 or 1 times)

### Changed
- Updated keyword patterns to include `if` statement

## [1.1.0] - 2025-01-09

### Added
- Support for `import` statements for Berry modules
- Support for `template` and `param` keywords for template definitions
- Support for `type` annotations in template parameters (`param name type color`)
- Support for `set` keyword for variable assignments
- Support for `reset` and `restart` keywords for value provider/animation control
- Support for `log` keyword for debug logging
- Support for `as` keyword in import and parameter declarations
- Mathematical functions: `abs`, `max`, `min`, `round`, `sqrt`, `scale`, `sin`, `cos`
- User function syntax: `user.function_name()` with proper highlighting
- Additional animation functions: `pulsating_animation`
- Additional value providers: `triangle`, `cosine_osc`, `sawtooth`, `color_cycle`, `strip_length`
- Additional easing types: `triangle`, `sine`, `sawtooth`, `elastic`, `bounce`
- Support for hexadecimal colors with `0x` prefix (e.g., `0xFF0000`, `0x80FF0000`)
- Additional animation properties: `opacity`, `priority`, `pos`, `beacon_size`, `slew_size`, `direction`, `tail_length`, `speed`, `period`, `cycle_period`, `min_value`, `max_value`, `duration`, `next`

### Changed
- Updated keyword patterns to match current DSL syntax based on actual examples
- Improved indentation rules to support `template` blocks with proper `{}`
- Enhanced color recognition to support both `#RRGGBB` and `0xRRGGBB` formats
- Expanded animation function list to include all currently implemented functions
- Updated oscillator functions to match actual DSL implementation

### Fixed
- Corrected animation function names to match actual DSL implementation
- Updated oscillator function patterns to include all available value providers
- Fixed keyword list to remove deprecated/unused keywords and add missing ones

## [1.0.0] - 2024-01-30

### Added
- Initial release of Animation DSL syntax highlighting
- Complete syntax highlighting for all DSL constructs:
  - Keywords (strip, color, palette, animation, sequence, etc.)
  - Animation functions (solid, rich_palette_animation, beacon_animation, etc.)
  - Oscillator functions (ramp, linear, smooth, square)
  - Colors (hex colors and 30+ named colors)
  - Time literals (ms, s, m, h)
  - Percentages (50%, 100%)
  - Comments (# line comments)
- Semantic token scopes that work with any VSCode theme
- Language configuration with:
  - Auto-closing pairs for brackets and quotes
  - Comment toggling support
  - Smart indentation for sequences and loops
  - Bracket matching
- File association for .anim files
- Comprehensive documentation and examples

### Features
- **Syntax Highlighting**: Full coverage of Animation DSL grammar
- **Theme Compatibility**: Works with any VSCode theme (dark, light, high contrast)
- **Language Features**: Auto-closing, indentation, bracket matching
- **Documentation**: Complete README with examples and usage guide

### Supported DSL Features
- Strip configuration (`strip length 60`)
- Color definitions (`color red = #FF0000`)
- Palette definitions with VRGB format
- Animation definitions (8 animation functions)
- Value providers (4 oscillator functions)
- Property assignments (`animation.priority = 10`)
- Sequences with play, wait, and repeat
- Comments with preservation
- All currently implemented DSL constructs

### Technical Details
- Based on TextMate grammar for syntax highlighting
- JSON-based snippet system with parameter placeholders
- Custom theme with semantic color coding
- Language configuration for VSCode integration
- Supports all Animation DSL file extensions (.anim)