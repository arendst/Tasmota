# DSL Compilation Summary

## Overview

This document contains a summary of the DSL compilation process, including symbol tables and compilation outputs for all processed files.

## heartbeat_pulse.anim

**Status:** ✅ Success

## Symbol Table

| Symbol             | Type                       | Builtin | Dangerous | Takes Args |
|--------------------|----------------------------|---------|-----------|------------|
| `background`       | animation                  |         |           |            |
| `beacon_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `center_pulse`     | animation                  |         |           |            |
| `heart_bg`         | color                      |         |           |            |
| `heart_glow`       | animation                  |         |           |            |
| `heartbeat1`       | animation                  |         |           |            |
| `heartbeat2`       | animation                  |         |           |            |
| `smooth`           | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `solid`            | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `square`           | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## twinkle_stars.anim

**Status:** ✅ Success

## Symbol Table

| Symbol              | Type                  | Builtin | Dangerous | Takes Args |
|---------------------|-----------------------|---------|-----------|------------|
| `background`        | animation             |         |           |            |
| `bright_flash`      | animation             |         |           |            |
| `night_sky`         | color                 |         |           |            |
| `solid`             | animation_constructor |    ✓    |    ⚠️     |     ✓      |
| `stars`             | animation             |         |           |            |
| `twinkle_animation` | animation_constructor |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## sequence_assignments_demo.anim

**Status:** ✅ Success

## Symbol Table

| Symbol                | Type                       | Builtin | Dangerous | Takes Args |
|-----------------------|----------------------------|---------|-----------|------------|
| `beacon_animation`    | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `blue`                | color                      |    ✓    |           |            |
| `brightness_demo`     | sequence                   |         |           |            |
| `brightness_high`     | variable                   |         |           |            |
| `brightness_low`      | variable                   |         |           |            |
| `closure_value`       | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `color_cycle`         | color_constructor          |    ✓    |    ⚠️     |     ✓      |
| `cosine_osc`          | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `cosine_val`          | value_provider             |         |           |            |
| `cylon_eye`           | sequence                   |         |           |            |
| `eye_color`           | color                      |         |           |            |
| `eye_palette`         | palette                    |         |           |            |
| `green`               | color                      |    ✓    |           |            |
| `main_demo`           | sequence                   |         |           |            |
| `multi_change`        | sequence                   |         |           |            |
| `pulsating_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `pulse_demo`          | animation                  |         |           |            |
| `red_eye`             | animation                  |         |           |            |
| `red`                 | color                      |    ✓    |           |            |
| `repeat_demo`         | sequence                   |         |           |            |
| `strip_len`           | value_provider             |         |           |            |
| `strip_length`        | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `triangle_val`        | value_provider             |         |           |            |
| `triangle`            | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `violet`              | color                      |    ✓    |           |            |
| `yellow`              | color                      |    ✓    |           |            |

### Compilation Output

```
SUCCESS
```

## fire_flicker.anim

**Status:** ✅ Success

## Symbol Table

| Symbol              | Type                       | Builtin | Dangerous | Takes Args |
|---------------------|----------------------------|---------|-----------|------------|
| `LINEAR`            | constant                   |    ✓    |           |            |
| `fire_base_color`   | color                      |         |           |            |
| `fire_base`         | animation                  |         |           |            |
| `fire_colors`       | palette                    |         |           |            |
| `fire_flicker`      | animation                  |         |           |            |
| `flicker_pattern`   | color                      |         |           |            |
| `rich_palette`      | color_constructor          |    ✓    |    ⚠️     |     ✓      |
| `smooth`            | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `solid`             | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `twinkle_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## demo_shutter_rainbow_central.anim

**Status:** ✅ Success

## Symbol Table

| Symbol               | Type     | Builtin | Dangerous | Takes Args |
|----------------------|----------|---------|-----------|------------|
| `blue`               | color    |    ✓    |           |            |
| `green`              | color    |    ✓    |           |            |
| `indigo`             | color    |    ✓    |           |            |
| `orange`             | color    |    ✓    |           |            |
| `rainbow_with_white` | palette  |         |           |            |
| `red`                | color    |    ✓    |           |            |
| `shutter_central`    | template |         |           |            |
| `white`              | color    |    ✓    |           |            |
| `yellow`             | color    |    ✓    |           |            |

### Compilation Output

```
SUCCESS
```

## disco_strobe.anim

**Status:** ✅ Success

## Symbol Table

| Symbol              | Type                       | Builtin | Dangerous | Takes Args |
|---------------------|----------------------------|---------|-----------|------------|
| `LINEAR`            | constant                   |    ✓    |           |            |
| `beacon_animation`  | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `disco_base`        | animation                  |         |           |            |
| `disco_colors`      | palette                    |         |           |            |
| `disco_pulse`       | animation                  |         |           |            |
| `disco_rich_color`  | color                      |         |           |            |
| `disco_sparkles`    | animation                  |         |           |            |
| `pulse_pattern`     | color                      |         |           |            |
| `rich_palette`      | color_constructor          |    ✓    |    ⚠️     |     ✓      |
| `sawtooth`          | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `solid`             | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `sparkle_pattern`   | color                      |         |           |            |
| `square`            | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `twinkle_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `white_flash`       | animation                  |         |           |            |

### Compilation Output

```
SUCCESS
```

## test_simple_math.anim

**Status:** ✅ Success

## Symbol Table

| Symbol          | Type                       | Builtin | Dangerous | Takes Args |
|-----------------|----------------------------|---------|-----------|------------|
| `closure_value` | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `x`             | variable                   |         |           |            |
| `y`             | variable                   |         |           |            |

### Compilation Output

```
SUCCESS
```

## matrix_rain.anim

**Status:** ✅ Success

## Symbol Table

| Symbol              | Type                  | Builtin | Dangerous | Takes Args |
|---------------------|-----------------------|---------|-----------|------------|
| `LINEAR`            | constant              |    ✓    |           |            |
| `background`        | animation             |         |           |            |
| `code_flash`        | animation             |         |           |            |
| `comet_animation`   | animation_constructor |    ✓    |    ⚠️     |     ✓      |
| `matrix_bg`         | color                 |         |           |            |
| `matrix_greens`     | palette               |         |           |            |
| `rich_palette`      | color_constructor     |    ✓    |    ⚠️     |     ✓      |
| `solid`             | animation_constructor |    ✓    |    ⚠️     |     ✓      |
| `stream1_pattern`   | color                 |         |           |            |
| `stream1`           | animation             |         |           |            |
| `stream2_pattern`   | color                 |         |           |            |
| `stream2`           | animation             |         |           |            |
| `stream3_pattern`   | color                 |         |           |            |
| `stream3`           | animation             |         |           |            |
| `twinkle_animation` | animation_constructor |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## demo_shutter_rainbow2.anim

**Status:** ✅ Success

## Symbol Table

| Symbol              | Type                       | Builtin | Dangerous | Takes Args |
|---------------------|----------------------------|---------|-----------|------------|
| `PALETTE_RAINBOW`   | palette_constant           |    ✓    |           |            |
| `beacon_animation`  | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `closure_value`     | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `col1`              | color                      |         |           |            |
| `col2`              | color                      |         |           |            |
| `color_cycle`       | color_constructor          |    ✓    |    ⚠️     |     ✓      |
| `duration`          | variable                   |         |           |            |
| `sawtooth`          | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `shutter_animation` | animation                  |         |           |            |
| `shutter_run`       | sequence                   |         |           |            |
| `shutter_size`      | value_provider             |         |           |            |
| `strip_len`         | value_provider             |         |           |            |
| `strip_length`      | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## scanner_larson.anim

**Status:** ✅ Success

## Symbol Table

| Symbol             | Type                       | Builtin | Dangerous | Takes Args |
|--------------------|----------------------------|---------|-----------|------------|
| `background`       | animation                  |         |           |            |
| `beacon_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `closure_value`    | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `pos_test`         | value_provider             |         |           |            |
| `scanner_bg`       | color                      |         |           |            |
| `scanner_trail`    | animation                  |         |           |            |
| `scanner`          | animation                  |         |           |            |
| `solid`            | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `triangle`         | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## test_complex_template.anim

**Status:** ✅ Success

## Symbol Table

| Symbol          | Type     | Builtin | Dangerous | Takes Args |
|-----------------|----------|---------|-----------|------------|
| `fire_palette`  | palette  |         |           |            |
| `ocean_palette` | palette  |         |           |            |
| `rainbow_pulse` | template |         |           |            |

### Compilation Output

```
SUCCESS
```

## demo_shutter_rainbow_leftright.anim

**Status:** ✅ Success

## Symbol Table

| Symbol               | Type     | Builtin | Dangerous | Takes Args |
|----------------------|----------|---------|-----------|------------|
| `blue`               | color    |    ✓    |           |            |
| `green`              | color    |    ✓    |           |            |
| `indigo`             | color    |    ✓    |           |            |
| `orange`             | color    |    ✓    |           |            |
| `rainbow_with_white` | palette  |         |           |            |
| `red`                | color    |    ✓    |           |            |
| `shutter_lr`         | template |         |           |            |
| `white`              | color    |    ✓    |           |            |
| `yellow`             | color    |    ✓    |           |            |

### Compilation Output

```
SUCCESS
```

## candy_cane.anim

**Status:** ✅ Success

## Symbol Table

| Symbol             | Type                       | Builtin | Dangerous | Takes Args |
|--------------------|----------------------------|---------|-----------|------------|
| `beacon_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `candy_red`        | color                      |         |           |            |
| `candy_white`      | color                      |         |           |            |
| `closure_value`    | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `move_speed`       | variable                   |         |           |            |
| `sawtooth`         | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `stripe10`         | animation                  |         |           |            |
| `stripe1`          | animation                  |         |           |            |
| `stripe2`          | animation                  |         |           |            |
| `stripe3`          | animation                  |         |           |            |
| `stripe4`          | animation                  |         |           |            |
| `stripe5`          | animation                  |         |           |            |
| `stripe6`          | animation                  |         |           |            |
| `stripe7`          | animation                  |         |           |            |
| `stripe8`          | animation                  |         |           |            |
| `stripe9`          | animation                  |         |           |            |

### Compilation Output

```
SUCCESS
```

## fail_color_predefined.anim

**Status:** ❌ Failed

### Compilation Output

```
dsl_compilation_error: Line 4: Transpilation failed: Line 4: Cannot redefine predefined color 'red'. Use a different name like 'red_custom' or 'my_red'
stack traceback:
	<unknown source>: in function `error`
	<unknown source>: in function `transpile`
	<unknown source>: in function `main`
```

## test_shutter_rainbow_bidir.anim

**Status:** ✅ Success

## Symbol Table

| Symbol               | Type     | Builtin | Dangerous | Takes Args |
|----------------------|----------|---------|-----------|------------|
| `blue`               | color    |    ✓    |           |            |
| `green`              | color    |    ✓    |           |            |
| `indigo`             | color    |    ✓    |           |            |
| `orange`             | color    |    ✓    |           |            |
| `rainbow_with_white` | palette  |         |           |            |
| `red`                | color    |    ✓    |           |            |
| `shutter_bidir`      | template |         |           |            |
| `white`              | color    |    ✓    |           |            |
| `yellow`             | color    |    ✓    |           |            |

### Compilation Output

```
SUCCESS
```

## property_assignment_demo.anim

**Status:** ✅ Success

## Symbol Table

| Symbol             | Type                  | Builtin | Dangerous | Takes Args |
|--------------------|-----------------------|---------|-----------|------------|
| `beacon_animation` | animation_constructor |    ✓    |    ⚠️     |     ✓      |
| `blue_custom`      | color                 |         |           |            |
| `center_pulse`     | animation             |         |           |            |
| `demo`             | sequence              |         |           |            |
| `green_custom`     | color                 |         |           |            |
| `left_pulse`       | animation             |         |           |            |
| `red_custom`       | color                 |         |           |            |
| `right_pulse`      | animation             |         |           |            |

### Compilation Output

```
SUCCESS
```

## cylon_generic.anim

**Status:** ✅ Success

## Symbol Table

| Symbol         | Type     | Builtin | Dangerous | Takes Args |
|----------------|----------|---------|-----------|------------|
| `cylon_effect` | template |         |           |            |
| `red`          | color    |    ✓    |           |            |
| `transparent`  | color    |    ✓    |           |            |

### Compilation Output

```
SUCCESS
```

## cylon_rainbow.anim

**Status:** ✅ Success

## Symbol Table

| Symbol             | Type                       | Builtin | Dangerous | Takes Args |
|--------------------|----------------------------|---------|-----------|------------|
| `beacon_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `closure_value`    | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `color_cycle`      | color_constructor          |    ✓    |    ⚠️     |     ✓      |
| `cosine_osc`       | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `cosine_val`       | value_provider             |         |           |            |
| `cylon_eye`        | sequence                   |         |           |            |
| `eye_color`        | color                      |         |           |            |
| `eye_duration`     | variable                   |         |           |            |
| `eye_palette`      | palette                    |         |           |            |
| `green`            | color                      |    ✓    |           |            |
| `red_eye`          | animation                  |         |           |            |
| `red`              | color                      |    ✓    |           |            |
| `strip_len`        | value_provider             |         |           |            |
| `strip_length`     | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `triangle_val`     | value_provider             |         |           |            |
| `triangle`         | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `violet`           | color                      |    ✓    |           |            |
| `yellow`           | color                      |    ✓    |           |            |

### Compilation Output

```
SUCCESS
```

## demo_shutter_rainbow_bidir.anim

**Status:** ✅ Success

## Symbol Table

| Symbol               | Type     | Builtin | Dangerous | Takes Args |
|----------------------|----------|---------|-----------|------------|
| `blue`               | color    |    ✓    |           |            |
| `green`              | color    |    ✓    |           |            |
| `indigo`             | color    |    ✓    |           |            |
| `orange`             | color    |    ✓    |           |            |
| `rainbow_with_white` | palette  |         |           |            |
| `red`                | color    |    ✓    |           |            |
| `shutter_bidir`      | template |         |           |            |
| `white`              | color    |    ✓    |           |            |
| `yellow`             | color    |    ✓    |           |            |

### Compilation Output

```
SUCCESS
```

## christmas_tree.anim

**Status:** ✅ Success

## Symbol Table

| Symbol              | Type                       | Builtin | Dangerous | Takes Args |
|---------------------|----------------------------|---------|-----------|------------|
| `LINEAR`            | constant                   |    ✓    |           |            |
| `beacon_animation`  | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `comet_animation`   | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `garland_pattern`   | color                      |         |           |            |
| `garland`           | animation                  |         |           |            |
| `ornament_colors`   | palette                    |         |           |            |
| `ornament_pattern`  | color                      |         |           |            |
| `ornaments`         | animation                  |         |           |            |
| `rich_palette`      | color_constructor          |    ✓    |    ⚠️     |     ✓      |
| `smooth`            | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `snow_sparkles`     | animation                  |         |           |            |
| `solid`             | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `tree_base`         | animation                  |         |           |            |
| `tree_green`        | color                      |         |           |            |
| `tree_star`         | animation                  |         |           |            |
| `twinkle_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## plasma_wave.anim

**Status:** ✅ Success

## Symbol Table

| Symbol                   | Type                       | Builtin | Dangerous | Takes Args |
|--------------------------|----------------------------|---------|-----------|------------|
| `SINE`                   | constant                   |    ✓    |           |            |
| `beacon_animation`       | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `plasma_base`            | animation                  |         |           |            |
| `plasma_colors`          | palette                    |         |           |            |
| `plasma_wave1`           | animation                  |         |           |            |
| `plasma_wave2`           | animation                  |         |           |            |
| `plasma_wave3`           | animation                  |         |           |            |
| `rich_palette_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `rich_palette`           | color_constructor          |    ✓    |    ⚠️     |     ✓      |
| `smooth`                 | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `wave1_pattern`          | color                      |         |           |            |
| `wave2_pattern`          | color                      |         |           |            |
| `wave3_pattern`          | color                      |         |           |            |

### Compilation Output

```
SUCCESS
```

## ocean_waves.anim

**Status:** ✅ Success

## Symbol Table

| Symbol                   | Type                       | Builtin | Dangerous | Takes Args |
|--------------------------|----------------------------|---------|-----------|------------|
| `SINE`                   | constant                   |    ✓    |           |            |
| `beacon_animation`       | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `foam`                   | animation                  |         |           |            |
| `ocean_base`             | animation                  |         |           |            |
| `ocean_colors`           | palette                    |         |           |            |
| `rich_palette_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `rich_palette`           | color_constructor          |    ✓    |    ⚠️     |     ✓      |
| `sawtooth`               | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `twinkle_animation`      | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `wave1_pattern`          | color                      |         |           |            |
| `wave1`                  | animation                  |         |           |            |
| `wave2_pattern`          | color                      |         |           |            |
| `wave2`                  | animation                  |         |           |            |

### Compilation Output

```
SUCCESS
```

## swipe_rainbow.anim

**Status:** ✅ Success

## Symbol Table

| Symbol            | Type                       | Builtin | Dangerous | Takes Args |
|-------------------|----------------------------|---------|-----------|------------|
| `blue`            | color                      |    ✓    |           |            |
| `color_cycle`     | color_constructor          |    ✓    |    ⚠️     |     ✓      |
| `green`           | color                      |    ✓    |           |            |
| `indigo`          | color                      |    ✓    |           |            |
| `olivary`         | color                      |         |           |            |
| `orange`          | color                      |    ✓    |           |            |
| `palette_olivary` | palette                    |         |           |            |
| `red`             | color                      |    ✓    |           |            |
| `slide_colors`    | sequence                   |         |           |            |
| `solid`           | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `strip_len`       | value_provider             |         |           |            |
| `strip_length`    | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `swipe_animation` | animation                  |         |           |            |
| `violet`          | color                      |    ✓    |           |            |
| `white`           | color                      |    ✓    |           |            |
| `yellow`          | color                      |    ✓    |           |            |

### Compilation Output

```
SUCCESS
```

## meteor_shower.anim

**Status:** ✅ Success

## Symbol Table

| Symbol              | Type                  | Builtin | Dangerous | Takes Args |
|---------------------|-----------------------|---------|-----------|------------|
| `background`        | animation             |         |           |            |
| `comet_animation`   | animation_constructor |    ✓    |    ⚠️     |     ✓      |
| `meteor1`           | animation             |         |           |            |
| `meteor2`           | animation             |         |           |            |
| `meteor3`           | animation             |         |           |            |
| `meteor4`           | animation             |         |           |            |
| `meteor_flash`      | animation             |         |           |            |
| `solid`             | animation_constructor |    ✓    |    ⚠️     |     ✓      |
| `space_bg`          | color                 |         |           |            |
| `stars`             | animation             |         |           |            |
| `twinkle_animation` | animation_constructor |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## palette_demo.anim

**Status:** ✅ Success

## Symbol Table

| Symbol                   | Type                  | Builtin | Dangerous | Takes Args |
|--------------------------|-----------------------|---------|-----------|------------|
| `PALETTE_FOREST`         | palette_constant      |    ✓    |           |            |
| `fire_anim`              | animation             |         |           |            |
| `fire_colors`            | palette               |         |           |            |
| `forest_anim`            | animation             |         |           |            |
| `ocean_anim`             | animation             |         |           |            |
| `ocean_colors`           | palette               |         |           |            |
| `palette_demo`           | sequence              |         |           |            |
| `rich_palette_animation` | animation_constructor |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## fail_value_provider_add.anim

**Status:** ❌ Failed

### Compilation Output

```
dsl_compilation_error: Line 4: Transpilation failed: Line 4: Expression 'animation.linear(engine)' cannot be used in computed expressions. This creates a new instance at each evaluation. Use either:
  set var_name = animation.linear(engine)()  # Single function call
  set computed = (existing_var + 1) / 2  # Computation with existing values
stack traceback:
	<unknown source>: in function `error`
	<unknown source>: in function `transpile`
	<unknown source>: in function `main`
```

## fail_name_predefined.anim

**Status:** ❌ Failed

### Compilation Output

```
dsl_compilation_error: Line 4: Transpilation failed: Line 4: Cannot redefine built-in symbol 'abs' (type: 4). Use a different name like 'abs_custom' or 'my_abs'
stack traceback:
	<unknown source>: in function `error`
	<unknown source>: in function `transpile`
	<unknown source>: in function `main`
```

## test_shutter_rainbow_central.anim

**Status:** ❌ Failed

### Compilation Output

```
dsl_compilation_error: Line 12: Transpilation failed: Line 12: Template body transpilation failed: Line 12: Expression 'animation.strip_length(engine)' cannot be used in computed expressions. This creates a new instance at each evaluation. Use either:
  set var_name = animation.strip_length(engine)()  # Single function call
  set computed = (existing_var + 1) / 2  # Computation with existing values
stack traceback:
	<unknown source>: in function `error`
	<unknown source>: in function `transpile`
	<unknown source>: in function `main`
```

## sunrise_sunset.anim

**Status:** ✅ Success

## Symbol Table

| Symbol                   | Type                       | Builtin | Dangerous | Takes Args |
|--------------------------|----------------------------|---------|-----------|------------|
| `beacon_animation`       | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `daylight_colors`        | palette                    |         |           |            |
| `daylight_cycle`         | animation                  |         |           |            |
| `rich_palette_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `smooth`                 | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `stars`                  | animation                  |         |           |            |
| `sun_glow`               | animation                  |         |           |            |
| `sun_position`           | animation                  |         |           |            |
| `twinkle_animation`      | animation_constructor      |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## simple_palette.anim

**Status:** ✅ Success

## Symbol Table

| Symbol                   | Type                  | Builtin | Dangerous | Takes Args |
|--------------------------|-----------------------|---------|-----------|------------|
| `blue`                   | color                 |    ✓    |           |            |
| `demo`                   | sequence              |         |           |            |
| `green`                  | color                 |    ✓    |           |            |
| `orange`                 | color                 |    ✓    |           |            |
| `rainbow_cycle`          | animation             |         |           |            |
| `rainbow`                | palette               |         |           |            |
| `red`                    | color                 |    ✓    |           |            |
| `rich_palette_animation` | animation_constructor |    ✓    |    ⚠️     |     ✓      |
| `yellow`                 | color                 |    ✓    |           |            |

### Compilation Output

```
SUCCESS
```

## demo_pattern_fire_opacity.anim

**Status:** ✅ Success

## Symbol Table

| Symbol                       | Type                       | Builtin | Dangerous | Takes Args |
|------------------------------|----------------------------|---------|-----------|------------|
| `background`                 | animation                  |         |           |            |
| `beacon_animation`           | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `closure_value`              | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `cosine_osc`                 | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `eye_mask`                   | animation                  |         |           |            |
| `eye_pos`                    | value_provider             |         |           |            |
| `fire_color`                 | color                      |         |           |            |
| `fire_colors`                | palette                    |         |           |            |
| `fire_pattern`               | animation                  |         |           |            |
| `palette_gradient_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `rich_palette`               | color_constructor          |    ✓    |    ⚠️     |     ✓      |
| `solid`                      | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `strip_len`                  | value_provider             |         |           |            |
| `strip_length`               | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `transparent`                | color                      |    ✓    |           |            |
| `white`                      | color                      |    ✓    |           |            |

### Compilation Output

```
SUCCESS
```

## neon_glow.anim

**Status:** ✅ Success

## Symbol Table

| Symbol                   | Type                       | Builtin | Dangerous | Takes Args |
|--------------------------|----------------------------|---------|-----------|------------|
| `LINEAR`                 | constant                   |    ✓    |           |            |
| `arc_sparkles`           | animation                  |         |           |            |
| `beacon_animation`       | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `neon_colors`            | palette                    |         |           |            |
| `neon_main`              | animation                  |         |           |            |
| `neon_surge`             | animation                  |         |           |            |
| `rich_palette_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `rich_palette`           | color_constructor          |    ✓    |    ⚠️     |     ✓      |
| `segment1`               | animation                  |         |           |            |
| `segment2`               | animation                  |         |           |            |
| `segment3`               | animation                  |         |           |            |
| `segment_pattern`        | color                      |         |           |            |
| `smooth`                 | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `solid`                  | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `square`                 | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `twinkle_animation`      | animation_constructor      |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## cylon_red_eye.anim

**Status:** ✅ Success

## Symbol Table

| Symbol             | Type                       | Builtin | Dangerous | Takes Args |
|--------------------|----------------------------|---------|-----------|------------|
| `beacon_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `closure_value`    | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `cosine_osc`       | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `red_eye`          | animation                  |         |           |            |
| `red`              | color                      |    ✓    |           |            |
| `strip_len`        | value_provider             |         |           |            |
| `strip_length`     | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## import_demo.anim

**Status:** ✅ Success

## Symbol Table

| Symbol           | Type                       | Builtin | Dangerous | Takes Args |
|------------------|----------------------------|---------|-----------|------------|
| `abs`            | math_function              |    ✓    |           |     ✓      |
| `blue`           | color                      |    ✓    |           |            |
| `breathing_blue` | animation                  |         |           |            |
| `closure_value`  | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `dynamic_green`  | animation                  |         |           |            |
| `green`          | color                      |    ✓    |           |            |
| `import_demo`    | sequence                   |         |           |            |
| `max`            | math_function              |    ✓    |           |     ✓      |
| `min`            | math_function              |    ✓    |           |     ✓      |
| `rand_demo`      | user_function              |    ✓    |           |     ✓      |
| `random_red`     | animation                  |         |           |            |
| `red`            | color                      |    ✓    |           |            |
| `solid`          | animation_constructor      |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## lava_lamp.anim

**Status:** ✅ Success

## Symbol Table

| Symbol                   | Type                       | Builtin | Dangerous | Takes Args |
|--------------------------|----------------------------|---------|-----------|------------|
| `SINE`                   | constant                   |    ✓    |           |            |
| `beacon_animation`       | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `blob1_pattern`          | color                      |         |           |            |
| `blob2_pattern`          | color                      |         |           |            |
| `blob3_pattern`          | color                      |         |           |            |
| `heat_shimmer`           | animation                  |         |           |            |
| `lava_base`              | animation                  |         |           |            |
| `lava_blob1`             | animation                  |         |           |            |
| `lava_blob2`             | animation                  |         |           |            |
| `lava_blob3`             | animation                  |         |           |            |
| `lava_colors`            | palette                    |         |           |            |
| `rich_palette_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `rich_palette`           | color_constructor          |    ✓    |    ⚠️     |     ✓      |
| `shimmer_pattern`        | color                      |         |           |            |
| `smooth`                 | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `twinkle_animation`      | animation_constructor      |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## police_lights.anim

**Status:** ✅ Success

## Symbol Table

| Symbol             | Type                       | Builtin | Dangerous | Takes Args |
|--------------------|----------------------------|---------|-----------|------------|
| `beacon_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `half_length`      | variable                   |         |           |            |
| `left_red`         | animation                  |         |           |            |
| `right_blue`       | animation                  |         |           |            |
| `solid`            | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `square`           | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `white_strobe`     | animation                  |         |           |            |

### Compilation Output

```
SUCCESS
```

## user_functions_demo.anim

**Status:** ✅ Success

## Symbol Table

| Symbol             | Type                       | Builtin | Dangerous | Takes Args |
|--------------------|----------------------------|---------|-----------|------------|
| `abs`              | math_function              |    ✓    |           |     ✓      |
| `blue`             | color                      |    ✓    |           |            |
| `closure_value`    | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `cyan`             | color                      |    ✓    |           |            |
| `max`              | math_function              |    ✓    |           |     ✓      |
| `min`              | math_function              |    ✓    |           |     ✓      |
| `orange`           | color                      |    ✓    |           |            |
| `purple`           | color                      |    ✓    |           |            |
| `rand_demo`        | user_function              |    ✓    |           |     ✓      |
| `random_base`      | animation                  |         |           |            |
| `random_bounded`   | animation                  |         |           |            |
| `random_complex`   | animation                  |         |           |            |
| `random_multi`     | animation                  |         |           |            |
| `random_variation` | animation                  |         |           |            |
| `round`            | math_function              |    ✓    |           |     ✓      |
| `solid`            | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `strip_len`        | value_provider             |         |           |            |
| `strip_length`     | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `white`            | color                      |    ✓    |           |            |

### Compilation Output

```
SUCCESS
```

## test_template_simple.anim

**Status:** ✅ Success

## Symbol Table

| Symbol         | Type     | Builtin | Dangerous | Takes Args |
|----------------|----------|---------|-----------|------------|
| `pulse_effect` | template |         |           |            |
| `red`          | color    |    ✓    |           |            |

### Compilation Output

```
SUCCESS
```

## test_compute_multiple.anim

**Status:** ✅ Success

## Symbol Table

| Symbol          | Type                       | Builtin | Dangerous | Takes Args |
|-----------------|----------------------------|---------|-----------|------------|
| `a`             | variable                   |         |           |            |
| `abs`           | math_function              |    ✓    |           |     ✓      |
| `b`             | variable                   |         |           |            |
| `closure_value` | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `strip_len`     | value_provider             |         |           |            |
| `strip_length`  | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## lightning_storm.anim

**Status:** ✅ Success

## Symbol Table

| Symbol                   | Type                       | Builtin | Dangerous | Takes Args |
|--------------------------|----------------------------|---------|-----------|------------|
| `SINE`                   | constant                   |    ✓    |           |            |
| `afterglow`              | animation                  |         |           |            |
| `beacon_animation`       | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `distant_flash`          | animation                  |         |           |            |
| `lightning_main`         | animation                  |         |           |            |
| `lightning_partial`      | animation                  |         |           |            |
| `rich_palette_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `solid`                  | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `square`                 | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `storm_bg`               | animation                  |         |           |            |
| `storm_colors`           | palette                    |         |           |            |
| `twinkle_animation`      | animation_constructor      |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## test_cylon_generic.anim

**Status:** ❌ Failed

### Compilation Output

```
dsl_compilation_error: Line 9: Transpilation failed: Line 9: Template body transpilation failed: Line 9: Unknown function or identifier 'abs2'. Make sure it's defined before use.
stack traceback:
	<unknown source>: in function `error`
	<unknown source>: in function `transpile`
	<unknown source>: in function `main`
```

## computed_values_demo.anim

**Status:** ✅ Success

## Symbol Table

| Symbol            | Type                       | Builtin | Dangerous | Takes Args |
|-------------------|----------------------------|---------|-----------|------------|
| `abs`             | math_function              |    ✓    |           |     ✓      |
| `base_speed`      | variable                   |         |           |            |
| `blue`            | color                      |    ✓    |           |            |
| `closure_value`   | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |
| `comet_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `red`             | color                      |    ✓    |           |            |
| `stream1`         | animation                  |         |           |            |
| `stream2`         | animation                  |         |           |            |
| `strip_len`       | value_provider             |         |           |            |
| `strip_length`    | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## breathing_colors.anim

**Status:** ✅ Success

## Symbol Table

| Symbol              | Type                       | Builtin | Dangerous | Takes Args |
|---------------------|----------------------------|---------|-----------|------------|
| `breathe_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `breathe_blue`      | color                      |         |           |            |
| `breathe_green`     | color                      |         |           |            |
| `breathe_orange`    | color                      |         |           |            |
| `breathe_palette`   | palette                    |         |           |            |
| `breathe_purple`    | color                      |         |           |            |
| `breathe_red`       | color                      |         |           |            |
| `breathing`         | animation                  |         |           |            |
| `palette_pattern`   | color                      |         |           |            |
| `rich_palette`      | color_constructor          |    ✓    |    ⚠️     |     ✓      |
| `smooth`            | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## test_template_simple_reusable.anim

**Status:** ✅ Success

## Symbol Table

| Symbol         | Type     | Builtin | Dangerous | Takes Args |
|----------------|----------|---------|-----------|------------|
| `pulse_effect` | template |         |           |            |
| `red`          | color    |    ✓    |           |            |

### Compilation Output

```
SUCCESS
```

## palette_showcase.anim

**Status:** ✅ Success

## Symbol Table

| Symbol                   | Type                  | Builtin | Dangerous | Takes Args |
|--------------------------|-----------------------|---------|-----------|------------|
| `SINE`                   | constant              |    ✓    |           |            |
| `aurora_borealis`        | palette               |         |           |            |
| `aurora_lights`          | animation             |         |           |            |
| `black`                  | color                 |    ✓    |           |            |
| `blue`                   | color                 |    ✓    |           |            |
| `cyan`                   | color                 |    ✓    |           |            |
| `fire_effect`            | animation             |         |           |            |
| `fire_gradient`          | palette               |         |           |            |
| `navy`                   | color                 |    ✓    |           |            |
| `ocean_depths`           | palette               |         |           |            |
| `ocean_waves`            | animation             |         |           |            |
| `orange`                 | color                 |    ✓    |           |            |
| `palette_showcase`       | sequence              |         |           |            |
| `purple`                 | color                 |    ✓    |           |            |
| `rich_palette_animation` | animation_constructor |    ✓    |    ⚠️     |     ✓      |
| `rich_palette`           | color_constructor     |    ✓    |    ⚠️     |     ✓      |
| `solid`                  | animation_constructor |    ✓    |    ⚠️     |     ✓      |
| `sunset_glow`            | animation             |         |           |            |
| `sunset_sky`             | palette               |         |           |            |
| `white`                  | color                 |    ✓    |           |            |
| `yellow`                 | color                 |    ✓    |           |            |

### Compilation Output

```
SUCCESS
```

## rainbow_cycle.anim

**Status:** ✅ Success

## Symbol Table

| Symbol              | Type                  | Builtin | Dangerous | Takes Args |
|---------------------|-----------------------|---------|-----------|------------|
| `color_cycle`       | color_constructor     |    ✓    |    ⚠️     |     ✓      |
| `rainbow_animation` | animation             |         |           |            |
| `rainbow_cycle`     | color                 |         |           |            |
| `rainbow_palette`   | palette               |         |           |            |
| `solid`             | animation_constructor |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## comet_chase.anim

**Status:** ✅ Success

## Symbol Table

| Symbol              | Type                  | Builtin | Dangerous | Takes Args |
|---------------------|-----------------------|---------|-----------|------------|
| `background`        | animation             |         |           |            |
| `comet_animation`   | animation_constructor |    ✓    |    ⚠️     |     ✓      |
| `comet_main`        | animation             |         |           |            |
| `comet_secondary`   | animation             |         |           |            |
| `comet_sparkles`    | animation             |         |           |            |
| `solid`             | animation_constructor |    ✓    |    ⚠️     |     ✓      |
| `space_blue`        | color                 |         |           |            |
| `twinkle_animation` | animation_constructor |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
SUCCESS
```

## Summary

- **Total files processed:** 47
- **Successfully compiled:** 42
- **Failed to compile:** 5

### Successful Files

- ✅ heartbeat_pulse.anim
- ✅ twinkle_stars.anim
- ✅ sequence_assignments_demo.anim
- ✅ fire_flicker.anim
- ✅ demo_shutter_rainbow_central.anim
- ✅ disco_strobe.anim
- ✅ test_simple_math.anim
- ✅ matrix_rain.anim
- ✅ demo_shutter_rainbow2.anim
- ✅ scanner_larson.anim
- ✅ test_complex_template.anim
- ✅ demo_shutter_rainbow_leftright.anim
- ✅ candy_cane.anim
- ✅ test_shutter_rainbow_bidir.anim
- ✅ property_assignment_demo.anim
- ✅ cylon_generic.anim
- ✅ cylon_rainbow.anim
- ✅ demo_shutter_rainbow_bidir.anim
- ✅ christmas_tree.anim
- ✅ plasma_wave.anim
- ✅ ocean_waves.anim
- ✅ swipe_rainbow.anim
- ✅ meteor_shower.anim
- ✅ palette_demo.anim
- ✅ sunrise_sunset.anim
- ✅ simple_palette.anim
- ✅ demo_pattern_fire_opacity.anim
- ✅ neon_glow.anim
- ✅ cylon_red_eye.anim
- ✅ import_demo.anim
- ✅ lava_lamp.anim
- ✅ police_lights.anim
- ✅ user_functions_demo.anim
- ✅ test_template_simple.anim
- ✅ test_compute_multiple.anim
- ✅ lightning_storm.anim
- ✅ computed_values_demo.anim
- ✅ breathing_colors.anim
- ✅ test_template_simple_reusable.anim
- ✅ palette_showcase.anim
- ✅ rainbow_cycle.anim
- ✅ comet_chase.anim

### Failed Files

- ❌ fail_color_predefined.anim
- ❌ fail_value_provider_add.anim
- ❌ fail_name_predefined.anim
- ❌ test_shutter_rainbow_central.anim
- ❌ test_cylon_generic.anim

---

