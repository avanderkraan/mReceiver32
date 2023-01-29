#ifndef HANDLETEXT_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define HANDLETEXT_H
#include "Arduino.h"

// define locales
const uint8_t EN = 0;
const uint8_t NL = 1;

/* gives translated text */
String getText(uint8_t locale, char const *textArray[]);
String getText(uint8_t locale, char const *textArray[], char const *replacement[], uint8_t numberOfReplacements);
/* replaces placeholder with give replacement */
String replace(String text, String replacement);

#endif