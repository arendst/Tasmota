# LVGL + Berry

This repo contains a full binding of LVGL to the Berry language. This project is similar to the Micropython+lvgl project in its spirit, and provides easy to use scripting for lvgl on embedded devices. It is used by the Tasmota open-source project for ESP32 based devices. 

**Supported version**: LVGL 8.2.0

## Why use LVGL + Berry

LVGL + Berry mapping provides an easy-to-use programming language on top of LVGL powerful APIs. This is ideal for rapid scripting as well as production applications. The Berry mapping comes from the open-source [Tasmota](https://tasmota.github.io/docs/) project for ESP32 based devices, and leverages a vivid ecosystem.

![lvgl_info](https://user-images.githubusercontent.com/49731213/153846806-2571f872-f85f-4f31-becb-b6bebc10a324.png)

Quickstart with this short tutorial [LVGL in less than 10 minutes with Tasmota](https://tasmota.github.io/docs/LVGL_in_10_minutes/).

[Berry](https://github.com/berry-lang/berry) is an ultra-lightweight dynamically typed embedded scripting language. It is designed for lower-performance embedded devices. The interpreter of Berry includes a one-pass compiler and register-based VM, all the code is written in ANSI C99. Berry offers a syntax very similar to Python, and is inspired from LUA VM.


## Quickstart

Let's start with a simple example:

```
#- start LVGL and init environment (can be called multiple times) -#
lv.start()

hres = lv.get_hor_res()       # horizontal resolution of default display
vres = lv.get_ver_res()       # vertical resolution of default display

scr = lv.scr_act()            # default screen object

#- Background with a gradient from black #000000 (bottom) to dark blue #0000A0 (top) -#
scr.set_style_bg_color(lv.color(0x0000A0), lv.PART_MAIN | lv.STATE_DEFAULT)
scr.set_style_bg_grad_color(lv.color(0x000000), lv.PART_MAIN | lv.STATE_DEFAULT)
scr.set_style_bg_grad_dir(lv.GRAD_DIR_VER, lv.PART_MAIN | lv.STATE_DEFAULT)
```

Let's add a label:

```
t = lv.label(scr)
t.set_style_text_color(lv.color(0xFFFFFF), lv.PART_MAIN | lv.STATE_DEFAULT)
t.set_text("Hello LVGL + Berry!")
t.set_align(lv.ALIGN_TOP_MID)
```

Let's add a style for buttons:

```
#- create a style for the buttons -#
btn_style = lv.style()
btn_style.set_radius(10)                        # radius of rounded corners
btn_style.set_bg_opa(lv.OPA_COVER)              # 100% background opacity
btn_style.set_bg_color(lv.color(0x1fa3ec))      # background color #1FA3EC (Tasmota Blue)
btn_style.set_border_color(lv.color(0x0000FF))  # border color #0000FF
btn_style.set_text_color(lv.color(0xFFFFFF))    # text color white #FFFFFF
```

Let's add a single button with using a symbol:

```
home_btn = lv.btn(scr)                            # center button
home_btn.set_size(80, 30)
home_btn.add_style(btn_style, lv.PART_MAIN | lv.STATE_DEFAULT)
home_btn.set_align(lv.ALIGN_BOTTOM_MID)
home_label = lv.label(home_btn)
home_label.set_text(lv.SYMBOL_HOME)                 # set text as Home icon
home_label.center()
```


Let's now add a colorwheel widget:

```
colw = lv.colorwheel(scr, false)
colw.set_size(100, 100)
colw.set_pos(20,30)
```

![LVGL_Berry](https://user-images.githubusercontent.com/49731213/153846625-52855e95-b1a2-4a68-a495-1759f490794e.png)


As a general rule:

- calls to general APIs are mapped to module `lv`. E.g. `lv_get_hor_res()` translates to 
- enums are also mapped to module `lv`
- widgets constructors are classes in module `lv`
- calls to widgets are object methods (skip the first argument which is implicit)


C call|Berry call
:---|:---
`lv_get_hor_res()`|`lv.get_hor_res()`
`LV_ALIGN_BOTTOM_MID`|`lv.ALIGN_BOTTOM_MID`
`lv_colorwheel_create(scr, false)`|`lv.colorwheel(scr, false)` or `lv.lv_colorwheel(scr, false)`
`lv_obj_set_size(colw, 100, 100)`|`colw.set_size(100, 100)`

## Supported features

lv_berry provides an object-oriented interface for LVGL, and covers over 99% of LVGL's APIs, noticeably:

- access to almost all LVGL APIs (over 1000+ calls)
- automatic type checking and type conversion based on `C` headers
- access to all `C` ENUMs constants (500+)
- full support for C callbacks in Berry
- full support for LVGL internal memory structures
- ability to create pure Berry custom widgets and custom classes
- supports only 16 bits display (swapped or not)

What is currently not supported (but could be if there is demand):

- multi-screen devices (no support in Tasmota anyways)
- bidirectional text support
- LVGL tasks (Berry has its own task system)
- non-16 bits display

## Type system

### Widgets classes

Although LVGL is C code and is not formally object oriented, LVGL widget follow an inheritance model. Each widget is a virtual subclass of `lv_obj` structure.

Berry builds an actual Object Oriented class system, with a base class `lv_obj` and subclasses.

The class names supported are defined in `convert.py` and are currently standard widgets:

```
'lv_arc', 'lv_bar', 'lv_btn', 'lv_btnmatrix', 'lv_canvas', 'lv_checkbox', 'lv_dropdown', 'lv_img', 'lv_label', 'lv_line', 'lv_roller', 'lv_slider', 'lv_switch', 'lv_table', 'lv_textarea'
```

And extra widgets:

```
'lv_chart', 'lv_colorwheel', 'lv_imgbtn', 'lv_led', 'lv_meter', 'lv_msgbox', 'lv_spinbox', 'lv_spinner'
```

Additional 'special' classes are (they do not inherit from `lv_obj`):

```
'lv_obj', 'lv_group', 'lv_style', 'lv_indev', 'lv_disp'
```

### Colors

An exception for LVGL colors, they are defined as 32 bits RGB values as follows, and not based on their C representation:

```C
COLOR_WHITE=0xFFFFFF
COLOR_SILVER=0xC0C0C0
COLOR_GRAY=0x808080
COLOR_BLACK=0x000000
COLOR_RED=0xFF0000
COLOR_MAROON=0x800000
COLOR_YELLOW=0xFFFF00
COLOR_OLIVE=0x808000
COLOR_LIME=0x00FF00
COLOR_GREEN=0x008000
COLOR_CYAN=0x00FFFF
COLOR_AQUA=0x00FFFF
COLOR_TEAL=0x008080
COLOR_BLUE=0x0000FF
COLOR_NAVY=0x000080
COLOR_MAGENTA=0xFF00FF
COLOR_PURPLE=0x800080
```

Example: `lv.COLOR_RED`

`lv_color` is a simple class that maps RGB 32 bits colors (as 32 bits int) to the internal representation of colors (usually 16 bits).

Don't be surprised that getting back a value is the 16 bits color converted to 32 bits - rounding errors may occur:

```
[Berry Console]
> c = lv_color(0x808080)
> c
lv_color(0xff838183 - native:0x1084)
```

Note:

- 0xff838183 - is the 32 bits color, with alpha channel (opaque)
- 0x1084 - is the native internal representation as 16 bits color with swapped bytes

## Build system

More details about the mapping are provided in [Tasmota LVGL Internals](https://tasmota.github.io/docs/LVGL_Internals/)

`lv_berry` uses a semi-automated parsing of lvgl's `C` headers to generate the Berry mapping. The process is automated, but may require small parser adjustments with new versions.

This module relies on [berry_mapping](https://github.com/berry-lang/berry_mapping) module to automate the mapping of C functions.

**Phase 1: Parse LVGL source**

This first phase parses most C headers from the LVGL source tree and generates two files:

- `mapping/lv_enum.h` containing all the `enum` values from LVGL (constants)
- `mapping/lv_funcs.h` containing all the functions of the LVGL API normalized to 1 function per line, and with cleaned argument signature.

```
(`cd` in folder tools)

❯ python3 preprocessor.py
(no output)
```

**Phase 2: Generate automatic Berry mapping**

From the two files created in the previous step, all the requires C files are created for the Berry mapping.

```
(`cd`in folder tools)

> python3 convert.py
| callback types['lv_group_focus_cb', 'lv_event_cb', 'lv_constructor_cb', 'lv_layout_update_cb', 'lv_obj_tree_walk_cb', 'lv_theme_apply_cb', 'lv_color_filter_cb', 'lv_anim_path_cb']
```

The output should look as above, and indicates the C function that have been ignored (if any) if their return type is listed above. It also lists the callback types supported.


**Phase 3: Generate the Berry pre-compiled stubs**

This phase is specific to Berry pre-compiled modules and classes.

```
(berry)

> python3 tools/coc/coc -o generate src default ../berry_mapping/src ../lv_berry/src ../lv_berry/generate -c default/berry_conf.h
(no output)
```

**Phase 4: compile Tasmota using platform.io as usual**

## Future projects

There is an on-going project to provide [OpenHASP](https://openhasp.haswitchplate.com/) emulation using LVGL+Berry. This will provide a super-easy to use JSON graphical description of UI interfaces, with no or very low knowledge of LVGL APIs.

Stay tuned!
