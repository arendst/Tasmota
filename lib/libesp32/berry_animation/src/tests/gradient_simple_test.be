# Simple test for gradient
import animation

print("Testing basic gradient functionality...")

# Create LED strip and engine
var strip = global.Leds(5)
var engine = animation.create_engine(strip)

# Test basic creation
var gradient = animation.gradient(engine)
assert(gradient != nil, "Should create gradient animation")

# Test parameter setting
gradient.color1 = 0xFF000000
gradient.color2 = 0xFFFF0000
gradient.gradient_type = 0
gradient.direction = 0

# Test parameter access
assert(gradient.color1 == 0xFF000000, "Should set color1")
assert(gradient.color2 == 0xFFFF0000, "Should set color2")
assert(gradient.gradient_type == 0, "Should set gradient type")
assert(gradient.direction == 0, "Should set direction")

# Test start and update
gradient.start(1000)
assert(gradient.is_running == true, "Should be running")

gradient.update(1000)
assert(gradient.is_running == true, "Should still be running after update")

# Test rendering
var frame = animation.frame_buffer(5)
var result = gradient.render(frame, 1000, engine.strip_length)
assert(result == true, "Should render successfully")

print("✓ Basic gradient test passed!")

return true
