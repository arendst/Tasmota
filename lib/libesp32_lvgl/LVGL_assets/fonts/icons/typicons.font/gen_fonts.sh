# font map: https://www.s-ings.com/typicons/
# https://github.com/stephenhutchings/typicons.font

./lv_font_conv.js --no-kerning --bpp 1 --size 24 -o typicons24.lvfont --format bin --font typicons.ttf -r 0xE000-0xE14F
./lv_font_conv.js --no-kerning --bpp 1 --size 36 -o typicons36.lvfont --format bin --font typicons.ttf -r 0xE000-0xE14F
./lv_font_conv.js --no-kerning --bpp 1 --size 48 -o typicons48.lvfont --format bin --font typicons.ttf -r 0xE000-0xE14F
./lv_font_conv.js --no-kerning --bpp 1 --size 64 -o typicons64.lvfont --format bin --font typicons.ttf -r 0xE000-0xE14F

./lv_font_conv.js --no-kerning --bpp 1 --size 24 -o typicons24.c --format bin --font typicons.ttf --format lvgl -r 0xE000-0xE14F
