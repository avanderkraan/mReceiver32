#ifndef HANDLEMDNS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define HANDLEMDNS_H
#include "ESPmDNS.h"
#include "WiFi.h"

String getMDNSHostname();
bool setHostname(String hostname);
void startmDNS();
void addService(String hostname);
String findFirstFreeHostname();

#endif