const char HTTP_HEAD_STYLE1[] PROGMEM =
  "div,fieldset,input,select{padding:5px;font-size:1em;}"
  "fieldset{background:var(--c_frm);}"  // COLOR_FORM, Also update HTTP_TIMER_STYLE
  "p{margin:0.5em 0;}"
  "input{width:100%;box-sizing:border-box;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;background:var(--c_in);color:var(--c_intxt);}"  // COLOR_INPUT, COLOR_INPUT_TEXT
  "input[type=checkbox],input[type=radio]{width:1em;margin-right:6px;vertical-align:-1px;}"
  "input[type=range]{width:99%;}"
  "select{width:100%;background:var(--c_in);color:var(--c_intxt);}"  // COLOR_INPUT, COLOR_INPUT_TEXT
  "textarea{resize:vertical;width:98%;height:318px;padding:5px;overflow:auto;background:var(--c_bg);color:var(--c_csltxt);}"  // COLOR_CONSOLE, COLOR_CONSOLE_TEXT
  "body{text-align:center;font-family:verdana,sans-serif;background:var(--c_bg);}"  // COLOR_BACKGROUND
  "td{padding:0px;}"
  "button{border:0;border-radius:0.3rem;background:var(--c_btn);color:var(--c_btntxt);line-height:2.4rem;font-size:1.2rem;width:100%;-webkit-transition-duration:0.4s;transition-duration:0.4s;cursor:pointer;}"  // COLOR_BUTTON, COLOR_BUTTON_TEXT
  "button:hover{background:var(--c_btnhvr);}"  // COLOR_BUTTON_HOVER
  ".bred{background:var(--c_btnrst);}"  // COLOR_BUTTON_RESET
  ".bred:hover{background:var(--c_btnrsthvr);}"  // COLOR_BUTTON_RESET_HOVER
  ".bgrn{background:var(--c_btnsv);}"  // COLOR_BUTTON_SAVE
  ".bgrn:hover{background:var(--c_btnsvhvr);}"  // COLOR_BUTTON_SAVE_HOVER
  "a{color:var(--c_btn);text-decoration:none;}"  // COLOR_BUTTON
  ".p{float:left;text-align:left;}"
  ".q{float:right;text-align:right;}"
  ".r{border-radius:0.3em;padding:2px;margin:4px 2px;}"
  ".hf{display:none;}";
