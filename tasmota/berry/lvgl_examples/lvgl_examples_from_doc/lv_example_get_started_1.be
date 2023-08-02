class CounterBtn
    var cnt
    var btn
    var label
    def init()
        self.cnt = 0
        #
        # Create a button with a label and react on click event.
        #

        var btn = lv.btn(lv.scr_act())                           # Add a button the current screen
        btn.set_pos(10, 10)                                      # Set its position
        btn.set_size(120, 50)                                    # Set its size
        btn.align(lv.ALIGN_CENTER,0,0)
        btn.add_event_cb(/ obj, evt -> self.btn_event_cb(evt), lv.EVENT_CLICKED, nil)   # Assign a callback to the button
        var label = lv.label(btn)                                    # Add a label to the button
        label.set_text("Button")                                 # Set the labels text
        label.center()

        self.btn = btn
        self.label = label
    end

    def btn_event_cb(evt)
        var code = evt.code
        if code == lv.EVENT_CLICKED
            self.cnt += 1
        end

        # Get the first child of the button which is the label and change its text
        self.label.set_text("Button: " + str(self.cnt))
    end
end

counterBtn = CounterBtn()
