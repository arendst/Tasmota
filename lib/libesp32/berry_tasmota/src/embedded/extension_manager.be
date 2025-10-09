#  extensions manager module for Berry
#
#

var extension_manager = module("extension_manager")

#@ solidify:extension_manager
class Extension_manager
  static var EXT_FOLDER = "/.extensions/"
  static var EXT_REPO = "https://ota.tasmota.com/extensions/"
  static var EXT_REPO_MANIFEST = "extensions.jsonl"
  static var EXT_REPO_FOLDER = "tapp/"
  var ext_repo

  #####################################################################################################
  # init - constructor
  #
  # Register as driver
  #
  def init()
    self.ext_repo = ""
    tasmota.add_driver(self)
  end

  #####################################################################################################
  # General static helper functions
  #####################################################################################################
  #####################################################################################################
  # version_string(v)
  #
  # Convert 32 bits version to "a.b.c.d" where version is 0xAABBCCDD
  #
  # @param v: int - version in format 0xAABBCCDD
  # @return string - string in format "a.b.c.d"
  #####################################################################################################
  static def version_string(v)
    return f"v{(v >> 24) & 0xFF}.{(v >> 16) & 0xFF}.{(v >> 8) & 0xFF}.{v & 0xFF}"
  end

  #####################################################################################################
  # tapp_name(wd)
  #
  # Takes a working directory 'wd' and extract the name of the tapp file.
  # Ex: '/.extensions/Leds_Panel.tapp#' becomes 'Leds_Panel'
  #
  # @param wv: string - the Working Dir of the tapp file like '/.extensions/Leds_Panel.tapp#'
  # @return string - the raw name of the tapp file, like 'Leds_Panel'
  #####################################################################################################
  static def tapp_name(wd)
    import string

    var from = 0
    var to = size(wd) - 1
    # if ends with '#' or '_' remove last char
    if (wd[to] == "#") || (wd[to] == "_")
      to -= 1
    end
    # remove suffix .tapp
    if string.find(wd, ".tapp", to + 1 - size(".tapp")) >= 0
      to -= size(".tapp")
    end
    # remove anything before the last '/'
    var idx
    while (idx := string.find(wd, '/', from)) >= 0
      from = idx + 1
    end
    # final result
    return wd[from .. to]
  end

  #####################################################################################################
  # General functions to get information about tapp files (installed or from store)
  #####################################################################################################
  #####################################################################################################
  # manifest_decode()
  #
  # Decode the manifest entry from JSONL (single line) into a validated format
  # or return 'nil' if something major went wrong (and logged)
  #
  # @param json_line: string - single JSONL entry, like '{"name":"Leds Panel","file":"Leds_Panel.tapp","version":"0x19090100","description":"[...]","author":"[...]"}
  # @return map or nil: normalized map or 'nil' if something went wrong
  #                     guaranteed fileds: 'name', 'description', 'file', 'version', 'author'
  static def manifest_decode(json_line)
    import json

    # try to parse json
    var entry = json.load(json_line)
    if (entry == nil)
      log(f"EXT: unable to parse manifest line '{json_line}'", 3)
      return nil
    end

    # check for mandatory fields
    if !entry.contains('name') || !entry.contains('file') || !entry.contains('version')
      log(f"EXT: manifest is missing 'name/file/version' in map '{entry}'")
      return nil
    end

    # build result with mandatory fields
    var result = {
      'name': entry['name'],
      'file': entry['file'],
      'version': int(entry['version'])
    }
    # add non-critical fields
    result['description'] = entry.find('description', "[no description]")
    result['author'] = entry.find('author', "")
    return result
  end

  #####################################################################################################
  # list_installed_ext()
  #
  # Return a map of installed tap files, by tapp file name
  # tapp_name -> path {'Leds_Panel.tapp': '/.extensions/Leds_Panel.tapp'}
  #
  # Example:
  # > extension_manager.list_installed_ext()
  # {'Leds_Panel': '/.extensions/Leds_Panel.tapp', 'Partition_Wizard': '/.extensions/Partition_Wizard.tapp_'}
  #
  # @return map: tapp_namt -> full path (or wd)
  static def list_installed_ext()
    # Read extensions in file system
    var installed_ext = {}

    for ext_path: _class.list_extensions()
      installed_ext[_class.tapp_name(ext_path)] = ext_path
    end
    # log(f"EXT: list_installed_ext={installed_ext}")
    return installed_ext
  end

  #####################################################################################################
  # list_extensions_in_fs()
  #
  # List all extensions in file-system, whether they are running or not
  #
  # Example:
  # > extension_manager.list_extensions_in_fs()
  # {'Leds Panel': '/.extensions/Leds_Panel.tapp', 'Partition Wizard': '/.extensions/Partition_Wizard.tapp_'}
  #
  # @return sortedmap: with Name of App as key, and following map:
  #   name, description, version (int), autorun (bool)
  static def list_extensions_in_fs()
    import string
    var sm = sortedmap()
    for ext: _class.list_extensions()
      var details = tasmota.read_extension_manifest(ext)
      if (details != nil)
        var name = details.find("name")
        if (name)
          sm[name] = ext
        end
      else
        log(f"EXT: unable to read details from '{ext}'", 3)
      end
    end
    return sm
  end

  #####################################################################################################
  # list_extensions()

  # Returns the list of enabled and disabled extensions
  # i.e. scan files in ".extensions" folder and keep files ending with ".tapp" and ".tapp_"
  #
  # @return list: list of extensions by path or by instance
  static def list_extensions()
    import path
    import string
    var l = []
    # read from fs
    for d: path.listdir(_class.EXT_FOLDER)
      if string.endswith(d, ".tapp") || string.endswith(d, ".tapp_")
        l.push(_class.EXT_FOLDER + d)
      end
    end
    # finish
    return l
  end

  #####################################################################################################
  # Methods to install / upgrade / delete extensions from the store
  #####################################################################################################
  #####################################################################################################
  # check_or_create_dir()
  #
  # Check that directory exists or create it
  static def check_or_create_dir(ext_folder)
    import path
    if path.exists(ext_folder)
      if path.isdir(ext_folder)
        return      # all good, already exists
      else
        path.remove(ext_folder)    # it's a file, remove it
      end
    end
    # `/.extensions/` does not exist
    path.mkdir(ext_folder)
    # final check
    if !path.exists(ext_folder) || !path.isdir(ext_folder)
      raise "io_error", f"cannot create folder '{ext_folder}'"
    end
    # all good, created successfully
  end

  #####################################################################################################
  # run_stop_ext(tapp_fname, run_stop)
  #
  # @param tapp_fname : string - name of tapp file to install from repository (ex: "Leds_Panel")
  # @param run_stop : bool - `true` to run , `false` to stop
  # @return bool - `true` if success
  static def run_stop_ext(tapp_fname, run_stop)
    # sanitize
    tapp_fname = _class.tapp_name(tapp_fname)

    # get the path for actual file
    var tapp_path = _class.list_installed_ext().find(tapp_fname)
    if tapp_path != nil
      if run_stop
        return tasmota.load(tapp_path)
      else
        return tasmota.unload_extension(tapp_path)
      end
    else
      return false
    end
  end

  #####################################################################################################
  # enable_disable_ext(tapp_fname, run_stop)
  #
  # @param tapp_fname : string - name of tapp file to enable or disable (ex: "Leds_Panel")
  # @param enable : bool - `true` to enable , `false` to disable
  # @return bool - `true` if success
  static def enable_disable_ext(tapp_fname, enable)
    import string
    # sanitize
    tapp_fname = _class.tapp_name(tapp_fname)

    # get the path for actual file
    var tapp_path = _class.list_installed_ext().find(tapp_fname)
    if tapp_path != nil

      var new_name
      if   enable && string.endswith(tapp_path, ".tapp_")
        new_name = tapp_path[0..-2]     # remove trailing '_'
      elif !enable && string.endswith(tapp_path, ".tapp")
        new_name = tapp_path + '_'      # add trailing '_'
      end

      if new_name
        import path
        var success = path.rename(tapp_path, new_name)
        if (success)                  # update any running extension with its new name
          if (tasmota._ext != nil) && tasmota._ext.contains(tapp_path)
            tasmota._ext[new_name] = tasmota._ext[tapp_path]
            tasmota._ext.remove(tapp_path)
          end
        end
        return success
      end
    end
    return false
  end

  #####################################################################################################
  # delete_ext(tapp_fname)
  #
  # @param tapp_fname : string - name of tapp file to delete from file system (ex: "Leds_Panel")
  # @return bool - `true` if success
  static def delete_ext(tapp_fname)
    # sanitize
    tapp_fname = _class.tapp_name(tapp_fname)

    # get the path for actual file
    var tapp_path = _class.list_installed_ext().find(tapp_fname)
    if tapp_path != nil
      import path
      _class.run_stop_ext(tapp_fname, false)      # stop the extension if it's running
      var success = path.remove(tapp_path)
      return success
    else
      return false
    end
  end

  #####################################################################################################
  # install_from_store(tapp_fname)
  #
  # @param tapp_fname : string - name of tapp file to install from repository
  # @return bool : 'true' if success
  def install_from_store(tapp_fname)
    import string
    import path
    # sanitize
    tapp_fname = self.tapp_name(tapp_fname) + ".tapp"
    # full url
    var ext_url = f"{self.ext_repo}{self.EXT_REPO_FOLDER}{tapp_fname}"
    log(f"EXT: installing from '{ext_url}'", 3)
    # load from web
    try
      # check if directory exists
      self.check_or_create_dir(self.EXT_FOLDER)   # raises an exception if failed

      var local_file = f"{self.EXT_FOLDER}{tapp_fname}"
      var cl = webclient()
      cl.begin(ext_url)
      var r = cl.GET()
      if r != 200
        log(f"EXT: return_code={r}", 2)
        return false
      end
      var ret = cl.write_file(local_file)
      cl.close()
      # test if file exists and tell its size
      if ret > 0 && path.exists(local_file)        
        log(f"EXT: successfully installed '{local_file}' {ret} bytes", 3)
        return true
      else
        raise "io_error", f"could not download into '{local_file}' ret={ret}"
      end
    except .. as e, m
      log(format("EXT: exception '%s' - '%s'", e, m), 2)
      return false
    end
  end

  #####################################################################################################
  # Web handlers
  #####################################################################################################
  # Displays a "Extension Manager" button on the configuration page
  def web_add_button()
    import webserver
    webserver.content_send("<p></p><form id=but_part_mgr style='display: block;' action='ext' method='get'><button>Extension Manager</button></form><p></p>")
  end

  #####################################################################################################
  # This HTTP GET manager controls which web controls are displayed
  #####################################################################################################
  def page_extensions_mgr_dispatcher()
    import webserver
    if !webserver.check_privileged_access() return nil end

    if (webserver.has_arg("store"))
      return self.page_extensions_store()
    else
      return self.page_extensions_mgr()
    end
  end

  #####################################################################################################
  # This HTTP GET manager controls which web controls are displayed
  #####################################################################################################
  def page_extensions_mgr()
    import webserver
    import string

    webserver.content_start('Extensions Manager')
    webserver.content_send_style()
    # webserver.content_send("<p><small>&nbsp;(This feature requires an internet connection)</small></p>")
    
    webserver.content_send("<div style='padding:0px 5px;text-align:center;'><h3><hr>Extension Manager<hr></h3></div>")

    webserver.content_send("<script>"
      "function loadext() {"
        "eb('store').disabled=true;"
        "x=new XMLHttpRequest();"
        "x.timeout=4000;"
        "x.onreadystatechange = () => {"
          "if(x.readyState==4){"
            "if(x.status==200){"
              "eb('inet').style.display='none';"
              "eb('store').outerHTML=x.responseText;"
            "}"
          "}"
        "};"
        "x.open('GET','?store=');"
        "x.send();"
      "}"
      "window.onload=function(){"
        "loadext();"
      "};"

      # scripts for Store
      "function toggleDesc(id) {"
        "var desc = document.getElementById('desc-' + id);"
        "var arrow = document.getElementById('arrow-' + id);"
        "if (desc.style.display === 'none' || desc.style.display === '') {"
          "desc.style.display = 'block';"
          "arrow.innerHTML = '▼';"
        "} else {"
          "desc.style.display = 'none';"
          "arrow.innerHTML = '▶';"
        "}"
      "}"
      # Simple filtering functions (optional enhancement)
      "function filterExtensions(query) {"
        "var items = document.getElementsByClassName('ext-store-item');"
        "query = query.toLowerCase();"
        "for (var i = 0; i < items.length; i++) {"
          "var name = items[i].getElementsByClassName('ext-name')[0].textContent.toLowerCase();"
          "var desc = items[i].getElementsByClassName('ext-desc')[0].textContent.toLowerCase();"
          "if (name.includes(query) || desc.includes(query)) {"
            "items[i].style.display = 'block';"
          "} else {"
            "items[i].style.display = 'none';"
          "}"
        "}"
      "}"
      "</script>"
    )

    webserver.content_send(
                          "<fieldset style='padding:0 5px;'>"
                          "<style>"
                          # Fix for small text - the key is width: min-content on parent */
                          ".ext-item{width:min-content;min-width:100%;}"
                          ".ext-item small{display:block;word-wrap:break-word;overflow-wrap:break-word;white-space:normal;padding-right:5px;padding-top:2px;}"
                          # Control bar styles
                          ".ext-controls{display:flex;gap:8px;align-items:center;margin-top:4px;padding:0px}"
                          # Small action buttons
                          ".btn-small{padding:0 6px;line-height:1.8rem;font-size:0.9rem;min-width:auto;width:auto;flex-shrink:0;}"
                          # form
                          "form{padding-top:0px;padding-bottom:0px;}"
                          # Running indicator
                          ".running-indicator{display:inline-block;width:8px;height:8px;border-radius:50%;margin-right:8px;background:var(--c_btnsvhvr);animation:pulse 1.5s infinite;}"
                          "@keyframes pulse{0%{opacity:1;}50%{opacity:0.5;}100%{opacity:1;}}"

                           # for store
                          # /* Extension Store specific styles */
                          ".store-header{display:flex;justify-content:space-between;align-items:center;margin-bottom:10px;}"
                          ".store-stats{font-size:0.9em;color:var(--c_in);}"
                          ".ext-store-item{background:var(--c_bg);border-radius:0.3em;margin-bottom:5px;padding:4px;}"
                          ".ext-header{display:flex;justify-content:space-between;align-items:center;cursor:pointer;user-select:none;padding:5px;}"
                          ".ext-title{display:flex;align-items:center;gap:6px;flex:1;padding:0;}"
                          ".ext-name{font-weight:bold;}"
                          ".ext-version{font-size:0.8em;}"
                          ".ext-arrow{color:var(--c_in);font-size:0.8em;}"
                          ".ext-badges{padding:0;}"
                          # ".ext-badges{margin-left:auto;gap:8px;align-items:center;}"
                          ".ext-details{width:min-content;min-width:100%;padding:0;display:none;}"
                          ".ext-desc{color:var(--c_in);font-size:0.8em;line-height:1.4;display:block;word-wrap:break-word;overflow-wrap:break-word;white-space:normal;padding:0 5px;}"

                          ".ext-actions{display:flex;gap:8px;padding:5px;}"
                          ".btn-action{padding:0 12px;line-height:1.8em;font-size:0.9em;flex:1;}"
                          ".installed-badge{border-color:var(--c_btnhvr);padding:0px 4px;border-radius:4px;font-size:0.7em;border-width:2px;border-style:solid;margin-right:3px;}"
                          # ".installed-badge{background:var(--c_btnsv);padding:2px 6px;border-radius:3px;font-size:0.7em;}"
                          ".update-badge{background:var(--c_btnhvr);padding:2px 6px;border-radius:4px;font-size:0.7em;margin-right:3px;animation:pulse 2s infinite;}"
                          "@keyframes pulse{0%{opacity:1;}50%{opacity:0.7;}100%{opacity:1;}}"
                          # ".category-filter{display:flex;gap:5px;margin-bottom:15px;overflow-x:auto;padding:5px 0;}"
                          # ".cat-btn{padding:5px 12px;background:#3a3a3a;border:1px solid var(--c_frm);border-radius:15px;color:#aaa;font-size:0.9em;white-space:nowrap;cursor:pointer;transition:all 0.2s;}"
                          # ".cat-btn:hover{background:#4a4a4a;color:var(--c_btntxt);}"
                          # ".cat-btn.active{background:#1fa3ec;color:var(--c_btntxt);border-color:var(--c_btn);}"
                          # ".search-box{width:100%;box-sizing:border-box;padding:8px;border:0;color:var(--c_txt);margin-bottom:10px;border-radius:0.3em;}"
                          # ".search-box::placeholder{color:var(--c_frm);}"

                           "</style>"
                           "<legend><b title='Running extensions'>&nbsp;Installed extensions</b></legend>")
    var installed_ext = self.list_extensions_in_fs(true)
    if size(installed_ext) > 0
      var ext_nb = 0
      while ext_nb < size(installed_ext)
        if (ext_nb > 0)       webserver.content_send("<hr style='margin:2px 0 0 0;'>")   end
        var ext_path = installed_ext.get_by_index(ext_nb)   # ex: '/.extensions/Partition_Wizard.tapp'
        var tapp_name = self.tapp_name(ext_path)
        var tapp_name_html = webserver.html_escape(tapp_name)
        var details = tasmota.read_extension_manifest(ext_path)
        var installed_version = int(details.find('version', 0))
        var running = tasmota._ext ? tasmota._ext.contains(ext_path) : false
        var running_indicator = running ? " <span class='running-indicator' title='Running'></span>" : ""
        var autorun = details.find("autorun", false)
        var back_green = "style='background:var(--c_btnsvhvr);'"
        var dark_blue = "style='background:var(--c_btnoff);'"
        webserver.content_send("<div class='ext-item'>")
        webserver.content_send(f"<span title='path: {tapp_name_html}'><b>{webserver.html_escape(details['name'])}</b>{running_indicator}</span><br>")
        webserver.content_send(f"<small>{webserver.html_escape(details['description'])}</small>")
        if (installed_version > 0)
          webserver.content_send(f"<small>{self.version_string(installed_version)}</small>")
        end

        webserver.content_send("<div class='ext-controls' style='padding-top:0px;padding-bottom:0px;'>")
        webserver.content_send("<form action='/ext' method='post' class='ext-controls'>")
        webserver.content_send(f"<button type='submit' class='btn-small' {running ? back_green :: dark_blue} name='{running ? 's' :: 'r'}{tapp_name_html}'>{running ? 'Running' :: 'Stopped'}</button>")
        webserver.content_send(f"<button type='submit' class='btn-small' {autorun ? '' :: dark_blue} name='{autorun ? 'a' :: 'A'}{tapp_name_html}'>Auto-run: {autorun ? 'ON' :: 'OFF'}</button>")
        webserver.content_send(f"<button type='submit' class='btn-small bred' name='d{tapp_name_html}' onclick='return confirm(\"Confirm deletion of {tapp_name_html}.tapp\")'>Uninstall</button>")
        webserver.content_send("</form></div></div>")

        ext_nb += 1
      end
    else
      # no installed extensions
      webserver.content_send("<div><small><i>No installed extension.</i></small></p>")
    end

    webserver.content_send("<p></p></fieldset><p></p>")

    webserver.content_send("<div style='padding:0px 5px;text-align:center;'><h3><hr>Online Store"
                           "<hr style='margin-bottom:0;'>"
                           "<span id='inet' style='font-size:small;font-weight:normal;''>&nbsp;(This feature requires an internet connection)</span>"
                           "</h3></div>")

    webserver.content_send("<b id='store'>[ <span style='color:var(--c_btnsv);'>Loading from Store...</span> ]</b>")

    webserver.content_button(webserver.BUTTON_MANAGEMENT) #- button back to management page -#
    webserver.content_stop()
  end

  #####################################################################################################
  # Extension Store
  #####################################################################################################
  def page_extensions_store()
    import webserver
    import string
    import json

    webserver.content_open(200, "text/html")
    # read manifest from ota.tasmota.com
    var item_jsonl
    try
      item_jsonl = self.load_manifest()
    except .. as e, m
      webserver.content_send("<b id='store'>[ <span style='color:var(--c_btnrst);'>Error loading manifest.</span> ]</b>")
      webserver.content_send(f"<p><small>{webserver.html_escape(m)}</small></p>")
      webserver.content_close()
      return
    end
    var item_json_count = string.count(item_jsonl, '"name":')

    webserver.content_send("<fieldset id='store'>")
                            
    webserver.content_send(f"<div class='store-header'>"
                              "<span>Browse Extensions</span>"
                              "<span class='store-stats'>{item_json_count} available</span>"
                            "</div>")
                            
    webserver.content_send("<input type='text' placeholder='Search extensions...' onkeyup='filterExtensions(this.value)'>"
                            "<p></p>")
    
    # Read extensions in file system
    # as a map tapp_name -> wd_path, ex {'Leds_Panel.tapp': '/.extensions/Leds_Panel.tapp'}
    var installed_ext = self.list_installed_ext()

    # Now parse application manifests
    var item_idx = 1
    var json_pos = 0       # starting char to parse JSONL
    while (json_pos < size(item_jsonl))   # item_idx negative means that we have nothing more to display
      var lf_pos = string.find(item_jsonl, "\n", json_pos)
      if (lf_pos < 0)   lf_pos = size(item_jsonl)   end       # go to end of string
      # extract the json from the line
      var json_line = item_jsonl[json_pos .. lf_pos]

      # ex: {"name":"Leds Panel","file":"Leds_Panel.tapp","version":"0x19090100","description":"Real-time display of WS2812 LEDs in browser with smooth animations and pattern editor.","author":"Stephan Hadinger"}
      var entry = self.manifest_decode(json_line)

      if (entry != nil)
        # entry is guaranteed to have the following fields: 'name', 'description', 'file', 'version', 'author'
        var app_version = entry['version']
        var app_version_web = self.version_string(app_version)
        var app_name_web = webserver.html_escape(entry['name'])
        var app_file = entry['file']
        var app_description_web = string.replace(webserver.html_escape(entry['description']), '\\n', '<br>')
        var app_author = entry['author']

        # now compute the status
        var installed = false
        var installed_version
        var installed_tapp_name
        installed_tapp_name = self.tapp_name(entry['file'])
        var installed_tapp_name_web = webserver.html_escape(installed_tapp_name)
        installed = installed_ext.contains(installed_tapp_name)
        if installed
          var installed_path = installed_ext[installed_tapp_name]
          var details = tasmota.read_extension_manifest(installed_path)
          installed_version = int(details.find('version', 0))
        end
        # We have 3 options:
        # - 'installed == false', only button "Install"
        # - 'installed' and 'installed_version < app_version', buttons "Upgrade" and "Delete"
        # - else 'installed' and version more recent, 1 button "Delete"
        var upgrade = installed && (installed_version < app_version)

        webserver.content_send(f"<div class='ext-store-item'>"
                                  "<div class='ext-header' onclick='toggleDesc(\"{item_idx}\")'>"
                                    "<div class='ext-title'>"
                                      "<span class='ext-name'>{app_name_web}</span>"
                                      "<span class='ext-version'><small>{self.version_string(app_version)}</small></span>"
                                    "</div>")
        if upgrade
          webserver.content_send(   "<div class='ext-badges'>"
                                      "<span class='update-badge'>Upgrade</span>"
                                    "</div>")
        elif installed
          webserver.content_send(   "<div class='ext-badges'>"
                                      "<span class='installed-badge'>Installed</span>"
                                    "</div>")
        end
        webserver.content_send(    f"<span id='arrow-{item_idx}' class='ext-arrow'>▶</span>"
                                  "</div>"
                                  "<div id='desc-{item_idx}' class='ext-details'>"
                                    "<div class='ext-desc'>"
                                      "{app_description_web}")
        if upgrade
          webserver.content_send(     f"<br>{self.version_string(installed_version)} → {app_version_web}")
        end
        webserver.content_send(     "</div>"
                                    "<form action='/ext' method='post' class='ext-actions'>"
                                      "<div style='width:30%'></div>")
        if installed
          if upgrade
            webserver.content_send(    f"<button type='submit' class='btn-action' name='u{installed_tapp_name_web}' onclick='return confirm(\"Confirm upgrade of {installed_tapp_name_web}\")'>Upgrade</button>")
          else
            webserver.content_send(     "<button type='submit' class='btn-action' style='visibility:hidden;'></button>")
          end
          webserver.content_send(    f"<button type='submit' class='btn-action bred' name='d{installed_tapp_name_web}' onclick='return confirm(\"Confirm deletion of {installed_tapp_name_web}\")'>Uninstall</button>")
        else
          webserver.content_send(    f"<button type='submit' class='btn-action' name='i{installed_tapp_name_web}' onclick='return confirm(\"Confirm installation of {app_name_web}\")'>Install</button>"
                                      "<button type='submit' class='btn-action bgrn' name='I{installed_tapp_name_web}' onclick='return confirm(\"Confirm installation of {app_name_web}\")'>Install+Run</button>")
        end
        webserver.content_send(     "</form>" 
                                  "</div>"
                                "</div>")

        item_idx += 1
      end

      json_pos = lf_pos + 1
    end

    webserver.content_send("<p></p>"
                           "<hr style='margin:2px 0 0 0;'>"
                           "<p></p>")
    webserver.content_send(f"<form action='/ext' method='post'>"
                            "<input type='text' id='x' name='x' placeholder='{self.ext_repo}'>"
                            "</form>")

    webserver.content_send("<p></p></fieldset><p></p>")
    webserver.content_close()
  end
    
  #####################################################################################################
  # Load manifest from ota.tasmota.com
  #####################################################################################################
  def load_manifest()
    try 
      import string

      var arch = tasmota.arch()         # architecture, ex: "esp32" - not used currently but might be useful
      var version = f"0x{tasmota.version():08X}"

      if !self.ext_repo
        var ota_url = tasmota.cmd("OtaUrl", true)['OtaUrl']
        var url_parts = string.split(ota_url, "/")
        self.ext_repo = f"{url_parts[0]}//{url_parts[2]}/extensions/" # http://otaserver/extensions/
      end
      var url = f"{self.ext_repo}{self.EXT_REPO_MANIFEST}?a={arch}&v={version}"
      log(f"EXT: fetching extensions manifest '{url}'", 3)
      # Add architeture and version information
      # They are not used for now but may be interesting in the future to serve
      # different content based on architecture (Ex: ESP32) and version (ex: 0x0E060001 for 14.6.0.1)
      # load extensions manifest
      var cl = webclient()
      cl.begin(url)
      var r = cl.GET()
      if r != 200
        if self.EXT_REPO != self.ext_repo
          if cl.get_size() < 0  # Happens on https://github.com/extensions/extensions.jsonl where 404 page is a lot of data
            cl.deinit()
            cl = webclient()
          else  
            cl.close()          # Fails to close if cl.get_size() < 0
          end
          self.ext_repo = self.EXT_REPO
          url = f"{self.ext_repo}{self.EXT_REPO_MANIFEST}?a={arch}&v={version}"
          log(f"EXT: fetching extensions manifest '{url}'", 3)
          cl.begin(url)
          r = cl.GET()
        end
        if r != 200
          log(f"EXT: error fetching manifest {r}", 2)
          raise "webclient_error", f"Error fetching manifest code={r}"
        end
      end
      var s = cl.get_string()
      cl.close()
      return s
    except .. as e, m
      log(format("EXT: exception '%s' - '%s'", e, m), 2)
      raise e, m
    end
  end
  
  #####################################################################################################
  # Web controller
  #
  # Applies the changes and restart
  #####################################################################################################
  # This HTTP POST manager handles the submitted web form data
  def page_extensions_ctl()
    import webserver
    import path
    import string
    if !webserver.check_privileged_access() return nil end

    try
      # log(f">>> {webserver.arg_name(0)=} {webserver.arg(0)=} {webserver.arg_size()=}")

      var btn_name = webserver.arg_name(0)
      var action = btn_name[0]          # first character
      var action_path = btn_name[1..]   # remove first character

      if (action == "r") || (action == "s")               # button "Run" or "Stop"
        self.run_stop_ext(action_path, action == "r")
      elif (action == "a") || (action == "A")             # button "Autorun", "A" enable, "a" disable
        self.enable_disable_ext(action_path, action == "A")
      elif (action == 'd')              # button "Delete"
        self.delete_ext(action_path)
      
      # Now try the store commands
      elif (action == 'u')              # Upgrade ext
        # first stop the app if it's running
        self.run_stop_ext(action_path, false)   # stop the extension
        var success = self.install_from_store(self.tapp_name(action_path))
      elif (action == 'i') || (action == 'I')  # Install ext ('I' for run as well)
        var success = self.install_from_store(self.tapp_name(action_path))
        if success
          if (action == 'I')            # run
            self.run_stop_ext(action_path, true)
          else                          # disable
            self.enable_disable_ext(action_path, false)
          end
        end

      # And finally try the provided repository website
      elif (action == 'x')              # User input repository website
        var url = webserver.arg(0)
        self.ext_repo = ""              # Use OtaUrl or default
        if size(url) > 0                # Input validation
          if url == "0"                 # Reset to use OtaUrl or default
          elif url == "1"               # Default repository
            self.ext_repo = self.EXT_REPO
          else                          # Process user input
            var url_parts = string.split(url, "/")
            if url_parts.size() > 2     # http: / / server / extensions
              var is_httpx = url_parts[0] == "http:" || url_parts[0] == "https:"
              var is_delim = url[-1] == '/'
              var is_extensions = url_parts[(is_delim)?-2:-1] == "extensions"
              if is_httpx && url_parts[1] == "" && is_extensions
                self.ext_repo = url
                if !is_delim
                  self.ext_repo += '/'
                end
              end
            end
          end
        end

      else
        log(f"EXT: wrong action '{btn_name}'", 3)
      end

      webserver.redirect(f"/ext")
    except .. as e, m
      log(f"EXT: Exception> '{e}' - {m}", 2)
      #- display error page -#
      webserver.content_start("Parameter error")      #- title of the web page -#
      webserver.content_send_style()                  #- send standard Tasmota styles -#

      webserver.content_send(f"<p style='width:340px;'><b>Exception:</b><br>'{webserver.html_escape(e)}'<br>{webserver.html_escape(m)}</p>")

      webserver.content_button(webserver.BUTTON_MANAGEMENT) #- button back to management page -#
      webserver.content_stop()                        #- end of web page -#
    end
  end

  # Add HTTP POST and GET handlers
  def web_add_handler()
    import webserver
    webserver.on('/ext', / -> self.page_extensions_mgr_dispatcher(), webserver.HTTP_GET)
    webserver.on('/ext', / -> self.page_extensions_ctl(), webserver.HTTP_POST)
  end
end

extension_manager.Extension_manager = Extension_manager
extension_manager.init = def (m)
  return m.Extension_manager()    # return an instance of this class
end
