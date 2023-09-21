![IRremoteESP8266 Library](./assets/images/banner.svg)

[![Build-Status](https://github.com/crankyoldgit/IRremoteESP8266/actions/workflows/Build.yml/badge.svg)](../../actions/workflows/Build.yml/badge.svg)
[![Code-Lint](https://github.com/crankyoldgit/IRremoteESP8266/actions/workflows/Lint.yml/badge.svg)](../../actions/workflows/Lint.yml)
[![Tests](https://github.com/crankyoldgit/IRremoteESP8266/actions/workflows/UnitTests.yml/badge.svg)](../../ctions/workflows/UnitTests.yml)
[![Dokumentation](https://github.com/crankyoldgit/IRremoteESP8266/actions/workflows/Documentation.yml/badge.svg)](../../actions/workflows/Documentation.yml)
[![arduino-library-badge](https://www.ardu-badge.com/badge/IRremoteESP8266.svg?)](https://www.ardu-badge.com/IRremoteESP8266)
[![Arduino-Bibliothek-Abzeichen](https://www.ardu-badge.com/badge/IRremoteESP8266.svg?)](https://www.ardu-badge.com/IRremoteESP8266)
[![Git-Lizenz](https://gitlicense.com/badge/crankyoldgit/IRremoteESP8266)](https://gitlicense.com/license/crankyoldgit/IRremoteESP8266)

Deze library maakt het mogelijk om Infraroodsignalen **te versturen en ontvangen** via het [Arduino framework](https://www.arduino.cc/) met veelgebruikte 940nm IR LEDs en IR ontvang modules. b.v. TSOP{17,22,24,36,38,44,48}* demodulatoren enz.

## v2.8.6 nu beschikbaar
Versie 2.8.6 van de bibliotheek is nu [beschikbaar](https://github.com/crankyoldgit/IRremoteESP8266/releases/latest). Bekijk de [Release Notes](ReleaseNotes.md) voor alle belangrijke veranderingen.

#### Upgraden vanaf pre-v2.0
Het gebruik van de bibliotheek is enigszins gewijzigd in v2.0. Je zult het gebruik moeten aanpassen om te kunnen werken met v2.0 en hoger. Je kunt meer lezen over de vereiste aanpassingen op onze [Upgraden naar v2.0](https://github.com/crankyoldgit/IRremoteESP8266/wiki/Upgrading-to-v2.0) pagina.

#### Upgraden vanaf pre-v2.5
De bibliotheek defineert constanten nu niet meer als `#define`, maar gebruikt
[const](https://google.github.io/styleguide/cppguide.html#Constant_Names) met
de juiste naamgeving volgens de
[C++ style guide](https://google.github.io/styleguide/cppguide.html).
Dit kan ertoe leiden dat oude programma's niet compileren.
De meest extern gebruikte `#define`s  zijn _gealiased_ voor beperkte
compatibiliteit voor projecten die de oude stijl gebruiken. In de toekomst zal alleen de
nieuwe `kConstantName` stijl worden ondersteund voor nieuwe protocoltoevoegingen.

In het onwaarschijnlijke geval dat het je code breekt, dan heb je misschien verwezen naar
iets wat je waarschijnlijk niet had moeten doen. Gelukkig is het redelijk simpel om de nieuwe naam
te bepalen vanaf de oude, b.v. `CONSTANT_NAME` naar `kConstantName`.
Gebruik gezond verstand of onderzoek de code van de bibliotheek als dit van toepassing is op jouw code.

## Ondersteunde Protocollen
De details van de ondersteunde protocollen en apparaten staan
[hier](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/SupportedProtocols.md) vermeld.

## Probleemoplossing
Voordat je een probleem meldt of om hulp vraagt, graag eerst onze [Probleemoplossingsgids](https://github.com/crankyoldgit/IRremoteESP8266/wiki/Troubleshooting-Guide) volgen.

## Veelgestelde Vragen
Enkele antwoorden op veel veelgestelde vragen en problemen staan op onze [F.A.Q. wiki pagina](https://github.com/crankyoldgit/IRremoteESP8266/wiki/Frequently-Asked-Questions).

## Bibliotheek API Documentatie
De bibliotheek gebruikt [Doxygen](https://www.doxygen.nl/index.html) om [automatisch documentatie](https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/) toe te voegen aan de [API](https://en.wikipedia.org/wiki/Application_programming_interface) van de [bibliotheek](https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/).
Je kunt de documentatie [hier](https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/) vinden.

## Installatie
##### Officiële versies via de Arduino IDE v1.8+ (Windows & Linux)
1. Klik op de _"Schets"_ -> _"Bibliotheek gebruiken"_ -> _"Bibliotheken beheren..."_ menuknoppen.
1. Vul `IRremoteESP8266` in bij _"Filter je zoekresultaten..."_ rechtsboven de pop-up.
1. Klik op het IRremoteESP8266 resultaat van de zoekopdracht.
1. Selecteer de versie die je wilt installeren en klik op _"Installeren"_.

##### Handmatige installatie voor Windows
1. Klik op de _"Clone or Download"_ knop, en kies dan _"[Download ZIP](https://github.com/crankyoldgit/IRremoteESP8266/archive->master.zip)"_.
1. Pak de inhoud van de gedownloade zip uit.
1. Hernoem de uitgepakte map naar _"IRremoteESP8266"_.
1. Verplaats de map naar de bibliotheken map. (voor Windows: `C:\Gebruikers\GEBRUIKERSNAAM\Documenten\Arduino\libraries\`)
1. Herstart de Arduino IDE.
1. Bekijk de voorbeelden.

##### Git gebruiken om de bibliotheken te installeren ( Linux )
```
cd ~/Arduino/libraries
git clone https://github.com/crankyoldgit/IRremoteESP8266.git
```
###### Om de bibliotheken te updaten naar de laatste versie
```
cd ~/Arduino/libraries/IRremoteESP8266 && git pull
```

## Bijdragen
Als je wilt [bijdragen](.github/CONTRIBUTING.md#how-can-i-contribute) aan dit project, hulp is altijd welkom bij:
- Het [melden](.github/CONTRIBUTING.md#reporting-bugs) van problemen en foutmeldingen
- Ideeën voor verbeteringen
- Verbeteringen van de documentatie
- [Het aanmaken van issues](.github/CONTRIBUTING.md#reporting-bugs) en [pull requests](.github/CONTRIBUTING.md#pull-requests)
- Het delen van deze bibliotheek

## Bijdragers
Bekijk alle bijdragers [hier](.github/Contributors.md)

## Bibliotheek Geschiedenis
Deze bibliotheek was oorspronkelijk gebaseerd op het werk van Ken Shirriff (https://github.com/shirriff/Arduino-IRremote/)

[Mark Szabo](https://github.com/crankyoldgit/IRremoteESP8266) heeft de IRsend class bijgewerkt om te werken op een ESP8266 en [Sebastien Warin](https://github.com/sebastienwarin/IRremoteESP8266) de ontvang & decodeer class (IRrecv).

Voor v2.0 is de bibliotheek bijna volledig herschreven met de mogelijkheden van de ESP8266 in het achterhoofd.
