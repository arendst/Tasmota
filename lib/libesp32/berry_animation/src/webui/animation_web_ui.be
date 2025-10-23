#
# berry_animation_webui.be - Web interface for Berry Animation Framework
#
# Provides a web-based DSL editor with live preview and code generation
# Integrates with existing Tasmota web infrastructure for memory efficiency
#
# Copyright (C) 2024  Tasmota Project
#

class AnimationWebUI
  var last_dsl_code
  var last_berry_code
  
  static var DEFAULT_DSL = 
    "# Simple Berry Animation Example - Cylon red eye\n"
    "\n"
    "set strip_len = strip_length()\n"
    "\n"
    "animation red_eye = beacon_animation(\n"
    "  color = red\n"
    "  pos = smooth(min_value = 0, max_value = strip_len - 2, duration = 5s)\n"
    "  beacon_size = 3       # small 3 pixels eye\n"
    "  slew_size = 2         # with 2 pixel shading around\n"
    ")\n"
    "\n"
    "run red_eye             # run the animation\n"

  def init()
    self.last_dsl_code = self.DEFAULT_DSL
    self.last_berry_code = ""
    
    # Add to main menu if not already present
    tasmota.add_driver(self)
    if tasmota.is_network_up()
      self.web_add_handler()      # if init is called after the network is up, `web_add_handler` event is not fired
    end
    
    log("LED: Berry Animation WebUI initialized", 3)
  end

  #####################################################################################################
  # Web handlers
  #####################################################################################################
  # Displays a "Extension Manager" button on the configuration page
  def web_add_button()
    import webserver
    webserver.content_send("<p></p><form id=but_part_mgr style='display: block;' action='berry_anim' method='get'><button>LED Animation Console</button></form><p></p>")
  end

  def handle_request()
    import webserver
    import animation_dsl
    
    # API requests (JSON responses)
    if webserver.has_arg("api")
      var api_type = webserver.arg("api")

      if   api_type == "action"
        # Action API (JSON response)
        webserver.content_open(200, "application/json")
        var result = {}
        
        if webserver.has_arg("action")
          var action = webserver.arg("action")
          
          if action == "compile" || action == "compile_only"
            if webserver.has_arg("dsl_code")
              self.last_dsl_code = webserver.arg("dsl_code")
              
              try
                # Compile DSL to Berry code
                self.last_berry_code = animation_dsl.compile(self.last_dsl_code)
                result["success"] = true
                result["berry_code"] = self.last_berry_code
                
                if action == "compile"
                  # Execute the animation
                  animation_dsl.execute(self.last_dsl_code)
                  result["message"] = "Animation compiled and started"
                else
                  result["message"] = "DSL compiled successfully"
                end
                
              except .. as e, msg
                result["success"] = false
                result["error"] = f"{e}: {msg}"
                self.last_berry_code = f"# Compilation failed\n# {result['error']}"
              end
            else
              result["success"] = false
              result["error"] = "No DSL code provided"
            end
            
          elif action == "stop"
            animation.init_strip()
              result["success"] = true
              result["message"] = "Animation stopped"
          else
            result["success"] = false
            result["error"] = f"Unknown action: {action}"
          end
        else
          result["success"] = false
          result["error"] = "No action specified"
        end
        
        import json
        webserver.content_send(json.dump(result))
        webserver.content_close()
      end
    else    
      # Default: serve main page (GET request)
      self.page_main()
    end
  end

  def page_main()
    import webserver
    webserver.content_start("Berry Animation Framework")
    webserver.content_send_style()
    
    # Add custom CSS for the animation editor
    webserver.content_send(
      "<style>"
      ".anim-container{min-width:350px;margin:0 auto;padding:10px;width:100%;max-width:none;}"
      "body > div{width:calc(100% - 20px) !important;max-width:1200px !important;display:block !important;box-sizing:border-box !important;}"
      ".anim-editor{width:100%;min-height:300px;font-family:monospace;font-size:12px;border:1px solid var(--c_frm);padding:8px;background:var(--c_intxt);color:#b19cd9;box-sizing:border-box;}"
      ".anim-output{width:100%;min-height:200px;font-family:monospace;font-size:11px;border:1px solid var(--c_frm);padding:8px;background:var(--c_intxt);color:#fb1;box-sizing:border-box;}"
      ".anim-error{color:var(--c_btnrst);background:#ffe6e6;padding:8px;border:1px solid var(--c_btnrst);margin:5px 0;}"
      ".anim-success{color:var(--c_btnsv);background:#e6ffe6;padding:8px;border:1px solid var(--c_btnsv);margin:5px 0;}"
      "button:disabled{opacity:0.5;cursor:not-allowed;}"
      ".textarea-container{position:relative;}"
      ".copy-btn{position:absolute;top:8px;right:0;width:20px;height:20px;cursor:pointer;user-select:none;transition:all 0.2s;background:transparent;border:none;}"
      ".anim-editor + .copy-btn::before,.anim-output + .copy-btn::before{content:'';position:absolute;top:2px;left:2px;width:10px;height:10px;border-left:2px solid var(--c_txt);border-top:2px solid var(--c_txt);background:transparent;}"
      ".anim-editor + .copy-btn::after,.anim-output + .copy-btn::after{content:'';position:absolute;top:6px;left:6px;width:10px;height:10px;border:2px solid var(--c_txt);}"
      ".copy-btn:hover::before,.copy-btn:hover::after{opacity:0.7;}"
      ".copy-message{position:absolute;top:35px;right:8px;background:var(--c_intxt);color:white;padding:4px 8px;border-radius:3px;font-size:11px;opacity:0;transition:opacity 0.3s;pointer-events:none;white-space:nowrap;}"
      ".copy-message.show{opacity:1;}"
      "</style>"
    )

    webserver.content_send(
      "<div class='anim-container'>"
      
      # DSL Editor
      "<h3>DSL Code Editor</h3>"
      "<div class='textarea-container'>"
      "<textarea id='dsl_code' class='anim-editor' spellcheck='false' placeholder='Enter your Berry Animation DSL code here...'>")
    webserver.content_send(self.last_dsl_code)    # we don't need to escape because it is a safe example
    webserver.content_send(
      "</textarea>"
      "<div class='copy-btn' onclick='copyDslCode()' title='Copy DSL code'></div>"
      "<div id='dsl-copy-msg' class='copy-message'></div>"
      "</div>"
      "<div id='status-message'><div class='anim-success'><strong>Status:</strong> Ready</div></div>"
      "<p></p>"
      "<button id='btn-compile' onclick='sendAction(\"compile\")' class='button bgrn'>Compile & Run</button><p></p>"
      "<button id='btn-compile-only' onclick='sendAction(\"compile_only\")' class='button'>Compile Only</button><p></p>"
      "<button id='btn-stop' onclick='sendAction(\"stop\")' class='button'>Stop Animation</button>"
    )

    # Generated Berry Code Display
    webserver.content_send(
      "<h3>Generated Berry Code</h3>"
      "<div class='textarea-container'>"
      "<textarea id='berry_output' class='anim-output' readonly>")
    webserver.content_send(webserver.html_escape(self.last_berry_code))
    webserver.content_send(
      "</textarea>"
      "<div class='copy-btn' onclick='copyBerryCode()' title='Copy Berry code'></div>"
      "<div id='berry-copy-msg' class='copy-message'></div>"
      "</div>"
    )

    # Add button at the end of the page
    webserver.content_button(webserver.BUTTON_MANAGEMENT)

    # Add JavaScript for AJAX
    webserver.content_send(
      "<script>"
      
      "function showStatus(message,isError){"
        "var statusDiv=eb('status-message');"
        "if(message){"
          "statusDiv.innerHTML='<div class=\"anim-'+(isError?'error':'success')+'\"><strong>'+(isError?'Error':'Success')+':</strong> '+message+'</div>';"
        "}else{"
          "statusDiv.innerHTML='';"
        "}"
      "}"
      
      "function showProcessingStatus(){"
        "var statusDiv=eb('status-message');"
        "statusDiv.innerHTML='<div class=\"anim-success\"><strong>Status:</strong> Processing...</div>';"
      "}"
      
      "function setButtonsDisabled(disabled){"
        "var btnIds=['btn-compile','btn-compile-only','btn-stop'];"
        "for(var i=0;i<btnIds.length;i++){"
          "var btn=eb(btnIds[i]);"
          "if(btn)btn.disabled=disabled;"
        "}"
      "}")
    webserver.content_send(
      "function sendAction(action){"
        "setButtonsDisabled(true);"
        "showProcessingStatus();"
        ""
        "var xhr=new XMLHttpRequest();"
        "var formData=new FormData();"
        "formData.append('action',action);"
        "if(action!=='stop'&&action!=='clear'){"
          "formData.append('dsl_code',eb('dsl_code').value);"
        "}"
        ""
        "xhr.open('POST','/berry_anim?api=action',true);"
        "xhr.onreadystatechange=function(){"
          "if(xhr.readyState===4){"
            "setButtonsDisabled(false);"
            "if(xhr.status===200){"
              "try{"
                "var result=JSON.parse(xhr.responseText);"
                "if(result.success){"
                  "showStatus(result.message,false);"
                  "if(result.berry_code!==undefined){"
                    "eb('berry_output').value=result.berry_code;"
                  "}"
                  "if(result.dsl_code!==undefined){"
                    "eb('dsl_code').value=result.dsl_code;"
                  "}"
                "}else{"
                  "showStatus(result.error,true);"
                  "if(result.error.includes('Compilation failed')){"
                    "eb('berry_output').value='# Compilation failed\\n# '+result.error;"
                  "}"
                "}"
              "}catch(e){"
                "showStatus('Invalid response from server',true);"
              "}"
            "}else{"
              "showStatus('Network error: '+xhr.status,true);"
            "}"
          "}"
        "};"
        "xhr.send(formData);"
      "}")
    webserver.content_send(
      "function showCopyMessage(msgId,text,isError){"
        "var msgDiv=eb(msgId);"
        "msgDiv.textContent=text;"
        "msgDiv.style.background='color-mix(in srgb, var('+(isError?'--c_btnrst':'--c_btnsv')+') 90%, transparent)';"
        "msgDiv.classList.add('show');"
        "setTimeout(function(){"
          "msgDiv.classList.remove('show');"
        "},2000);"
      "}"
      
      "function copyTextarea(textareaId,msgId){"
        "var textarea=eb(textareaId);"
        "textarea.select();"
        "textarea.setSelectionRange(0,99999);"
        "try{"
          "document.execCommand('copy');"
          "showCopyMessage(msgId,'Copied!',false);"
        "}catch(err){"
          "showCopyMessage(msgId,'Copy failed',true);"
        "}"
      "}"
      
      "function copyDslCode(){copyTextarea('dsl_code','dsl-copy-msg');}"
      "function copyBerryCode(){copyTextarea('berry_output','berry-copy-msg');}"
      
      "</script>"
    )

    webserver.content_stop()
  end


  # Add HTTP POST and GET handlers
  def web_add_handler()
    import webserver
    webserver.on("/berry_anim", / -> self.handle_request())
  end
  
  def deinit()
    # Cleanup when module is unloaded
    log("LED: Berry Animation WebUI deinitialized", 3)
  end
end

return {
  "animation_web_ui": AnimationWebUI
}
