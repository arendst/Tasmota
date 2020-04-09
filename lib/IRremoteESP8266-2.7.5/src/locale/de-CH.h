// Copyright 2019 - Martin (@finfinack)
// Locale/language file for German / Switzerland.
// This file will override the default values located in `defaults.h`.
#ifndef LOCALE_DE_CH_H_
#define LOCALE_DE_CH_H_

// Import German / Germany as default overrides.
#include "locale/de-DE.h"

// As we have loaded another language, we need to #undef anything we need
// to update/change.

#undef D_STR_ON
#define D_STR_ON "Ii"
#undef D_STR_OFF
#define D_STR_OFF "Us"
#undef D_STR_TOGGLE
#define D_STR_TOGGLE "Umschalte"
#undef D_STR_SLEEP
#define D_STR_SLEEP "Schlafe"
#undef D_STR_LIGHT
#define D_STR_LIGHT "Liecht"
#undef D_STR_POWERFUL
#define D_STR_POWERFUL "Starch"
#undef D_STR_QUIET
#define D_STR_QUIET "Liislig"
#undef D_STR_CLEAN
#define D_STR_CLEAN "Reinige"
#undef D_STR_PURIFY
#define D_STR_PURIFY "Frische"
#undef D_STR_HEALTH
#define D_STR_HEALTH "Gsundheit"
#undef D_STR_HUMID
#define D_STR_HUMID "Füecht"
#undef D_STR_SAVE
#define D_STR_SAVE "Speichere"
#undef D_STR_EYE
#define D_STR_EYE "Aug"
#undef D_STR_FOLLOW
#define D_STR_FOLLOW "Folge"
#undef D_STR_HOLD
#define D_STR_HOLD "Halte"
#undef D_STR_BUTTON
#define D_STR_BUTTON "Chnopf"
#undef D_STR_UP
#define D_STR_UP "Ufe"
#undef D_STR_TEMPUP
#define D_STR_TEMPUP D_STR_TEMP " " D_STR_UP
#undef D_STR_DOWN
#define D_STR_DOWN "Abe"
#undef D_STR_TEMPDOWN
#define D_STR_TEMPDOWN D_STR_TEMP " " D_STR_DOWN
#undef D_STR_CHANGE
#define D_STR_CHANGE "Wechsele"
#undef D_STR_MOVE
#define D_STR_MOVE "Verschiebe"
#undef D_STR_SET
#define D_STR_SET "Setze"
#undef D_STR_CANCEL
#define D_STR_CANCEL "Abbreche"
#undef D_STR_WEEKLY
#define D_STR_WEEKLY "Wüchentlich"
#undef D_STR_WEEKLYTIMER
#define D_STR_WEEKLYTIMER D_STR_WEEKLY " " D_STR_TIMER
#undef D_STR_OUTSIDE
#define D_STR_OUTSIDE "Dusse"
#undef D_STR_LOUD
#define D_STR_LOUD "Luut"
#undef D_STR_UPPER
#define D_STR_UPPER "Obe"
#undef D_STR_LOWER
#define D_STR_LOWER "Une"
#undef D_STR_CIRCULATE
#define D_STR_CIRCULATE "Zirkuliere"
#undef D_STR_CEILING
#define D_STR_CEILING "Decki"
#undef D_STR_6THSENSE
#define D_STR_6THSENSE "6te Sinn"

#undef D_STR_COOL
#define D_STR_COOL "Chüehle"
#undef D_STR_HEAT
#define D_STR_HEAT "Heize"
#undef D_STR_DRY
#define D_STR_DRY "Tröchne"

#undef D_STR_MED
#define D_STR_MED "Mit"
#undef D_STR_MEDIUM
#define D_STR_MEDIUM "Mittel"

#undef D_STR_HIGHEST
#define D_STR_HIGHEST "Höchscht"
#undef D_STR_HIGH
#define D_STR_HIGH "Höch"
#undef D_STR_HI
#define D_STR_HI "H"
#undef D_STR_MID
#define D_STR_MID "M"
#undef D_STR_MIDDLE
#define D_STR_MIDDLE "Mittel"
#undef D_STR_LOW
#define D_STR_LOW "Tüüf"
#undef D_STR_LO
#define D_STR_LO "T"
#undef D_STR_LOWEST
#define D_STR_LOWEST "Tüfschte"
#undef D_STR_MAXRIGHT
#define D_STR_MAXRIGHT D_STR_MAX " " D_STR_RIGHT
#undef D_STR_RIGHTMAX_NOSPACE
#define D_STR_RIGHTMAX_NOSPACE D_STR_RIGHT D_STR_MAX
#undef D_STR_MAXLEFT
#define D_STR_MAXLEFT D_STR_MAX " " D_STR_LEFT
#undef D_STR_LEFTMAX_NOSPACE
#define D_STR_LEFTMAX_NOSPACE D_STR_LEFT D_STR_MAX
#undef D_STR_CENTRE
#define D_STR_CENTRE "Mitti"
#undef D_STR_TOP
#define D_STR_TOP "Obe"
#undef D_STR_BOTTOM
#define D_STR_BOTTOM "Une"

#undef D_STR_DAY
#define D_STR_DAY "Tag"
#undef D_STR_DAYS
#define D_STR_DAYS "Täg"
#undef D_STR_HOUR
#define D_STR_HOUR "Stund"
#undef D_STR_HOURS
#define D_STR_HOURS D_STR_HOUR "e"
#undef D_STR_MINUTE
#define D_STR_MINUTE "Minute"
#undef D_STR_MINUTES
#define D_STR_MINUTES D_STR_MINUTE
#undef D_STR_SECONDS
#define D_STR_SECONDS D_STR_SECOND
#undef D_STR_NOW
#define D_STR_NOW "Jetz"

#undef D_STR_NO
#define D_STR_NO "Nei"

#undef D_STR_REPEAT
#define D_STR_REPEAT "Wiederhole"

// IRrecvDumpV2
#undef D_STR_TIMESTAMP
#define D_STR_TIMESTAMP "Ziitstämpfel"
#undef D_STR_IRRECVDUMP_STARTUP
#define D_STR_IRRECVDUMP_STARTUP \
    "IRrecvDumpV2 lauft und wartet uf IR Iigab ufem Pin %d"
#undef D_WARN_BUFFERFULL
#define D_WARN_BUFFERFULL \
    "WARNUNG: IR Code isch zgross für de Buffer (>= %d). " \
    "Dem Resultat sött mer nöd vertraue bevor das behobe isch. " \
    "Bearbeite & vergrössere `kCaptureBufferSize`."

#endif  // LOCALE_DE_CH_H_
