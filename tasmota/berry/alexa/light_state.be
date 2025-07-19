# tests for light_state

l = light_state(light_state.RGBW)

print(l)

l.set_rgb(10,20,30)
assert(l.r == 10)
assert(l.g == 20)
assert(l.b == 30)

assert(l.hue == 211)
assert(l.sat == 169)
assert(l.bri == 30)

assert(l.power == false)
l.set_power(true)
assert(l.power == true)

# light.get()
#{'bri': 102, 'hue': 0, 'power': false, 'sat': 77, 'rgb': '664747', 'channels': [102, 71, 71]}

l.set_huesat(100,200)
assert(l.hue == 100)
assert(l.sat == 200)
assert(l.bri == 30)

assert(l.r == 14)
assert(l.g == 30)
assert(l.b == 6)

assert(l.mode_rgb)

l.set_ct(400)
assert(l.ct == 400)

assert(l.mode_ct)

# assert(l.x == 0.215944)
# assert(l.y == 0.700957)

l.set_xy(0.3, 0.4)

# red
r = light_state(light_state.RGB)
r.set_rgb(255,0,0)
assert(r.hue == 0)
assert(r.sat == 255)
assert(r.bri == 255)
# x/y = 0.735 / 0.265

b = light_state(light_state.RGB)
b.set_rgb(0,0,255)
# x/y 0.157 / 0.018

y = light_state(light_state.RGB)
y.set_rgb(255,255,0)
# x/y 0.422332 0.547914

c = light_state(light_state.RGB)
c.set_xy(0.422, 0.548)

# 5 channels
l = light_state(light_state.RGBCT)

l.set_rgb(30,40,90)
assert(str(l.get()) == "{'rgb': '1E285A', 'hue': 230, 'type': 5, 'power': false, 'bri': 90, 'mode_rgb': true, 'sat': 170, 'mode_ct': false, 'channels': [30, 40, 90, 0, 0]}")

l.set_ct(340)
assert(str(l.get()) == "{'mode_ct': true, 'ct': 340, 'channels': [0, 0, 0, 42, 48], 'bri': 90, 'type': 5, 'mode_rgb': false, 'power': false}")

# gamma functions
assert(light_state.gamma8(0) == 0)
assert(light_state.gamma8(100) == 20)
assert(light_state.gamma8(255) == 255)

assert(light_state.gamma10(0) == 0)
assert(light_state.gamma10(100) == 7)
assert(light_state.gamma10(255) == 25)
assert(light_state.gamma10(1023) == 1023)

assert(light_state.reverse_gamma10(0) == 0)
assert(light_state.reverse_gamma10(7) == 107)
assert(light_state.reverse_gamma10(25) == 253)
assert(light_state.reverse_gamma10(1023) == 1023)
