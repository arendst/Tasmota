// Copyright 2022 - Stijn (@stijnb1234)
// Locale/language file for Dutch / The Netherlands.
// This file will override the default values located in `defaults.h`.
#ifndef LOCALE_NL_NL_H_
#define LOCALE_NL_NL_H_

#define D_STR_UNKNOWN "ONBEKEND"
#define D_STR_POWER "Stroom"
#define D_STR_PREVIOUS "Vorige"
#define D_STR_ON "Aan"
#define D_STR_OFF "Uit"
#define D_STR_MODE "Modus"
#define D_STR_TOGGLE "Omschakelen"
#define D_STR_SLEEP "Slaap"
#define D_STR_LIGHT "Licht"
#define D_STR_POWERFUL "Sterk"
#define D_STR_QUIET "Rustig"
#define D_STR_ECONO "Eco"
#define D_STR_SWING "Zwaai"
#define D_STR_BEEP "Piep"
#define D_STR_MOULD "Schimmel"
#define D_STR_CLEAN "Reinigen"
#define D_STR_PURIFY "Zuiver"
#define D_STR_TIMER "Timer"
#define D_STR_ONTIMER D_STR_TIMER " " D_STR_ON
#define D_STR_OFFTIMER D_STR_TIMER " " D_STR_OFF
#define D_STR_CLOCK "Klok"
#define D_STR_COMMAND "Commando"
#define D_STR_XFAN "XVentilator"
#define D_STR_HEALTH "Gezondheid"
#define D_STR_IFEEL "IkVoel"
#define D_STR_ISEE "IkZie"
#define D_STR_HUMID "Vochtigheid"
#define D_STR_SAVE "Opslaan"
#define D_STR_EYE "Ogen"
#define D_STR_FOLLOW "Volgen"
#define D_STR_FRESH "Fris"
#define D_STR_HOLD "Houd"
#define D_STR_BUTTON "Knop"
#define D_STR_NIGHT "Nacht"
#define D_STR_SILENT "Stil"
#define D_STR_UP "Omhoog"
#define D_STR_TEMPUP D_STR_TEMP " " D_STR_UP
#define D_STR_DOWN "Omlaag"
#define D_STR_TEMPDOWN D_STR_TEMP " " D_STR_DOWN
#define D_STR_CHANGE "Wisselen"
#define D_STR_MOVE "Verplaatsen"
#define D_STR_SET "Instellen"
#define D_STR_CANCEL "Annuleren"
#define D_STR_COMFORT "Comfortabel"
#define D_STR_WEEKLY "Weekelijks"
#define D_STR_WEEKLYTIMER D_STR_TIMER " " D_STR_WEEKLY
#define D_STR_FAST "Snel"
#define D_STR_SLOW "Langzaam"
#define D_STR_AIRFLOW "Luchtstroom"
#define D_STR_STEP "Stap"
#define D_STR_NA "N/A"
#define D_STR_OUTSIDE "Buiten"
#define D_STR_LOUD "Luid"
#define D_STR_UPPER "Boven"
#define D_STR_LOWER "Beneden"
#define D_STR_BREEZE "Wind"
#define D_STR_CIRCULATE "Circulatie"
#define D_STR_CEILING "Plafond"
#define D_STR_WALL "Muur"
#define D_STR_ROOM "Kamer"
#define D_STR_6THSENSE "6e Zintuig"
#define D_STR_FIXED "Vast"

#define D_STR_AUTOMATIC "Automatisch"
#define D_STR_MANUAL "Handmatig"
#define D_STR_COOL "Koelen"
#define D_STR_HEAT "Verwarmen"
#define D_STR_FAN "Venilator"
#define D_STR_FANONLY "alleen_fan"
#define D_STR_DRY "Drogen"

#define D_STR_MED "Mid"
#define D_STR_MEDIUM "Medium"

#define D_STR_HIGHEST "Hoogste"
#define D_STR_HIGH "Hoog"
#define D_STR_HI "H"
#define D_STR_MID "M"
#define D_STR_MIDDLE "Medium"
#define D_STR_LOW "Laag"
#define D_STR_LO "L"
#define D_STR_LOWEST "Laagste"
#define D_STR_RIGHT "Rechts"
#define D_STR_MAXRIGHT D_STR_MAX " " D_STR_RIGHT
#define D_STR_RIGHTMAX_NOSPACE D_STR_RIGHT D_STR_MAX
#define D_STR_LEFT "Links"
#define D_STR_MAXLEFT D_STR_MAX " " D_STR_LEFT
#define D_STR_LEFTMAX_NOSPACE D_STR_LEFT D_STR_MAX
#define D_STR_WIDE "Breed"
#define D_STR_CENTRE "Midden"
#define D_STR_TOP "Boven"
#define D_STR_BOTTOM "Onder"

#define D_STR_DAY "Dag"
#define D_STR_DAYS D_STR_DAY "en"
#define D_STR_HOUR "Uur"
#define D_STR_HOURS D_STR_HOUR
#define D_STR_MINUTE "Minuut"
#define D_STR_MINUTES "Minuten"
#define D_STR_SECOND "Seconde"
#define D_STR_SECONDS D_STR_SECOND "n"
#define D_STR_NOW "Nu"
#define D_STR_THREELETTERDAYS "ZonMaaDinWoeDonVriZat"

#define D_STR_YES "Ja"
#define D_STR_NO "Nee"
#define D_STR_TRUE "Waar"
#define D_STR_FALSE "Niet Waar"

#define D_STR_REPEAT "Herhalen"
#define D_STR_PREVIOUS "Vorige"
#define D_STR_DISPLAY "Display"
#define D_STR_INSIDE "Binnen"
#define D_STR_POWERBUTTON "Hoofdschakelaar"
#define D_STR_PREVIOUSPOWER "Vorige inschakelstatus"
#define D_STR_DISPLAYTEMP "Temperatuurweergave"

// IRrecvDumpV2+
#define D_STR_TIMESTAMP "Tijdsaanduiding"
#define D_STR_LIBRARY "Bibliotheek"
#define D_STR_TOLERANCE "Tolerantie"
#define D_STR_MESGDESC "Beschrijving"
#define D_STR_IRRECVDUMP_STARTUP \
    "IRrecvDump draait en wacht op IR-signaal op pin %d"
#define D_WARN_BUFFERFULL \
    "WAARSCHUWING: IR-code is te groot voor buffer (>= %d). " \
    "Het resultaat kan niet worden vertrouwd totdat het is verholpen. " \
    "Wijzig & vergroot `kCaptureBufferSize`."

#endif  // LOCALE_NL_NL_H_
