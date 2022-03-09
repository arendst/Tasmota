def event_handler(obj, evt)
    var code = evt.code

    if code == lv.EVENT_CLICKED
            print("Clicked event seen")
    elif code == lv.EVENT_VALUE_CHANGED
        print("Value changed seen")
    end
end
        
# create a simple button
btn1 = lv.btn(lv.scr_act())

# attach the callback
btn1.add_event_cb(event_handler,lv.EVENT_ALL, nil)

btn1.align(lv.ALIGN_CENTER,0,-40)
label=lv.label(btn1)
label.set_text("Button")

# create a toggle button
btn2 = lv.btn(lv.scr_act())

# attach the callback
#btn2.add_event_cb(event_handler,lv.EVENT.VALUE_CHANGED,None)
btn2.add_event_cb(event_handler,lv.EVENT_ALL, nil)

btn2.align(lv.ALIGN_CENTER,0,40)
btn2.add_flag(lv.OBJ_FLAG_CHECKABLE)
btn2.set_height(lv.SIZE_CONTENT)

label=lv.label(btn2)
label.set_text("Toggle")
label.center()
