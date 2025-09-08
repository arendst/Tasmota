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
> process_value context=2 result=ExpressionResult(expr='0xFF110000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='heart_bg_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFF0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='150', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='20', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.square(engine)
  provider.min_value = 0
  provider.max_value = 255
  provider.duration = 150
  provider.duty_cycle = 20
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFCC0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='180', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='150', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.square(engine)
  provider.min_value = 0
  provider.max_value = 180
  provider.duration = 150
  provider.duty_cycle = 15
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=5 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFF660000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='30', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='100', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 30
  provider.max_value = 100
  provider.duration = 1000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=5 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='30', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='20', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='100', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.square(engine)
  provider.min_value = 0
  provider.max_value = 200
  provider.duration = 100
  provider.duty_cycle = 10
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
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
> process_value context=2 result=ExpressionResult(expr='0xFF000033', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='night_sky_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='500', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFAA', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='300', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_ - 2', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len_ - 2'
=== transform_expression_for_closure 'animation.resolve(strip_len_) - 2'
> process_value context=1 result=ExpressionResult(expr='5000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.triangle(engine)
  provider.min_value = 0
  provider.max_value = animation.create_closure_value(engine, def (engine) return animation.resolve(strip_len_) - 2 end)
  provider.duration = 5000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_ - 2', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len_ - 2'
=== transform_expression_for_closure 'animation.resolve(strip_len_) - 2'
> process_value context=1 result=ExpressionResult(expr='5000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.cosine_osc(engine)
  provider.min_value = 0
  provider.max_value = animation.create_closure_value(engine, def (engine) return animation.resolve(strip_len_) - 2 end)
  provider.duration = 5000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='64', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='eye_palette_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='eye_color_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='cosine_val_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFF0000FF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='triangle_val_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=5 result=ExpressionResult(expr='cosine_val_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='brightness_low_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='brightness_high_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='0xFFFF0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=5 result=ExpressionResult(expr='brightness_low_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='0xFF008000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=5 result=ExpressionResult(expr='brightness_high_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='0xFF0000FF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=6 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='triangle_val_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=5 result=ExpressionResult(expr='cosine_val_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='triangle_val_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=5 result=ExpressionResult(expr='cosine_val_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='brightness_low_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='brightness_high_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='0xFFFF0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=5 result=ExpressionResult(expr='brightness_low_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='0xFF008000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=5 result=ExpressionResult(expr='brightness_high_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='fire_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.LINEAR', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='fire_base_color_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='180', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='800', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 180
  provider.max_value = 255
  provider.duration = 800
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='fire_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.LINEAR', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='flicker_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='(strip_len_ + 1) / 2', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure '(strip_len_ + 1) / 2'
=== transform_expression_for_closure '(animation.resolve(strip_len_) + 1) / 2'
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=1 result=ExpressionResult(expr='duration_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 0
  provider.max_value = strip_len_
  provider.duration = duration_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='colors_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='colors_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col2_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col1_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len2_ - (shutter_size_ + 1) / 2', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len2_ - (shutter_size_ + 1) / 2'
=== transform_expression_for_closure 'animation.resolve(strip_len2_) - (animation.resolve(shutter_size_) + 1) / 2'
> process_value context=1 result=ExpressionResult(expr='shutter_size_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col1_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col2_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len2_ - (strip_len_ - shutter_size_ + 1) / 2', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len2_ - (strip_len_ - shutter_size_ + 1) / 2'
=== transform_expression_for_closure 'animation.resolve(strip_len2_) - (animation.resolve(strip_len_) - animation.resolve(shutter_size_) + 1) / 2'
> process_value context=1 result=ExpressionResult(expr='strip_len_ - shutter_size_', funcs=false, vars=true, props=false, comp=true, type=value_provider, instance=nil)
+++ transform_expression_for_closure 'strip_len_ - shutter_size_'
=== transform_expression_for_closure 'animation.resolve(strip_len_) - animation.resolve(shutter_size_)'
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=6 result=ExpressionResult(expr='col1_.palette_size', funcs=false, vars=false, props=true, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=6 result=ExpressionResult(expr='col1_.palette_size', funcs=false, vars=false, props=true, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='rainbow_with_white_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=4 result=ExpressionResult(expr='1500', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='disco_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.LINEAR', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='disco_rich_color_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='100', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='30', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.square(engine)
  provider.min_value = 0
  provider.max_value = 255
  provider.duration = 100
  provider.duty_cycle = 30
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='50', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.square(engine)
  provider.min_value = 0
  provider.max_value = 255
  provider.duration = 50
  provider.duty_cycle = 10
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=5 result=ExpressionResult(expr='20', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='disco_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='500', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.LINEAR', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='sparkle_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='80', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='disco_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='800', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.LINEAR', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='pulse_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='56', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 4
  provider.max_value = 56
  provider.duration = 2000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
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
> process_value context=1 result=ExpressionResult(expr='2000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='x_ + 4', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'x_ + 4'
=== transform_expression_for_closure 'animation.resolve(x_) + 4'
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
> process_value context=2 result=ExpressionResult(expr='0xFF000000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='matrix_bg_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='50', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='matrix_greens_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.LINEAR', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='stream1_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1500', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='matrix_greens_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1800', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.LINEAR', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='stream2_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='matrix_greens_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2500', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.LINEAR', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='180', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='stream3_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1800', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='6', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFF00FFAA', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='150', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='20', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
SUCCESS
```

## demo_shutter_rainbow2.anim

**Status:** ✅ Success

## Symbol Table

| Symbol              | Type                       | Builtin | Dangerous | Takes Args |
|---------------------|----------------------------|---------|-----------|------------|
| `PALETTE_RAINBOW`   | palette_constant           |    ✓    |           |            |
| `beacon_animation`  | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
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
> process_value context=1 result=ExpressionResult(expr='3000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=1 result=ExpressionResult(expr='duration_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 0
  provider.max_value = strip_len_
  provider.duration = duration_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='animation.PALETTE_RAINBOW', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.PALETTE_RAINBOW', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col1_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col2_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='shutter_size_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
SUCCESS
```

## scanner_larson.anim

**Status:** ✅ Success

## Symbol Table

| Symbol             | Type                       | Builtin | Dangerous | Takes Args |
|--------------------|----------------------------|---------|-----------|------------|
| `background`       | animation                  |         |           |            |
| `beacon_animation` | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `pos_test`         | value_provider             |         |           |            |
| `scanner_bg`       | color                      |         |           |            |
| `scanner_trail`    | animation                  |         |           |            |
| `scanner`          | animation                  |         |           |            |
| `solid`            | animation_constructor      |    ✓    |    ⚠️     |     ✓      |
| `triangle`         | value_provider_constructor |    ✓    |    ⚠️     |     ✓      |

### Compilation Output

```
> process_value context=2 result=ExpressionResult(expr='0xFF110000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='scanner_bg_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFF0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='57', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.triangle(engine)
  provider.min_value = 2
  provider.max_value = 57
  provider.duration = 2000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0xFF660000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='6', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='57', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.triangle(engine)
  provider.min_value = 2
  provider.max_value = 57
  provider.duration = 2000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=5 result=ExpressionResult(expr='pos_test_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=5 result=ExpressionResult(expr='128', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='pal1_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='duration_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='cycle_color_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='duration_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='back_color_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='fire_palette_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=4 result=ExpressionResult(expr='ocean_palette_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=4 result=ExpressionResult(expr='3000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='0xFF001100', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=1 result=ExpressionResult(expr='duration_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 0
  provider.max_value = strip_len_
  provider.duration = duration_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='colors_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='colors_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col2_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col1_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='shutter_size_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='rainbow_with_white_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=4 result=ExpressionResult(expr='1500', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=2 result=ExpressionResult(expr='0xFFFF0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=2 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='candy_red_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='candy_white_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='9', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='candy_red_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='candy_white_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='21', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='candy_red_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='27', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='candy_white_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='33', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='candy_red_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='39', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='candy_white_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='45', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='candy_red_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='51', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='candy_white_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='57', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='63', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='move_speed_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 3
  provider.max_value = 63
  provider.duration = move_speed_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='9', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='69', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='move_speed_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 9
  provider.max_value = 69
  provider.duration = move_speed_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='75', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='move_speed_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 15
  provider.max_value = 75
  provider.duration = move_speed_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='21', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='81', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='move_speed_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 21
  provider.max_value = 81
  provider.duration = move_speed_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='27', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='87', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='move_speed_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 27
  provider.max_value = 87
  provider.duration = move_speed_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='33', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='93', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='move_speed_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 33
  provider.max_value = 93
  provider.duration = move_speed_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='39', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='99', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='move_speed_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 39
  provider.max_value = 99
  provider.duration = move_speed_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='45', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='105', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='move_speed_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 45
  provider.max_value = 105
  provider.duration = move_speed_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='51', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='111', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='move_speed_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 51
  provider.max_value = 111
  provider.duration = move_speed_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='57', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='117', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='move_speed_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 57
  provider.max_value = 117
  provider.duration = move_speed_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
SUCCESS
```

## fail_color_predefined.anim

**Status:** ❌ Failed

### Compilation Output

```
DSL compilation failed
Errors:
  Line 4: Cannot redefine predefined color 'red'. Use a different name like 'red_custom' or 'my_red'
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
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_ + 0', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len_ + 0'
=== transform_expression_for_closure 'animation.resolve(strip_len_) + 0'
> process_value context=1 result=ExpressionResult(expr='duration_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 0
  provider.max_value = animation.create_closure_value(engine, def (engine) return animation.resolve(strip_len_) + 0 end)
  provider.duration = duration_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='colors_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='colors_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col2_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col1_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='shutter_size_ + 0', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'shutter_size_ + 0'
=== transform_expression_for_closure 'animation.resolve(shutter_size_) + 0'
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col1_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col2_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_ - shutter_size_', funcs=false, vars=true, props=false, comp=true, type=value_provider, instance=nil)
+++ transform_expression_for_closure 'strip_len_ - shutter_size_'
=== transform_expression_for_closure 'animation.resolve(strip_len_) - animation.resolve(shutter_size_)'
> process_value context=1 result=ExpressionResult(expr='0 + 0', funcs=false, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=6 result=ExpressionResult(expr='col1_.palette_size', funcs=false, vars=false, props=true, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=6 result=ExpressionResult(expr='col1_.palette_size', funcs=false, vars=false, props=true, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='rainbow_with_white_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=4 result=ExpressionResult(expr='1500', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=2 result=ExpressionResult(expr='0xFFFF0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=2 result=ExpressionResult(expr='0xFF0000FF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=2 result=ExpressionResult(expr='0xFF00FF00', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='red_custom_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='blue_custom_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='30', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='green_custom_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='45', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='150', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='eye_color_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='back_color_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(-1)', funcs=false, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_ - 2', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len_ - 2'
=== transform_expression_for_closure 'animation.resolve(strip_len_) - 2'
> process_value context=1 result=ExpressionResult(expr='duration_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.cosine_osc(engine)
  provider.min_value = (-1)
  provider.max_value = animation.create_closure_value(engine, def (engine) return animation.resolve(strip_len_) - 2 end)
  provider.duration = duration_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='0xFFFF0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=4 result=ExpressionResult(expr='0x00000000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=4 result=ExpressionResult(expr='3000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='5000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='eye_palette_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_ - 2', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len_ - 2'
=== transform_expression_for_closure 'animation.resolve(strip_len_) - 2'
> process_value context=1 result=ExpressionResult(expr='eye_duration_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.cosine_osc(engine)
  provider.min_value = 0
  provider.max_value = animation.create_closure_value(engine, def (engine) return animation.resolve(strip_len_) - 2 end)
  provider.duration = eye_duration_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_ - 2', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len_ - 2'
=== transform_expression_for_closure 'animation.resolve(strip_len_) - 2'
> process_value context=1 result=ExpressionResult(expr='eye_duration_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.triangle(engine)
  provider.min_value = 0
  provider.max_value = animation.create_closure_value(engine, def (engine) return animation.resolve(strip_len_) - 2 end)
  provider.duration = eye_duration_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='eye_color_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='cosine_val_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='triangle_val_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=5 result=ExpressionResult(expr='cosine_val_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=1 result=ExpressionResult(expr='duration_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 0
  provider.max_value = strip_len_
  provider.duration = duration_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='colors_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='colors_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col2_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col1_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='shutter_size_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col1_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col2_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_ - shutter_size_', funcs=false, vars=true, props=false, comp=true, type=value_provider, instance=nil)
+++ transform_expression_for_closure 'strip_len_ - shutter_size_'
=== transform_expression_for_closure 'animation.resolve(strip_len_) - animation.resolve(shutter_size_)'
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=6 result=ExpressionResult(expr='col1_.palette_size', funcs=false, vars=false, props=true, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=6 result=ExpressionResult(expr='col1_.palette_size', funcs=false, vars=false, props=true, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='rainbow_with_white_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=4 result=ExpressionResult(expr='1500', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=2 result=ExpressionResult(expr='0xFF006600', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='tree_green_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='ornament_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.LINEAR', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='ornament_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='800', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFF00', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='58', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='20', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 200
  provider.max_value = 255
  provider.duration = 2000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='400', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='ornament_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.LINEAR', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='garland_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='6', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='plasma_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='6000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='plasma_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='wave1_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='20', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='40', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 0
  provider.max_value = 40
  provider.duration = 8000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='plasma_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='180', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='wave2_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='45', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='45', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='10000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 45
  provider.max_value = 15
  provider.duration = 10000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='plasma_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='220', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='wave3_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='20', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='6', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='12', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='20', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='50', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='6000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 20
  provider.max_value = 50
  provider.duration = 6000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='150', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 150
  provider.max_value = 255
  provider.duration = 12000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
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
> process_value context=1 result=ExpressionResult(expr='ocean_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='ocean_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='6000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='wave1_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='6', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='48', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 0
  provider.max_value = 48
  provider.duration = 5000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='ocean_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='180', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='wave2_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='52', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='52', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='7000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 52
  provider.max_value = 8
  provider.duration = 7000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='6', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='300', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='palette_olivary_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='olivary_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=2 result=ExpressionResult(expr='0xFF000011', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='space_bg_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1500', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFFAA00', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='12', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFAAAAFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1800', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFFAAAA', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='14', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2500', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFCCCCCC', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='100', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='25', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='fire_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='ocean_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.PALETTE_FOREST', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=6 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
SUCCESS
```

## fail_value_provider_add.anim

**Status:** ❌ Failed

### Compilation Output

```
> process_value context=1 result=ExpressionResult(expr='animation.linear(engine) + nil', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation.linear(engine) + nil'
=== transform_expression_for_closure 'animation.linear(engine) + nil'
> process_value context=1 result=ExpressionResult(expr='animation.triangle(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='animation.linear(engine) + nil', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation.linear(engine) + nil'
=== transform_expression_for_closure 'animation.linear(engine) + nil'
DSL compilation failed
Errors:
  Line 4: Function 'triangle' cannot be used in computed expressions. This creates a new instance at each evaluation. Use either:
  set var_name = triangle()  # Single function call
  set computed = (existing_var + 1) / 2  # Computation with existing values
  Line 6: Function 'triangle' cannot be used in computed expressions. This creates a new instance at each evaluation. Use either:
  set var_name = triangle()  # Single function call
  set computed = (existing_var + 1) / 2  # Computation with existing values
```

## fail_name_predefined.anim

**Status:** ❌ Failed

### Compilation Output

```
DSL compilation failed
Errors:
  Line 4: Cannot redefine built-in symbol 'abs' (type: 4). Use a different name like 'abs_custom' or 'my_abs'
```

## test_shutter_rainbow_central.anim

**Status:** ❌ Failed

### Compilation Output

```
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='(strip_len_ + 1) / 2', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure '(strip_len_ + 1) / 2'
=== transform_expression_for_closure '(animation.resolve(strip_len_) + 1) / 2'
> process_value context=1 result=ExpressionResult(expr='1 + nil', funcs=false, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(nil)', funcs=false, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=1 result=ExpressionResult(expr='duration_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.sawtooth(engine)
  provider.min_value = 0
  provider.max_value = strip_len_
  provider.duration = duration_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='colors_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='colors_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col2_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='col1_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len3_ - shutter_size_ / 2', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len3_ - shutter_size_ / 2'
=== transform_expression_for_closure 'animation.resolve(strip_len3_) - animation.resolve(shutter_size_) / 2'
> process_value context=1 result=ExpressionResult(expr='shutter_size_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='rainbow_with_white_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=4 result=ExpressionResult(expr='1500', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
DSL compilation failed
Errors:
  Line 38: Template 'shutter_central' body error: Line 12: Function 'strip_length' cannot be used in computed expressions. This creates a new instance at each evaluation. Use either:
  set var_name = strip_length()  # Single function call
  set computed = (existing_var + 1) / 2  # Computation with existing values
  Line 38: Template 'shutter_central' body error: Line 13: Function 'strip_length' cannot be used in computed expressions. This creates a new instance at each evaluation. Use either:
  set var_name = strip_length()  # Single function call
  set computed = (existing_var + 1) / 2  # Computation with existing values
  Line 38: Template 'shutter_central' body error: Line 13: Expected ')'
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
> process_value context=1 result=ExpressionResult(expr='daylight_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='60000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFAA', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='55', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='30000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 5
  provider.max_value = 55
  provider.duration = 30000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='30000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 0
  provider.max_value = 255
  provider.duration = 30000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0xFFFFCC88', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='16', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='55', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='30000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 5
  provider.max_value = 55
  provider.duration = 30000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='150', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='30000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 0
  provider.max_value = 150
  provider.duration = 30000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='6', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='30000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 255
  provider.max_value = 0
  provider.duration = 30000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
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
> process_value context=1 result=ExpressionResult(expr='rainbow_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='fire_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFF000088', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='20', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(-1)', funcs=false, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_ - 2', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len_ - 2'
=== transform_expression_for_closure 'animation.resolve(strip_len_) - 2'
> process_value context=1 result=ExpressionResult(expr='6000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.cosine_osc(engine)
  provider.min_value = (-1)
  provider.max_value = animation.create_closure_value(engine, def (engine) return animation.resolve(strip_len_) - 2 end)
  provider.duration = 6000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0x00000000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='eye_pos_', funcs=false, vars=true, props=false, comp=false, type=value_provider, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='fire_color_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_ / 4', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len_ / 4'
=== transform_expression_for_closure 'animation.resolve(strip_len_) / 4'
> process_value context=1 result=ExpressionResult(expr='eye_mask_', funcs=false, vars=true, props=false, comp=false, type=animation, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='neon_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.LINEAR', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='220', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 220
  provider.max_value = 255
  provider.duration = 200
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='50', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.square(engine)
  provider.min_value = 0
  provider.max_value = 255
  provider.duration = 50
  provider.duty_cycle = 2
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=5 result=ExpressionResult(expr='20', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='neon_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.LINEAR', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='segment_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='6', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='segment_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='24', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='segment_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='42', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFAAAAFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='100', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='0xFFFF0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_ - 2', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len_ - 2'
=== transform_expression_for_closure 'animation.resolve(strip_len_) - 2'
> process_value context=1 result=ExpressionResult(expr='5000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.cosine_osc(engine)
  provider.min_value = 0
  provider.max_value = animation.create_closure_value(engine, def (engine) return animation.resolve(strip_len_) - 2 end)
  provider.duration = 5000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='0xFFFF0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=5 result=ExpressionResult(expr='animation.get_user_function('rand_demo')(engine)', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation.get_user_function('rand_demo')(engine)'
=== transform_expression_for_closure 'animation.get_user_function('rand_demo')(engine)'
> process_value context=1 result=ExpressionResult(expr='0xFF0000FF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=4 result=ExpressionResult(expr='50', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='animation.get_user_function('rand_demo')(engine) + 100', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='animation._math.min(255, animation.get_user_function('rand_demo')(engine) + 100)', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='animation._math.max(50, animation._math.min(255, animation.get_user_function('rand_demo')(engine) + 100))', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation._math.max(50, animation._math.min(255, animation.get_user_function('rand_demo')(engine) + 100))'
=== transform_expression_for_closure 'animation._math.max(50, animation._math.min(255, animation.get_user_function('rand_demo')(engine) + 100))'
> process_value context=1 result=ExpressionResult(expr='0xFF008000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=4 result=ExpressionResult(expr='animation.get_user_function('rand_demo')(engine) - 128', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='animation._math.abs(animation.get_user_function('rand_demo')(engine) - 128) + 64', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation._math.abs(animation.get_user_function('rand_demo')(engine) - 128) + 64'
=== transform_expression_for_closure 'animation._math.abs(animation.get_user_function('rand_demo')(engine) - 128) + 64'
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
> process_value context=1 result=ExpressionResult(expr='lava_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='180', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='lava_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='blob1_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='9', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='18', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='9', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='51', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='20000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 9
  provider.max_value = 51
  provider.duration = 20000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='lava_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='10000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='220', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='blob2_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='46', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='14', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='46', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='14', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='25000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 46
  provider.max_value = 14
  provider.duration = 25000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='lava_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='blob3_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='25', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='6', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='25', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='35', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='18000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 25
  provider.max_value = 35
  provider.duration = 18000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='lava_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='6000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='shimmer_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='6', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='1500', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='30', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFF0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='400', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='50', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.square(engine)
  provider.min_value = 0
  provider.max_value = 255
  provider.duration = 400
  provider.duty_cycle = 50
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0xFF0000FF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='45', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='400', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='50', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.square(engine)
  provider.min_value = 255
  provider.max_value = 0
  provider.duration = 400
  provider.duty_cycle = 50
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='100', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.square(engine)
  provider.min_value = 0
  provider.max_value = 255
  provider.duration = 100
  provider.duty_cycle = 5
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=5 result=ExpressionResult(expr='20', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='0xFF0000FF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='animation.get_user_function('rand_demo')(engine)', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation.get_user_function('rand_demo')(engine)'
=== transform_expression_for_closure 'animation.get_user_function('rand_demo')(engine)'
> process_value context=1 result=ExpressionResult(expr='0xFFFFA500', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='50', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='animation.get_user_function('rand_demo')(engine) + 100', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='animation._math.min(255, animation.get_user_function('rand_demo')(engine) + 100)', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='animation._math.max(50, animation._math.min(255, animation.get_user_function('rand_demo')(engine) + 100))', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation._math.max(50, animation._math.min(255, animation.get_user_function('rand_demo')(engine) + 100))'
=== transform_expression_for_closure 'animation._math.max(50, animation._math.min(255, animation.get_user_function('rand_demo')(engine) + 100))'
> process_value context=1 result=ExpressionResult(expr='0xFF800080', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='animation.get_user_function('rand_demo')(engine) - 128', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='animation._math.abs(animation.get_user_function('rand_demo')(engine) - 128) + 64', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation._math.abs(animation.get_user_function('rand_demo')(engine) - 128) + 64'
=== transform_expression_for_closure 'animation._math.abs(animation.get_user_function('rand_demo')(engine) - 128) + 64'
> process_value context=1 result=ExpressionResult(expr='0xFF00FFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='100', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='animation.get_user_function('rand_demo')(engine)', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='animation._math.max(100, animation.get_user_function('rand_demo')(engine))', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation._math.max(100, animation.get_user_function('rand_demo')(engine))'
=== transform_expression_for_closure 'animation._math.max(100, animation.get_user_function('rand_demo')(engine))'
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='20', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='animation.get_user_function('rand_demo')(engine) - 100', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='(animation.get_user_function('rand_demo')(engine) + 128) / 2 + animation._math.abs(animation.get_user_function('rand_demo')(engine) - 100)', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='animation._math.round((animation.get_user_function('rand_demo')(engine) + 128) / 2 + animation._math.abs(animation.get_user_function('rand_demo')(engine) - 100))', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation._math.round((animation.get_user_function('rand_demo')(engine) + 128) / 2 + animation._math.abs(animation.get_user_function('rand_demo')(engine) - 100))'
=== transform_expression_for_closure 'animation._math.round((animation.get_user_function('rand_demo')(engine) + 128) / 2 + animation._math.abs(animation.get_user_function('rand_demo')(engine) - 100))'
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
> process_value context=1 result=ExpressionResult(expr='base_color_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='duration_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='brightness_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='0xFFFF0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=4 result=ExpressionResult(expr='2000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='204', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='strip_len_ / 4', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len_ / 4'
=== transform_expression_for_closure 'animation.resolve(strip_len_) / 4'
> process_value context=4 result=ExpressionResult(expr='strip_len_ / 4', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation._math.abs(strip_len_ / 4)', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation._math.abs(strip_len_ / 4)'
=== transform_expression_for_closure 'animation._math.abs(animation.resolve(strip_len_) / 4)'
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
> process_value context=1 result=ExpressionResult(expr='storm_colors_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='100', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='80', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.square(engine)
  provider.min_value = 0
  provider.max_value = 255
  provider.duration = 80
  provider.duty_cycle = 3
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=5 result=ExpressionResult(expr='20', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFAA', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='30', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='20', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='15', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='120', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.square(engine)
  provider.min_value = 0
  provider.max_value = 200
  provider.duration = 120
  provider.duty_cycle = 4
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='0xFF4444FF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='80', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.square(engine)
  provider.min_value = 0
  provider.max_value = 80
  provider.duration = 200
  provider.duty_cycle = 8
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=5 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFF666699', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='300', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
SUCCESS
```

## test_cylon_generic.anim

**Status:** ❌ Failed

### Compilation Output

```
> process_value context=1 result=ExpressionResult(expr='nil', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=4 result=ExpressionResult(expr='strip_len_ / 2', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation._math.abs(strip_len_ / 2)', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation._math.abs(strip_len_ / 2)'
=== transform_expression_for_closure 'animation._math.abs(animation.resolve(strip_len_) / 2)'
> process_value context=1 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.strip_length(engine)
  provider.is_running = 0
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='animation.sawtooth(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='animation.triangle(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=4 result=ExpressionResult(expr='osc1_ + osc2_', funcs=false, vars=true, props=false, comp=true, type=value_provider, instance=nil)
> process_value context=4 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation._math.scale(osc1_ + osc2_, 0, 200, 0, 255)', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation._math.scale(osc1_ + osc2_, 0, 200, 0, 255)'
=== transform_expression_for_closure 'animation._math.scale(animation.resolve(osc1_) + animation.resolve(osc2_), 0, 200, 0, 255)'
> process_value context=1 result=ExpressionResult(expr='animation.sawtooth(engine) + nil', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation.sawtooth(engine) + nil'
=== transform_expression_for_closure 'animation.sawtooth(engine) + nil'
> process_value context=1 result=ExpressionResult(expr='eye_color_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='back_color_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(-1)', funcs=false, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_ - 2', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len_ - 2'
=== transform_expression_for_closure 'animation.resolve(strip_len_) - 2'
> process_value context=1 result=ExpressionResult(expr='duration_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.cosine_osc(engine)
  provider.min_value = (-1)
  provider.max_value = animation.create_closure_value(engine, def (engine) return animation.resolve(strip_len_) - 2 end)
  provider.duration = duration_
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
> process_value context=1 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
DSL compilation failed
Errors:
  Line 29: Template 'cylon_effect' body error: Line 9: Unknown function or identifier 'abs2'. Make sure it's defined before use.
  Line 29: Template 'cylon_effect' body error: Line 17: Function 'triangle' cannot be used in computed expressions. This creates a new instance at each evaluation. Use either:
  set var_name = triangle()  # Single function call
  set computed = (existing_var + 1) / 2  # Computation with existing values
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
> process_value context=1 result=ExpressionResult(expr='animation.strip_length(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=StripLengthProvider)
> process_value context=1 result=ExpressionResult(expr='0xFFFF0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=4 result=ExpressionResult(expr='strip_len_ / 4', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation._math.abs(strip_len_ / 4)', funcs=true, vars=false, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'animation._math.abs(strip_len_ / 4)'
=== transform_expression_for_closure 'animation._math.abs(animation.resolve(strip_len_) / 4)'
> process_value context=1 result=ExpressionResult(expr='1.5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2.0', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFF0000FF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='strip_len_ / 8 + (2 * strip_len_) - 10', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len_ / 8 + (2 * strip_len_) - 10'
=== transform_expression_for_closure 'animation.resolve(strip_len_) / 8 + (2 * animation.resolve(strip_len_)) - 10'
> process_value context=1 result=ExpressionResult(expr='base_speed_ * 1.5', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'base_speed_ * 1.5'
=== transform_expression_for_closure 'animation.resolve(base_speed_) * 1.5'
> process_value context=1 result=ExpressionResult(expr='(-1)', funcs=false, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='strip_len_ / 5', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len_ / 5'
=== transform_expression_for_closure 'animation.resolve(strip_len_) / 5'
> process_value context=5 result=ExpressionResult(expr='strip_len_ * 4', funcs=false, vars=true, props=false, comp=true, type=variable, instance=nil)
+++ transform_expression_for_closure 'strip_len_ * 4'
=== transform_expression_for_closure 'animation.resolve(strip_len_) * 4'
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
> process_value context=2 result=ExpressionResult(expr='0xFFFF0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=2 result=ExpressionResult(expr='0xFF00FF00', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=2 result=ExpressionResult(expr='0xFF0000FF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=2 result=ExpressionResult(expr='0xFF800080', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=2 result=ExpressionResult(expr='0xFFFF8000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='breathe_palette_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='15000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='palette_pattern_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='100', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='100', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='255', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='4000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='(def (engine)
  var provider = animation.smooth(engine)
  provider.min_value = 100
  provider.max_value = 255
  provider.duration = 4000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=value_provider, instance=OscillatorValueProvider)
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
> process_value context=1 result=ExpressionResult(expr='base_color_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='duration_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=5 result=ExpressionResult(expr='brightness_', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='0xFFFF0000', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=4 result=ExpressionResult(expr='2000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=4 result=ExpressionResult(expr='204', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='fire_gradient_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(def (engine)
  var provider = animation.rich_palette(engine)
  provider.palette = fire_gradient_
  provider.cycle_period = 3000
  return provider
end)(engine)', funcs=true, vars=false, props=false, comp=false, type=color, instance=RichPaletteColorProvider)
> process_value context=1 result=ExpressionResult(expr='ocean_depths_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='200', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='aurora_borealis_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='12000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='180', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='sunset_sky_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='6000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='animation.SINE', funcs=false, vars=true, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='220', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=6 result=ExpressionResult(expr='3', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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
> process_value context=1 result=ExpressionResult(expr='rainbow_palette_', funcs=false, vars=true, props=false, comp=false, type=palette, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='rainbow_cycle_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
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
> process_value context=2 result=ExpressionResult(expr='0xFF000066', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='space_blue_', funcs=false, vars=true, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFFFFFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='10', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='2000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='7', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFFF4500', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='3000', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='(-1)', funcs=false, vars=false, props=false, comp=true, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='5', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='0xFFAAAAFF', funcs=false, vars=false, props=false, comp=false, type=color, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='400', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
> process_value context=1 result=ExpressionResult(expr='8', funcs=false, vars=false, props=false, comp=false, type=variable, instance=nil)
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

