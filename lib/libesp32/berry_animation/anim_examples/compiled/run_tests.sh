#!/bin/bash
# Test runner for compiled DSL examples
# Generated automatically by compile_all_examples.sh

set -e

BERRY_CMD="./berry -s -g -m lib/libesp32/berry_animation/src -e 'import tasmota def log(x,l) tasmota.log(x,l) end '"
COMPILED_DIR="lib/libesp32/berry_animation/anim_examples/compiled"

echo "Running compiled DSL examples..."
echo "==============================="

SUCCESS_COUNT=0
TOTAL_COUNT=0

for berry_file in "$COMPILED_DIR"/*.be; do
    if [ -f "$berry_file" ]; then
        filename=$(basename "$berry_file")
        echo -n "Testing $filename... "
        
        ((TOTAL_COUNT++))
        
        if eval "$BERRY_CMD \"$berry_file\"" > /dev/null 2>&1; then
            echo "✓"
            ((SUCCESS_COUNT++))
        else
            echo "✗"
            echo "  Error details:"
            eval "$BERRY_CMD \"$berry_file\"" 2>&1 | sed 's/^/    /'
        fi
    fi
done

echo ""
echo "Test Results: $SUCCESS_COUNT/$TOTAL_COUNT examples ran successfully"
