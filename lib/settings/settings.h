#ifndef SETTINGS_H        // To make sure you don't declare the function more than once by including the header multiple times.
#define SETTINGS_H
#include "Preferences.h"  // used to store and read settings

class Settings
{
public:
  const char * PREFERENCE_NAMESPACE_SETTINGS = "settings";
  const char * PREFERENCE_LABEL_SETTINGS= "settings";
  const char * INIT_NUMBER = "0";
  const char * DEVICE_KEY = "1";
  const char * LANGUAGE = "2";
  const char * MAJOR = "10";
  const char * MINOR = "11";
  const char * PATCH = "12";
  const char * START_AS_ACCESSPOINT = "20";
  const char * TARGET_SERVER = "30";
  const char * TARGET_PORT = "31";
  const char * TARGET_PATH = "32";
  const char * ROLEMODEL = "40";
  const char * ROLEMODELRPM = "41";
  const char * STEPS_PER_REVOLUTION = "50";
  const char * MAXIMUM_SPEED = "51";
  const char * DIRECTION = "52";
  const char * MOTOR_INTERFACE_TYPE = "53";

  /* time between two sse pushes in ms */
  const uint16_t SSE_RETRY = 1000;

  /* wait period in milliseconds */
  uint32_t WAIT_PERIOD = 200;

private:
  /* 4 bytes to store, version of this firmware */
  uint8_t major = 0;   // max 2^8 = 256
  uint8_t minor = 1;   // max 2^8 = 256
  uint16_t patch = 4;  // max 2^16 = 65536

  /* data in the non-volatile-storage partition */
  Preferences preferences;

  /* start as Access Point or as Network client */
  bool startAsAccessPoint = false;

  /* factoryStartAsAccessPoint is false */
  bool factoryStartAsAccessPoint = false;

  /* interval for sending data to the target server */
  uint16_t sendPeriod = 2000;

  /* target server, max size = 32 */
  String targetServer = "http://www.draaiendemolens.nl";
  /* factoryTargetServer */
  String factoryTargetServer = "http://www.draaiendemolens.nl";

  /* target port */
  uint16_t targetPort = 80;
  /* factoryTargetPort server */
  uint16_t factoryTargetPort = 80;

  /* target path, max size = 16 */
  String targetPath = "/eat/";
  /* factoryTargetPath server */
  String factoryTargetPath = "/eat/";

  /* check for first saved initialization */
  const uint8_t INITCHECK = 60;

  /* 1 byte to store, holds check for first initialization */
  uint8_t initNumber = 0;
  /* 1 byte to store, factory setting, holds check for first initialization */
  uint8_t factoryInitNumber = 0;

  /* 37 bytes to store, together with the MAC address, the identification of a device */
  String deviceKey = "88888888-4444-4444-4444-121212121212";
  /* 37 bytes to store, factory setting, together with the MAC address, the identification of a device */
  String factoryDeviceKey = "88888888-4444-4444-4444-121212121212";

  /* 3 bytes to store, language */
  String language = "NL";

  /* 3 bytes to store, factory settings for language */
  String factoryLanguage = "NL";

  /* 17 bytes to store, last given IP number to the device from a WiFi network, will not be saved */
  String lastNetworkIP = "Unknown";

  /* roleModel, this is where the model gets the data from, max size = 32 */
  String roleModel = "None";
  /* factoryRoleModel, this is where the model gets the data from, max size = 32 */
  String factoryRoleModel = "None";  // None means no roleModel defined

  /* rolemodel maxRPM, arbitrary */
  uint8_t maxRoleModelRPM = 15;

  /* rolemodel RPM */
  uint8_t roleModelRPM = maxRoleModelRPM;

  /* motor properties */
  uint16_t stepsPerRevolution = 4076; // change this in the database to fit the number of steps per revolution
  uint16_t maxSpeed = 1000;           // each motortype has its own maximum speed
  int8_t direction = 1;              // 1 or -1; some motors are wired reversed
  uint8_t motorInterfaceType = 8;    // AccelStepper::HALF4WIRE;
  /* factory settings for motor_properties */
  uint16_t factoryStepsPerRevolution = 4076; // change this in the database to fit the number of steps per revolution
  uint16_t factoryMaxSpeed = 1000;           // each motortype has its own maximum speed
  int8_t factoryDirection = 1;              // 1 or -1; some motors are wired reversed
  uint8_t factoryMotorInterfaceType = 8;    // AccelStepper::HALF4WIRE;


public:
  Settings()
  {
    this->preferences = Preferences();
    // this->initSettings(); // is called through the browser
    // call bootSettings()) from setup() to initialize Settings data
  };

  ~Settings()
  {
  };

public:
/* initialize settings */
void bootSettings();

private:
  /* set the namespace for the storage */
  bool setNamespace();

  /* freeup the storage namespace, leave values stored */
  void endNamespace();

  /* converts a string of numbers to an integer */
  uint8_t atoi8_t(String s);

  /* converts a string of numbers to an integer */
  uint16_t atoi16_t(String s);

  /* converts a string of numbers to an unsigned 32 bits number */
  uint32_t atoi32_t(String s);

  /* check to see if the preferense settings are already there */
  bool isFreshInstallation();

  /* check to see if the Firmware has been updated */
  bool isUpdated();

  /* checks for new update, using the version number and sets the new version number */
  size_t setupUpdatedFirmware();

public:

  /* get version number, used for firmware updates */
  String getFirmwareVersion();

  /* does the initial setup of the settings and saves the values in Preferences, returns true if it succeeds */
  size_t setupPreferences();

  /* saves settings in Preferences, returns true if it succeeds */
  size_t saveSettings();

  /* erase settings, removes all Preferences keys, returns true if it succeeds */
  bool eraseSettings();

  /* set Settings value to factory values and saves the values in Preferences, returns true if it succeeds */
  size_t initSettings();

  /* get Settings from Preferences */
  bool getSettings();

  /* saves rolemodel setting */
  size_t saveRoleModelSetting();

  /* saves rolemodel RPM */
  size_t saveRoleModelRPM();

  /* saves DeviceKey in Preferences */
  size_t saveDeviceKey();

  /* saves the value startAsAccessPoint in Preferences  */
  size_t saveStartAsAccessPoint();

  /* saves targetServer in Preferences  */
  size_t saveTargetServerStuff();

  /* return deviceKey */
  String getDeviceKey();

  /* set deviceKey without saving it to Preferences */
  void setDeviceKey(String myDeviceKey);

  /* period for sending data to the target server */
  uint16_t getSendPeriod();

  /* process the request interval from seconds to milliseconds, coming from the target server */
  void setRequestInterval(String requestInterval);

  /* return factory setting for targetServer */
  String getFactoryTargetServer();

  /* targetServer */
  String getTargetServer();

  /* set target server */
  void setTargetServer(String targetServer);

  /* return factory setting for targetPort */
  uint16_t getFactoryTargetPort();

  /* targetPort */
  uint16_t getTargetPort();

  /* set target port */
  void setTargetPort(String port);

  /* return factory setting for targetPath */
  String getFactoryTargetPath();

  /* targetPath */
  String getTargetPath();

  /* set target path */
  void setTargetPath(String targetPath);

  /* return factory setting for roleModel */
  String getFactoryRoleModel();

  /* roleModel */
  String getRoleModel();

  /* set roleModel */
  void setRoleModel(String roleModel);

  /* get arbitrary value of maxRPM or the roleModel */
  uint8_t getMaxRoleModelRPM();

  /* get factory setting for stepsPerRevolution */
  uint16_t getFactoryStepsPerRevolution();

  /* get factory setting for the motors maximum speed */
  uint16_t getFactoryMaxSpeed();

  /* get factory setting for the motor direction */
  int8_t getFactoryDirection();

  /* get factory setting for the motor interface type (AccelStepper) */
  uint8_t getFactoryMotorInterfaceType();

  /* get setting for stepsPerRevolution */
  uint16_t getStepsPerRevolution();

  /* get setting for the motors maximum speed */
  uint16_t getMaxSpeed();

  /* get current rolemodel speedsetting */
  uint8_t getRoleModelRPM();

  /* get setting for the motor direction */
  int8_t getDirection();

  /* get setting for the motor interface type (AccelStepper) */
  uint8_t getMotorInterfaceType();

  /* set setting for stepsPerRevolution */
  void setStepsPerRevolution(uint16_t stepsPerRevolution);

  /* set setting for the motors maximum speed */
  void setMaxSpeed(uint16_t maxSpeed);

  /* set current rolemodel speed */
  void setRoleModelRPM(uint8_t roleModelRPM);

  /* set setting for the motor direction */
  void setDirection(int8_t direction);

  /* set setting for the motor interface type (AccelStepper) */
  void setMotorInterfaceType(uint8_t motorInterfaceType);

  /* saves settings for motor properties */
  size_t saveMotorSettings();

 /* returns factory setting beginAs AccessPoint for WiFi start-mode, translated to "ap" or "network" */
  String getFactoryStartModeWiFi();

  /* return start as Access point or as network client */
  bool beginAsAccessPoint();

  /* set start as Access point or as network client */
  void beginAsAccessPoint(bool beginAsAccessPointValue);

  /* language, automatically saved */
  size_t setLanguage(String language);

  /* language, automatically saved */
  String getLanguage();

  /* network station last known IP address, will not be saved saved */
  void setLastNetworkIP(String lastNetworkIP);

  /* network station last known IP address, will not be saved */
  String getLastNetworkIP();
};
#endif