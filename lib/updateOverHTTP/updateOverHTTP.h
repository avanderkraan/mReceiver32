#ifndef UPATEOVERHTTP_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define UPATEOVERHTTP_H
#include "WiFi.h"          // WiFi Library (you most likely already have this in your sketch)
#include <esp_task_wdt.h>

const String UPDATEOVERHTTP_FAILED ="[update] Update Failed";
const String UPDATEOVERHTTP_NO_UPDATE = "[update] No Update";
const String UPDATEOVERHTTP_OK = "[update] Update ok";
const String UPDATEOVERHTTP_NO_INTERNET = "[update] No connection with the server";
const uint32_t WDT_UPDATE_OTA_S = 15;   // see https://github.com/esphome/esphome/pull/4172
/* 
serverURL is the hostname
serverPort the portnumber
uploadScript is the path and filename of the uploadscript on the server
version is an optional parameter that can give the current version
*/  
String updateOverHTTP(WiFiClient wifiClient, String serverURL, uint16_t serverPort, String uploadScript, String version);

#endif