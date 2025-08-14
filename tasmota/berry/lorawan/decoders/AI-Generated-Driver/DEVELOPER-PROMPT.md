# LoRaWAN IoT Sensor Driver Development Specification

## Task Definition
Generate complete Berry driver implementation for LoRaWAN sensor from PDF specification using LwDecode framework.

## Environment Configuration
```yaml
base_path: C:\Project\tasmota\Tasmota
paths:
  berry_docs: ${base_path}/.doc_for_ai/
  lwdecode: ${base_path}/tasmota/berry/lorawan/decoders/
  output: ${lwdecode}/AI-Generated-Driver/
  vendor: ${output}/vendor/${vendor_lowercase}/

required_inputs:
  - PDF specification (attached)
  - Vendor name (extract from PDF)
  - Model name (extract from PDF)

auto_load:
  - ${berry_docs}/BERRY_LANGUAGE_REFERENCE.md
  - ${berry_docs}/BERRY_TASMOTA.md
  - ${lwdecode}/DEFAULT_AI_TEMPLATE.md
  - ${output}/emoji-reference.md
```

## Implementation Rules

### Priority 1: Core Driver
```
constraints:
  - ESP32 memory limit: 4MB flash, 320KB RAM
  - Berry stack limit: 256 levels
  - String concat limit: minimize operations
  - Object creation: pool where possible
  
requirements:
  - Decode ALL uplinks from PDF
  - Binary payload parsing per specification
  - Little/Big endian handling per docs
  - CRC/checksum validation if specified
  - Error codes: return nil on failure

critical_requirements:
  decode_completeness:
    - "MUST decode EVERY channel type mentioned in the PDF"
    - "MUST NOT skip ANY uplink data - all fields are important"
    - "Include ALL metadata fields: versions, SN, device type, reset events"
    - "Include ALL configuration states reported in uplinks"
    - "If a channel appears in the PDF, it MUST be decoded to data{}"
    - "Never use 'elif channel_id == X && channel_type == Y i += Z' without decoding"
    
  validation_checklist:
    before_completion:
      - "Count all unique channel types in PDF"
      - "Count all decoded channel types in code"
      - "Verify 100% match between PDF and implementation"
      - "Check no 'skip' comments without actual data extraction"
      
  code_patterns:
    forbidden:
      - "Skipping known channel types without decoding"
      - "Comments like '# Skip' or '# Unused' for documented channels"
    required:
      - "Every channel must either decode data OR log as unknown"
      - "All configuration states must be stored and reported"
```

### Priority 2: Display Format
```
ui_rules:
  emoji_only: true
  text_labels: forbidden
  line_count: 1 (max 2 if justified)
  value_format: "emoji value unit"
  status_format: "emoji last_seen"
  
emoji_selection:
  1. Check emoji-reference.md
  2. Use existing mapping if found
  3. Add new if needed with justification
  4. Update reference file
```

### Priority 3: Code Standards
```
structure:
  - Class: LwDecode_${MODEL}
  - Methods: decodeUplink(), add_web_sensor()
  - Error handling: try/except blocks
  - Comments: English, inline, concise
  
patterns:
  - Use bitwise operations for flags
  - Pre-calculate constants
  - Cache repeated calculations
  - Avoid string concatenation loops
```

## Execution Workflow

### Phase 1: Analysis
```
1. Parse PDF → Extract:
   - Vendor/Model identifiers
   - Uplink types and IDs
   - Payload structures
   - Unit conversions
   - Bit field definitions

2. Verify existing implementation:
   - Check ${vendor}/${MODEL}.be
   - Backup if exists → ${MODEL}.${VERSION}.be
```

### Phase 2: Generation
```
outputs:
  driver:
    path: ${vendor}/${MODEL}.be
    content:
      - Header with revision history
      - Class definition
      - Decode methods for ALL uplinks
      - Display formatter
      - Helper functions, if the helper function can be useful in general propose the implementations directly at the framework level
      
  documentation:
    path: ${vendor}/${MODEL}.md
    sections:
      - Implementation summary
      - Uplink coverage matrix
      - Performance metrics
      - Integration examples
      - Generation prompt
      
  emoji_reference:
    path: ${output}/emoji-reference.md
    update:
      - Add new mappings
      - Update usage counts
      - Log timestamp
```

### Phase 3: Validation
```
checklist:
  ✓ All uplinks implemented
  ✓ Payload sizes match spec
  ✓ Units correctly converted
  ✓ Emoji mappings consistent
  ✓ Memory usage optimized
  ✓ Error paths tested
```

## Output Requirements

### File Operations
- Check existence before write
- Request permission for overwrites
- Report access failures immediately
- Use atomic writes when possible

### Response Format
```markdown
## Completed
Driver: ${path}
Documentation: ${path}
Emoji Reference: [updated|created]
Coverage: X/Y uplinks
Performance: XXms decode time
```

### Prohibited Actions
- No step-by-step narration
- No code preview in response
- No implementation details shown
- No verbose explanations
- No artifact generation display

## Critical Constraints

### Memory Optimization
- Reuse objects where possible
- Minimize string operations
- Use integer math over float
- Preallocate buffers
- Clear unused references

### Error Handling
- Silent failure with nil return
- Log only critical errors
- No exception propagation
- Graceful degradation
- Timeout protection

### Compatibility
- Berry 0.1.10+ syntax only
- Tasmota 13.0+ APIs
- UTF-8 encoding throughout
- CRLF line endings (Windows)
- No external dependencies

## Quality Metrics
```yaml
success_criteria:
  functional:
    - 100% uplink coverage
    - Zero runtime errors
    - <50ms decode time
    
  code:
    - <500 lines total
    - <80 chars per line
    - >30% comment ratio
    
  ui:
    - 100% emoji usage
    - Single line display
    - Clear value presentation
```

## Example Execution
```
Input: "Generate driver for attached PDF"
Process: [Silent analysis and generation]
Output: "## Completed
Driver: vendor/milesight/WS301.be
Documentation: vendor/milesight/WS301.md
Emoji Reference: updated
Coverage: 12/12 uplinks
Performance: 23ms decode time"
```