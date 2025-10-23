# TasmotaLED Library Documentation

## Executive Summary

TasmotaLED is a lightweight, high-performance library for controlling addressable LED strips on ESP32 microcontrollers. It serves as a streamlined replacement for the NeoPixelBus library, focusing on efficient pixel pushing with minimal memory overhead while supporting multiple hardware acceleration methods (RMT, SPI, I2S).

**Key Features:**
- Hardware-accelerated LED control via RMT (preferred), SPI, or I2S
- Support for WS2812 and SK6812 LED strips
- 3-byte (RGB) and 4-byte (RGBW) pixel formats
- Flexible pixel ordering (GRB, RGB, RBG, BRG, BGR, GBR)
- Zero-copy buffer management for optimal performance
- ESP32 platform exclusive (ESP8266 not supported)

**Version:** 1.0  
**Author:** Stephan Hadinger  
**License:** GNU General Public License v3.0  
**Copyright:** 2024

---

## Table of Contents

1. [Architecture Overview](#architecture-overview)
2. [Class Hierarchy](#class-hierarchy)
3. [LED Type Encoding System](#led-type-encoding-system)
4. [Hardware Acceleration](#hardware-acceleration)
5. [API Reference](#api-reference)
6. [Configuration Guide](#configuration-guide)
7. [Performance Characteristics](#performance-characteristics)
8. [Usage Examples](#usage-examples)
9. [Integration with Tasmota](#integration-with-tasmota)
10. [Troubleshooting](#troubleshooting)

---

## Architecture Overview

### Design Philosophy

TasmotaLED is designed with the following principles:

1. **Minimal Memory Footprint**: Uses only two buffers (work and show) with no buffer swapping overhead
2. **Hardware Acceleration First**: Leverages ESP32's RMT, SPI, or I2S peripherals for precise timing
3. **Zero-Copy Operations**: Direct buffer manipulation without intermediate copies
4. **NeoPixelBus Compatibility**: Similar API for easy migration from NeoPixelBus
5. **Compile-Time Optimization**: Hardware support can be selectively enabled/disabled

### System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Application Layer                        │
│              (Tasmota Light Driver)                         │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────────┐
│                   TasmotaLED Class                          │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │  _buf_work   │  │  _buf_show   │  │ Pixel Format │     │
│  │  (editable)  │─▶│  (internal)  │  │  Conversion  │     │
│  └──────────────┘  └──────────────┘  └──────────────┘     │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────────┐
│              TasmotaLEDPusher (Abstract)                    │
└────────────────────┬────────────────────────────────────────┘
                     │
        ┌────────────┼────────────┐
        ▼            ▼            ▼
┌──────────────┐ ┌──────────┐ ┌──────────┐
│ RMT Pusher   │ │   SPI    │ │   I2S    │
│  (Primary)   │ │  Pusher  │ │  Pusher  │
└──────┬───────┘ └────┬─────┘ └────┬─────┘
       │              │            │
       ▼              ▼            ▼
┌─────────────────────────────────────────┐
│         ESP32 Hardware Peripherals      │
│    (RMT / SPI / I2S Controllers)        │
└─────────────────────────────────────────┘
```


### Memory Management

TasmotaLED uses a dual-buffer system:

1. **Work Buffer (`_buf_work`)**: 
   - Directly accessible by application
   - Stores pixels in WRGB or RGB format (0xWWRRGGBB or 0xRRGGBB)
   - Can be modified at any time
   - Size: `pixel_count × pixel_size` bytes

2. **Show Buffer (`_buf_show`)**:
   - Internal buffer for hardware transmission
   - Stores pixels in LED strip format (e.g., GRB, GRBW)
   - Populated during `Show()` call with format conversion
   - Size: `pixel_count × pixel_size` bytes

**Memory Allocation Example:**
- 512 RGB pixels: 2 × 512 × 3 = 3,072 bytes (~3 KB)
- 512 RGBW pixels: 2 × 512 × 4 = 4,096 bytes (~4 KB)

---

## Class Hierarchy

### Class Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                      TasmotaLED                             │
├─────────────────────────────────────────────────────────────┤
│ - _type: uint16_t                                           │
│ - _pixel_order: uint8_t                                     │
│ - _w_before: bool                                           │
│ - _timing: uint8_t                                          │
│ - _started: bool                                            │
│ - _dirty: bool                                              │
│ - _raw_format: bool                                         │
│ - _pixel_count: uint16_t                                    │
│ - _pixel_size: uint8_t                                      │
│ - _buf_work: uint8_t*                                       │
│ - _buf_show: uint8_t*                                       │
│ - _pixel_matrix: const uint8_t(*)[3]                        │
│ - _pusher: TasmotaLEDPusher*                                │
├─────────────────────────────────────────────────────────────┤
│ + TasmotaLED(type, num_leds)                                │
│ + ~TasmotaLED()                                             │
│ + Begin(): bool                                             │
│ + Show(): void                                              │
│ + SetPixelColor(index, wrgb): void                          │
│ + GetPixelColor(index): uint32_t                            │
│ + ClearTo(rgbw, first, last): void                          │
│ + SetPixelCount(num_leds): void                             │
│ + SetPixelSubType(type): void                               │
│ + SetPusher(pusher): void                                   │
│ + CanShow(): bool                                           │
│ + Pixels(): uint8_t*                                        │
│ + PixelCount(): uint16_t                                    │
│ + PixelSize(): uint8_t                                      │
│ + GetType(): uint8_t                                        │
│ + IsDirty(): bool                                           │
│ + Dirty(): void                                             │
│ + SetRawFormat(raw): void                                   │
└─────────────────────────────────────────────────────────────┘
                            │
                            │ uses
                            ▼
┌─────────────────────────────────────────────────────────────┐
│              TasmotaLEDPusher (Abstract)                    │
├─────────────────────────────────────────────────────────────┤
│ # _initialized: bool                                        │
│ # _err: esp_err_t                                           │
│ # _pixel_count: uint16_t                                    │
│ # _pixel_size: uint16_t                                     │
│ # _led_timing: const TasmotaLED_Timing*                     │
├─────────────────────────────────────────────────────────────┤
│ + Begin(pixel_count, pixel_size, timing): bool              │
│ + Push(buf): bool = 0                                       │
│ + CanShow(): bool = 0                                       │
│ + SetPixelCount(pixel_count): bool = 0                      │
│ + Initialized(): bool                                       │
│ + Error(): esp_err_t                                        │
│ + static ResolveHardware(hw): uint32_t                      │
│ + static Create(hw, gpio): TasmotaLEDPusher*                │
└─────────────────────────────────────────────────────────────┘
                            △
                            │
          ┌─────────────────┼─────────────────┐
          │                 │                 │
┌─────────┴──────────┐ ┌────┴────────┐ ┌─────┴──────────┐
│ TasmotaLEDPusherRMT│ │TasmotaLED   │ │ TasmotaLED     │
│                    │ │PusherSPI    │ │PusherI2S       │
├────────────────────┤ ├─────────────┤ ├────────────────┤
│ - _pin: int8_t     │ │- _pin: int8_t│ │(Future)        │
│ - _channel: handle │ │- _spi_strip │ │                │
│ - _led_encoder     │ │- _with_dma  │ │                │
│ - _tx_config       │ │             │ │                │
├────────────────────┤ ├─────────────┤ ├────────────────┤
│ + Push(): bool     │ │+ Push(): bool│ │                │
│ + CanShow(): bool  │ │+ CanShow()  │ │                │
└────────────────────┘ └─────────────┘ └────────────────┘
```


---

## LED Type Encoding System

### Type Encoding Structure

The LED type is encoded in a 16-bit value with the following bit layout:

```
Bits 15-8: Timing Code (WS2812, SK6812, etc.)
Bit  7:    W Channel Position (0=after RGB, 1=before RGB)
Bits 6-4:  Pixel Order (GRB, RGB, RBG, BRG, BGR, GBR)
Bits 3-0:  Bytes per Pixel (3=RGB, 4=RGBW)

┌────────┬───┬───────┬──────────┐
│15    8 │ 7 │ 6   4 │ 3      0 │
├────────┼───┼───────┼──────────┤
│ Timing │ W │ Order │   Size   │
└────────┴───┴───────┴──────────┘
```

### Pixel Size Encoding (Bits 0-3)

| Value | Enum | Description | Bytes per Pixel |
|-------|------|-------------|-----------------|
| `0x0` | `TasmotaLed_1_Def` | Default (same as RGB) | 3 |
| `0x1` | `TasmotaLed_3_RGB` | RGB format | 3 |
| `0x2` | `TasmotaLed_4_WRGB` | RGBW format | 4 |

### Pixel Order Encoding (Bits 4-6)

| Value | Enum | Description | Channel Order |
|-------|------|-------------|---------------|
| `0b000` | `TasmotaLed_Def` | Default (GRB) | G, R, B |
| `0b001` | `TasmotaLed_GRB` | Green-Red-Blue | G, R, B |
| `0b010` | `TasmotaLed_RGB` | Red-Green-Blue | R, G, B |
| `0b011` | `TasmotaLed_RBG` | Red-Blue-Green | R, B, G |
| `0b100` | `TasmotaLed_BRG` | Blue-Red-Green | B, R, G |
| `0b101` | `TasmotaLed_BGR` | Blue-Green-Red | B, G, R |
| `0b110` | `TasmotaLed_GBR` | Green-Blue-Red | G, B, R |

### W Channel Position (Bit 7)

| Value | Enum | Description | Format |
|-------|------|-------------|--------|
| `0` | `TasmotaLed_xxxW` | W after color | RGB + W |
| `1` | `TasmotaLed_Wxxx` | W before color | W + RGB |

### Timing Code (Bits 8-15)

| Value | Enum | Description | T0H | T0L | T1H | T1L | Reset |
|-------|------|-------------|-----|-----|-----|-----|-------|
| `0` | `TasmotaLed_WS2812` | WS2812/WS2812B | 400ns | 850ns | 800ns | 450ns | 80µs |
| `1` | `TasmotaLed_SK6812` | SK6812 | 300ns | 900ns | 600ns | 600ns | 80µs |

### Predefined LED Types

```cpp
enum TasmotaLEDTypes : uint16_t {
  // WS2812 with GRB ordering (most common)
  ws2812_grb  = TasmotaLed_3_RGB | TasmotaLed_GRB | TasmotaLed_WS2812,
  // Value: 0x0011 (decimal 17)
  
  // SK6812 with GRBW ordering and W after RGB
  sk6812_grbw = TasmotaLed_4_WRGB | TasmotaLed_GRB | TasmotaLed_xxxW | TasmotaLed_SK6812,
  // Value: 0x0112 (decimal 274)
  
  // SK6812 with GRB ordering (no white channel)
  sk6812_grb  = TasmotaLed_3_RGB | TasmotaLed_GRB | TasmotaLed_SK6812,
  // Value: 0x0111 (decimal 273)
};
```

### Type Encoding Examples

```cpp
// WS2812 RGB strip with GRB ordering
uint16_t type1 = TasmotaLed_3_RGB | TasmotaLed_GRB | TasmotaLed_WS2812;
// Binary: 0000 0000 0001 0001 = 0x0011

// SK6812 RGBW strip with RGB ordering and W first
uint16_t type2 = TasmotaLed_4_WRGB | TasmotaLed_RGB | TasmotaLed_Wxxx | TasmotaLed_SK6812;
// Binary: 0000 0001 1010 0010 = 0x01A2

// WS2812 RGB strip with BGR ordering
uint16_t type3 = TasmotaLed_3_RGB | TasmotaLed_BGR | TasmotaLed_WS2812;
// Binary: 0000 0000 0101 0001 = 0x0051
```


---

## Hardware Acceleration

### Hardware Support Matrix

| Hardware | ESP32 | ESP32-S2 | ESP32-S3 | ESP32-C3 | ESP32-C2 | ESP32-C6 |
|----------|-------|----------|----------|----------|----------|----------|
| **RMT** | ✅ | ✅ | ✅ | ✅ | ❌ | ✅ |
| **SPI** | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| **I2S** | ✅ | ✅ | ✅ | ❌ | ❌ | ❌ |

### Hardware Selection

Hardware acceleration is selected using the `TasmotaLEDHardware` enum:

```cpp
enum TasmotaLEDHardware : uint32_t {
  TasmotaLed_HW_Default = 0x000000,  // Auto-select best available
  TasmotaLed_RMT  = (1 << 0) << 16,  // 0x010000 - RMT peripheral
  TasmotaLed_SPI  = (1 << 1) << 16,  // 0x020000 - SPI peripheral
  TasmotaLed_I2S  = (1 << 2) << 16,  // 0x040000 - I2S peripheral
  TasmotaLed_HW_None = 0xFF << 16,   // 0xFF0000 - No hardware support
};
```

### Hardware Resolution Priority

When `TasmotaLed_HW_Default` is specified, the library selects hardware in this order:

1. **RMT** (if available) - Preferred for precision and low CPU overhead
2. **I2S** (if available) - Good for large strips
3. **SPI** (if available) - Fallback option, works on all ESP32 variants

### Compile-Time Configuration

Hardware support can be enabled/disabled at compile time:

```cpp
// In your build configuration or before including TasmotaLED
#define TASMOTALED_HARDWARE_RMT 1  // Enable RMT (default: 1)
#define TASMOTALED_HARDWARE_I2S 0  // Enable I2S (default: 0)
#define TASMOTALED_HARDWARE_SPI 0  // Enable SPI (default: 0)
```

**Note:** If no hardware is explicitly enabled, SPI is automatically enabled as fallback.

### RMT Implementation

**Advantages:**
- Precise timing control (25ns resolution at 40MHz)
- Low CPU overhead
- Non-blocking operation with DMA
- Supports up to 8 channels (ESP32) or 4 channels (ESP32-S2/S3/C3)

**Technical Details:**
- Clock resolution: 40 MHz (25ns per tick)
- Memory block: 192 symbols (768 bytes)
- Transaction queue depth: 4
- Encoder: Custom LED strip encoder with bit-level control

**Timing Precision:**
```
WS2812: T0H=400ns (16 ticks), T0L=850ns (34 ticks)
        T1H=800ns (32 ticks), T1L=450ns (18 ticks)
        Reset=80µs (3200 ticks)

SK6812: T0H=300ns (12 ticks), T0L=900ns (36 ticks)
        T1H=600ns (24 ticks), T1L=600ns (24 ticks)
        Reset=80µs (3200 ticks)
```

### SPI Implementation

**Advantages:**
- Available on all ESP32 variants
- Works on ESP32-C2 (no RMT support)
- Reliable fallback option

**Technical Details:**
- Clock frequency: 2.5 MHz
- Each LED bit encoded as 3 SPI bits
- Bit encoding: `0` = `100`, `1` = `110`
- DMA support: Optional (auto-selected)
- Memory overhead: 3× pixel buffer size

**Bit Encoding:**
```
LED bit 0: SPI bits 100 (high for 400ns, low for 800ns at 2.5MHz)
LED bit 1: SPI bits 110 (high for 800ns, low for 400ns at 2.5MHz)
```

### I2S Implementation

**Status:** Planned but not yet implemented

**Advantages:**
- Very high throughput
- Excellent for large LED installations
- Parallel data transmission

---

## API Reference

### TasmotaLED Class

#### Constructor

```cpp
TasmotaLED(uint16_t type, uint16_t num_leds)
```

Creates a new TasmotaLED instance.

**Parameters:**
- `type`: LED type encoding (see LED Type Encoding section)
- `num_leds`: Number of LEDs in the strip

**Example:**
```cpp
// Create a 60-LED WS2812 strip with GRB ordering
TasmotaLED strip(ws2812_grb, 60);

// Create a 100-LED SK6812 RGBW strip
TasmotaLED strip(sk6812_grbw, 100);
```

#### Destructor

```cpp
~TasmotaLED()
```

Cleans up resources, frees buffers, and deletes the pusher instance.

#### Initialization Methods

##### SetPusher()

```cpp
void SetPusher(TasmotaLEDPusher *pusher)
```

Sets the hardware pusher implementation. Must be called before `Begin()`.

**Parameters:**
- `pusher`: Pointer to a TasmotaLEDPusher instance (RMT, SPI, or I2S)

**Example:**
```cpp
TasmotaLED strip(ws2812_grb, 60);
TasmotaLEDPusher *pusher = TasmotaLEDPusher::Create(TasmotaLed_RMT, 5);
strip.SetPusher(pusher);
```

##### Begin()

```cpp
bool Begin(void)
```

Initializes the hardware pusher and prepares the strip for operation.

**Returns:** `true` if initialization successful, `false` otherwise

**Example:**
```cpp
if (strip.Begin()) {
  // Strip is ready to use
} else {
  // Initialization failed
}
```

#### Pixel Manipulation Methods

##### SetPixelColor()

```cpp
void SetPixelColor(int32_t index, uint32_t wrgb)
```

Sets the color of a single pixel.

**Parameters:**
- `index`: Pixel index (0-based, negative values count from end)
- `wrgb`: Color value in 0xWWRRGGBB format (or 0xRRGGBB for RGB strips)

**Example:**
```cpp
strip.SetPixelColor(0, 0xFF0000);      // Red
strip.SetPixelColor(1, 0x00FF00);      // Green
strip.SetPixelColor(2, 0x0000FF);      // Blue
strip.SetPixelColor(-1, 0xFFFFFF);     // Last pixel white
strip.SetPixelColor(10, 0x80FF8000);   // RGBW: W=0x80, R=0xFF, G=0x80, B=0x00
```

##### GetPixelColor()

```cpp
uint32_t GetPixelColor(int32_t index)
```

Retrieves the color of a single pixel.

**Parameters:**
- `index`: Pixel index (0-based, negative values count from end)

**Returns:** Color value in 0xWWRRGGBB or 0xRRGGBB format

**Example:**
```cpp
uint32_t color = strip.GetPixelColor(5);
uint8_t red = (color >> 16) & 0xFF;
uint8_t green = (color >> 8) & 0xFF;
uint8_t blue = color & 0xFF;
```

##### ClearTo()

```cpp
void ClearTo(uint32_t rgbw, int32_t first = 0, int32_t last = -1)
```

Sets a range of pixels to the same color.

**Parameters:**
- `rgbw`: Color value in 0xWWRRGGBB or 0xRRGGBB format
- `first`: First pixel index (default: 0)
- `last`: Last pixel index (default: -1 = last pixel)

**Example:**
```cpp
strip.ClearTo(0x000000);           // Clear entire strip to black
strip.ClearTo(0xFF0000, 0, 9);     // Set first 10 pixels to red
strip.ClearTo(0x00FF00, 10, 19);   // Set pixels 10-19 to green
strip.ClearTo(0x0000FF, -10, -1);  // Set last 10 pixels to blue
```

##### Show()

```cpp
void Show(void)
```

Pushes the pixel buffer to the LED strip. Converts from WRGB/RGB format to the strip's native format and transmits via hardware.

**Example:**
```cpp
strip.SetPixelColor(0, 0xFF0000);
strip.SetPixelColor(1, 0x00FF00);
strip.Show();  // Update the strip
```

##### CanShow()

```cpp
bool CanShow(void) const
```

Checks if the strip is ready to accept a new `Show()` command.

**Returns:** `true` if ready, `false` if previous transmission still in progress

**Example:**
```cpp
if (strip.CanShow()) {
  strip.Show();
}
```

#### Configuration Methods

##### SetPixelCount()

```cpp
void SetPixelCount(uint16_t num_leds)
```

Changes the number of LEDs in the strip. Reallocates buffers if necessary.

**Parameters:**
- `num_leds`: New number of LEDs

**Example:**
```cpp
strip.SetPixelCount(100);  // Resize to 100 LEDs
```

##### SetPixelSubType()

```cpp
void SetPixelSubType(uint8_t type)
```

Changes only the pixel ordering and size (lower 8 bits of type), keeping timing unchanged.

**Parameters:**
- `type`: New subtype (bits 0-7 of type encoding)

**Example:**
```cpp
// Change from GRB to RGB ordering
strip.SetPixelSubType(TasmotaLed_3_RGB | TasmotaLed_RGB);
```

##### SetRawFormat()

```cpp
void SetRawFormat(bool raw_format)
```

Enables/disables raw format mode. When enabled, buffer is copied directly without format conversion.

**Parameters:**
- `raw_format`: `true` for raw mode, `false` for automatic conversion

**Example:**
```cpp
strip.SetRawFormat(true);  // Disable format conversion
```

#### Query Methods

##### PixelCount()

```cpp
uint16_t PixelCount(void) const
```

Returns the number of LEDs in the strip.

##### PixelSize()

```cpp
uint8_t PixelSize(void) const
```

Returns the number of bytes per pixel (3 for RGB, 4 for RGBW).

##### GetType()

```cpp
uint8_t GetType(void) const
```

Returns the LED type encoding.

##### Pixels()

```cpp
uint8_t * Pixels(void) const
```

Returns a pointer to the work buffer for direct manipulation.

**Example:**
```cpp
uint8_t *buf = strip.Pixels();
// Direct buffer access (advanced usage)
buf[0] = 0xFF;  // Set first byte
```

##### IsDirty()

```cpp
bool IsDirty(void) const
```

Returns the dirty flag (for NeoPixelBus compatibility, but not used internally).

##### Dirty()

```cpp
void Dirty(void)
```

Sets the dirty flag to `true`.


### TasmotaLEDPusher Class (Abstract)

#### Static Factory Methods

##### Create()

```cpp
static TasmotaLEDPusher * Create(uint32_t hw, int8_t gpio)
```

Creates a pusher instance for the specified hardware type.

**Parameters:**
- `hw`: Hardware type (TasmotaLed_RMT, TasmotaLed_SPI, TasmotaLed_I2S, or TasmotaLed_HW_Default)
- `gpio`: GPIO pin number for LED data output

**Returns:** Pointer to pusher instance, or `nullptr` if creation failed

**Example:**
```cpp
TasmotaLEDPusher *pusher = TasmotaLEDPusher::Create(TasmotaLed_RMT, 5);
if (pusher) {
  strip.SetPusher(pusher);
}
```

##### ResolveHardware()

```cpp
static uint32_t ResolveHardware(uint32_t hw)
```

Resolves hardware type based on SOC capabilities.

**Parameters:**
- `hw`: Requested hardware type

**Returns:** Resolved hardware type (removes unsupported flags)

#### Instance Methods

##### Begin()

```cpp
virtual bool Begin(uint16_t pixel_count, uint16_t pixel_size, 
                   const TasmotaLED_Timing * led_timing)
```

Initializes the hardware pusher.

**Parameters:**
- `pixel_count`: Number of LEDs
- `pixel_size`: Bytes per pixel (3 or 4)
- `led_timing`: Pointer to timing structure

**Returns:** `true` if successful

##### Push()

```cpp
virtual bool Push(uint8_t *buf) = 0
```

Pushes pixel data to the LED strip (pure virtual).

**Parameters:**
- `buf`: Pointer to pixel buffer in strip format

**Returns:** `true` if successful

##### CanShow()

```cpp
virtual bool CanShow(void) = 0
```

Checks if hardware is ready for next transmission (pure virtual).

**Returns:** `true` if ready

##### SetPixelCount()

```cpp
virtual bool SetPixelCount(uint16_t pixel_count) = 0
```

Updates pixel count (pure virtual).

**Parameters:**
- `pixel_count`: New number of pixels

**Returns:** `true` if successful

##### Initialized()

```cpp
bool Initialized(void) const
```

Returns initialization status.

##### Error()

```cpp
esp_err_t Error(void) const
```

Returns last error code.

---

## Configuration Guide

### Basic Setup

#### Step 1: Include Headers

```cpp
#include "TasmotaLED.h"
#include "TasmotaLEDPusher.h"
```

#### Step 2: Create Strip Instance

```cpp
// For WS2812 RGB strip with 60 LEDs
TasmotaLED strip(ws2812_grb, 60);

// For SK6812 RGBW strip with 100 LEDs
TasmotaLED strip(sk6812_grbw, 100);

// Custom configuration
uint16_t custom_type = TasmotaLed_3_RGB | TasmotaLed_RGB | TasmotaLed_WS2812;
TasmotaLED strip(custom_type, 144);
```

#### Step 3: Create and Assign Pusher

```cpp
// Auto-select best hardware for GPIO 5
TasmotaLEDPusher *pusher = TasmotaLEDPusher::Create(TasmotaLed_HW_Default, 5);
if (pusher) {
  strip.SetPusher(pusher);
} else {
  // Handle error
}

// Or explicitly request RMT
TasmotaLEDPusher *pusher = TasmotaLEDPusher::Create(TasmotaLed_RMT, 5);
```

#### Step 4: Initialize

```cpp
if (!strip.Begin()) {
  // Handle initialization error
  Serial.println("Failed to initialize LED strip");
}
```

#### Step 5: Use the Strip

```cpp
// Set colors
strip.ClearTo(0x000000);           // Clear to black
strip.SetPixelColor(0, 0xFF0000);  // First pixel red
strip.SetPixelColor(1, 0x00FF00);  // Second pixel green
strip.Show();                       // Update strip
```

### Advanced Configuration

#### Custom LED Type

```cpp
// Create custom type: RGB, BGR ordering, WS2812 timing
uint16_t custom_type = TasmotaLed_3_RGB |      // 3 bytes per pixel
                       TasmotaLed_BGR |        // BGR ordering
                       TasmotaLed_WS2812;      // WS2812 timing

TasmotaLED strip(custom_type, 60);
```

#### RGBW with W Channel First

```cpp
// SK6812 RGBW with W channel before RGB
uint16_t rgbw_type = TasmotaLed_4_WRGB |       // 4 bytes per pixel
                     TasmotaLed_RGB |          // RGB ordering
                     TasmotaLed_Wxxx |         // W first
                     TasmotaLed_SK6812;        // SK6812 timing

TasmotaLED strip(rgbw_type, 100);
```

#### Multiple Strips

```cpp
// Strip 1 on GPIO 5
TasmotaLED strip1(ws2812_grb, 60);
TasmotaLEDPusher *pusher1 = TasmotaLEDPusher::Create(TasmotaLed_RMT, 5);
strip1.SetPusher(pusher1);
strip1.Begin();

// Strip 2 on GPIO 18
TasmotaLED strip2(sk6812_grbw, 100);
TasmotaLEDPusher *pusher2 = TasmotaLEDPusher::Create(TasmotaLed_RMT, 18);
strip2.SetPusher(pusher2);
strip2.Begin();
```

#### Raw Format Mode

```cpp
strip.SetRawFormat(true);  // Disable automatic format conversion

// Now buffer must be in strip's native format (e.g., GRB for WS2812)
uint8_t *buf = strip.Pixels();
buf[0] = 0xFF;  // Green
buf[1] = 0x00;  // Red
buf[2] = 0x00;  // Blue
strip.Show();
```

### Compile-Time Configuration

#### Enable/Disable Hardware Support

In your `platformio.ini` or build flags:

```ini
build_flags = 
  -DTASMOTALED_HARDWARE_RMT=1
  -DTASMOTALED_HARDWARE_SPI=1
  -DTASMOTALED_HARDWARE_I2S=0
```

Or in your code before including TasmotaLED:

```cpp
#define TASMOTALED_HARDWARE_RMT 1
#define TASMOTALED_HARDWARE_SPI 0
#define TASMOTALED_HARDWARE_I2S 0

#include "TasmotaLED.h"
```


---

## Performance Characteristics

### Memory Usage

#### Per-Strip Overhead

| Component | Size | Description |
|-----------|------|-------------|
| TasmotaLED object | ~60 bytes | Class instance |
| Work buffer | `pixels × size` | Editable buffer (RGB or RGBW) |
| Show buffer | `pixels × size` | Internal transmission buffer |
| Pusher object | ~40-80 bytes | Hardware implementation |
| **Total** | **~100 + (2 × pixels × size)** | |

#### Examples

| Configuration | Memory Usage |
|---------------|--------------|
| 60 RGB pixels | ~460 bytes |
| 144 RGB pixels | ~964 bytes |
| 300 RGB pixels | ~1,900 bytes |
| 512 RGB pixels | ~3,172 bytes |
| 60 RGBW pixels | ~580 bytes |
| 144 RGBW pixels | ~1,252 bytes |

#### SPI Additional Overhead

SPI requires 3× buffer for encoding:

| Configuration | Additional Memory |
|---------------|-------------------|
| 60 RGB pixels | +540 bytes |
| 144 RGB pixels | +1,296 bytes |
| 300 RGB pixels | +2,700 bytes |

### Timing Performance

#### Buffer Operations (512 RGB pixels)

| Operation | Time | Notes |
|-----------|------|-------|
| `ClearTo(0x000000)` | ~15 µs | Optimized memset |
| `ClearTo(0xFF0000)` | ~124 µs | Per-pixel write |
| `SetPixelColor()` | ~0.2 µs | Single pixel |
| Format conversion | ~124 µs | RGB→GRB conversion |
| `Show()` (RMT) | ~16.2 ms | Actual transmission |
| `Show()` (SPI) | ~16.6 ms | Transmission + encoding |

#### Transmission Times

Transmission time depends on pixel count and timing:

**WS2812 (30 µs per pixel):**
- 60 pixels: ~1.8 ms
- 144 pixels: ~4.3 ms
- 300 pixels: ~9.0 ms
- 512 pixels: ~15.4 ms

**SK6812 (30 µs per pixel):**
- Similar to WS2812

**Maximum Refresh Rate:**
- 60 pixels: ~555 Hz
- 144 pixels: ~232 Hz
- 300 pixels: ~111 Hz
- 512 pixels: ~65 Hz

### CPU Overhead

#### RMT Implementation
- **Setup:** ~100 µs (one-time)
- **Per Show():** ~150 µs (format conversion + DMA setup)
- **During transmission:** Near zero (DMA handles transfer)
- **CPU usage:** <1% during transmission

#### SPI Implementation
- **Setup:** ~200 µs (one-time)
- **Per Show():** ~550 µs (encoding + format conversion)
- **During transmission:** Near zero (DMA handles transfer)
- **CPU usage:** <2% during transmission

### Optimization Tips

1. **Minimize Show() Calls**
   ```cpp
   // Bad: Multiple Show() calls
   for (int i = 0; i < 60; i++) {
     strip.SetPixelColor(i, color);
     strip.Show();  // 60 transmissions!
   }
   
   // Good: Single Show() call
   for (int i = 0; i < 60; i++) {
     strip.SetPixelColor(i, color);
   }
   strip.Show();  // 1 transmission
   ```

2. **Use ClearTo() for Bulk Operations**
   ```cpp
   // Faster than loop with SetPixelColor()
   strip.ClearTo(0xFF0000, 0, 29);  // First 30 pixels red
   ```

3. **Check CanShow() Before Show()**
   ```cpp
   if (strip.CanShow()) {
     strip.Show();
   }
   ```

4. **Direct Buffer Access for Complex Patterns**
   ```cpp
   uint8_t *buf = strip.Pixels();
   // Direct manipulation (advanced)
   for (int i = 0; i < strip.PixelCount() * strip.PixelSize(); i++) {
     buf[i] = pattern[i];
   }
   strip.Show();
   ```

---

## Usage Examples

### Basic Initialization

```cpp
#include "TasmotaLED.h"
#include "TasmotaLEDPusher.h"

// Create strip instance
TasmotaLED strip(ws2812_grb, 60);

void setup() {
  // Create pusher with auto hardware selection
  TasmotaLEDPusher *pusher = TasmotaLEDPusher::Create(TasmotaLed_HW_Default, 5);
  if (pusher) {
    strip.SetPusher(pusher);
    if (strip.Begin()) {
      // Strip ready
      strip.ClearTo(0x000000);  // Clear to black
      strip.Show();
    }
  }
}
```

### Setting Pixels

```cpp
// Set individual pixels
strip.SetPixelColor(0, 0xFF0000);   // Red
strip.SetPixelColor(1, 0x00FF00);   // Green
strip.SetPixelColor(2, 0x0000FF);   // Blue

// Set range of pixels
strip.ClearTo(0xFF0000, 0, 9);      // First 10 pixels red

// Update strip
strip.Show();
```

### RGBW Control

```cpp
TasmotaLED strip(sk6812_grbw, 60);

// Pure white using W channel
strip.ClearTo(0xFF000000);  // W=0xFF, RGB=0

// Warm white (W + Red)
strip.ClearTo(0x80FF0000);  // W=0x80, R=0xFF, G=0, B=0

// RGB color (no W)
strip.ClearTo(0x00FF00FF);  // W=0, R=0xFF, G=0, B=0xFF (Magenta)

strip.Show();
```

### Non-Blocking Updates

```cpp
void loop() {
  // Check if strip is ready before updating
  if (strip.CanShow()) {
    // Update pixels
    for (int i = 0; i < strip.PixelCount(); i++) {
      strip.SetPixelColor(i, colors[i]);
    }
    strip.Show();
  }
  
  // Other code continues without blocking
}
```


---

## Integration with Tasmota

### Tasmota Light Driver Integration

TasmotaLED is designed to integrate seamlessly with Tasmota's light driver system. Here's how it fits into the Tasmota architecture:

#### Integration Points

1. **Driver Initialization (FUNC_INIT)**
   ```cpp
   void TasmotaLEDInit(void) {
     // Create strip based on Settings
     uint16_t led_type = Settings->light_type;
     uint16_t led_count = Settings->light_pixels;
     int8_t gpio = Pin(GPIO_LED);
     
     strip = new TasmotaLED(led_type, led_count);
     
     // Create pusher with hardware auto-selection
     TasmotaLEDPusher *pusher = TasmotaLEDPusher::Create(
       TasmotaLed_HW_Default, gpio);
     
     if (pusher) {
       strip->SetPusher(pusher);
       if (strip->Begin()) {
         AddLog(LOG_LEVEL_INFO, "LED: Initialized %d pixels on GPIO %d", 
                led_count, gpio);
       }
     }
   }
   ```

2. **Pixel Updates (FUNC_SET_CHANNELS)**
   ```cpp
   void TasmotaLEDUpdate(void) {
     // Get color data from Tasmota light engine
     for (int i = 0; i < strip->PixelCount(); i++) {
       uint32_t color = GetPixelColorFromLightEngine(i);
       strip->SetPixelColor(i, color);
     }
     
     // Push to hardware
     if (strip->CanShow()) {
       strip->Show();
     }
   }
   ```

3. **Configuration Changes**
   ```cpp
   void TasmotaLEDReconfigure(void) {
     // Handle pixel count changes
     if (new_pixel_count != strip->PixelCount()) {
       strip->SetPixelCount(new_pixel_count);
     }
     
     // Handle type changes
     if (new_led_type != strip->GetType()) {
       strip->SetPixelSubType(new_led_type & 0xFF);
     }
   }
   ```

### Tasmota Commands

When integrated with Tasmota, these commands control the LED strip:

| Command | Description | Example |
|---------|-------------|---------|
| `Pixels` | Set number of LEDs | `Pixels 60` |
| `Scheme` | Set color scheme | `Scheme 2` |
| `Speed` | Set animation speed | `Speed 10` |
| `Width` | Set effect width | `Width 1` |
| `Color` | Set RGB color | `Color #FF0000` |
| `White` | Set white channel | `White 50` |
| `Dimmer` | Set brightness | `Dimmer 75` |
| `Fade` | Enable fade | `Fade 1` |

### Settings Storage

Tasmota stores LED configuration in Settings structure:

```cpp
struct SETTINGS {
  // ...
  uint16_t light_pixels;        // Number of LEDs
  uint8_t  light_type;          // LED type encoding
  uint8_t  light_scheme;        // Animation scheme
  uint8_t  light_speed;         // Animation speed
  uint8_t  light_width;         // Effect width
  // ...
};
```

### Hardware Selection in Tasmota

Tasmota automatically selects the best hardware based on:

1. **SOC Capabilities**: Checks what hardware is available
2. **GPIO Availability**: Ensures GPIO is not in use
3. **Performance Requirements**: Selects RMT for best performance
4. **Fallback**: Uses SPI if RMT unavailable

```cpp
uint32_t TasmotaSelectHardware(void) {
  uint32_t hw = TasmotaLed_HW_Default;
  
  // Force SPI on ESP32-C2 (no RMT)
  #ifdef CONFIG_IDF_TARGET_ESP32C2
    hw = TasmotaLed_SPI;
  #endif
  
  // User override via SetOption
  if (Settings->flag5.led_use_spi) {
    hw = TasmotaLed_SPI;
  }
  
  return hw;
}
```

---

## Troubleshooting

### Common Issues

#### 1. Strip Not Lighting Up

**Symptoms:** No LEDs light up after calling `Show()`

**Possible Causes:**
- Pusher not initialized
- Wrong GPIO pin
- Incorrect LED type
- Power supply issues

**Solutions:**
```cpp
// Check initialization
if (!strip.Begin()) {
  Serial.println("Failed to initialize");
  // Check pusher creation
  if (pusher == nullptr) {
    Serial.println("Pusher creation failed");
  }
}

// Verify GPIO
Serial.printf("Using GPIO %d\n", gpio_pin);

// Test with simple pattern
strip.ClearTo(0xFF0000);  // All red
strip.Show();
```

#### 2. Wrong Colors

**Symptoms:** Colors appear incorrect (e.g., red shows as green)

**Possible Causes:**
- Incorrect pixel ordering
- Wrong LED type

**Solutions:**
```cpp
// Try different orderings
strip.SetPixelSubType(TasmotaLed_3_RGB | TasmotaLed_RGB);  // RGB
strip.SetPixelSubType(TasmotaLed_3_RGB | TasmotaLed_GRB);  // GRB
strip.SetPixelSubType(TasmotaLed_3_RGB | TasmotaLed_BGR);  // BGR

// Test each ordering
strip.ClearTo(0xFF0000);  // Should be red
strip.Show();
```

#### 3. Flickering or Glitches

**Symptoms:** Random flickering, incorrect colors, or partial updates

**Possible Causes:**
- Timing issues
- Insufficient power
- Long cable runs
- Electromagnetic interference

**Solutions:**
```cpp
// Try different timing
uint16_t type_ws2812 = TasmotaLed_3_RGB | TasmotaLed_GRB | TasmotaLed_WS2812;
uint16_t type_sk6812 = TasmotaLed_3_RGB | TasmotaLed_GRB | TasmotaLed_SK6812;

// Check power supply (5V, sufficient amperage)
// Add capacitor (1000µF) across power supply
// Keep data cable short (<1m) or use level shifter
```

#### 4. Memory Allocation Failures

**Symptoms:** `Begin()` returns false, crashes, or resets

**Possible Causes:**
- Too many LEDs for available memory
- Memory fragmentation

**Solutions:**
```cpp
// Check available heap
Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());

// Calculate required memory
uint32_t required = strip.PixelCount() * strip.PixelSize() * 2;
Serial.printf("Required: %d bytes\n", required);

// Reduce pixel count if necessary
if (required > ESP.getFreeHeap() / 2) {
  strip.SetPixelCount(smaller_count);
}
```

#### 5. Performance Issues

**Symptoms:** Slow updates, low frame rate

**Possible Causes:**
- Too many `Show()` calls
- Inefficient pixel updates
- Wrong hardware selection

**Solutions:**
```cpp
// Batch updates
for (int i = 0; i < strip.PixelCount(); i++) {
  strip.SetPixelColor(i, colors[i]);
}
strip.Show();  // Single show call

// Use ClearTo() for bulk operations
strip.ClearTo(0xFF0000, 0, 29);  // Faster than loop

// Check hardware
Serial.printf("Using hardware: 0x%08X\n", hw_type);

// Verify CanShow() before Show()
if (strip.CanShow()) {
  strip.Show();
}
```

### Debug Logging

Enable debug logging to diagnose issues:

```cpp
// In Tasmota, set log level
SerialLog 4  // Debug level

// Look for these messages:
// LED: RMT gpio 5
// LED: SPI gpio 5
// LED: Error create RMT bus failed 5 err=XXX
```

### Hardware-Specific Issues

#### ESP32-C2
- Only SPI supported (no RMT)
- Ensure `TASMOTALED_HARDWARE_SPI` is enabled

#### ESP32-S2/S3
- RMT channels limited to 4
- Check channel availability if using multiple strips

#### ESP32 Classic
- RMT channels limited to 8
- Most flexible hardware support

### Timing Verification

Verify timing with oscilloscope or logic analyzer:

**WS2812 Expected:**
- T0H: 400ns ±150ns
- T0L: 850ns ±150ns
- T1H: 800ns ±150ns
- T1L: 450ns ±150ns
- Reset: >50µs

**SK6812 Expected:**
- T0H: 300ns ±150ns
- T0L: 900ns ±150ns
- T1H: 600ns ±150ns
- T1L: 600ns ±150ns
- Reset: >80µs

---

## Appendix

### Pixel Order Matrix

The library uses a lookup table for pixel ordering:

```cpp
static const uint8_t TASMOTALED_CHANNEL_ORDERS[8][3] = {
  {1, 0, 2},  // Def=GRB (0)
  {1, 0, 2},  // GRB (1)
  {0, 1, 2},  // RGB (2)
  {0, 2, 1},  // RBG (3)
  {2, 1, 0},  // BRG (4)
  {1, 2, 0},  // BGR (5)
  {2, 0, 1},  // GBR (6)
  {1, 0, 2}   // GRB (7) - fallback
};
```

**Interpretation:**
- Index 0: Position of Red channel
- Index 1: Position of Green channel
- Index 2: Position of Blue channel

**Example:** GRB ordering `{1, 0, 2}` means:
- Red goes to position 1
- Green goes to position 0
- Blue goes to position 2
- Result: G(0) R(1) B(2)

### Timing Structure

```cpp
typedef struct TasmotaLED_Timing {
  uint16_t T0H;    // Bit 0 high time (nanoseconds)
  uint16_t T0L;    // Bit 0 low time (nanoseconds)
  uint16_t T1H;    // Bit 1 high time (nanoseconds)
  uint16_t T1L;    // Bit 1 low time (nanoseconds)
  uint32_t Reset;  // Reset time (nanoseconds)
} TasmotaLED_Timing;
```

### Comparison with NeoPixelBus

| Feature | TasmotaLED | NeoPixelBus |
|---------|------------|-------------|
| **Memory** | Lower (2 buffers) | Higher (3+ buffers) |
| **Code Size** | ~15 KB | ~40 KB |
| **Hardware** | RMT, SPI, I2S | RMT, I2S, UART, BitBang |
| **ESP8266** | ❌ No | ✅ Yes |
| **ESP32** | ✅ Yes | ✅ Yes |
| **Buffer Swap** | No (copy on Show) | Yes (double buffering) |
| **Dirty Flag** | Ignored | Used |
| **API** | Similar | Full-featured |
| **Performance** | Optimized | Feature-rich |

### License

```
TasmotaLED - Lightweight implementation for addressable LEDs
Copyright (C) 2024 Stephan Hadinger

This library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
```

---

## Revision History

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | 2024 | Initial release with RMT and SPI support |

---

**End of Documentation**
