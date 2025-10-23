# Simple test for GradientAnimation
import animation

print("Testing basic GradientAnimation functionality...")

# Create LED strip and engine
var strip = global.Leds(5)
var engine = animation.create_engine(strip)

# Test basic creation
var gradient = animation.gradient_animation(engine)
assert(gradient != nil, "Should create gradient animation")

# Test parameter setting
gradient.color = 0xFFFF0000
gradient.gradient_type = 0
gradient.movement_speed = 50

# Test parameter access
assert(gradient.color == 0xFFFF0000, "Should set color")
assert(gradient.gradient_type == 0, "Should set gradient type")
assert(gradient.movement_speed == 50, "Should set movement speed")

# Test start and update
gradient.start(1000)
assert(gradient.is_running == true, "Should be running")

var result = gradient.update(1000)
assert(result == true, "Should update successfully")

# Test rendering
var frame = animation.frame_buffer(5, 1)
result = gradient.render(frame, 1000)
assert(result == true, "Should render successfully")

print("✓ Basic GradientAnimation test passed!")

return true