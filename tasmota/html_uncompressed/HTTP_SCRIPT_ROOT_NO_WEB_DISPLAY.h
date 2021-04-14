const char HTTP_SCRIPT_ROOT[] PROGMEM =
  "var ft;"
  "function la(p){"
    "a=p||'';"
    "clearTimeout(ft);clearTimeout(lt);"
    "if(x!=null){x.abort()}"             // Abort if no response within 2 seconds (happens on restart 1)
    "x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(x.readyState==4&&x.status==200){"
        "var s=x.responseText.replace(/{t}/g,\"<table style='width:100%%'>\")"
                            ".replace(/{s}/g,\"<tr><th>\")"
//                            ".replace(/{m}/g,\"</th><td>\")"
                            ".replace(/{m}/g,\"</th><td style='width:20px;white-space:nowrap'>\")"  // I want a right justified column with left justified text
                            ".replace(/{e}/g,\"</td></tr>\");"
        "eb('l1').innerHTML=s;"
        "clearTimeout(ft);clearTimeout(lt);"
        "lt=setTimeout(la,%d);"               // Settings.web_refresh
      "}"
    "};"
    "x.open('GET','.?m=1'+a,true);"       // ?m related to Webserver->hasArg("m")
    "x.send();"
    "ft=setTimeout(la,20000);"               // 20s failure timeout
  "}";
