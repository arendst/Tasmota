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
  "td{padding:0px;}";
