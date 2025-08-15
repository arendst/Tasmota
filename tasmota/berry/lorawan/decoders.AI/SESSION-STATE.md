# Session Recreation Settings
# Version: 1.3.0 | Updated: 2025-01-17

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
**Read-Only Commands**: "verify", "check", "show", "list" = NO file changes allowed, read-only operations only. In pipeline operations, this restriction applies only to the specific step, not the entire sequence.

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

## Prompt Library & Usage Patterns

### Framework Operations
- `load the new session state` - Reload settings after changes
- `verify [file/structure]` - Read-only inspection (no changes)
- `check if [component] is up to date` - Status verification
- `show git commands for [action]` - Display commit format
- `list all available drivers` - Inventory check
- `show framework status` - Health check

### Driver Generation
- `Generate driver for [VENDOR] [MODEL]` - Full driver creation
- `Load PDF and create [MODEL] driver` - With document upload
- `Update existing [MODEL] driver with [changes]` - Modification
- `Test driver with payload [HEX]` - Validation
- `Create minimal driver for [TYPE] sensor` - Basic template
- `Generate batch drivers from [folder] PDFs` - Multiple sensors
- `Convert legacy driver to v1.8.0 format` - Migration
- `Add downlink commands to [MODEL]` - Enhancement
- `Optimize [MODEL] for memory usage` - Performance tuning

### Framework Development
- `Update framework to v[X.Y.Z]` - Version upgrade
- `Patch all drivers for [feature]` - Bulk modifications
- `Add [functionality] to framework core` - Enhancement
- `Migrate drivers from [old] to [new] structure` - Refactoring
- `Implement new formatter for [unit]` - Display extension
- `Add error handling pattern for [scenario]` - Robustness
- `Create helper function for [operation]` - Utility
- `Benchmark framework performance` - Analysis

### Documentation Maintenance
- `Update README.md to reflect v[X.Y.Z]` - Version sync
- `Generate changelog for v[X.Y.Z]` - Release notes
- `Verify documentation consistency` - Cross-reference check
- `Update emoji-reference.md with [new mappings]` - Standardization
- `Create API documentation for [module]` - Reference
- `Generate driver compatibility matrix` - Coverage report
- `Update DEVELOPER-PROMPT.md template` - Instruction refresh

### Testing & Validation
- `Test all drivers with sample payloads` - Comprehensive check
- `Validate [MODEL] against PDF specs` - Compliance verify
- `Check memory footprint of drivers` - Resource analysis
- `Benchmark decode performance` - Speed testing
- `Test error scenarios for [component]` - Edge cases
- `Validate emoji display on different devices` - UI testing
- `Check Berry syntax compliance` - Language validation

### Pipeline Operations
```
verify current state → update component → test changes → commit with (AI) format
check dependencies → generate driver → validate output → update references
list outdated drivers → batch update → test collection → commit changes
analyze PDF → extract specs → generate MAP → create driver → test → document
```

### Maintenance & Cleanup
- `Clean up temporary files` - Housekeeping
- `Remove deprecated drivers` - Cleanup
- `Archive old versions` - Organization
- `Update driver dependencies` - Maintenance
- `Consolidate duplicate functions` - Refactoring
- `Check for unused emojis` - Reference cleanup
- `Validate git history integrity` - Quality assurance

### Emergency & Recovery
- `Rollback framework to v[X.Y.Z]` - Emergency revert
- `Restore driver from backup` - Recovery
- `Fix broken driver [MODEL]` - Emergency repair
- `Debug framework crash` - Critical analysis
- `Recover from corrupted session` - State restoration
- `Emergency disable problematic driver` - Safety measure

### Development Workflow
- `Start new feature branch` - Version control
- `Prepare release candidate` - Release management
- `Review code quality standards` - QA check
- `Update coding guidelines` - Standards maintenance
- `Create development roadmap` - Planning
- `Schedule maintenance window` - Operations
