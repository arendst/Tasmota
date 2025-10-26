# TasmotaLED

A lightweight, high-performance library for controlling addressable LED strips on ESP32 microcontrollers.

## Overview

TasmotaLED is a streamlined replacement for NeoPixelBus, designed specifically for the Tasmota firmware. It focuses on efficient pixel pushing with minimal memory overhead while supporting multiple hardware acceleration methods.

**Key Features:**
- Hardware-accelerated via RMT (preferred), SPI, or I2S
- Support for WS2812 and SK6812 LED strips
- 3-byte (RGB) and 4-byte (RGBW) pixel formats
- Flexible pixel ordering (GRB, RGB, RBG, BRG, BGR, GBR)
- Minimal memory footprint (2 buffers only)
- ESP32 platform exclusive

## Quick Start

```cpp
#include "TasmotaLED.h"
#include "TasmotaLEDPusher.h"

// Create 60-LED WS2812 strip on GPIO 5
TasmotaLED strip(ws2812_grb, 60);

void setup() {
  TasmotaLEDPusher *pusher = TasmotaLEDPusher::Create(TasmotaLed_HW_Default, 5);
  strip.SetPusher(pusher);
  strip.Begin();
  
  // Set colors and display
  strip.ClearTo(0xFF0000);  // All red
  strip.Show();
}
```

## Architecture

### Class Hierarchy

```
┌─────────────────────────────────────┐
│          TasmotaLED                 │
│  ┌──────────────┐  ┌──────────────┐│
│  │  _buf_work   │  │  _buf_show   ││
│  │  (editable)  │─▶│  (internal)  ││
│  └──────────────┘  └──────────────┘│
└──────────────┬──────────────────────┘
               │ uses
               ▼
┌──────────────────────────────────────┐
│     TasmotaLEDPusher (Abstract)      │
└──────────────┬───────────────────────┘
               │
       ┌───────┼───────┐
       ▼       ▼       ▼
    ┌─────┐ ┌─────┐ ┌─────┐
    │ RMT │ │ SPI │ │ I2S │
    └─────┘ └─────┘ └─────┘
```

### Hardware Support

| Hardware | ESP32 | ESP32-S2 | ESP32-S3 | ESP32-C3 | ESP32-C2 | ESP32-C6 |
|----------|-------|----------|----------|----------|----------|----------|
| **RMT**  | ✅    | ✅       | ✅       | ✅       | ❌       | ✅       |
| **SPI**  | ✅    | ✅       | ✅       | ✅       | ✅       | ✅       |
| **I2S**  | ✅    | ✅       | ✅       | ❌       | ❌       | ❌       |

**Selection Priority:** RMT → I2S → SPI (auto-selected based on SOC capabilities)

## Configuration

### LED Type Encoding

LED types are encoded in a 16-bit value:

```
Bits 15-8: Timing (WS2812=0, SK6812=1)
Bit  7:    W Position (0=after RGB, 1=before RGB)
Bits 6-4:  Pixel Order (GRB, RGB, BGR, etc.)
Bits 3-0:  Bytes/Pixel (3=RGB, 4=RGBW)
```

**Predefined Types:**
```cpp
ws2812_grb   // WS2812 with GRB ordering (most common)
sk6812_grbw  // SK6812 with GRBW ordering
sk6812_grb   // SK6812 with GRB ordering (no white)
```

**Custom Types:**
```cpp
uint16_t custom = TasmotaLed_3_RGB |      // 3 bytes per pixel
                  TasmotaLed_RGB |        // RGB ordering
                  TasmotaLed_WS2812;      // WS2812 timing
```

### Pixel Ordering Options

| Enum | Order | Description |
|------|-------|-------------|
| `TasmotaLed_GRB` | G, R, B | Green-Red-Blue (default) |
| `TasmotaLed_RGB` | R, G, B | Red-Green-Blue |
| `TasmotaLed_BGR` | B, G, R | Blue-Green-Red |
| `TasmotaLed_RBG` | R, B, G | Red-Blue-Green |
| `TasmotaLed_BRG` | B, R, G | Blue-Red-Green |
| `TasmotaLed_GBR` | G, B, R | Green-Blue-Red |

## Timing Specifications

### WS2812 Timing
```
T0H:  400ns  (bit 0 high time)
T0L:  850ns  (bit 0 low time)
T1H:  800ns  (bit 1 high time)
T1L:  450ns  (bit 1 low time)
Reset: 80µs  (reset pulse)
```

### SK6812 Timing
```
T0H:  300ns  (bit 0 high time)
T0L:  900ns  (bit 0 low time)
T1H:  600ns  (bit 1 high time)
T1L:  600ns  (bit 1 low time)
Reset: 80µs  (reset pulse)
```

### RMT Implementation
- Clock: 40 MHz (25ns resolution)
- Precision: ±25ns per timing parameter
- Memory: 192 symbols per channel
- Non-blocking with DMA

### SPI Implementation
- Clock: 2.5 MHz
- Encoding: 3 SPI bits per LED bit
- Bit 0: `100` pattern
- Bit 1: `110` pattern
- Memory: 3× pixel buffer size

## Memory Usage

| Configuration | Memory Required |
|---------------|-----------------|
| 60 RGB pixels | ~460 bytes |
| 144 RGB pixels | ~964 bytes |
| 300 RGB pixels | ~1,900 bytes |
| 512 RGB pixels | ~3,172 bytes |
| 60 RGBW pixels | ~580 bytes |

**Formula:** `~100 bytes + (2 × pixels × bytes_per_pixel)`

**SPI Additional:** Add `3 × pixels × bytes_per_pixel` for encoding buffer

## Performance

### Transmission Times (WS2812/SK6812)
- 60 pixels: ~1.8 ms (max 555 Hz)
- 144 pixels: ~4.3 ms (max 232 Hz)
- 300 pixels: ~9.0 ms (max 111 Hz)
- 512 pixels: ~15.4 ms (max 65 Hz)

### CPU Overhead
- **RMT:** <1% during transmission (DMA-based)
- **SPI:** <2% during transmission (DMA-based)
- **Format conversion:** ~124 µs for 512 RGB pixels

## API Highlights

### Core Methods

```cpp
// Initialization
bool Begin()
void SetPusher(TasmotaLEDPusher *pusher)

// Pixel manipulation
void SetPixelColor(int32_t index, uint32_t wrgb)
uint32_t GetPixelColor(int32_t index)
void ClearTo(uint32_t rgbw, int32_t first = 0, int32_t last = -1)

// Display
void Show()
bool CanShow()

// Configuration
void SetPixelCount(uint16_t num_leds)
void SetPixelSubType(uint8_t type)

// Query
uint16_t PixelCount() const
uint8_t PixelSize() const
uint8_t * Pixels() const
```

### Color Format

**RGB Strips (3 bytes):**
```cpp
0xRRGGBB
// Example: 0xFF0000 = Red
```

**RGBW Strips (4 bytes):**
```cpp
0xWWRRGGBB
// Example: 0xFF000000 = Pure white (W channel)
// Example: 0x00FF0000 = Red (RGB channels)
```

## Compile-Time Configuration

Enable/disable hardware support:

```cpp
#define TASMOTALED_HARDWARE_RMT 1  // Enable RMT (default: 1)
#define TASMOTALED_HARDWARE_SPI 0  // Enable SPI (default: 0)
#define TASMOTALED_HARDWARE_I2S 0  // Enable I2S (default: 0)
```

**Note:** If no hardware is enabled, SPI is automatically enabled as fallback.

## Documentation

For complete documentation including detailed API reference, integration guide, troubleshooting, and advanced usage, see:

**[TASMOTALED_DOCUMENTATION.md](TASMOTALED_DOCUMENTATION.md)**

## License

GNU General Public License v3.0

Copyright (C) 2024 Stephan Hadinger

## Credits

- **Author:** Stephan Hadinger
- **Project:** Tasmota Firmware
- **Inspired by:** NeoPixelBus library
- **RMT Encoder:** Based on ESP-IDF examples
