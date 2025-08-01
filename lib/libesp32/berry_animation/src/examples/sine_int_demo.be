# Sine Int Demo
#
# This example demonstrates the fixed-point sine implementation
# that is optimized for performance on embedded systems.
#
# It shows how to use tasmota.sine_int() for efficient sine calculations
# in animations and other applications.

import global
import tasmota

print("Fixed-point sine demo")
print("=====================")
print("tasmota.sine_int() is an optimized fixed-point sine function")
print("Input: angle in range [0, 32767] representing [0, 2π]")
print("       where 8192 represents π/2")
print("Output: value in range [-4096, 4096] representing [-1.0, 1.0]")
print("")

# Print a table of sine values at different angles
print("Angle (degrees) | Angle (internal) | Sine value")
print("--------------- | --------------- | ----------")

# Calculate sine values for angles from 0 to 360 degrees in 30-degree increments
var degrees = 0
while (degrees < 361)
  # Convert degrees to the internal angle representation
  # 360 degrees = 32768 units
  var angle = tasmota.scale_uint(degrees, 0, 360, 0, 32768)
  
  # Calculate the sine value
  var sine = tasmota.sine_int(angle)
  
  # Print the result
  print(format("%15d | %15d | %10d", degrees, angle, sine))

  degrees += 30
end

print("")
print("Visualization of sine wave:")
print("")

# Create a simple ASCII visualization of the sine wave
var width = 60  # Width of the visualization
var height = 10  # Height of the visualization

# Create a buffer for the visualization
var buffer = []
for i:0..height-1
  var line = []
  for j:0..width-1
    line.push(" ")
  end
  buffer.push(line)
end

# Draw the sine wave
for x:0..width-1
  # Calculate the angle (0 to 2π over the width)
  var angle = tasmota.scale_uint(x, 0, width, 0, 32768)
  
  # Calculate the sine value (-4096 to 4096)
  var sine = tasmota.sine_int(angle)
  
  # Map the sine value to the height of the visualization
  var y = (height / 2) - tasmota.scale_uint(sine, -4096, 4096, -height/2, height/2)
  y = y < 0 ? 0 : (y >= height ? height - 1 : y)
  
  # Draw the point
  buffer[y][x] = "*"
end

# Draw the x-axis
var x_axis = height / 2
for x:0..width-1
  buffer[x_axis][x] = "-"
end

# Print the visualization
for y:0..height-1
  var line = ""
  for x:0..width-1
    line += buffer[y][x]
  end
  print(line)
end

print("")
print("Demo completed!")
return true