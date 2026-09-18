# Design Document: Matter UI Parameter System

## Reference Documentation

This design document assumes familiarity with the Berry language and its Tasmota integration:

- #[[file:.doc_for_ai/BERRY_LANGUAGE_REFERENCE.md]]
- #[[file:.doc_for_ai/BERRY_TASMOTA.md]]

## Overview

The Matter UI Parameter System introduces a flexible, schema-driven approach to endpoint configuration in Tasmota's Matter implementation. The system replaces the current single-parameter limitation with a multi-parameter architecture that supports complex device configurations while maintaining backward compatibility.

### Key Design Decisions

1. **Schema-Driven Architecture**: Parameter definitions are declarative, stored as static data in plugin classes
2. **Compact Pipe-Separated Format**: Short 1-letter identifiers minimize memory usage in Berry classes
3. **Client-Side Form Generation**: JavaScript generates form fields dynamically in the browser when endpoint type changes
4. **Runtime Schema Delivery**: Schemas are collected from plugin classes and sent to browser at page load
5. **Browser-Computed Full Config JSON**: When adding or changing endpoints, the browser computes the entire `config` JSON object and sends it as a single string to the server, eliminating per-field server-side parsing
6. **Server-Computed `nextep`**: The `nextep` value is never sent by the browser; it is always recomputed server-side from the config keys using `adjust_next_ep()`
7. **Backward Compatibility**: Automatic migration from single-parameter to multi-parameter format

### Design Goals

- Support multiple parameters per endpoint type
- Instant UI updates when changing endpoint type (no page reload)
- Maintain Tasmota's minimal memory footprint
- Preserve existing configurations during upgrade
- Follow Tasmota WebUI coding guidelines
- Enable easy addition of new endpoint types
- Minimize server-side parsing by having the browser send complete config JSON

## Architecture

### System Components

```
┌─────────────────────────────────────────────────────────────┐
│                     Web Browser (User)                       │
│  ┌──────────────────────────────────────────────────────┐   │
│  │         Client-Side Config Builder (JS)              │   │
│  │  - parseSchema(str) - parse compact format           │   │
│  │  - genFields(type, containerId) - generate HTML      │   │
│  │  - buildConfigJson() - build full config JSON        │   │
│  │  - ps = {...} - Schema_Map (sent at runtime)         │   │
│  │  - cfg = {...} - Current config (sent at runtime)    │   │
│  └──────────────────────────────────────────────────────┘   │
└────────────────────────┬────────────────────────────────────┘
                         │ HTTP POST (config_json=<full config>)
                         ▼
┌─────────────────────────────────────────────────────────────┐
│                  Matter_UI (Berry Module)                   │
│  ┌──────────────────────────────────────────────────────┐   │
│  │         Schema Collection                            │   │
│  │  - collect_schemas() - gather from all plugins       │   │
│  │  - generate_schema_js() - output as JavaScript       │   │
│  └──────────────────────────────────────────────────────┘   │
│  ┌──────────────────────────────────────────────────────┐   │
│  │         Server-Side Processing                       │   │
│  │  - Receive full config JSON string                   │   │
│  │  - json.load() to parse                              │   │
│  │  - Validate endpoint types exist                     │   │
│  │  - Replace plugins_config wholesale                  │   │
│  │  - adjust_next_ep() to recompute nextep              │   │
│  │  - Reconcile live plugin instances                   │   │
│  └──────────────────────────────────────────────────────┘   │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│              Plugin Classes (Matter_Plugin_*)               │
│  ┌──────────────────────────────────────────────────────┐   │
│  │  static var ARG = "relay"                            │   │
│  │  static var SCHEMA = "relay|"                        │   │
│  │                      "t:i|"                          │   │
│  │                      "h:Relay<x> number|"            │   │
│  │                      "d:1"                           │   │
│  └──────────────────────────────────────────────────────┘   │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│              _matter_device.json (Persistence)               │
└─────────────────────────────────────────────────────────────┘
```

### Data Flow

#### Flow 1: Page Load (Configuration Page)

1. User requests `/matterc` page
2. Server collects SCHEMA from all plugin classes
3. Server generates JavaScript with:
   - `ps` (Schema_Map) for form generation
   - `cfg` (current config) as JSON for the browser to know existing state
4. Browser has all schemas and current config available

#### Flow 2: Type Selection Change (Add New Endpoint)

1. User selects endpoint type from dropdown
2. JavaScript `genFields()` called with new type
3. JS parses compact schema string with `parseSchema()`
4. Fields generated instantly from parsed schema
5. No server round-trip required

#### Flow 3: Configuration Save (Add/Change/Delete Endpoint)

1. User modifies endpoints (add, change name/args, delete)
2. Browser JavaScript builds the complete `config` JSON object from the current UI state
3. Browser POSTs `config_json=<JSON string>` to `/matterc`
4. Server receives the JSON string, parses with `json.load()`
5. Server validates that all endpoint types exist in `plugins_classes`
6. Server replaces `self.device.plugins_config` with the new config
7. Server calls `self.device.adjust_next_ep()` to recompute `nextep`
8. Server reconciles live plugin instances (add new, remove deleted, update changed)
9. Server calls `self.device.save_param()` to persist

#### Flow 4: Add New Endpoint

1. User fills in name, type, and parameters in the "Add" form
2. Browser JavaScript:
   - Allocates the next endpoint number (from a JS variable `nextep` sent at page load)
   - Builds the config entry for the new endpoint
   - Merges it into the existing `cfg` object
   - Sends the full `cfg` as `config_json`
3. Server processes as in Flow 3

## Components and Interfaces

### 1. Parameter Schema Definition

(Unchanged from previous design - compact pipe-separated format with SCHEMA, SCHEMA2, SCHEMA3...)

#### SCHEMA Structure

Each plugin class defines static SCHEMA strings using a compact pipe-separated format where the first element is the arg name (no prefix). Multiple parameters are supported using numbered variables: `SCHEMA`, `SCHEMA2`, `SCHEMA3`, etc.

```berry
# Single parameter (most plugins) - first element is arg name
static var SCHEMA = "relay|"                       # arg name
                    "t:i|"                         # type: int
                    "h:1-8|"                       # hint
                    "d:1"                          # default

# Multiple parameters (complex plugins like Air Quality)
static var SCHEMA = "co2|"                         # arg name
                    "t:t|"                         # type: text
                    "h:CO2 filter (ex: SCD40#CarbonDioxide)"
static var SCHEMA2 = "no2|t:t|h:NO2 filter (ex: SEN55#NO2)"
static var SCHEMA3 = "pm1|t:t|h:PM1 filter (ex: SEN55#PM1)"
```

**Format Specification:**
- First element is the arg name (no prefix required)
- Remaining elements use short 1-letter identifiers with `key:value` format
- Order doesn't matter for key:value elements after the arg name
- Numbered schemas (SCHEMA, SCHEMA2, SCHEMA3...) define multiple parameters
- Child classes automatically inherit parent schemas

**Short Identifiers:**
- First element (no prefix) - Arg name (parameter key name) - REQUIRED
- `l:` - Label (display name shown above the field, defaults to arg name if omitted)
- `t:` - Type: `t` (text, DEFAULT), `i` (int), `s` (select), `c` (checkbox)
- `h:` - Hint (placeholder/tooltip)
- `d:` - Default value
- `r:` - Required: `1` = required, omit or `0` = optional (default)
- `o:` - Options for select type (format: `C:Celsius,F:Fahrenheit`)

### 2. Client-Side Config Builder (JavaScript)

The browser is responsible for building the complete config JSON. The server sends the current config as a JS variable at page load.

#### Data sent at page load

```javascript
// Schema map for form generation
var ps = {
  "light1": {"relay": "relay|l:Relay|t:i|h:1-8|d:1"},
  "temperature": {"filter": "filter|l:Filter|h:BME280#Temperature"},
  "airquality": {
    "co2": "co2|l:CO2|h:CO2 filter",
    "no2": "no2|l:NO2|h:NO2 filter",
    "pm1": "pm1|l:PM1|h:PM1 filter"
  }
};

// Current endpoint configuration (the "config" part of _matter_device.json)
var cfg = {
  "2": {"name": "foo", "type": "light3"},
  "4": {"relay": 3, "type": "light1"}
};

// Next endpoint number to allocate
var nextep = 5;
```

#### JavaScript Functions

The browser-side JS includes:

- `parseSchema(s)` - Parse compact schema string (unchanged)
- `genField(name, key, def)` - Generate HTML for a single field (unchanged)
- `genFields(type, containerId)` - Generate all fields for an endpoint type (unchanged)
- `buildConfigJson()` - NEW: Collect all endpoint data from the UI and build the complete config JSON string
- `addEndpoint()` - NEW: Add a new endpoint to `cfg`, allocate next ep number, rebuild UI
- `deleteEndpoint(ep)` - NEW: Remove an endpoint from `cfg`, rebuild UI
- `submitConfig()` - NEW: Build config JSON and submit via hidden form field

#### Form Submission

Instead of individual form fields per endpoint, the form contains a single hidden input:

```html
<form action='/matterc' method='post'>
  <!-- visible UI for editing endpoints -->
  <input type='hidden' name='config_json' id='config_json'>
  <button onclick='submitConfig()' name='config'>Save</button>
</form>
```

When the user clicks Save, `submitConfig()`:
1. Reads all endpoint data from the UI (names, types, args)
2. Builds the complete `cfg` object
3. Serializes to JSON string
4. Sets the hidden input value
5. Submits the form

### 3. Server-Side Processing (Berry)

The server-side handling is greatly simplified. Instead of parsing individual form fields, it receives the complete config JSON.

```berry
class Matter_UI
  # Handle config_json POST parameter
  # The browser sends the entire "config" section as a JSON string
  def handle_config_json()
    import webserver
    import json
    
    var config_json_str = webserver.arg("config_json")
    if config_json_str == nil || config_json_str == ""
      raise "value_error", "Empty configuration"
    end
    
    var new_config = json.load(config_json_str)
    if new_config == nil
      raise "value_error", "Invalid JSON configuration"
    end
    
    # Validate all endpoint types exist
    for ep_str: new_config.keys()
      var conf = new_config[ep_str]
      var typ = conf.find('type')
      if typ == nil
        raise "value_error", "Endpoint " + ep_str + " missing type"
      end
      if self.device.plugins_classes.find(typ) == nil
        raise "value_error", "Unknown type '" + typ + "' for endpoint " + ep_str
      end
    end
    
    # Replace config wholesale
    self.device.plugins_config = new_config
    
    # Recompute nextep from config keys
    self.device.adjust_next_ep()
    
    # Reconcile live plugin instances
    self.reconcile_plugins(new_config)
    
    # Save
    self.device.plugins_persist = true
    self.device.save_param()
  end
  
  # Reconcile live plugin instances with new config
  # - Remove plugins for deleted endpoints
  # - Add plugins for new endpoints
  # - Update plugins for changed endpoints
  def reconcile_plugins(new_config)
    # Build set of current endpoint numbers
    var current_eps = {}
    for pl: self.device.plugins
      current_eps[pl.get_endpoint()] = pl
    end
    
    # Build set of new endpoint numbers
    var new_eps = {}
    for ep_str: new_config.keys()
      new_eps[int(ep_str)] = new_config[ep_str]
    end
    
    # Remove plugins for endpoints no longer in config
    var idx = 0
    while idx < size(self.device.plugins)
      var pl = self.device.plugins[idx]
      var ep = pl.get_endpoint()
      if !new_eps.contains(ep) && ep > 0  # don't remove root/aggregator
        self.device.plugins.remove(idx)
      else
        idx += 1
      end
    end
    
    # Add/update plugins for endpoints in new config
    for ep_str: new_config.keys()
      var ep = int(ep_str)
      var conf = new_config[ep_str]
      var typ = conf.find('type')
      
      if current_eps.contains(ep)
        # Endpoint exists - update its configuration
        var pl = current_eps[ep]
        pl.parse_configuration(conf)
      else
        # New endpoint - instantiate plugin
        var pi_class = self.device.plugins_classes.find(typ)
        if pi_class != nil
          var pi = pi_class(self.device, ep, conf)
          self.device.plugins.push(pi)
        end
      end
    end
    
    self.device.signal_endpoints_changed()
  end
end
```

### 4. Server-Side Schema Collection (Berry)

(Unchanged - `generate_schema_js()` and `collect_plugin_schemas()`)

Additionally, the server now also sends the current config as JavaScript:

```berry
def generate_config_js()
  import webserver
  import json
  
  # Send current config as JS variable
  webserver.content_send("var cfg=")
  webserver.content_send(json.dump(self.device.plugins_config))
  webserver.content_send(";")
  
  # Send nextep for new endpoint allocation
  webserver.content_send(format("var nextep=%i;", self.device.next_ep))
end
```

## Data Models

### Configuration JSON Structure (`_matter_device.json`)

```json
{
  "distinguish": 3498,
  "passcode": 21719991,
  "ipv4only": false,
  "disable_bridge_mode": false,
  "nextep": 4,
  "config": {
    "2": {
      "name": "Living Room Light",
      "type": "light1",
      "relay": 1
    },
    "3": {
      "name": "Temperature Sensor",
      "type": "temperature",
      "filter": "BME280#Temperature"
    },
    "4": {
      "name": "Air Quality",
      "type": "airquality",
      "co2": "SCD40#CarbonDioxide",
      "no2": "SEN55#NO2",
      "pm1": "SEN55#PM1",
      "pm2_5": "SEN55#PM2.5",
      "pm10": "SEN55#PM10",
      "tvoc": "SEN55#TVOC"
    }
  }
}
```

The `nextep` field is always recomputed server-side from the config keys. It is stored for consistency but never trusted from the browser.

### What the browser sends (POST)

```
config_json={"2":{"name":"foo","type":"light3"},"4":{"relay":3,"type":"light1"}}
```

A single form field containing the complete config JSON. The server:
1. Parses with `json.load()`
2. Validates types
3. Replaces `plugins_config`
4. Calls `adjust_next_ep()` to derive `nextep`
5. Reconciles plugins
6. Saves

### Compact Schema Format Reference

```
Format: "arg_name|key:value|key:value|..."

First element: arg name (parameter key) - REQUIRED, no prefix

Short identifiers (key:value format):
  l: - label (display name shown above the field, defaults to arg name)
  t: - type (t=text [DEFAULT], i=int, s=select, c=checkbox)
  h: - hint
  d: - default
  r: - required (1=required, omit or 0=optional)
  o: - options (select only, format: C:Celsius,F:Fahrenheit)
```

## Error Handling

### Client-Side Validation
- HTML5 validation attributes (required)
- Browser prevents form submission if validation fails
- JSON.stringify ensures valid JSON is sent

### Server-Side Validation
- `json.load()` returns nil for invalid JSON
- All endpoint types validated against `plugins_classes`
- Missing `type` field detected and rejected

## Testing Strategy

### Property-Based Tests

1. **Schema Structure Validity**: Any parsed schema has valid type and appropriate attributes
2. **Form Generation Completeness**: Generated HTML contains correct elements for schema
3. **Configuration Round-Trip**: Browser builds JSON, server parses and saves, reload shows same config
4. **Validation Enforcement**: Invalid types or malformed JSON are rejected server-side
