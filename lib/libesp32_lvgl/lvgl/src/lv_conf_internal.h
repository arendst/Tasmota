/**
 * GENERATED FILE, DO NOT EDIT IT!
 * @file lv_conf_internal.h
 * Make sure all the defines of lv_conf.h have a default value
**/

#ifndef LV_CONF_INTERNAL_H
#define LV_CONF_INTERNAL_H
/* clang-format off */

/*Config options*/
#define LV_OS_NONE          0
#define LV_OS_PTHREAD       1
#define LV_OS_FREERTOS      2
#define LV_OS_CMSIS_RTOS2   3
#define LV_OS_RTTHREAD      4
#define LV_OS_WINDOWS       5
#define LV_OS_CUSTOM        255

#define LV_STDLIB_BUILTIN           0
#define LV_STDLIB_CLIB              1
#define LV_STDLIB_MICROPYTHON       2
#define LV_STDLIB_RTTHREAD          3
#define LV_STDLIB_CUSTOM            255

#define LV_DRAW_SW_ASM_NONE         0
#define LV_DRAW_SW_ASM_NEON         1
#define LV_DRAW_SW_ASM_HELIUM       2
#define LV_DRAW_SW_ASM_CUSTOM       255

/* Handle special Kconfig options */
#ifndef LV_KCONFIG_IGNORE
    #include "lv_conf_kconfig.h"
    #ifdef CONFIG_LV_CONF_SKIP
        #define LV_CONF_SKIP
    #endif
#endif

/*If "lv_conf.h" is available from here try to use it later.*/
#ifdef __has_include
    #if __has_include("lv_conf.h")
        #ifndef LV_CONF_INCLUDE_SIMPLE
            #define LV_CONF_INCLUDE_SIMPLE
        #endif
    #endif
#endif

/*If lv_conf.h is not skipped include it*/
#ifndef LV_CONF_SKIP
    #ifdef LV_CONF_PATH                           /*If there is a path defined for lv_conf.h use it*/
        #define __LV_TO_STR_AUX(x) #x
        #define __LV_TO_STR(x) __LV_TO_STR_AUX(x)
        #include __LV_TO_STR(LV_CONF_PATH)
        #undef __LV_TO_STR_AUX
        #undef __LV_TO_STR
    #elif defined(LV_CONF_INCLUDE_SIMPLE)         /*Or simply include lv_conf.h is enabled*/
        #include "lv_conf.h"
    #else
        #include "../../lv_conf.h"                /*Else assume lv_conf.h is next to the lvgl folder*/
    #endif
    #if !defined(LV_CONF_H) && !defined(LV_CONF_SUPPRESS_DEFINE_CHECK)
        /* #include will sometimes silently fail when __has_include is used */
        /* https://gcc.gnu.org/bugzilla/show_bug.cgi?id=80753 */
        #pragma message("Possible failure to include lv_conf.h, please read the comment in this file if you get errors")
    #endif
#endif

#ifdef CONFIG_LV_COLOR_DEPTH
    #define _LV_KCONFIG_PRESENT
#endif

/*----------------------------------
 * Start parsing lv_conf_template.h
 -----------------------------------*/

/*====================
   COLOR SETTINGS
 *====================*/

/*Color depth: 8 (A8), 16 (RGB565), 24 (RGB888), 32 (XRGB8888)*/
#ifndef LV_COLOR_DEPTH
    #ifdef CONFIG_LV_COLOR_DEPTH
        #define LV_COLOR_DEPTH CONFIG_LV_COLOR_DEPTH
    #else
        #define LV_COLOR_DEPTH 16
    #endif
#endif

/*=========================
   STDLIB WRAPPER SETTINGS
 *=========================*/

/* Possible values
 * - LV_STDLIB_BUILTIN:     LVGL's built in implementation
 * - LV_STDLIB_CLIB:        Standard C functions, like malloc, strlen, etc
 * - LV_STDLIB_MICROPYTHON: MicroPython implementation
 * - LV_STDLIB_RTTHREAD:    RT-Thread implementation
 * - LV_STDLIB_CUSTOM:      Implement the functions externally
 */
#ifndef LV_USE_STDLIB_MALLOC
    #ifdef CONFIG_LV_USE_STDLIB_MALLOC
        #define LV_USE_STDLIB_MALLOC CONFIG_LV_USE_STDLIB_MALLOC
    #else
        #define LV_USE_STDLIB_MALLOC    LV_STDLIB_BUILTIN
    #endif
#endif
#ifndef LV_USE_STDLIB_STRING
    #ifdef CONFIG_LV_USE_STDLIB_STRING
        #define LV_USE_STDLIB_STRING CONFIG_LV_USE_STDLIB_STRING
    #else
        #define LV_USE_STDLIB_STRING    LV_STDLIB_BUILTIN
    #endif
#endif
#ifndef LV_USE_STDLIB_SPRINTF
    #ifdef CONFIG_LV_USE_STDLIB_SPRINTF
        #define LV_USE_STDLIB_SPRINTF CONFIG_LV_USE_STDLIB_SPRINTF
    #else
        #define LV_USE_STDLIB_SPRINTF   LV_STDLIB_BUILTIN
    #endif
#endif


#if LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
    /*Size of the memory available for `lv_malloc()` in bytes (>= 2kB)*/
    #ifndef LV_MEM_SIZE
        #ifdef CONFIG_LV_MEM_SIZE
            #define LV_MEM_SIZE CONFIG_LV_MEM_SIZE
        #else
            #define LV_MEM_SIZE (64 * 1024U)          /*[bytes]*/
        #endif
    #endif

    /*Size of the memory expand for `lv_malloc()` in bytes*/
    #ifndef LV_MEM_POOL_EXPAND_SIZE
        #ifdef CONFIG_LV_MEM_POOL_EXPAND_SIZE
            #define LV_MEM_POOL_EXPAND_SIZE CONFIG_LV_MEM_POOL_EXPAND_SIZE
        #else
            #define LV_MEM_POOL_EXPAND_SIZE 0
        #endif
    #endif

    /*Set an address for the memory pool instead of allocating it as a normal array. Can be in external SRAM too.*/
    #ifndef LV_MEM_ADR
        #ifdef CONFIG_LV_MEM_ADR
            #define LV_MEM_ADR CONFIG_LV_MEM_ADR
        #else
            #define LV_MEM_ADR 0     /*0: unused*/
        #endif
    #endif
    /*Instead of an address give a memory allocator that will be called to get a memory pool for LVGL. E.g. my_malloc*/
    #if LV_MEM_ADR == 0
        #ifndef LV_MEM_POOL_INCLUDE
            #ifdef CONFIG_LV_MEM_POOL_INCLUDE
                #define LV_MEM_POOL_INCLUDE CONFIG_LV_MEM_POOL_INCLUDE
            #else
                #undef LV_MEM_POOL_INCLUDE
            #endif
        #endif
        #ifndef LV_MEM_POOL_ALLOC
            #ifdef CONFIG_LV_MEM_POOL_ALLOC
                #define LV_MEM_POOL_ALLOC CONFIG_LV_MEM_POOL_ALLOC
            #else
                #undef LV_MEM_POOL_ALLOC
            #endif
        #endif
    #endif
#endif  /*LV_USE_MALLOC == LV_STDLIB_BUILTIN*/

/*====================
   HAL SETTINGS
 *====================*/

/*Default display refresh, input device read and animation step period.*/
#ifndef LV_DEF_REFR_PERIOD
    #ifdef CONFIG_LV_DEF_REFR_PERIOD
        #define LV_DEF_REFR_PERIOD CONFIG_LV_DEF_REFR_PERIOD
    #else
        #define LV_DEF_REFR_PERIOD  33      /*[ms]*/
    #endif
#endif

/*Default Dot Per Inch. Used to initialize default sizes such as widgets sized, style paddings.
 *(Not so important, you can adjust it to modify default sizes and spaces)*/
#ifndef LV_DPI_DEF
    #ifdef CONFIG_LV_DPI_DEF
        #define LV_DPI_DEF CONFIG_LV_DPI_DEF
    #else
        #define LV_DPI_DEF 130     /*[px/inch]*/
    #endif
#endif

/*=================
 * OPERATING SYSTEM
 *=================*/
/*Select an operating system to use. Possible options:
 * - LV_OS_NONE
 * - LV_OS_PTHREAD
 * - LV_OS_FREERTOS
 * - LV_OS_CMSIS_RTOS2
 * - LV_OS_RTTHREAD
 * - LV_OS_WINDOWS
 * - LV_OS_CUSTOM */
#ifndef LV_USE_OS
    #ifdef CONFIG_LV_USE_OS
        #define LV_USE_OS CONFIG_LV_USE_OS
    #else
        #define LV_USE_OS   LV_OS_NONE
    #endif
#endif

#if LV_USE_OS == LV_OS_CUSTOM
    #ifndef LV_OS_CUSTOM_INCLUDE
        #ifdef CONFIG_LV_OS_CUSTOM_INCLUDE
            #define LV_OS_CUSTOM_INCLUDE CONFIG_LV_OS_CUSTOM_INCLUDE
        #else
            #define LV_OS_CUSTOM_INCLUDE <stdint.h>
        #endif
    #endif
#endif

/*========================
 * RENDERING CONFIGURATION
 *========================*/

/*Align the stride of all layers and images to this bytes*/
#ifndef LV_DRAW_BUF_STRIDE_ALIGN
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_DRAW_BUF_STRIDE_ALIGN
            #define LV_DRAW_BUF_STRIDE_ALIGN CONFIG_LV_DRAW_BUF_STRIDE_ALIGN
        #else
            #define LV_DRAW_BUF_STRIDE_ALIGN 0
        #endif
    #else
        #define LV_DRAW_BUF_STRIDE_ALIGN                1
    #endif
#endif

/*Align the start address of draw_buf addresses to this bytes*/
#ifndef LV_DRAW_BUF_ALIGN
    #ifdef CONFIG_LV_DRAW_BUF_ALIGN
        #define LV_DRAW_BUF_ALIGN CONFIG_LV_DRAW_BUF_ALIGN
    #else
        #define LV_DRAW_BUF_ALIGN                       4
    #endif
#endif

#ifndef LV_USE_DRAW_SW
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_DRAW_SW
            #define LV_USE_DRAW_SW CONFIG_LV_USE_DRAW_SW
        #else
            #define LV_USE_DRAW_SW 0
        #endif
    #else
        #define LV_USE_DRAW_SW 1
    #endif
#endif
#if LV_USE_DRAW_SW == 1
    /* Set the number of draw unit.
     * > 1 requires an operating system enabled in `LV_USE_OS`
     * > 1 means multiply threads will render the screen in parallel */
    #ifndef LV_DRAW_SW_DRAW_UNIT_CNT
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_DRAW_SW_DRAW_UNIT_CNT
                #define LV_DRAW_SW_DRAW_UNIT_CNT CONFIG_LV_DRAW_SW_DRAW_UNIT_CNT
            #else
                #define LV_DRAW_SW_DRAW_UNIT_CNT 0
            #endif
        #else
            #define LV_DRAW_SW_DRAW_UNIT_CNT    1
        #endif
    #endif

    /* Use Arm-2D to accelerate the sw render */
    #ifndef LV_USE_DRAW_ARM2D_SYNC
        #ifdef CONFIG_LV_USE_DRAW_ARM2D_SYNC
            #define LV_USE_DRAW_ARM2D_SYNC CONFIG_LV_USE_DRAW_ARM2D_SYNC
        #else
            #define LV_USE_DRAW_ARM2D_SYNC      0
        #endif
    #endif

    /* If a widget has `style_opa < 255` (not `bg_opa`, `text_opa` etc) or not NORMAL blend mode
     * it is buffered into a "simple" layer before rendering. The widget can be buffered in smaller chunks.
     * "Transformed layers" (if `transform_angle/zoom` are set) use larger buffers
     * and can't be drawn in chunks. */

    /*The target buffer size for simple layer chunks.*/
    #ifndef LV_DRAW_SW_LAYER_SIMPLE_BUF_SIZE
        #ifdef CONFIG_LV_DRAW_SW_LAYER_SIMPLE_BUF_SIZE
            #define LV_DRAW_SW_LAYER_SIMPLE_BUF_SIZE CONFIG_LV_DRAW_SW_LAYER_SIMPLE_BUF_SIZE
        #else
            #define LV_DRAW_SW_LAYER_SIMPLE_BUF_SIZE    (24 * 1024)   /*[bytes]*/
        #endif
    #endif

    /* 0: use a simple renderer capable of drawing only simple rectangles with gradient, images, texts, and straight lines only
     * 1: use a complex renderer capable of drawing rounded corners, shadow, skew lines, and arcs too */
    #ifndef LV_DRAW_SW_COMPLEX
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_DRAW_SW_COMPLEX
                #define LV_DRAW_SW_COMPLEX CONFIG_LV_DRAW_SW_COMPLEX
            #else
                #define LV_DRAW_SW_COMPLEX 0
            #endif
        #else
            #define LV_DRAW_SW_COMPLEX          1
        #endif
    #endif

    #if LV_DRAW_SW_COMPLEX == 1
        /*Allow buffering some shadow calculation.
        *LV_DRAW_SW_SHADOW_CACHE_SIZE is the max. shadow size to buffer, where shadow size is `shadow_width + radius`
        *Caching has LV_DRAW_SW_SHADOW_CACHE_SIZE^2 RAM cost*/
        #ifndef LV_DRAW_SW_SHADOW_CACHE_SIZE
            #ifdef CONFIG_LV_DRAW_SW_SHADOW_CACHE_SIZE
                #define LV_DRAW_SW_SHADOW_CACHE_SIZE CONFIG_LV_DRAW_SW_SHADOW_CACHE_SIZE
            #else
                #define LV_DRAW_SW_SHADOW_CACHE_SIZE 0
            #endif
        #endif

        /* Set number of maximally cached circle data.
        * The circumference of 1/4 circle are saved for anti-aliasing
        * radius * 4 bytes are used per circle (the most often used radiuses are saved)
        * 0: to disable caching */
        #ifndef LV_DRAW_SW_CIRCLE_CACHE_SIZE
            #ifdef CONFIG_LV_DRAW_SW_CIRCLE_CACHE_SIZE
                #define LV_DRAW_SW_CIRCLE_CACHE_SIZE CONFIG_LV_DRAW_SW_CIRCLE_CACHE_SIZE
            #else
                #define LV_DRAW_SW_CIRCLE_CACHE_SIZE 4
            #endif
        #endif
    #endif

    #ifndef LV_USE_DRAW_SW_ASM
        #ifdef CONFIG_LV_USE_DRAW_SW_ASM
            #define LV_USE_DRAW_SW_ASM CONFIG_LV_USE_DRAW_SW_ASM
        #else
            #define  LV_USE_DRAW_SW_ASM     LV_DRAW_SW_ASM_NONE
        #endif
    #endif

    #if LV_USE_DRAW_SW_ASM == LV_DRAW_SW_ASM_CUSTOM
        #ifndef LV_DRAW_SW_ASM_CUSTOM_INCLUDE
            #ifdef CONFIG_LV_DRAW_SW_ASM_CUSTOM_INCLUDE
                #define LV_DRAW_SW_ASM_CUSTOM_INCLUDE CONFIG_LV_DRAW_SW_ASM_CUSTOM_INCLUDE
            #else
                #define  LV_DRAW_SW_ASM_CUSTOM_INCLUDE ""
            #endif
        #endif
    #endif
#endif

/* Use NXP's VG-Lite GPU on iMX RTxxx platforms. */
#ifndef LV_USE_DRAW_VGLITE
    #ifdef CONFIG_LV_USE_DRAW_VGLITE
        #define LV_USE_DRAW_VGLITE CONFIG_LV_USE_DRAW_VGLITE
    #else
        #define LV_USE_DRAW_VGLITE 0
    #endif
#endif

#if LV_USE_DRAW_VGLITE
    /* Enable blit quality degradation workaround recommended for screen's dimension > 352 pixels. */
    #ifndef LV_USE_VGLITE_BLIT_SPLIT
        #ifdef CONFIG_LV_USE_VGLITE_BLIT_SPLIT
            #define LV_USE_VGLITE_BLIT_SPLIT CONFIG_LV_USE_VGLITE_BLIT_SPLIT
        #else
            #define LV_USE_VGLITE_BLIT_SPLIT 0
        #endif
    #endif

    #if LV_USE_OS
        /* Enable VGLite draw async. Queue multiple tasks and flash them once to the GPU. */
        #ifndef LV_USE_VGLITE_DRAW_ASYNC
            #ifdef _LV_KCONFIG_PRESENT
                #ifdef CONFIG_LV_USE_VGLITE_DRAW_ASYNC
                    #define LV_USE_VGLITE_DRAW_ASYNC CONFIG_LV_USE_VGLITE_DRAW_ASYNC
                #else
                    #define LV_USE_VGLITE_DRAW_ASYNC 0
                #endif
            #else
                #define LV_USE_VGLITE_DRAW_ASYNC 1
            #endif
        #endif
    #endif

    /* Enable VGLite asserts. */
    #ifndef LV_USE_VGLITE_ASSERT
        #ifdef CONFIG_LV_USE_VGLITE_ASSERT
            #define LV_USE_VGLITE_ASSERT CONFIG_LV_USE_VGLITE_ASSERT
        #else
            #define LV_USE_VGLITE_ASSERT 0
        #endif
    #endif
#endif

/* Use NXP's PXP on iMX RTxxx platforms. */
#ifndef LV_USE_DRAW_PXP
    #ifdef CONFIG_LV_USE_DRAW_PXP
        #define LV_USE_DRAW_PXP CONFIG_LV_USE_DRAW_PXP
    #else
        #define LV_USE_DRAW_PXP 0
    #endif
#endif

#if LV_USE_DRAW_PXP
    /* Enable PXP asserts. */
    #ifndef LV_USE_PXP_ASSERT
        #ifdef CONFIG_LV_USE_PXP_ASSERT
            #define LV_USE_PXP_ASSERT CONFIG_LV_USE_PXP_ASSERT
        #else
            #define LV_USE_PXP_ASSERT 0
        #endif
    #endif
#endif

/* Use Renesas Dave2D on RA  platforms. */
#ifndef LV_USE_DRAW_DAVE2D
    #ifdef CONFIG_LV_USE_DRAW_DAVE2D
        #define LV_USE_DRAW_DAVE2D CONFIG_LV_USE_DRAW_DAVE2D
    #else
        #define LV_USE_DRAW_DAVE2D 0
    #endif
#endif

/* Draw using cached SDL textures*/
#ifndef LV_USE_DRAW_SDL
    #ifdef CONFIG_LV_USE_DRAW_SDL
        #define LV_USE_DRAW_SDL CONFIG_LV_USE_DRAW_SDL
    #else
        #define LV_USE_DRAW_SDL 0
    #endif
#endif

/* Use VG-Lite GPU. */
#ifndef LV_USE_DRAW_VG_LITE
    #ifdef CONFIG_LV_USE_DRAW_VG_LITE
        #define LV_USE_DRAW_VG_LITE CONFIG_LV_USE_DRAW_VG_LITE
    #else
        #define LV_USE_DRAW_VG_LITE 0
    #endif
#endif

#if LV_USE_DRAW_VG_LITE
/* Enable VG-Lite custom external 'gpu_init()' function */
#ifndef LV_VG_LITE_USE_GPU_INIT
    #ifdef CONFIG_LV_VG_LITE_USE_GPU_INIT
        #define LV_VG_LITE_USE_GPU_INIT CONFIG_LV_VG_LITE_USE_GPU_INIT
    #else
        #define LV_VG_LITE_USE_GPU_INIT 0
    #endif
#endif

/* Enable VG-Lite assert. */
#ifndef LV_VG_LITE_USE_ASSERT
    #ifdef CONFIG_LV_VG_LITE_USE_ASSERT
        #define LV_VG_LITE_USE_ASSERT CONFIG_LV_VG_LITE_USE_ASSERT
    #else
        #define LV_VG_LITE_USE_ASSERT 0
    #endif
#endif

#endif

/*=======================
 * FEATURE CONFIGURATION
 *=======================*/

/*-------------
 * Logging
 *-----------*/

/*Enable the log module*/
#ifndef LV_USE_LOG
    #ifdef CONFIG_LV_USE_LOG
        #define LV_USE_LOG CONFIG_LV_USE_LOG
    #else
        #define LV_USE_LOG 0
    #endif
#endif
#if LV_USE_LOG

    /*How important log should be added:
    *LV_LOG_LEVEL_TRACE       A lot of logs to give detailed information
    *LV_LOG_LEVEL_INFO        Log important events
    *LV_LOG_LEVEL_WARN        Log if something unwanted happened but didn't cause a problem
    *LV_LOG_LEVEL_ERROR       Only critical issue, when the system may fail
    *LV_LOG_LEVEL_USER        Only logs added by the user
    *LV_LOG_LEVEL_NONE        Do not log anything*/
    #ifndef LV_LOG_LEVEL
        #ifdef CONFIG_LV_LOG_LEVEL
            #define LV_LOG_LEVEL CONFIG_LV_LOG_LEVEL
        #else
            #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
        #endif
    #endif

    /*1: Print the log with 'printf';
    *0: User need to register a callback with `lv_log_register_print_cb()`*/
    #ifndef LV_LOG_PRINTF
        #ifdef CONFIG_LV_LOG_PRINTF
            #define LV_LOG_PRINTF CONFIG_LV_LOG_PRINTF
        #else
            #define LV_LOG_PRINTF 0
        #endif
    #endif

    /*1: Enable print timestamp;
     *0: Disable print timestamp*/
    #ifndef LV_LOG_USE_TIMESTAMP
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_LOG_USE_TIMESTAMP
                #define LV_LOG_USE_TIMESTAMP CONFIG_LV_LOG_USE_TIMESTAMP
            #else
                #define LV_LOG_USE_TIMESTAMP 0
            #endif
        #else
            #define LV_LOG_USE_TIMESTAMP 1
        #endif
    #endif

    /*1: Print file and line number of the log;
     *0: Do not print file and line number of the log*/
    #ifndef LV_LOG_USE_FILE_LINE
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_LOG_USE_FILE_LINE
                #define LV_LOG_USE_FILE_LINE CONFIG_LV_LOG_USE_FILE_LINE
            #else
                #define LV_LOG_USE_FILE_LINE 0
            #endif
        #else
            #define LV_LOG_USE_FILE_LINE 1
        #endif
    #endif

    /*Enable/disable LV_LOG_TRACE in modules that produces a huge number of logs*/
    #ifndef LV_LOG_TRACE_MEM
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_LOG_TRACE_MEM
                #define LV_LOG_TRACE_MEM CONFIG_LV_LOG_TRACE_MEM
            #else
                #define LV_LOG_TRACE_MEM 0
            #endif
        #else
            #define LV_LOG_TRACE_MEM        1
        #endif
    #endif
    #ifndef LV_LOG_TRACE_TIMER
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_LOG_TRACE_TIMER
                #define LV_LOG_TRACE_TIMER CONFIG_LV_LOG_TRACE_TIMER
            #else
                #define LV_LOG_TRACE_TIMER 0
            #endif
        #else
            #define LV_LOG_TRACE_TIMER      1
        #endif
    #endif
    #ifndef LV_LOG_TRACE_INDEV
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_LOG_TRACE_INDEV
                #define LV_LOG_TRACE_INDEV CONFIG_LV_LOG_TRACE_INDEV
            #else
                #define LV_LOG_TRACE_INDEV 0
            #endif
        #else
            #define LV_LOG_TRACE_INDEV      1
        #endif
    #endif
    #ifndef LV_LOG_TRACE_DISP_REFR
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_LOG_TRACE_DISP_REFR
                #define LV_LOG_TRACE_DISP_REFR CONFIG_LV_LOG_TRACE_DISP_REFR
            #else
                #define LV_LOG_TRACE_DISP_REFR 0
            #endif
        #else
            #define LV_LOG_TRACE_DISP_REFR  1
        #endif
    #endif
    #ifndef LV_LOG_TRACE_EVENT
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_LOG_TRACE_EVENT
                #define LV_LOG_TRACE_EVENT CONFIG_LV_LOG_TRACE_EVENT
            #else
                #define LV_LOG_TRACE_EVENT 0
            #endif
        #else
            #define LV_LOG_TRACE_EVENT      1
        #endif
    #endif
    #ifndef LV_LOG_TRACE_OBJ_CREATE
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_LOG_TRACE_OBJ_CREATE
                #define LV_LOG_TRACE_OBJ_CREATE CONFIG_LV_LOG_TRACE_OBJ_CREATE
            #else
                #define LV_LOG_TRACE_OBJ_CREATE 0
            #endif
        #else
            #define LV_LOG_TRACE_OBJ_CREATE 1
        #endif
    #endif
    #ifndef LV_LOG_TRACE_LAYOUT
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_LOG_TRACE_LAYOUT
                #define LV_LOG_TRACE_LAYOUT CONFIG_LV_LOG_TRACE_LAYOUT
            #else
                #define LV_LOG_TRACE_LAYOUT 0
            #endif
        #else
            #define LV_LOG_TRACE_LAYOUT     1
        #endif
    #endif
    #ifndef LV_LOG_TRACE_ANIM
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_LOG_TRACE_ANIM
                #define LV_LOG_TRACE_ANIM CONFIG_LV_LOG_TRACE_ANIM
            #else
                #define LV_LOG_TRACE_ANIM 0
            #endif
        #else
            #define LV_LOG_TRACE_ANIM       1
        #endif
    #endif
    #ifndef LV_LOG_TRACE_CACHE
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_LOG_TRACE_CACHE
                #define LV_LOG_TRACE_CACHE CONFIG_LV_LOG_TRACE_CACHE
            #else
                #define LV_LOG_TRACE_CACHE 0
            #endif
        #else
            #define LV_LOG_TRACE_CACHE      1
        #endif
    #endif

#endif  /*LV_USE_LOG*/

/*-------------
 * Asserts
 *-----------*/

/*Enable asserts if an operation is failed or an invalid data is found.
 *If LV_USE_LOG is enabled an error message will be printed on failure*/
#ifndef LV_USE_ASSERT_NULL
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_ASSERT_NULL
            #define LV_USE_ASSERT_NULL CONFIG_LV_USE_ASSERT_NULL
        #else
            #define LV_USE_ASSERT_NULL 0
        #endif
    #else
        #define LV_USE_ASSERT_NULL          1   /*Check if the parameter is NULL. (Very fast, recommended)*/
    #endif
#endif
#ifndef LV_USE_ASSERT_MALLOC
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_ASSERT_MALLOC
            #define LV_USE_ASSERT_MALLOC CONFIG_LV_USE_ASSERT_MALLOC
        #else
            #define LV_USE_ASSERT_MALLOC 0
        #endif
    #else
        #define LV_USE_ASSERT_MALLOC        1   /*Checks is the memory is successfully allocated or no. (Very fast, recommended)*/
    #endif
#endif
#ifndef LV_USE_ASSERT_STYLE
    #ifdef CONFIG_LV_USE_ASSERT_STYLE
        #define LV_USE_ASSERT_STYLE CONFIG_LV_USE_ASSERT_STYLE
    #else
        #define LV_USE_ASSERT_STYLE         0   /*Check if the styles are properly initialized. (Very fast, recommended)*/
    #endif
#endif
#ifndef LV_USE_ASSERT_MEM_INTEGRITY
    #ifdef CONFIG_LV_USE_ASSERT_MEM_INTEGRITY
        #define LV_USE_ASSERT_MEM_INTEGRITY CONFIG_LV_USE_ASSERT_MEM_INTEGRITY
    #else
        #define LV_USE_ASSERT_MEM_INTEGRITY 0   /*Check the integrity of `lv_mem` after critical operations. (Slow)*/
    #endif
#endif
#ifndef LV_USE_ASSERT_OBJ
    #ifdef CONFIG_LV_USE_ASSERT_OBJ
        #define LV_USE_ASSERT_OBJ CONFIG_LV_USE_ASSERT_OBJ
    #else
        #define LV_USE_ASSERT_OBJ           0   /*Check the object's type and existence (e.g. not deleted). (Slow)*/
    #endif
#endif

/*Add a custom handler when assert happens e.g. to restart the MCU*/
#ifndef LV_ASSERT_HANDLER_INCLUDE
    #ifdef CONFIG_LV_ASSERT_HANDLER_INCLUDE
        #define LV_ASSERT_HANDLER_INCLUDE CONFIG_LV_ASSERT_HANDLER_INCLUDE
    #else
        #define LV_ASSERT_HANDLER_INCLUDE <stdint.h>
    #endif
#endif
#ifndef LV_ASSERT_HANDLER
    #ifdef CONFIG_LV_ASSERT_HANDLER
        #define LV_ASSERT_HANDLER CONFIG_LV_ASSERT_HANDLER
    #else
        #define LV_ASSERT_HANDLER while(1);   /*Halt by default*/
    #endif
#endif

/*-------------
 * Debug
 *-----------*/

/*1: Draw random colored rectangles over the redrawn areas*/
#ifndef LV_USE_REFR_DEBUG
    #ifdef CONFIG_LV_USE_REFR_DEBUG
        #define LV_USE_REFR_DEBUG CONFIG_LV_USE_REFR_DEBUG
    #else
        #define LV_USE_REFR_DEBUG 0
    #endif
#endif

/*1: Draw a red overlay for ARGB layers and a green overlay for RGB layers*/
#ifndef LV_USE_LAYER_DEBUG
    #ifdef CONFIG_LV_USE_LAYER_DEBUG
        #define LV_USE_LAYER_DEBUG CONFIG_LV_USE_LAYER_DEBUG
    #else
        #define LV_USE_LAYER_DEBUG 0
    #endif
#endif

/*1: Draw overlays with different colors for each draw_unit's tasks.
 *Also add the index number of the draw unit on white background.
 *For layers add the index number of the draw unit on black background.*/
#ifndef LV_USE_PARALLEL_DRAW_DEBUG
    #ifdef CONFIG_LV_USE_PARALLEL_DRAW_DEBUG
        #define LV_USE_PARALLEL_DRAW_DEBUG CONFIG_LV_USE_PARALLEL_DRAW_DEBUG
    #else
        #define LV_USE_PARALLEL_DRAW_DEBUG 0
    #endif
#endif

/*-------------
 * Others
 *-----------*/

#ifndef LV_ENABLE_GLOBAL_CUSTOM
    #ifdef CONFIG_LV_ENABLE_GLOBAL_CUSTOM
        #define LV_ENABLE_GLOBAL_CUSTOM CONFIG_LV_ENABLE_GLOBAL_CUSTOM
    #else
        #define LV_ENABLE_GLOBAL_CUSTOM 0
    #endif
#endif
#if LV_ENABLE_GLOBAL_CUSTOM
    /*Header to include for the custom 'lv_global' function"*/
    #ifndef LV_GLOBAL_CUSTOM_INCLUDE
        #ifdef CONFIG_LV_GLOBAL_CUSTOM_INCLUDE
            #define LV_GLOBAL_CUSTOM_INCLUDE CONFIG_LV_GLOBAL_CUSTOM_INCLUDE
        #else
            #define LV_GLOBAL_CUSTOM_INCLUDE <stdint.h>
        #endif
    #endif
#endif

/*Default cache size in bytes.
 *Used by image decoders such as `lv_lodepng` to keep the decoded image in the memory.
 *If size is not set to 0, the decoder will fail to decode when the cache is full.
 *If size is 0, the cache function is not enabled and the decoded mem will be released immediately after use.*/
#ifndef LV_CACHE_DEF_SIZE
    #ifdef CONFIG_LV_CACHE_DEF_SIZE
        #define LV_CACHE_DEF_SIZE CONFIG_LV_CACHE_DEF_SIZE
    #else
        #define LV_CACHE_DEF_SIZE       0
    #endif
#endif

/*Default number of image header cache entries. The cache is used to store the headers of images
 *The main logic is like `LV_CACHE_DEF_SIZE` but for image headers.*/
#ifndef LV_IMAGE_HEADER_CACHE_DEF_CNT
    #ifdef CONFIG_LV_IMAGE_HEADER_CACHE_DEF_CNT
        #define LV_IMAGE_HEADER_CACHE_DEF_CNT CONFIG_LV_IMAGE_HEADER_CACHE_DEF_CNT
    #else
        #define LV_IMAGE_HEADER_CACHE_DEF_CNT 0
    #endif
#endif

/*Number of stops allowed per gradient. Increase this to allow more stops.
 *This adds (sizeof(lv_color_t) + 1) bytes per additional stop*/
#ifndef LV_GRADIENT_MAX_STOPS
    #ifdef CONFIG_LV_GRADIENT_MAX_STOPS
        #define LV_GRADIENT_MAX_STOPS CONFIG_LV_GRADIENT_MAX_STOPS
    #else
        #define LV_GRADIENT_MAX_STOPS   2
    #endif
#endif

/* Adjust color mix functions rounding. GPUs might calculate color mix (blending) differently.
 * 0: round down, 64: round up from x.75, 128: round up from half, 192: round up from x.25, 254: round up */
#ifndef LV_COLOR_MIX_ROUND_OFS
    #ifdef CONFIG_LV_COLOR_MIX_ROUND_OFS
        #define LV_COLOR_MIX_ROUND_OFS CONFIG_LV_COLOR_MIX_ROUND_OFS
    #else
        #define LV_COLOR_MIX_ROUND_OFS  0
    #endif
#endif

/* Add 2 x 32 bit variables to each lv_obj_t to speed up getting style properties */
#ifndef LV_OBJ_STYLE_CACHE
    #ifdef CONFIG_LV_OBJ_STYLE_CACHE
        #define LV_OBJ_STYLE_CACHE CONFIG_LV_OBJ_STYLE_CACHE
    #else
        #define LV_OBJ_STYLE_CACHE      0
    #endif
#endif

/* Add `id` field to `lv_obj_t` */
#ifndef LV_USE_OBJ_ID
    #ifdef CONFIG_LV_USE_OBJ_ID
        #define LV_USE_OBJ_ID CONFIG_LV_USE_OBJ_ID
    #else
        #define LV_USE_OBJ_ID           0
    #endif
#endif

/* Use lvgl builtin method for obj ID */
#ifndef LV_USE_OBJ_ID_BUILTIN
    #ifdef CONFIG_LV_USE_OBJ_ID_BUILTIN
        #define LV_USE_OBJ_ID_BUILTIN CONFIG_LV_USE_OBJ_ID_BUILTIN
    #else
        #define LV_USE_OBJ_ID_BUILTIN   0
    #endif
#endif

/*Use obj property set/get API*/
#ifndef LV_USE_OBJ_PROPERTY
    #ifdef CONFIG_LV_USE_OBJ_PROPERTY
        #define LV_USE_OBJ_PROPERTY CONFIG_LV_USE_OBJ_PROPERTY
    #else
        #define LV_USE_OBJ_PROPERTY 0
    #endif
#endif

/* VG-Lite Simulator */
/*Requires: LV_USE_THORVG_INTERNAL or LV_USE_THORVG_EXTERNAL */
#ifndef LV_USE_VG_LITE_THORVG
    #ifdef CONFIG_LV_USE_VG_LITE_THORVG
        #define LV_USE_VG_LITE_THORVG CONFIG_LV_USE_VG_LITE_THORVG
    #else
        #define LV_USE_VG_LITE_THORVG  0
    #endif
#endif

#if LV_USE_VG_LITE_THORVG

    /*Enable LVGL's blend mode support*/
    #ifndef LV_VG_LITE_THORVG_LVGL_BLEND_SUPPORT
        #ifdef CONFIG_LV_VG_LITE_THORVG_LVGL_BLEND_SUPPORT
            #define LV_VG_LITE_THORVG_LVGL_BLEND_SUPPORT CONFIG_LV_VG_LITE_THORVG_LVGL_BLEND_SUPPORT
        #else
            #define LV_VG_LITE_THORVG_LVGL_BLEND_SUPPORT 0
        #endif
    #endif

    /*Enable YUV color format support*/
    #ifndef LV_VG_LITE_THORVG_YUV_SUPPORT
        #ifdef CONFIG_LV_VG_LITE_THORVG_YUV_SUPPORT
            #define LV_VG_LITE_THORVG_YUV_SUPPORT CONFIG_LV_VG_LITE_THORVG_YUV_SUPPORT
        #else
            #define LV_VG_LITE_THORVG_YUV_SUPPORT 0
        #endif
    #endif

    /*Enable 16 pixels alignment*/
    #ifndef LV_VG_LITE_THORVG_16PIXELS_ALIGN
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_VG_LITE_THORVG_16PIXELS_ALIGN
                #define LV_VG_LITE_THORVG_16PIXELS_ALIGN CONFIG_LV_VG_LITE_THORVG_16PIXELS_ALIGN
            #else
                #define LV_VG_LITE_THORVG_16PIXELS_ALIGN 0
            #endif
        #else
            #define LV_VG_LITE_THORVG_16PIXELS_ALIGN 1
        #endif
    #endif

    /*Enable multi-thread render*/
    #ifndef LV_VG_LITE_THORVG_THREAD_RENDER
        #ifdef CONFIG_LV_VG_LITE_THORVG_THREAD_RENDER
            #define LV_VG_LITE_THORVG_THREAD_RENDER CONFIG_LV_VG_LITE_THORVG_THREAD_RENDER
        #else
            #define LV_VG_LITE_THORVG_THREAD_RENDER 0
        #endif
    #endif

#endif

/*=====================
 *  COMPILER SETTINGS
 *====================*/

/*For big endian systems set to 1*/
#ifndef LV_BIG_ENDIAN_SYSTEM
    #ifdef CONFIG_LV_BIG_ENDIAN_SYSTEM
        #define LV_BIG_ENDIAN_SYSTEM CONFIG_LV_BIG_ENDIAN_SYSTEM
    #else
        #define LV_BIG_ENDIAN_SYSTEM 0
    #endif
#endif

/*Define a custom attribute to `lv_tick_inc` function*/
#ifndef LV_ATTRIBUTE_TICK_INC
    #ifdef CONFIG_LV_ATTRIBUTE_TICK_INC
        #define LV_ATTRIBUTE_TICK_INC CONFIG_LV_ATTRIBUTE_TICK_INC
    #else
        #define LV_ATTRIBUTE_TICK_INC
    #endif
#endif

/*Define a custom attribute to `lv_timer_handler` function*/
#ifndef LV_ATTRIBUTE_TIMER_HANDLER
    #ifdef CONFIG_LV_ATTRIBUTE_TIMER_HANDLER
        #define LV_ATTRIBUTE_TIMER_HANDLER CONFIG_LV_ATTRIBUTE_TIMER_HANDLER
    #else
        #define LV_ATTRIBUTE_TIMER_HANDLER
    #endif
#endif

/*Define a custom attribute to `lv_display_flush_ready` function*/
#ifndef LV_ATTRIBUTE_FLUSH_READY
    #ifdef CONFIG_LV_ATTRIBUTE_FLUSH_READY
        #define LV_ATTRIBUTE_FLUSH_READY CONFIG_LV_ATTRIBUTE_FLUSH_READY
    #else
        #define LV_ATTRIBUTE_FLUSH_READY
    #endif
#endif

/*Required alignment size for buffers*/
#ifndef LV_ATTRIBUTE_MEM_ALIGN_SIZE
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_ATTRIBUTE_MEM_ALIGN_SIZE
            #define LV_ATTRIBUTE_MEM_ALIGN_SIZE CONFIG_LV_ATTRIBUTE_MEM_ALIGN_SIZE
        #else
            #define LV_ATTRIBUTE_MEM_ALIGN_SIZE 0
        #endif
    #else
        #define LV_ATTRIBUTE_MEM_ALIGN_SIZE 1
    #endif
#endif

/*Will be added where memories needs to be aligned (with -Os data might not be aligned to boundary by default).
 * E.g. __attribute__((aligned(4)))*/
#ifndef LV_ATTRIBUTE_MEM_ALIGN
    #ifdef CONFIG_LV_ATTRIBUTE_MEM_ALIGN
        #define LV_ATTRIBUTE_MEM_ALIGN CONFIG_LV_ATTRIBUTE_MEM_ALIGN
    #else
        #define LV_ATTRIBUTE_MEM_ALIGN
    #endif
#endif

/*Attribute to mark large constant arrays for example font's bitmaps*/
#ifndef LV_ATTRIBUTE_LARGE_CONST
    #ifdef CONFIG_LV_ATTRIBUTE_LARGE_CONST
        #define LV_ATTRIBUTE_LARGE_CONST CONFIG_LV_ATTRIBUTE_LARGE_CONST
    #else
        #define LV_ATTRIBUTE_LARGE_CONST
    #endif
#endif

/*Compiler prefix for a big array declaration in RAM*/
#ifndef LV_ATTRIBUTE_LARGE_RAM_ARRAY
    #ifdef CONFIG_LV_ATTRIBUTE_LARGE_RAM_ARRAY
        #define LV_ATTRIBUTE_LARGE_RAM_ARRAY CONFIG_LV_ATTRIBUTE_LARGE_RAM_ARRAY
    #else
        #define LV_ATTRIBUTE_LARGE_RAM_ARRAY
    #endif
#endif

/*Place performance critical functions into a faster memory (e.g RAM)*/
#ifndef LV_ATTRIBUTE_FAST_MEM
    #ifdef CONFIG_LV_ATTRIBUTE_FAST_MEM
        #define LV_ATTRIBUTE_FAST_MEM CONFIG_LV_ATTRIBUTE_FAST_MEM
    #else
        #define LV_ATTRIBUTE_FAST_MEM
    #endif
#endif

/*Export integer constant to binding. This macro is used with constants in the form of LV_<CONST> that
 *should also appear on LVGL binding API such as Micropython.*/
#ifndef LV_EXPORT_CONST_INT
    #ifdef CONFIG_LV_EXPORT_CONST_INT
        #define LV_EXPORT_CONST_INT CONFIG_LV_EXPORT_CONST_INT
    #else
        #define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning /*The default value just prevents GCC warning*/
    #endif
#endif

/*Prefix all global extern data with this*/
#ifndef LV_ATTRIBUTE_EXTERN_DATA
    #ifdef CONFIG_LV_ATTRIBUTE_EXTERN_DATA
        #define LV_ATTRIBUTE_EXTERN_DATA CONFIG_LV_ATTRIBUTE_EXTERN_DATA
    #else
        #define LV_ATTRIBUTE_EXTERN_DATA
    #endif
#endif

/* Use `float` as `lv_value_precise_t` */
#ifndef LV_USE_FLOAT
    #ifdef CONFIG_LV_USE_FLOAT
        #define LV_USE_FLOAT CONFIG_LV_USE_FLOAT
    #else
        #define LV_USE_FLOAT            0
    #endif
#endif

/*==================
 *   FONT USAGE
 *===================*/

/*Montserrat fonts with ASCII range and some symbols using bpp = 4
 *https://fonts.google.com/specimen/Montserrat*/
#ifndef LV_FONT_MONTSERRAT_8
    #ifdef CONFIG_LV_FONT_MONTSERRAT_8
        #define LV_FONT_MONTSERRAT_8 CONFIG_LV_FONT_MONTSERRAT_8
    #else
        #define LV_FONT_MONTSERRAT_8  0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_10
    #ifdef CONFIG_LV_FONT_MONTSERRAT_10
        #define LV_FONT_MONTSERRAT_10 CONFIG_LV_FONT_MONTSERRAT_10
    #else
        #define LV_FONT_MONTSERRAT_10 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_12
    #ifdef CONFIG_LV_FONT_MONTSERRAT_12
        #define LV_FONT_MONTSERRAT_12 CONFIG_LV_FONT_MONTSERRAT_12
    #else
        #define LV_FONT_MONTSERRAT_12 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_14
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_FONT_MONTSERRAT_14
            #define LV_FONT_MONTSERRAT_14 CONFIG_LV_FONT_MONTSERRAT_14
        #else
            #define LV_FONT_MONTSERRAT_14 0
        #endif
    #else
        #define LV_FONT_MONTSERRAT_14 1
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_16
    #ifdef CONFIG_LV_FONT_MONTSERRAT_16
        #define LV_FONT_MONTSERRAT_16 CONFIG_LV_FONT_MONTSERRAT_16
    #else
        #define LV_FONT_MONTSERRAT_16 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_18
    #ifdef CONFIG_LV_FONT_MONTSERRAT_18
        #define LV_FONT_MONTSERRAT_18 CONFIG_LV_FONT_MONTSERRAT_18
    #else
        #define LV_FONT_MONTSERRAT_18 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_20
    #ifdef CONFIG_LV_FONT_MONTSERRAT_20
        #define LV_FONT_MONTSERRAT_20 CONFIG_LV_FONT_MONTSERRAT_20
    #else
        #define LV_FONT_MONTSERRAT_20 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_22
    #ifdef CONFIG_LV_FONT_MONTSERRAT_22
        #define LV_FONT_MONTSERRAT_22 CONFIG_LV_FONT_MONTSERRAT_22
    #else
        #define LV_FONT_MONTSERRAT_22 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_24
    #ifdef CONFIG_LV_FONT_MONTSERRAT_24
        #define LV_FONT_MONTSERRAT_24 CONFIG_LV_FONT_MONTSERRAT_24
    #else
        #define LV_FONT_MONTSERRAT_24 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_26
    #ifdef CONFIG_LV_FONT_MONTSERRAT_26
        #define LV_FONT_MONTSERRAT_26 CONFIG_LV_FONT_MONTSERRAT_26
    #else
        #define LV_FONT_MONTSERRAT_26 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_28
    #ifdef CONFIG_LV_FONT_MONTSERRAT_28
        #define LV_FONT_MONTSERRAT_28 CONFIG_LV_FONT_MONTSERRAT_28
    #else
        #define LV_FONT_MONTSERRAT_28 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_30
    #ifdef CONFIG_LV_FONT_MONTSERRAT_30
        #define LV_FONT_MONTSERRAT_30 CONFIG_LV_FONT_MONTSERRAT_30
    #else
        #define LV_FONT_MONTSERRAT_30 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_32
    #ifdef CONFIG_LV_FONT_MONTSERRAT_32
        #define LV_FONT_MONTSERRAT_32 CONFIG_LV_FONT_MONTSERRAT_32
    #else
        #define LV_FONT_MONTSERRAT_32 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_34
    #ifdef CONFIG_LV_FONT_MONTSERRAT_34
        #define LV_FONT_MONTSERRAT_34 CONFIG_LV_FONT_MONTSERRAT_34
    #else
        #define LV_FONT_MONTSERRAT_34 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_36
    #ifdef CONFIG_LV_FONT_MONTSERRAT_36
        #define LV_FONT_MONTSERRAT_36 CONFIG_LV_FONT_MONTSERRAT_36
    #else
        #define LV_FONT_MONTSERRAT_36 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_38
    #ifdef CONFIG_LV_FONT_MONTSERRAT_38
        #define LV_FONT_MONTSERRAT_38 CONFIG_LV_FONT_MONTSERRAT_38
    #else
        #define LV_FONT_MONTSERRAT_38 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_40
    #ifdef CONFIG_LV_FONT_MONTSERRAT_40
        #define LV_FONT_MONTSERRAT_40 CONFIG_LV_FONT_MONTSERRAT_40
    #else
        #define LV_FONT_MONTSERRAT_40 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_42
    #ifdef CONFIG_LV_FONT_MONTSERRAT_42
        #define LV_FONT_MONTSERRAT_42 CONFIG_LV_FONT_MONTSERRAT_42
    #else
        #define LV_FONT_MONTSERRAT_42 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_44
    #ifdef CONFIG_LV_FONT_MONTSERRAT_44
        #define LV_FONT_MONTSERRAT_44 CONFIG_LV_FONT_MONTSERRAT_44
    #else
        #define LV_FONT_MONTSERRAT_44 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_46
    #ifdef CONFIG_LV_FONT_MONTSERRAT_46
        #define LV_FONT_MONTSERRAT_46 CONFIG_LV_FONT_MONTSERRAT_46
    #else
        #define LV_FONT_MONTSERRAT_46 0
    #endif
#endif
#ifndef LV_FONT_MONTSERRAT_48
    #ifdef CONFIG_LV_FONT_MONTSERRAT_48
        #define LV_FONT_MONTSERRAT_48 CONFIG_LV_FONT_MONTSERRAT_48
    #else
        #define LV_FONT_MONTSERRAT_48 0
    #endif
#endif

/*Demonstrate special features*/
#ifndef LV_FONT_MONTSERRAT_28_COMPRESSED
    #ifdef CONFIG_LV_FONT_MONTSERRAT_28_COMPRESSED
        #define LV_FONT_MONTSERRAT_28_COMPRESSED CONFIG_LV_FONT_MONTSERRAT_28_COMPRESSED
    #else
        #define LV_FONT_MONTSERRAT_28_COMPRESSED 0  /*bpp = 3*/
    #endif
#endif
#ifndef LV_FONT_DEJAVU_16_PERSIAN_HEBREW
    #ifdef CONFIG_LV_FONT_DEJAVU_16_PERSIAN_HEBREW
        #define LV_FONT_DEJAVU_16_PERSIAN_HEBREW CONFIG_LV_FONT_DEJAVU_16_PERSIAN_HEBREW
    #else
        #define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0  /*Hebrew, Arabic, Persian letters and all their forms*/
    #endif
#endif
#ifndef LV_FONT_SIMSUN_16_CJK
    #ifdef CONFIG_LV_FONT_SIMSUN_16_CJK
        #define LV_FONT_SIMSUN_16_CJK CONFIG_LV_FONT_SIMSUN_16_CJK
    #else
        #define LV_FONT_SIMSUN_16_CJK            0  /*1000 most common CJK radicals*/
    #endif
#endif

/*Pixel perfect monospace fonts*/
#ifndef LV_FONT_UNSCII_8
    #ifdef CONFIG_LV_FONT_UNSCII_8
        #define LV_FONT_UNSCII_8 CONFIG_LV_FONT_UNSCII_8
    #else
        #define LV_FONT_UNSCII_8  0
    #endif
#endif
#ifndef LV_FONT_UNSCII_16
    #ifdef CONFIG_LV_FONT_UNSCII_16
        #define LV_FONT_UNSCII_16 CONFIG_LV_FONT_UNSCII_16
    #else
        #define LV_FONT_UNSCII_16 0
    #endif
#endif

/*Optionally declare custom fonts here.
 *You can use these fonts as default font too and they will be available globally.
 *E.g. #define LV_FONT_CUSTOM_DECLARE   LV_FONT_DECLARE(my_font_1) LV_FONT_DECLARE(my_font_2)*/
#ifndef LV_FONT_CUSTOM_DECLARE
    #ifdef CONFIG_LV_FONT_CUSTOM_DECLARE
        #define LV_FONT_CUSTOM_DECLARE CONFIG_LV_FONT_CUSTOM_DECLARE
    #else
        #define LV_FONT_CUSTOM_DECLARE
    #endif
#endif

/*Always set a default font*/
#ifndef LV_FONT_DEFAULT
    #ifdef CONFIG_LV_FONT_DEFAULT
        #define LV_FONT_DEFAULT CONFIG_LV_FONT_DEFAULT
    #else
        #define LV_FONT_DEFAULT &lv_font_montserrat_14
    #endif
#endif

/*Enable handling large font and/or fonts with a lot of characters.
 *The limit depends on the font size, font face and bpp.
 *Compiler error will be triggered if a font needs it.*/
#ifndef LV_FONT_FMT_TXT_LARGE
    #ifdef CONFIG_LV_FONT_FMT_TXT_LARGE
        #define LV_FONT_FMT_TXT_LARGE CONFIG_LV_FONT_FMT_TXT_LARGE
    #else
        #define LV_FONT_FMT_TXT_LARGE 0
    #endif
#endif

/*Enables/disables support for compressed fonts.*/
#ifndef LV_USE_FONT_COMPRESSED
    #ifdef CONFIG_LV_USE_FONT_COMPRESSED
        #define LV_USE_FONT_COMPRESSED CONFIG_LV_USE_FONT_COMPRESSED
    #else
        #define LV_USE_FONT_COMPRESSED 0
    #endif
#endif

/*Enable drawing placeholders when glyph dsc is not found*/
#ifndef LV_USE_FONT_PLACEHOLDER
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_FONT_PLACEHOLDER
            #define LV_USE_FONT_PLACEHOLDER CONFIG_LV_USE_FONT_PLACEHOLDER
        #else
            #define LV_USE_FONT_PLACEHOLDER 0
        #endif
    #else
        #define LV_USE_FONT_PLACEHOLDER 1
    #endif
#endif

/*=================
 *  TEXT SETTINGS
 *=================*/

/**
 * Select a character encoding for strings.
 * Your IDE or editor should have the same character encoding
 * - LV_TXT_ENC_UTF8
 * - LV_TXT_ENC_ASCII
 */
#ifndef LV_TXT_ENC
    #ifdef CONFIG_LV_TXT_ENC
        #define LV_TXT_ENC CONFIG_LV_TXT_ENC
    #else
        #define LV_TXT_ENC LV_TXT_ENC_UTF8
    #endif
#endif

/*Can break (wrap) texts on these chars*/
#ifndef LV_TXT_BREAK_CHARS
    #ifdef CONFIG_LV_TXT_BREAK_CHARS
        #define LV_TXT_BREAK_CHARS CONFIG_LV_TXT_BREAK_CHARS
    #else
        #define LV_TXT_BREAK_CHARS " ,.;:-_)]}"
    #endif
#endif

/*If a word is at least this long, will break wherever "prettiest"
 *To disable, set to a value <= 0*/
#ifndef LV_TXT_LINE_BREAK_LONG_LEN
    #ifdef CONFIG_LV_TXT_LINE_BREAK_LONG_LEN
        #define LV_TXT_LINE_BREAK_LONG_LEN CONFIG_LV_TXT_LINE_BREAK_LONG_LEN
    #else
        #define LV_TXT_LINE_BREAK_LONG_LEN 0
    #endif
#endif

/*Minimum number of characters in a long word to put on a line before a break.
 *Depends on LV_TXT_LINE_BREAK_LONG_LEN.*/
#ifndef LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN
    #ifdef CONFIG_LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN
        #define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN CONFIG_LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN
    #else
        #define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3
    #endif
#endif

/*Minimum number of characters in a long word to put on a line after a break.
 *Depends on LV_TXT_LINE_BREAK_LONG_LEN.*/
#ifndef LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN
    #ifdef CONFIG_LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN
        #define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN CONFIG_LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN
    #else
        #define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3
    #endif
#endif

/*Support bidirectional texts. Allows mixing Left-to-Right and Right-to-Left texts.
 *The direction will be processed according to the Unicode Bidirectional Algorithm:
 *https://www.w3.org/International/articles/inline-bidi-markup/uba-basics*/
#ifndef LV_USE_BIDI
    #ifdef CONFIG_LV_USE_BIDI
        #define LV_USE_BIDI CONFIG_LV_USE_BIDI
    #else
        #define LV_USE_BIDI 0
    #endif
#endif
#if LV_USE_BIDI
    /*Set the default direction. Supported values:
    *`LV_BASE_DIR_LTR` Left-to-Right
    *`LV_BASE_DIR_RTL` Right-to-Left
    *`LV_BASE_DIR_AUTO` detect texts base direction*/
    #ifndef LV_BIDI_BASE_DIR_DEF
        #ifdef CONFIG_LV_BIDI_BASE_DIR_DEF
            #define LV_BIDI_BASE_DIR_DEF CONFIG_LV_BIDI_BASE_DIR_DEF
        #else
            #define LV_BIDI_BASE_DIR_DEF LV_BASE_DIR_AUTO
        #endif
    #endif
#endif

/*Enable Arabic/Persian processing
 *In these languages characters should be replaced with an other form based on their position in the text*/
#ifndef LV_USE_ARABIC_PERSIAN_CHARS
    #ifdef CONFIG_LV_USE_ARABIC_PERSIAN_CHARS
        #define LV_USE_ARABIC_PERSIAN_CHARS CONFIG_LV_USE_ARABIC_PERSIAN_CHARS
    #else
        #define LV_USE_ARABIC_PERSIAN_CHARS 0
    #endif
#endif

/*==================
 * WIDGETS
 *================*/

/*Documentation of the widgets: https://docs.lvgl.io/latest/en/html/widgets/index.html*/

#ifndef LV_WIDGETS_HAS_DEFAULT_VALUE
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_WIDGETS_HAS_DEFAULT_VALUE
            #define LV_WIDGETS_HAS_DEFAULT_VALUE CONFIG_LV_WIDGETS_HAS_DEFAULT_VALUE
        #else
            #define LV_WIDGETS_HAS_DEFAULT_VALUE 0
        #endif
    #else
        #define LV_WIDGETS_HAS_DEFAULT_VALUE  1
    #endif
#endif

#ifndef LV_USE_ANIMIMG
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_ANIMIMG
            #define LV_USE_ANIMIMG CONFIG_LV_USE_ANIMIMG
        #else
            #define LV_USE_ANIMIMG 0
        #endif
    #else
        #define LV_USE_ANIMIMG    1
    #endif
#endif

#ifndef LV_USE_ARC
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_ARC
            #define LV_USE_ARC CONFIG_LV_USE_ARC
        #else
            #define LV_USE_ARC 0
        #endif
    #else
        #define LV_USE_ARC        1
    #endif
#endif

#ifndef LV_USE_BAR
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_BAR
            #define LV_USE_BAR CONFIG_LV_USE_BAR
        #else
            #define LV_USE_BAR 0
        #endif
    #else
        #define LV_USE_BAR        1
    #endif
#endif

#ifndef LV_USE_BUTTON
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_BUTTON
            #define LV_USE_BUTTON CONFIG_LV_USE_BUTTON
        #else
            #define LV_USE_BUTTON 0
        #endif
    #else
        #define LV_USE_BUTTON        1
    #endif
#endif

#ifndef LV_USE_BUTTONMATRIX
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_BUTTONMATRIX
            #define LV_USE_BUTTONMATRIX CONFIG_LV_USE_BUTTONMATRIX
        #else
            #define LV_USE_BUTTONMATRIX 0
        #endif
    #else
        #define LV_USE_BUTTONMATRIX  1
    #endif
#endif

#ifndef LV_USE_CALENDAR
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_CALENDAR
            #define LV_USE_CALENDAR CONFIG_LV_USE_CALENDAR
        #else
            #define LV_USE_CALENDAR 0
        #endif
    #else
        #define LV_USE_CALENDAR   1
    #endif
#endif
#if LV_USE_CALENDAR
    #ifndef LV_CALENDAR_WEEK_STARTS_MONDAY
        #ifdef CONFIG_LV_CALENDAR_WEEK_STARTS_MONDAY
            #define LV_CALENDAR_WEEK_STARTS_MONDAY CONFIG_LV_CALENDAR_WEEK_STARTS_MONDAY
        #else
            #define LV_CALENDAR_WEEK_STARTS_MONDAY 0
        #endif
    #endif
    #if LV_CALENDAR_WEEK_STARTS_MONDAY
        #ifndef LV_CALENDAR_DEFAULT_DAY_NAMES
            #ifdef CONFIG_LV_CALENDAR_DEFAULT_DAY_NAMES
                #define LV_CALENDAR_DEFAULT_DAY_NAMES CONFIG_LV_CALENDAR_DEFAULT_DAY_NAMES
            #else
                #define LV_CALENDAR_DEFAULT_DAY_NAMES {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"}
            #endif
        #endif
    #else
        #ifndef LV_CALENDAR_DEFAULT_DAY_NAMES
            #ifdef CONFIG_LV_CALENDAR_DEFAULT_DAY_NAMES
                #define LV_CALENDAR_DEFAULT_DAY_NAMES CONFIG_LV_CALENDAR_DEFAULT_DAY_NAMES
            #else
                #define LV_CALENDAR_DEFAULT_DAY_NAMES {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"}
            #endif
        #endif
    #endif

    #ifndef LV_CALENDAR_DEFAULT_MONTH_NAMES
        #ifdef CONFIG_LV_CALENDAR_DEFAULT_MONTH_NAMES
            #define LV_CALENDAR_DEFAULT_MONTH_NAMES CONFIG_LV_CALENDAR_DEFAULT_MONTH_NAMES
        #else
            #define LV_CALENDAR_DEFAULT_MONTH_NAMES {"January", "February", "March",  "April", "May",  "June", "July", "August", "September", "October", "November", "December"}
        #endif
    #endif
    #ifndef LV_USE_CALENDAR_HEADER_ARROW
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_USE_CALENDAR_HEADER_ARROW
                #define LV_USE_CALENDAR_HEADER_ARROW CONFIG_LV_USE_CALENDAR_HEADER_ARROW
            #else
                #define LV_USE_CALENDAR_HEADER_ARROW 0
            #endif
        #else
            #define LV_USE_CALENDAR_HEADER_ARROW 1
        #endif
    #endif
    #ifndef LV_USE_CALENDAR_HEADER_DROPDOWN
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_USE_CALENDAR_HEADER_DROPDOWN
                #define LV_USE_CALENDAR_HEADER_DROPDOWN CONFIG_LV_USE_CALENDAR_HEADER_DROPDOWN
            #else
                #define LV_USE_CALENDAR_HEADER_DROPDOWN 0
            #endif
        #else
            #define LV_USE_CALENDAR_HEADER_DROPDOWN 1
        #endif
    #endif
#endif  /*LV_USE_CALENDAR*/

#ifndef LV_USE_CANVAS
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_CANVAS
            #define LV_USE_CANVAS CONFIG_LV_USE_CANVAS
        #else
            #define LV_USE_CANVAS 0
        #endif
    #else
        #define LV_USE_CANVAS     1
    #endif
#endif

#ifndef LV_USE_CHART
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_CHART
            #define LV_USE_CHART CONFIG_LV_USE_CHART
        #else
            #define LV_USE_CHART 0
        #endif
    #else
        #define LV_USE_CHART      1
    #endif
#endif

#ifndef LV_USE_CHECKBOX
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_CHECKBOX
            #define LV_USE_CHECKBOX CONFIG_LV_USE_CHECKBOX
        #else
            #define LV_USE_CHECKBOX 0
        #endif
    #else
        #define LV_USE_CHECKBOX   1
    #endif
#endif

#ifndef LV_USE_DROPDOWN
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_DROPDOWN
            #define LV_USE_DROPDOWN CONFIG_LV_USE_DROPDOWN
        #else
            #define LV_USE_DROPDOWN 0
        #endif
    #else
        #define LV_USE_DROPDOWN   1   /*Requires: lv_label*/
    #endif
#endif

#ifndef LV_USE_IMAGE
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_IMAGE
            #define LV_USE_IMAGE CONFIG_LV_USE_IMAGE
        #else
            #define LV_USE_IMAGE 0
        #endif
    #else
        #define LV_USE_IMAGE      1   /*Requires: lv_label*/
    #endif
#endif

#ifndef LV_USE_IMAGEBUTTON
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_IMAGEBUTTON
            #define LV_USE_IMAGEBUTTON CONFIG_LV_USE_IMAGEBUTTON
        #else
            #define LV_USE_IMAGEBUTTON 0
        #endif
    #else
        #define LV_USE_IMAGEBUTTON     1
    #endif
#endif

#ifndef LV_USE_KEYBOARD
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_KEYBOARD
            #define LV_USE_KEYBOARD CONFIG_LV_USE_KEYBOARD
        #else
            #define LV_USE_KEYBOARD 0
        #endif
    #else
        #define LV_USE_KEYBOARD   1
    #endif
#endif

#ifndef LV_USE_LABEL
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_LABEL
            #define LV_USE_LABEL CONFIG_LV_USE_LABEL
        #else
            #define LV_USE_LABEL 0
        #endif
    #else
        #define LV_USE_LABEL      1
    #endif
#endif
#if LV_USE_LABEL
    #ifndef LV_LABEL_TEXT_SELECTION
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_LABEL_TEXT_SELECTION
                #define LV_LABEL_TEXT_SELECTION CONFIG_LV_LABEL_TEXT_SELECTION
            #else
                #define LV_LABEL_TEXT_SELECTION 0
            #endif
        #else
            #define LV_LABEL_TEXT_SELECTION 1 /*Enable selecting text of the label*/
        #endif
    #endif
    #ifndef LV_LABEL_LONG_TXT_HINT
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_LABEL_LONG_TXT_HINT
                #define LV_LABEL_LONG_TXT_HINT CONFIG_LV_LABEL_LONG_TXT_HINT
            #else
                #define LV_LABEL_LONG_TXT_HINT 0
            #endif
        #else
            #define LV_LABEL_LONG_TXT_HINT 1  /*Store some extra info in labels to speed up drawing of very long texts*/
        #endif
    #endif
    #ifndef LV_LABEL_WAIT_CHAR_COUNT
        #ifdef CONFIG_LV_LABEL_WAIT_CHAR_COUNT
            #define LV_LABEL_WAIT_CHAR_COUNT CONFIG_LV_LABEL_WAIT_CHAR_COUNT
        #else
            #define LV_LABEL_WAIT_CHAR_COUNT 3  /*The count of wait chart*/
        #endif
    #endif
#endif

#ifndef LV_USE_LED
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_LED
            #define LV_USE_LED CONFIG_LV_USE_LED
        #else
            #define LV_USE_LED 0
        #endif
    #else
        #define LV_USE_LED        1
    #endif
#endif

#ifndef LV_USE_LINE
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_LINE
            #define LV_USE_LINE CONFIG_LV_USE_LINE
        #else
            #define LV_USE_LINE 0
        #endif
    #else
        #define LV_USE_LINE       1
    #endif
#endif

#ifndef LV_USE_LIST
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_LIST
            #define LV_USE_LIST CONFIG_LV_USE_LIST
        #else
            #define LV_USE_LIST 0
        #endif
    #else
        #define LV_USE_LIST       1
    #endif
#endif

#ifndef LV_USE_MENU
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_MENU
            #define LV_USE_MENU CONFIG_LV_USE_MENU
        #else
            #define LV_USE_MENU 0
        #endif
    #else
        #define LV_USE_MENU       1
    #endif
#endif

#ifndef LV_USE_MSGBOX
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_MSGBOX
            #define LV_USE_MSGBOX CONFIG_LV_USE_MSGBOX
        #else
            #define LV_USE_MSGBOX 0
        #endif
    #else
        #define LV_USE_MSGBOX     1
    #endif
#endif

#ifndef LV_USE_ROLLER
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_ROLLER
            #define LV_USE_ROLLER CONFIG_LV_USE_ROLLER
        #else
            #define LV_USE_ROLLER 0
        #endif
    #else
        #define LV_USE_ROLLER     1   /*Requires: lv_label*/
    #endif
#endif

#ifndef LV_USE_SCALE
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_SCALE
            #define LV_USE_SCALE CONFIG_LV_USE_SCALE
        #else
            #define LV_USE_SCALE 0
        #endif
    #else
        #define LV_USE_SCALE      1
    #endif
#endif

#ifndef LV_USE_SLIDER
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_SLIDER
            #define LV_USE_SLIDER CONFIG_LV_USE_SLIDER
        #else
            #define LV_USE_SLIDER 0
        #endif
    #else
        #define LV_USE_SLIDER     1   /*Requires: lv_bar*/
    #endif
#endif

#ifndef LV_USE_SPAN
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_SPAN
            #define LV_USE_SPAN CONFIG_LV_USE_SPAN
        #else
            #define LV_USE_SPAN 0
        #endif
    #else
        #define LV_USE_SPAN       1
    #endif
#endif
#if LV_USE_SPAN
    /*A line text can contain maximum num of span descriptor */
    #ifndef LV_SPAN_SNIPPET_STACK_SIZE
        #ifdef CONFIG_LV_SPAN_SNIPPET_STACK_SIZE
            #define LV_SPAN_SNIPPET_STACK_SIZE CONFIG_LV_SPAN_SNIPPET_STACK_SIZE
        #else
            #define LV_SPAN_SNIPPET_STACK_SIZE 64
        #endif
    #endif
#endif

#ifndef LV_USE_SPINBOX
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_SPINBOX
            #define LV_USE_SPINBOX CONFIG_LV_USE_SPINBOX
        #else
            #define LV_USE_SPINBOX 0
        #endif
    #else
        #define LV_USE_SPINBOX    1
    #endif
#endif

#ifndef LV_USE_SPINNER
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_SPINNER
            #define LV_USE_SPINNER CONFIG_LV_USE_SPINNER
        #else
            #define LV_USE_SPINNER 0
        #endif
    #else
        #define LV_USE_SPINNER    1
    #endif
#endif

#ifndef LV_USE_SWITCH
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_SWITCH
            #define LV_USE_SWITCH CONFIG_LV_USE_SWITCH
        #else
            #define LV_USE_SWITCH 0
        #endif
    #else
        #define LV_USE_SWITCH     1
    #endif
#endif

#ifndef LV_USE_TEXTAREA
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_TEXTAREA
            #define LV_USE_TEXTAREA CONFIG_LV_USE_TEXTAREA
        #else
            #define LV_USE_TEXTAREA 0
        #endif
    #else
        #define LV_USE_TEXTAREA   1   /*Requires: lv_label*/
    #endif
#endif
#if LV_USE_TEXTAREA != 0
    #ifndef LV_TEXTAREA_DEF_PWD_SHOW_TIME
        #ifdef CONFIG_LV_TEXTAREA_DEF_PWD_SHOW_TIME
            #define LV_TEXTAREA_DEF_PWD_SHOW_TIME CONFIG_LV_TEXTAREA_DEF_PWD_SHOW_TIME
        #else
            #define LV_TEXTAREA_DEF_PWD_SHOW_TIME 1500    /*ms*/
        #endif
    #endif
#endif

#ifndef LV_USE_TABLE
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_TABLE
            #define LV_USE_TABLE CONFIG_LV_USE_TABLE
        #else
            #define LV_USE_TABLE 0
        #endif
    #else
        #define LV_USE_TABLE      1
    #endif
#endif

#ifndef LV_USE_TABVIEW
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_TABVIEW
            #define LV_USE_TABVIEW CONFIG_LV_USE_TABVIEW
        #else
            #define LV_USE_TABVIEW 0
        #endif
    #else
        #define LV_USE_TABVIEW    1
    #endif
#endif

#ifndef LV_USE_TILEVIEW
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_TILEVIEW
            #define LV_USE_TILEVIEW CONFIG_LV_USE_TILEVIEW
        #else
            #define LV_USE_TILEVIEW 0
        #endif
    #else
        #define LV_USE_TILEVIEW   1
    #endif
#endif

#ifndef LV_USE_WIN
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_WIN
            #define LV_USE_WIN CONFIG_LV_USE_WIN
        #else
            #define LV_USE_WIN 0
        #endif
    #else
        #define LV_USE_WIN        1
    #endif
#endif

/*==================
 * THEMES
 *==================*/

/*A simple, impressive and very complete theme*/
#ifndef LV_USE_THEME_DEFAULT
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_THEME_DEFAULT
            #define LV_USE_THEME_DEFAULT CONFIG_LV_USE_THEME_DEFAULT
        #else
            #define LV_USE_THEME_DEFAULT 0
        #endif
    #else
        #define LV_USE_THEME_DEFAULT 1
    #endif
#endif
#if LV_USE_THEME_DEFAULT

    /*0: Light mode; 1: Dark mode*/
    #ifndef LV_THEME_DEFAULT_DARK
        #ifdef CONFIG_LV_THEME_DEFAULT_DARK
            #define LV_THEME_DEFAULT_DARK CONFIG_LV_THEME_DEFAULT_DARK
        #else
            #define LV_THEME_DEFAULT_DARK 0
        #endif
    #endif

    /*1: Enable grow on press*/
    #ifndef LV_THEME_DEFAULT_GROW
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_THEME_DEFAULT_GROW
                #define LV_THEME_DEFAULT_GROW CONFIG_LV_THEME_DEFAULT_GROW
            #else
                #define LV_THEME_DEFAULT_GROW 0
            #endif
        #else
            #define LV_THEME_DEFAULT_GROW 1
        #endif
    #endif

    /*Default transition time in [ms]*/
    #ifndef LV_THEME_DEFAULT_TRANSITION_TIME
        #ifdef CONFIG_LV_THEME_DEFAULT_TRANSITION_TIME
            #define LV_THEME_DEFAULT_TRANSITION_TIME CONFIG_LV_THEME_DEFAULT_TRANSITION_TIME
        #else
            #define LV_THEME_DEFAULT_TRANSITION_TIME 80
        #endif
    #endif
#endif /*LV_USE_THEME_DEFAULT*/

/*A very simple theme that is a good starting point for a custom theme*/
#ifndef LV_USE_THEME_SIMPLE
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_THEME_SIMPLE
            #define LV_USE_THEME_SIMPLE CONFIG_LV_USE_THEME_SIMPLE
        #else
            #define LV_USE_THEME_SIMPLE 0
        #endif
    #else
        #define LV_USE_THEME_SIMPLE 1
    #endif
#endif

/*A theme designed for monochrome displays*/
#ifndef LV_USE_THEME_MONO
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_THEME_MONO
            #define LV_USE_THEME_MONO CONFIG_LV_USE_THEME_MONO
        #else
            #define LV_USE_THEME_MONO 0
        #endif
    #else
        #define LV_USE_THEME_MONO 1
    #endif
#endif

/*==================
 * LAYOUTS
 *==================*/

/*A layout similar to Flexbox in CSS.*/
#ifndef LV_USE_FLEX
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_FLEX
            #define LV_USE_FLEX CONFIG_LV_USE_FLEX
        #else
            #define LV_USE_FLEX 0
        #endif
    #else
        #define LV_USE_FLEX 1
    #endif
#endif

/*A layout similar to Grid in CSS.*/
#ifndef LV_USE_GRID
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_GRID
            #define LV_USE_GRID CONFIG_LV_USE_GRID
        #else
            #define LV_USE_GRID 0
        #endif
    #else
        #define LV_USE_GRID 1
    #endif
#endif

/*====================
 * 3RD PARTS LIBRARIES
 *====================*/

/*File system interfaces for common APIs */

/*API for fopen, fread, etc*/
#ifndef LV_USE_FS_STDIO
    #ifdef CONFIG_LV_USE_FS_STDIO
        #define LV_USE_FS_STDIO CONFIG_LV_USE_FS_STDIO
    #else
        #define LV_USE_FS_STDIO 0
    #endif
#endif
#if LV_USE_FS_STDIO
    #ifndef LV_FS_STDIO_LETTER
        #ifdef CONFIG_LV_FS_STDIO_LETTER
            #define LV_FS_STDIO_LETTER CONFIG_LV_FS_STDIO_LETTER
        #else
            #define LV_FS_STDIO_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
        #endif
    #endif
    #ifndef LV_FS_STDIO_PATH
        #ifdef CONFIG_LV_FS_STDIO_PATH
            #define LV_FS_STDIO_PATH CONFIG_LV_FS_STDIO_PATH
        #else
            #define LV_FS_STDIO_PATH ""         /*Set the working directory. File/directory paths will be appended to it.*/
        #endif
    #endif
    #ifndef LV_FS_STDIO_CACHE_SIZE
        #ifdef CONFIG_LV_FS_STDIO_CACHE_SIZE
            #define LV_FS_STDIO_CACHE_SIZE CONFIG_LV_FS_STDIO_CACHE_SIZE
        #else
            #define LV_FS_STDIO_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
        #endif
    #endif
#endif

/*API for open, read, etc*/
#ifndef LV_USE_FS_POSIX
    #ifdef CONFIG_LV_USE_FS_POSIX
        #define LV_USE_FS_POSIX CONFIG_LV_USE_FS_POSIX
    #else
        #define LV_USE_FS_POSIX 0
    #endif
#endif
#if LV_USE_FS_POSIX
    #ifndef LV_FS_POSIX_LETTER
        #ifdef CONFIG_LV_FS_POSIX_LETTER
            #define LV_FS_POSIX_LETTER CONFIG_LV_FS_POSIX_LETTER
        #else
            #define LV_FS_POSIX_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
        #endif
    #endif
    #ifndef LV_FS_POSIX_PATH
        #ifdef CONFIG_LV_FS_POSIX_PATH
            #define LV_FS_POSIX_PATH CONFIG_LV_FS_POSIX_PATH
        #else
            #define LV_FS_POSIX_PATH ""         /*Set the working directory. File/directory paths will be appended to it.*/
        #endif
    #endif
    #ifndef LV_FS_POSIX_CACHE_SIZE
        #ifdef CONFIG_LV_FS_POSIX_CACHE_SIZE
            #define LV_FS_POSIX_CACHE_SIZE CONFIG_LV_FS_POSIX_CACHE_SIZE
        #else
            #define LV_FS_POSIX_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
        #endif
    #endif
#endif

/*API for CreateFile, ReadFile, etc*/
#ifndef LV_USE_FS_WIN32
    #ifdef CONFIG_LV_USE_FS_WIN32
        #define LV_USE_FS_WIN32 CONFIG_LV_USE_FS_WIN32
    #else
        #define LV_USE_FS_WIN32 0
    #endif
#endif
#if LV_USE_FS_WIN32
    #ifndef LV_FS_WIN32_LETTER
        #ifdef CONFIG_LV_FS_WIN32_LETTER
            #define LV_FS_WIN32_LETTER CONFIG_LV_FS_WIN32_LETTER
        #else
            #define LV_FS_WIN32_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
        #endif
    #endif
    #ifndef LV_FS_WIN32_PATH
        #ifdef CONFIG_LV_FS_WIN32_PATH
            #define LV_FS_WIN32_PATH CONFIG_LV_FS_WIN32_PATH
        #else
            #define LV_FS_WIN32_PATH ""         /*Set the working directory. File/directory paths will be appended to it.*/
        #endif
    #endif
    #ifndef LV_FS_WIN32_CACHE_SIZE
        #ifdef CONFIG_LV_FS_WIN32_CACHE_SIZE
            #define LV_FS_WIN32_CACHE_SIZE CONFIG_LV_FS_WIN32_CACHE_SIZE
        #else
            #define LV_FS_WIN32_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
        #endif
    #endif
#endif

/*API for FATFS (needs to be added separately). Uses f_open, f_read, etc*/
#ifndef LV_USE_FS_FATFS
    #ifdef CONFIG_LV_USE_FS_FATFS
        #define LV_USE_FS_FATFS CONFIG_LV_USE_FS_FATFS
    #else
        #define LV_USE_FS_FATFS 0
    #endif
#endif
#if LV_USE_FS_FATFS
    #ifndef LV_FS_FATFS_LETTER
        #ifdef CONFIG_LV_FS_FATFS_LETTER
            #define LV_FS_FATFS_LETTER CONFIG_LV_FS_FATFS_LETTER
        #else
            #define LV_FS_FATFS_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
        #endif
    #endif
    #ifndef LV_FS_FATFS_CACHE_SIZE
        #ifdef CONFIG_LV_FS_FATFS_CACHE_SIZE
            #define LV_FS_FATFS_CACHE_SIZE CONFIG_LV_FS_FATFS_CACHE_SIZE
        #else
            #define LV_FS_FATFS_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
        #endif
    #endif
#endif

/*API for memory-mapped file access. */
#ifndef LV_USE_FS_MEMFS
    #ifdef CONFIG_LV_USE_FS_MEMFS
        #define LV_USE_FS_MEMFS CONFIG_LV_USE_FS_MEMFS
    #else
        #define LV_USE_FS_MEMFS 0
    #endif
#endif
#if LV_USE_FS_MEMFS
    #ifndef LV_FS_MEMFS_LETTER
        #ifdef CONFIG_LV_FS_MEMFS_LETTER
            #define LV_FS_MEMFS_LETTER CONFIG_LV_FS_MEMFS_LETTER
        #else
            #define LV_FS_MEMFS_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
        #endif
    #endif
#endif

/*LODEPNG decoder library*/
#ifndef LV_USE_LODEPNG
    #ifdef CONFIG_LV_USE_LODEPNG
        #define LV_USE_LODEPNG CONFIG_LV_USE_LODEPNG
    #else
        #define LV_USE_LODEPNG 0
    #endif
#endif

/*PNG decoder(libpng) library*/
#ifndef LV_USE_LIBPNG
    #ifdef CONFIG_LV_USE_LIBPNG
        #define LV_USE_LIBPNG CONFIG_LV_USE_LIBPNG
    #else
        #define LV_USE_LIBPNG 0
    #endif
#endif

/*BMP decoder library*/
#ifndef LV_USE_BMP
    #ifdef CONFIG_LV_USE_BMP
        #define LV_USE_BMP CONFIG_LV_USE_BMP
    #else
        #define LV_USE_BMP 0
    #endif
#endif

/* JPG + split JPG decoder library.
 * Split JPG is a custom format optimized for embedded systems. */
#ifndef LV_USE_TJPGD
    #ifdef CONFIG_LV_USE_TJPGD
        #define LV_USE_TJPGD CONFIG_LV_USE_TJPGD
    #else
        #define LV_USE_TJPGD 0
    #endif
#endif

/* libjpeg-turbo decoder library.
 * Supports complete JPEG specifications and high-performance JPEG decoding. */
#ifndef LV_USE_LIBJPEG_TURBO
    #ifdef CONFIG_LV_USE_LIBJPEG_TURBO
        #define LV_USE_LIBJPEG_TURBO CONFIG_LV_USE_LIBJPEG_TURBO
    #else
        #define LV_USE_LIBJPEG_TURBO 0
    #endif
#endif

/*GIF decoder library*/
#ifndef LV_USE_GIF
    #ifdef CONFIG_LV_USE_GIF
        #define LV_USE_GIF CONFIG_LV_USE_GIF
    #else
        #define LV_USE_GIF 0
    #endif
#endif
#if LV_USE_GIF
/*GIF decoder accelerate*/
#ifndef LV_GIF_CACHE_DECODE_DATA
    #ifdef CONFIG_LV_GIF_CACHE_DECODE_DATA
        #define LV_GIF_CACHE_DECODE_DATA CONFIG_LV_GIF_CACHE_DECODE_DATA
    #else
        #define LV_GIF_CACHE_DECODE_DATA 0
    #endif
#endif
#endif


/*Decode bin images to RAM*/
#ifndef LV_BIN_DECODER_RAM_LOAD
    #ifdef CONFIG_LV_BIN_DECODER_RAM_LOAD
        #define LV_BIN_DECODER_RAM_LOAD CONFIG_LV_BIN_DECODER_RAM_LOAD
    #else
        #define LV_BIN_DECODER_RAM_LOAD 0
    #endif
#endif

/*RLE decompress library*/
#ifndef LV_USE_RLE
    #ifdef CONFIG_LV_USE_RLE
        #define LV_USE_RLE CONFIG_LV_USE_RLE
    #else
        #define LV_USE_RLE 0
    #endif
#endif

/*QR code library*/
#ifndef LV_USE_QRCODE
    #ifdef CONFIG_LV_USE_QRCODE
        #define LV_USE_QRCODE CONFIG_LV_USE_QRCODE
    #else
        #define LV_USE_QRCODE 0
    #endif
#endif

/*Barcode code library*/
#ifndef LV_USE_BARCODE
    #ifdef CONFIG_LV_USE_BARCODE
        #define LV_USE_BARCODE CONFIG_LV_USE_BARCODE
    #else
        #define LV_USE_BARCODE 0
    #endif
#endif

/*FreeType library*/
#ifndef LV_USE_FREETYPE
    #ifdef CONFIG_LV_USE_FREETYPE
        #define LV_USE_FREETYPE CONFIG_LV_USE_FREETYPE
    #else
        #define LV_USE_FREETYPE 0
    #endif
#endif
#if LV_USE_FREETYPE
    /*Memory used by FreeType to cache characters in kilobytes*/
    #ifndef LV_FREETYPE_CACHE_SIZE
        #ifdef CONFIG_LV_FREETYPE_CACHE_SIZE
            #define LV_FREETYPE_CACHE_SIZE CONFIG_LV_FREETYPE_CACHE_SIZE
        #else
            #define LV_FREETYPE_CACHE_SIZE 768
        #endif
    #endif

    /*Let FreeType to use LVGL memory and file porting*/
    #ifndef LV_FREETYPE_USE_LVGL_PORT
        #ifdef CONFIG_LV_FREETYPE_USE_LVGL_PORT
            #define LV_FREETYPE_USE_LVGL_PORT CONFIG_LV_FREETYPE_USE_LVGL_PORT
        #else
            #define LV_FREETYPE_USE_LVGL_PORT 0
        #endif
    #endif

    /* Maximum number of opened FT_Face/FT_Size objects managed by this cache instance. */
    /* (0:use system defaults) */
    #ifndef LV_FREETYPE_CACHE_FT_FACES
        #ifdef CONFIG_LV_FREETYPE_CACHE_FT_FACES
            #define LV_FREETYPE_CACHE_FT_FACES CONFIG_LV_FREETYPE_CACHE_FT_FACES
        #else
            #define LV_FREETYPE_CACHE_FT_FACES 8
        #endif
    #endif
    #ifndef LV_FREETYPE_CACHE_FT_SIZES
        #ifdef CONFIG_LV_FREETYPE_CACHE_FT_SIZES
            #define LV_FREETYPE_CACHE_FT_SIZES CONFIG_LV_FREETYPE_CACHE_FT_SIZES
        #else
            #define LV_FREETYPE_CACHE_FT_SIZES 8
        #endif
    #endif
    #ifndef LV_FREETYPE_CACHE_FT_GLYPH_CNT
        #ifdef CONFIG_LV_FREETYPE_CACHE_FT_GLYPH_CNT
            #define LV_FREETYPE_CACHE_FT_GLYPH_CNT CONFIG_LV_FREETYPE_CACHE_FT_GLYPH_CNT
        #else
            #define LV_FREETYPE_CACHE_FT_GLYPH_CNT 256
        #endif
    #endif
#endif

/* Built-in TTF decoder */
#ifndef LV_USE_TINY_TTF
    #ifdef CONFIG_LV_USE_TINY_TTF
        #define LV_USE_TINY_TTF CONFIG_LV_USE_TINY_TTF
    #else
        #define LV_USE_TINY_TTF 0
    #endif
#endif
#if LV_USE_TINY_TTF
    /* Enable loading TTF data from files */
    #ifndef LV_TINY_TTF_FILE_SUPPORT
        #ifdef CONFIG_LV_TINY_TTF_FILE_SUPPORT
            #define LV_TINY_TTF_FILE_SUPPORT CONFIG_LV_TINY_TTF_FILE_SUPPORT
        #else
            #define LV_TINY_TTF_FILE_SUPPORT 0
        #endif
    #endif
#endif

/*Rlottie library*/
#ifndef LV_USE_RLOTTIE
    #ifdef CONFIG_LV_USE_RLOTTIE
        #define LV_USE_RLOTTIE CONFIG_LV_USE_RLOTTIE
    #else
        #define LV_USE_RLOTTIE 0
    #endif
#endif

/*Enable Vector Graphic APIs*/
#ifndef LV_USE_VECTOR_GRAPHIC
    #ifdef CONFIG_LV_USE_VECTOR_GRAPHIC
        #define LV_USE_VECTOR_GRAPHIC CONFIG_LV_USE_VECTOR_GRAPHIC
    #else
        #define LV_USE_VECTOR_GRAPHIC  0
    #endif
#endif

/* Enable ThorVG (vector graphics library) from the src/libs folder */
#ifndef LV_USE_THORVG_INTERNAL
    #ifdef CONFIG_LV_USE_THORVG_INTERNAL
        #define LV_USE_THORVG_INTERNAL CONFIG_LV_USE_THORVG_INTERNAL
    #else
        #define LV_USE_THORVG_INTERNAL 0
    #endif
#endif

/* Enable ThorVG by assuming that its installed and linked to the project */
#ifndef LV_USE_THORVG_EXTERNAL
    #ifdef CONFIG_LV_USE_THORVG_EXTERNAL
        #define LV_USE_THORVG_EXTERNAL CONFIG_LV_USE_THORVG_EXTERNAL
    #else
        #define LV_USE_THORVG_EXTERNAL 0
    #endif
#endif

/*Enable LZ4 compress/decompress lib*/
#ifndef LV_USE_LZ4
    #ifdef CONFIG_LV_USE_LZ4
        #define LV_USE_LZ4 CONFIG_LV_USE_LZ4
    #else
        #define LV_USE_LZ4  0
    #endif
#endif

/*Use lvgl built-in LZ4 lib*/
#ifndef LV_USE_LZ4_INTERNAL
    #ifdef CONFIG_LV_USE_LZ4_INTERNAL
        #define LV_USE_LZ4_INTERNAL CONFIG_LV_USE_LZ4_INTERNAL
    #else
        #define LV_USE_LZ4_INTERNAL  0
    #endif
#endif

/*Use external LZ4 library*/
#ifndef LV_USE_LZ4_EXTERNAL
    #ifdef CONFIG_LV_USE_LZ4_EXTERNAL
        #define LV_USE_LZ4_EXTERNAL CONFIG_LV_USE_LZ4_EXTERNAL
    #else
        #define LV_USE_LZ4_EXTERNAL  0
    #endif
#endif

/*FFmpeg library for image decoding and playing videos
 *Supports all major image formats so do not enable other image decoder with it*/
#ifndef LV_USE_FFMPEG
    #ifdef CONFIG_LV_USE_FFMPEG
        #define LV_USE_FFMPEG CONFIG_LV_USE_FFMPEG
    #else
        #define LV_USE_FFMPEG 0
    #endif
#endif
#if LV_USE_FFMPEG
    /*Dump input information to stderr*/
    #ifndef LV_FFMPEG_DUMP_FORMAT
        #ifdef CONFIG_LV_FFMPEG_DUMP_FORMAT
            #define LV_FFMPEG_DUMP_FORMAT CONFIG_LV_FFMPEG_DUMP_FORMAT
        #else
            #define LV_FFMPEG_DUMP_FORMAT 0
        #endif
    #endif
#endif

/*==================
 * OTHERS
 *==================*/

/*1: Enable API to take snapshot for object*/
#ifndef LV_USE_SNAPSHOT
    #ifdef CONFIG_LV_USE_SNAPSHOT
        #define LV_USE_SNAPSHOT CONFIG_LV_USE_SNAPSHOT
    #else
        #define LV_USE_SNAPSHOT 0
    #endif
#endif

/*1: Enable system monitor component*/
#ifndef LV_USE_SYSMON
    #ifdef CONFIG_LV_USE_SYSMON
        #define LV_USE_SYSMON CONFIG_LV_USE_SYSMON
    #else
        #define LV_USE_SYSMON   0
    #endif
#endif
#if LV_USE_SYSMON
    /*Get the idle percentage. E.g. uint32_t my_get_idle(void);*/
    #ifndef LV_SYSMON_GET_IDLE
        #ifdef CONFIG_LV_SYSMON_GET_IDLE
            #define LV_SYSMON_GET_IDLE CONFIG_LV_SYSMON_GET_IDLE
        #else
            #define LV_SYSMON_GET_IDLE lv_timer_get_idle
        #endif
    #endif

    /*1: Show CPU usage and FPS count
     * Requires `LV_USE_SYSMON = 1`*/
    #ifndef LV_USE_PERF_MONITOR
        #ifdef CONFIG_LV_USE_PERF_MONITOR
            #define LV_USE_PERF_MONITOR CONFIG_LV_USE_PERF_MONITOR
        #else
            #define LV_USE_PERF_MONITOR 0
        #endif
    #endif
    #if LV_USE_PERF_MONITOR
        #ifndef LV_USE_PERF_MONITOR_POS
            #ifdef CONFIG_LV_USE_PERF_MONITOR_POS
                #define LV_USE_PERF_MONITOR_POS CONFIG_LV_USE_PERF_MONITOR_POS
            #else
                #define LV_USE_PERF_MONITOR_POS LV_ALIGN_BOTTOM_RIGHT
            #endif
        #endif

        /*0: Displays performance data on the screen, 1: Prints performance data using log.*/
        #ifndef LV_USE_PERF_MONITOR_LOG_MODE
            #ifdef CONFIG_LV_USE_PERF_MONITOR_LOG_MODE
                #define LV_USE_PERF_MONITOR_LOG_MODE CONFIG_LV_USE_PERF_MONITOR_LOG_MODE
            #else
                #define LV_USE_PERF_MONITOR_LOG_MODE 0
            #endif
        #endif
    #endif

    /*1: Show the used memory and the memory fragmentation
     * Requires `LV_USE_BUILTIN_MALLOC = 1`
     * Requires `LV_USE_SYSMON = 1`*/
    #ifndef LV_USE_MEM_MONITOR
        #ifdef CONFIG_LV_USE_MEM_MONITOR
            #define LV_USE_MEM_MONITOR CONFIG_LV_USE_MEM_MONITOR
        #else
            #define LV_USE_MEM_MONITOR 0
        #endif
    #endif
    #if LV_USE_MEM_MONITOR
        #ifndef LV_USE_MEM_MONITOR_POS
            #ifdef CONFIG_LV_USE_MEM_MONITOR_POS
                #define LV_USE_MEM_MONITOR_POS CONFIG_LV_USE_MEM_MONITOR_POS
            #else
                #define LV_USE_MEM_MONITOR_POS LV_ALIGN_BOTTOM_LEFT
            #endif
        #endif
    #endif

#endif /*LV_USE_SYSMON*/

/*1: Enable the runtime performance profiler*/
#ifndef LV_USE_PROFILER
    #ifdef CONFIG_LV_USE_PROFILER
        #define LV_USE_PROFILER CONFIG_LV_USE_PROFILER
    #else
        #define LV_USE_PROFILER 0
    #endif
#endif
#if LV_USE_PROFILER
    /*1: Enable the built-in profiler*/
    #ifndef LV_USE_PROFILER_BUILTIN
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_USE_PROFILER_BUILTIN
                #define LV_USE_PROFILER_BUILTIN CONFIG_LV_USE_PROFILER_BUILTIN
            #else
                #define LV_USE_PROFILER_BUILTIN 0
            #endif
        #else
            #define LV_USE_PROFILER_BUILTIN 1
        #endif
    #endif
    #if LV_USE_PROFILER_BUILTIN
        /*Default profiler trace buffer size*/
        #ifndef LV_PROFILER_BUILTIN_BUF_SIZE
            #ifdef CONFIG_LV_PROFILER_BUILTIN_BUF_SIZE
                #define LV_PROFILER_BUILTIN_BUF_SIZE CONFIG_LV_PROFILER_BUILTIN_BUF_SIZE
            #else
                #define LV_PROFILER_BUILTIN_BUF_SIZE (16 * 1024)     /*[bytes]*/
            #endif
        #endif
    #endif

    /*Header to include for the profiler*/
    #ifndef LV_PROFILER_INCLUDE
        #ifdef CONFIG_LV_PROFILER_INCLUDE
            #define LV_PROFILER_INCLUDE CONFIG_LV_PROFILER_INCLUDE
        #else
            #define LV_PROFILER_INCLUDE "lvgl/src/misc/lv_profiler_builtin.h"
        #endif
    #endif

    /*Profiler start point function*/
    #ifndef LV_PROFILER_BEGIN
        #ifdef CONFIG_LV_PROFILER_BEGIN
            #define LV_PROFILER_BEGIN CONFIG_LV_PROFILER_BEGIN
        #else
            #define LV_PROFILER_BEGIN    LV_PROFILER_BUILTIN_BEGIN
        #endif
    #endif

    /*Profiler end point function*/
    #ifndef LV_PROFILER_END
        #ifdef CONFIG_LV_PROFILER_END
            #define LV_PROFILER_END CONFIG_LV_PROFILER_END
        #else
            #define LV_PROFILER_END      LV_PROFILER_BUILTIN_END
        #endif
    #endif

    /*Profiler start point function with custom tag*/
    #ifndef LV_PROFILER_BEGIN_TAG
        #ifdef CONFIG_LV_PROFILER_BEGIN_TAG
            #define LV_PROFILER_BEGIN_TAG CONFIG_LV_PROFILER_BEGIN_TAG
        #else
            #define LV_PROFILER_BEGIN_TAG LV_PROFILER_BUILTIN_BEGIN_TAG
        #endif
    #endif

    /*Profiler end point function with custom tag*/
    #ifndef LV_PROFILER_END_TAG
        #ifdef CONFIG_LV_PROFILER_END_TAG
            #define LV_PROFILER_END_TAG CONFIG_LV_PROFILER_END_TAG
        #else
            #define LV_PROFILER_END_TAG   LV_PROFILER_BUILTIN_END_TAG
        #endif
    #endif
#endif

/*1: Enable Monkey test*/
#ifndef LV_USE_MONKEY
    #ifdef CONFIG_LV_USE_MONKEY
        #define LV_USE_MONKEY CONFIG_LV_USE_MONKEY
    #else
        #define LV_USE_MONKEY 0
    #endif
#endif

/*1: Enable grid navigation*/
#ifndef LV_USE_GRIDNAV
    #ifdef CONFIG_LV_USE_GRIDNAV
        #define LV_USE_GRIDNAV CONFIG_LV_USE_GRIDNAV
    #else
        #define LV_USE_GRIDNAV 0
    #endif
#endif

/*1: Enable lv_obj fragment*/
#ifndef LV_USE_FRAGMENT
    #ifdef CONFIG_LV_USE_FRAGMENT
        #define LV_USE_FRAGMENT CONFIG_LV_USE_FRAGMENT
    #else
        #define LV_USE_FRAGMENT 0
    #endif
#endif

/*1: Support using images as font in label or span widgets */
#ifndef LV_USE_IMGFONT
    #ifdef CONFIG_LV_USE_IMGFONT
        #define LV_USE_IMGFONT CONFIG_LV_USE_IMGFONT
    #else
        #define LV_USE_IMGFONT 0
    #endif
#endif

/*1: Enable an observer pattern implementation*/
#ifndef LV_USE_OBSERVER
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_USE_OBSERVER
            #define LV_USE_OBSERVER CONFIG_LV_USE_OBSERVER
        #else
            #define LV_USE_OBSERVER 0
        #endif
    #else
        #define LV_USE_OBSERVER 1
    #endif
#endif

/*1: Enable Pinyin input method*/
/*Requires: lv_keyboard*/
#ifndef LV_USE_IME_PINYIN
    #ifdef CONFIG_LV_USE_IME_PINYIN
        #define LV_USE_IME_PINYIN CONFIG_LV_USE_IME_PINYIN
    #else
        #define LV_USE_IME_PINYIN 0
    #endif
#endif
#if LV_USE_IME_PINYIN
    /*1: Use default thesaurus*/
    /*If you do not use the default thesaurus, be sure to use `lv_ime_pinyin` after setting the thesauruss*/
    #ifndef LV_IME_PINYIN_USE_DEFAULT_DICT
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_IME_PINYIN_USE_DEFAULT_DICT
                #define LV_IME_PINYIN_USE_DEFAULT_DICT CONFIG_LV_IME_PINYIN_USE_DEFAULT_DICT
            #else
                #define LV_IME_PINYIN_USE_DEFAULT_DICT 0
            #endif
        #else
            #define LV_IME_PINYIN_USE_DEFAULT_DICT 1
        #endif
    #endif
    /*Set the maximum number of candidate panels that can be displayed*/
    /*This needs to be adjusted according to the size of the screen*/
    #ifndef LV_IME_PINYIN_CAND_TEXT_NUM
        #ifdef CONFIG_LV_IME_PINYIN_CAND_TEXT_NUM
            #define LV_IME_PINYIN_CAND_TEXT_NUM CONFIG_LV_IME_PINYIN_CAND_TEXT_NUM
        #else
            #define LV_IME_PINYIN_CAND_TEXT_NUM 6
        #endif
    #endif

    /*Use 9 key input(k9)*/
    #ifndef LV_IME_PINYIN_USE_K9_MODE
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_IME_PINYIN_USE_K9_MODE
                #define LV_IME_PINYIN_USE_K9_MODE CONFIG_LV_IME_PINYIN_USE_K9_MODE
            #else
                #define LV_IME_PINYIN_USE_K9_MODE 0
            #endif
        #else
            #define LV_IME_PINYIN_USE_K9_MODE      1
        #endif
    #endif
    #if LV_IME_PINYIN_USE_K9_MODE == 1
        #ifndef LV_IME_PINYIN_K9_CAND_TEXT_NUM
            #ifdef CONFIG_LV_IME_PINYIN_K9_CAND_TEXT_NUM
                #define LV_IME_PINYIN_K9_CAND_TEXT_NUM CONFIG_LV_IME_PINYIN_K9_CAND_TEXT_NUM
            #else
                #define LV_IME_PINYIN_K9_CAND_TEXT_NUM 3
            #endif
        #endif
    #endif /*LV_IME_PINYIN_USE_K9_MODE*/
#endif

/*1: Enable file explorer*/
/*Requires: lv_table*/
#ifndef LV_USE_FILE_EXPLORER
    #ifdef CONFIG_LV_USE_FILE_EXPLORER
        #define LV_USE_FILE_EXPLORER CONFIG_LV_USE_FILE_EXPLORER
    #else
        #define LV_USE_FILE_EXPLORER                     0
    #endif
#endif
#if LV_USE_FILE_EXPLORER
    /*Maximum length of path*/
    #ifndef LV_FILE_EXPLORER_PATH_MAX_LEN
        #ifdef CONFIG_LV_FILE_EXPLORER_PATH_MAX_LEN
            #define LV_FILE_EXPLORER_PATH_MAX_LEN CONFIG_LV_FILE_EXPLORER_PATH_MAX_LEN
        #else
            #define LV_FILE_EXPLORER_PATH_MAX_LEN        (128)
        #endif
    #endif
    /*Quick access bar, 1:use, 0:not use*/
    /*Requires: lv_list*/
    #ifndef LV_FILE_EXPLORER_QUICK_ACCESS
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_FILE_EXPLORER_QUICK_ACCESS
                #define LV_FILE_EXPLORER_QUICK_ACCESS CONFIG_LV_FILE_EXPLORER_QUICK_ACCESS
            #else
                #define LV_FILE_EXPLORER_QUICK_ACCESS 0
            #endif
        #else
            #define LV_FILE_EXPLORER_QUICK_ACCESS        1
        #endif
    #endif
#endif

/*==================
 * DEVICES
 *==================*/

/*Use SDL to open window on PC and handle mouse and keyboard*/
#ifndef LV_USE_SDL
    #ifdef CONFIG_LV_USE_SDL
        #define LV_USE_SDL CONFIG_LV_USE_SDL
    #else
        #define LV_USE_SDL              0
    #endif
#endif
#if LV_USE_SDL
    #ifndef LV_SDL_INCLUDE_PATH
        #ifdef CONFIG_LV_SDL_INCLUDE_PATH
            #define LV_SDL_INCLUDE_PATH CONFIG_LV_SDL_INCLUDE_PATH
        #else
            #define LV_SDL_INCLUDE_PATH    <SDL2/SDL.h>
        #endif
    #endif
    #ifndef LV_SDL_RENDER_MODE
        #ifdef CONFIG_LV_SDL_RENDER_MODE
            #define LV_SDL_RENDER_MODE CONFIG_LV_SDL_RENDER_MODE
        #else
            #define LV_SDL_RENDER_MODE     LV_DISPLAY_RENDER_MODE_DIRECT   /*LV_DISPLAY_RENDER_MODE_DIRECT is recommended for best performance*/
        #endif
    #endif
    #ifndef LV_SDL_BUF_COUNT
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_SDL_BUF_COUNT
                #define LV_SDL_BUF_COUNT CONFIG_LV_SDL_BUF_COUNT
            #else
                #define LV_SDL_BUF_COUNT 0
            #endif
        #else
            #define LV_SDL_BUF_COUNT       1    /*1 or 2*/
        #endif
    #endif
    #ifndef LV_SDL_FULLSCREEN
        #ifdef CONFIG_LV_SDL_FULLSCREEN
            #define LV_SDL_FULLSCREEN CONFIG_LV_SDL_FULLSCREEN
        #else
            #define LV_SDL_FULLSCREEN      0    /*1: Make the window full screen by default*/
        #endif
    #endif
    #ifndef LV_SDL_DIRECT_EXIT
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_SDL_DIRECT_EXIT
                #define LV_SDL_DIRECT_EXIT CONFIG_LV_SDL_DIRECT_EXIT
            #else
                #define LV_SDL_DIRECT_EXIT 0
            #endif
        #else
            #define LV_SDL_DIRECT_EXIT     1    /*1: Exit the application when all SDL windows are closed*/
        #endif
    #endif
#endif

/*Use X11 to open window on Linux desktop and handle mouse and keyboard*/
#ifndef LV_USE_X11
    #ifdef CONFIG_LV_USE_X11
        #define LV_USE_X11 CONFIG_LV_USE_X11
    #else
        #define LV_USE_X11              0
    #endif
#endif
#if LV_USE_X11
    #ifndef LV_X11_DIRECT_EXIT
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_X11_DIRECT_EXIT
                #define LV_X11_DIRECT_EXIT CONFIG_LV_X11_DIRECT_EXIT
            #else
                #define LV_X11_DIRECT_EXIT 0
            #endif
        #else
            #define LV_X11_DIRECT_EXIT         1  /*Exit the application when all X11 windows have been closed*/
        #endif
    #endif
    #ifndef LV_X11_DOUBLE_BUFFER
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_X11_DOUBLE_BUFFER
                #define LV_X11_DOUBLE_BUFFER CONFIG_LV_X11_DOUBLE_BUFFER
            #else
                #define LV_X11_DOUBLE_BUFFER 0
            #endif
        #else
            #define LV_X11_DOUBLE_BUFFER       1  /*Use double buffers for endering*/
        #endif
    #endif
    /*select only 1 of the following render modes (LV_X11_RENDER_MODE_PARTIAL preferred!)*/
    #ifndef LV_X11_RENDER_MODE_PARTIAL
        #ifdef _LV_KCONFIG_PRESENT
            #ifdef CONFIG_LV_X11_RENDER_MODE_PARTIAL
                #define LV_X11_RENDER_MODE_PARTIAL CONFIG_LV_X11_RENDER_MODE_PARTIAL
            #else
                #define LV_X11_RENDER_MODE_PARTIAL 0
            #endif
        #else
            #define LV_X11_RENDER_MODE_PARTIAL 1  /*Partial render mode (preferred)*/
        #endif
    #endif
    #ifndef LV_X11_RENDER_MODE_DIRECT
        #ifdef CONFIG_LV_X11_RENDER_MODE_DIRECT
            #define LV_X11_RENDER_MODE_DIRECT CONFIG_LV_X11_RENDER_MODE_DIRECT
        #else
            #define LV_X11_RENDER_MODE_DIRECT  0  /*direct render mode*/
        #endif
    #endif
    #ifndef LV_X11_RENDER_MODE_FULL
        #ifdef CONFIG_LV_X11_RENDER_MODE_FULL
            #define LV_X11_RENDER_MODE_FULL CONFIG_LV_X11_RENDER_MODE_FULL
        #else
            #define LV_X11_RENDER_MODE_FULL    0  /*Full render mode*/
        #endif
    #endif
#endif

/*Driver for /dev/fb*/
#ifndef LV_USE_LINUX_FBDEV
    #ifdef CONFIG_LV_USE_LINUX_FBDEV
        #define LV_USE_LINUX_FBDEV CONFIG_LV_USE_LINUX_FBDEV
    #else
        #define LV_USE_LINUX_FBDEV      0
    #endif
#endif
#if LV_USE_LINUX_FBDEV
    #ifndef LV_LINUX_FBDEV_BSD
        #ifdef CONFIG_LV_LINUX_FBDEV_BSD
            #define LV_LINUX_FBDEV_BSD CONFIG_LV_LINUX_FBDEV_BSD
        #else
            #define LV_LINUX_FBDEV_BSD           0
        #endif
    #endif
    #ifndef LV_LINUX_FBDEV_RENDER_MODE
        #ifdef CONFIG_LV_LINUX_FBDEV_RENDER_MODE
            #define LV_LINUX_FBDEV_RENDER_MODE CONFIG_LV_LINUX_FBDEV_RENDER_MODE
        #else
            #define LV_LINUX_FBDEV_RENDER_MODE   LV_DISPLAY_RENDER_MODE_PARTIAL
        #endif
    #endif
    #ifndef LV_LINUX_FBDEV_BUFFER_COUNT
        #ifdef CONFIG_LV_LINUX_FBDEV_BUFFER_COUNT
            #define LV_LINUX_FBDEV_BUFFER_COUNT CONFIG_LV_LINUX_FBDEV_BUFFER_COUNT
        #else
            #define LV_LINUX_FBDEV_BUFFER_COUNT  0
        #endif
    #endif
    #ifndef LV_LINUX_FBDEV_BUFFER_SIZE
        #ifdef CONFIG_LV_LINUX_FBDEV_BUFFER_SIZE
            #define LV_LINUX_FBDEV_BUFFER_SIZE CONFIG_LV_LINUX_FBDEV_BUFFER_SIZE
        #else
            #define LV_LINUX_FBDEV_BUFFER_SIZE   60
        #endif
    #endif
#endif

/*Use Nuttx to open window and handle touchscreen*/
#ifndef LV_USE_NUTTX
    #ifdef CONFIG_LV_USE_NUTTX
        #define LV_USE_NUTTX CONFIG_LV_USE_NUTTX
    #else
        #define LV_USE_NUTTX    0
    #endif
#endif

#if LV_USE_NUTTX
    #ifndef LV_USE_NUTTX_LIBUV
        #ifdef CONFIG_LV_USE_NUTTX_LIBUV
            #define LV_USE_NUTTX_LIBUV CONFIG_LV_USE_NUTTX_LIBUV
        #else
            #define LV_USE_NUTTX_LIBUV    0
        #endif
    #endif

    /*Use Nuttx custom init API to open window and handle touchscreen*/
    #ifndef LV_USE_NUTTX_CUSTOM_INIT
        #ifdef CONFIG_LV_USE_NUTTX_CUSTOM_INIT
            #define LV_USE_NUTTX_CUSTOM_INIT CONFIG_LV_USE_NUTTX_CUSTOM_INIT
        #else
            #define LV_USE_NUTTX_CUSTOM_INIT    0
        #endif
    #endif

    /*Driver for /dev/lcd*/
    #ifndef LV_USE_NUTTX_LCD
        #ifdef CONFIG_LV_USE_NUTTX_LCD
            #define LV_USE_NUTTX_LCD CONFIG_LV_USE_NUTTX_LCD
        #else
            #define LV_USE_NUTTX_LCD      0
        #endif
    #endif
    #if LV_USE_NUTTX_LCD
        #ifndef LV_NUTTX_LCD_BUFFER_COUNT
            #ifdef CONFIG_LV_NUTTX_LCD_BUFFER_COUNT
                #define LV_NUTTX_LCD_BUFFER_COUNT CONFIG_LV_NUTTX_LCD_BUFFER_COUNT
            #else
                #define LV_NUTTX_LCD_BUFFER_COUNT    0
            #endif
        #endif
        #ifndef LV_NUTTX_LCD_BUFFER_SIZE
            #ifdef CONFIG_LV_NUTTX_LCD_BUFFER_SIZE
                #define LV_NUTTX_LCD_BUFFER_SIZE CONFIG_LV_NUTTX_LCD_BUFFER_SIZE
            #else
                #define LV_NUTTX_LCD_BUFFER_SIZE     60
            #endif
        #endif
    #endif

    /*Driver for /dev/input*/
    #ifndef LV_USE_NUTTX_TOUCHSCREEN
        #ifdef CONFIG_LV_USE_NUTTX_TOUCHSCREEN
            #define LV_USE_NUTTX_TOUCHSCREEN CONFIG_LV_USE_NUTTX_TOUCHSCREEN
        #else
            #define LV_USE_NUTTX_TOUCHSCREEN    0
        #endif
    #endif

#endif

/*Driver for /dev/dri/card*/
#ifndef LV_USE_LINUX_DRM
    #ifdef CONFIG_LV_USE_LINUX_DRM
        #define LV_USE_LINUX_DRM CONFIG_LV_USE_LINUX_DRM
    #else
        #define LV_USE_LINUX_DRM        0
    #endif
#endif

/*Interface for TFT_eSPI*/
#ifndef LV_USE_TFT_ESPI
    #ifdef CONFIG_LV_USE_TFT_ESPI
        #define LV_USE_TFT_ESPI CONFIG_LV_USE_TFT_ESPI
    #else
        #define LV_USE_TFT_ESPI         0
    #endif
#endif

/*Driver for evdev input devices*/
#ifndef LV_USE_EVDEV
    #ifdef CONFIG_LV_USE_EVDEV
        #define LV_USE_EVDEV CONFIG_LV_USE_EVDEV
    #else
        #define LV_USE_EVDEV    0
    #endif
#endif

/*Drivers for LCD devices connected via SPI/parallel port*/
#ifndef LV_USE_ST7735
    #ifdef CONFIG_LV_USE_ST7735
        #define LV_USE_ST7735 CONFIG_LV_USE_ST7735
    #else
        #define LV_USE_ST7735		0
    #endif
#endif
#ifndef LV_USE_ST7789
    #ifdef CONFIG_LV_USE_ST7789
        #define LV_USE_ST7789 CONFIG_LV_USE_ST7789
    #else
        #define LV_USE_ST7789		0
    #endif
#endif
#ifndef LV_USE_ST7796
    #ifdef CONFIG_LV_USE_ST7796
        #define LV_USE_ST7796 CONFIG_LV_USE_ST7796
    #else
        #define LV_USE_ST7796		0
    #endif
#endif
#ifndef LV_USE_ILI9341
    #ifdef CONFIG_LV_USE_ILI9341
        #define LV_USE_ILI9341 CONFIG_LV_USE_ILI9341
    #else
        #define LV_USE_ILI9341		0
    #endif
#endif

#ifndef LV_USE_GENERIC_MIPI
    #ifdef CONFIG_LV_USE_GENERIC_MIPI
        #define LV_USE_GENERIC_MIPI CONFIG_LV_USE_GENERIC_MIPI
    #else
        #define LV_USE_GENERIC_MIPI (LV_USE_ST7735 | LV_USE_ST7789 | LV_USE_ST7796 | LV_USE_ILI9341)
    #endif
#endif

/* LVGL Windows backend */
#ifndef LV_USE_WINDOWS
    #ifdef CONFIG_LV_USE_WINDOWS
        #define LV_USE_WINDOWS CONFIG_LV_USE_WINDOWS
    #else
        #define LV_USE_WINDOWS    0
    #endif
#endif

/*==================
* EXAMPLES
*==================*/

/*Enable the examples to be built with the library*/
#ifndef LV_BUILD_EXAMPLES
    #ifdef _LV_KCONFIG_PRESENT
        #ifdef CONFIG_LV_BUILD_EXAMPLES
            #define LV_BUILD_EXAMPLES CONFIG_LV_BUILD_EXAMPLES
        #else
            #define LV_BUILD_EXAMPLES 0
        #endif
    #else
        #define LV_BUILD_EXAMPLES 1
    #endif
#endif

/*===================
 * DEMO USAGE
 ====================*/

/*Show some widget. It might be required to increase `LV_MEM_SIZE` */
#ifndef LV_USE_DEMO_WIDGETS
    #ifdef CONFIG_LV_USE_DEMO_WIDGETS
        #define LV_USE_DEMO_WIDGETS CONFIG_LV_USE_DEMO_WIDGETS
    #else
        #define LV_USE_DEMO_WIDGETS 0
    #endif
#endif
#if LV_USE_DEMO_WIDGETS
    #ifndef LV_DEMO_WIDGETS_SLIDESHOW
        #ifdef CONFIG_LV_DEMO_WIDGETS_SLIDESHOW
            #define LV_DEMO_WIDGETS_SLIDESHOW CONFIG_LV_DEMO_WIDGETS_SLIDESHOW
        #else
            #define LV_DEMO_WIDGETS_SLIDESHOW 0
        #endif
    #endif
#endif

/*Demonstrate the usage of encoder and keyboard*/
#ifndef LV_USE_DEMO_KEYPAD_AND_ENCODER
    #ifdef CONFIG_LV_USE_DEMO_KEYPAD_AND_ENCODER
        #define LV_USE_DEMO_KEYPAD_AND_ENCODER CONFIG_LV_USE_DEMO_KEYPAD_AND_ENCODER
    #else
        #define LV_USE_DEMO_KEYPAD_AND_ENCODER 0
    #endif
#endif

/*Benchmark your system*/
#ifndef LV_USE_DEMO_BENCHMARK
    #ifdef CONFIG_LV_USE_DEMO_BENCHMARK
        #define LV_USE_DEMO_BENCHMARK CONFIG_LV_USE_DEMO_BENCHMARK
    #else
        #define LV_USE_DEMO_BENCHMARK 0
    #endif
#endif

/*Render test for each primitives. Requires at least 480x272 display*/
#ifndef LV_USE_DEMO_RENDER
    #ifdef CONFIG_LV_USE_DEMO_RENDER
        #define LV_USE_DEMO_RENDER CONFIG_LV_USE_DEMO_RENDER
    #else
        #define LV_USE_DEMO_RENDER 0
    #endif
#endif

/*Stress test for LVGL*/
#ifndef LV_USE_DEMO_STRESS
    #ifdef CONFIG_LV_USE_DEMO_STRESS
        #define LV_USE_DEMO_STRESS CONFIG_LV_USE_DEMO_STRESS
    #else
        #define LV_USE_DEMO_STRESS 0
    #endif
#endif

/*Music player demo*/
#ifndef LV_USE_DEMO_MUSIC
    #ifdef CONFIG_LV_USE_DEMO_MUSIC
        #define LV_USE_DEMO_MUSIC CONFIG_LV_USE_DEMO_MUSIC
    #else
        #define LV_USE_DEMO_MUSIC 0
    #endif
#endif
#if LV_USE_DEMO_MUSIC
    #ifndef LV_DEMO_MUSIC_SQUARE
        #ifdef CONFIG_LV_DEMO_MUSIC_SQUARE
            #define LV_DEMO_MUSIC_SQUARE CONFIG_LV_DEMO_MUSIC_SQUARE
        #else
            #define LV_DEMO_MUSIC_SQUARE    0
        #endif
    #endif
    #ifndef LV_DEMO_MUSIC_LANDSCAPE
        #ifdef CONFIG_LV_DEMO_MUSIC_LANDSCAPE
            #define LV_DEMO_MUSIC_LANDSCAPE CONFIG_LV_DEMO_MUSIC_LANDSCAPE
        #else
            #define LV_DEMO_MUSIC_LANDSCAPE 0
        #endif
    #endif
    #ifndef LV_DEMO_MUSIC_ROUND
        #ifdef CONFIG_LV_DEMO_MUSIC_ROUND
            #define LV_DEMO_MUSIC_ROUND CONFIG_LV_DEMO_MUSIC_ROUND
        #else
            #define LV_DEMO_MUSIC_ROUND     0
        #endif
    #endif
    #ifndef LV_DEMO_MUSIC_LARGE
        #ifdef CONFIG_LV_DEMO_MUSIC_LARGE
            #define LV_DEMO_MUSIC_LARGE CONFIG_LV_DEMO_MUSIC_LARGE
        #else
            #define LV_DEMO_MUSIC_LARGE     0
        #endif
    #endif
    #ifndef LV_DEMO_MUSIC_AUTO_PLAY
        #ifdef CONFIG_LV_DEMO_MUSIC_AUTO_PLAY
            #define LV_DEMO_MUSIC_AUTO_PLAY CONFIG_LV_DEMO_MUSIC_AUTO_PLAY
        #else
            #define LV_DEMO_MUSIC_AUTO_PLAY 0
        #endif
    #endif
#endif

/*Flex layout demo*/
#ifndef LV_USE_DEMO_FLEX_LAYOUT
    #ifdef CONFIG_LV_USE_DEMO_FLEX_LAYOUT
        #define LV_USE_DEMO_FLEX_LAYOUT CONFIG_LV_USE_DEMO_FLEX_LAYOUT
    #else
        #define LV_USE_DEMO_FLEX_LAYOUT     0
    #endif
#endif

/*Smart-phone like multi-language demo*/
#ifndef LV_USE_DEMO_MULTILANG
    #ifdef CONFIG_LV_USE_DEMO_MULTILANG
        #define LV_USE_DEMO_MULTILANG CONFIG_LV_USE_DEMO_MULTILANG
    #else
        #define LV_USE_DEMO_MULTILANG       0
    #endif
#endif

/*Widget transformation demo*/
#ifndef LV_USE_DEMO_TRANSFORM
    #ifdef CONFIG_LV_USE_DEMO_TRANSFORM
        #define LV_USE_DEMO_TRANSFORM CONFIG_LV_USE_DEMO_TRANSFORM
    #else
        #define LV_USE_DEMO_TRANSFORM       0
    #endif
#endif

/*Demonstrate scroll settings*/
#ifndef LV_USE_DEMO_SCROLL
    #ifdef CONFIG_LV_USE_DEMO_SCROLL
        #define LV_USE_DEMO_SCROLL CONFIG_LV_USE_DEMO_SCROLL
    #else
        #define LV_USE_DEMO_SCROLL          0
    #endif
#endif

/*Vector graphic demo*/
#ifndef LV_USE_DEMO_VECTOR_GRAPHIC
    #ifdef CONFIG_LV_USE_DEMO_VECTOR_GRAPHIC
        #define LV_USE_DEMO_VECTOR_GRAPHIC CONFIG_LV_USE_DEMO_VECTOR_GRAPHIC
    #else
        #define LV_USE_DEMO_VECTOR_GRAPHIC  0
    #endif
#endif


/*----------------------------------
 * End of parsing lv_conf_template.h
 -----------------------------------*/

#ifndef __ASSEMBLY__
LV_EXPORT_CONST_INT(LV_DPI_DEF);
LV_EXPORT_CONST_INT(LV_DRAW_BUF_STRIDE_ALIGN);
LV_EXPORT_CONST_INT(LV_DRAW_BUF_ALIGN);
#endif

#undef _LV_KCONFIG_PRESENT

/*Set some defines if a dependency is disabled*/
#if LV_USE_LOG == 0
    #define LV_LOG_LEVEL            LV_LOG_LEVEL_NONE
    #define LV_LOG_TRACE_MEM        0
    #define LV_LOG_TRACE_TIMER      0
    #define LV_LOG_TRACE_INDEV      0
    #define LV_LOG_TRACE_DISP_REFR  0
    #define LV_LOG_TRACE_EVENT      0
    #define LV_LOG_TRACE_OBJ_CREATE 0
    #define LV_LOG_TRACE_LAYOUT     0
    #define LV_LOG_TRACE_ANIM       0
#endif  /*LV_USE_LOG*/


/*If running without lv_conf.h add typedefs with default value*/
#ifdef LV_CONF_SKIP
    #if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)    /*Disable warnings for Visual Studio*/
        #define _CRT_SECURE_NO_WARNINGS
    #endif
#endif  /*defined(LV_CONF_SKIP)*/

#endif  /*LV_CONF_INTERNAL_H*/
