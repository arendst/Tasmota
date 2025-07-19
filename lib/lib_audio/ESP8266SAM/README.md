# ESP8266SAM
Speech synthesis for ESP8266 and ESP-32 using ported SAM code

This is a port, wrapper, and update of the reverse-engineered speech synthesizer Software Automatic Mouth (SAM).  Utilize it with the [ESP8266Audio library](https://github.com/earlephilhower/ESP8266Audio/) to have your ESP speak via a DAC or a direct-drive speaker.  No web services are required, everything from text parsing to speech generation is done directly on the ESP.

[Original sources](https://github.com/s-macke/SAM)

This version has been reworked to generate full 8-bit speech formants as well as proper time-series waveforms.

## Usage
You will need:
1. The ESP8266Audio library installed in your Arduino/libraries folder ( git clone https://github.com/earlephilhower/ESP8266Audio )
2. An audio output device compatible with ESP8266Audio.  There are two described in its readme, and either the DAC or 1-transistor amplifier will work fine.

Then in your code instantiate an AudioOutput object, ->begin() it, and then simply create the SAM object and call Say().

## About SAM
SAM is a very basic speech synthesizer originally developed in 1979 and ported to the Commodore 64.  It was designed to run on very low memory 8-bit processors, and as such is a great for the ESP8266 with it's limited RAM.  

The quality of speech isn't phenomenal.  To paraphrase Samuel Johnson, "It is not done well; but you are surprised to find it done at all."  The same could be said of the decompiled, disassembled, C-translated source code.

Output is fixed at 22050Hz due to some hardcoded delays to help match C64 internal timing parameters.

The voice is formant generated and can be modified by setting things such as speed, pitch, mouth and throat configuration, and even sending in phonetic codes instead of English text.  See @s-macke's repository for more information.

## Cool uses of SAM
Jan Derogee has used ESP8266SAM to build a complete VIC-20 compatible speech synthesis cartridge that works with BASIC applications.  It also has a great background on early speech synthesis and a neat construction and demo video.  Check it out at https://janderogee.com/projects/SerialSpeechSynthesisSAM/SerialSpeechSynthesisSAM.htm


## License
While the ESP8266 wrapper is my own, the SAM software is a reverse-engineered version of a software published more than 34 years ago by "Don't ask Software".

The company no longer exists. Any attempt to contact the original authors failed. Hence S.A.M. can be best described as Abandonware (http://en.wikipedia.org/wiki/Abandonware)

As long this is the case I cannot put my code under any specific open source software license. However the software might be used under the "Fair Use" act (https://en.wikipedia.org/wiki/FAIR_USE_Act) in the USA.
