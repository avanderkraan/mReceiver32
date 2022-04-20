#ifndef HANDLEMDNS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define HANDLEMDNS_H
#include "ESPmDNS.h"
#include "WiFi.h"

const char * getFirstFreeHostName(const char * hostName);
bool startmDNS();
bool _setStationHostname();
#endif