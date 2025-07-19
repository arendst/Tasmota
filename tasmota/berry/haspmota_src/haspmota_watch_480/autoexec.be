# This is a simple demo of displaying Renaissance Watch adapted for 480x480 displays
#
# Just install `haspmota_watch_480.tapp` in the filesystem
#
# Generate with
# rm haspmota_watch_480.tapp ; zip -j -0 haspmota_watch_480.tapp ../haspmota_src/haspmota_watch_480/*


# Test if display is at least 480x480
lv.start()
hres = lv.get_hor_res()       # should be 320
vres = lv.get_ver_res()       # should be 240
if lv.get_hor_res() < 480 || lv.get_ver_res() < 480
    print("Renaissance Watch 480 demo requires a screen at least 480x480")
    return
end

import haspmota
haspmota.start(false, tasmota.wd + "pages.jsonl")

var prev_day = -1
def set_watch()
    import global
    var now = tasmota.rtc()
    var time_raw = now['local']
    var time = tasmota.time_dump(time_raw)
    # set second
    if global.contains("p23b13")
        global.p23b13.angle = 60 * time['sec']
    end
    # set minutes
    global.p100b12.angle = 60 * time['min'] + (time['sec'] / 10) * 10
    # set hours
    global.p100b11.angle = 300 * (time['hour'] % 12) + time['min'] * 5
    # set day
    if time['day'] != prev_day
        global.p100b15.text = str(time['day'])
        prev_day = time['day']
    end
end

def run_watch()
    set_watch()
    tasmota.set_timer(1000, run_watch)
end
run_watch()

def watch_in()
    import global
    global.p0b90.hidden = true
    global.p0b101.bg_opa = 0
    global.p0b101.border_opa = 0
    global.p0b102.bg_opa = 0
    global.p0b102.border_opa = 0
    global.p0b102.text_opa = 0
    global.p0b103.bg_opa = 0
    global.p0b103.border_opa = 0
end

def watch_out()
    import global
    global.p0b90.hidden = false
    global.p0b101.bg_opa = 255
    global.p0b101.border_opa = 255
    global.p0b102.bg_opa = 255
    global.p0b102.border_opa = 255
    global.p0b102.text_opa = 255
    global.p0b103.bg_opa = 255
    global.p0b103.border_opa = 255
end

tasmota.add_rule("hasp#p100=in", watch_in)
tasmota.add_rule("hasp#p100=out", watch_out)