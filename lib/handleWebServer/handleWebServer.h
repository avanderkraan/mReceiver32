#ifndef HANDLEWEBSERVER_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define HANDLEWEBSERVER_H
#include "WebServer.h"       //Local WebServer used to serve the configuration portal
#include "settings.h"
#include "WiFiSettings.h"

/* show the help page */
void help(WebServer &server, Settings * pSettings);
void help_nl(WebServer &server, Settings * pSettings);
/* choose spinning */
void spin(WebServer &server, Settings * pSettings);
void spin_nl(WebServer &server, Settings * pSettings);
/* show info mode */
void info(WebServer &server, Settings * pSettings, WiFiSettings * pWifiSettings);
void info_nl(WebServer &server, Settings * pSettings, WiFiSettings * pWifiSettings);
/* choose wifi connection, (Access Point or Station -todo: get SSID for Station-) */
void wifi(WebServer &server, Settings * pSettings, WiFiSettings * pWifiSettings);
void wifi_nl(WebServer &server, Settings * pSettings, WiFiSettings * pWifiSettings);
/* sending data through sse */
// not language dependent
void sse(WebServer &server, Settings * pSettings, uint32_t revolutions, uint32_t viewPulsesPerMinute);

#endif