# AI Assistant for LoRaWAN IoT Sensor Driver Development

## Objective
Create a complete driver implementation for the specified IoT sensor using the LwDecode framework architecture.

## Context
- **Tasmota Home Path**: `C:\Project\tasmota\Tasmota`
- **Berry Language Reference**: Use `<TASMOTA_HOME_PATH>/.doc_for_ai/BERRY_LANGUAGE_REFERENCE.md` and `<TASMOTA_HOME_PATH>/.doc_for_ai/BERRY_TASMOTA.md` if not already provided as attachment
- **LwDecode Home**: `<TASMOTA_HOME_PATH>/tasmota/berry/lorawan/decoders`
- **Framework**: LwDecode framework, find it in `<LWDECODE_HOME>` if not already provided as attachment
- **Output Home Path**: Use `<LWDECODE_HOME>/AI-Generated-Driver`
- **Emoji Reference file**: Use `<OUTPUT_HOME>/emoji-reference.md`
- **Reference Templates**: Can be provided as attachment; if not provided use `<LWDECODE_HOME>/DEFAULT_AI_TEMPLATE.md`; if file not present, draft the default template using framework patterns
- **Target Device**: IoT sensor specified in PDF documentation
- **Documentation**: Device specification PDF (attached)

## Requirements

### Driver Implementation
1. **Core Functionality**
   - ESP32 Limits considerations
   - Parse sensor payload data according to PDF specifications
   - Implement all documented measurement types and data formats
   - Handle device configuration and status messages
   - Implement error handling and data validation
   - Keep the code minimal but readable
   - Keep updated the revision history inside the code, create it if not present

2. **Framework Compliance**
   - Follow Berry for tasmota implementation
   - Follow LwDecode architectural patterns
   - Use provided driver templates as reference
   - Implement required interface methods and data structures
   - Maintain compatibility with framework components

3. **Data Processing**
   - Decode all sensor measurement values per specification
   - Apply unit conversions as documented
   - Handle calibration parameters if present
   - Support all documented transmission modes

### UI/Display Guidelines
- **Emoji Usage**: Use emojis instead of text labels whenever possible
- **Emoji Consistency**: Always use same emoji for same unit (reference emoji-reference.md)
- **Measurement Display**: For numeric measurements (temperature, watts, etc.), show value next to emoji
- **Status Display**: For non-measurement data, show device "last seen" timestamp instead of value
- **Implementation Coverage**: Implement ALL uplinks documented in specification
- **Mandatory Check**: Verify emoji usage against existing reference before implementation
- **Text Label Enforcement**: Strictly avoid text labels unless absolutely impossible to represent with emoji
- **Tooltip Management**: Use descriptive tooltips only when emoji meaning requires clarification
- **UI Compliance**: Ensure all display elements follow emoji-first approach with minimal text overlay
- **Single Line Display**: Driver output should generally occupy 1 line; if >2 lines, rationalize displayed information
- **Logical Sequence**: Organize sensor sequence with logical ordering (priority, measurement type, etc.)
- **Minimal & Efficient**: Keep UI minimal and efficient for intended scope

### Automated Documentation
- **Keep the Emoji Tool**: Automatically generate and maintain `<OUTPUT_HOME>/emoji-reference.md`
- **Format**: Table with emoji symbol, unicode code, name, sensor usage list from legacy drivers, sensor usage list from AI Generated drivers
- **Update**: Add new emojis during driver development, maintain existing entries
- **Consistency Enforcement**: Use existing emoji-reference.md to ensure same emoji for same units
- **Version History**: Maintain version history section in emoji-reference.md with changes and dates

### Code Quality
- Follow Berry coding standards and Tasmota conventions from template drivers
- Add comprehensive code comments in English
- Include parameter descriptions, valid ranges, and units
- Document error codes and handling logic
- Ensure cross-platform compatibility (Windows/Linux/macOS paths)

## Process Guidelines
- **Clarification Required**: Ask for clarification on any ambiguous measurements/sensors/uplinks before implementation
- **File Management**: Always check if files exist before writing, request permission for overwrites
- **No Step-by-Step Display**: Do not show processing steps during development
- **Super Minimal Output**: Provide only ESSENTIAL information after work
- **No Code Generation Display**: Do not show code generation of the artifacts
- **Error Handling**: Report any file access issues or missing dependencies

## Deliverables
1. **Driver Source Code**
   - Complete implementation following framework patterns
   - Comprehensive inline documentation
   - Error handling and validation logic
   - If decoder already exists, backup the current version, and replace it
   - Naming convention and output file: `<OUTPUT_HOME>/vendor/[vendor_name_lowercase]/[DRIVER_NAME_uppercase].be`
     (example for "Milesight WS522" -> `<OUTPUT_HOME>/vendor/milesight/WS522.be`)

2. **Pull Request Description** (Markdown format)
   - Feature overview and implementation approach
   - Technical details and architectural decisions
   - Testing methodology and validation results
   - Breaking changes or compatibility notes
   - Usage examples and integration instructions
   - References to PDF specification sections
   - Naming convention & output file: same of the source code, but with `.md` extension
   - Add usage and statistics metric about the execution of the workloads
   - Include at the end a reusable prompt that describe how this driver was generated

3. **Emoji Reference Tool** (Markdown format)
   - Create and maintain comprehensive emoji usage list
   - Save as markdown file in the output directory
   - Include: emoji symbol, unicode code, name, sensors using it, and so on

4. **Final Recap** (Minimal summary of generated components)

## Validation Criteria
- Successfully parse all documented payload formats
- Pass framework compliance review
- Handle edge cases and error conditions
- Maintain consistent code style with existing drivers
- Complete coverage of all documented uplinks