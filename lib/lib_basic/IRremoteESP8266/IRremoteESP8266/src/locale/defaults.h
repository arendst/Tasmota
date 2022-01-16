// Copyright 2019 - David Conran (@crankyoldgit)
// The default text to use throughout the library.
// The library will use this text if no locale (_IR_LOCALE_) is set or if
// the locale doesn't define particular values.
// If they are defined, this file should NOT override them.
//
// This file should contain a #define for every translateable/locale dependant
// string used by the library. Language specific files don't have to include
// everything.
//
// NOTE: ASCII/UTF-8 characters only. Unicode is NOT supported.
//
// The defaults are English (AU) / en-AU. Australia (AU) is pretty much the same
// as English (UK) for this libraries use case.
#ifndef LOCALE_DEFAULTS_H_
#define LOCALE_DEFAULTS_H_

#ifndef D_STR_UNKNOWN
#define D_STR_UNKNOWN "UNKNOWN"
#endif  // D_STR_UNKNOWN
#ifndef D_STR_PROTOCOL
#define D_STR_PROTOCOL "Protocol"
#endif  // D_STR_PROTOCOL
#ifndef D_STR_POWER
#define D_STR_POWER "Power"
#endif  // D_STR_POWER
#ifndef D_STR_PREVIOUS
#define D_STR_PREVIOUS "Previous"
#endif  // D_STR_PREVIOUS
#ifndef D_STR_ON
#define D_STR_ON "On"
#endif  // D_STR_ON
#ifndef D_STR_1
#define D_STR_1 "1"
#endif  // D_STR_1
#ifndef D_STR_OFF
#define D_STR_OFF "Off"
#endif  // D_STR_OFF
#ifndef D_STR_0
#define D_STR_0 "0"
#endif  // D_STR_0
#ifndef D_STR_MODE
#define D_STR_MODE "Mode"
#endif  // D_STR_MODE
#ifndef D_STR_TOGGLE
#define D_STR_TOGGLE "Toggle"
#endif  // D_STR_TOGGLE
#ifndef D_STR_TURBO
#define D_STR_TURBO "Turbo"
#endif  // D_STR_TURBO
#ifndef D_STR_SUPER
#define D_STR_SUPER "Super"
#endif  // D_STR_SUPER
#ifndef D_STR_SLEEP
#define D_STR_SLEEP "Sleep"
#endif  // D_STR_SLEEP
#ifndef D_STR_LIGHT
#define D_STR_LIGHT "Light"
#endif  // D_STR_LIGHT
#ifndef D_STR_POWERFUL
#define D_STR_POWERFUL "Powerful"
#endif  // D_STR_POWERFUL
#ifndef D_STR_QUIET
#define D_STR_QUIET "Quiet"
#endif  // D_STR_QUIET
#ifndef D_STR_ECONO
#define D_STR_ECONO "Econo"
#endif  // D_STR_ECONO
#ifndef D_STR_SWING
#define D_STR_SWING "Swing"
#endif  // D_STR_SWING
#ifndef D_STR_SWINGH
#define D_STR_SWINGH D_STR_SWING"(H)"  // Set `D_STR_SWING` first!
#endif  // D_STR_SWINGH
#ifndef D_STR_SWINGV
#define D_STR_SWINGV D_STR_SWING"(V)"  // Set `D_STR_SWING` first!
#endif  // D_STR_SWINGV
#ifndef D_STR_BEEP
#define D_STR_BEEP "Beep"
#endif  // D_STR_BEEP
#ifndef D_STR_MOULD
#define D_STR_MOULD "Mould"
#endif  // D_STR_MOULD
#ifndef D_STR_CLEAN
#define D_STR_CLEAN "Clean"
#endif  // D_STR_CLEAN
#ifndef D_STR_PURIFY
#define D_STR_PURIFY "Purify"
#endif  // D_STR_PURIFY
#ifndef D_STR_TIMER
#define D_STR_TIMER "Timer"
#endif  // D_STR_TIMER
#ifndef D_STR_ONTIMER
#define D_STR_ONTIMER D_STR_ON " " D_STR_TIMER  // Set `D_STR_ON` first!
#endif  // D_STR_ONTIMER
#ifndef D_STR_OFFTIMER
#define D_STR_OFFTIMER D_STR_OFF " " D_STR_TIMER  // Set `D_STR_OFF` first!
#endif  // D_STR_OFFTIMER
#ifndef D_STR_TIMERMODE
#define D_STR_TIMERMODE D_STR_TIMER " " D_STR_MODE  // Set `D_STR_MODE` first!
#endif  // D_STR_TIMERMODE
#ifndef D_STR_CLOCK
#define D_STR_CLOCK "Clock"
#endif  // D_STR_CLOCK
#ifndef D_STR_COMMAND
#define D_STR_COMMAND "Command"
#endif  // D_STR_COMMAND
#ifndef D_STR_XFAN
#define D_STR_XFAN "XFan"
#endif  // D_STR_XFAN
#ifndef D_STR_HEALTH
#define D_STR_HEALTH "Health"
#endif  // D_STR_HEALTH
#ifndef D_STR_MODEL
#define D_STR_MODEL "Model"
#endif  // D_STR_MODEL
#ifndef D_STR_TEMP
#define D_STR_TEMP "Temp"
#endif  // D_STR_TEMP
#ifndef D_STR_IFEEL
#define D_STR_IFEEL "IFeel"
#endif  // D_STR_IFEEL
#ifndef D_STR_ISEE
#define D_STR_ISEE "ISee"
#endif  // D_STR_ISEE
#ifndef D_STR_HUMID
#define D_STR_HUMID "Humid"
#endif  // D_STR_HUMID
#ifndef D_STR_SAVE
#define D_STR_SAVE "Save"
#endif  // D_STR_SAVE
#ifndef D_STR_EYE
#define D_STR_EYE "Eye"
#endif  // D_STR_EYE
#ifndef D_STR_FOLLOW
#define D_STR_FOLLOW "Follow"
#endif  // D_STR_FOLLOW
#ifndef D_STR_ION
#define D_STR_ION "Ion"
#endif  // D_STR_ION
#ifndef D_STR_FRESH
#define D_STR_FRESH "Fresh"
#endif  // D_STR_FRESH
#ifndef D_STR_HOLD
#define D_STR_HOLD "Hold"
#endif  // D_STR_HOLD
#ifndef D_STR_8C_HEAT
#define D_STR_8C_HEAT "8C " D_STR_HEAT  // Set `D_STR_HEAT` first!
#endif  // D_STR_8C_HEAT
#ifndef D_STR_10C_HEAT
#define D_STR_10C_HEAT "10C " D_STR_HEAT  // Set `D_STR_HEAT` first!
#endif  // D_STR_10C_HEAT
#ifndef D_STR_BUTTON
#define D_STR_BUTTON "Button"
#endif  // D_STR_BUTTON
#ifndef D_STR_NIGHT
#define D_STR_NIGHT "Night"
#endif  // D_STR_NIGHT
#ifndef D_STR_SILENT
#define D_STR_SILENT "Silent"
#endif  // D_STR_SILENT
#ifndef D_STR_FILTER
#define D_STR_FILTER "Filter"
#endif  // D_STR_FILTER
#ifndef D_STR_3D
#define D_STR_3D "3D"
#endif  // D_STR_3D
#ifndef D_STR_CELSIUS
#define D_STR_CELSIUS "Celsius"
#endif  // D_STR_CELSIUS
#ifndef D_STR_FAHRENHEIT
#define D_STR_FAHRENHEIT "Fahrenheit"
#endif  // D_STR_FAHRENHEIT
#ifndef D_STR_CELSIUS_FAHRENHEIT
#define D_STR_CELSIUS_FAHRENHEIT D_STR_CELSIUS "/" D_STR_FAHRENHEIT
#endif  // D_STR_CELSIUS_FAHRENHEIT
#ifndef D_STR_UP
#define D_STR_UP "Up"
#endif  // D_STR_UP
#ifndef D_STR_TEMPUP
#define D_STR_TEMPUP D_STR_TEMP " " D_STR_UP  // Set `D_STR_TEMP` first!
#endif  // D_STR_TEMPUP
#ifndef D_STR_DOWN
#define D_STR_DOWN "Down"
#endif  // D_STR_DOWN
#ifndef D_STR_TEMPDOWN
#define D_STR_TEMPDOWN D_STR_TEMP " " D_STR_DOWN  // Set `D_STR_TEMP` first!
#endif  // D_STR_TEMPDOWN
#ifndef D_STR_CHANGE
#define D_STR_CHANGE "Change"
#endif  // D_STR_CHANGE
#ifndef D_STR_START
#define D_STR_START "Start"
#endif  // D_STR_START
#ifndef D_STR_STOP
#define D_STR_STOP "Stop"
#endif  // D_STR_STOP
#ifndef D_STR_MOVE
#define D_STR_MOVE "Move"
#endif  // D_STR_MOVE
#ifndef D_STR_SET
#define D_STR_SET "Set"
#endif  // D_STR_SET
#ifndef D_STR_CANCEL
#define D_STR_CANCEL "Cancel"
#endif  // D_STR_CANCEL
#ifndef D_STR_COMFORT
#define D_STR_COMFORT "Comfort"
#endif  // D_STR_COMFORT
#ifndef D_STR_SENSOR
#define D_STR_SENSOR "Sensor"
#endif  // D_STR_SENSOR
#ifndef D_STR_ABSENSEDETECT
#define D_STR_ABSENSEDETECT "Absense detect"
#endif  // D_STR_ABSENSEDETECT
#ifndef D_STR_DIRECT
#define D_STR_DIRECT "Direct"
#endif  // D_STR_DIRECT
#ifndef D_STR_INDIRECT
#define D_STR_INDIRECT "Indirect"
#endif  // D_STR_INDIRECT
#ifndef D_STR_DIRECTINDIRECTMODE
#define D_STR_DIRECTINDIRECTMODE D_STR_DIRECT " / " \
D_STR_INDIRECT " " D_STR_MODE
#endif  // D_STR_DIRECTINDIRECTMODE
#ifndef D_STR_DISPLAY
#define D_STR_DISPLAY "Display"
#endif  // D_STR_DISPLAY
#ifndef D_STR_WEEKLY
#define D_STR_WEEKLY "Weekly"
#endif  // D_STR_WEEKLY
#ifndef D_STR_WEEKLYTIMER
#define D_STR_WEEKLYTIMER D_STR_WEEKLY " " D_STR_TIMER  // Needs `D_STR_WEEKLY`!
#endif  // D_STR_WEEKLYTIMER
#ifndef D_STR_WIFI
#define D_STR_WIFI "WiFi"
#endif  // D_STR_WIFI
#ifndef D_STR_LAST
#define D_STR_LAST "Last"
#endif  // D_STR_LAST
#ifndef D_STR_FAST
#define D_STR_FAST "Fast"
#endif  // D_STR_FAST
#ifndef D_STR_SLOW
#define D_STR_SLOW "Slow"
#endif  // D_STR_SLOW
#ifndef D_STR_AIRFLOW
#define D_STR_AIRFLOW "Air Flow"
#endif  // D_STR_AIRFLOW
#ifndef D_STR_STEP
#define D_STR_STEP "Step"
#endif  // D_STR_STEP
#ifndef D_STR_NA
#define D_STR_NA "N/A"
#endif  // D_STR_NA
#ifndef D_STR_INSIDE
#define D_STR_INSIDE "Inside"
#endif  // D_STR_INSIDE
#ifndef D_STR_OUTSIDE
#define D_STR_OUTSIDE "Outside"
#endif  // D_STR_OUTSIDE
#ifndef D_STR_LOUD
#define D_STR_LOUD "Loud"
#endif  // D_STR_LOUD
#ifndef D_STR_UPPER
#define D_STR_UPPER "Upper"
#endif  // D_STR_UPPER
#ifndef D_STR_LOWER
#define D_STR_LOWER "Lower"
#endif  // D_STR_LOWER
#ifndef D_STR_BREEZE
#define D_STR_BREEZE "Breeze"
#endif  // D_STR_BREEZE
#ifndef D_STR_CIRCULATE
#define D_STR_CIRCULATE "Circulate"
#endif  // D_STR_CIRCULATE
#ifndef D_STR_CEILING
#define D_STR_CEILING "Ceiling"
#endif  // D_STR_CEILING
#ifndef D_STR_WALL
#define D_STR_WALL "Wall"
#endif  // D_STR_WALL
#ifndef D_STR_ROOM
#define D_STR_ROOM "Room"
#endif  // D_STR_ROOM
#ifndef D_STR_6THSENSE
#define D_STR_6THSENSE "6th Sense"
#endif  // D_STR_6THSENSE
#ifndef D_STR_ZONEFOLLOW
#define D_STR_ZONEFOLLOW "Zone Follow"
#endif  // D_STR_ZONEFOLLOW
#ifndef D_STR_FIXED
#define D_STR_FIXED "Fixed"
#endif  // D_STR_FIXED
#ifndef D_STR_TYPE
#define D_STR_TYPE "Type"
#endif  // D_STR_TYPE
#ifndef D_STR_SPECIAL
#define D_STR_SPECIAL "Special"
#endif  // D_STR_SPECIAL
#ifndef D_STR_RECYCLE
#define D_STR_RECYCLE "Recycle"
#endif  // D_STR_RECYCLE
#ifndef D_STR_ID
#define D_STR_ID "Id"
#endif  // D_STR_ID
#ifndef D_STR_VANE
#define D_STR_VANE "Vane"
#endif  // D_STR_VANE
#ifndef D_STR_LOCK
#define D_STR_LOCK "Lock"
#endif  // D_STR_LOCK

#ifndef D_STR_AUTO
#define D_STR_AUTO "Auto"
#endif  // D_STR_AUTO
#ifndef D_STR_AUTOMATIC
#define D_STR_AUTOMATIC "Automatic"
#endif  // D_STR_AUTOMATIC
#ifndef D_STR_MANUAL
#define D_STR_MANUAL "Manual"
#endif  // D_STR_MANUAL
#ifndef D_STR_COOL
#define D_STR_COOL "Cool"
#endif  // D_STR_COOL
#ifndef D_STR_COOLING
#define D_STR_COOLING "Cooling"
#endif  // D_STR_COOLING
#ifndef D_STR_HEAT
#define D_STR_HEAT "Heat"
#endif  // D_STR_HEAT
#ifndef D_STR_HEATING
#define D_STR_HEATING "Heating"
#endif  // D_STR_HEATING
#ifndef D_STR_FAN
#define D_STR_FAN "Fan"
#endif  // D_STR_FAN
#ifndef D_STR_FANONLY
#define D_STR_FANONLY "fan-only"
#endif  // D_STR_FANONLY
#ifndef D_STR_FAN_ONLY
#define D_STR_FAN_ONLY "fan_only"
#endif  // D_STR_FAN_ONLY
#ifndef D_STR_ONLY
#define D_STR_ONLY "Only"
#endif  // D_STR_ONLY
#ifndef D_STR_FANSPACEONLY
#define D_STR_FANSPACEONLY D_STR_FAN " " D_STR_ONLY
#endif  // D_STR_FANSPACEONLY
#ifndef D_STR_FANONLYNOSPACE
#define D_STR_FANONLYNOSPACE D_STR_FAN D_STR_ONLY
#endif  // D_STR_FANONLYNOSPACE
#ifndef D_STR_DRY
#define D_STR_DRY "Dry"
#endif  // D_STR_DRY
#ifndef D_STR_DRYING
#define D_STR_DRYING "Drying"
#endif  // D_STR_DRYING
#ifndef D_STR_DEHUMIDIFY
#define D_STR_DEHUMIDIFY "Dehumidify"
#endif  // D_STR_DEHUMIDIFY

#ifndef D_STR_MAX
#define D_STR_MAX "Max"
#endif  // D_STR_MAX
#ifndef D_STR_MAXIMUM
#define D_STR_MAXIMUM "Maximum"
#endif  // D_STR_MAXIMUM
#ifndef D_STR_MIN
#define D_STR_MIN "Min"
#endif  // D_STR_MIN
#ifndef D_STR_MINIMUM
#define D_STR_MINIMUM "Minimum"
#endif  // D_STR_MINIMUM
#ifndef D_STR_MED
#define D_STR_MED "Med"
#endif  // D_STR_MED
#ifndef D_STR_MEDIUM
#define D_STR_MEDIUM "Medium"
#endif  // D_STR_MEDIUM

#ifndef D_STR_HIGHEST
#define D_STR_HIGHEST "Highest"
#endif  // D_STR_HIGHEST
#ifndef D_STR_HIGH
#define D_STR_HIGH "High"
#endif  // D_STR_HIGH
#ifndef D_STR_HI
#define D_STR_HI "Hi"
#endif  // D_STR_HI
#ifndef D_STR_MID
#define D_STR_MID "Mid"
#endif  // D_STR_MID
#ifndef D_STR_MIDDLE
#define D_STR_MIDDLE "Middle"
#endif  // D_STR_MIDDLE
#ifndef D_STR_LOW
#define D_STR_LOW "Low"
#endif  // D_STR_LOW
#ifndef D_STR_LO
#define D_STR_LO "Lo"
#endif  // D_STR_LO
#ifndef D_STR_LOWEST
#define D_STR_LOWEST "Lowest"
#endif  // D_STR_LOWEST
#ifndef D_STR_RIGHT
#define D_STR_RIGHT "Right"
#endif  // D_STR_RIGHT
#ifndef D_STR_MAXRIGHT
#define D_STR_MAXRIGHT D_STR_MAX " " D_STR_RIGHT  // Set `D_STR_MAX` first!
#endif  // D_STR_MAXRIGHT
#ifndef D_STR_MAXRIGHT_NOSPACE
#define D_STR_MAXRIGHT_NOSPACE D_STR_MAX D_STR_RIGHT  // Set `D_STR_MAX` first!
#endif  // D_STR_MAXRIGHT_NOSPACE
#ifndef D_STR_RIGHTMAX
#define D_STR_RIGHTMAX D_STR_RIGHT " " D_STR_MAX  // Set `D_STR_MAX` first!
#endif  // D_STR_RIGHTMAX
#ifndef D_STR_RIGHTMAX_NOSPACE
#define D_STR_RIGHTMAX_NOSPACE D_STR_RIGHT D_STR_MAX  // Set `D_STR_MAX` first!
#endif  // D_STR_RIGHTMAX_NOSPACE
#ifndef D_STR_LEFT
#define D_STR_LEFT "Left"
#endif  // D_STR_LEFT
#ifndef D_STR_MAXLEFT
#define D_STR_MAXLEFT D_STR_MAX " " D_STR_LEFT  // Set `D_STR_MAX` first!
#endif  // D_STR_MAXLEFT
#ifndef D_STR_MAXLEFT_NOSPACE
#define D_STR_MAXLEFT_NOSPACE D_STR_MAX D_STR_LEFT  // Set `D_STR_MAX` first!
#endif  // D_STR_MAXLEFT_NOSPACE
#ifndef D_STR_LEFTMAX
#define D_STR_LEFTMAX D_STR_LEFT " " D_STR_MAX  // Set `D_STR_MAX` first!
#endif  // D_STR_LEFTMAX
#ifndef D_STR_LEFTMAX_NOSPACE
#define D_STR_LEFTMAX_NOSPACE D_STR_LEFT D_STR_MAX  // Set `D_STR_MAX` first!
#endif  // D_STR_LEFTMAX_NOSPACE
#ifndef D_STR_WIDE
#define D_STR_WIDE "Wide"
#endif  // D_STR_WIDE
#ifndef D_STR_CENTRE
#define D_STR_CENTRE "Centre"
#endif  // D_STR_CENTRE
#ifndef D_STR_TOP
#define D_STR_TOP "Top"
#endif  // D_STR_TOP
#ifndef D_STR_BOTTOM
#define D_STR_BOTTOM "Bottom"
#endif  // D_STR_BOTTOM

// Compound words/phrases/descriptions from pre-defined words.
// Note: Obviously these need to be defined *after* their component words.
#ifndef D_STR_ECONOTOGGLE
#define D_STR_ECONOTOGGLE D_STR_ECONO " " D_STR_TOGGLE
#endif  // D_STR_ECONOTOGGLE
#ifndef D_STR_EYEAUTO
#define D_STR_EYEAUTO D_STR_EYE " " D_STR_AUTO
#endif  // D_STR_EYEAUTO
#ifndef D_STR_LIGHTTOGGLE
#define D_STR_LIGHTTOGGLE D_STR_LIGHT " " D_STR_TOGGLE
#endif  // D_STR_LIGHTTOGGLE
#ifndef D_STR_OUTSIDEQUIET
#define D_STR_OUTSIDEQUIET D_STR_OUTSIDE " " D_STR_QUIET
#endif  // D_STR_OUTSIDEQUIET
#ifndef D_STR_POWERTOGGLE
#define D_STR_POWERTOGGLE D_STR_POWER " " D_STR_TOGGLE
#endif  // D_STR_POWERTOGGLE
#ifndef D_STR_POWERBUTTON
#define D_STR_POWERBUTTON D_STR_POWER " " D_STR_BUTTON
#endif  // D_STR_POWERBUTTON
#ifndef D_STR_PREVIOUSPOWER
#define D_STR_PREVIOUSPOWER D_STR_PREVIOUS " " D_STR_POWER
#endif  // D_STR_PREVIOUSPOWER
#ifndef D_STR_DISPLAYTEMP
#define D_STR_DISPLAYTEMP D_STR_DISPLAY " " D_STR_TEMP
#endif  // D_STR_DISPLAYTEMP
#ifndef D_STR_SENSORTEMP
#define D_STR_SENSORTEMP D_STR_SENSOR " " D_STR_TEMP
#endif  // D_STR_SENSORTEMP
#ifndef D_STR_SLEEP_TIMER
#define D_STR_SLEEP_TIMER D_STR_SLEEP " " D_STR_TIMER
#endif  // D_STR_SLEEP_TIMER
#ifndef D_STR_SWINGVMODE
#define D_STR_SWINGVMODE D_STR_SWINGV " " D_STR_MODE
#endif  // D_STR_SWINGVMODE
#ifndef D_STR_SWINGVTOGGLE
#define D_STR_SWINGVTOGGLE D_STR_SWINGV " " D_STR_TOGGLE
#endif  // D_STR_SWINGVTOGGLE
#ifndef D_STR_TURBOTOGGLE
#define D_STR_TURBOTOGGLE D_STR_TURBO " " D_STR_TOGGLE
#endif  // D_STR_TURBOTOGGLE

// Separators
#ifndef D_CHR_TIME_SEP
#define D_CHR_TIME_SEP ':'
#endif  // D_CHR_TIME_SEP
#ifndef D_STR_SPACELBRACE
#define D_STR_SPACELBRACE " ("
#endif  // D_STR_SPACELBRACE
#ifndef D_STR_COMMASPACE
#define D_STR_COMMASPACE ", "
#endif  // D_STR_COMMASPACE
#ifndef D_STR_COLONSPACE
#define D_STR_COLONSPACE ": "
#endif  // D_STR_COLONSPACE
#ifndef D_STR_DASH
#define D_STR_DASH "-"
#endif  // D_STR_DASH

#ifndef D_STR_DAY
#define D_STR_DAY "Day"
#endif  // D_STR_DAY
#ifndef D_STR_DAYS
#define D_STR_DAYS D_STR_DAY "s"
#endif  // D_STR_DAYS
#ifndef D_STR_HOUR
#define D_STR_HOUR "Hour"
#endif  // D_STR_HOUR
#ifndef D_STR_HOURS
#define D_STR_HOURS D_STR_HOUR "s"
#endif  // D_STR_HOURS
#ifndef D_STR_MINUTE
#define D_STR_MINUTE "Minute"
#endif  // D_STR_MINUTE
#ifndef D_STR_MINUTES
#define D_STR_MINUTES D_STR_MINUTE "s"
#endif  // D_STR_MINUTES
#ifndef D_STR_SECOND
#define D_STR_SECOND "Second"
#endif  // D_STR_SECOND
#ifndef D_STR_SECONDS
#define D_STR_SECONDS D_STR_SECOND "s"
#endif  // D_STR_SECONDS
#ifndef D_STR_NOW
#define D_STR_NOW "Now"
#endif  // D_STR_NOW
#ifndef D_STR_THREELETTERDAYS
#define D_STR_THREELETTERDAYS "SunMonTueWedThuFriSat"
#endif  // D_STR_THREELETTERDAYS

#ifndef D_STR_YES
#define D_STR_YES "Yes"
#endif  // D_STR_YES
#ifndef D_STR_NO
#define D_STR_NO "No"
#endif  // D_STR_NO
#ifndef D_STR_TRUE
#define D_STR_TRUE "True"
#endif  // D_STR_TRUE
#ifndef D_STR_FALSE
#define D_STR_FALSE "False"
#endif  // D_STR_FALSE

#ifndef D_STR_REPEAT
#define D_STR_REPEAT "Repeat"
#endif  // D_STR_REPEAT
#ifndef D_STR_CODE
#define D_STR_CODE "Code"
#endif  // D_STR_CODE
#ifndef D_STR_BITS
#define D_STR_BITS "Bits"
#endif  // D_STR_BITS

// Model Names
#ifndef D_STR_YAW1F
#define D_STR_YAW1F "YAW1F"
#endif  // D_STR_YAW1F
#ifndef D_STR_YBOFB
#define D_STR_YBOFB "YBOFB"
#endif  // D_STR_YBOFB
#ifndef D_STR_V9014557_A
#define D_STR_V9014557_A "V9014557-A"
#endif  // D_STR_V9014557_A
#ifndef D_STR_V9014557_B
#define D_STR_V9014557_B "V9014557-B"
#endif  // D_STR_V9014557_B
#ifndef D_STR_RLT0541HTA_A
#define D_STR_RLT0541HTA_A "R-LT0541-HTA-A"
#endif  // D_STR_RLT0541HTA_A
#ifndef D_STR_RLT0541HTA_B
#define D_STR_RLT0541HTA_B "R-LT0541-HTA-B"
#endif  // D_STR_RLT0541HTA_B
#ifndef D_STR_ARRAH2E
#define D_STR_ARRAH2E "ARRAH2E"
#endif  // D_STR_ARRAH2E
#ifndef D_STR_ARDB1
#define D_STR_ARDB1 "ARDB1"
#endif  // D_STR_ARDB1
#ifndef D_STR_ARREB1E
#define D_STR_ARREB1E "ARREB1E"
#endif  // D_STR_ARREB1E
#ifndef D_STR_ARJW2
#define D_STR_ARJW2 "ARJW2"
#endif  // D_STR_ARJW2
#ifndef D_STR_ARRY4
#define D_STR_ARRY4 "ARRY4"
#endif  // D_STR_ARRY4
#ifndef D_STR_ARREW4E
#define D_STR_ARREW4E "ARREW4E"
#endif  // D_STR_ARREW4E
#ifndef D_STR_GE6711AR2853M
#define D_STR_GE6711AR2853M "GE6711AR2853M"
#endif  // D_STR_GE6711AR2853M
#ifndef D_STR_AKB75215403
#define D_STR_AKB75215403 "AKB75215403"
#endif  // D_STR_AKB75215403
#ifndef D_STR_AKB74955603
#define D_STR_AKB74955603 "AKB74955603"
#endif  // D_STR_AKB74955603
#ifndef D_STR_AKB73757604
#define D_STR_AKB73757604 "AKB73757604"
#endif  // D_STR_AKB73757604
#ifndef D_STR_KKG9AC1
#define D_STR_KKG9AC1 "KKG9AC1"
#endif  // D_STR_KKG9AC1
#ifndef D_STR_KKG29AC1
#define D_STR_KKG29AC1 "KKG29AC1"
#endif  // D_STR_KKG9AC1
#ifndef D_STR_LKE
#define D_STR_LKE "LKE"
#endif  // D_STR_LKE
#ifndef D_STR_NKE
#define D_STR_NKE "NKE"
#endif  // D_STR_NKE
#ifndef D_STR_DKE
#define D_STR_DKE "DKE"
#endif  // D_STR_DKE
#ifndef D_STR_PKR
#define D_STR_PKR "PKR"
#endif  // D_STR_PKR
#ifndef D_STR_JKE
#define D_STR_JKE "JKE"
#endif  // D_STR_JKE
#ifndef D_STR_CKP
#define D_STR_CKP "CKP"
#endif  // D_STR_CKP
#ifndef D_STR_RKR
#define D_STR_RKR "RKR"
#endif  // D_STR_RKR
#ifndef D_STR_PANASONICLKE
#define D_STR_PANASONICLKE "PANASONICLKE"
#endif  // D_STR_PANASONICLKE
#ifndef D_STR_PANASONICNKE
#define D_STR_PANASONICNKE "PANASONICNKE"
#endif  // D_STR_PANASONICNKE
#ifndef D_STR_PANASONICDKE
#define D_STR_PANASONICDKE "PANASONICDKE"
#endif  // D_STR_PANASONICDKE
#ifndef D_STR_PANASONICPKR
#define D_STR_PANASONICPKR "PANASONICPKR"
#endif  // D_STR_PANASONICPKR
#ifndef D_STR_PANASONICJKE
#define D_STR_PANASONICJKE "PANASONICJKE"
#endif  // D_STR_PANASONICJKE
#ifndef D_STR_PANASONICCKP
#define D_STR_PANASONICCKP "PANASONICCKP"
#endif  // D_STR_PANASONICCKP
#ifndef D_STR_PANASONICRKR
#define D_STR_PANASONICRKR "PANASONICRKR"
#endif  // D_STR_PANASONICRKR
#ifndef D_STR_A907
#define D_STR_A907 "A907"
#endif  // D_STR_A907
#ifndef D_STR_A705
#define D_STR_A705 "A705"
#endif  // D_STR_A705
#ifndef D_STR_A903
#define D_STR_A903 "A903"
#endif  // D_STR_A903
#ifndef D_STR_TAC09CHSD
#define D_STR_TAC09CHSD "TAC09CHSD"
#endif  // D_STR_TAC09CHSD
#ifndef D_STR_GZ055BE1
#define D_STR_GZ055BE1 "GZ055BE1"
#endif  // D_STR_GZ055BE1
#ifndef D_STR_122LZF
#define D_STR_122LZF "122LZF"
#endif  // D_STR_122LZF
#ifndef D_STR_DG11J13A
#define D_STR_DG11J13A "DG11J13A"
#endif  // D_STR_DG11J13A
#ifndef D_STR_DG11J104
#define D_STR_DG11J104 "DG11J104"
#endif  // D_STR_DG11J104
#ifndef D_STR_DG11J191
#define D_STR_DG11J191 "DG11J191"
#endif  // D_STR_DG11J191

// Protocols Names
#ifndef D_STR_AIRTON
#define D_STR_AIRTON "AIRTON"
#endif  // D_STR_AIRTON
#ifndef D_STR_AIRWELL
#define D_STR_AIRWELL "AIRWELL"
#endif  // D_STR_AIRWELL
#ifndef D_STR_AIWA_RC_T501
#define D_STR_AIWA_RC_T501 "AIWA_RC_T501"
#endif  // D_STR_AIWA_RC_T501
#ifndef D_STR_AMCOR
#define D_STR_AMCOR "AMCOR"
#endif  // D_STR_AMCOR
#ifndef D_STR_ARGO
#define D_STR_ARGO "ARGO"
#endif  // D_STR_ARGO
#ifndef D_STR_ARRIS
#define D_STR_ARRIS "ARRIS"
#endif  // D_STR_ARRIS
#ifndef D_STR_BOSE
#define D_STR_BOSE "BOSE"
#endif  // D_STR_BOSE
#ifndef D_STR_CARRIER_AC
#define D_STR_CARRIER_AC "CARRIER_AC"
#endif  // D_STR_CARRIER_AC
#ifndef D_STR_CARRIER_AC40
#define D_STR_CARRIER_AC40 D_STR_CARRIER_AC "40"
#endif  // D_STR_CARRIER_AC40
#ifndef D_STR_CARRIER_AC64
#define D_STR_CARRIER_AC64 D_STR_CARRIER_AC "64"
#endif  // D_STR_CARRIER_AC64
#ifndef D_STR_COOLIX
#define D_STR_COOLIX "COOLIX"
#endif  // D_STR_COOLIX
#ifndef D_STR_COOLIX48
#define D_STR_COOLIX48 D_STR_COOLIX "48"
#endif  // D_STR_COOLIX48
#ifndef D_STR_CORONA_AC
#define D_STR_CORONA_AC "CORONA_AC"
#endif  // D_STR_CORONA_AC
#ifndef D_STR_DAIKIN
#define D_STR_DAIKIN "DAIKIN"
#endif  // D_STR_DAIKIN
#ifndef D_STR_DAIKIN128
#define D_STR_DAIKIN128 "DAIKIN128"
#endif  // D_STR_DAIKIN128
#ifndef D_STR_DAIKIN152
#define D_STR_DAIKIN152 "DAIKIN152"
#endif  // D_STR_DAIKIN152
#ifndef D_STR_DAIKIN160
#define D_STR_DAIKIN160 "DAIKIN160"
#endif  // D_STR_DAIKIN160
#ifndef D_STR_DAIKIN176
#define D_STR_DAIKIN176 "DAIKIN176"
#endif  // D_STR_DAIKIN176
#ifndef D_STR_DAIKIN2
#define D_STR_DAIKIN2 "DAIKIN2"
#endif  // D_STR_DAIKIN2
#ifndef D_STR_DAIKIN216
#define D_STR_DAIKIN216 "DAIKIN216"
#endif  // D_STR_DAIKIN216
#ifndef D_STR_DAIKIN64
#define D_STR_DAIKIN64 "DAIKIN64"
#endif  // D_STR_DAIKIN64
#ifndef D_STR_DELONGHI_AC
#define D_STR_DELONGHI_AC "DELONGHI_AC"
#endif  // D_STR_DELONGHI_AC
#ifndef D_STR_DENON
#define D_STR_DENON "DENON"
#endif  // D_STR_DENON
#ifndef D_STR_DISH
#define D_STR_DISH "DISH"
#endif  // D_STR_DISH
#ifndef D_STR_DOSHISHA
#define D_STR_DOSHISHA "DOSHISHA"
#endif  // D_STR_DOSHISHA
#ifndef D_STR_ECOCLIM
#define D_STR_ECOCLIM "ECOCLIM"
#endif  // D_STR_ECOCLIM
#ifndef D_STR_ELECTRA_AC
#define D_STR_ELECTRA_AC "ELECTRA_AC"
#endif  // D_STR_ELECTRA_AC
#ifndef D_STR_ELITESCREENS
#define D_STR_ELITESCREENS "ELITESCREENS"
#endif  // D_STR_ELITESCREENS
#ifndef D_STR_EPSON
#define D_STR_EPSON "EPSON"
#endif  // D_STR_EPSON
#ifndef D_STR_FUJITSU_AC
#define D_STR_FUJITSU_AC "FUJITSU_AC"
#endif  // D_STR_FUJITSU_AC
#ifndef D_STR_GICABLE
#define D_STR_GICABLE "GICABLE"
#endif  // D_STR_GICABLE
#ifndef D_STR_GLOBALCACHE
#define D_STR_GLOBALCACHE "GLOBALCACHE"
#endif  // D_STR_GLOBALCACHE
#ifndef D_STR_GOODWEATHER
#define D_STR_GOODWEATHER "GOODWEATHER"
#endif  // D_STR_GOODWEATHER
#ifndef D_STR_GREE
#define D_STR_GREE "GREE"
#endif  // D_STR_GREE
#ifndef D_STR_HAIER_AC
#define D_STR_HAIER_AC "HAIER_AC"
#endif  // D_STR_HAIER_AC
#ifndef D_STR_HAIER_AC_YRW02
#define D_STR_HAIER_AC_YRW02 "HAIER_AC_YRW02"
#endif  // D_STR_HAIER_AC_YRW02
#ifndef D_STR_HAIER_AC176
#define D_STR_HAIER_AC176 "HAIER_AC176"
#endif  // D_STR_HAIER_AC176
#ifndef D_STR_HITACHI_AC
#define D_STR_HITACHI_AC "HITACHI_AC"
#endif  // D_STR_HITACHI_AC
#ifndef D_STR_HITACHI_AC1
#define D_STR_HITACHI_AC1 "HITACHI_AC1"
#endif  // D_STR_HITACHI_AC1
#ifndef D_STR_HITACHI_AC2
#define D_STR_HITACHI_AC2 "HITACHI_AC2"
#endif  // D_STR_HITACHI_AC2
#ifndef D_STR_HITACHI_AC3
#define D_STR_HITACHI_AC3 "HITACHI_AC3"
#endif  // D_STR_HITACHI_AC3
#ifndef D_STR_HITACHI_AC344
#define D_STR_HITACHI_AC344 "HITACHI_AC344"
#endif  // D_STR_HITACHI_AC344
#ifndef D_STR_HITACHI_AC424
#define D_STR_HITACHI_AC424 "HITACHI_AC424"
#endif  // D_STR_HITACHI_AC424
#ifndef D_STR_INAX
#define D_STR_INAX "INAX"
#endif  // D_STR_INAX
#ifndef D_STR_JVC
#define D_STR_JVC "JVC"
#endif  // D_STR_JVC
#ifndef D_STR_KELON
#define D_STR_KELON "KELON"
#endif  // D_STR_KELON
#ifndef D_STR_KELVINATOR
#define D_STR_KELVINATOR "KELVINATOR"
#endif  // D_STR_KELVINATOR
#ifndef D_STR_LASERTAG
#define D_STR_LASERTAG "LASERTAG"
#endif  // D_STR_LASERTAG
#ifndef D_STR_LEGOPF
#define D_STR_LEGOPF "LEGOPF"
#endif  // D_STR_LEGOPF
#ifndef D_STR_LG
#define D_STR_LG "LG"
#endif  // D_STR_LG
#ifndef D_STR_LG2
#define D_STR_LG2 "LG2"
#endif  // D_STR_LG2
#ifndef D_STR_LUTRON
#define D_STR_LUTRON "LUTRON"
#endif  // D_STR_LUTRON
#ifndef D_STR_MAGIQUEST
#define D_STR_MAGIQUEST "MAGIQUEST"
#endif  // D_STR_MAGIQUEST
#ifndef D_STR_METZ
#define D_STR_METZ "METZ"
#endif  // D_STR_METZ
#ifndef D_STR_MIDEA
#define D_STR_MIDEA "MIDEA"
#endif  // D_STR_MIDEA
#ifndef D_STR_MIDEA24
#define D_STR_MIDEA24 "MIDEA24"
#endif  // D_STR_MIDEA24
#ifndef D_STR_MILESTAG2
#define D_STR_MILESTAG2 "MILESTAG2"
#endif  // D_STR_MILESTAG2
#ifndef D_STR_MIRAGE
#define D_STR_MIRAGE "MIRAGE"
#endif  // D_STR_MIRAGE
#ifndef D_STR_MITSUBISHI
#define D_STR_MITSUBISHI "MITSUBISHI"
#endif  // D_STR_MITSUBISHI
#ifndef D_STR_MITSUBISHI112
#define D_STR_MITSUBISHI112 "MITSUBISHI112"
#endif  // D_STR_MITSUBISHI112
#ifndef D_STR_MITSUBISHI136
#define D_STR_MITSUBISHI136 "MITSUBISHI136"
#endif  // D_STR_MITSUBISHI136
#ifndef D_STR_MITSUBISHI2
#define D_STR_MITSUBISHI2 "MITSUBISHI2"
#endif  // D_STR_MITSUBISHI2
#ifndef D_STR_MITSUBISHI_AC
#define D_STR_MITSUBISHI_AC "MITSUBISHI_AC"
#endif  // D_STR_MITSUBISHI_AC
#ifndef D_STR_MITSUBISHI_HEAVY_152
#define D_STR_MITSUBISHI_HEAVY_152 "MITSUBISHI_HEAVY_152"
#endif  // D_STR_MITSUBISHI_HEAVY_152
#ifndef D_STR_MITSUBISHI_HEAVY_88
#define D_STR_MITSUBISHI_HEAVY_88 "MITSUBISHI_HEAVY_88"
#endif  // D_STR_MITSUBISHI_HEAVY_88
#ifndef D_STR_MULTIBRACKETS
#define D_STR_MULTIBRACKETS "MULTIBRACKETS"
#endif  // D_STR_MULTIBRACKETS
#ifndef D_STR_MWM
#define D_STR_MWM "MWM"
#endif  // D_STR_MWM
#ifndef D_STR_NEC
#define D_STR_NEC "NEC"
#endif  // D_STR_NEC
#ifndef D_STR_NEC_LIKE
#define D_STR_NEC_LIKE D_STR_NEC "_LIKE"
#endif  // D_STR_NEC_LIKE
#ifndef D_STR_NEC_NON_STRICT
#define D_STR_NEC_NON_STRICT D_STR_NEC " (NON-STRICT)"
#endif  // D_STR_NEC_NON_STRICT
#ifndef D_STR_NEOCLIMA
#define D_STR_NEOCLIMA "NEOCLIMA"
#endif  // D_STR_NEOCLIMA
#ifndef D_STR_NIKAI
#define D_STR_NIKAI "NIKAI"
#endif  // D_STR_NIKAI
#ifndef D_STR_PANASONIC
#define D_STR_PANASONIC "PANASONIC"
#endif  // D_STR_PANASONIC
#ifndef D_STR_PANASONIC_AC
#define D_STR_PANASONIC_AC "PANASONIC_AC"
#endif  // D_STR_PANASONIC_AC
#ifndef D_STR_PANASONIC_AC32
#define D_STR_PANASONIC_AC32 D_STR_PANASONIC_AC"32"
#endif  // D_STR_PANASONIC_AC32
#ifndef D_STR_PIONEER
#define D_STR_PIONEER "PIONEER"
#endif  // D_STR_PIONEER
#ifndef D_STR_PRONTO
#define D_STR_PRONTO "PRONTO"
#endif  // D_STR_PRONTO
#ifndef D_STR_RAW
#define D_STR_RAW "RAW"
#endif  // D_STR_RAW
#ifndef D_STR_RC5
#define D_STR_RC5 "RC5"
#endif  // D_STR_RC5
#ifndef D_STR_RC5X
#define D_STR_RC5X "RC5X"
#endif  // D_STR_RC5X
#ifndef D_STR_RC6
#define D_STR_RC6 "RC6"
#endif  // D_STR_RC6
#ifndef D_STR_RCMM
#define D_STR_RCMM "RCMM"
#endif  // D_STR_RCMM
#ifndef D_STR_RHOSS
#define D_STR_RHOSS "RHOSS"
#endif  // D_STR_RHOSS
#ifndef D_STR_SAMSUNG
#define D_STR_SAMSUNG "SAMSUNG"
#endif  // D_STR_SAMSUNG
#ifndef D_STR_SAMSUNG36
#define D_STR_SAMSUNG36 "SAMSUNG36"
#endif  // D_STR_SAMSUNG36
#ifndef D_STR_SAMSUNG_AC
#define D_STR_SAMSUNG_AC "SAMSUNG_AC"
#endif  // D_STR_SAMSUNG_AC
#ifndef D_STR_SANYO
#define D_STR_SANYO "SANYO"
#endif  // D_STR_SANYO
#ifndef D_STR_SANYO_AC
#define D_STR_SANYO_AC "SANYO_AC"
#endif  // D_STR_SANYO_AC
#ifndef D_STR_SANYO_AC88
#define D_STR_SANYO_AC88 "SANYO_AC88"
#endif  // D_STR_SANYO_AC88
#ifndef D_STR_SANYO_LC7461
#define D_STR_SANYO_LC7461 "SANYO_LC7461"
#endif  // D_STR_SANYO_LC7461
#ifndef D_STR_SHARP
#define D_STR_SHARP "SHARP"
#endif  // D_STR_SHARP
#ifndef D_STR_SHARP_AC
#define D_STR_SHARP_AC "SHARP_AC"
#endif  // D_STR_SHARP_AC
#ifndef D_STR_SHERWOOD
#define D_STR_SHERWOOD "SHERWOOD"
#endif  // D_STR_SHERWOOD
#ifndef D_STR_SONY
#define D_STR_SONY "SONY"
#endif  // D_STR_SONY
#ifndef D_STR_SONY_38K
#define D_STR_SONY_38K "SONY_38K"
#endif  // D_STR_SONY_38K
#ifndef D_STR_SYMPHONY
#define D_STR_SYMPHONY "SYMPHONY"
#endif  // D_STR_SYMPHONY
#ifndef D_STR_TCL112AC
#define D_STR_TCL112AC "TCL112AC"
#endif  // D_STR_TCL112AC
#ifndef D_STR_TECHNIBEL_AC
#define D_STR_TECHNIBEL_AC "TECHNIBEL_AC"
#endif  // D_STR_TECHNIBEL_AC
#ifndef D_STR_TECO
#define D_STR_TECO "TECO"
#endif  // D_STR_TECO
#ifndef D_STR_TEKNOPOINT
#define D_STR_TEKNOPOINT "TEKNOPOINT"
#endif  // D_STR_TEKNOPOINT
#ifndef D_STR_TOSHIBA_AC
#define D_STR_TOSHIBA_AC "TOSHIBA_AC"
#endif  // D_STR_TOSHIBA_AC
#ifndef D_STR_TRANSCOLD
#define D_STR_TRANSCOLD "TRANSCOLD"
#endif  // D_STR_TRANSCOLD
#ifndef D_STR_TROTEC
#define D_STR_TROTEC "TROTEC"
#endif  // D_STR_TROTEC
#ifndef D_STR_TROTEC_3550
#define D_STR_TROTEC_3550 D_STR_TROTEC "_3550"
#endif  // D_STR_TROTEC_3550
#ifndef D_STR_TRUMA
#define D_STR_TRUMA "TRUMA"
#endif  // D_STR_TRUMA
#ifndef D_STR_UNUSED
#define D_STR_UNUSED "UNUSED"
#endif  // D_STR_UNUSED
#ifndef D_STR_VESTEL_AC
#define D_STR_VESTEL_AC "VESTEL_AC"
#endif  // D_STR_VESTEL_AC
#ifndef D_STR_VOLTAS
#define D_STR_VOLTAS "VOLTAS"
#endif  // D_STR_VOLTAS
#ifndef D_STR_WHIRLPOOL_AC
#define D_STR_WHIRLPOOL_AC "WHIRLPOOL_AC"
#endif  // D_STR_WHIRLPOOL_AC
#ifndef D_STR_WHYNTER
#define D_STR_WHYNTER "WHYNTER"
#endif  // D_STR_WHYNTER
#ifndef D_STR_XMP
#define D_STR_XMP "XMP"
#endif  // D_STR_XMP
#ifndef D_STR_ZEPEAL
#define D_STR_ZEPEAL "ZEPEAL"
#endif  // D_STR_ZEPEAL

// IRrecvDumpV2+
#ifndef D_STR_TIMESTAMP
#define D_STR_TIMESTAMP "Timestamp"
#endif  // D_STR_TIMESTAMP
#ifndef D_STR_LIBRARY
#define D_STR_LIBRARY "Library"
#endif  // D_STR_LIBRARY
#ifndef D_STR_MESGDESC
#define D_STR_MESGDESC "Mesg Desc."
#endif  // D_STR_MESGDESC
#ifndef D_STR_TOLERANCE
#define D_STR_TOLERANCE "Tolerance"
#endif  // D_STR_TOLERANCE
#ifndef D_STR_IRRECVDUMP_STARTUP
#define D_STR_IRRECVDUMP_STARTUP \
    "IRrecvDump is now running and waiting for IR input on Pin %d"
#endif  // D_STR_IRRECVDUMP_STARTUP
#ifndef D_WARN_BUFFERFULL
#define D_WARN_BUFFERFULL \
    "WARNING: IR code is too big for buffer (>= %d). " \
    "This result shouldn't be trusted until this is resolved. " \
    "Edit & increase `kCaptureBufferSize`."
#endif  // D_WARN_BUFFERFULL

#endif  // LOCALE_DEFAULTS_H_
