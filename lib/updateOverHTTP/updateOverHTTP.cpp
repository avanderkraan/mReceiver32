#include "WiFi.h"          // WiFi Library (you most likely already have this in your sketch)
#include "HTTPUpdate.h"
#include "updateOverHTTP.h"

String updateOverHTTP(WiFiClient wifiClient, String serverURL, uint16_t serverPort, String uploadScript, String version) {
  String result = "";
  if (WiFi.getMode() == WIFI_MODE_STA)
  {
    uint8_t pos = serverURL.indexOf("://");
    if (pos > -1)
    {
      serverURL = serverURL.substring(pos + 3);  // get rid of protocol
    }
    HTTPUpdate httpUpdate;
    httpUpdate.rebootOnUpdate(false);
    t_httpUpdate_return ret = httpUpdate.update(wifiClient, serverURL, serverPort, uploadScript, version);
    switch(ret) {
        case HTTP_UPDATE_FAILED:
            //Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            result = UPDATEOVERHTTP_FAILED;
            break;
        case HTTP_UPDATE_NO_UPDATES:
            //Serial.println("[update] Update no Update");
            result = UPDATEOVERHTTP_NO_UPDATE;
            break;
        case HTTP_UPDATE_OK:
            //Serial.println("[update] Update ok"); // may not be called since we reboot the ESP
            result = UPDATEOVERHTTP_OK;
            break;
    }
  }
  else {
    //Serial.println("Device must be in station mode");
    result = UPDATEOVERHTTP_NO_INTERNET;
  }
  return result;
}
