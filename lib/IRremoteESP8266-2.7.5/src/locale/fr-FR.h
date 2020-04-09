// Copyright 2019 - Mathieu D(@Knackie)
// Locale/language file for French / Quebec.
// This file will override the default values located in `defaults.h`.
#ifndef LOCALE_FR_FR_H_
#define LOCALE_FR_FR_H_

#define D_STR_UNKNOWN "INCONNU"
#define D_STR_PROTOCOL "Protocole"
#define D_STR_TOGGLE "Bascule"
#define D_STR_SLEEP "Pause"
#define D_STR_LIGHT "Lumière"
#define D_STR_POWERFUL "Puissance"
#define D_STR_PREVIOUS "Precedente"
#define D_STR_PREVIOUSPOWER D_STR_POWER " " D_STR_PREVIOUS
#define D_STR_QUIET "Silence"
#define D_STR_ECONO "Economie"
#define D_STR_BEEP "Bip"
#define D_STR_MOULD "Moule"
#define D_STR_CLEAN "Nettoyer"
#define D_STR_PURIFY "Purifier"
#define D_STR_ON "On"
#define D_STR_OFF "Off"
#define D_STR_ONTIMER D_STR_ON " " D_STR_TIMER
#define D_STR_OFFTIMER D_STR_OFF " " D_STR_TIMER
#define D_STR_CLOCK "Heure"
#define D_STR_COMMAND "Commandement"
#define D_STR_HEALTH "Santé"
#define D_STR_TEMP "Temporaire"
#define D_STR_HUMID "Humidité"
#define D_STR_SAVE "Sauvegarder"
#define D_STR_EYE "Oeil"
#define D_STR_FOLLOW "Suivre"
#define D_STR_FRESH "Frais"
#define D_STR_HOLD "Maintenir"
#define D_STR_BUTTON "Bouton"
#define D_STR_NIGHT "Nuit"
#define D_STR_SILENT "Silence"
#define D_STR_UP "En haut"
#define D_STR_TEMPUP D_STR_TEMP " " D_STR_UP
#define D_STR_DOWN "En bas"
#define D_STR_TEMPDOWN D_STR_TEMP " " D_STR_DOWN
#define D_STR_CHANGE "Changement"
#define D_STR_SET "Mettre"
#define D_STR_CANCEL "Annuler"
#define D_STR_COMFORT "Confort"
#define D_STR_WEEKLY "Chaque semaine"
#define D_STR_WEEKLYTIMER D_STR_WEEKLY " " D_STR_TIMER
#define D_STR_FAST "Rapide"
#define D_STR_SLOW "Lent"
#define D_STR_AIRFLOW "Ebauche"
#define D_STR_STEP "Etape"
#define D_STR_OUTSIDE "Plein air"
#define D_STR_LOUD "Fort"
#define D_STR_UPPER "Au dessus"
#define D_STR_LOWER "En dessous"
#define D_STR_BREEZE "Brise"
#define D_STR_CIRCULATE "Faire circuler"
#define D_STR_CEILING "Plafond"
#define D_STR_WALL "Mur"
#define D_STR_ROOM "Pièce"
#define D_STR_6THSENSE "6ter Sens"
#define D_STR_FIXED "Fixer"

#define D_STR_AUTOMATIC "Automatique"
#define D_STR_MANUAL "Manuel"
#define D_STR_COOL "Frais"
#define D_STR_HEAT "Chaleur"
#define D_STR_FAN "Ventillateur"
#define D_STR_FANONLY "Seul_fan"
#define D_STR_DRY "Sec"

#define D_STR_MEDIUM "Moyen"

#define D_STR_HIGHEST "Le plus haut"
#define D_STR_HIGH "Haut"
#define D_STR_HI "H"
#define D_STR_MID "M"
#define D_STR_MIDDLE "Moitié"
#define D_STR_LOW "Bas"
#define D_STR_LO "B"
#define D_STR_LOWEST "Le plus bas"
#define D_STR_RIGHT "Droite"
#define D_STR_MAX "Max"
#define D_STR_MAXRIGHT D_STR_MAX " " D_STR_RIGHT
#define D_STR_RIGHTMAX_NOSPACE D_STR_RIGHT D_STR_MAX
#define D_STR_LEFT "Gauche"
#define D_STR_MAXLEFT D_STR_MAX " " D_STR_LEFT
#define D_STR_LEFTMAX_NOSPACE D_STR_LEFT D_STR_MAX
#define D_STR_WIDE "Large"
#define D_STR_TOP "Au-dessus"
#define D_STR_BOTTOM "En-dessous"

#define D_STR_DAY "Jour"
#define D_STR_HOUR "Heure"
#define D_STR_SECOND "Seconde"
#define D_STR_NOW "Maintenant"
#define D_STR_THREELETTERDAYS "LunMarMerJeuVenSamDim"

#define D_STR_YES "Oui"
#define D_STR_NO "Non"
#define D_STR_TRUE "Vrai"
#define D_STR_FALSE "Faux"

#define D_STR_REPEAT "Répetition"

// IRrecvDumpV2
#define D_STR_TIMESTAMP "Horodatage"
#define D_STR_LIBRARY "Bibliothèque"
#define D_STR_MESGDESC "Rèférence"
#define D_STR_IRRECVDUMP_STARTUP \
    "IRrecvDumpV2 fonctionne et attend l’entrée IR sur la broche %d"
#define D_WARN_BUFFERFULL \
    "ATTENTION: IR Code est trop gros pour le buffer (>= %d). " \
    "Le résultat ne doit pas être approuvé avant que cela soit résolu. " \
    "Modifier et agrandir `kCaptureBufferSize`."

#endif  // LOCALE_FR_FR_H_
