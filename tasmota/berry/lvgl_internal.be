#-
 - Internal representation of LVGL
 -
 -#

class lv_screen_ntv # this one is embedded
  def init() end
end

class lv_screen : lv_screen_ntv # screen type and configuration
  var type, sub_type
  var width, height
  var rotation
  var opt
  def init(type, sub_type)
    super(self).init()
    self.type = type
    self.sub_type = int(sub_type)
    # default values
    self.width = 120
    self.height = 80
    self.rotation = 0
    self.opt = nil
  end

  def set_size(width, height)
    self.width = int(width)
    self.height = int(height)
  end

  def set_rotation(rotation)
    self.rotation = rotation
  end

  def set_opt(r0,r1,r2,r3)
    self.opt=[r0,r1,r2,r3]
  end
end

####################

class lv_connect_ntv def init() end end  # this one is embedded in code

class lv_connect : lv_connect_ntv # connection scheme for the screen
  var type, sub_type

  def init()
    super(self).init()
    self.type = ""    # unknown type
    self.sub_type = ""
  end
end

class lv_connect_spi : lv_connect # spi connection
  def init()
    super(self).init()
    self.type = "spi"
  end
end

class lv_connect_hspi : lv_connect_spi # hardware spi
  var cs, dc, rst

  def init(cs, dc, rst)
    super(self).init()
    self.sub_type = "hspi"
    self.cs = int(cs)
    self.dc = int(dc)
    self.rst = int(rst)
  end
end

##############################################

class lvlg_ntv
  def scr_act() end
end

class lvlg : lvlg_ntv
  def init(screen, connect)
    # do some stuff
  end
end


sc = lv_screen('ili934x', 1)
sp = lv_connect_hspi(1,2,3)
