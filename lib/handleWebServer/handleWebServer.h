#ifndef HANDLEWEBSERVER_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define HANDLEWEBSERVER_H
#include "WebServer.h"       //Local WebServer used to serve the configuration portal
#include "settings.h"
#include "WiFiSettings.h"
#include "handleText.h"

/* show bootstrap-like menu page */
void menu(WebServer &server, Settings * pSettings, WiFiSettings * pWifiSettings, String language, uint8_t menuButton);

/* sending data through sse */
// not language dependent
void sse(WebServer &server, Settings * pSettings, uint32_t revolutions, uint32_t viewPulsesPerMinute);

#endif