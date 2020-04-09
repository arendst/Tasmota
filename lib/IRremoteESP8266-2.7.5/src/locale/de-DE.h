// Copyright 2019 - Martin (@finfinack)
// Locale/language file for German / Germany.
// This file will override the default values located in `defaults.h`.
#ifndef LOCALE_DE_DE_H_
#define LOCALE_DE_DE_H_

#define D_STR_UNKNOWN "UNBEKANNT"
#define D_STR_PROTOCOL "Protokoll"
#define D_STR_ON "Ein"
#define D_STR_OFF "Aus"
#define D_STR_MODE "Modus"
#define D_STR_TOGGLE "Umschalten"
#define D_STR_SLEEP "Schlafen"
#define D_STR_LIGHT "Licht"
#define D_STR_POWERFUL "Stark"
#define D_STR_QUIET "Ruhig"
#define D_STR_ECONO "Eco"
#define D_STR_BEEP "Piep"
#define D_STR_MOULD "Schimmel"
#define D_STR_CLEAN "Reinigen"
#define D_STR_PURIFY "Frischen"
#define D_STR_TIMER "Timer"
#define D_STR_ONTIMER D_STR_ON " " D_STR_TIMER
#define D_STR_OFFTIMER D_STR_OFF " " D_STR_TIMER
#define D_STR_CLOCK "Uhr"
#define D_STR_COMMAND "Befehl"
#define D_STR_HEALTH "Gesundheit"
#define D_STR_TEMP "Temp"
#define D_STR_HUMID "Feucht"
#define D_STR_SAVE "Speichern"
#define D_STR_EYE "Auge"
#define D_STR_FOLLOW "Folgen"
#define D_STR_FRESH "Frisch"
#define D_STR_HOLD "Halten"
#define D_STR_BUTTON "Knopf"
#define D_STR_NIGHT "Nacht"
#define D_STR_SILENT "Ruhig"
#define D_STR_UP "Hinauf"
#define D_STR_TEMPUP D_STR_TEMP " " D_STR_UP
#define D_STR_DOWN "Hinunter"
#define D_STR_TEMPDOWN D_STR_TEMP " " D_STR_DOWN
#define D_STR_CHANGE "Wechseln"
#define D_STR_MOVE "Verschieben"
#define D_STR_SET "Setzen"
#define D_STR_CANCEL "Abbrechen"
#define D_STR_COMFORT "Komfort"
#define D_STR_WEEKLY "Wöchentlich"
#define D_STR_WEEKLYTIMER D_STR_WEEKLY " " D_STR_TIMER
#define D_STR_FAST "Schnell"
#define D_STR_SLOW "Langsam"
#define D_STR_AIRFLOW "Luftzug"
#define D_STR_STEP "Schritt"
#define D_STR_NA "N/A"
#define D_STR_OUTSIDE "Draussen"
#define D_STR_LOUD "Laut"
#define D_STR_UPPER "Oben"
#define D_STR_LOWER "Unten"
#define D_STR_BREEZE "Brise"
#define D_STR_CIRCULATE "Zirkulieren"
#define D_STR_CEILING "Decke"
#define D_STR_WALL "Wand"
#define D_STR_ROOM "Raum"
#define D_STR_6THSENSE "6ter Sinn"
#define D_STR_FIXED "Fixiert"

#define D_STR_AUTOMATIC "Automatisch"
#define D_STR_MANUAL "Manuell"
#define D_STR_COOL "Kühlen"
#define D_STR_HEAT "Heizen"
#define D_STR_FAN "Lüfter"
#define D_STR_FANONLY "nur_lüfter"
#define D_STR_DRY "Trocken"

#define D_STR_MED "Mit"
#define D_STR_MEDIUM "Mittel"

#define D_STR_HIGHEST "Höchste"
#define D_STR_HIGH "Hoch"
#define D_STR_HI "H"
#define D_STR_MID "M"
#define D_STR_MIDDLE "Mittel"
#define D_STR_LOW "Tief"
#define D_STR_LO "T"
#define D_STR_LOWEST "Tiefste"
#define D_STR_RIGHT "Rechts"
#define D_STR_MAXRIGHT D_STR_MAX " " D_STR_RIGHT
#define D_STR_RIGHTMAX_NOSPACE D_STR_RIGHT D_STR_MAX
#define D_STR_LEFT "Links"
#define D_STR_MAXLEFT D_STR_MAX " " D_STR_LEFT
#define D_STR_LEFTMAX_NOSPACE D_STR_LEFT D_STR_MAX
#define D_STR_WIDE "Breit"
#define D_STR_CENTRE "Mitte"
#define D_STR_TOP "Oben"
#define D_STR_BOTTOM "Unten"

#define D_STR_DAY "Tag"
#define D_STR_DAYS D_STR_DAY "e"
#define D_STR_HOUR "Stunde"
#define D_STR_HOURS D_STR_HOUR "n"
#define D_STR_MINUTES D_STR_MINUTE "n"
#define D_STR_SECOND "Sekunde"
#define D_STR_SECONDS D_STR_SECOND "n"
#define D_STR_NOW "Jetzt"
// These don't translate well to German as typically only 2 letter
// abbreviations are used. Hence, this is an approximation.
#define D_STR_THREELETTERDAYS "SonMonDieMitDonFreSam"

#define D_STR_YES "Ja"
#define D_STR_NO "Nein"
#define D_STR_TRUE "Wahr"
#define D_STR_FALSE "Falsch"

#define D_STR_REPEAT "Wiederholen"

// IRrecvDumpV2
#define D_STR_TIMESTAMP "Zeitstempel"
#define D_STR_LIBRARY "Bibliothek"
#define D_STR_MESGDESC "Nachr. Beschr."
#define D_STR_IRRECVDUMP_STARTUP \
    "IRrecvDumpV2 läuft und wartet auf IR Eingabe auf Pin %d"
#define D_WARN_BUFFERFULL \
    "WARNUNG: IR Code ist zu gross für Buffer (>= %d). " \
    "Dem Resultat sollte nicht vertraut werden bevor das behoben ist. " \
    "Bearbeite & vergrössere `kCaptureBufferSize`."

#endif  // LOCALE_DE_DE_H_
