const char HTTP_HEAD_STYLE_TOOLTIP[] PROGMEM =
  // Enable tooltips on touch media
  "@media(pointer:coarse),(hover:none){"
    "[title]{position:relative;display:inline-flex;justify-content:center;}"
    "[title]:hover::after{"
      "content:attr(title);position:absolute;top:90%;"
      "color:var(--c_btntxt);background-color:var(--c_btn);border:1px solid;"
      "width:fit-content;"
      "padding:2px 5px;"
      "font-style:normal;"    // Disable italic and/or bold
      "font-size:12px;"
      "white-space:nowrap;"   // Stay on one line
    "}"
  "}";
