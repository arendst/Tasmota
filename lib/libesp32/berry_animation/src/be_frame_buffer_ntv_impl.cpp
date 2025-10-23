/*
  xdrv_52_3_berry_leds.ino - Berry scripting language, native fucnctions

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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
#ifdef USE_BERRY_ANIMATION

extern uint16_t changeUIntScale(uint16_t inum, uint16_t ifrom_min, uint16_t ifrom_max,uint16_t ito_min, uint16_t ito_max);
extern uint32_t ApplyBriGamma(uint32_t color_a /* 0xRRGGBB */, uint32_t bri /* 0..255 */, bool gamma);

extern "C" {
  // frame_buffer_ntv.blend(color1:int, color2:int) -> int
  // Blend two colors using color2's alpha channel
  // color1: destination color (ARGB format - 0xAARRGGBB)
  // color2: source color (ARGB format - 0xAARRGGBB)
  int32_t be_animation_ntv_blend(bvm *vm);
  int32_t be_animation_ntv_blend(bvm *vm) {
    uint32_t color1 = be_toint(vm, 1);
    uint32_t color2 = be_toint(vm, 2);
    
    // Extract components from color1 (destination)
    uint32_t a1 = (color1 >> 24) & 0xFF;
    uint32_t r1 = (color1 >> 16) & 0xFF;
    uint32_t g1 = (color1 >>  8) & 0xFF;
    uint32_t b1 = (color1      ) & 0xFF;
    
    // Extract components from color2 (source)
    uint32_t a2 = (color2 >> 24) & 0xFF;
    uint32_t r2 = (color2 >> 16) & 0xFF;
    uint32_t g2 = (color2 >>  8) & 0xFF;
    uint32_t b2 = (color2      ) & 0xFF;
    
    // Fast path: if source is fully transparent, return destination unchanged
    if (a2 == 0) {
      be_pushint(vm, color1);
      be_return(vm);
    }
    
    // Blend RGB channels using source alpha
    uint8_t r = changeUIntScale(255 - a2, 0, 255, 0, r1) + changeUIntScale(a2, 0, 255, 0, r2);
    uint8_t g = changeUIntScale(255 - a2, 0, 255, 0, g1) + changeUIntScale(a2, 0, 255, 0, g2);
    uint8_t b = changeUIntScale(255 - a2, 0, 255, 0, b1) + changeUIntScale(a2, 0, 255, 0, b2);
    
    // Blend alpha channels: a = a1 + (255 - a1) * a2 / 255
    uint32_t a = a1 + changeUIntScale((255 - a1) * a2, 0, 255 * 255, 0, 255);
    
    // Clamp alpha to valid range
    if (a > 255) { a = 255; }
    
    // Combine components into result
    uint32_t result = (a << 24) | (r << 16) | (g << 8) | b;
    be_pushint(vm, result);
    be_return(vm);
  }

  // frame_buffer_ntv.blend_linear(color1:int, color2:int, alpha:int) -> int
  //
  int32_t be_animation_ntv_blend_linear(bvm *vm);
  int32_t be_animation_ntv_blend_linear(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    // we skip argument type testing since we're in a controlled environment
    uint32_t color_a = be_toint(vm, 1);
    uint32_t color_b = be_toint(vm, 2);
    uint32_t alpha = be_toint(vm, 3);
    uint32_t r = (color_a >> 16) & 0xFF;
    uint32_t g = (color_a >>  8) & 0xFF;
    uint32_t b = (color_a      ) & 0xFF;
    uint32_t a = (color_a >> 24) & 0xFF;
    uint32_t r2 = (color_b >> 16) & 0xFF;
    uint32_t g2 = (color_b >>  8) & 0xFF;
    uint32_t b2 = (color_b      ) & 0xFF;
    uint32_t a2 = (color_b >> 24) & 0xFF;
    uint8_t r3 = changeUIntScale(alpha, 0, 255, r2, r);
    uint8_t g3 = changeUIntScale(alpha, 0, 255, g2, g);
    uint8_t b3 = changeUIntScale(alpha, 0, 255, b2, b);
    uint8_t a3 = changeUIntScale(alpha, 0, 255, a2, a);
    uint32_t rgb = (a3 << 24) | (r3 << 16) | (g3 << 8) | b3;
    be_pushint(vm, rgb);
    be_return(vm);
  }

  // frame_buffer_ntv.blend_pixels(dest_bytes:bytes(), src_bytes:bytes(), region_start:int, region_end:int) -> nil
  // Blend source buffer into destination buffer using per-pixel alpha
  // Standard ARGB convention: alpha 0 = transparent, 255 = opaque
  int32_t be_animation_ntv_blend_pixels(bvm *vm);
  int32_t be_animation_ntv_blend_pixels(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    size_t dest_len = 0;
    uint32_t * dest_buf = (uint32_t*) be_tobytes(vm, 1, &dest_len);
    size_t src_len = 0;
    const uint32_t * src_buf = (const uint32_t*) be_tobytes(vm, 2, &src_len);
    if (dest_buf == NULL || src_buf == NULL) {
      be_raise(vm, "argument_error", "needs bytes() arguments");
    }
    int32_t region_start = 0;
    int32_t region_end = -1;
    if (top >= 3 && be_isint(vm, 3)) {
      region_start = be_toint(vm, 3);
    }
    if (top >= 4 && be_isint(vm,4)) {
      region_end = be_toint(vm, 4);
    }
    
    // Calculate pixel counts
    size_t dest_pixels = dest_len / 4;
    size_t src_pixels = src_len / 4;
    // Clamp to smallest of both
    if (dest_pixels < src_pixels) { dest_pixels = src_pixels; }
    if (src_pixels < dest_pixels) { src_pixels = dest_pixels; }
    
    // Validate region bounds
    if (region_start < 0) { region_start += dest_pixels; }
    if (region_end < 0) { region_end += dest_pixels; }
    if (region_start < 0) { region_start = 0; }
    if (region_end < 0) { region_end = 0; }
    if (region_start >= dest_pixels) { be_return_nil(vm); }
    if (region_end >= dest_pixels) { region_end = dest_pixels - 1; }
    if (region_end < region_start) { be_return_nil(vm); }
    
    // Blend pixels in the specified region
    for (int32_t i = region_start; i <= region_end; i++) {
      uint32_t color2 = src_buf[i];
      uint32_t a2 = (color2 >> 24) & 0xFF;
      
      // Only blend if source has some alpha (standard ARGB: 0=transparent, 255=opaque)
      if (a2 > 0) {
        if (a2 == 255) {
          // Fully opaque source, just copy it
          dest_buf[i] = color2;
        } else {
          // Partially transparent, need to blend
          uint32_t color1 = dest_buf[i];
          
          // Extract components from color1 (destination)
          uint32_t a1 = (color1 >> 24) & 0xFF;
          uint32_t r1 = (color1 >> 16) & 0xFF;
          uint32_t g1 = (color1 >>  8) & 0xFF;
          uint32_t b1 = (color1      ) & 0xFF;
          
          // Extract components from color2 (source) - already have a2
          uint32_t r2 = (color2 >> 16) & 0xFF;
          uint32_t g2 = (color2 >>  8) & 0xFF;
          uint32_t b2 = (color2      ) & 0xFF;
          
          // Blend RGB channels using source alpha
          uint8_t r = changeUIntScale(255 - a2, 0, 255, 0, r1) + changeUIntScale(a2, 0, 255, 0, r2);
          uint8_t g = changeUIntScale(255 - a2, 0, 255, 0, g1) + changeUIntScale(a2, 0, 255, 0, g2);
          uint8_t b = changeUIntScale(255 - a2, 0, 255, 0, b1) + changeUIntScale(a2, 0, 255, 0, b2);
          
          // Blend alpha channels: a = a1 + (255 - a1) * a2 / 255
          uint32_t a = a1 + changeUIntScale((255 - a1) * a2, 0, 255 * 255, 0, 255);
          if (a > 255) { a = 255; }
          
          // Write blended result
          dest_buf[i] = (a << 24) | (r << 16) | (g << 8) | b;
        }
      }
      // If a2 == 0 (fully transparent), leave destination unchanged
    }
    
    be_return_nil(vm);
  }

  // frame_buffer_ntv.gradient_fill(pixels:bytes(), color1:int, color2:int, start_pos:int, end_pos:int) -> nil
  // Create a gradient fill in the buffer
  int32_t be_animation_ntv_gradient_fill(bvm *vm);
  int32_t be_animation_ntv_gradient_fill(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    size_t pixels_len = 0;
    uint32_t * pixels_buf = (uint32_t*) be_tobytes(vm, 1, &pixels_len);
    if (pixels_buf == NULL) {
      be_raise(vm, "argument_error", "needs bytes() argument");
    }
    uint32_t color1 = be_toint(vm, 2);
    uint32_t color2 = be_toint(vm, 3);
    int32_t start_pos = 0;
    int32_t end_pos = -1;
    if (top >= 4 && be_isint(vm, 4)) {
      start_pos = be_toint(vm, 4);
    }
    if (top >= 5 && be_isint(vm, 5)) {
      end_pos = be_toint(vm, 5);
    }
    
    // Calculate pixel count
    size_t width = pixels_len / 4;
    
    // Handle negative indices (Python-style)
    if (start_pos < 0) { start_pos += width; }
    if (end_pos < 0) { end_pos += width; }
    
    // Clamp to valid range
    if (start_pos < 0) { start_pos = 0; }
    if (end_pos < 0) { end_pos = 0; }
    if (start_pos >= width) { be_return_nil(vm); }
    if (end_pos >= width) { end_pos = width - 1; }
    if (end_pos < start_pos) { be_return_nil(vm); }
    
    // Set first pixel directly
    pixels_buf[start_pos] = color1;
    
    // If only one pixel, we're done
    if (start_pos == end_pos) {
      be_return_nil(vm);
    }
    
    // Set last pixel directly
    pixels_buf[end_pos] = color2;
    
    // If only two pixels, we're done
    if (end_pos - start_pos <= 1) {
      be_return_nil(vm);
    }
    
    // Extract components from color1 (ARGB format)
    uint32_t a1 = (color1 >> 24) & 0xFF;
    uint32_t r1 = (color1 >> 16) & 0xFF;
    uint32_t g1 = (color1 >>  8) & 0xFF;
    uint32_t b1 = (color1      ) & 0xFF;
    
    // Extract components from color2 (ARGB format)
    uint32_t a2 = (color2 >> 24) & 0xFF;
    uint32_t r2 = (color2 >> 16) & 0xFF;
    uint32_t g2 = (color2 >>  8) & 0xFF;
    uint32_t b2 = (color2      ) & 0xFF;
    
    // Calculate the total number of steps
    int32_t steps = end_pos - start_pos;
    
    // Fill the gradient for intermediate pixels
    for (int32_t i = start_pos + 1; i < end_pos; i++) {
      int32_t pos = i - start_pos;
      
      // Linear interpolation using changeUIntScale
      uint8_t r = changeUIntScale(pos, 0, steps, r1, r2);
      uint8_t g = changeUIntScale(pos, 0, steps, g1, g2);
      uint8_t b = changeUIntScale(pos, 0, steps, b1, b2);
      uint8_t a = changeUIntScale(pos, 0, steps, a1, a2);
      
      // Combine components into a 32-bit value (ARGB format)
      pixels_buf[i] = (a << 24) | (r << 16) | (g << 8) | b;
    }
    
    be_return_nil(vm);
  }

  // frame_buffer_ntv.blend_color(pixels:bytes(), color:int, start_pos:int, end_pos:int) -> nil
  // Blend a specific region with a solid color using the color's alpha channel
  int32_t be_animation_ntv_blend_color(bvm *vm);
  int32_t be_animation_ntv_blend_color(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    size_t pixels_len = 0;
    uint32_t * pixels_buf = (uint32_t*) be_tobytes(vm, 1, &pixels_len);
    if (pixels_buf == NULL) {
      be_raise(vm, "argument_error", "needs bytes() argument");
    }
    uint32_t color = be_toint(vm, 2);
    int32_t start_pos = 0;
    int32_t end_pos = -1;
    if (top >= 3 && be_isint(vm, 3)) {
      start_pos = be_toint(vm, 3);
    }
    if (top >= 4 && be_isint(vm, 4)) {
      end_pos = be_toint(vm, 4);
    }
    
    // Calculate pixel count
    size_t width = pixels_len / 4;
    
    // Handle negative indices (Python-style)
    if (start_pos < 0) { start_pos += width; }
    if (end_pos < 0) { end_pos += width; }
    
    // Clamp to valid range
    if (start_pos < 0) { start_pos = 0; }
    if (end_pos < 0) { end_pos = 0; }
    if (start_pos >= width) { be_return_nil(vm); }
    if (end_pos >= width) { end_pos = width - 1; }
    if (end_pos < start_pos) { be_return_nil(vm); }
    
    // Extract alpha from color
    uint32_t a2 = (color >> 24) & 0xFF;
    
    // Only blend if the color has some alpha
    if (a2 == 0) {
      be_return_nil(vm);  // Fully transparent, nothing to do
    }
    
    // Extract components from color (source)
    uint32_t r2 = (color >> 16) & 0xFF;
    uint32_t g2 = (color >>  8) & 0xFF;
    uint32_t b2 = (color      ) & 0xFF;
    
    // Blend the pixels in the specified region
    for (int32_t i = start_pos; i <= end_pos; i++) {
      uint32_t color1 = pixels_buf[i];
      
      // Extract components from color1 (destination)
      uint32_t a1 = (color1 >> 24) & 0xFF;
      uint32_t r1 = (color1 >> 16) & 0xFF;
      uint32_t g1 = (color1 >>  8) & 0xFF;
      uint32_t b1 = (color1      ) & 0xFF;
      
      // Blend RGB channels using source alpha
      uint8_t r = changeUIntScale(255 - a2, 0, 255, 0, r1) + changeUIntScale(a2, 0, 255, 0, r2);
      uint8_t g = changeUIntScale(255 - a2, 0, 255, 0, g1) + changeUIntScale(a2, 0, 255, 0, g2);
      uint8_t b = changeUIntScale(255 - a2, 0, 255, 0, b1) + changeUIntScale(a2, 0, 255, 0, b2);
      
      // Blend alpha channels: a = a1 + (255 - a1) * a2 / 255
      uint32_t a = a1 + changeUIntScale((255 - a1) * a2, 0, 255 * 255, 0, 255);
      if (a > 255) { a = 255; }
      
      // Write blended result
      pixels_buf[i] = (a << 24) | (r << 16) | (g << 8) | b;
    }
    
    be_return_nil(vm);
  }

  // frame_buffer_ntv.apply_opacity(pixels:bytes(), opacity:int|bytes(), start_pos:int, end_pos:int) -> nil
  // Apply an opacity adjustment to a region of the buffer
  // opacity can be an int (0-511) or bytes() buffer for mask mode
  int32_t be_animation_ntv_apply_opacity(bvm *vm);
  int32_t be_animation_ntv_apply_opacity(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    size_t pixels_len = 0;
    uint32_t * pixels_buf = (uint32_t*) be_tobytes(vm, 1, &pixels_len);
    if (pixels_buf == NULL) {
      be_raise(vm, "argument_error", "needs bytes() argument");
    }
    
    int32_t start_pos = 0;
    int32_t end_pos = -1;
    if (top >= 3 && be_isint(vm, 3)) {
      start_pos = be_toint(vm, 3);
    }
    if (top >= 4 && be_isint(vm, 4)) {
      end_pos = be_toint(vm, 4);
    }
    
    // Calculate pixel count
    size_t width = pixels_len / 4;
    
    // Handle negative indices (Python-style)
    if (start_pos < 0) { start_pos += width; }
    if (end_pos < 0) { end_pos += width; }
    
    // Clamp to valid range
    if (start_pos < 0) { start_pos = 0; }
    if (end_pos < 0) { end_pos = 0; }
    if (start_pos >= width) { be_return_nil(vm); }
    if (end_pos >= width) { end_pos = width - 1; }
    if (end_pos < start_pos) { be_return_nil(vm); }
    
    // Check if opacity is bytes (mask mode) or int (value mode)
    if (top >= 2 && be_isbytes(vm, 2)) {
      // Mask mode: use another buffer as opacity mask
      size_t mask_len = 0;
      const uint32_t * mask_buf = (const uint32_t*) be_tobytes(vm, 2, &mask_len);
      if (mask_buf == NULL) {
        be_raise(vm, "argument_error", "mask needs bytes() argument");
      }
      
      size_t mask_width = mask_len / 4;
      
      // Validate mask size - use smaller width
      if (mask_width < width) { width = mask_width; }
      if (end_pos >= width) { end_pos = width - 1; }
      
      // Apply mask opacity
      for (int32_t i = start_pos; i <= end_pos; i++) {
        uint32_t color = pixels_buf[i];
        uint32_t mask_color = mask_buf[i];
        
        // Extract alpha from mask as opacity factor (0-255)
        uint32_t mask_opacity = (mask_color >> 24) & 0xFF;
        
        // Extract components from color (ARGB format)
        uint32_t a = (color >> 24) & 0xFF;
        uint32_t r = (color >> 16) & 0xFF;
        uint32_t g = (color >>  8) & 0xFF;
        uint32_t b = (color      ) & 0xFF;
        
        // Apply mask opacity to alpha channel
        a = changeUIntScale(mask_opacity, 0, 255, 0, a);
        
        // Write result
        pixels_buf[i] = (a << 24) | (r << 16) | (g << 8) | b;
      }
    } else {
      // Number mode: uniform opacity adjustment
      int32_t opacity_value = 255;
      if (top >= 2 && be_isint(vm, 2)) {
        opacity_value = be_toint(vm, 2);
      }
      
      // Ensure opacity is in valid range (0-511)
      if (opacity_value < 0) { opacity_value = 0; }
      if (opacity_value > 511) { opacity_value = 511; }
      
      // Apply opacity adjustment
      for (int32_t i = start_pos; i <= end_pos; i++) {
        uint32_t color = pixels_buf[i];
        
        // Extract components (ARGB format)
        uint32_t a = (color >> 24) & 0xFF;
        uint32_t r = (color >> 16) & 0xFF;
        uint32_t g = (color >>  8) & 0xFF;
        uint32_t b = (color      ) & 0xFF;
        
        // Adjust alpha
        // For opacity 0-255: scale down alpha
        // For opacity 256-511: scale up alpha (but cap at 255)
        if (opacity_value <= 255) {
          a = changeUIntScale(opacity_value, 0, 255, 0, a);
        } else {
          // Scale up alpha: map 256-511 to 1.0-2.0 multiplier
          a = a + changeUIntScale(a * (opacity_value - 255), 0, 255 * 256, 0, 255);
          if (a > 255) { a = 255; }  // Cap at maximum alpha
        }
        
        // Write result
        pixels_buf[i] = (a << 24) | (r << 16) | (g << 8) | b;
      }
    }
    
    be_return_nil(vm);
  }

  // frame_buffer_ntv.apply_brightness(pixels:bytes(), brightness:int|bytes(), start_pos:int, end_pos:int) -> nil
  // Apply a brightness adjustment to a region of the buffer
  // brightness can be an int (0-511) or bytes() buffer for mask mode
  int32_t be_animation_ntv_apply_brightness(bvm *vm);
  int32_t be_animation_ntv_apply_brightness(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    size_t pixels_len = 0;
    uint32_t * pixels_buf = (uint32_t*) be_tobytes(vm, 1, &pixels_len);
    if (pixels_buf == NULL) {
      be_raise(vm, "argument_error", "needs bytes() argument");
    }
    
    int32_t start_pos = 0;
    int32_t end_pos = -1;
    if (top >= 3 && be_isint(vm, 3)) {
      start_pos = be_toint(vm, 3);
    }
    if (top >= 4 && be_isint(vm, 4)) {
      end_pos = be_toint(vm, 4);
    }
    
    // Calculate pixel count
    size_t width = pixels_len / 4;
    
    // Handle negative indices (Python-style)
    if (start_pos < 0) { start_pos += width; }
    if (end_pos < 0) { end_pos += width; }
    
    // Clamp to valid range
    if (start_pos < 0) { start_pos = 0; }
    if (end_pos < 0) { end_pos = 0; }
    if (start_pos >= width) { be_return_nil(vm); }
    if (end_pos >= width) { end_pos = width - 1; }
    if (end_pos < start_pos) { be_return_nil(vm); }
    
    // Check if brightness is bytes (mask mode) or int (value mode)
    if (top >= 2 && be_isbytes(vm, 2)) {
      // Mask mode: use another buffer as brightness mask
      size_t mask_len = 0;
      const uint32_t * mask_buf = (const uint32_t*) be_tobytes(vm, 2, &mask_len);
      size_t mask_width = mask_len / 4;
      
      // Validate mask size - use smaller width
      if (mask_width < width) { width = mask_width; }
      if (end_pos >= width) { end_pos = width - 1; }
      
      // Apply mask brightness
      for (int32_t i = start_pos; i <= end_pos; i++) {
        uint32_t color = pixels_buf[i];
        uint32_t mask_color = mask_buf[i];
        
        // Extract alpha from mask as brightness factor (0-255)
        uint32_t mask_brightness = (mask_color >> 24) & 0xFF;
        
        // Extract components from color (ARGB format)
        uint32_t a = (color >> 24) & 0xFF;
        uint32_t r = (color >> 16) & 0xFF;
        uint32_t g = (color >>  8) & 0xFF;
        uint32_t b = (color      ) & 0xFF;
        
        // Apply mask brightness to RGB channels
        r = changeUIntScale(mask_brightness, 0, 255, 0, r);
        g = changeUIntScale(mask_brightness, 0, 255, 0, g);
        b = changeUIntScale(mask_brightness, 0, 255, 0, b);
        
        // Write result
        pixels_buf[i] = (a << 24) | (r << 16) | (g << 8) | b;
      }
    } else {
      // Number mode: uniform brightness adjustment
      int32_t brightness_value = 255;
      if (top >= 2 && be_isint(vm, 2)) {
        brightness_value = be_toint(vm, 2);
      }
      
      // Ensure brightness is in valid range (0-511)
      if (brightness_value < 0) { brightness_value = 0; }
      if (brightness_value > 511) { brightness_value = 511; }
      
      // Apply brightness adjustment
      for (int32_t i = start_pos; i <= end_pos; i++) {
        uint32_t color = pixels_buf[i];
        
        // Extract components (ARGB format)
        uint32_t a = (color >> 24) & 0xFF;
        uint32_t r = (color >> 16) & 0xFF;
        uint32_t g = (color >>  8) & 0xFF;
        uint32_t b = (color      ) & 0xFF;
        
        // Adjust brightness
        // For brightness 0-255: scale down RGB
        // For brightness 256-511: scale up RGB (but cap at 255)
        if (brightness_value <= 255) {
          r = changeUIntScale(r, 0, 255, 0, brightness_value);
          g = changeUIntScale(g, 0, 255, 0, brightness_value);
          b = changeUIntScale(b, 0, 255, 0, brightness_value);
        } else {
          // Scale up RGB: map 256-511 to 1.0-2.0 multiplier
          int32_t multiplier = brightness_value - 255;  // 0-256 range
          r = r + changeUIntScale(r * multiplier, 0, 255 * 256, 0, 255);
          g = g + changeUIntScale(g * multiplier, 0, 255 * 256, 0, 255);
          b = b + changeUIntScale(b * multiplier, 0, 255 * 256, 0, 255);
          if (r > 255) { r = 255; }  // Cap at maximum
          if (g > 255) { g = 255; }
          if (b > 255) { b = 255; }
        }
        
        // Write result
        pixels_buf[i] = (a << 24) | (r << 16) | (g << 8) | b;
      }
    }
    
    be_return_nil(vm);
  }

  // frame_buffer_ntv.fill_pixels(pixels:bytes(), color:int, start_pos:int, end_pos:int) -> nil
  // Fill a region of the buffer with a specific color
  int32_t be_animation_ntv_fill_pixels(bvm *vm);
  int32_t be_animation_ntv_fill_pixels(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    size_t pixels_len = 0;
    uint32_t * pixels_buf = (uint32_t*) be_tobytes(vm, 1, &pixels_len);
    if (pixels_buf == NULL) {
      be_raise(vm, "argument_error", "needs bytes() argument");
    }
    uint32_t color = be_toint(vm, 2);
    int32_t start_pos = 0;
    int32_t end_pos = -1;
    if (top >= 3 && be_isint(vm, 3)) {
      start_pos = be_toint(vm, 3);
    }
    if (top >= 4 && be_isint(vm, 4)) {
      end_pos = be_toint(vm, 4);
    }
    
    // Calculate pixel count
    size_t width = pixels_len / 4;
    
    // Handle negative indices (Python-style)
    if (start_pos < 0) { start_pos += width; }
    if (end_pos < 0) { end_pos += width; }
    
    // Clamp to valid range
    if (start_pos < 0) { start_pos = 0; }
    if (end_pos < 0) { end_pos = 0; }
    if (start_pos >= width) { be_return_nil(vm); }
    if (end_pos >= width) { end_pos = width - 1; }
    if (end_pos < start_pos) { be_return_nil(vm); }
    
    // Fill the region with the color
    for (int32_t i = start_pos; i <= end_pos; i++) {
      pixels_buf[i] = color;
    }
    
    be_return_nil(vm);
  }

  // // Leds_frame.paste_pixels(neopixel:bytes(), led_buffer:bytes(), bri:int 0..100, gamma:bool)
  // //
  // // Copy from ARGB buffer to RGB
  // int32_t be_leds_paste_pixels(bvm *vm);
  // int32_t be_leds_paste_pixels(bvm *vm) {
  //   int32_t top = be_top(vm); // Get the number of arguments
  //   if (top >= 2 && be_isbytes(vm, 2)) {
  //     size_t src_len = 0;
  //     uint32_t * src_buf = (uint32_t*) be_tobytes(vm, 1, &src_len);
  //     size_t dest_len = 0;
  //     uint8_t * dest_buf = (uint8_t*) be_tobytes(vm, 2, &dest_len);

  //     uint32_t bri255 = 255;
  //     if (top >= 3 && be_isint(vm, 3)) {
  //       bri255 = be_toint(vm, 3);
  //     }
  //     bool gamma = false;
  //     if (top >= 4 && be_isbool(vm, 4)) {
  //       gamma = be_tobool(vm, 4);
  //     }

  //     size_t pixels_count = src_len / 4;
  //     if (pixels_count > dest_len / 3) { pixels_count = dest_len / 3; }
  //     if (pixels_count > 0) {
  //       for (size_t i = 0; i < pixels_count; i++) {
  //         uint32_t src_argb = ApplyBriGamma(src_buf[i], bri255, gamma);
  //         uint32_t src_r = (src_argb >> 16) & 0xFF;
  //         uint32_t src_g = (src_argb >>  8) & 0xFF;
  //         uint32_t src_b = (src_argb      ) & 0xFF;
  //         dest_buf[i * 3 + 0] = src_r;
  //         dest_buf[i * 3 + 1] = src_g;
  //         dest_buf[i * 3 + 2] = src_b;
  //       }
  //     }
  //     be_return_nil(vm);
  //   }
  //   be_raise(vm, "type_error", nullptr);
  // }


}

#endif // USE_BERRY_ANIMATION
#endif // USE_WS2812
#endif  // USE_BERRY
