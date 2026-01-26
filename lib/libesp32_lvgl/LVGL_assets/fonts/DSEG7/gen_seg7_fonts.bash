#!/bin/bash
#
# Convert fonts to files and C files
#

FONT_CONV="lv_font_conv"
FONT_DIR="font_files/"
C_DIR="c_files/"

# from size 8 to 18, pixel perfect non-italic

${FONT_CONV} --font DSEG7Classic-Bold.ttf -r 0x20-0x3F --size  8 --format bin --bpp 1 -o ${FONT_DIR}/seg7_8.lvfont
${FONT_CONV} --font DSEG7Classic-Bold.ttf -r 0x20-0x3F --size 10 --format bin --bpp 1 -o ${FONT_DIR}/seg7_10.lvfont
${FONT_CONV} --font DSEG7Classic-Bold.ttf -r 0x20-0x3F --size 12 --format bin --bpp 1 -o ${FONT_DIR}/seg7_12.lvfont
${FONT_CONV} --font DSEG7Classic-Bold.ttf -r 0x20-0x3F --size 14 --format bin --bpp 1 -o ${FONT_DIR}/seg7_14.lvfont
${FONT_CONV} --font DSEG7Classic-Bold.ttf -r 0x20-0x3F --size 16 --format bin --bpp 1 -o ${FONT_DIR}/seg7_16.lvfont
${FONT_CONV} --font DSEG7Classic-Bold.ttf -r 0x20-0x3F --size 18 --format bin --bpp 1 -o ${FONT_DIR}/seg7_18.lvfont

# above 18, 2 bits per pixel, italic

${FONT_CONV} --font DSEG7Classic-BoldItalic.ttf -r 0x20-0x3F --size 20 --format bin --bpp 2 -o ${FONT_DIR}/seg7_20.lvfont
${FONT_CONV} --font DSEG7Classic-BoldItalic.ttf -r 0x20-0x3F --size 24 --format bin --bpp 2 -o ${FONT_DIR}/seg7_24.lvfont
${FONT_CONV} --font DSEG7Classic-BoldItalic.ttf -r 0x20-0x3F --size 28 --format bin --bpp 2 -o ${FONT_DIR}/seg7_28.lvfont
${FONT_CONV} --font DSEG7Classic-BoldItalic.ttf -r 0x20-0x3F --size 36 --format bin --bpp 2 -o ${FONT_DIR}/seg7_36.lvfont
${FONT_CONV} --font DSEG7Classic-BoldItalic.ttf -r 0x20-0x3F --size 48 --format bin --bpp 2 -o ${FONT_DIR}/seg7_48.lvfont

# from size 8 to 18, pixel perfect non-italic

${FONT_CONV} --font DSEG7Classic-Bold.ttf -r 0x20-0x3F --size  8 --format lvgl --bpp 1 -o ${C_DIR}/seg7_8.c
${FONT_CONV} --font DSEG7Classic-Bold.ttf -r 0x20-0x3F --size 10 --format lvgl --bpp 1 -o ${C_DIR}/seg7_10.c
${FONT_CONV} --font DSEG7Classic-Bold.ttf -r 0x20-0x3F --size 12 --format lvgl --bpp 1 -o ${C_DIR}/seg7_12.c
${FONT_CONV} --font DSEG7Classic-Bold.ttf -r 0x20-0x3F --size 14 --format lvgl --bpp 1 -o ${C_DIR}/seg7_14.c
${FONT_CONV} --font DSEG7Classic-Bold.ttf -r 0x20-0x3F --size 16 --format lvgl --bpp 1 -o ${C_DIR}/seg7_16.c
${FONT_CONV} --font DSEG7Classic-Bold.ttf -r 0x20-0x3F --size 18 --format lvgl --bpp 1 -o ${C_DIR}/seg7_18.c

# above 18, 2 bits per pixel, italic

${FONT_CONV} --font DSEG7Classic-BoldItalic.ttf -r 0x20-0x3F --size 20 --format lvgl --bpp 2 -o ${C_DIR}/seg7_20.c
${FONT_CONV} --font DSEG7Classic-BoldItalic.ttf -r 0x20-0x3F --size 24 --format lvgl --bpp 2 -o ${C_DIR}/seg7_24.c
${FONT_CONV} --font DSEG7Classic-BoldItalic.ttf -r 0x20-0x3F --size 28 --format lvgl --bpp 2 -o ${C_DIR}/seg7_28.c
${FONT_CONV} --font DSEG7Classic-BoldItalic.ttf -r 0x20-0x3F --size 36 --format lvgl --bpp 2 -o ${C_DIR}/seg7_36.c
${FONT_CONV} --font DSEG7Classic-BoldItalic.ttf -r 0x20-0x3F --size 48 --format lvgl --bpp 2 -o ${C_DIR}/seg7_48.c
