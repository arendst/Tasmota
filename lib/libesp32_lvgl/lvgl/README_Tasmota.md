# Tasmota patches

Following patches need to be applied when updating LVGL:

`src/extra/libs/lv_extra.c`

Comment the `lv_freetype_init()` portion. The actual initializatio is done in Tasmota code
because parameters depend on Tasmota defines and presence of PSRAM.

``` C
// TASMOTA Specific, the initialization is done in Tasmota code to adjust with PSRAM
// #if LV_USE_FREETYPE
//     /*Init freetype library*/
// #  if LV_FREETYPE_CACHE_SIZE >= 0
//     lv_freetype_init(LV_FREETYPE_CACHE_FT_FACES, LV_FREETYPE_CACHE_FT_SIZES, LV_FREETYPE_CACHE_SIZE);
// #  else
//     lv_freetype_init(0, 0, 0);
// #  endif
// #endif
```
