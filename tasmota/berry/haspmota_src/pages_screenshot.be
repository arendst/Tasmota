# simple script to make screenshots of HASPmota demo

def pageshot(p, cb)
  global.(f"p{p}").show()
  tasmota.set_timer(1000, 
    def ()
      log(f"SCRSHT: screenshot for page {p}")
      tasmota.cmd("backlog time 1704112620")
      tasmota.set_timer(1000,
        def ()
          var s = lv.screenshot()
          import path
          path.rename(s, f"/HASPmota_{p:02i}.bmp")
          log(f"SCRSHT: generated /HASPmota_{p:02i}.bmp")
          tasmota.defer(cb)
        end
      )
    end
  )
end

def next_page(p, last)
  pageshot(p,
    def ()
      var p_next = p + 1
      if p_next <= last
        next_page(p_next, last)
      else
        log(f"SCRSHT: finished")
      end
    end
  )
end

def start()
  next_page(1, 25)
end

