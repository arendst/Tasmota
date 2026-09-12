const char HTTP_HEAD_STYLE_TOOLTIP[] PROGMEM =
  // Enable tooltips on touch media
  "@media(pointer:coarse),(hover:none){"
    "[title]{position:relative;}"
    "[title]:hover::after{"
      "content:attr(title);"
      "position:absolute;"
      "z-index:999;"
      "top:100%;"
      "left:50%;"
      "transform:translateX(-50%);"
      "color:var(--c_btntxt);"
      "background-color:var(--c_btn);"
      "border:1px solid;"
      "padding:2px 5px;"
      "font-style:normal;"    // Disable italic and/or bold
      "font-size:12px;"
      "white-space:pre;"      // Stay on one line except sending \n
    "}"
  "}";
