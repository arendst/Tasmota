// Copyright 2019 - David Conran (@crankyoldgit)

#ifndef I18N_H_
#define I18N_H_

#include "IRremoteESP8266.h"

// Load the appropriate locale header file.
#ifndef _IR_LOCALE_
#define _IR_LOCALE_ en-AU
#endif  // _IR_LOCALE_

#define ENQUOTE_(x) #x
#define ENQUOTE(x) ENQUOTE_(x)

// Load the desired/requested locale.
#ifdef _IR_LOCALE_
#include ENQUOTE(locale/_IR_LOCALE_.h)
#endif  // _IR_LOCALE_

// Now that any specific locale has been loaded, we can safely load the defaults
// as the defaults should not override anything that has now set.
#include "locale/defaults.h"

#endif  // I18N_H_
