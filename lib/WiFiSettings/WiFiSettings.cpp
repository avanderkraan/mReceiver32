#include "WiFiSettings.h"

bool WiFiSettings::setNamespace()
{
    return this->preferences.begin(PREFERENCE_NAMESPACE_WIFI, false, "nvs");  // partition label?
}

void WiFiSettings::endNamespace()
{
    this->preferences.end();
}

size_t WiFiSettings::saveAuthorizationAccessPoint()
{
    size_t result = 0;
    if (this->setNamespace())
    {
        result += this->preferences.putString(SSID_ACCESSPOINT, this->ssidAccessPoint);
        result += this->preferences.putString(PASSWORD_ACCESSPOINT, this->passwordAccessPoint);

        //char myssidAccessPoint[33];  // one more for the null character
        //char myPasswordAccessPoint[33];  // one more for the null character
        this->endNamespace();
    }
    return result;
}

size_t WiFiSettings::saveAuthorizationNetwork()
{
    size_t result = 0;
    if (this->setNamespace())
    {
        result += this->preferences.putString(SSID_STATION, this->ssidNetwork);
        result += this->preferences.putString(PASSWORD_STATION, this->passwordNetwork);

        //char myssidNetwork[33];  // one more for the null character
        //char myPasswordNetwork[33];  // one more for the null character
        this->endNamespace();
    }
    return result;
}

bool WiFiSettings::eraseWiFiSettings() {
    bool result = false;
    if (this->setNamespace())
    {
        this->passwordAccessPoint = "";
        this->ssidAccessPoint = "";
        this->passwordNetwork = "";
        this->ssidNetwork = "";
        result = this->preferences.clear();
        this->endNamespace();
    }
    return result;
}

bool WiFiSettings::eraseAccessPointSettings() {
    bool result = false;
    if (this->setNamespace())
    {
        this->passwordAccessPoint = "";
        this->ssidAccessPoint = "";
        result = (this->preferences.remove(SSID_ACCESSPOINT) &&
                  this->preferences.remove(PASSWORD_ACCESSPOINT));
        this->endNamespace();
    }
    return result;
}

bool WiFiSettings::eraseNetworkSettings() {
    bool result = false;
    if (this->setNamespace())
    {
        this->passwordNetwork = "";
        this->ssidNetwork = "";
        result = (this->preferences.remove(SSID_STATION) &&
                  this->preferences.remove(PASSWORD_STATION));
        this->endNamespace();
    }
    return result;
}

bool WiFiSettings::setAccessPointSSID(String ssid)
{
    if (sizeof(ssid) <= 33)
    {
        this->ssidAccessPoint = ssid;
        return true;
    }
    return false;
}

String WiFiSettings::getAccessPointSSID()
{
    return this->ssidAccessPoint;
}

bool WiFiSettings::setAccessPointPassword(String password)
{
    if (sizeof(password) <= 33)
    {
        this->passwordAccessPoint = password;
        return true;
    }
    return false;
}

String WiFiSettings::getAccessPointPassword()
{
    return this->passwordAccessPoint;
}

bool WiFiSettings::setNetworkSSID(String ssid)
{
    if (sizeof(ssid) <= 33)
    {
        this->ssidNetwork = ssid;
        return true;
    }
    return false;
}

String WiFiSettings::getNetworkSSID()
{
    return this->ssidNetwork;
}

bool WiFiSettings::setNetworkPassword(String password)
{
    if (sizeof(password) <= 33)
    {
        this->passwordNetwork = password;
        return true;
    }
    return false;
}

String WiFiSettings::getNetworkPassword()
{
    return this->passwordNetwork;
}

String WiFiSettings::readAccessPointSSID()
{
    Serial.println("bij readAccessPointSSID");
    Serial.println(this->preferences.freeEntries());
    Serial.println(this->setNamespace());
    Serial.println("bij readAccessPointSSID");

    String result = "";
    if (this->setNamespace())
    {
        //char myAccessPointSSID[33];
        result = this->preferences.getString(SSID_ACCESSPOINT, "");
        this->endNamespace();
    }
    return result;
}

String WiFiSettings::readAccessPointPassword()
{
    String result = "";
    if (this->setNamespace())
    {
        //char myAccessPointPassword[33];
        result = this->preferences.getString(PASSWORD_ACCESSPOINT, "");
        Serial.print("bij readAccessPointPassword");
        Serial.println(sizeof(result));
        Serial.println(result);
        Serial.print("bij readAccessPointPassword");
        this->endNamespace();
    }
    return result;
}

String WiFiSettings::readNetworkSSID()
{
    String result = "";
    if (this->setNamespace())
    {
        //char myNetworkSSID[33];
        result = this->preferences.getString(SSID_STATION, "");
        this->endNamespace();
    }
    return result;
}

String WiFiSettings::readNetworkPassword()
{
    String result = "";
    if (this->setNamespace())
    {
        //char myNetworkPassword[33];
        result = this->preferences.getString(PASSWORD_STATION, "");
        this->endNamespace();
    }
    return result;
}