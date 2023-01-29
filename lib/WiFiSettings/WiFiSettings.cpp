#include "WiFiSettings.h"

bool WiFiSettings::setNamespace()
{
    return this->preferences.begin(this->PREFERENCE_NAMESPACE_WIFI, false, this->PREFERENCE_LABEL_WIFI);  // partition label?
}

void WiFiSettings::endNamespace()
{
    this->preferences.end();
}

void WiFiSettings::bootWiFi()
{
    //if (this->preferences.begin(this->PREFERENCE_NAMESPACE_WIFI, false, this->PREFERENCE_LABEL_WIFI))
    if (this->setNamespace())
    {
        String _password = this->preferences.getString(this->PASSWORD_ACCESSPOINT);
        this->endNamespace();
        if (_password.length() < this->MIN_PASSWORD)
        {
          this->setAccessPointSSID(this->ssidAccessPoint);
          this->setAccessPointPassword(this->passwordAccessPoint);
          this->saveAuthorizationAccessPoint();
        }
    }
}

size_t WiFiSettings::saveAuthorizationAccessPoint()
{
    size_t result = 0;
    if (this->setNamespace())
    {
        if (this->ssidAccessPoint != this->preferences.getString(this->SSID_ACCESSPOINT))
        {
            this->preferences.remove(this->SSID_ACCESSPOINT);
            result += this->preferences.putString(this->SSID_ACCESSPOINT, this->ssidAccessPoint);
        }
        String _encrypted_password = this->preferences.getString(this->PASSWORD_ACCESSPOINT);
        if (this->passwordAccessPoint != this->decrypt(_encrypted_password))
        {
            this->preferences.remove(this->PASSWORD_ACCESSPOINT);
            //result += this->preferences.putString(this->PASSWORD_ACCESSPOINT, this->passwordAccessPoint);
            result += this->preferences.putString(this->PASSWORD_ACCESSPOINT, this->encrypt(this->passwordAccessPoint));
        }
        this->endNamespace();
    }
    return result;
}

size_t WiFiSettings::saveAuthorizationNetwork()
{
    size_t result = 0;
    if (this->setNamespace())
    {
        if (this->ssidNetwork != this->preferences.getString(this->SSID_STATION))
        {
            this->preferences.remove(this->SSID_STATION);
            result += this->preferences.putString(this->SSID_STATION, this->ssidNetwork);
        }
        String _encrypted_password = this->preferences.getString(this->PASSWORD_STATION);
        if (this->passwordNetwork != this->decrypt(_encrypted_password))
        //if (this->passwordNetwork != this->preferences.getString(this->PASSWORD_STATION))
        {
            this->preferences.remove(this->PASSWORD_STATION);
            //result += this->preferences.putString(this->PASSWORD_STATION, this->passwordNetwork);
            result += this->preferences.putString(this->PASSWORD_STATION, this->encrypt(this->passwordNetwork));
        }
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
        result = (this->preferences.remove(this->SSID_ACCESSPOINT) &&
                  this->preferences.remove(this->PASSWORD_ACCESSPOINT));
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
        result = (this->preferences.remove(this->SSID_STATION) &&
                  this->preferences.remove(this->PASSWORD_STATION));
        this->endNamespace();
    }
    return result;
}

bool WiFiSettings::setAccessPointSSID(String ssid)
{
    if (sizeof(ssid) <= this->MAX_SSID)
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
    if (sizeof(password) <= this->MAX_PASSWORD)
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
    if (sizeof(ssid) <= this->MAX_SSID)
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
    if (sizeof(password) <= this->MAX_PASSWORD)
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
    String result = "";
    if (this->setNamespace())
    {
        //result = this->preferences.getString(this->SSID_ACCESSPOINT, this->ssidAccessPoint);
        result = this->preferences.getString(this->SSID_ACCESSPOINT);
        if (result.length() == 0)
        {
            result = this->ssidAccessPoint;
        }
        this->endNamespace();
    }
    return result;
}

String WiFiSettings::readAccessPointPassword()
{
    String result = "";
    if (this->setNamespace())
    {
        String _encrypted_password = this->preferences.getString(this->PASSWORD_ACCESSPOINT);
        result = this->decrypt(_encrypted_password);
        if (result.length() == 0)
        {
            result = this->passwordAccessPoint;
        }
        this->endNamespace();
    }
    return result;
}

String WiFiSettings::readNetworkSSID()
{
    String result = "";
    if (this->setNamespace())
    {
        result = this->preferences.getString(this->SSID_STATION);
        Serial.println(result.length());
        Serial.println(result.length() == 0);
        Serial.println(this->ssidNetwork);
        if (result.length() == 0)
        {
            result = this->ssidNetwork;
        }
        this->endNamespace();
    }
    return result;
}

String WiFiSettings::readNetworkPassword()
{
    String result = "";
    if (this->setNamespace())
    {
        String _encrypted_password = this->preferences.getString(this->PASSWORD_STATION);
        result = this->decrypt(_encrypted_password);
        //result = this->preferences.getString(this->PASSWORD_STATION);
        if (result.length() == 0)
        {
            result = this->passwordNetwork;
        }
        this->endNamespace();
    }
    return result;
}

String WiFiSettings::encrypt(String plainText)
{
  // method of Dencrypt can only be used once becasue the initialization vector changes with an encryption
  // leaving this scope takes care of calling the Dencrypt destructor
  Dencrypt dencrypt = Dencrypt();
  return dencrypt.encrypt(plainText);
}

String WiFiSettings::decrypt(String encryptedText)
{
  // method of Dencrypt can only be used once becasue the initialization vector changes with a decryption
  // leaving this scope takes care of calling the Dencrypt destructor
  Dencrypt dencrypt = Dencrypt();
  return dencrypt.decrypt(encryptedText);
}