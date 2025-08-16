# Session State v2.1.1

## System Status
- **Framework**: ✅ LwDecode v1.8.0 Stable
- **Template**: ✅ v2.1.10 Enhanced TestPayload
- **File System**: ✅ Complete Structure Mapped
- **Dependencies**: ✅ All Core Documents Loaded
- **Performance**: ✅ <3ms Average Decode Time
- **Memory**: ✅ ESP32 Optimized (<600 bytes/decode)

## Root Directory
```
C:\Project\tasmota\Tasmota\tasmota\berry\lorawan\decoders.AI/
```

## Critical Constraints
- **Path Restriction**: NEVER access folders above ROOT
- **Edit Policy**: Only edit/run/create when directly asked
- **Language**: English ONLY for all responses
- **Path Format**: Always use relative paths from ROOT
- **Response Mode**: Concise by default, detailed when requested

## Response Additions **FOR EVERY RESPONSE**
- at the end of the process produce in sequence:
  1) if modified file > 0 then render git commands snippet by `Git Workflow`
  2) detailed statistics about token used by this action

## File Structure (Verified)
```
ROOT/
├── Core Framework (10 files)
│   ├── SESSION-STATE.md           ← This file
│   ├── DEVELOPER-PROMPT.md        ← Template v2.1.10
│   ├── README.md                  ← Framework overview
│   ├── FRAMEWORK.md               ← Implementation guide
│   ├── LwDecode.be               ← Core framework code
│   ├── emoji-reference.md         ← 35+ emoji mappings
│   ├── GENERATED-DRIVER-LIST.md   ← Driver registry
│   ├── PR-DESCRIPTION.md          ← Templates
│   ├── EXAMPLE-PROMPTS.md         ← Usage examples
│   └── update_versions.py         ← Maintenance
└── Driver Storage (32 files)
    ├── vendor/dragino/            ← 6 drivers + 6 docs + 6 MAP
    │   ├── D2x.be/.md/-MAP.md     ← Multi-probe temp
    │   ├── DDS75-LB.be/.md/-MAP.md ← Distance sensor
    │   ├── LDS02.be/.md/-MAP.md   ← Door sensor
    │   ├── LHT52.be/.md/-MAP.md   ← Temp/humidity
    │   ├── LHT65.be/.md/-MAP.md   ← Multi-sensor
    │   └── PS-LB.be/.md/-MAP.md   ← Pressure sensor
    └── vendor/milesight/          ← 6 drivers + 6 docs + 2 MAP
        ├── AM300.be/.md           ← Air quality
        ├── WS101.be/.md/-MAP.md   ← Smart button
        ├── WS202.be/.md           ← PIR/light
        ├── WS301.be/.md           ← Door/window
        ├── WS523.be/.md           ← Smart socket
        └── WS52x.be/.md/-MAP.md   ← Socket series
```

## Current Statistics
- **Total Files**: 42 (10 framework + 32 drivers)
- **Active Drivers**: 12 (.be files)
- **Documentation**: 12 (.md files)
- **MAP Cache**: 8 (protocol specs)
- **Vendors**: 2 (Dragino, Milesight)
- **Total Channels**: 191 (100% coverage)
- **Physical Tests**: 4 running (33% coverage)
- **Success Rate**: 99.0%

## Operational Capabilities
- **PDF Analysis**: Extract specs + MAP caching
- **Driver Generation**: Complete uplink/downlink coverage
- **Berry Compliance**: Reserved word validation
- **Memory Optimization**: ESP32 constraints adherence
- **Global Storage**: Multi-node persistence
- **Command System**: Lw prefix enforcement
- **Error Recovery**: Stack traces + retry logic
- **Documentation**: Complete test examples
- **Emoji System**: 35+ sensor formatters
- **Physical Testing**: Test status tracking (4 drivers running)
- **Version Control**: Automated git workflow integration

## Template v2.1.10 Features
- **Enhanced TestPayload**: Multi-parameter support
  - Format: `Lw[MODEL]TestPayload<node> <hex>`
  - Format: `Lw[MODEL]TestPayload<node> <fport>,<hex>`
  - Format: `Lw[MODEL]TestPayload<node> <rssi>,<fport>,<hex>`
- **Command Prefix**: "Lw" enforced for all commands
- **Parameter Validation**: Enhanced range/enum checking
- **Error Handling**: Memory/stack overflow recovery
- **Troubleshooting**: Complete issue/solution matrix

## Git Workflow
```bash
cd <C:\Project\tasmota\Tasmota\tasmota\berry\lorawan\decoders.AI>

# Driver files (single line)
git add vendor/vendor_name/MODEL.be vendor/vendor_name/MODEL.md vendor/vendor_name/MODEL-MAP.md

# Framework files (one per line)
git add GENERATED-DRIVER-LIST.md
git add SESSION-STATE.md
git add emoji-reference.md

# Commit template
git commit -m "(AI) [MODEL|MODULE] v1.0.0 (FW v2.1.10) <description>"

# NO PUSH
```

## Ready State
- ✅ Framework v1.8.0 loaded and operational
- ✅ Template v2.1.10 enhanced with TestPayload improvements
- ✅ Complete file system mapped (42 files verified)
- ✅ All 10 core documents loaded in memory
- ✅ Driver registry updated with current statistics
- ✅ Physical test tracking active (4 drivers running)
- ✅ Error recovery patterns active
- ✅ Memory optimization patterns loaded
- ✅ Command standardization enforced
- ✅ Git workflow integration configured

## Awaiting Input
- **PDF Upload**: For new driver generation
- **Driver Updates**: For existing driver modifications
- **Test Management**: Update physical test status
- **Framework Updates**: For system enhancements
- **Documentation**: For guide improvements
- **Statistics**: Real-time channel/driver tracking

---
*Session State v2.1.1 - Statistics and Test Tracking Synchronized*  
*Last Updated: 2025-08-16*  
*Status: Operational and Ready*