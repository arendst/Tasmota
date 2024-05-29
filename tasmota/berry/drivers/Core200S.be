import string
import webserver

class StateEnum
    var value_map
    var value

    def init(value_map, value)
        self.value_map = value_map
        if type(value) == 'int'
            for k : self.value_map.keys()
                if self.value_map[k] == value
                    self.value = value
                    break
                end
            end
        elif type(value) == 'string'
            if self.value_map.contains(value)
                self.value = self.value_map[value]
            end
        end

        if self.value == nil
            raise "StateEnumError", "Invalid type or value"
        end
    end

    def tostring()
        for k : self.value_map.keys()
            if self.value_map[k] == self.value
                return k
            end
        end
        return "invalid"
    end

    def toint()
        return self.value
    end

    def ==(other)
        return self.value == other.value
    end

end

class OnOffState : StateEnum
    static ON = OnOffState("on")
    static OFF = OnOffState("off")
    def init(value)
        super(self).init({"off": 0, "on": 1}, value)
    end
end

class FanMode : OnOffState
    def init(value)
        super(self).init(value)
    end
end

class ChildLockMode : OnOffState
    def init(value)
        super(self).init(value)
    end
end

class SleepMode : OnOffState
    def init(value)
        super(self).init(value)
    end
end

class DisplayAutoOffMode : StateEnum
    static OFF = DisplayAutoOffMode("off")
    static ON = DisplayAutoOffMode("on")
    def init(value)
        super(self).init({"off": 1, "on": 0}, value)
    end
end

class DisplayMode : StateEnum
    def init(value)
        super(self).init({"off": 0, "on": 100}, value)
    end
end

class FanSpeedMode : StateEnum
    static LOW = FanSpeedMode("low")
    static MED = FanSpeedMode("med")
    static FULL = FanSpeedMode("full")

    def init(value)
        super(self).init({"low": 1, "med": 2, "full": 3}, value)
    end
end

class NightLightMode : StateEnum
    static OFF = NightLightMode("off")
    static LOW = NightLightMode("low")
    static FULL = NightLightMode("full")
    def init(value)
        super(self).init({"off": 0, "low": 50, "full": 100}, value)
    end
end

def modulo(num, mod)
    return ((num % mod) + mod) % mod
end

def info_c2s(msg)
    log("C2S: " + msg, 2)
end

def log_c2s(msg)
    log("C2S: " + msg)
end

class Core200SSerial : serial
    var msg_count
    var read_buffer
    var error_count
    var expected_replies

    def init()
        super(self).init(16, 17, 115200)
        self.reset_counters()
        self.read_buffer = bytes("")
    end

    def deinit()
        self.close()
    end

    static def calc_checksum(msg)
        var checksum = 0xff
        var i = 0
        while i < msg.size()
            if (i != 5)
                checksum -= msg.get(i)
                checksum = modulo(checksum, 256)
            end
            i += 1
        end
        return checksum
    end

    def send(msg)
        self.send_nowait(msg)
        tasmota.delay(100)
        return self.read_msg()
    end

    def send_nowait(msg)
        if msg.size() >= 2 && msg[1] == 0x22
            self.expected_replies += 1
        end

        # increase and set message counter
        self.msg_count += 1
        msg[2] = self.msg_count

        # insert checksum
        msg[5] = self.calc_checksum(msg)

        # send message
        var i = 0
        while i < msg.size()
            self.write(msg[i])
            i += 1
        end

        log_c2s("Send " + msg.tohex())
    end

    def available()
        self.read_buffer += self.read()
        return self.read_buffer.siez()
    end

    # read but don't remove from buffer
    def read_ahead(num)
        var content = self.read()
        self.read_buffer += content

        if self.read_buffer.size() >= num
            return self.read_buffer[0..(num-1)]
        end

        return nil
    end

    # read an remove from buffer
    def consume(num)
        var content = self.read_ahead(num)
        if content != nil
            self.read_buffer = self.read_buffer[num..]
        end
        return content
    end

    def read_msg(allow_no_msg)
        var max_read_attempts = 10
        var header = self.read_ahead(6)
        if header == nil
            header = bytes("0000")
        end
        while header[0] != 0xa5 || (header[1] != 0x12 && header[1] != 0x22)
            if header != bytes("0000")
                self.consume(1)
            else
                max_read_attempts -= 1
                if max_read_attempts <= 0
                    if !allow_no_msg
                        info_c2s("Too many read attempts (couldn't read message header)")
                        self.error_count += 1
                    end
                    return bytes("")
                end
                tasmota.delay(100)
            end
            header = self.read_ahead(6)
            if header == nil
                header = bytes("0000")
            end
        end

        if header == bytes("0000")
            info_c2s("Too many read attempts (couldn't read message header)")
            self.error_count += 1
            return bytes("")
        else
            self.consume(6)
        end

        log_c2s("Got Header " + header.tohex())

        var payload = self.consume(header[3])
        while payload == nil
            max_read_attempts -= 1
            if max_read_attempts <= 0
                if !allow_no_msg
                    self.error_count += 1
                    info_c2s("Too many read attempts (couldn't read message payload)")
                end
                return bytes("")
            end
            payload = self.consume(header[3])
        end

        var msg = header + payload
        info_c2s("Full Message " + msg.tohex())

        self.expected_replies -= 1

        var checksum = self.calc_checksum(msg)
        if checksum != msg[5]
            info_c2s("Invalid Checksum")
            return bytes("")
        end

        return msg

    end

    def check_errors()
        return self.error_count > 10 || self.expected_replies > 10
    end

    def reset_counters()
        self.error_count = 0
        self.expected_replies = 0
        self.msg_count = 0
    end
end

ser = Core200SSerial()

class Core200S
    var wifi_on
    var second_counter
    var fan_mode
    var sl_mode
    var fan_speed_mode
    var dp_mode
    var dp_auto_off_mode
    var cl_mode
    var nl_mode
    var timer_remaining
    var timer_total

    def init()
        self.wifi_on = nil
        self.second_counter = 0
        self.timer_remaining = 0
        self.timer_total = 0
        self.init_cmds()
    end

    def init_cmds()
        ser.flush()
        ser.reset_counters()

        tasmota.delay(1000)
        ser.send(bytes("a522010500aa01e2a50000"))

        tasmota.delay(200)
        ser.read_msg(false)

        tasmota.delay(200)
        ser.send_nowait(bytes("a512000400a301604000"))

        tasmota.delay(200)
        ser.send(bytes("a5220204009001614000"))

        tasmota.delay(200)
        self.set_wifi_led(tasmota.wifi() != {})

        tasmota.delay(200)
        self.query_state()

        tasmota.delay(200)
        self.query_timer()

    end

    def set_wifi_led(on)
        if self.wifi_on != on
            var msg
            if on
                msg = bytes("a522050a00630129a100017d007d0000")
            else
                msg = bytes("a522030a00760129a10000f401f40100")
            end
            ser.send_nowait(msg)
            self.wifi_on = on
        end
    end

    def query_state()
        ser.send_nowait(bytes("a5220604008c01614000"))
    end

    def query_timer()
        ser.send_nowait(bytes("a522070400250165a200"))
    end

    def set_fan_speed(mode)
        var msg = bytes("a522070700250160a2000001")
        msg.add(mode.toint(), 1)
        return ser.send(msg) != bytes("")
    end

    def toggle_fan_speed()
        if self.fan_speed_mode == FanSpeedMode.LOW
            return self.set_fan_speed(FanSpeedMode.MED)
        elif self.fan_speed_mode == FanSpeedMode.MED
            return self.set_fan_speed(FanSpeedMode.FULL)
        else
            # default case for nil value
            return self.set_fan_speed(FanSpeedMode.LOW)
        end
    end

    def set_fan_mode(mode)
        var msg = bytes("a5220705008a0100a000")
        msg.add(mode.toint(), 1)
        return ser.send(msg) != bytes("")
    end

    def toggle_fan_mode()
        if self.fan_mode == FanMode.ON
            return self.set_fan_mode(FanMode.OFF)
        else
            return self.set_fan_mode(FanMode.ON)
        end
    end

    def set_night_light_mode(mode)
        var msg = bytes("a522090600210103a00000")
        msg.add(mode.toint(), 1)
        return ser.send(msg) != bytes("")
    end

    def toggle_night_light()
        if self.nl_mode == NightLightMode.FULL
            return self.set_night_light_mode(NightLightMode.LOW)
        elif self.nl_mode == NightLightMode.LOW
            return self.set_night_light_mode(NightLightMode.OFF)
        else
            return self.set_night_light_mode(NightLightMode.FULL)
        end
    end

    def set_child_lock_mode(mode)
        var msg = bytes("a5220705005a0100d100")
        msg.add(mode.toint(), 1)
        return ser.send(msg) != bytes("")
    end

    def toggle_child_lock()
        if self.cl_mode == ChildLockMode.ON
            return self.set_child_lock_mode(ChildLockMode.OFF)
        else
            return self.set_child_lock_mode(ChildLockMode.ON)
        end
    end

    def set_sleep_mode(mode)
        if mode == SleepMode.ON
            var msg = bytes("a522070500a501e0a50001")
            return ser.send(msg) != bytes("")
        else
            return self.set_fan_speed(self.fan_speed_mode)
        end
    end

    def toggle_sleep_mode()
        if self.sl_mode == SleepMode.ON
            return self.set_sleep_mode(SleepMode.OFF)
        else
            return self.set_sleep_mode(SleepMode.ON)
        end
    end

    def set_display_auto_off_mode(mode)
        var msg = bytes("a522070500210105a100")
        if mode == DisplayAutoOffMode.ON
            msg.add(0x00, 1)
        else
            msg.add(0x64, 1)
        end
        return ser.send(msg) != bytes("")
    end

    def toggle_display_auto_off()
        if self.dp_auto_off_mode == DisplayAutoOffMode.ON
            return self.set_display_auto_off_mode(DisplayAutoOffMode.OFF)
        else
            return self.set_display_auto_off_mode(DisplayAutoOffMode.ON)
        end
    end

    def set_timer(seconds)
        # check it timer if set and don't try to clean timer if not set
        # (The stock firmware also checks if timer is set before it cleans it)
        if seconds == 0 && self.timer_total == 0
            return true
        end
        var msg = bytes("a522080800210164a20000000000")
        msg.set(10, seconds, 3)
        return ser.send(msg) != bytes("")
    end

    def parse_status_message(msg)
        var filter_reset_msg = bytes("a522070400250165a200")
        if msg.size() == 22
            self.fan_mode = FanMode(msg[13])
            log_c2s("FanMode: " +  self.fan_mode.tostring())
            self.sl_mode = SleepMode(msg[14])
            log_c2s("SleepMode: " +  self.sl_mode.tostring())
            self.fan_speed_mode = FanSpeedMode(msg[15])
            log_c2s("FanSpeedMode: " +  self.fan_speed_mode.tostring())
            self.dp_mode = DisplayMode(msg[16])
            log_c2s("DisplayMode: " +  self.dp_mode.tostring())
            self.dp_auto_off_mode = DisplayAutoOffMode(msg[17])
            log_c2s("DisplayAutoOffMode: " +  self.dp_auto_off_mode.tostring())
            self.cl_mode = ChildLockMode(msg[20])
            log_c2s("ChildLockMode: " +  self.cl_mode.tostring())
            self.nl_mode = NightLightMode(msg[21])
            log_c2s("NightLightMode: " +  self.nl_mode.tostring())

            # When timer is set it sends a status message (but not a message that actually contains the timer values)
            # Thus on each status update we should query if a timer has been set
            if msg[1] == 0x22
                tasmota.delay(50)
                self.query_timer()
            end
        elif filter_reset_msg == msg
            info_c2s("Detected Filter Reset. Not Implemented...")
        elif msg.size() == 18
            self.timer_remaining = msg.get(10, 3)
            self.timer_total = msg.get(14, 3)
            log_c2s(format("TimerRemaining: %d", self.timer_remaining))
            log_c2s(format("TimerTotal: %d", self.timer_total))
        end


    end

    def every_250ms()
        var msg = ser.read_msg(true)

        while msg != bytes("")
            info_c2s("Incoming Message " + msg.tohex())

            if msg[0] == 0xa5
                if msg[1] == 0x22
                    ser.send_nowait(bytes("a512000400a301604000"))
                    self.parse_status_message(msg)
                elif msg[1] == 0x12
                    self.parse_status_message(msg)
                else
                    log_c2s("Unrecognized message type")
                end
            end
            msg = ser.read_msg(true)
        end
    end

    def every_second()
        self.second_counter += 1
        if self.second_counter == 60
            self.second_counter = 0
            self.set_wifi_led(tasmota.wifi() != {})
            self.query_timer()
        elif modulo(self.second_counter, 5) == 0
            if self.timer_total != 0
                self.query_timer()
            end
        end

        if ser.check_errors()
            self.init_cmds()
        end
    end

    def json_append()
        tasmota.response_append(",\"C200S\":{")

        tasmota.response_append(format("\"FanSpeedMode\":\"%s\"", self.fan_speed_mode))
        tasmota.response_append(format(",\"FanMode\":\"%s\"", self.fan_mode))
        tasmota.response_append(format(",\"NightLightMode\":\"%s\"", self.nl_mode))
        tasmota.response_append(format(",\"ChildLockMode\":\"%s\"", self.cl_mode))
        tasmota.response_append(format(",\"SleepMode\":\"%s\"", self.sl_mode))
        tasmota.response_append(format(",\"DisplayMode\":\"%s\"", self.dp_mode))
        tasmota.response_append(format(",\"DisplayAutoOff\":\"%s\"", self.dp_auto_off_mode))
        tasmota.response_append(format(",\"TimerTotal\":%d", self.timer_total))
        tasmota.response_append(format(",\"TimerRemaining\":%d", self.timer_remaining))

        tasmota.response_append("}")
    end

    static def secondsToReadableTime(seconds)
        var hours = seconds / 3600
        seconds -= hours * 3600
        var minutes = seconds / 60
        seconds -= minutes * 60
        return format("%02d:%02d:%02d", hours, minutes, seconds)
    end

    def web_sensor()
        var msg = format(
            "{s}Fan Mode{m}%s{e}"..
            "{s}Fan Speed{m}%s{e}"..
            "{s}Night Light{m}%s{e}"..
            "{s}Child Lock{m}%s{e}"..
            "{s}Sleep Mode{m}%s{e}"..
            "{s}Display Mode{m}%s{e}"..
            "{s}Display Auto Off{m}%s{e}",
            self.fan_mode, self.fan_speed_mode, self.nl_mode, self.cl_mode,
            self.sl_mode, self.dp_mode, self.dp_auto_off_mode
        )
        tasmota.web_send_decimal(msg)

        if self.timer_total != 0
            var timer_msg = format(
                "{s}Timer Remaining{m}%s{e}"..
                "{s}Timer Total{m}%s{e}",
                self.secondsToReadableTime(self.timer_remaining), self.secondsToReadableTime(self.timer_total)
            )
            tasmota.web_send_decimal(timer_msg)
        else
            tasmota.web_send_decimal("{s}Timer{m}not active{e}")
        end
    end

    def web_add_main_button()
        var button_config = "<td style=\"width:33.33%%\"><button onclick=\"fetch('cm?' + new URLSearchParams({cmnd: 'C2S_%s toggle'}))\">%s</button></td>"
        webserver.content_send("<p></p><center><table style=\"width:100%\"><tbody><tr>")

        webserver.content_send(format(button_config, "fan_mode", "Mode"))
        webserver.content_send(format(button_config, "fan_speed", "Speed"))
        webserver.content_send(format(button_config, "sleep_mode", "Sleep"))

        webserver.content_send("</tr><tr>")

        webserver.content_send(format(button_config, "night_light", "Light"))
        webserver.content_send(format(button_config, "child_lock", "Lock"))
        webserver.content_send(format(button_config, "display_auto_off", "Display"))
        
        webserver.content_send("</tr></tbody></table></center>")

    end

end

var c200s = Core200S()

tasmota.add_driver(c200s)

tasmota.add_cmd('C2S_fan_speed', def(cmd, idx, payload)
    var res = false
    if string.startswith(payload, "toggle")
        res = c200s.toggle_fan_speed()
    elif payload != ""
        res = c200s.set_fan_speed(FanSpeedMode(payload))
    else
        tasmota.resp_cmnd(format("{\"FanSpeedMode\": \"%s\"}", c200s.fan_speed_mode))
        return
    end

    if res
        tasmota.resp_cmnd_done()
    end
end)

tasmota.add_cmd('C2S_fan_mode', def(cmd, idx, payload)
    var res = false
    if string.startswith(payload, "toggle")
        res = c200s.toggle_fan_mode()
    elif payload != ""
        res = c200s.set_fan_mode(FanMode(payload))
    else
        tasmota.resp_cmnd(format("{\"FanMode\": \"%s\"}", c200s.fan_mode))
        return
    end

    if res
        tasmota.resp_cmnd_done()
    end
end)

tasmota.add_cmd('C2S_child_lock', def(cmd, idx, payload)
    var res = false
    if string.startswith(payload, "toggle")
        res = c200s.toggle_child_lock()
    elif payload != ""
        res = c200s.set_child_lock_mode(ChildLockMode(payload))
    else
        tasmota.resp_cmnd(format("{\"ChildLockMode\": \"%s\"}", c200s.cl_mode))
        return
    end

    if res
        tasmota.resp_cmnd_done()
    end
end)

tasmota.add_cmd('C2S_night_light', def(cmd, idx, payload)
    var res = false
    if string.startswith(payload, "toggle")
        res = c200s.toggle_night_light()
    elif payload != ""
        res = c200s.set_night_light_mode(NightLightMode(payload))
    else
        tasmota.resp_cmnd(format("{\"NightLightMode\": \"%s\"}", c200s.nl_mode))
        return
    end

    if res
        tasmota.resp_cmnd_done()
    end
end)

tasmota.add_cmd('C2S_sleep_mode', def(cmd, idx, payload)
    var res = false
    if string.startswith(payload, "toggle")
        res = c200s.toggle_sleep_mode()
    elif payload != ""
        res = c200s.set_sleep_mode(SleepMode(payload))
    else
        tasmota.resp_cmnd(format("{\"SleepMode\": \"%s\"}", c200s.sl_mode))
        return
    end

    if res
        tasmota.resp_cmnd_done()
    end
end)

tasmota.add_cmd('C2S_display_auto_off', def(cmd, idx, payload)
    var res = false
    if string.startswith(payload, "toggle")
        res = c200s.toggle_display_auto_off()
    elif payload != ""
        res = c200s.set_display_auto_off_mode(DisplayAutoOffMode(payload))
    else
        tasmota.resp_cmnd(format("{\"DisplayAutoOff\": \"%s\"}", c200s.dp_auto_off_mode))
        return
    end

    if res
        tasmota.resp_cmnd_done()
    end
end)

tasmota.add_cmd('C2S_timer', def(cmd, idx, payload)
    if payload != ""
        var seconds = int(payload)

        if seconds >= 24*60*60 || seconds < 0
            # timer only supports max value of 24h - 1s
            return
        end

        if c200s.set_timer(seconds)
            tasmota.resp_cmnd_done()
        end
    else
        tasmota.resp_cmnd(format("{\"TimerTotal\":%d,\"TimerRemaining\":%d}", c200s.timer_total, c200s.timer_remaining))
    end
end)
