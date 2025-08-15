# AI-Powered LoRaWAN Driver Development Framework

## Overview

This framework automates LoRaWAN sensor driver development for Tasmota using Claude AI. It transforms manufacturer PDFs into production-ready Berry code with complete uplink/downlink coverage, emoji-based UI, and comprehensive documentation.

## Core Components

- **[DEVELOPER-PROMPT.md](DEVELOPER-PROMPT.md)** - Single AI prompt (v2.1.8) for complete driver generation
- **[LwDecode.be](LwDecode.be)** - Framework core (v1.8.0) with error handling and multi-node support
- **[FRAMEWORK.md](FRAMEWORK.md)** - Technical implementation details and API reference
- **[SESSION-STATE.md](SESSION-STATE.md)** - AI session configuration and loaded components

## Key Features

**Complete Protocol Coverage**: 100% uplink decoding and downlink command implementation per manufacturer specifications

**Framework Integration**: Enhanced error handling with stack traces, global node storage, and automatic recovery

**Developer Experience**: 
- MAP file caching for fast regeneration
- Test command generation (`ModelTestPayload<port> <hex>`)
- Node management (`ModelNodeStats`, `ModelClearNode`)
- Comprehensive documentation with usage examples

**Production Ready**: Memory optimized for ESP32, Berry syntax compliance, emoji-based UI formatting

## Usage

### Session Management
The [SESSION-STATE.md](SESSION-STATE.md) maintains AI session configuration:
- Loaded framework components and versions
- Working directory context (`decoders.AI/`)
- Auto-reload file monitoring
- Response behavior settings

### Driver Generation
1. Load framework: Reference `DEVELOPER-PROMPT.md` in Claude
2. Provide PDF specification or cached MAP file
3. AI generates complete driver with documentation
4. Output: `vendor/manufacturer/MODEL.be`, `MODEL.md`, `MODEL-MAP.md`

### Testing
```bash
# Load driver
load("vendor/milesight/WS52x.be")

# Test uplink decoding
WS52xTestPayload85 037464000480ABCD12340550

# Test downlink commands  
WS52xControl1 on
WS52xInterval2 30
```

## Current Status

**Tested Drivers**: 4 Milesight sensors successfully migrated and operational
**Untested**: All other generated drivers require hardware validation
**Framework**: Stable v1.8.0 with enhanced error handling

## Generated Drivers
See [GENERATED-DRIVER-LIST.md](GENERATED-DRIVER-LIST.md) for complete registry:
- Browan: 1 driver (5 channels)
- Dragino: 2 drivers (20 channels) 
- Milesight: 5 drivers (105 channels)

## Contributing

**AI-Only Zone**: All commits under `decoders.AI/` should be made exclusively by the AI agent to maintain consistency and framework integrity.

**Hardware Testing**: We need developers with actual LoRaWAN hardware to validate generated drivers beyond the tested Milesight devices.

**Framework Enhancement**: Contributions to core framework, new formatters, or additional vendor support welcome via standard PR process.

## Links

- [Framework Documentation](FRAMEWORK.md)
- [Driver Registry](GENERATED-DRIVER-LIST.md) 
- [Emoji Reference](emoji-reference.md)
- [Core Framework](LwDecode.be)

The framework aims to democratize LoRaWAN driver development while maintaining code quality and consistency. Testing and feedback from the community will help validate the AI-generated approach across diverse hardware.