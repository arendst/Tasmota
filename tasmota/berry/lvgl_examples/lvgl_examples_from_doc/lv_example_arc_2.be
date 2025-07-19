#
# An `lv_timer` to call periodically to set the angles of the arc
#
class ArcLoader
    var a
    def init()
        self.a = 270
    end
        
    def arc_loader_cb(tim,arc)
        # print(tim,arc)
        self.a += 5

        arc.set_end_angle(self.a)

        if self.a >= 270 + 360
            tim.del()
        end
    end
end


#
# Create an arc which acts as a loader.
#

# Create an Arc
arc = lv.arc(lv.scr_act())
arc.set_bg_angles(0, 360)
arc.set_angles(270, 270)
arc.center()

# create the loader
arc_loader = ArcLoader()

# Create an `lv_timer` to update the arc.

timer = lv.timer_create_basic()
timer.set_period(20)
timer.set_cb(/ src -> arc_loader.arc_loader_cb(timer,arc))
