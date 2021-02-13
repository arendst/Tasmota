![IRremoteESP8266 Library](./assets/images/banner.svg)

[![Build Status](https://travis-ci.org/crankyoldgit/IRremoteESP8266.svg?branch=master)](https://travis-ci.org/crankyoldgit/IRremoteESP8266)
[![arduino-library-badge](https://www.ardu-badge.com/badge/IRremoteESP8266.svg?)](https://www.ardu-badge.com/IRremoteESP8266)
[![Average time to resolve an issue](http://isitmaintained.com/badge/resolution/crankyoldgit/IRremoteESP8266.svg)](http://isitmaintained.com/project/crankyoldgit/IRremoteESP8266 "Average time to resolve an issue")
[![Percentage of issues still open](http://isitmaintained.com/badge/open/crankyoldgit/IRremoteESP8266.svg)](http://isitmaintained.com/project/crankyoldgit/IRremoteESP8266 "Percentage of issues still open")
[![GitLicense](https://gitlicense.com/badge/crankyoldgit/IRremoteESP8266)](https://gitlicense.com/license/crankyoldgit/IRremoteESP8266)

Cette librairie vous permetra de **recevoir et d'envoyer des signaux** infrarouge sur le protocole [ESP8266](https://github.com/esp8266/Arduino) ou sur le protocole
[ESP32](https://github.com/espressif/arduino-esp32) en utilisant le [Arduino framework](https://www.arduino.cc/) qui utilise la norme 940nm IR LEDs et le module basique de reception d'onde IR. Exemple : TSOP{17,22,24,36,38,44,48}* modules etc.

## v2.7.15 disponible
Version 2.7.15 de la libraire est maintenant [disponible](https://github.com/crankyoldgit/IRremoteESP8266/releases/latest). Vous pouvez voir le [Release Notes](ReleaseNotes.md) pour tous les changements importants.

#### mise à jour depuis pre-v2.0
L'utilisation de la librairie à un peu changer depuis la version in v2.0. Si vous voulez l'utiliser vous devrez changer votre utilisation aussi. Vous pouvez vous renseigner sur les précondition d'utilisation ici : [Upgrade to v2.0](https://github.com/crankyoldgit/IRremoteESP8266/wiki/Upgrading-to-v2.0) page.

#### Mise à jour depuis pre-v2.5
La librairie à changer, elle n'utilise plus les constantes déclarées comme `#define` mais comme :
[const](https://google.github.io/styleguide/cppguide.html#Constant_Names) avec le nom approprié par le langage
[C++ style guide](https://google.github.io/styleguide/cppguide.html).
Il se peut que d'ancien programme ne compile pas.
Le cas le plus utilisé de `#define`s à été remplacé par  _aliased_ pour limiter
la compatibilité de revenir en arrière pour les vieux projet. En revenant en arrière seulement la
nouvelle `kConstantName` style est supporté.

Dans le cas peu probable, votre code serait cassé, alors vous avez peut-être fait référence à
quelque chose que vous ne devriez probablement pas avoir.Vous devez être capable de determiner le nouveau nom
qui remplacera l'ancien. exemple : `CONSTANT_NAME` par `kConstantName`.
Si vous avez un problème examinez le code pour trouver le problème.

## Protocoles supportés
Vous pouvez trouver le détails des protocoles et machines supportés
[here](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/SupportedProtocols.md).

## Dépannage
Avant de reporter un probème ou de demander de l'aide, essayez de suivre notre [guide de dépannage](https://github.com/crankyoldgit/IRremoteESP8266/wiki/Troubleshooting-Guide) first.

## Questions fréquentes
Les questions les plus fréquentes sont ici, avec des réponses [F.A.Q. wiki page](https://github.com/crankyoldgit/IRremoteESP8266/wiki/Frequently-Asked-Questions).

## Documentation API de la bibliothèque
Cette bibliothèque utilise [Doxygen](https://www.doxygen.nl/index.html) pour [documenter automatiquement](https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/) [l'API](https://en.wikipedia.org/wiki/Application_programming_interface) de la [bibliothèque](https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/). Vous pouvez le trouver [ici](https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/).

## Installation
##### Officiel releases avec l'Arduino IDE v1.8+ (Windows & Linux)
1. Cliquez sur _"Sketch"_ -> _"Include Library"_ -> _"Manage Libraries..."_ Menu items.
1. Entrez `IRremoteESP8266` dans le _"Filter your search..."_ barre de recherche en haut à droite.
1. Cliquez sur le IRremoteESP8266 pour avoir les résultats de la recherche.
1. Selectionnez la version que vous voulez installer et cliquez sur _"Install"_.

## Library API Documentation
This library uses [Doxygen](https://www.doxygen.nl/index.html) to [automatically document](https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/) the [library's](https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/) [API](https://en.wikipedia.org/wiki/Application_programming_interface).
You can find it [here](https://crankyoldgit.github.io/IRremoteESP8266/doxygen/html/).

##### Installation manuelle pour Windows
1. cliquez le boutton sur _"Clone or Download"_ , et  _"[Download ZIP](https://github.com/crankyoldgit/IRremoteESP8266/archive->master.zip)"_ on the page.
1. Extraire l'archive.
1. renommez le fichier par _"IRremoteESP8266"_.
1. déplacer le fichier dans votre fichier de bibliothèques. (Pour windows : `C:\Users\VOTRE_NOM\Documents\Arduino\libraries\`)
1. Redemarrez arduino IDE.
1. Regardez les exemples.

##### En utilisant GIT ( Linux )
```
cd ~/Arduino/libraries
git clone https://github.com/crankyoldgit/IRremoteESP8266.git
```
###### Pour se mettre à jour
```
cd ~/Arduino/libraries/IRremoteESP8266 && git pull
```

## Contribution
Si vous voulez  [contribuer](.github/CONTRIBUTING.md#how-can-i-contribute) au projet, pour les erreurs:
- [Reporting](.github/CONTRIBUTING.md#reporting-bugs) bug et erreurs
- Demander des améliorations
- Améliorer notre documentation
- [Création d'issues](.github/CONTRIBUTING.md#reporting-bugs) et [pull requests](.github/CONTRIBUTING.md#pull-requests)
- Parlez de cettre librairie à d'autres personnes

## Contributeurs
disponible [ici](.github/Contributors.md)

## Historique de la bibliothèque
Elle est basée sur le travail de Shirriff (https://github.com/shirriff/Arduino-IRremote/)

[Mark Szabo](https://github.com/crankyoldgit/IRremoteESP8266) à mis a jour la IRsend class pour qu'elle soit fonctionnelle sur ESP8266 et [Sebastien Warin](https://github.com/sebastienwarin/IRremoteESP8266) s'est occupé de la partie réception et décodage (IRrecv class).

Comme pour la version 2.0, la bibliothèque à été completement réécrite avec les ressources sur ESP8266.
