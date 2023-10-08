// Copyright 2020 - Enrico Gueli (@egueli)
// Locale/language file for Italian.
// This file will override the default values located in `defaults.h`.

#ifndef LOCALE_IT_IT_H_
#define LOCALE_IT_IT_H_

#define D_STR_UNKNOWN "SCONOSCIUTO"
#define D_STR_PROTOCOL "Protocollo"
#define D_STR_POWER "Accensione"
#define D_STR_PREVIOUS "Precedente"
#define D_STR_PREVIOUSPOWER D_STR_POWER " " D_STR_PREVIOUS
#define D_STR_ON "Acceso"
#define D_STR_OFF "Spento"
#define D_STR_MODE "Modalità"
#define D_STR_TOGGLE "Alterna"
#define D_STR_SLEEP "Sonno"
#define D_STR_LIGHT "Leggero"
#define D_STR_POWERFUL "Forte"
#define D_STR_QUIET "Silenzioso"
#define D_STR_ECONO "Eco"
#define D_STR_SWING "Swing"
#define D_STR_SWINGH D_STR_SWING"(O)"  // Set `D_STR_SWING` first!
#define D_STR_SWINGV D_STR_SWING"(V)"  // Set `D_STR_SWING` first!
#define D_STR_MOULD "Muffa"
#define D_STR_CLEAN "Pulizia"
#define D_STR_PURIFY "Purifica"
#define D_STR_TIMER "Timer"
#define D_STR_ONTIMER D_STR_ON " " D_STR_TIMER  // Set `D_STR_ON` first!
#define D_STR_OFFTIMER D_STR_OFF " " D_STR_TIMER  // Set `D_STR_OFF` first!
#define D_STR_CLOCK "Orologio"
#define D_STR_COMMAND "Comando"
#define D_STR_MODEL "Modello"
#define D_STR_TEMP "Temp"
#define D_STR_HUMID "Umido"
#define D_STR_SAVE "Salva"
#define D_STR_EYE "Occhio"
#define D_STR_FOLLOW "Segui"
#define D_STR_ION "Ioni"
#define D_STR_FRESH "Fresco"
#define D_STR_HOLD "Mantieni"
#define D_STR_8C_HEAT "8C " D_STR_HEAT  // Set `D_STR_HEAT` first!
#define D_STR_BUTTON "Pulsante"
#define D_STR_NIGHT "Notte"
#define D_STR_SILENT "Silenzioso"
#define D_STR_FILTER "Filtro"
#define D_STR_UP "Su"
#define D_STR_TEMPUP D_STR_TEMP " " D_STR_UP  // Set `D_STR_TEMP` first!
#define D_STR_DOWN "Giù"
#define D_STR_TEMPDOWN D_STR_TEMP " " D_STR_DOWN  // Set `D_STR_TEMP` first!
#define D_STR_CHANGE "Cambia"
#define D_STR_START "Avvia"
#define D_STR_STOP "Ferma"
#define D_STR_MOVE "Muovi"
#define D_STR_SET "Imposta"
#define D_STR_CANCEL "Annulla"
#define D_STR_SENSOR "Sensore"
#define D_STR_WEEKLY "Settimanale"
#define D_STR_WEEKLYTIMER D_STR_WEEKLY " " D_STR_TIMER  // Needs `D_STR_WEEKLY`!
#define D_STR_LAST "Ultimo"
#define D_STR_FAST "Veloce"
#define D_STR_SLOW "Lento"
#define D_STR_AIRFLOW "Flusso d'aria"
#define D_STR_STEP "Passo"
#define D_STR_NA "N/D"
#define D_STR_OUTSIDE "Esterno"
#define D_STR_LOUD "Rumoroso"
#define D_STR_UPPER "Superiore"
#define D_STR_LOWER "Inferiore"
#define D_STR_CIRCULATE "Circolare"
#define D_STR_CEILING "Soffitto"
#define D_STR_WALL "Muro"
#define D_STR_ROOM "Camera"
#define D_STR_FIXED "Fisso"

#define D_STR_AUTO "Auto"
#define D_STR_AUTOMATIC "Automatico"
#define D_STR_MANUAL "Manuale"
#define D_STR_COOL "Fresco"
#define D_STR_HEAT "Caldo"
#define D_STR_FAN "Ventola"
#define D_STR_FANONLY "solo_ventola"
#define D_STR_DRY "Secco"

#define D_STR_MAX "Max"
#define D_STR_MAXIMUM "Massimo"
#define D_STR_MINIMUM "Minimo"
#define D_STR_MEDIUM "Medio"

#define D_STR_HIGHEST "Molto alto"
#define D_STR_HIGH "Alto"
#define D_STR_MID "Med"
#define D_STR_MIDDLE "Medio"
#define D_STR_LOW "Basso"
#define D_STR_LOWEST "Bassissimo"
#define D_STR_RIGHT "Destra"
#define D_STR_MAXRIGHT D_STR_MAX " " D_STR_RIGHT  // Set `D_STR_MAX` first!
#define D_STR_RIGHTMAX_NOSPACE D_STR_RIGHT D_STR_MAX  // Set `D_STR_MAX` first!
#define D_STR_LEFT "Sinistra"
#define D_STR_MAXLEFT D_STR_MAX " " D_STR_LEFT  // Set `D_STR_MAX` first!
#define D_STR_LEFTMAX_NOSPACE D_STR_LEFT D_STR_MAX  // Set `D_STR_MAX` first!
#define D_STR_WIDE "Largo"
#define D_STR_CENTRE "Centro"
#define D_STR_TOP "Superiore"
#define D_STR_BOTTOM "Inferiore"
// Compound words/phrases/descriptions from pre-defined words.
// Note: Obviously these need to be defined *after* their component words.

#define D_STR_EYEAUTO D_STR_EYE " " D_STR_AUTO
#define D_STR_LIGHTTOGGLE D_STR_LIGHT " " D_STR_TOGGLE
#define D_STR_OUTSIDEQUIET D_STR_OUTSIDE " " D_STR_QUIET
#define D_STR_POWERTOGGLE D_STR_POWER " " D_STR_TOGGLE
#define D_STR_SENSORTEMP D_STR_SENSOR " " D_STR_TEMP
#define D_STR_SLEEP_TIMER D_STR_SLEEP " " D_STR_TIMER
#define D_STR_SWINGVMODE D_STR_SWINGV " " D_STR_MODE
#define D_STR_SWINGVTOGGLE D_STR_SWINGV " " D_STR_TOGGLE
// Separators
#ifndef D_CHR_TIME_SEP
#define D_CHR_TIME_SEP '.'
#endif  // D_CHR_TIME_SEP

#define D_STR_SPACELBRACE " ("
#define D_STR_COMMASPACE ", "
#define D_STR_COLONSPACE ": "

#define D_STR_DAY "Giorno"
#define D_STR_DAYS D_STR_DAY "s"
#define D_STR_HOUR "Ore"
#define D_STR_HOURS D_STR_HOUR "s"
#define D_STR_MINUTE "Minuti"
#define D_STR_MINUTES D_STR_MINUTE "s"
#define D_STR_SECOND "Secondi"
#define D_STR_SECONDS D_STR_SECOND "s"
#define D_STR_NOW "Adesso"
#define D_STR_THREELETTERDAYS "DomLunMarMerGioVenSab"

#define D_STR_YES "Sì"
#define D_STR_TRUE "Vero"
#define D_STR_FALSE "Falso"

#define D_STR_REPEAT "Ripeti"
#define D_STR_CODE "Codice"
#define D_STR_BITS "Bit"

// IRrecvDumpV2+
#define D_STR_LIBRARY "Libreria"
#define D_STR_MESGDESC "Desc. Mess."
#define D_STR_IRRECVDUMP_STARTUP \
    "IRrecvDump è ora attivo e in attesa di segnali IR dal pin %d"

#ifndef D_WARN_BUFFERFULL
#define D_WARN_BUFFERFULL \
    "ATTENZIONE: il codice IR è troppo grande per il buffer (>= %d). " \
    "Non fare affidamento a questi risultati finché questo problema " \
    "non è risolto." \
    "Modifica e aumenta `kCaptureBufferSize`."
#endif  // D_WARN_BUFFERFULL

#endif  // LOCALE_IT_IT_H_
