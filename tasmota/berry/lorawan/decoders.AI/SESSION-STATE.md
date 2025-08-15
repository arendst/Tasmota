# Session State

## Current Configuration
- **Mode**: Concise (active)
- **Template Version**: DEVELOPER-PROMPT.md v2.1.8
- **Framework**: LwDecode for Tasmota Berry v1.8.0
- **Working Directory**: `C:\Project\tasmota\Tasmota\tasmota\berry\lorawan\decoders.AI`

## Path Context
- **Base**: C:\Project\tasmota\Tasmota
- **Decoders**: ${base}/tasmota/berry/lorawan/decoders.AI/
- **Output**: ${decoders}
- **Vendor**: ${output}/vendor/${vendor_lowercase}/

## Response Behavior
- Concise responses by default
- Use relative paths from decoders.AI directory
- Detailed responses only when explicitly requested
- Maintain full quality for code/artifacts
- Auto-reload file changes when detected

## Loaded Documents
- BERRY_LANGUAGE_REFERENCE.md (Berry syntax & stdlib)
- BERRY_TASMOTA.md (Tasmota-specific features)
- DEVELOPER-PROMPT.md v2.1.8 (AI generation template)
- GENERATED-DRIVER-LIST.md (driver registry)
- README.md (Framework overview v1.8.0)
- FRAMEWORK.md (Implementation details)
- LwDecode.be (Core framework v1.8.0)

## Auto-Reload Active
- DEVELOPER-PROMPT.md
- SESSION-STATE.md
- README.md
- FRAMEWORK.md
- LwDecode.be
- Any files in decoders.AI directory

## Capabilities Ready
- PDF analysis and MAP caching
- 100% uplink/downlink coverage
- Berry compliance (no reserved words)
- Global node storage patterns
- Test command generation
- Emoji formatting system
- Documentation generation
- Enhanced error handling with stack traces

## Framework Features Loaded
- LwSensorFormatter_cls with emoji system
- lwdecode_cls with error recovery
- Integer/crypto helpers (uint16le/be, CRC, BCD)
- Downlink commands (SendDownlink/SendDownlinkMap)
- Web UI caching and battery/RSSI indicators
- Multi-node support up to 128 devices

---
*Last Updated: Session with auto-reload activated*
