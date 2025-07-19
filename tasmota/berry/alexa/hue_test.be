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

# import hue_ntv
# print(l._p)
# print(hue_ntv.light_state(l))

# print(hue_ntv.full_state(3, l, "aaa", "bbb", "ccc"))

import hue_bridge
hue_bridge.add_light(10, l, "Synthetic Light", "V1", "DeadParrot")

l1 = light_state(light_state.DIMMER)
hue_bridge.add_light(11, l1, "Synthetic Dimmer", "V1", "DeadParrot")
l2 = light_state(light_state.CT)
# hue_bridge.add_light(12, l2, "Synthetic CT", "V1", "DeadParrot")
hue_bridge.add_light(12, l2, "Synthetic CT")

l5 = light_state(light_state.RGBCT)
hue_bridge.add_light(15, l5, "Synthetic RGBCT")

