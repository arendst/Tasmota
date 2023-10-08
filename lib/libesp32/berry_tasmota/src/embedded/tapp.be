#-  Tasmota apps module for Berry -#
#- -#

var tapp_module = module("tapp")

#@ solidify:tapp_module
tapp_module.init = def (m)

  class Tapp

    def init()
      tasmota.add_driver(self)
    end

    def autoexec()
      import path
      import string

      var dir = path.listdir("/")

      for d: dir
        if string.find(d, ".tapp") > 0
          tasmota.log(format("TAP: Loaded Tasmota App '%s'", d), 2)
          tasmota.load(d + "#autoexec.be")
        end
      end
    end
  end

  return Tapp()    # return an instance of this class
end

# aa = autoconf_module.init(autoconf_module)
# import webserver
# webserver.on('/ac2', / -> aa.page_autoconf_mgr(), webserver.HTTP_GET)
return tapp_module
