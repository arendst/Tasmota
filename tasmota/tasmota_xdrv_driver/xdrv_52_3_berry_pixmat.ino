/*
  xdrv_52_3_berry_pixmat.ino - Berry scripting language, native functions

  Copyright (C) 2021 Christian Baars & Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_BERRY
#include <berry.h>
#ifdef USE_WS2812

#include "be_constobj.h"
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <limits>

// Forward declare native core so it can appear in signatures
struct PixmatCore;

// Prototypes for helpers
static PixmatCore* self_core(bvm* vm);
static inline bool in_bounds(const PixmatCore* mc, int x, int y);

struct PixmatCore {
    uint8_t* data = nullptr;
    int width = 0;
    int height = 0;
    int bpp = 0;
    bool serpentine = false;
    bool external = false;

    inline size_t index(int x, int y) const {
        return static_cast<size_t>((y * width + x) * bpp);
    }

    inline void load(int x, int y, uint8_t* out) const {
        int phys_x = (serpentine && (y & 1)) ? (width - 1 - x) : x;
        const size_t idx = static_cast<size_t>((y * width + phys_x) * bpp);
        memcpy(out, data + idx, bpp);
    }

    inline void store(int x, int y, const uint8_t* in) {
        int phys_x = (serpentine && (y & 1)) ? (width - 1 - x) : x;
        const size_t idx = static_cast<size_t>((y * width + phys_x) * bpp);
        memcpy(data + idx, in, bpp);
    }

    inline size_t bytes_size() const {
        return static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(bpp);
    }

    inline void blit(const PixmatCore* src, int dx, int dy) {
      uint8_t pix[8]; // enough for max bpp
      for (int sy = 0; sy < src->height; ++sy) {
          int dypos = sy + dy;
          if (dypos < 0 || dypos >= height) continue;
          for (int sx = 0; sx < src->width; ++sx) {
              int dxpos = sx + dx;
              if (dxpos < 0 || dxpos >= width) continue;
              src->load(sx, sy, pix);
              store(dxpos, dypos, pix);
          }
      }
    }
};

/* fetch native pointer from this .p */
static PixmatCore* self_core(bvm* vm) {
    be_getmember(vm, 1, ".p");
    PixmatCore* mc = (PixmatCore*) be_tocomptr(vm, -1);
    be_pop(vm, 1);
    return mc;
}

static inline uint8_t mul8(uint8_t a, uint8_t b){ return (a * b) >> 8; }

static inline bool in_bounds(const PixmatCore* mc, int x, int y) {
    return (x >= 0 && x < mc->width && y >= 0 && y < mc->height);
}

/* helper: apply brightness scaling to a pixel */
static inline void apply_brightness(uint8_t *px, int bpp, uint8_t bri) {
    if (bri == 255) return;
    for (int i = 0; i < bpp; ++i) {
        px[i] = mul8(px[i], bri);
    }
}

static inline void unpack_color(uint32_t c, int bpp, uint8_t out[8]) {
    if (bpp == 1) { out[0] = c & 0xFF; return; }
    if (bpp == 3) { out[0] = (c>>16)&0xFF; out[1] = (c>>8)&0xFF; out[2] = c&0xFF; return; }
    if (bpp == 4) { out[0] = (c>>24)&0xFF; out[1] = (c>>16)&0xFF; out[2] = (c>>8)&0xFF; out[3] = c&0xFF; }
}

extern "C" {

// Prototypes for all Berry entry points
int be_pixmat_init(bvm* vm);
int be_pixmat_deinit(bvm* vm);
int be_pixmat_get(bvm* vm);
int be_pixmat_set(bvm* vm);
int be_pixmat_blit(bvm* vm);
int be_pixmat_scroll(bvm* vm);
int be_pixmat_clear(bvm* vm);

int be_pixmat_init(bvm* vm) {
    int argc = be_top(vm);
    auto* mc = new PixmatCore();

    // overload: pixmat(bitplane_bytes, bytes_per_line)
    if (be_isbytes(vm, 2) && be_isint(vm, 3) && argc == 3) {
        size_t len = 0;
        const uint8_t* bits = (const uint8_t*)be_tobytes(vm, 2, &len);
        int bytes_per_line = be_toint(vm, 3);
        if (bytes_per_line <= 0 || (len % bytes_per_line) != 0) {
            delete mc;
            be_raise(vm, "value_error", "invalid bitplane dimensions");
        }
        mc->width  = bytes_per_line * 8;
        mc->height = (int)(len / bytes_per_line);
        mc->bpp    = 1;
        mc->serpentine = false;
        mc->external   = false;

        size_t need = mc->bytes_size();
        mc->data = (uint8_t*)malloc(need);
        if (!mc->data) {
            delete mc;
            be_raise(vm, "runtime_error", "alloc fail");
        }
        memset(mc->data, 0, need);

        // Unpack bits into 1‑bpp luminance values (0 or 255)
        for (int y = 0; y < mc->height; ++y) {
            for (int xb = 0; xb < bytes_per_line; ++xb) {
                uint8_t b = bits[y * bytes_per_line + xb];
                for (int bit = 0; bit < 8; ++bit) {
                    if (b & (1 << (7 - bit))) {
                        mc->data[y * mc->width + xb * 8 + bit] = 255;
                    }
                }
            }
        }

    }
    // overload: wrap external buffer
    else if (be_isbytes(vm, 2)) {
        size_t len = 0;
        const void* ptr = be_tobytes(vm, 2, &len);
        mc->width = be_toint(vm, 3);
        mc->height = be_toint(vm, 4);
        mc->bpp = be_toint(vm, 5);
        mc->serpentine = (argc >= 6) ? be_tobool(vm, 6) : false;
        mc->external = true;

        if (mc->width <= 0 || mc->height <= 0 || mc->bpp <= 0) {
            delete mc; be_raise(vm, "value_error", "invalid dimensions or bpp");
        }
        size_t need = (size_t)mc->width * (size_t)mc->height;
        if (mc->bpp > 0 && need > (std::numeric_limits<size_t>::max)() / (size_t)mc->bpp) {
            delete mc; be_raise(vm, "value_error", "size overflow");
        }
        need *= (size_t)mc->bpp;
        if (len < need) { delete mc; be_raise(vm, "value_error", "buffer too small"); }
        mc->data = (uint8_t*)ptr;
        be_pushvalue(vm, 2);
        be_setmember(vm, 1, "_buf");
    }
    // overload: allocate new buffer
    else if (be_isint(vm, 2)) {
        mc->width = be_toint(vm, 2);
        mc->height = be_toint(vm, 3);
        mc->bpp = be_toint(vm, 4);
        mc->serpentine = (argc >= 5) ? be_tobool(vm, 5) : false;
        mc->external = false;

        if (mc->width <= 0 || mc->height <= 0 || mc->bpp <= 0) {
            delete mc; be_raise(vm, "value_error", "invalid dimensions or bpp");
        }
        size_t need = (size_t)mc->width * (size_t)mc->height;
        if (mc->bpp > 0 && need > (std::numeric_limits<size_t>::max)() / (size_t)mc->bpp) {
            delete mc; be_raise(vm, "value_error", "size overflow");
        }
        need *= (size_t)mc->bpp;
        mc->data = (uint8_t*)malloc(need);
        if (!mc->data) { delete mc; be_raise(vm, "runtime_error", "alloc fail"); }
        memset(mc->data, 0, need);
    }
    else {
        delete mc;
        be_raise(vm, "type_error",
            "pixmat(bitlines:bytes,bytes_per_line) or pixmat(buf:bytes,w,h,bpp,[serp]) or pixmat(w,h,bpp,[serp])");
    }

    be_pushcomptr(vm, (void*)mc);
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
}

int be_pixmat_deinit(bvm* vm) {
    auto* mc = self_core(vm);
    if (mc) {
        if (!mc->external && mc->data) free(mc->data);
        delete mc;
        be_pushnil(vm); be_setmember(vm, 1, ".p");
    }
    be_return_nil(vm);
}

int be_pixmat_clear(bvm* vm) {
    auto* mc = self_core(vm);
    if (!mc) be_raise(vm, "type_error", "clear([val:int])");
    int val = (be_top(vm) >= 2) ? (be_toint(vm, 2) & 0xFF) : 0;
    memset(mc->data, val, mc->bytes_size());
    be_return_nil(vm);
}

/* get */
int be_pixmat_get(bvm* vm) {
    auto* mc = self_core(vm);
    if (!mc || be_top(vm) < 3) be_raise(vm, "type_error", "get(x,y)");
    int x = be_toint(vm, 2), y = be_toint(vm, 3);
    if (!in_bounds(mc, x, y)) be_return_nil(vm);
    uint8_t v[8]; mc->load(x, y, v);
    if (mc->bpp == 3) { unsigned int c = (v[0] << 16) | (v[1] << 8) | v[2]; be_pushint(vm, (bint)c); }
    else if (mc->bpp == 4) { unsigned int c = (v[0] << 24) | (v[1] << 16) | (v[2] << 8) | v[3]; be_pushint(vm, (bint)c); }
    else if (mc->bpp == 1) { be_pushint(vm, v[0]); }
    else { be_newlist(vm); for (int i=0;i<mc->bpp;++i){ be_pushint(vm, v[i]); be_data_push(vm, -2);} }
    be_return(vm);
}

int be_pixmat_set(bvm* vm) {
    auto* mc = self_core(vm);
    int argc = be_top(vm);
    if (!mc || argc < 4) {
        be_raise(vm, "type_error",
            "set(x:int,y:int,val[,bri:int]) or set(x:int,y:int,h:int,s:int,v:int[,bri:int])");
    }
    int x = be_toint(vm, 2), y = be_toint(vm, 3);
    if (!in_bounds(mc, x, y)) return 0;
    uint8_t vals[8] = {0};

    // default brightness
    uint8_t bri = (argc == 5 || argc == 7) ? (uint8_t)be_toint(vm, argc) : 255;

    if (be_isint(vm, 4) && (argc == 4 || argc == 5)) {
        // RGB packed int path
        unsigned int color = (unsigned int)be_toint(vm, 4);
        unpack_color(color, mc->bpp, vals);
    }
    else if (argc >= 6 && argc <= 7) {
        // HSV path: set(x,y,h,s,v[,bri])
        int h = be_toint(vm, 4);
        int s = be_toint(vm, 5);
        int v = be_toint(vm, 6);
        if (argc == 7) bri = (uint8_t)be_toint(vm, 7);

        h %= 360; if (h < 0) h += 360;
        uint16_t hue = (uint16_t)((h * 1536L) / 360L);
        uint8_t sat = (uint8_t)s, val = (uint8_t)v;
        uint8_t sextant = hue >> 8, frac = hue & 0xFF;
        uint8_t p = (val * (255 - sat)) >> 8;
        uint8_t q = (val * (255 - ((sat * frac) >> 8))) >> 8;
        uint8_t t = (val * (255 - ((sat * (255 - frac)) >> 8))) >> 8;

        if (mc->bpp >= 3) {
            switch (sextant) {
                case 0: vals[0] = val; vals[1] = t;   vals[2] = p;   break;
                case 1: vals[0] = q;   vals[1] = val; vals[2] = p;   break;
                case 2: vals[0] = p;   vals[1] = val; vals[2] = t;   break;
                case 3: vals[0] = p;   vals[1] = q;   vals[2] = val; break;
                case 4: vals[0] = t;   vals[1] = p;   vals[2] = val; break;
                default:vals[0] = val; vals[1] = p;   vals[2] = q;   break;
            }
        } else if (mc->bpp == 1) {
            vals[0] = val;
        }
    }
    else {
        be_raise(vm, "type_error", "unsupported argument pattern");
    }
    apply_brightness(vals, mc->bpp, bri);
    mc->store(x, y, vals);
    be_return_nil(vm);
}

int be_pixmat_blit(bvm* vm) {
    auto* dest = self_core(vm);
    if (!dest || be_top(vm) < 4) be_raise(vm, "type_error", "blit(src,dx,dy[,bri:int][,tint:int])");

    be_getmember(vm, 2, ".p");
    PixmatCore* src = (PixmatCore*)be_tocomptr(vm, -1);
    be_pop(vm, 1);
    if (!src) be_raise(vm, "type_error", "invalid src matrix");

    const int dx = be_toint(vm, 3);
    const int dy = be_toint(vm, 4);

    const bool same_bpp = (src->bpp == dest->bpp);
    const bool mono_to_color = (src->bpp == 1 && (dest->bpp == 1 || dest->bpp >= 3));
    if (!same_bpp && !mono_to_color) be_raise(vm, "value_error", "unsupported bpp conversion");

    int bri = 255;
    uint8_t tint[8] = {255,255,255,255,0,0,0,0}; // identity tint by default
    bool has_tint = false;

    if (be_top(vm) >= 5 && be_isint(vm, 5)) bri =  (uint8_t)be_toint(vm, 5);
    if (be_top(vm) >= 6 && be_isint(vm, 6)) { unpack_color((uint32_t)be_toint(vm, 6), dest->bpp, tint); has_tint = true; }

    // default tint for mono->color if none given: keep identity (white)
    if (mono_to_color && !has_tint) has_tint = false; // identity tint

    // Fast path
    if (same_bpp && bri == 255 && !has_tint) { dest->blit(src, dx, dy); be_return_nil(vm); }

    // Slow path
    uint8_t s[8], d[8];
    for (int sy = 0; sy < src->height; ++sy) {
        int dypos = dy + sy; if (dypos < 0 || dypos >= dest->height) continue;
        for (int sx = 0; sx < src->width; ++sx) {
            int dxpos = dx + sx; if (dxpos < 0 || dxpos >= dest->width) continue;

            src->load(sx, sy, s);
            if (src->bpp == 1 && s[0] == 0) continue; // skip transparent mono pixel
            if (same_bpp) {
                memcpy(d, s, dest->bpp);
                if (has_tint) for (int c = 0; c < dest->bpp; ++c) d[c] = mul8(d[c], tint[c]);
                apply_brightness(d, dest->bpp, bri);
                dest->store(dxpos, dypos, d);
            } else {
                // mono -> color (or mono -> mono)
                uint8_t L = s[0];
                for (int c = 0; c < dest->bpp; ++c) d[c] = has_tint ? mul8(L, tint[c]) : L;
                apply_brightness(d, dest->bpp, bri);
                dest->store(dxpos, dypos, d);
            }
        }
    }
    be_return_nil(vm);
}

int be_pixmat_scroll(bvm* vm) {
    auto* d = self_core(vm);
    if (!d || be_top(vm) < 2)
        be_raise(vm, "type_error", "scroll(dir[,src])");

    int dir  = be_toint(vm, 2) & 3;
    int w    = d->width;
    int h    = d->height;
    int bpp  = d->bpp;

    PixmatCore* s = nullptr;
    if (be_top(vm) >= 3 && !be_isnil(vm, 3)) {
        be_getmember(vm, 3, ".p");
        s = (PixmatCore*)be_tocomptr(vm, -1);
        be_pop(vm, 1);
    }

    size_t need = (dir < 2 ? w : h) * bpp;
    uint8_t edge[256], pix[8];

    auto save_row  = [&](int y, PixmatCore* m) { for (int x = 0; x < w; ++x) m->load(x, y, edge + x * bpp); };
    auto save_col  = [&](int x, PixmatCore* m) { for (int y = 0; y < h; ++y) m->load(x, y, edge + y * bpp); };
    auto write_row = [&](int y) { for (int x = 0; x < w; ++x) d->store(x, y, edge + x * bpp); };
    auto write_col = [&](int x) { for (int y = 0; y < h; ++y) d->store(x, y, edge + y * bpp); };

    switch (dir) {
        case 0:
            save_row(!s || s == d ? 0 : h - 1, !s || s == d ? d : s);
            for (int y = 0; y < h - 1; ++y)
                for (int x = 0; x < w; ++x) {
                    d->load(x, y + 1, pix);
                    d->store(x, y, pix);
                }
            write_row(h - 1);
            break;

        case 1:
            save_col(!s || s == d ? 0 : w - 1, !s || s == d ? d : s);
            for (int y = 0; y < h; ++y)
                for (int x = 0; x < w - 1; ++x) {
                    d->load(x + 1, y, pix);
                    d->store(x, y, pix);
                }
            write_col(w - 1);
            break;

        case 2:
            save_row(!s || s == d ? h - 1 : 0, !s || s == d ? d : s);
            for (int y = h - 1; y > 0; --y)
                for (int x = 0; x < w; ++x) {
                    d->load(x, y - 1, pix);
                    d->store(x, y, pix);
                }
            write_row(0);
            break;

        case 3:
            save_col(!s || s == d ? w - 1 : 0, !s || s == d ? d : s);
            for (int y = 0; y < h; ++y)
                for (int x = w - 1; x > 0; --x) {
                    d->load(x - 1, y, pix);
                    d->store(x, y, pix);
                }
            write_col(0);
            break;
    }

    be_return_nil(vm);
}

} // extern "C"

#endif // USE_WS2812
#endif // USE_BERRY


/*
pixmat API (Berry)
==================

Constructor overloads:
-----------------------
pixmat(bitplane_bytes:bytes, bytes_per_line:int)
    Create 1‑bpp mono matrix from packed bitplane data.

pixmat(buf:bytes, width:int, height:int, bpp:int[, serpentine:bool])
    Wrap an existing pixel buffer (no copy).

pixmat(width:int, height:int, bpp:int[, serpentine:bool])
    Allocate a new zero‑filled buffer.

Methods:
--------
clear([val:int])
    Fill entire matrix with val (default 0).

get(x:int, y:int) -> int | list
    1‑bpp: luminance (0–255)
    3‑bpp: packed RGB 0xRRGGBB
    4‑bpp: packed RGBW 0xRRGGBBWW
    other bpp: list of channel values.

set(x:int, y:int, rgb:int[, bri:int])
    Set pixel from packed RGB, optional brightness scale.

set(x:int, y:int, h:int, s:int, v:int[, bri:int])
    Set pixel from HSV (h=0–359°, s/v=0–255), optional brightness.

blit(src:pixmat, dx:int, dy:int[, bri:int][, tint:int])
    Copy pixels from src into this matrix at offset.
    Optional brightness scale and RGB tint.
    Supports mono→color expansion.

scroll(dir:int[, src:pixmat])
    Scroll content by one pixel:
        dir=0: up
        dir=1: left
        dir=2: down
        dir=3: right
    If src given, fill vacated row/col from src.

Notes:
------
- bpp = bytes per pixel (1=mono, 3=RGB, 4=RGBW)
- serpentine=true reverses odd rows in memory
- All operations are in‑place on the underlying buffer
- Brightness/tint use integer per‑channel scaling

*/
