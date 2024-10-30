var antiburn = module('antiburn')

antiburn.init = def (m)
    class Antiburn
        var antiburn            # the lv_obj object used as a plain color
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

                if self.antiburn == nil
                    var antiburn = lv.obj(lv.layer_sys())
                    antiburn.set_style_radius(0, 0)
                    antiburn.set_style_border_width(0, 0)
                    antiburn.set_style_bg_opa(255, 0)
                    antiburn.set_pos(0, 0)
                    antiburn.set_width(lv.get_hor_res())
                    antiburn.set_height(lv.get_ver_res())
                    
                    antiburn.add_event_cb(/->self.stop(), lv.EVENT_PRESSED, 0)
                    self.antiburn = antiburn
                end
                self.antiburn.set_style_bg_opa(255, 0)
                self.antiburn.add_flag(lv.OBJ_FLAG_CLICKABLE)
                self.antiburn.move_foreground()

                self.running = true
                self.cycle(0)
            end
        end
        def cycle(i)
            if !self.running || self.antiburn == nil return nil end
            if i < 30
                self.antiburn.set_style_bg_color(lv.color_hex(self.colors[i % 5]), 0)
                tasmota.set_timer(1000, /->self.cycle(i+1))
            else
                self.stop()
            end
        end
        def stop()
            if self.running && self.antiburn != nil
                self.antiburn.set_style_bg_opa(0, 0)
                self.antiburn.clear_flag(lv.OBJ_FLAG_CLICKABLE)
                self.running = false
                self.antiburn.del()
                self.antiburn = nil
            end    
        end
    end
    return Antiburn()
end

return antiburn
