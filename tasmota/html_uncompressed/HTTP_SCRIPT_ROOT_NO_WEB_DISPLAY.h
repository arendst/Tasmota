const char HTTP_SCRIPT_ROOT[] PROGMEM =
  "function la(p){"
    "var a='';"
    "if(la.arguments.length==1){"
      "a=p;"
      "clearTimeout(lt);"
    "}"
    "if(x!=null){x.abort();}"             // Abort if no response within 2 seconds (happens on restart 1)
    "x=new XMLHttpRequest();"
    "x.onreadystatechange=function(){"
      "if(x.readyState==4&&x.status==200){"
        "var s=x.responseText.replace(/{t}/g,\"<table style='width:100%%'>\")"
                            ".replace(/{s}/g,\"<tr><th>\")"
//                            ".replace(/{m}/g,\"</th><td>\")"
                            ".replace(/{m}/g,\"</th><td style='width:20px;white-space:nowrap'>\")"  // I want a right justified column with left justified text
                            ".replace(/{e}/g,\"</td></tr>\")"
                            ".replace(/{c}/g,\"%%'><div style='text-align:center;font-weight:\");"
        "eb('l1').innerHTML=s;"
      "}"
    "};"
    "x.open('GET','.?m=1'+a,true);"       // ?m related to Webserver->hasArg("m")
    "x.send();"
    "lt=setTimeout(la,%d);"               // Settings.web_refresh
  "}";
