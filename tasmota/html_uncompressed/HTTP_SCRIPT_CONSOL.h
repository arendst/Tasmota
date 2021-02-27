const char HTTP_SCRIPT_CONSOL[] PROGMEM =
  "var sn=0,id=0,ft,ltm=%d;"                      // Scroll position, Get most of weblog initially
  "function l(p){"                        // Console log and command service
    "var c,o='';"
    "clearTimeout(lt);"
    "clearTimeout(ft);"
    "t=eb('t1');"
    "if(p==1){"
      "c=eb('c1');"                       // Console command id
      "o='&c1='+encodeURIComponent(c.value);"
      "c.value='';"
      "t.scrollTop=99999;"
      "sn=t.scrollTop;"
    "}"
    "if(t.scrollTop>=sn){"                // User scrolled back so no updates
      "if(x!=null){x.abort();}"           // Abort if no response within 2 seconds (happens on restart 1)
      "x=new XMLHttpRequest();"
      "x.onreadystatechange=function(){"
        "if(x.readyState==4&&x.status==200){"
          "var z,d;"
          "d=x.responseText.split(/}1/);"  // Field separator
          "id=d.shift();"
          "if(d.shift()==0){t.value='';}"
          "z=d.shift();"
          "if(z.length>0){t.value+=z;}"
          "t.scrollTop=99999;"
          "sn=t.scrollTop;"
          "clearTimeout(ft);"
          "lt=setTimeout(l,ltm);" // webrefresh timer....
        "}"
      "};"
      "x.open('GET','cs?c2='+id+o,true);"  // Related to Webserver->hasArg("c2") and WebGetArg("c2", stmp, sizeof(stmp))
      "x.send();"
      "ft=setTimeout(l,20000);" // fail timeout, triggered 20s after asking for XHR
    "}else{"
      "lt=setTimeout(l,ltm);" // webrefresh timer....
    "}"
    "return false;"
  "}"
  "wl(l);"                                // Load initial console text

  // Console command history
  "var hc=[],cn=0;"                       // hc = History commands, cn = Number of history being shown
  "function h(){"
//    "if(!(navigator.maxTouchPoints||'ontouchstart'in document.documentElement)){eb('c1').autocomplete='off';}"  // No touch so stop browser autocomplete
    "eb('c1').addEventListener('keydown',function(e){"
      "var b=eb('c1'),c=e.keyCode;"       // c1 = Console command id
      "if(38==c||40==c){b.autocomplete='off';}"  // ArrowUp or ArrowDown must be a keyboard so stop browser autocomplete
      "38==c?(++cn>hc.length&&(cn=hc.length),b.value=hc[cn-1]||''):"   // ArrowUp
      "40==c?(0>--cn&&(cn=0),b.value=hc[cn-1]||''):"                   // ArrowDown
      "13==c&&(hc.length>19&&hc.pop(),hc.unshift(b.value),cn=0)"       // Enter, 19 = Max number -1 of commands in history
    "});"
  "}"
  "wl(h);";                               // Add console command key eventlistener after name has been synced with id (= wl(jd))
  
