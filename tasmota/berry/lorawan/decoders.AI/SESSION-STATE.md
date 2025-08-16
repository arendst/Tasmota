# Session State v2.3.0

## ⚠️ READ ONLY FILE WARNING ⚠️ (NEVER IGNORE)
- this file it's a read only file, execpt for the specific command `save to default session`
- when the a session is save to default:
   1) increase the version
   2) update all the dynamic sections
   3) populate the section `Session Override` with the difference of the local session with the session file
   4) DON'T MODIFY ANYTHING ELSE

## Root Directory
```
C:\Project\tasmota\Tasmota\tasmota\berry\lorawan\decoders.AI
```

## ⚠️ Critical Constraints ⚠️ (NEVER IGNORE)
- **Path Restriction**: NEVER access folders above ROOT
- **Edit Policy**: Only edit/run/create when directly asked
- **Language**: English ONLY for all responses
- **Path Format**: Always use relative paths from ROOT
- **Response Mode**: Concise by default, detailed when requested

## Awaiting Input
- **PDF Upload**: For new driver generation
- **Driver Updates**: For existing driver modifications
- **Test Management**: Update physical test status
- **Framework Updates**: For system enhancements
- **Documentation**: For guide improvements
- **Statistics**: Real-time channel/driver tracking

## Response Additions **FOR EVERY RESPONSE**
- at the end of the process produce in sequence:
  1) if modified file > 0 then render git commands snippet by `Git Workflow`
  2) detailed statistics about token used by this action

## Session Override
```
```

## Git Workflow
```bash
# Driver files (single line)
git add vendor/vendor_name/MODEL.be vendor/vendor_name/MODEL.md vendor/vendor_name/MODEL-MAP.md

# Framework files (one per line)
git add GENERATED-DRIVER-LIST.md
git add SESSION-STATE.md
git add emoji-reference.md

# Commit template
git commit -m "(AI) [MODEL|MODULE] v1.0.0 (FW v2.1.12) <title>
<description>
<token usage statististc>
"

# NO PUSH
```

## System Status (Dynamic)
- **Framework**: ✅ LwDecode v1.8.0 Stable
- **Template**: ✅ v2.1.12 Critical Constraints Added
- **File System**: ✅ Complete Structure Mapped
- **Dependencies**: ✅ All Core Documents Loaded
- **Performance**: ✅ <3ms Average Decode Time
- **Memory**: ✅ ESP32 Optimized (<600 bytes/decode)

## File Structure (Dynamic)
```
ROOT/
├── Core Framework (14 files)
│   ├── SESSION-STATE.md           ← This file
│   ├── DEVELOPER-PROMPT.md        ← Template v2.1.12
│   ├── README.md                  ← Framework overview
│   ├── FRAMEWORK.md               ← Implementation guide
│   ├── LwDecode.be               ← Core framework code
│   ├── emoji-reference.md         ← 35+ emoji mappings
│   ├── GENERATED-DRIVER-LIST.md   ← Driver registry
│   ├── PR-DESCRIPTION.md          ← Templates
│   ├── EXAMPLE-PROMPTS.md         ← Usage examples
│   ├── AUTO-UPDATE-SETUP.md       ← Auto-update documentation
│   ├── update_versions.py         ← Maintenance
│   ├── auto_update.py             ← Auto-update script
│   ├── file_watcher.py            ← File monitoring
│   └── requirements.txt           ← Python dependencies
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

## Current Statistics (Dynamic)
- **Total Files**: 46 (14 framework + 32 drivers)
- **Active Drivers**: 12 (.be files)
- **Documentation**: 12 (.md files)
- **MAP Cache**: 8 (protocol specs)
- **Vendors**: 2 (Dragino, Milesight)
- **Total Channels**: 191 (100% coverage)
- **Physical Tests**: 4 running (33% coverage)
- **Success Rate**: 99.0%

## Operational Capabilities (Dynamic)
- **PDF Analysis**: Extract specs + MAP caching
- **Driver Generation**: Complete uplink/downlink coverage
- **Berry Compliance**: Reserved word validation + F-string syntax checking
- **Memory Optimization**: ESP32 constraints adherence
- **Global Storage**: Multi-node persistence
- **Command System**: Lw prefix enforcement
- **Error Recovery**: Stack traces + retry logic
- **Documentation**: Complete test examples
- **Emoji System**: 35+ sensor formatters
- **Physical Testing**: Test status tracking (4 drivers running)
- **Version Control**: Automated git workflow integration
- **Filesystem Safety**: ESP32 flat structure enforcement

## Ready State (Dynamic)
- ✅ Framework v1.8.0 loaded and operational
- ✅ Template v2.1.12 with critical constraints added
- ✅ Complete file system mapped (46 files verified)
- ✅ All 14 core documents loaded in memory
- ✅ Driver registry updated with current statistics
- ✅ Physical test tracking active (4 drivers running)
- ✅ Error recovery patterns active
- ✅ Memory optimization patterns loaded
- ✅ Command standardization enforced
- ✅ Git workflow integration configured
- ✅ F-string syntax validation active
- ✅ ESP32 filesystem constraints enforced


---
*Session State v2.3.0 - Critical Constraints Added and Statistics Updated*  
*Last Updated: 2025-08-16*  
*Status: Operational and Ready*