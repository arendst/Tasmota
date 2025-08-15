#!/usr/bin/env python3
"""Update all AI-generated drivers to v1.0.0 with date 2025-08-14"""

import os
import re

def update_be_file(filepath):
    """Update .be driver file header"""
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # Pattern variations to match different header formats
    patterns = [
        # Pattern 1: AI-Generated Driver v1.0
        (r'# AI-Generated Driver v\d+\.\d+.*?\n', ''),
        # Pattern 2: v1.0 (2024-12)
        (r'#\s*v\d+\.\d+\s*\([^)]+\):[^\n]*', '# v1.0.0 (2025-08-14): Initial generation from PDF specification'),
        # Pattern 3: Revision History
        (r'# Revision History:', '# Changelog:'),
        # Pattern 4: Version in comment
        (r'# Version: \d+\.\d+', '# Version: 1.0.0'),
        # Pattern 5: Generated date
        (r'# Generated: \d{4}-\d{2}-\d{2}', '# Generated: 2025-08-14'),
    ]
    
    for pattern, replacement in patterns:
        if replacement:
            content = re.sub(pattern, replacement, content)
    
    # Ensure proper header format if not present
    if '# Generated: 2025-08-14' not in content:
        lines = content.split('\n')
        # Find where to insert
        for i, line in enumerate(lines):
            if 'LoRaWAN Decoder' in line:
                # Insert standard header after decoder line
                lines.insert(i+1, '# Generated: 2025-08-14 | Version: 1.0.0 | Revision: 1')
                lines.insert(i+2, '#-------------------------------------------------------------')
                break
        content = '\n'.join(lines)
    
    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(content)
    print(f"Updated: {filepath}")

def update_md_file(filepath):
    """Update .md documentation file"""
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # Update patterns
    patterns = [
        (r'\*\*Driver Version\*\*: [\d\.]+', '**Driver Version**: 1.0.0'),
        (r'\*\*Generated\*\*: \d{4}-\d{2}-\d{2}', '**Generated**: 2025-08-14'),
        (r'- v[\d\.]+ \([^)]+\):[^\n]*', '- v1.0.0 (2025-08-14): Initial generation from PDF specification'),
    ]
    
    for pattern, replacement in patterns:
        content = re.sub(pattern, replacement, content)
    
    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(content)
    print(f"Updated: {filepath}")

# Process all vendor directories
base_path = r'C:\Project\tasmota\Tasmota\tasmota\berry\lorawan\decoders\AI-Generated-Driver\vendor'

for vendor in ['browan', 'dragino', 'milesight']:
    vendor_path = os.path.join(base_path, vendor)
    if os.path.exists(vendor_path):
        for filename in os.listdir(vendor_path):
            filepath = os.path.join(vendor_path, filename)
            if filename.endswith('.be'):
                update_be_file(filepath)
            elif filename.endswith('.md'):
                update_md_file(filepath)

print("All files updated to v1.0.0 with date 2025-08-14")
