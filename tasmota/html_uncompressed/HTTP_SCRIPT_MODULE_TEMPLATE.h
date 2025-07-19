const char HTTP_SCRIPT_MODULE_TEMPLATE[] PROGMEM =
  "var os,hs;"
  "function ce(i,q){"                     // Create index select
    "var o=document.createElement('option');"
    "o.textContent=i;"
    "q.appendChild(o);"
  "}"
  "function ot(g,s){"                     // g = id and name, s = value
    "var a=s&0xffe0,b=0,c,p,l,t=qs('#h'+g),u=s&0x001f;"
    "l=t.options.length;"                 // Remove current options
    "for(i=l;i;i--){p=t.options[i-1].parentNode;p.removeChild(t.options[i-1]);}"
    "l=hs.length;"                        // Find max indexes for s
    "for(i=0;i<l;i++){c=hs[i]&0xffe0;if(a==c){b=hs[i]&0x001f;break;}}"
    "s>>=5;"                              // Add options
    "for(i=0;i<=b;i++){ce(i+1,t);}"       // Add index 1 to 32
    "eb('h'+g).value=u+1;"                // Set selected value
    "t.style.visibility=(b>0)?'':'hidden';"
  "}"
  "function sk(s,g){"                     // s = value, g = id and name
    "var o=os.replace(/}2/g,\"<option value='\").replace(/}3/g,\"</option>\");"
    "eb('g'+g).innerHTML=o;"
    "eb('g'+g).value=(g<99)?s&0xffe0:s;"
    "if(g<99){ot(g,s);}"
  "}";
