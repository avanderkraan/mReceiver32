#include "handleText.h"

String getText(uint8_t locale, char const *textArray[])
{
  uint8_t numberOfReplacements = 0;
  const char *replacement[] = {};
  return getText(locale, textArray, replacement, numberOfReplacements);
}

String getText(uint8_t locale, char const *textArray[], char const *replacement[], uint8_t numberOfReplacements)
{
  String result = textArray[locale];
  for (uint8_t i = 0; i < numberOfReplacements; i++)
  {
    result = replace(result, String(replacement[i]));
  }
  return result;
}

String replace(String text, String replacement)
{
  String result = text;
  uint8_t start = text.indexOf("${");
  if (start > 0)
  {
    result = text.substring(0, start);
    String rest = text.substring(start);
    uint8_t end = rest.indexOf("}");
    if (end > 0)
    {
      result += replacement + rest.substring(end + 1);
    }
  }
  return result;
}
