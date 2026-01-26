#!/bin/bash
# Test runner for successfully compiled DSL examples

BERRY_CMD="./berry -s -g -m lib/libesp32/berry_animation"
COMPILED_DIR="compiled"

echo "Testing successfully compiled DSL examples..."
echo "============================================="

SUCCESS_COUNT=0
TOTAL_COUNT=0

echo -n "Testing aurora_borealis.be... "
if $BERRY_CMD "$COMPILED_DIR/aurora_borealis.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing breathing_colors.be... "
if $BERRY_CMD "$COMPILED_DIR/breathing_colors.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing candy_cane.be... "
if $BERRY_CMD "$COMPILED_DIR/candy_cane.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing christmas_tree.be... "
if $BERRY_CMD "$COMPILED_DIR/christmas_tree.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing comet_chase.be... "
if $BERRY_CMD "$COMPILED_DIR/comet_chase.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing disco_strobe.be... "
if $BERRY_CMD "$COMPILED_DIR/disco_strobe.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing fire_flicker.be... "
if $BERRY_CMD "$COMPILED_DIR/fire_flicker.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing heartbeat_pulse.be... "
if $BERRY_CMD "$COMPILED_DIR/heartbeat_pulse.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing lava_lamp.be... "
if $BERRY_CMD "$COMPILED_DIR/lava_lamp.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing lightning_storm.be... "
if $BERRY_CMD "$COMPILED_DIR/lightning_storm.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing matrix_rain.be... "
if $BERRY_CMD "$COMPILED_DIR/matrix_rain.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing meteor_shower.be... "
if $BERRY_CMD "$COMPILED_DIR/meteor_shower.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing neon_glow.be... "
if $BERRY_CMD "$COMPILED_DIR/neon_glow.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing ocean_waves.be... "
if $BERRY_CMD "$COMPILED_DIR/ocean_waves.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing palette_demo.be... "
if $BERRY_CMD "$COMPILED_DIR/palette_demo.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing palette_showcase.be... "
if $BERRY_CMD "$COMPILED_DIR/palette_showcase.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing plasma_wave.be... "
if $BERRY_CMD "$COMPILED_DIR/plasma_wave.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing police_lights.be... "
if $BERRY_CMD "$COMPILED_DIR/police_lights.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing property_assignment_demo.be... "
if $BERRY_CMD "$COMPILED_DIR/property_assignment_demo.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing rainbow_cycle.be... "
if $BERRY_CMD "$COMPILED_DIR/rainbow_cycle.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing scanner_larson.be... "
if $BERRY_CMD "$COMPILED_DIR/scanner_larson.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing simple_palette.be... "
if $BERRY_CMD "$COMPILED_DIR/simple_palette.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing sunrise_sunset.be... "
if $BERRY_CMD "$COMPILED_DIR/sunrise_sunset.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))

echo -n "Testing twinkle_stars.be... "
if $BERRY_CMD "$COMPILED_DIR/twinkle_stars.be" > /dev/null 2>&1; then
    echo "✓"
    ((SUCCESS_COUNT++))
else
    echo "✗"
fi
((TOTAL_COUNT++))


echo ""
echo "Test Results: $SUCCESS_COUNT/$TOTAL_COUNT examples executed successfully"
