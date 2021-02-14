![IRremoteESP8266 Library](./assets/images/banner.svg)

[![Build-Status](https://travis-ci.org/crankyoldgit/IRremoteESP8266.svg?branch=master)](https://travis-ci.org/crankyoldgit/IRremoteESP8266)
[![Arduino-Bibliothek-Abzeichen](https://www.ardu-badge.com/badge/IRremoteESP8266.svg?)](https://www.ardu-badge.com/IRremoteESP8266)
[![Durchschnittliche Zeit bis zur Problemlösung](http://isitmaintained.com/badge/resolution/crankyoldgit/IRremoteESP8266.svg)](http://isitmaintained.com/project/crankyoldgit/IRremoteESP8266 "Resolution Time")
[![Prozentsatz der offenen Probleme](http://isitmaintained.com/badge/open/crankyoldgit/IRremoteESP8266.svg)](http://isitmaintained.com/project/crankyoldgit/IRremoteESP8266 "Open issues")
[![Git-Lizenz](https://gitlicense.com/badge/crankyoldgit/IRremoteESP8266)](https://gitlicense.com/license/crankyoldgit/IRremoteESP8266)

Diese Programmbibliothek ermöglicht das **Senden _und_ Empfangen** von Infrarotsignalen mit [ESP8266](https://github.com/esp8266/Arduino)- und
[ESP32](https://github.com/espressif/arduino-esp32)-Mikrocontrollern mithilfe des [Arduino-Frameworks](https://www.arduino.cc/) und handelsüblichen 940nm Infrarot-LEDs undIR-Empfängermodulen, wie zum Beispiel TSOP{17,22,24,36,38,44,48}*-Demodulatoren.

## v2.7.15 jetzt verfügbar
Version 2.7.15 der Bibliothek ist nun [verfügbar](https://github.com/crankyoldgit/IRremoteESP8266/releases/latest). Die [Versionshinweise](ReleaseNotes.md) enthalten alle wichtigen Neuerungen.

#### Hinweis für Nutzer von Versionen vor v2.0
Die Benutzung der Bibliothek hat sich mit Version 2.0 leicht geändert. Einige Anpassungen im aufrufenden Code werden nötig sein, um mit Version ab 2.0 korrekt zu funktionieren. Mehr zu den Anpassungen finden sich auf unserer [Upgrade to v2.0](https://github.com/crankyoldgit/IRremoteESP8266/wiki/Upgrading-to-v2.0)-Seite.

#### Hinweis für Nutzer von Versionen vor v2.5
Die Bibliothek benutzt zur Konstantendefinition nun statt `#define` den [const](https://google.github.io/styleguide/cppguide.html#Constant_Names)-Ansatz mit der Benennung in Einklang mit dem [C++-Style-Guide](https://google.github.io/styleguide/cppguide.html).
Dies kann eventuell dazu führen, daß sich ältere Programme nicht mehr kompilieren lassen.
Die mutmaßlich am häufigsten extern verwendeten `#define`s wurden daher ge_aliased_, um eine gewisse Rückwärts-Kompatibilität zu gewährleisten. In künftigen Protokollimplementationen wird nur noch der neue `kConstantName`-Stil unterstützt werden.

Falls die Änderungen dazu geführt haben, daß der aufrufende Code nicht mehr funktioniert, wurde vermutlich etwas referenziert, was nicht referenziert werden sollte. Den neuen Kontanten-Namen herauszufinden sollte nach der Form `CONSTANT_NAME` nach `kConstantName` leicht möglich sein.
Etwas gesunder Menschenverstand oder ein Blick in den Bibliotheksquellcode helfen, falls aufrufender Code betroffen ist.

## Unterstützte Protokolle
Details zu den unterstützten Protokollen und Geräten befinden sich [hier](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/SupportedProtocols.md).

## Fehlersuche
Bitte erst den [Troubleshooting Guide](https://github.com/crankyoldgit/IRremoteESP8266/wiki/Troubleshooting-Guide) lesen, bevor Probleme gemeldet werden oder um Hilfe gebeten wird.

## FAQ - häufige Fragen
Einige Antworten zu häufig gestellten Fragen sind auf unserer [F.A.Q. Wiki-Seite](https://github.com/crankyoldgit/IRremoteESP8266/wiki/Frequently-Asked-Questions) hinterlegt.

## Library API-Dokumentation
Diese Bibliothek benutzt [Doxygen](https://www.doxygen.nl/index.html) zur [automatischen Dokumentation](https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/) der [API](https://en.wikipedia.org/wiki/Application_programming_interface) dieser [Bibliothek](https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/).
Sie ist [hier](https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/) zu finden.

## Installation
##### Installation von offiziellen Releases über die Arduino-IDE v1.8+ (Windows & Linux)
1. Das Untermenü _"Sketch"_ -> _"Bibliothek einbinden"_ -> _"Bibliotheken verwalten..."_ aufrufen.
1. In das Suchfeld oben rechts (_"Grenzen Sie Ihre Suche ein..."_) `IRremoteESP8266` eintragen.
1. Bei den Suchergebnissen IRremoteESP8266 auswählen.
1. Die Version markieren, die installiert werden soll, und dann _"Installieren"_ klicken.

##### Manuelle Installation (Windows)
1. Auf der Website auf den grünen _"Code"_-Knopf klicken, dann _"[Download ZIP](https://github.com/crankyoldgit/IRremoteESP8266/archive->master.zip)"_ auswählen.
1. Die heruntergeladene Zip-Datei entpacken.
1. Den entpackten Dateiordner in _"IRremoteESP8266"_ umbenennen.
1. Diesen Ordner anschließend in den Bibliotheken-Pfad verschieben. (Unter Windows: `C:\Users\BENUTZER\Dokumente\Arduino\libraries\`)
1. Die Arduino-IDE neu starten.
1. Unter den Beispielen finden sich neue Einträge.

##### Benutzung von Git für die Installation der Bibliothek (Linux)
```
cd ~/Arduino/libraries
git clone https://github.com/crankyoldgit/IRremoteESP8266.git
```
###### Um die neueste Version der Bibliothek zu beziehen
```
cd ~/Arduino/libraries/IRremoteESP8266 && git pull
```

## Mithelfen
Anregungen für die [Mithilfe](.github/CONTRIBUTING.md#how-can-i-contribute) am Projekt:
- Das [Melden](.github/CONTRIBUTING.md#reporting-bugs) von Bugs und Fehlern
- Das Einreichen von Verbesserungs- und Erweiterungsvorschlägen
- Das Erstellen und Verbessern der Dokumentation
- Das [Melden von Problemen](.github/CONTRIBUTING.md#reporting-bugs) und Einreichen von [Pull-Requests](.github/CONTRIBUTING.md#pull-requests)
- Anderen Leuten von dieser Bibliothek erzählen

## Beitragende
Die Beitragenden sind [hier](.github/Contributors.md) aufgelistet.

## Historie der Bibliothek
Diese Bibliothek basiert auf Ken Shirriff's Vorarbeit (https://github.com/shirriff/Arduino-IRremote/).

[Mark Szabo](https://github.com/crankyoldgit/IRremoteESP8266) programmierte die IRsend-Klassen auf ESP8266 und [Sebastien Warin](https://github.com/sebastienwarin/IRremoteESP8266) war verantwortlich für die Empfangs- und Dekodier-Teile (IRrecv-Klassen).

Die Bibliothek wurde ab Version v2.0 fast komplett neu geschrieben, um besser auf die ESP8266-Ressourcen Rücksicht zu nehmen.
