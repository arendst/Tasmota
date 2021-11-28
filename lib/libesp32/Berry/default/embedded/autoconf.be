#-  autocong module for Berry -#
#- -#
#- To solidify: -#
#-
  # load only persis_module and persist_module.init
  import autoconf
  solidify.dump(autoconf_module)
  # copy and paste into `be_autoconf_lib.c`
-#
#-

# For external compile:

display = module("display")
self = nil
tasmota = nil
def load() end

-#

var autoconf_module = module("autoconf")

autoconf_module.init = def (m)

  class Autoconf
    var _archive
    var _error

    def init()
      import path
      import string

      var dir = path.listdir("/")
      var entry
      tasmota.add_driver(self)

      var i = 0
      while i < size(dir)
        if string.find(dir[i], ".autoconf") > 0   # does the file contain '*.autoconf', >0 to skip `.autoconf`
          if entry != nil
            # we have multiple configuration files, not allowed
            print(string.format("CFG: multiple autoconf files found, aborting ('%s' + '%s')", entry, dir[i]))
            self._error = true
            return nil
          end
          entry = dir[i]
        end
        i += 1
      end

      if entry == nil
        tasmota.log("CFG: no '*.autoconf' file found", 2)
        return nil
      end

      self._archive = entry
    end


    # ####################################################################################################
    # Manage first time marker
    # ####################################################################################################
    def is_first_time()
      import path
      return !path.exists("/.autoconf")
    end
    def set_first_time()
      var f = open("/.autoconf", "w")
      f.close()
    end
    def clear_first_time()
      import path
      path.remove("/.autoconf")
    end

    # ####################################################################################################
    # Delete all autoconfig files present
    # ####################################################################################################
    def delete_all_configs()
      import path
      import string
      var dir = path.listdir("/")

      for d:dir
        if string.find(d, ".autoconf") > 0   # does the file contain '*.autoconf'
          path.remove(d)
        end
      end
    end

    # ####################################################################################################
    # Get current module
    # contains the name of the archive without leading `/`, ex: `M5Stack_Fire.autoconf`
    # or `nil` if none
    # ####################################################################################################
    def get_current_module_path()
      return self._archive
    end
    def get_current_module_name()
      return self._archive[0..-10]
    end

    # ####################################################################################################
    # Load templates from Github
    # ####################################################################################################
    def load_templates()
      import string
      import json
      try 
        var url = string.format("https://raw.githubusercontent.com/tasmota/autoconf/main/%s_manifest.json", tasmota.arch())
        tasmota.log(string.format("CFG: loading '%s'", url), 3)
        # load the template
        var cl = webclient()
        cl.begin(url)
        var r = cl.GET()
        if r != 200
          tasmota.log(string.format("CFG: return_code=%i", r), 2)
          return nil
        end
        var s = cl.get_string()
        cl.close()
        # convert to json
        var j = json.load(s)
        tasmota.log(string.format("CFG: loaded '%s'", str(j)), 3)

        var t = j.find("files")
        if isinstance(t, list)
          return t
        end

        return nil
      except .. as e, m
        tasmota.log(string.format("CFG: exception '%s' - '%s'", e, m), 2)
        return nil
      end
    end

    # ####################################################################################################
    # Init web handlers
    # ####################################################################################################
    # Displays a "Autocong" button on the configuration page
    def web_add_config_button()
      import webserver
      webserver.content_send("<p><form id=ac action='ac' style='display: block;' method='get'><button>&#129668; Auto-configuration</button></form></p>")
    end


    # This HTTP GET manager controls which web controls are displayed
    def page_autoconf_mgr()
      import webserver
      import string
      if !webserver.check_privileged_access() return nil end

      webserver.content_start('Auto-configuration')
      webserver.content_send_style()
      webserver.content_send("<p><small>&nbsp;(This feature requires an internet connection)</small></p>")
      
      var cur_module = self.get_current_module_path()
      var cur_module_display = cur_module ? string.tr(self.get_current_module_name(), "_", " ") : self._error ? "&lt;Error: apply new or remove&gt;" : "&lt;None&gt;"

      webserver.content_send("<fieldset><style>.bdis{background:#888;}.bdis:hover{background:#888;}</style>")
      webserver.content_send(string.format("<legend><b title='Autoconfiguration'>&nbsp;Current auto-configuration</b></legend>"))
      webserver.content_send(string.format("<p>Current configuration: </p><p><b>%s</b></p>", cur_module_display))

      if cur_module
        # add button to reapply template
        webserver.content_send("<p><form id=reapply style='display: block;' action='/ac' method='post' ")
        webserver.content_send("onsubmit='return confirm(\"This will cause a restart.\");'>")
        webserver.content_send("<button name='reapply' class='button bgrn'>Re-apply current configuration</button>")
        webserver.content_send("</form></p>")
      end
      webserver.content_send("<p></p></fieldset><p></p>")

      webserver.content_send("<fieldset><style>.bdis{background:#888;}.bdis:hover{background:#888;}</style>")
      webserver.content_send(string.format("<legend><b title='New autoconf'>&nbsp;Select new auto-configuration</b></legend>"))

      webserver.content_send("<p><form id=zip style='display: block;' action='/ac' method='post' ")
      webserver.content_send("onsubmit='return confirm(\"This will change the current configuration and cause a restart.\");'>")
      webserver.content_send("<label>Choose a device configuration:</label><br>")
      webserver.content_send("<select name='zip'>")

      var templates = self.load_templates()
      webserver.content_send("<option value='reset'>&lt;Remove autoconf&gt;</option>")
      for t:templates
        webserver.content_send(string.format("<option value='%s'>%s</option>", t, string.tr(t, "_", " ")))
      end

      webserver.content_send("</select><p></p>")

      webserver.content_send("<button name='zipapply' class='button bgrn'>Apply configuration</button>")
      # webserver.content_send(string.format("<input name='ota' type='hidden' value='%d'>", ota_num))
      webserver.content_send("</form></p>")


      webserver.content_send("<p></p></fieldset><p></p>")
      webserver.content_button(webserver.BUTTON_CONFIGURATION)
      webserver.content_stop()
    end

    # ####################################################################################################
    # Web controller
    #
    # Applies the changes and restart
    # ####################################################################################################
    # This HTTP POST manager handles the submitted web form data
    def page_autoconf_ctl()
      import webserver
      import string
      import path
      if !webserver.check_privileged_access() return nil end

      try
        if webserver.has_arg("reapply")
          tasmota.log("CFG: removing first time marker", 2);
          # print("CFG: removing first time marker")
          self.clear_first_time()
          #- and force restart -#
          webserver.redirect("/?rst=")

        elif webserver.has_arg("zip")
          # remove any remaining autoconf file
          tasmota.log("CFG: removing autoconf files", 2);
          # print("CFG: removing autoconf files")
          self.delete_all_configs()

          # get the name of the configuration file
          var arch_name = webserver.arg("zip")

          if arch_name != "reset"
            var url = string.format("https://raw.githubusercontent.com/tasmota/autoconf/main/%s/%s.autoconf", tasmota.arch(), arch_name)
            tasmota.log(string.format("CFG: downloading '%s'", url), 2);

            var local_file = string.format("%s.autoconf", arch_name)

            # download file and write directly to file system
            var cl = webclient()
            cl.begin(url)
            var r = cl.GET()
            if r != 200  raise "connection_error", string.format("return code=%i", r) end
            cl.write_file(local_file)
            cl.close()
          end

          # remove marker to reapply template
          self.clear_first_time()

          #- and force restart -#
          webserver.redirect("/?rst=")
        else
          raise "value_error", "Unknown command"
        end
      except .. as e, m
        print(string.format("CFG: Exception> '%s' - %s", e, m))
        #- display error page -#
        webserver.content_start("Parameter error")           #- title of the web page -#
        webserver.content_send_style()                  #- send standard Tasmota styles -#

        webserver.content_send(string.format("<p style='width:340px;'><b>Exception:</b><br>'%s'<br>%s</p>", e, m))

        webserver.content_button(webserver.BUTTON_CONFIGURATION) #- button back to management page -#
        webserver.content_stop()                        #- end of web page -#
      end
    end

    # Add HTTP POST and GET handlers
    def web_add_handler()
      import webserver
      webserver.on('/ac', / -> self.page_autoconf_mgr(), webserver.HTTP_GET)
      webserver.on('/ac', / -> self.page_autoconf_ctl(), webserver.HTTP_POST)
    end
    

    # reset the configuration information (but don't restart)
    # i.e. remove any autoconf file
    def reset()
      import path
      import string

      var dir = path.listdir("/")
      var entry

      var i = 0
      while i < size(dir)
        var fname = dir[i]
        if string.find(fname, ".autoconf") > 0   # does the file contain '*.autoconf'
          path.remove(fname)
          print(string.format("CFG: removed file '%s'", fname))
        end
        i += 1
      end

      self._archive = nil
      self._error = nil
    end

    # called by the synthetic event `preinit`
    def preinit()
      if self._archive == nil  return end
      # try to launch `preinit.be`
      import path

      var fname = self._archive + '#preinit.be'
      if path.exists(fname)
        tasmota.log("CFG: loading "+fname, 3)
        load(fname)
        tasmota.log("CFG: loaded  "+fname, 3)
      end
    end

    def run_bat(fname)    # read a '*.bat' file and run each command
      import string
      var f
      try
        f = open(fname, "r")       # open file in read-only mode, it is expected to exist
        while true
          var line = f.readline()         # read each line, can contain a terminal '\n', empty if end of file
          if size(line) == 0 break end    # end of file

          if line[-1] == "\n"  line = line[0..-2] end  # remove any trailing '\n'
          if size(line) > 0
            tasmota.cmd(line)             # run the command
          end
        end
        f.close()                         # close, we don't expect exception with read-only, could be added later though
      except .. as e, m
        print(string.format('CFG: could not run %s (%s - %s)', fname, e, m))
        f.close()
      end
    end

    # called by the synthetic event `autoexec`
    def autoexec()
      if self._archive == nil  return end
      # try to launch `preinit.be`
      import path

      # Step 1. if first run, only apply `init.bat`
      var fname = self._archive + '#init.bat'
      if self.is_first_time() && path.exists(fname)
        # create the '.autoconf' file to avoid running it again, even if it crashed
        self.set_first_time()

        # if path.exists(fname)    # we know it exists from initial test
        self.run_bat(fname)
        tasmota.log("CFG: 'init.bat' done, restarting", 2)
        tasmota.cmd("Restart 1")
        return                    # if init was run, force a restart anyways and don't run the remaining code
        # end
      end

      # Step 2. if 'display.ini' is present, launch Universal Display
      fname = self._archive + '#display.ini'
      if gpio.pin_used(gpio.OPTION_A, 2) && path.exists(fname)
        if path.exists("display.ini")
          tasmota.log("CFG: skipping 'display.ini' because already present in file-system", 2)
        else
          import display
          var f = open(fname,"r")
          var desc = f.read()
          f.close()
          display.start(desc)
        end
      end

      # Step 3. if 'autoexec.bat' is present, run it
      fname = self._archive + '#autoexec.bat'
      if path.exists(fname)
        tasmota.log("CFG: running "+fname, 3)
        self.run_bat(fname)
        tasmota.log("CFG: ran  "+fname, 3)
      end
      
      # Step 4. if 'autoexec.be' is present, load it
      fname = self._archive + '#autoexec.be'
      if path.exists(fname)
        tasmota.log("CFG: loading "+fname, 3)
        load(fname)
        tasmota.log("CFG: loaded  "+fname, 3)
      end
    end
  end

  return Autoconf()    # return an instance of this class
end

aa = autoconf_module.init(autoconf_module)
import webserver
webserver.on('/ac2', / -> aa.page_autoconf_mgr(), webserver.HTTP_GET)
return autoconf_module
