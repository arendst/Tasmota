#-
Simplified Tasmota TM1637 driver written in Berry
Might be helpful in the case of using multiple displays
Supports only the 4 digit basic display

DIO_PIN and CLK_PIN are your esp32 pin numbers
How to use

> load('tm1637')
> tm = Tm1637(DIO_PIN, CLK_PIN)
> tm.set_on(4)
> tm.print('0123')
> tm.print(456)

Add custom commands to the native Tasmota console:
> tm_add_custom_commands(DIO_PIN, CLK_PIN)

And then:
TmBrightness 2
TmPrint 0123
TmPrint -5.67

Note: adding these commands to autoexec.be should be performed via creating an additional .be file with the content:
  tm_add_custom_commands(DIO_PIN, CLK_PIN)

and then loading it in autoexec.be via load('tm1637') and load('script_name')
The direct addition may not work
-#

class Tm1637
    static var CMD_CTRL = 0x80
    static var CMD_DISP_ON = 0x08
    static var CMD_DATA = 0x40
    static var CMD_ADDR = 0xC0

    static var SYMB_DOT = 0x80

    static var DIGIT_MAP = {
        '0': 0x3F,
        '1': 0x06,
        '2': 0x5B,
        '3': 0x4F,
        '4': 0x66,
        '5': 0x6D,
        '6': 0x7D,
        '7': 0x07,
        '8': 0x7F,
        '9': 0x6F,
        '-': 0x40,
        ' ': 0x00
    }

    var PIN_DIO
    var PIN_CLK

    def init(dio, clk)
        self.PIN_DIO = dio
        self.PIN_CLK = clk
        gpio.pin_mode(self.PIN_DIO, gpio.OUTPUT)
        gpio.pin_mode(self.PIN_CLK, gpio.OUTPUT)
        gpio.digital_write(self.PIN_DIO, 1)
        gpio.digital_write(self.PIN_CLK, 1)
    end

    def start()
        gpio.digital_write(self.PIN_DIO, 1)
        gpio.digital_write(self.PIN_CLK, 1)
        gpio.digital_write(self.PIN_DIO, 0)
    end

    def stop()
        gpio.digital_write(self.PIN_CLK, 0)
        gpio.digital_write(self.PIN_DIO, 0)
        gpio.digital_write(self.PIN_CLK, 1)
        gpio.digital_write(self.PIN_DIO, 1)
    end

    def ack()
        gpio.digital_write(self.PIN_CLK, 0)
        gpio.pin_mode(self.PIN_DIO, gpio.INPUT_PULLUP)
        var ack_state = gpio.digital_read(self.PIN_DIO) == 0
        gpio.digital_write(self.PIN_CLK, 1)
        gpio.digital_write(self.PIN_CLK, 0)
        gpio.pin_mode(self.PIN_DIO, gpio.OUTPUT)
        return ack_state
    end

    def write_bit(bitval)
        gpio.digital_write(self.PIN_CLK, 0)
        gpio.digital_write(self.PIN_DIO, bitval)
        gpio.digital_write(self.PIN_CLK, 1)
    end

    def write_byte(byteval)
        for pos: 0..7
            self.write_bit((byteval >> pos) & 0x01)
        end
    end

    def send_command(command)
        self.start()
        self.write_byte(command)
        var ack_state = self.ack()
        self.stop()
        return ack_state
    end

    def send_data(data)
        var ack_state = true
        self.start()
        for i : 0..size(data)-1
            self.write_byte(data[i])
            ack_state = self.ack() && ack_state
        end
        self.stop()
        return ack_state
    end

    #  0-8 range, 0 to 'OFF'
    def set_on(brightness)
        if brightness == nil || brightness > 8
            brightness = 8
        elif brightness < 0
            brightness = 0
        end
        var cmd = self.CMD_CTRL
        if brightness
            cmd |= self.CMD_DISP_ON
            brightness -= 1
        end
        return self.send_command(cmd | brightness)
    end

    def print(num)
        import string

        num = str(num)
        var max_str_len = 4

        do
            var dot_pos = string.find(num, '.')
            if dot_pos >= 0 && dot_pos < 5
                max_str_len = 5
            end
        end
        if size(num) > max_str_len
            num = string.split(num, max_str_len)[0]
        end
        num = format('%4s', num)
        var payload = bytes(-5)
        payload[0] = self.CMD_ADDR
        var int_offset = 1
        for i : 0..size(num)-1
            if num[i] == '.'
                payload[i] |= self.SYMB_DOT
                int_offset = 0
            else
                payload[i + int_offset] = self.DIGIT_MAP[num[i]]
            end
        end
        var ack_state = self.send_command(self.CMD_DATA) && self.send_data(payload)
        if !ack_state
            log('TM1637 - no ACK, please check connections')
        end
        return ack_state
    end

    def clear()
        self.print('    ')
    end

    # Won't be called on the system restart
    def deinit()
        self.set_on(0)
    end
end

def tm_add_custom_commands(dio, clk)
    var tm = Tm1637(dio, clk)
    tm.clear()
    tm.set_on(4)
    tasmota.add_cmd('tmprint', def(cmd, idx, payload)
        tm.print(payload) ? tasmota.resp_cmnd_done() : tasmota.resp_cmnd_failed()
    end)
    #  0-8 range, 0 to 'OFF'
    tasmota.add_cmd('tmbrightness', def(cmd, idx, payload)
        tm.set_on(int(payload)) ? tasmota.resp_cmnd_done() : tasmota.resp_cmnd_failed()
    end)
    log("Tasmota custom commands registered: TmPrint, TmBrightness")
end
