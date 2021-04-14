const char HTTP_SCRIPT_ROOT[] PROGMEM =
  "function la(p){"
    "if(typeof(EventSource)!==\"undefined\"){"
      "var e=new EventSource('?m=1');"
      "e.onmessage=function(event){"
        "eb('l1').innerHTML=event.data.replace(/{t}/g,\"<table style='width:100%%'>\")"
                                     ".replace(/{s}/g,\"<tr><th>\")"
    //                                 ".replace(/{m}/g,\"</th><td>\")"
                                     ".replace(/{m}/g,\"</th><td style='width:20px;white-space:nowrap'>\")"  // I want a right justified column with left justified text
                                     ".replace(/{e}/g,\"</td></tr>\");"
      "}"
    "}"
    "a=p||'';"
    "clearTimeout(lt);"
    "if(x!=null){x.abort()}"             // Abort if no response within 2 seconds (happens on restart 1)
    "x=new XMLHttpRequest();"
    "x.open('GET','.?m=1'+a,true);"      // ?m related to Webserver->hasArg("m")
    "x.send();"
    "lt=setTimeout(la,20000);"           // 20s failure timeout
  "}";
