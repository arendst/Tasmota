const char HTTP_SCRIPT_ROOT_PART2[] PROGMEM =
  "function lc(v,i,p){"
    "if(eb('s')){"                        // Check if Saturation is in DOM otherwise javascript fails on la()
      "if(v=='h'||v=='d'){"               // Hue or Brightness changed so change Saturation colors too
        "var sl=eb('sl4').value;"
        "eb('s').style.background='linear-gradient(to right,rgb('+sl+'%%,'+sl+'%%,'+sl+'%%),hsl('+eb('sl2').value+',100%%,50%%))';"
      "}"
    "}"
    "la('&'+v+i+'='+p);"
  "}"
  "wl(la);";
