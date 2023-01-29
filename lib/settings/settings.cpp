#include "settings.h"

/*
Char	        putChar(const char* key, int8_t value)
Unsigned Char	putUChar(const char* key, int8_t value)
Short	        putShort(const char* key, int16_t value)
Unsigned Short	putUShort(const char* key, uint16_t value)
Int	            putInt(const char* key, int32_t value)
Unsigned Int	putUInt(const char* key, uint32_t value)
Long	        putLong(const char* key, int32_t value)
Unsigned Long	putULong(const char* key, uint32_t value)
Long64	        putLong64(const char* key, int64_t value)
Unsigned Long64	putULong64(const char* key, uint64_t value)
Float	        putFloat(const char* key, const float_t value)
Double	        putDouble(const char* key, const double_t value)
Bool	        putBool(const char* key, const bool value)
String	        putString(const char* key, const String value)
Bytes	        putBytes(const char* key, const void* value, size_t len)
*/

bool Settings::setNamespace()
{
  return this->preferences.begin(this->PREFERENCE_NAMESPACE_SETTINGS, false, this->PREFERENCE_LABEL_SETTINGS);  // partition label?
}

void Settings::endNamespace()
{
  this->preferences.end();
}

void Settings::bootSettings()
{
// this->preferences.begin(this->PREFERENCE_NAMESPACE_SETTINGS, false, this->PREFERENCE_LABEL_SETTINGS);  // partition label?
    this->setupPreferences();
    this->setupUpdatedFirmware();
}

String Settings::getFirmwareVersion()
{
  return String(this->major) + "." + String(this->minor) + "." + String(this->patch);
}

String Settings::getDeviceKey()
{
  return this->deviceKey;
}

void Settings::setDeviceKey(String myDeviceKey)
{
  this->deviceKey = myDeviceKey;
}

uint16_t Settings::getSendPeriod()
{
  return this->sendPeriod;
}

void Settings::setRequestInterval(String requestInterval)
{
  uint16_t result = this->atoi16_t(requestInterval);
  if (result == 0)
  {
    result = 5;  // set to 5 seconds
  }
  this->sendPeriod = result * 1000;  // milliseconds
}

String Settings::getTargetServer()
{
  return this->targetServer;
}

uint16_t Settings::getTargetPort()
{
  return this->targetPort;
}

String Settings::getTargetPath()
{
  return this->targetPath;
}

String Settings::getFactoryRoleModel()
{
  return this->factoryRoleModel;
}

String Settings::getRoleModel()
{
  return this->roleModel;
}

void Settings::setRoleModel(String roleModel)
{
  this->roleModel = roleModel;
  this->saveRoleModelSetting();
}

uint8_t Settings::getMaxRoleModelRPM()
{
  return this->maxRoleModelRPM;
}

uint16_t Settings::getFactoryStepsPerRevolution()
{
  return this->factoryStepsPerRevolution;
}

uint16_t Settings::getStepsPerRevolution()
{
  return this->stepsPerRevolution;
}

void Settings::setStepsPerRevolution(uint16_t stepsPerRevolution)
{
  this->stepsPerRevolution = stepsPerRevolution;
}

uint16_t Settings::getFactoryMaxSpeed()
{
  return this->factoryMaxSpeed;
}

uint16_t Settings::getMaxSpeed()
{
  return this->maxSpeed;
}

void Settings::setMaxSpeed(uint16_t maxSpeed)
{
  this->maxSpeed = maxSpeed;
}

int8_t Settings::getFactoryDirection()
{
  return this->factoryDirection;
}

int8_t Settings::getDirection()
{
  return this->direction;
}

void Settings::setDirection(int8_t direction)
{
  this->direction = direction;
}

uint8_t Settings::getFactoryMotorInterfaceType()
{
  return this->factoryMotorInterfaceType;
}

uint8_t Settings::getMotorInterfaceType()
{
  return this->motorInterfaceType;
}

void Settings::setMotorInterfaceType(uint8_t motorInterfaceType)
{
  this->motorInterfaceType = motorInterfaceType;
}

uint8_t Settings::atoi8_t(String s) 
{
    uint8_t i, n;
    n = 0;
    for (i = 0; s[i] >= '0' && s[i] <= '9'; i++)
        n = 10 * n +(s[i] - '0');
    return n;
}

uint16_t Settings::atoi16_t(String s) 
{
    uint8_t i;
    uint16_t n;
    n = 0;
    for (i = 0; s[i] >= '0' && s[i] <= '9'; i++)
        n = 10 * n +(s[i] - '0');
    return n;
}

uint32_t Settings::atoi32_t(String s) 
{
    uint8_t i;
    uint32_t n;
    n = 0;
    for (i = 0; s[i] >= '0' && s[i] <= '9'; i++)
        n = 10 * n +(s[i] - '0');
    return n;
}

size_t Settings::setupPreferences()
{
    if (this->isFreshInstallation() == true)
    {
        // set settings as initialized
        this->initNumber = this->INITCHECK;
        // clear everything for this preference
        this->setNamespace();
        this->preferences.clear();
        this->endNamespace();
        // and save fresh settings
        this->saveSettings();
    }
    return this->getSettings();
}

size_t Settings::setupUpdatedFirmware()
{
    size_t result = 0;
    if (this->isUpdated())
    {
        if (this->setNamespace())
        {
            result += this->preferences.putUChar(this->MAJOR, this->major);
            result += this->preferences.putUChar(this->MINOR, this->minor);
            result += this->preferences.putUShort(this->PATCH, this->patch);
            this->endNamespace();
        }
    }
    return result;
}

size_t Settings::saveSettings()
{
    size_t result = 0;
    if (this->setNamespace())
    {
        result += this->preferences.putUChar(this->INIT_NUMBER, this->initNumber);
        result += this->preferences.putUChar(this->MAJOR, this->major);
        result += this->preferences.putUChar(this->MINOR, this->minor);
        result += this->preferences.putUShort(this->PATCH, this->patch);
        result += this->preferences.putString(this->LANGUAGE, this->language);
        result += this->preferences.putBool(this->START_AS_ACCESSPOINT, this->startAsAccessPoint);
        result += this->preferences.putString(this->TARGET_SERVER, this->targetServer);
        result += this->preferences.putUShort(this->TARGET_PORT, this->targetPort);
        result += this->preferences.putString(this->TARGET_PATH, this->targetPath);
        result += this->preferences.putString(this->ROLEMODEL, this->roleModel);
        result += this->preferences.putChar(this->ROLEMODELRPM, this->roleModelRPM);
        result += this->preferences.putUShort(this->STEPS_PER_REVOLUTION, this->stepsPerRevolution);
        result += this->preferences.putUShort(this->MAXIMUM_SPEED, this->maxSpeed);
        result += this->preferences.putUChar(this->DIRECTION, this->direction);
        result += this->preferences.putUChar(this->MOTOR_INTERFACE_TYPE, this->motorInterfaceType);
        result += this->preferences.putString(this->DEVICE_KEY, this->deviceKey);
        this->endNamespace();
    }
    return result;
}

bool Settings::isUpdated() {
    bool result = false;
    if (this->setNamespace())
    {
        uint8_t currentMajor = this->major;
        uint8_t currentMinor = this->minor;
        uint16_t currentPatch = this->patch;

        currentMajor = this->preferences.getUChar(this->MAJOR);
        currentMinor = this->preferences.getUChar(this->MINOR);
        currentPatch = this->preferences.getUShort(this->PATCH);
        
        result = this->major != currentMajor &&
                 this->minor != currentMinor &&
                 this->patch != currentPatch;
        this->endNamespace();
    }
    return result;
}

bool Settings::isFreshInstallation() {
    // if result == true then default values have to be set in preferences
    // this happens when the preference is not alreay in use
    //           or when the INITCHECK got a new value than was stored in "initNr"
    bool result = true;  // true means: set default values in preferences
    if (this->setNamespace())
    {
        this->initNumber = this->preferences.getUChar(this->INIT_NUMBER);
        result = (this->initNumber == 0 ||
                  this->initNumber != this->INITCHECK);
        this->endNamespace();
    }
    return result;
}

bool Settings::eraseSettings() {
    bool result = false;
    if (this->setNamespace())
    {
        result = this->preferences.clear();
        this->endNamespace();
    }
    return result;
}

size_t Settings::initSettings()
{
    size_t result = 0;
    if (this->setNamespace())
    {
        result += this->preferences.putUChar(this->INIT_NUMBER, this->factoryInitNumber);
        result += this->preferences.putUChar(this->MAJOR, this->major);
        result += this->preferences.putUChar(this->MINOR, this->minor);
        result += this->preferences.putUShort(this->PATCH, this->patch);
        result += this->preferences.putString(this->LANGUAGE, this->factoryLanguage);
        result += this->preferences.putBool(this->START_AS_ACCESSPOINT, this->factoryStartAsAccessPoint);
        result += this->preferences.putString(this->TARGET_SERVER, this->factoryTargetServer);
        result += this->preferences.putUShort(this->TARGET_PORT, this->factoryTargetPort);
        result += this->preferences.putString(this->TARGET_PATH, this->factoryTargetPath);
        result += this->preferences.putString(this->ROLEMODEL, this->factoryRoleModel);
        result += this->preferences.putChar(this->ROLEMODELRPM, this->roleModelRPM);
        result += this->preferences.putUShort(this->STEPS_PER_REVOLUTION, this->factoryStepsPerRevolution);
        result += this->preferences.putUShort(this->MAXIMUM_SPEED, this->factoryMaxSpeed);
        result += this->preferences.putBool(this->DIRECTION, this->factoryDirection);
        result += this->preferences.putUChar(this->MOTOR_INTERFACE_TYPE, this->factoryMotorInterfaceType);
        result += this->preferences.putString(this->DEVICE_KEY, this->factoryDeviceKey);

        this->endNamespace();
    }
    return result;
}

bool Settings::getSettings()
{
    bool result = false;
    if (this->setNamespace())
    {
        this->initNumber = this->preferences.getUChar(this->INIT_NUMBER);
        this->major = this->preferences.getUChar(this->MAJOR);
        this->minor = this->preferences.getUChar(this->MINOR);
        this->patch = this->preferences.getUShort(this->PATCH);
        this->language = this->preferences.getString(this->LANGUAGE);
        this->startAsAccessPoint = this->preferences.getBool(this->START_AS_ACCESSPOINT);
        this->targetServer = this->preferences.getString(this->TARGET_SERVER);
        this->targetPort = this->preferences.getUShort(this->TARGET_PORT);
        this->targetPath = this->preferences.getString(this->TARGET_PATH);
        this->roleModel = this->preferences.getString(this->ROLEMODEL);
        this->roleModelRPM = this->preferences.getChar(this->ROLEMODELRPM);
        this->stepsPerRevolution = this->preferences.getUShort(this->STEPS_PER_REVOLUTION);
        this->maxSpeed = this->preferences.getUShort(this->MAXIMUM_SPEED);
        this->direction = this->preferences.getBool(this->DIRECTION);
        this->motorInterfaceType = this->preferences.getUChar(this->MOTOR_INTERFACE_TYPE);
        this->deviceKey = this->preferences.getString(this->DEVICE_KEY);

        this->endNamespace();
        result = true;
    }
    return result;
}

size_t Settings::saveDeviceKey()
{
    size_t result = 0;
    if (this-setNamespace())
    {
      if (this->deviceKey != this->preferences.getString(this->DEVICE_KEY))
      {
        this->preferences.remove(this->DEVICE_KEY);
        result += this->preferences.putString(this->DEVICE_KEY, this->deviceKey);
      }
      this->endNamespace();
    }
    return result;
}

size_t Settings::saveRoleModelSetting()
{
    size_t result = 0;
    if (this->setNamespace())
    {
      if (this->roleModel != this->preferences.getString(this->ROLEMODEL))
      {
        this->preferences.remove(this->ROLEMODEL);
        result += this->preferences.putString(this->ROLEMODEL, this->roleModel);
      }
      this->endNamespace();
    }
    return result;
}

size_t Settings::saveRoleModelRPM()
{
    size_t result = 0;
    if (this->setNamespace())
    {
      result += this->preferences.putChar(this->ROLEMODELRPM, this->roleModelRPM);
      this->endNamespace();
    }
    return result;
}

size_t Settings::saveStartAsAccessPoint()
{
    size_t result = 0;
    if (this->setNamespace())
    {
        result += this->preferences.putBool(this->START_AS_ACCESSPOINT, this->startAsAccessPoint);
        this->endNamespace();
    }
    return result;
}

size_t Settings::saveTargetServerStuff()
{
    size_t result = 0;
    if (this->setNamespace())
    {
      if (this->targetServer != this->preferences.getString(this->TARGET_SERVER))
      {
        this->preferences.remove(this->TARGET_SERVER);
        result += this->preferences.putString(this->TARGET_SERVER, this->targetServer);
      }
      result += this->preferences.putUShort(this->TARGET_PORT, this->targetPort);
      if (this->targetPath != this->preferences.getString(this->TARGET_PATH))
      {
        this->preferences.remove(this->TARGET_PATH);
        result += this->preferences.putString(this->TARGET_PATH, this->targetPath);
      }
      this->endNamespace();
    }
    return result;
}

size_t Settings::saveMotorSettings()
{
    size_t result = 0;
    if (this->setNamespace())
    {
        result += this->preferences.putUShort(this->STEPS_PER_REVOLUTION, this->stepsPerRevolution);
        result += this->preferences.putUShort(this->MAXIMUM_SPEED, this->maxSpeed);
        result += this->preferences.putBool(this->DIRECTION, this->direction);
        result += this->preferences.putUChar(this->MOTOR_INTERFACE_TYPE, this->motorInterfaceType);
        this->endNamespace();
    }
    return result;
}

bool Settings::beginAsAccessPoint()
{
  return this->startAsAccessPoint;
}

void Settings::beginAsAccessPoint(bool myBeginAsAccessPointValue)
{
  this->startAsAccessPoint = myBeginAsAccessPointValue;
}

String Settings::getFactoryStartModeWiFi()
{
  return this->factoryStartAsAccessPoint ? "ap" : "network";
}

String Settings::getFactoryTargetServer()
{
  return this->factoryTargetServer;
}

void Settings::setTargetServer(String targetServer)
{
  this->targetServer = targetServer;
}

uint16_t Settings::getFactoryTargetPort()
{
  return this->factoryTargetPort;
}

void Settings::setTargetPort(String targetPort)
{
  this->targetPort = this->atoi16_t(targetPort);
}

String Settings::getFactoryTargetPath()
{
  return this->factoryTargetPath;
}

void Settings::setTargetPath(String targetPath)
{
  this->targetPath = targetPath;
}

size_t Settings::setLanguage(String language)
{
    size_t result = 0;
    if (this->setNamespace())
    {
      this->language = language;
      if (this->language != this->preferences.getString(this->LANGUAGE))
      {
        this->preferences.remove(this->LANGUAGE);
        result += this->preferences.putString(this->LANGUAGE, this->language);
      }
      this->endNamespace();
    }
    return result;
}

String Settings::getLanguage()
{
  return this->language;
}

void Settings::setLastNetworkIP(String lastNetworkIP)
{
  this->lastNetworkIP = lastNetworkIP;
}

String Settings::getLastNetworkIP()
{
  return this->lastNetworkIP;
}

uint8_t Settings::getRoleModelRPM()
{
  return this->roleModelRPM;
}

void Settings::setRoleModelRPM(uint8_t roleModelSpeed)
{
  this->roleModelRPM = roleModelSpeed;
}
