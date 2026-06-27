const char HTTP_HEAD_STYLE1[] PROGMEM =
  "div,fieldset,input,select{padding:5px;font-size:1em;}"
  "fieldset{background:var(--c_frm);border:1px solid rgba(255,255,255,.08);border-radius:10px;padding:12px 14px;margin:0 0 8px;}"  // COLOR_FORM, Also update HTTP_TIMER_STYLE
  "p{margin:0.5em 0;}"
  "input{width:100%;box-sizing:border-box;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;background:var(--c_in);color:var(--c_intxt);border:1px solid rgba(255,255,255,.1);border-radius:8px;font-family:ui-monospace,SFMono-Regular,Menlo,Consolas,monospace;}"  // COLOR_INPUT, COLOR_INPUT_TEXT
  "input:focus,select:focus,textarea:focus{outline:2px solid var(--c_btn);outline-offset:1px;}"
  "input[type=checkbox],input[type=radio]{width:1em;margin-right:6px;vertical-align:-1px;}"
  "input[type=range]{width:99%;}"
  "select{width:100%;background:var(--c_in);color:var(--c_intxt);border:1px solid rgba(255,255,255,.1);border-radius:8px;}"  // COLOR_INPUT, COLOR_INPUT_TEXT
  "textarea{resize:vertical;width:98%;height:318px;padding:5px;overflow:auto;background:var(--c_csl);color:var(--c_csltxt);border:1px solid rgba(255,255,255,.1);border-radius:8px;font-family:ui-monospace,SFMono-Regular,Menlo,Consolas,monospace;}"  // COLOR_CONSOLE, COLOR_CONSOLE_TEXT
  "body{text-align:center;font-family:system-ui,-apple-system,Segoe UI,Roboto,sans-serif;background:var(--c_bg);}"  // COLOR_BACKGROUND
  "td{padding:0px;}"
  "hr{border:0;border-top:1px solid rgba(255,255,255,.12);margin:.6em 0;}"
  "h2{font-size:1.3em;font-weight:600;margin:.15em 0;}"            // Device name
  "h3{font-size:.72em;font-weight:600;letter-spacing:.13em;text-transform:uppercase;opacity:.55;margin:.4em 0 .1em;}"  // Module name eyebrow
  "h4{font-family:ui-monospace,SFMono-Regular,Menlo,Consolas,monospace;font-size:.78em;font-weight:400;opacity:.55;margin:.3em 0;}";  // Hostname / IP data
