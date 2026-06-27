const char HTTP_HEAD_STYLE2[] PROGMEM =
  "button{border:1px solid rgba(255,255,255,.14);border-radius:8px;background:transparent;color:var(--c_txt);line-height:2.4rem;font-size:1.05rem;font-family:inherit;font-weight:600;width:100%;transition:.2s;cursor:pointer;}"  // Navigation buttons - quiet ghost style
  "button:hover{border-color:var(--c_btn);color:var(--c_btn);background:rgba(31,163,236,.08);}"  // COLOR_BUTTON_HOVER
  "button:focus-visible{outline:2px solid var(--c_btn);outline-offset:2px;}"
  "button[id^=o]{border:0;background:var(--c_btn);color:var(--c_btntxt);}"  // Device control buttons - solid accent (COLOR_BUTTON, COLOR_BUTTON_TEXT)
  "button[id^=o]:hover{background:var(--c_btnhvr);box-shadow:0 0 16px -3px var(--c_btn);}"  // COLOR_BUTTON_HOVER
  ".bred{border:0;background:var(--c_btnrst);color:#fff;}"  // COLOR_BUTTON_RESET
  ".bred:hover{background:var(--c_btnrsthvr);color:#fff;box-shadow:0 0 14px -3px var(--c_btnrst);}"  // COLOR_BUTTON_RESET_HOVER
  ".bgrn{border:0;background:var(--c_btnsv);color:#fff;}"  // COLOR_BUTTON_SAVE
  ".bgrn:hover{background:var(--c_btnsvhvr);color:#fff;box-shadow:0 0 14px -3px var(--c_btnsv);}"  // COLOR_BUTTON_SAVE_HOVER
  "a{color:var(--c_btn);text-decoration:none;}"  // COLOR_BUTTON
  ".p{float:left;text-align:left;}"
  ".q{float:right;text-align:right;}"
  ".r{border-radius:0.3em;padding:2px;margin:4px 2px;}"
  ".hf{display:none;}"
  "@media(prefers-reduced-motion){button{transition:none;}}";
