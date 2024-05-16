# OpenHASP demo
# rm haspmota_demo.tapp ; zip -j -0 haspmota_demo.tapp ../haspmota_src/haspmota_demo/*

if !tasmota.memory().contains("psram")
    print("HSP: Error: OpenHASP demo requires PSRAM")
    return
end

import lv_tasmota_log
import lv_tasmota_info
import lv_wifi_graph

import haspmota
haspmota.start(false, tasmota.wd + "pages.jsonl")

var prev_day = -1
def set_watch()
    import global
    var now = tasmota.rtc()
    var time_raw = now['local']
    var time = tasmota.time_dump(time_raw)
    # set second
    global.p5b13.angle = 60 * time['sec']
    # set minutes
    global.p5b12.angle = 60 * time['min']
    # set hours
    global.p5b11.angle = 300 * (time['hour'] % 12) + time['min'] * 5
    # set day
    if time['day'] != prev_day
        global.p5b15.text = str(time['day'])
        prev_day = time['day']
    end
end

def run_watch()
    set_watch()
    tasmota.set_timer(100, run_watch)
end
run_watch()

def p5_in()
    import global
	global.p0b101.bg_opa = 0
	global.p0b102.bg_opa = 0
	global.p0b103.bg_opa = 0
	global.p0b11.bg_opa = 0
end

def p5_out()
    import global
	global.p0b101.bg_opa = 255
	global.p0b102.bg_opa = 255
	global.p0b103.bg_opa = 255
	global.p0b11.bg_opa = 255
end

tasmota.add_rule("hasp#p5=in", p5_in)
tasmota.add_rule("hasp#p5=out", p5_out)
