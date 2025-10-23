# Unit tests for EngineProxy class
#
# Tests the ability to create animations that combine both rendering
# and orchestration of sub-animations and sequences.

import animation

print("Starting EngineProxy Tests...")

# Create test engine
var strip = global.Leds(30)
var engine = animation.create_engine(strip)

# Test 1: Basic creation
print("\n=== Test 1: Basic Creation ===")
var proxy = animation.engine_proxy(engine)
assert(proxy != nil, "Engine proxy should be created")
assert(isinstance(proxy, animation.playable), "Engine proxy should be a Playable")
assert(isinstance(proxy, animation.animation), "Engine proxy should be an Animation")
assert(proxy.is_running == false, "Engine proxy should not be running initially")
print("✓ Basic creation test passed")

# Test 2: Add child animations
print("\n=== Test 2: Add Child Animations ===")
var child1 = animation.solid(engine)
child1.color = 0xFFFF0000  # Red
child1.name = "red_child"

var child2 = animation.solid(engine)
child2.color = 0xFF00FF00  # Green
child2.name = "green_child"

proxy.add(child1)
proxy.add(child2)
assert(size(proxy.animations) == 2, "Should have 2 animations")
print("✓ Add child animations test passed")

# Test 3: Add child sequence
print("\n=== Test 3: Add Child Sequence ===")
var seq = animation.sequence_manager(engine, 1)
seq.push_play_step(child1, 1000)
seq.push_wait_step(500)
seq.push_play_step(child2, 1000)

proxy.add(seq)
assert(size(proxy.animations) == 2, "Should have 2 animations total")
assert(size(proxy.sequences) == 1, "Should have 1 sequence")
print("✓ Add child sequence test passed")

# Test 4: Start engine proxy (should start all animations)
print("\n=== Test 4: Start Engine Proxy ===")
engine.time_ms = 1000
proxy.start(engine.time_ms)
assert(proxy.is_running == true, "Engine proxy should be running")
assert(child1.is_running == true, "Child1 should be running")
assert(child2.is_running == true, "Child2 should be running")
assert(seq.is_running == true, "Sequence should be running")
print("✓ Start engine proxy test passed")

# Test 5: Update engine proxy (should update all animations)
print("\n=== Test 5: Update Engine Proxy ===")
engine.time_ms = 1500
var result = proxy.update(engine.time_ms)
assert(result == true, "Engine proxy should still be running")
print("✓ Update engine proxy test passed")

# Test 6: Render engine proxy
print("\n=== Test 6: Render Engine Proxy ===")
var frame = animation.frame_buffer(30)
engine.time_ms = 2000
result = proxy.render(frame, engine.time_ms)
# Rendering should work (may or may not modify frame depending on animations)
print("✓ Render engine proxy test passed")

# Test 7: Stop engine proxy (should stop all animations)
print("\n=== Test 7: Stop Engine Proxy ===")
proxy.stop()
assert(proxy.is_running == false, "Engine proxy should be stopped")
assert(child1.is_running == false, "Child1 should be stopped")
assert(child2.is_running == false, "Child2 should be stopped")
assert(seq.is_running == false, "Sequence should be stopped")
print("✓ Stop engine proxy test passed")

# Test 8: Remove child
print("\n=== Test 8: Remove Child ===")
proxy.remove(child1)
assert(size(proxy.animations) == 1, "Should have 1 animations after removal")
proxy.remove(seq)
assert(size(proxy.animations) == 1, "Should have 1 child after sequence removal")
assert(size(proxy.sequences) == 0, "Should have 0 sequences after removal")
print("✓ Remove child test passed")

# Test 9: Engine proxy with own rendering
print("\n=== Test 9: Engine Proxy with Own Rendering ===")
var proxy2 = animation.engine_proxy(engine)
proxy2.color = 0xFF0000FF  # Blue background
proxy2.name = "blue_proxy"

var pulse = animation.breathe_animation(engine)
pulse.color = 0xFFFFFF00  # Yellow
pulse.period = 2000
pulse.name = "yellow_pulse"

proxy2.add(pulse)
engine.time_ms = 3000
proxy2.start(engine.time_ms)

var frame2 = animation.frame_buffer(30)
result = proxy2.render(frame2, engine.time_ms)
assert(result == true, "Engine proxy with own rendering should modify frame")
print("✓ Engine proxy with own rendering test passed")

# Test 10: Engine integration
print("\n=== Test 10: Engine Integration ===")
var proxy3 = animation.engine_proxy(engine)
proxy3.color = 0xFFFF00FF  # Magenta
proxy3.priority = 15
proxy3.name = "engine_proxy"

# Add to engine (should work since EngineProxy is a Playable)
engine.add(proxy3)
assert(size(engine.get_animations()) == 1, "Engine should have 1 animation")
print("✓ Engine integration test passed")

# Test 11: Type checking
print("\n=== Test 11: Type Checking ===")
assert(isinstance(proxy, animation.playable), "Engine proxy is a Playable")
assert(isinstance(proxy, animation.animation), "Engine proxy is an Animation")
assert(!isinstance(proxy, animation.sequence_manager), "Engine proxy is not a SequenceManager")
print("✓ Type checking test passed")

# Test 12: String representation
print("\n=== Test 12: String Representation ===")
var str_repr = str(proxy2)
assert(str_repr != nil, "String representation should exist")
print(f"Engine proxy string: {str_repr}")
print("✓ String representation test passed")

print("\n" + "="*50)
print("🎉 All EngineProxy tests passed!")
print("="*50)
