# Teleinfo (Aka TIC) Universal Library

This is a generic Teleinfo French Meter Measure Library, it can be used on Arduino like device and also such as Spark Core, Particle, ESP8266, Raspberry PI or anywhere you can do Cpp code ...

You can see Teleinformation official french datasheet [there][1] and this one is for [Linky][3].

Since this is really dedicated to French energy measuring system, I will continue in French.

# Installation

Copier le contenu de ce dossier (download zip) dans le dossier libraries de votre environnement Arduino Vous devriez avoir maintenant quelque chose comme `your_sketchbook_folder/libraries/LibTeleinfo` et ce dossier doit contentir les fichiers .cpp et .h ainsi que le sous dossier `examples`.
<br/>
Pour trouver votre dossier de sketchbook, dans l'environnement IDE, allez dans File>Preferences.
<br/>
allez voir ce [tutorial][2] sur les librairies Arduino si beoin.
<br/>

# Documentation

J'ai écrit un article [dédié][10] sur cette librairie, vous pouvez aussi voir les [catégories][6] associées à la téléinfo sur mon [blog][7].
Pour les commentaires et le support vous pouvez allez sur le [forum][8] dédié ou dans la [communauté][9] 

# Sketch d'exemples

- [Arduino_Softserial_Etiquette][3] Affiche des informations de téléinformation reçue étiquette par étiquette
- [Arduino_Softserial_Blink][11] Affiche des informations de téléinformation reçue trame par trame avec clignotement LED court/long si les données ont été modifiés
- [Arduino_Softserial_JSON][4] Retourne les informations de téléinformation au format JSON sur la liaison série.
- [Raspberry_JSON][12] Retourne les informations de téléinformation au format JSON sur stdout.
- [Wifinfo][5] ESP8266, ESP32 Wifi Teleinformation, Web + Rest + bonus, version en cours de développement, à venir mais un article [dédié][13] est déjà présent sur mon blog
- [ESP32][14] ESP32 Basic test pour WifInfo32 nouveau nom Denky :-)
- [ESP32_Passthru][14] ESP32 PassThru Basic test pour Denky D4, affiche les données et les stats de la téléinfo dans la console série
- [ESP8266_DataChanged][15] ESP8266 Surveille et affiche les données changées entre 2 trames, clignote la LED RGB en fonction
- [Teleinfo_DenkyD4][16] ESP32 Denky D4 Basic test et stats pour le nouveau Denky D4 basé sur l'ESP32-Pico-V3-02
- [Teleinfo_Stats][18] ESP32 Programme de test et statistiques pour la qualité de réception

# Pourquoi

- J'utilise la téléinfo dans plusieurs de mes programmes et j'en avais marre de devoir faire des copier/coller de code constament, j'ai donc décidé de faire une librairie commune que j'utilise sans me poser de question

# License

Cette oeuvre est mise à disposition selon les termes de la Licence Creative Commons Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 4.0 International.

Si vous êtes une entreprise et que vous souhaitez participer car vous utilisez cette librairie dans du hardware (box, automate, ...), vous pouvez toujours m'envoyer un exemplaire de votre fabrication, c'est toujours sympa de voir ce qui est fait avec ce code ;-)

# Addon

Ajout des compteurs d'erreurs, et du traitement du caracteres EOT d'interruption de trames

# Divers

Vous pouvez aller voir les nouveautés et autres projets sur [blog][7] 

[1]: https://www.enedis.fr/sites/default/files/Enedis-NOI-CPT_02E.pdf
[2]: http://learn.adafruit.com/arduino-tips-tricks-and-techniques/arduino-libraries
[3]: https://www.enedis.fr/sites/default/files/Enedis-NOI-CPT_54E.pdf
[6]: https://hallard.me/category/tinfo/
[7]: https://hallard.me
[8]: https://community.hallard.me/category/7
[9]: https://community.hallard.me
[10]: https://hallard.me/libteleinfo

[3]: https://github.com/hallard/LibTeleinfo/blob/master/examples/Arduino_Softserial/Arduino_Softserial_Etiquette.ino
[4]: https://github.com/hallard/LibTeleinfo/blob/master/examples/Arduino_Softserial_JSON/Arduino_Softserial_JSON.ino
[5]: https://github.com/hallard/LibTeleinfo/tree/master/examples/Wifinfo/Wifinfo.ino
[11]: https://github.com/hallard/LibTeleinfo/blob/master/examples/Arduino_Softserial/Arduino_Softserial_Blink.ino
[12]: https://github.com/hallard/LibTeleinfo/blob/master/examples/Raspberry_JSON/raspjson.cpp
[13]: https://hallard.me/wifiinfo/
[14]: https://github.com/hallard/LibTeleinfo/blob/master/examples/ESP32/ESP32.ino
[15]: https://github.com/hallard/LibTeleinfo/blob/master/examples/ESP8266_DataChanged/ESP8266_DataChanged.ino
[16]: https://github.com/hallard/LibTeleinfo/blob/master/examples/Teleinfo_DenkyD4/Teleinfo_DenkyD4.ino
[17]: https://github.com/hallard/LibTeleinfo/blob/master/examples/ESP32_Passthru/ESP32_Passthru.ino
[18]: https://github.com/hallard/LibTeleinfo/blob/master/examples/Teleinfo_Stats/Teleinfo_Stats.ino


