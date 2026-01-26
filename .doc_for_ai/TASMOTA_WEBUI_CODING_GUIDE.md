# Tasmota WebUI Coding Guide

This guide provides comprehensive instructions for coding WebUI interfaces for Tasmota, based on analysis of the actual Tasmota web interface source code and screenshots.

## Overview

Tasmota's WebUI is a lightweight, embedded web interface designed for ESP8266/ESP32 microcontrollers with severe memory constraints. The interface follows a minimalist design philosophy while providing comprehensive device control and configuration capabilities.

### Visual Design Analysis

Based on the actual Tasmota WebUI screenshots, the interface features:

1. **Main Control Page**: 
   - Large status display showing device state ("OFF")
   - Color control sliders with visual gradients (hue, saturation, brightness)
   - Toggle buttons for device control
   - Clean button layout with consistent spacing

2. **Configuration Pages**:
   - Nested fieldsets for logical grouping
   - Form elements with proper labels and placeholders
   - Consistent button styling with color coding (blue for navigation, green for save, red for dangerous actions)
   - Mobile-optimized layout with full-width buttons

3. **Navigation Structure**:
   - Hierarchical menu system
   - Clear visual separation between sections
   - Consistent header with device name and Tasmota branding

## Core Design Principles

### 1. Memory Efficiency
- Minimal HTML/CSS/JavaScript footprint
- Inline styles and scripts to reduce HTTP requests
- Compressed and minified code
- CSS variables for theming without duplication

### 2. Responsive Design
- Mobile-first approach with `viewport` meta tag
- Flexible layouts that work on small screens
- Touch-friendly button sizes and spacing
- Minimal external dependencies

### 3. Dark Theme by Default
- Professional dark color scheme
- High contrast for readability
- Consistent color variables throughout

### 4. Progressive Enhancement
- Core functionality works without JavaScript
- JavaScript enhances user experience
- Graceful degradation for older browsers

## HTML Structure Pattern

### Basic Page Template

```html
<!DOCTYPE html>
<html lang="en" class="">
<head>
    <meta charset='utf-8'>
    <meta name="viewport" content="width=device-width,initial-scale=1"/>
    <link rel="icon" href="data:image/x-icon;base64,AAABAAEAEBACAAEAAQCwAAAAFgAAACgAAAAQAAAAIAAAAAEAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA////AP5/b+H6X2/h8k9v4eZnb+Hud2/h7ndv4e53b+FmZm/hMkxv4ZgZb+HOc2/h5+dv4fPPb+H5n2/h/D9v4f5/b+EAAO4EAADuBAAA7gQAAO4EAADuBAAA7gQAAO4EAADuBAAA7gQAAO4EAADuBAAA7gQAAO4EAADuBAAA7gQAAO4E">
    <title>Tasmota Configuration</title>
    <script>
        // Core JavaScript functions
    </script>
    <style>
        /* CSS styles */
    </style>
</head>
<body>
    <div style='background:var(--c_bg);text-align:left;display:inline-block;color:var(--c_txt);min-width:340px;position:relative;'>
        <!-- Page content -->
    </div>
</body>
</html>
```

### Key HTML Structure Elements

1. **Container Div**: Main wrapper with consistent styling
2. **Header Section**: Device name and page title
3. **Content Area**: Forms, buttons, and configuration options
4. **Footer**: Version information and links

## CSS Design System

### Color Variables

Tasmota uses CSS custom properties for consistent theming:

```css
:root {
    --c_bg: #252525;        /* Background color */
    --c_frm: #4f4f4f;       /* Form/fieldset background */
    --c_ttl: #eaeaea;       /* Title text color */
    --c_txt: #eaeaea;       /* Regular text color */
    --c_txtwrn: #ff5661;    /* Warning text color */
    --c_txtscc: #008000;    /* Success text color */
    --c_btn: #1fa3ec;       /* Primary button color */
    --c_btnoff: #08405e;    /* Disabled button color */
    --c_btntxt: #faffff;    /* Button text color */
    --c_btnhvr: #0e70a4;    /* Button hover color */
    --c_btnrst: #d43535;    /* Reset/danger button color */
    --c_btnrsthvr: #931f1f; /* Reset button hover */
    --c_btnsv: #47c266;     /* Save button color */
    --c_btnsvhvr: #5aaf6f;  /* Save button hover */
    --c_in: #dddddd;        /* Input background */
    --c_intxt: #000000;     /* Input text color */
    --c_csl: #1f1f1f;       /* Console background */
    --c_csltxt: #65c115;    /* Console text color */
    --c_tab: #999999;       /* Tab color */
    --c_tabtxt: #faffff;    /* Tab text color */
}
```

### Typography and Layout

```css
body {
    text-align: center;
    font-family: verdana, sans-serif;
    background: var(--c_bg);
}

div, fieldset, input, select {
    padding: 5px;
    font-size: 1em;
}

fieldset {
    background: var(--c_frm);
}

p {
    margin: 0.5em 0;
}
```

### Input Styling

```css
input {
    width: 100%;
    box-sizing: border-box;
    -webkit-box-sizing: border-box;
    -moz-box-sizing: border-box;
    background: var(--c_in);
    color: var(--c_intxt);
}

input[type=checkbox], input[type=radio] {
    width: 1em;
    margin-right: 6px;
    vertical-align: -1px;
}

input[type=range] {
    width: 99%;
}

select {
    width: 100%;
    background: var(--c_in);
    color: var(--c_intxt);
}
```

### Button System

```css
button {
    border: 0;
    border-radius: 0.3rem;
    background: var(--c_btn);
    color: var(--c_btntxt);
    line-height: 2.4rem;
    font-size: 1.2rem;
    width: 100%;
    -webkit-transition-duration: 0.4s;
    transition-duration: 0.4s;
    cursor: pointer;
}

button:hover {
    background: var(--c_btnhvr);
}

.bred {
    background: var(--c_btnrst);
}

.bred:hover {
    background: var(--c_btnrsthvr);
}

.bgrn {
    background: var(--c_btnsv);
}

.bgrn:hover {
    background: var(--c_btnsvhvr);
}
```

## JavaScript Patterns

### Core Utility Functions

```javascript
// Element selection shortcuts
var eb = s => document.getElementById(s);
var qs = s => document.querySelector(s);

// Password visibility toggle
var sp = i => eb(i).type = (eb(i).type === 'text' ? 'password' : 'text');

// Window load event handler
var wl = f => window.addEventListener('load', f);

// Auto-assign names to form elements
function jd() {
    var t = 0, i = document.querySelectorAll('input,button,textarea,select');
    while (i.length >= t) {
        if (i[t]) {
            i[t]['name'] = (i[t].hasAttribute('id') && (!i[t].hasAttribute('name'))) 
                ? i[t]['id'] : i[t]['name'];
        }
        t++;
    }
}

// Show/hide elements with class 'hf'
function sf(s) {
    var t = 0, i = document.querySelectorAll('.hf');
    while (i.length >= t) {
        if (i[t]) {
            i[t].style.display = s ? 'block' : 'none';
        }
        t++;
    }
}

wl(jd); // Auto-assign names on load
```

### AJAX Communication

```javascript
var x = null, lt, to, tp, pc = '';

// Load data with AJAX
function la(p) {
    a = p || '';
    clearTimeout(ft);
    clearTimeout(lt);
    if (x != null) { x.abort(); }
    
    x = new XMLHttpRequest();
    x.onreadystatechange = () => {
        if (x.readyState == 4 && x.status == 200) {
            var s = x.responseText
                .replace(/{t}/g, "<table style='width:100%'>")
                .replace(/{s}/g, "<tr><th>")
                .replace(/{m}/g, "</th><td style='width:20px;white-space:nowrap'>")
                .replace(/{e}/g, "</td></tr>");
            eb('l1').innerHTML = s;
            clearTimeout(ft);
            clearTimeout(lt);
            lt = setTimeout(la, 400); // Auto-refresh every 400ms
        }
    };
    x.open('GET', '.?m=1' + a, true);
    x.send();
    ft = setTimeout(la, 2e4); // Fallback timeout 20 seconds
}

// Control function for sliders and buttons
function lc(v, i, p) {
    if (eb('s')) {
        if (v == 'h' || v == 'd') {
            var sl = eb('sl4').value;
            eb('s').style.background = 'linear-gradient(to right,rgb(' + sl + '%,' + sl + '%,' + sl + '%),hsl(' + eb('sl2').value + ',100%,50%))';
        }
    }
    la('&' + v + i + '=' + p);
}
```

### Form Handling

```javascript
// Submit form with UI feedback
function su(t) {
    eb('f3').style.display = 'none';
    eb('f2').style.display = 'block';
    t.form.submit();
}

// File upload with validation
function upl(t) {
    var sl = t.form['u2'].files[0].slice(0, 1);
    var rd = new FileReader();
    rd.onload = () => {
        var bb = new Uint8Array(rd.result);
        if (bb.length == 1 && bb[0] == 0xE9) {
            fct(t); // Factory reset check
        } else {
            t.form.submit();
        }
    };
    rd.readAsArrayBuffer(sl);
    return false;
}

// Factory reset confirmation
function fct(t) {
    var x = new XMLHttpRequest();
    x.open('GET', '/u4?u4=fct&api=', true);
    x.onreadystatechange = () => {
        if (x.readyState == 4 && x.status == 200) {
            var s = x.responseText;
            if (s == 'false') setTimeout(() => { fct(t); }, 6000);
            if (s == 'true') setTimeout(() => { su(t); }, 1000);
        } else if (x.readyState == 4 && x.status == 0) {
            setTimeout(() => { fct(t); }, 2000);
        }
    };
    x.send();
}
```

## Page Layout Patterns

### Configuration Menu Layout

Based on the Tasmota configuration menu, here's the standard layout pattern:

```html
<div style='background:var(--c_bg);text-align:left;display:inline-block;color:var(--c_txt);min-width:340px;position:relative;'>
    <!-- Header -->
    <div style='text-align:center;color:var(--c_ttl);'>
        <noscript>To use Tasmota, please enable JavaScript<br></noscript>
        <h3>ESP32-DevKit</h3>
        <h2>Tasmota</h2>
    </div>
    
    <!-- Page Title -->
    <div style='padding:0px 5px;text-align:center;'>
        <h3><hr>Configuration<hr></h3>
    </div>
    
    <!-- Menu Items -->
    <p></p>
    <form id="but7" style="display:block;" action='md' method='get'>
        <button>Module</button>
    </form>
    
    <p></p>
    <form id="but8" style="display:block;" action='wi' method='get'>
        <button>WiFi</button>
    </form>
    
    <!-- More menu items... -->
    
    <!-- Footer -->
    <div style='text-align:right;font-size:11px;'>
        <hr>
        <a href='https://github.com/arendst/Tasmota' target='_blank' style='color:#aaa;'>
            Tasmota 15.0.1.4 (tasmota) by Theo Arends
        </a>
    </div>
</div>
```

### Configuration Form Layout

For configuration pages with forms:

```html
<fieldset>
    <legend><b>&nbsp;Other parameters&nbsp;</b></legend>
    <form method='get' action='co'>
        
        <!-- Template Section -->
        <fieldset>
            <legend><b>&nbsp;Template&nbsp;</b></legend>
            <p>
                <input id='t1' placeholder="Template" value='{&quot;NAME&quot;:&quot;ESP32-DevKit&quot;,&quot;GPIO&quot;:[1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1376,0,1,224,1,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,1],&quot;FLAG&quot;:0,&quot;BASE&quot;:1}'>
            </p>
            <p>
                <label>
                    <input id='t2' type='checkbox' checked disabled>
                    <b>Activate</b>
                </label>
            </p>
        </fieldset>
        
        <!-- Password Field with Toggle -->
        <br>
        <label>
            <b>Web Admin Password</b>
            <input type='checkbox' onclick='sp("wp")'>
        </label>
        <br>
        <input id='wp' type='password' placeholder="Web Admin Password" value="****">
        
        <!-- Checkboxes -->
        <br><br>
        <label>
            <input id='b3' type='checkbox' checked>
            <b>HTTP API enable</b>
        </label>
        
        <!-- Text Inputs -->
        <br><br>
        <label><b>Device Name</b> (Tasmota)</label>
        <br>
        <input id='dn' placeholder="" value="Tasmota">
        
        <!-- Radio Buttons -->
        <fieldset>
            <legend><b>&nbsp;Emulation&nbsp;</b></legend>
            <p>
                <label>
                    <input id='r0' name='b2' type='radio' value='0'>
                    <b>None</b>
                </label>
                <br>
                <label>
                    <input id='r2' name='b2' type='radio' value='2' checked>
                    <b>Hue Bridge</b> multi device
                </label>
            </p>
        </fieldset>
        
        <!-- Submit Button -->
        <br>
        <button name='save' type='submit' class='button bgrn'>Save</button>
    </form>
</fieldset>
```

### Main Page with Controls

The main control page features a prominent status display and interactive controls. Based on the screenshot analysis:

```html
<!-- Dynamic Content Area -->
<div style='padding:0;' id='l1' name='l1'></div>

<!-- Control Buttons -->
<table style='width:100%'>
    <tr>
        <td style='width:100%'>
            <button id='o1' onclick='la("&o=1");'>Toggle 1</button>
        </td>
    </tr>
</table>

<!-- Color Controls -->
<table style='width:100%'>
    <!-- Hue Slider -->
    <tr>
        <td colspan='2' style='width:100%'>
            <div id='b' class='r' style='background-image:linear-gradient(to right,#800,#f00 5%,#ff0 20%,#0f0 35%,#0ff 50%,#00f 65%,#f0f 80%,#f00 95%,#800);'>
                <input id='sl2' type='range' min='0' max='359' value='95' onchange='lc("h",0,value)'>
            </div>
        </td>
    </tr>
    
    <!-- Saturation Slider -->
    <tr>
        <td colspan='2' style='width:100%'>
            <div id='s' class='r' style='background-image:linear-gradient(to right,#CCCCCC,#6AFF00);'>
                <input id='sl3' type='range' min='0' max='100' value='94' onchange='lc("n",0,value)'>
            </div>
        </td>
    </tr>
    
    <!-- Brightness Control -->
    <tr>
        <td style='width:15%'>
            <button id='o2' onclick='la("&o=2");'>T2</button>
        </td>
        <td colspan='1' style='width:85%'>
            <div id='c' class='r' style='background-image:linear-gradient(to right,#000,#fff);'>
                <input id='sl4' type='range' min='0' max='100' value='80' onchange='lc("d",0,value)'>
            </div>
        </td>
    </tr>
</table>

<!-- Button State Script -->
<script>
    eb('o1').style.background = 'var(--c_btnoff)';
</script>
```

## Advanced UI Components



## Advanced UI Components

### Range Sliders with Visual Feedback

```css
.r {
    border-radius: 0.3em;
    padding: 2px;
    margin: 4px 2px;
}
```

### Textarea for Console/Logs

```css
textarea {
    resize: vertical;
    width: 98%;
    height: 318px;
    padding: 5px;
    overflow: auto;
    background: var(--c_bg);
    color: var(--c_csltxt);
}
```

### Hidden Elements

```css
.hf {
    display: none;
}
```

### Utility Classes

```css
.p {
    float: left;
    text-align: left;
}

.q {
    float: right;
    text-align: right;
}

a {
    color: var(--c_btn);
    text-decoration: none;
}

td {
    padding: 0px;
}
```

## Best Practices

### 1. Memory Optimization
- Use inline styles for unique elements
- Minimize JavaScript object creation
- Reuse DOM elements where possible
- Use CSS variables for consistent theming

### 2. User Experience
- Provide immediate visual feedback for actions
- Use consistent button sizing and spacing
- Implement proper form validation
- Show loading states during operations

### 3. Accessibility
- Use semantic HTML elements
- Provide proper labels for form controls
- Ensure sufficient color contrast
- Support keyboard navigation

### 4. Performance
- Minimize HTTP requests
- Use efficient DOM manipulation
- Implement proper error handling
- Cache frequently accessed elements

### 5. Responsive Design
- Use flexible layouts
- Test on various screen sizes
- Ensure touch-friendly interface
- Provide appropriate viewport settings

## Integration with Tasmota Backend





## Common UI Patterns

### 1. Toggle Buttons (from original source)
```html
<button id='o1' onclick='la("&o=1");'>Toggle 1</button>
```

### 2. Configuration Sections
```html
<fieldset>
    <legend><b>&nbsp;Section Title&nbsp;</b></legend>
    <!-- Configuration options -->
</fieldset>
```

### 3. Input with Label
```html
<label><b>Setting Name</b> (default)</label>
<br>
<input id='setting' placeholder="Enter value" value="current_value">
```

### 4. Checkbox with Label
```html
<label>
    <input id='option' type='checkbox' checked>
    <b>Option Description</b>
</label>
```

### 5. Radio Button Group
```html
<fieldset>
    <legend><b>&nbsp;Emulation&nbsp;</b></legend>
    <p>
        <label>
            <input name='emulation' type='radio' value='0'>
            <b>None</b>
        </label>
        <br>
        <label>
            <input name='emulation' type='radio' value='2' checked>
            <b>Hue Bridge</b> multi device
        </label>
    </p>
</fieldset>
```

### 6. Password Field with Toggle
```html
<label>
    <b>Web Admin Password</b>
    <input type='checkbox' onclick='sp("wp")' style='width:auto;margin-left:10px;'>
</label>
<br>
<input id='wp' type='password' placeholder="Web Admin Password" value="****">
```

### 7. Template Configuration (from original source)
```html
<fieldset>
    <legend><b>&nbsp;Template&nbsp;</b></legend>
    <p>
        <input id='t1' placeholder="Template" value='{&quot;NAME&quot;:&quot;ESP32-DevKit&quot;,&quot;GPIO&quot;:[1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1376,0,1,224,1,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,1],&quot;FLAG&quot;:0,&quot;BASE&quot;:1}'>
    </p>
    <p>
        <label>
            <input id='t2' type='checkbox' checked disabled>
            <b>Activate</b>
        </label>
    </p>
</fieldset>
```

### 8. Color Control Sliders (from original source)
```html
<table style='width:100%'>
    <tr>
        <td colspan='2' style='width:100%'>
            <div id='b' class='r' style='background-image:linear-gradient(to right,#800,#f00 5%,#ff0 20%,#0f0 35%,#0ff 50%,#00f 65%,#f0f 80%,#f00 95%,#800);'>
                <input id='sl2' type='range' min='0' max='359' value='95' onchange='lc("h",0,value)'>
            </div>
        </td>
    </tr>
    <tr>
        <td colspan='2' style='width:100%'>
            <div id='s' class='r' style='background-image:linear-gradient(to right,#CCCCCC,#6AFF00);'>
                <input id='sl3' type='range' min='0' max='100' value='94' onchange='lc("n",0,value)'>
            </div>
        </td>
    </tr>
    <tr>
        <td style='width:15%'>
            <button id='o2' onclick='la("&o=2");'>T2</button>
        </td>
        <td colspan='1' style='width:85%'>
            <div id='c' class='r' style='background-image:linear-gradient(to right,#000,#fff);'>
                <input id='sl4' type='range' min='0' max='100' value='80' onchange='lc("d",0,value)'>
            </div>
        </td>
    </tr>
</table>
```

This guide provides the foundation for creating Tasmota-compatible WebUI interfaces that are efficient, user-friendly, and consistent with the existing design system. The visual specifications are based on actual Tasmota WebUI screenshots showing the main control page, configuration forms, and navigation menus.