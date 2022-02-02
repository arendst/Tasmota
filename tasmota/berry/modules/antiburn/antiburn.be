var antiburn = module('antiburn')

antiburn.init = def (m)
    class Antiburn
        var scr_original
        var scr_antiburn
        var running
        static colors = [
            0x000000,
            0xff0000,
            0x00ff00,
            0x0000ff,
            0xffffff
        ]
        def init()
            self.running = false
        end
        def start()
            if self.running 
                return
            else
                lv.start()
                self.scr_original = lv.scr_act()
                self.scr_antiburn = lv.obj(0)
                lv.scr_load(self.scr_antiburn)
                self.scr_antiburn.add_event_cb(/->self.stop(), lv.EVENT_PRESSED, 0)
                self.running = true
                self.cycle(0)
            end
        end
        def cycle(i)
            if !self.running return end
            if i < 30
                self.scr_antiburn.set_style_bg_color(lv.color_hex(self.colors[i % 5]), 0)
                tasmota.set_timer(1000, /->self.cycle(i+1))
            else
                self.stop()
            end
        end
        def stop()
            if self.running && self.scr_antiburn != nil
                lv.scr_load(self.scr_original)
                self.running = false
                self.scr_antiburn.del()
                self.scr_antiburn = nil
            end    
        end
    end
    return Antiburn()
end

return antiburn