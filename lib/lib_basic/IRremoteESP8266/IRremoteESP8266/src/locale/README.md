# Internationalisation (I18N) & Locale Files

This directory contains the files used by the library to store the text it uses. If you want to add support for a language, this is the
correct place. If you are adding text strings to a routine, you should use the ones here.

## Changing the language/locale used by the library.
There are several ways to change which locale file is used by the library. Use which ever one suits your needs best.
To keep the space used by the library to a minimum, all methods require the change to happen at compile time.
There is _no_ runtime option to change locales.

### Change `_IR_LOCALE_` in the `src/IRremoteESP8266.h` file.
In the [IRremoteESP8266.h](../IRremoteESP8266.h#L57-L59) file, find and locate the lines that look like:
```c++
#ifndef _IR_LOCALE_
#define _IR_LOCALE_ en-AU
#endif  // _IR_LOCALE_
```

Change `en-AU` to the language & country that best suits your needs. e.g. `de-DE` for Germany/German.

### Use a compile-time build flag.
Use the compiler flag: `-D_IR_LOCALE_=en-AU` when compiling the library. Especially when compiling the `IRtext.cpp` file.
Change `en-AU` to a value which matches one of the file names in this directory. e.g. `de-DE` for Germany/German, which will use
the `de_DE.h` file.

### Use the appropriate pre-prepared build environment. _(PlatformIO only)_
If you examine the `platformio.ini` file located in the same directory as the example code you may find pre-setup compile environments
for the different supported locales.
Choose the appropriate one for you language by asking PlatformIO to build or upload using that environment.
e.g. See `IRrecvDumpV2`'s [platformio.ini](../../examples/IRrecvDumpV2/platformio.ini)

### Use a custom `build_flags`. _(PlatformIO only)_
Edit the `platformio.ini` file in the directory containing your example/source code.
Either in the default PlatformIO environment (`[env]`), or in which ever PlatformIO environment you using,
change or add the following line:
```
build_flags = -D_IR_LOCALE_=en-AU ; Or use which ever locale variable you want.
```

Every time you change that line, you should do a `pio clean` or choose the `clean` option from the build menu, to ensure a fresh copy
of `IRtext.o` is created.

## Adding support for a new locale/language.

Only [ASCII](https://en.wikipedia.org/wiki/ASCII#8-bit_codes)/[UTF-8](https://en.wikipedia.org/wiki/UTF-8) 8-bit characters are supported.
[Unicode](https://en.wikipedia.org/wiki/Unicode) is **not** supported. Unicode may work. It may not. It's just not supported.
i.e. If Arduino's `Serial.print()` can handle it, it will probably work.

### Copy/create a new locale file in this directory.
Copy [en-AU.h](en-AU.h) or which every is a closer fit for your language to `xx-YY.h` where `xx` is the [ISO code](https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes) for the language.
e.g. `en` is English. `de` is German etc. and `YY` is the ISO country code. e.g. `AU` is Australia.
Modify the comments and all `LOCALE_EN_AU_H_`s in the file to `LOCALE_XX_YY_H_` for your locale.


### Override any `#&zwj;define` values that reside in `defaults.h`
Go through the [defaults.h](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/locale/defaults.h) file, and find any `#&zwj;define` lines that define a macro starting with `D_` that has text
that needs to change for your locale.
Copy or create a corresponding `#&zwj;define D_STR_HELLOWORLD "Hello World"` in your `xx-YY.h` file, and translate the text appropriately
e.g. `#&zwj;define D_STR_HELLOWORLD "Bonjour le monde"` (French)

Any values you `#&zwj;define` in `xx-YY.h` will override the corresponding value in the [defaults.h](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/locale/defaults.h) file.

### Supporting a dialect/regional variant of another _existing_ language/locale.
Similar to the previous step, if you only need to modify a small subset of the strings used in another locale file, then include the
other locale file and then make sure to `#&zwj;undef` any strings that need to be (re-)changed.
See the [Swiss-German](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/locale/de-CH.h) for an example of how to do this. i.e. It `#&zwj;include "locale/de-DE.h"`s the German locale, and
redefines any strings that are not standard [German](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/locale/de-DE.h).

## Adding new text strings to the library.
If you need to add an entirely new string to the library to support some feature etc. e.g. _"Widget"_.
You should first understand how the library tries to do this such that it is easy to support different languages for it.

1. Use a constant named `kWidgetStr` in the appropriate statement in the `.cpp` file.
2. Edit [IRtext.cpp](IRtext.cpp), and add the appropriate line for your new constant. e.g.
```c++
String kWidgetStr = D_STR_WIDGET;
```
The `kWidgetStr` variable will house the sole copy of the string for the entire library. This limits any duplication.
The `D_STR_WIDGET` macro will be what is targeted by the different language / locales files.

3. Edit [locale/defaults.h](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/locale/defaults.h), and add the appropriate stanza for your new string. e.g.
```c++
#ifndef D_STR_WIDGET
#define D_STR_WIDGET "Turbo"
#endif  // D_STR_WIDGET
```


4. _(Manual)_ Update [IRtext.h](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/IRtext.h), and add the appropriate line for your new constant. e.g.
```c++
extern const String kWidgetStr;
```
For any file that `#&zwj;include <IRtext.h>`s this file, it will tell it that the string is stored elsewhere,
and to look for it elsewhere at the object linking stage of the build. This is what makes the string be referenced from a central location.

4. _(Automatic)_ Run `tools/generate_irtext_h.sh` to update [IRtext.h](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/IRtext.h).
In the [src/locale](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/locale/) directory. Run the `../../tools/generate_irtext_h.sh` command. It will update the file for you automatically.
