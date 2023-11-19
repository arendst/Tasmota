const char HTTP_HEAD_LAST_SCRIPT[] PROGMEM =
  "function jd(){"                        // Add label name='' based on provided id=''
    "var t=0,i=document.querySelectorAll('input,button,textarea,select');"
    "while(i.length>=t){"
      "if(i[t]){"
        "i[t]['name']=(i[t].hasAttribute('id')&&(!i[t].hasAttribute('name')))?i[t]['id']:i[t]['name'];"
      "}"
      "t++;"
    "}"
  "}"
  "function sf(s){"
    "var t=0,i=document.querySelectorAll('.hf');"
    "while(i.length>=t){"
      "if(i[t]){"
        "i[t].style.display=s?'block':'none';"
      "}"
      "t++;"
    "}"
  "}"
  "wl(jd);"                               // Add name='' to any id='' in input,button,textarea,select
  "</script>";
