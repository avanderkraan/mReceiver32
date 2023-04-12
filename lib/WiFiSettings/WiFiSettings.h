#ifndef WIFISETTINGS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define WIFISETTINGS_H
#include "WiFi.h"         // WiFi Library (you most likely already have this in your sketch)
#include "Preferences.h"  // used to store and read settings
#include "dencrypt.h"     // used to encrypt wifi data

class WiFiSettings
{
public:
  const char * PREFERENCE_NAMESPACE_WIFI = "wifisettings";
  const char * PREFERENCE_LABEL_WIFI = "wifisettings";  
  //const char * PREFERENCE_LABEL_WIFI = "nvs";  
  // with use of partition size 0x3000 is looked if the label had to be nvs, otherwise it did't work
  // or the command 'idf.py erase-flash all' did the job
  // now, the size is set to 0x4000
  //"wifisettings" seems to be nvs and nothing else for WiFi
  const char * SSID_ACCESSPOINT = "w10";
  const char * PASSWORD_ACCESSPOINT = "w11";
  const char * SSID_STATION = "w12";
  const char * PASSWORD_STATION = "w13";

  /* wait period in milliseconds */
  uint32_t WAIT_PERIOD = 200;

private:
  /* data in the non-volatile-storage partition */
  Preferences preferences;

  /* maximum length of SSID Name string, excluding NULL character */
  const uint8_t MAX_SSID = 32;
  
  /* minumum length of password string, excluding NULL character */
  const uint8_t MIN_PASSWORD = 8;

  /* maximum length of password string, excluding NULL character */
  const uint8_t MAX_PASSWORD = 32;
  
  /* SSID */
  String ssidAccessPoint = "ESP-";

  /* Password */
  String passwordAccessPoint = "administrator";

  /* SSID */
  String ssidNetwork = "";

  /* Password */
  String passwordNetwork = "";

public:
  WiFiSettings()
  {
    this->preferences = Preferences();
    // Storage for AP and Network SSID, plus AP and Network Password
    // assume fresh start if key SSID_ACCESSPOINT is not present
    // call bootWiFi() from setup() to initialize WiFi data
  };

  ~WiFiSettings()
  {
  };

  /* initialize WiFiSettings with WiFi data */
  void bootWiFi();

  /* set the namespace for the storage */
  bool setNamespace();

  /* freeup the storage namespace, leave values stored */
  void endNamespace();

  /* check to see if the preferense settings are already there */
  bool isFreshInstallation();
 
  /* saves settings in EEPROM starting on EEPROM-address (default = 0), returns length of saved bytes */
  size_t saveAuthorizationAccessPoint();

  /* saves settings in EEPROM starting on EEPROM-address (default = 0), returns length of saved bytes */
  size_t saveAuthorizationNetwork();

  /* set the Access Point SSID in the class variable, max 32 char */
  bool setAccessPointSSID(String ssid);

  /* get the Access Point SSID from the class variable */
  String getAccessPointSSID();

  /* set the Access Point Password in the class variable, max 32 char */
  bool setAccessPointPassword(String password);

  /* get the Access Point Password in the class variable */
  String getAccessPointPassword();

  /* set the WiFi Network SSID in the class variable, max 32 char */
  bool setNetworkSSID(String ssid);

  /* get the WiFi Network SSID from the class variable */
  String getNetworkSSID();

  /* set the WiFi Network Password in the class variable, max 32 char */
  bool setNetworkPassword(String password);

  /* read AccessPoint SSID from EEPROM, does NOT save it in the class variable */
  String readAccessPointSSID();

  /* read AccessPoint Password from EEPROM, does NOT save it in the class variable */
  String readAccessPointPassword();

  /* read Network SSID from EEPROM, does NOT save it in the class variable */
  String readNetworkSSID();

 /* get the WiFi Network Password in the class variable */
  String getNetworkPassword();

  /* read Network Password from EEPROM, does NOT save it in the class variable */
  String readNetworkPassword();

  /* erase settings, set value ff on every EEPROM Access Point Settings address, set class AccessPoint vaiables SSID and Password to factory value, returns true if it succeeds */
  bool eraseAccessPointSettings();

  /* erase settings, set value ff on every EEPROM Network Settings address, set class Network vaiables SSID and Password to factory value, returns true if it succeeds */
  bool eraseNetworkSettings();

  /* erase settings, set value ff on every EEPROM WiFiSettings address, set class AccessPoint and Network vaiables SSID and Password to factory value, returns true if it succeeds */
  bool eraseWiFiSettings();

  /* encrypt wifi data */
  String encrypt(String plainText);

  /* decrypt wifi data */
  String decrypt(String encryptedText);

};

#endif