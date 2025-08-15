# Session Recreation Settings
# Version: 1.1.0 | Updated: 2025-01-17

## Style Configuration
**Style**: Concise Mode
- Reduces output tokens while maintaining quality
- Avoids unnecessary preamble/postamble
- Maintains full quality for code/artifacts
- Can be disabled via Claude UI if needed

## Behavioral Settings
**Git Commit Format**: Always show git commands using (AI) prefix format
**Session State Updates**: Update SESSION-STATE.md when settings change (with version bump)
**Versioning**: SESSION-STATE.md follows semantic versioning (major.minor.patch)
**Language Policy**: User can write in any language (English/Italian), Claude MUST respond only in English

## Context Setup
**Project**: LoRaWAN AI Framework Enhancement
**Location**: `C:\Project\tasmota\Tasmota\tasmota\berry\lorawan\decoders.AI\`

## Current State
- **Framework Version**: v1.8.0 (enhanced error handling)
- **Template Version**: v2.1.8
- **Recent Work**: Updated README.md to reflect v1.8.0 capabilities
- **Git Status**: README.md updated, ready for commit

## Key Files Modified
1. `LwDecode.be` - Added safe_load_decoder, log_error, stack traces
2. `DEVELOPER-PROMPT.md` - Updated error handling patterns
3. `FRAMEWORK.md` - Added error handling documentation
4. Multiple driver files - Enhanced error handling integration

## Session Goals
1. ✅ Framework Enhancement
2. ✅ Always up-to-date documentation
3. ✅ Always up-to-date references
4. ✅ Create new drivers
5. ✅ Patched existing drivers
6. 🔄 Create single commit

## Key Commands Used
- File system operations via tools
- Git workflow management
- Berry/Tasmota LoRaWAN development

## Files Loaded
- BERRY_LANGUAGE_REFERENCE.md
- BERRY_TASMOTA.md
- Framework and driver files

This setup provides context for continuing LoRaWAN framework development with enhanced error handling and debugging capabilities.
