#-  Tasmota apps module for Berry -#
#- -#

var tapp_module = module("tapp")

#@ solidify:tapp_module
tapp_module.init = def (m)

  class Tapp

    # we make sure the instance is registered as a driver to receive the "autoexec" event
    def init()
      tasmota.add_driver(self)
    end

    # Takes a list of files, and run any tapp file's autoexec.be
    static def autoexec_dir(dir_name)
      import path
      import string
      for d: path.listdir(dir_name)
        if string.endswith(d, ".tapp")
          log(f"TAP: Loaded Tasmota App '{dir_name}{d}'", 2)
          tasmota.load(dir_name + d)
        end
      end
    end

    # react to the "autoexec" event that is triggered at first tick when Berry starts
    # Note: this is alled before the file "/autoexec.be" is loaded
    #
    # We run all `*.tapp` that are located in "/.extensions/" and in "/" in this order
    def autoexec()
      self.autoexec_dir("/.extensions/")
      self.autoexec_dir("/")
    end
  end

  return Tapp()    # return an instance of this class
end

return tapp_module
