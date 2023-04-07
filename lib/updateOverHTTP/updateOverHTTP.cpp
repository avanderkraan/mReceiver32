#include "WiFi.h"          // WiFi Library (you most likely already have this in your sketch)
#include "HTTPUpdate.h"
#include "updateOverHTTP.h"

String updateOverHTTP(WiFiClient wifiClient, String serverURL, uint16_t serverPort, String uploadScript, String version) {
  String result = "";
  Serial.print("mode in updateoverhttp is wifi_mode_sta?: ");
  Serial.println(WiFi.getMode() == WIFI_MODE_STA);
  if (WiFi.getMode() == WIFI_MODE_STA)
  {
    uint8_t pos = serverURL.indexOf("://");
    if (pos > -1)
    {
      serverURL = serverURL.substring(pos + 3);  // get rid of protocol
    }
    Serial.print("set esp_task_wdt_deinit() to 30 because calling device needs this time");
    //Serial.println(esp_task_wdt_deinit());
    Serial.println(esp_task_wdt_init(WDT_UPDATE_OTA_S, 0) == ESP_OK);
    HTTPUpdate httpUpdate;
    httpUpdate.rebootOnUpdate(false);
    t_httpUpdate_return ret = httpUpdate.update(wifiClient, serverURL, serverPort, uploadScript, version);
    delay(200);
    switch(ret) {
        case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
            result = UPDATEOVERHTTP_FAILED;
            break;
        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("[update] Update no Update");
            result = UPDATEOVERHTTP_NO_UPDATE;
            break;
        case HTTP_UPDATE_OK:
            Serial.println("[update] Update ok"); // may not be called since we reboot the ESP
            result = UPDATEOVERHTTP_OK;
            Serial.print("set to default value: 5 ");
            Serial.println(esp_task_wdt_init(CONFIG_ESP_TASK_WDT_TIMEOUT_S, 0) == ESP_OK);
            break;
    }
  }
  else {
    // Device must be in station mode;
    result = UPDATEOVERHTTP_NO_INTERNET;
  }
  return result;
}
