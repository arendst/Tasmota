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
#ifndef D_STR_ON
#define D_STR_ON "On"
#endif  // D_STR_ON
#ifndef D_STR_OFF
#define D_STR_OFF "Off"
#endif  // D_STR_OFF
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
#ifndef D_STR_HEAT
#define D_STR_HEAT "Heat"
#endif  // D_STR_HEAT
#ifndef D_STR_FAN
#define D_STR_FAN "Fan"
#endif  // D_STR_FAN
#ifndef D_STR_FANONLY
#define D_STR_FANONLY "fan_only"
#endif  // D_STR_FANONLY
#ifndef D_STR_DRY
#define D_STR_DRY "Dry"
#endif  // D_STR_DRY

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
#ifndef D_STR_RIGHTMAX_NOSPACE
#define D_STR_RIGHTMAX_NOSPACE D_STR_RIGHT D_STR_MAX  // Set `D_STR_MAX` first!
#endif  // D_STR_RIGHTMAX_NOSPACE
#ifndef D_STR_LEFT
#define D_STR_LEFT "Left"
#endif  // D_STR_LEFT
#ifndef D_STR_MAXLEFT
#define D_STR_MAXLEFT D_STR_MAX " " D_STR_LEFT  // Set `D_STR_MAX` first!
#endif  // D_STR_MAXLEFT
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

// IRrecvDumpV2
#ifndef D_STR_TIMESTAMP
#define D_STR_TIMESTAMP "Timestamp"
#endif  // D_STR_TIMESTAMP
#ifndef D_STR_LIBRARY
#define D_STR_LIBRARY "Library"
#endif  // D_STR_LIBRARY
#ifndef D_STR_MESGDESC
#define D_STR_MESGDESC "Mesg Desc."
#endif  // D_STR_MESGDESC
#ifndef D_STR_IRRECVDUMP_STARTUP
#define D_STR_IRRECVDUMP_STARTUP \
    "IRrecvDumpV2 is now running and waiting for IR input on Pin %d"
#endif  // D_STR_IRRECVDUMP_STARTUP
#ifndef D_WARN_BUFFERFULL
#define D_WARN_BUFFERFULL \
    "WARNING: IR code is too big for buffer (>= %d). " \
    "This result shouldn't be trusted until this is resolved. " \
    "Edit & increase `kCaptureBufferSize`."
#endif  // D_WARN_BUFFERFULL

#endif  // LOCALE_DEFAULTS_H_
