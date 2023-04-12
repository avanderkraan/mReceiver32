#include "Arduino.h"
#include <Preferences.h>
//#include "WiFi.h"          // WiFi Library (you most likely already have this in your sketch)
#include "WebServer.h"     // Local WebServer used to serve the configuration portal

#include "handlemDNS.h"
//#include "ESPmDNS.h"
//#include <WiFiUdp.h>

#include "updateOverHTTP.h"

#include "settings.h"
#include "WiFiSettings.h"
#include "handleWebServer.h"
#include "handleHTTPClient.h"
#include <AccelStepper.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SSD1306.h>

// workaround to prevent brownout detecting when starting WiFi is to 
// NOT detect brownout
// https://arduino.stackexchange.com/questions/76690/esp32-brownout-detector-was-triggered-upon-wifi-begin
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
// in setup() : WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

// debug, zie setup()
//#include "esp_flash_encrypt.h"
//#include <nvs.h>
//#include <nvs_flash.h>

Preferences pref;


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define I2C_SDA GPIO_NUM_1 //23//esp32 // Wire SDA
#define I2C_SCL GPIO_NUM_0 //22//esp32 // Wire SCL

bool hasI2C = false;

const int motorPin1 = GPIO_NUM_7; //27;//esp32 //4;   // IN1
const int motorPin2 = GPIO_NUM_6; //26;//esp32 //5;   // IN2
const int motorPin3 = GPIO_NUM_5; //25;//esp32 //6;   // IN3
const int motorPin4 = GPIO_NUM_4;//33;//esp32 //7;   // IN4

// WIFI URL: http://192.168.4.1/ or http://model.local/
/////////////////////
// Pin Definitions //
/////////////////////

// D5 gives troubles when it is high at the start.

const uint8_t BUTTON = GPIO_NUM_9;           // was D8 // Digital pin to read button-push

const uint8_t ACCESSPOINT_LED = GPIO_NUM_3;//4;esp32 //3;  // was D1 
const uint8_t STATION_LED = GPIO_NUM_10;//2;esp32      // was D2

// variables for reset to STA mode
unsigned long lastStartMillis = millis();
const uint32_t NO_STA_MAX_MILLIS = 300000;  // 300 000 millis leaves an interval of 5 minutes
bool eepromStartModeAP = false;     // see setup, holds the startmode from eeprom

const uint32_t RELAX_PERIOD = 2;    // Is also a small energy saving, in milliseconds
const uint32_t TOO_LONG = 60000;    // after this period the pulsesPerMinute = 0 (in milliseconds)
bool permissionToDetect = false;    // all sensors must have had a positive value 

uint32_t startPulse = millis();     // set the offset time for a passing a pulse
uint32_t pulsesPerMinute = 0;       // holds the value of pulses per minute
uint32_t revolutions = 0;           // holds the value of revolutions of the first axis, calculated with ratio
uint32_t viewPulsesPerMinute = 0;   // holds the value of ends per minute calculated with ratio

String INDEPENDENT = "independent"; // means that motor is driven independent of any response

// defined in handleWebServer (JavaScript) 
//"const buttonAnchorArray = [\"pills-spin-tab\", \"pills-wifi-tab\", \"pills-info-tab\"];\r\n";
uint8_t menuButton = 0;             // holds the menuButton value, is used in handleWebServer

Settings settings = Settings();
Settings* pSettings = &settings;

//Preferences preferences1 = Preferences();

// stepsPerrevolution, maxSpeed, direction, motorInterfaceType are coming from settings/database
// stepsPerRevolution for your motor 28BYJ-48 is 2038 for FULLxWIRE
int16_t stepsPerRevolution = pSettings->getStepsPerRevolution();  // 4096 change this to fit the number of steps per revolution
int16_t maxSpeed = pSettings->getMaxSpeed();     // 1000 for type 28BYJ-48
int8_t direction = pSettings->getDirection();    // 1 or -1, some motors are wired reversed
uint8_t motorInterfaceType = pSettings->getMotorInterfaceType(); //AccelStepper::HALF4WIRE;
int16_t motorSpeedStepper = 0;
int16_t previousMotorSpeedStepper = motorSpeedStepper;

AccelStepper myStepper = AccelStepper(motorInterfaceType, motorPin1, motorPin3, motorPin2, motorPin4);

WebServer server(80);
WiFiClient wifiClient;

//////////////////////
// WiFi Definitions //
//////////////////////
WiFiSettings wifiSettings = WiFiSettings();
WiFiSettings* pWifiSettings = &wifiSettings;

//////////////////////
// AsyncHTTPrequest //
//////////////////////
asyncHTTPrequest aRequest;
long lastSendMillis;

// detectButtonFlag lets the program know that a network-toggle is going on
bool detectButtonFlag = false;

// detectUpdateFlag is True is an update from the server is requested
bool detectUpdateFlag = false;

// updateSucceeded is true if the update succeeded or if a restart is asked, so a restart can be done
bool updateSucceeded = false;

// detectInfoRequest is True if info is requested by the server
bool detectInfoRequest = false;

// Forward declaration
void setupWiFiAsAccessPoint();
void handleInfo();
void switchToAccessPoint();
void initServer();

void ICACHE_RAM_ATTR detectButton();
void buttonInterruptOn();
void buttonInterruptOff();

void toggleWiFi();
/*
2^8 = 256
2^16 = 65536
2^32 = 4294967296
2^64 = 18446744073709551616
*/

///WebServer server(80);
///WiFiClient wifiClient;

// start Settings and EEPROM stuff
void saveSettings() {
  pSettings->saveSettings();
  handleInfo();
}

void getSettings() {
  pSettings->getSettings();
  handleInfo();
}

void eraseSettings() {
  pSettings->eraseSettings();
  pSettings->getSettings();   // otherwise the previous values of Settings are used
  handleInfo();
}

void initSettings() {
  pSettings->initSettings();
  pSettings->getSettings();   // otherwise the previous values of Settings are used
  handleInfo();
}
// end Settings and EEPROM stuff

String IPAddress2String(const IPAddress &ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
         String(ipAddress[1]) + String(".") +\
         String(ipAddress[2]) + String(".") +\
         String(ipAddress[3]);
}

void show(bool clear, String message, int16_t x, int16_t y)
{
  if (hasI2C)
  {
    if (clear == true)
    { // Clear the buffer
      display.clearDisplay();
      //delay(2000);
    }
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(x, y);
    display.cp437(true);
    display.println(message);
    display.display();
  }
}

void setupWiFiAsAccessPoint(){
  digitalWrite(STATION_LED, HIGH);
  digitalWrite(ACCESSPOINT_LED, HIGH);

  WiFi.mode(WIFI_MODE_AP);
  Serial.println(WiFi.getMode());
  String myssid = pWifiSettings->readAccessPointSSID();
  String mypass = pWifiSettings->readAccessPointPassword();

  Serial.println(myssid);
  Serial.println(mypass);
  Serial.println(WiFi.softAPmacAddress());
  Serial.println(WiFi.macAddress());

  if ((myssid == "") || (myssid == "ESP-") || (WiFi.softAPSSID().startsWith("ESP_")))
  {
    myssid = String("ESP-")+WiFi.macAddress(); // WiFi.softAPmacAddress();
    pWifiSettings->setAccessPointSSID(myssid);
    pWifiSettings->setAccessPointPassword(mypass);
    pWifiSettings->saveAuthorizationAccessPoint();
  }
  IPAddress local_IP(192,168,4,1);
  IPAddress gateway(192,168,4,1);
  IPAddress subnet(255,255,255,0);

  Serial.println("Setting soft-AP ... ");

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  
  Serial.print("Connecting to AP mode");
  // do NOT add more parameters to softAP, otherwise changing password will result in an error
  // mypass needs minimum of 8 characters, otherwise it shall fail !
  Serial.println(WiFi.softAP(myssid.c_str(),mypass.c_str()) ? "Ready" : "Failed!");

  delay(500);

  Serial.println(WiFi.getMode());
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  Serial.println(WiFi.softAPmacAddress());

  digitalWrite(ACCESSPOINT_LED, HIGH);
  digitalWrite(STATION_LED, LOW);

  show(true, IPAddress2String(WiFi.softAPIP()), 0, 0);
  pSettings->beginAsAccessPoint(true);
}

void setupWiFiAsStation() {
  bool networkConnected = false;

  digitalWrite(STATION_LED, HIGH);
  digitalWrite(ACCESSPOINT_LED, HIGH);
  String mynetworkssid = pWifiSettings->readNetworkSSID();
  if (mynetworkssid != "") {
    String mynetworkpass = pWifiSettings->readNetworkPassword(); 
    WiFi.mode(WIFI_MODE_STA);

    WiFi.begin(mynetworkssid.c_str(), mynetworkpass.c_str()); 
    Serial.print("Connecting to a WiFi Network");
    int toomuch = 30;  //gives 30 seconds to connect to a Wifi network
    while ((WiFi.status() != WL_CONNECTED) && (toomuch > 0))
    {
      delay(1000);
      Serial.print(".");
      toomuch -=1;
    }
    if (toomuch > 0) {
      Serial.println();

      Serial.print("Connected, IP address: ");
      Serial.println("local ip address");
      Serial.println(WiFi.localIP());
      Serial.println(WiFi.gatewayIP());
      Serial.println(WiFi.macAddress());
    
      networkConnected = true;
      pSettings->setLastNetworkIP(WiFi.localIP().toString());

      digitalWrite(ACCESSPOINT_LED, LOW);
      digitalWrite(STATION_LED, HIGH);
      show(true, IPAddress2String(WiFi.localIP()), 0, 0);
      pSettings->beginAsAccessPoint(false);

        Serial.println(WiFi.getMode());

    }

  }
  if (networkConnected == false) {
    // no network found, fall back to access point
    Serial.println("no network SSID found/selected, fall back to AccessPoint mode");
    switchToAccessPoint();
  }
}

void resetWiFiManagerToFactoryDefaults () {

  // WiFi.disconnect(true);  // true argument should also erase ssid and password
  // https://www.pieterverhees.nl/sparklesagarbage/esp8266/130-difference-between-esp-reset-and-esp-restart
  Serial.println("try to disconnect works only when WiFi.begin() was successfully called");
  int toomuch = 2;
  while (toomuch > 0) {
    int getValue = WiFi.disconnect(true);
    if (getValue != 0) {
      toomuch = 0;
    }
    Serial.println(String(getValue));
    delay(5000);
    Serial.println("waited 5 seconds");
    toomuch -= 1;
  }
}

void switchToAccessPoint() {
  pSettings->beginAsAccessPoint(!  pSettings->beginAsAccessPoint());  // toggle
  delay(pSettings->WAIT_PERIOD);

  server.close();
  resetWiFiManagerToFactoryDefaults();
  delay(pSettings->WAIT_PERIOD);

  setupWiFiAsAccessPoint();
  delay(pSettings->WAIT_PERIOD);

  initServer();

  // switch, so domain-name has to be found in this domain-realm
  delay(pSettings->WAIT_PERIOD);
  startmDNS();
  // end domain name server check
}

void switchToNetwork() {
  delay(pSettings->WAIT_PERIOD);

  server.close();
  resetWiFiManagerToFactoryDefaults();
  delay(pSettings->WAIT_PERIOD);

  setupWiFiAsStation();
  delay(pSettings->WAIT_PERIOD);

  initServer();

  // switch, so domain-name has to be found in this domain-realm
  delay(pSettings->WAIT_PERIOD);
  startmDNS();
  // end domain name server check
}

void delayInMillis(uint8_t ms)
{
  for (uint8_t i = 0; i <= ms; i++)
  {
    delayMicroseconds(250);   // delay in the loop could cause an exception (9) when using interrupts
    delayMicroseconds(250);   // delay in the loop could cause an exception (9) when using interrupts
    delayMicroseconds(250);   // delay in the loop could cause an exception (9) when using interrupts
    delayMicroseconds(250);   // delay in the loop could cause an exception (9) when using interrupts
  }
}

void ICACHE_RAM_ATTR detectButton() {  // ICACHE_RAM_ATTR is voor interrupts
  // this function is called after a change of pressed button  
  buttonInterruptOff();  // to prevent exception

  delayInMillis(10);     // prevent bounce
  
  if (digitalRead(BUTTON) == LOW)
  {
    detectButtonFlag = true;
    // only toggle between AP and STA by using the button, not saving in EEPROM
  }
  buttonInterruptOn();   // to prevent exception
}

void buttonInterruptOn() {
  attachInterrupt(digitalPinToInterrupt(BUTTON), detectButton, FALLING);
}

void buttonInterruptOff() {
  detachInterrupt(BUTTON);
}

void handleWiFi() {
  menuButton = 1;
  menu(server, pSettings, pWifiSettings, pSettings->getLanguage(), menuButton);
}

void handleSpin() {
  menuButton = 0;
  menu(server, pSettings, pWifiSettings, pSettings->getLanguage(), menuButton);
}

void handleSse() {
  sse(server, pSettings, revolutions, viewPulsesPerMinute);
}

void handleInfo() {
  menuButton = 2;
  menu(server, pSettings, pWifiSettings, pSettings->getLanguage(), menuButton);

  Serial.println("wifi gegevens");
  Serial.print("readAccessPointSSID: ");
  Serial.println(pWifiSettings->readAccessPointSSID());
  Serial.print("readNetworkSSID: ");
  Serial.println(pWifiSettings->readNetworkSSID());

  Serial.print("Chip ID: ");
  Serial.print(ESP.getChipModel());
  Serial.print(".");
  Serial.println(ESP.getChipRevision());
  // 32c3 Serial.println(ESP.getFlashChipId());
 
  // 32c3 Serial.print("Chip Real Size: ");
  // 32c3 Serial.println(ESP.getFlashChipRealSize());
 
  Serial.print("Chip Size: ");
  Serial.println(ESP.getFlashChipSize());
 
  Serial.print("Chip Speed: ");
  Serial.println(ESP.getFlashChipSpeed());
 
  Serial.print("Chip Mode: ");
  Serial.println(ESP.getFlashChipMode());

  Serial.print("firmware version: ");
  Serial.println(pSettings->getFirmwareVersion());

  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

  Serial.print("target server: ");
  Serial.println(pSettings->getTargetServer());

  Serial.print("target port: ");
  Serial.println(pSettings->getTargetPort());

  Serial.print("target path: ");
  Serial.println(pSettings->getFactoryTargetPath());

  Serial.print("connected roleModel: ");
  Serial.println(pSettings->getRoleModel());

  Serial.print("motor steps per revolution: ");
  Serial.println(pSettings->getStepsPerRevolution());
  
  Serial.print("motor maximum speed: ");
  Serial.println(pSettings->getMaxSpeed());
  
  Serial.print("motor direction: ");
  Serial.println(pSettings->getDirection());
  
  Serial.print("motor motorInterfaceType: ");
  Serial.println(pSettings->getMotorInterfaceType());
}

String updateFirmware(String requestedVersion)
{
  buttonInterruptOff();

  digitalWrite(STATION_LED, HIGH);
  digitalWrite(ACCESSPOINT_LED, HIGH);

  String serverUrl = pSettings->getTargetServer();
  uint16_t serverPort = pSettings->getTargetPort();
  String uploadScript = "/update/updateFirmware/?device=model&version=" + requestedVersion;
  String version = pSettings->getFirmwareVersion();
  String result = updateOverHTTP(wifiClient, serverUrl, serverPort, uploadScript, version);

  if (result == UPDATEOVERHTTP_OK)
  {
    updateSucceeded = true;
  }
  else
  {
    // restore settings
    buttonInterruptOn();
    if (WiFi.getMode() == WIFI_MODE_STA)
    {
      digitalWrite(STATION_LED, HIGH);
      digitalWrite(ACCESSPOINT_LED, LOW);
    }
    else
    {
      digitalWrite(STATION_LED, LOW);
      digitalWrite(ACCESSPOINT_LED, HIGH);
    }
  return result;
  }
  
  digitalWrite(STATION_LED, LOW);
  digitalWrite(ACCESSPOINT_LED, LOW);
  return result;
}

void handleVersion() {
  uint8_t argumentCounter = 0;
  String result = "";
  String result_nl = "";

  if (server.method() == HTTP_POST)
  {
    argumentCounter = server.args();
    String name = "";
    for (uint8_t i=0; i< server.args(); i++){
      if (server.argName(i) == "name") {
        name = server.arg(i);
      }
    }
    // search name 
    if (name == "update")
    {
      if (WiFi.getMode() == WIFI_MODE_STA)
      {
        detectInfoRequest = true;
      }
      if (argumentCounter > 0)
      {
        result = updateFirmware("latest");
        if (result == UPDATEOVERHTTP_OK)
        {
          updateSucceeded = true;
        }
      }
    }
  }
  if (pSettings->getLanguage() == "NL")
  {
    if (result == UPDATEOVERHTTP_FAILED)
    {
      result_nl = "[update] Update mislukt";
    }
    if (result == UPDATEOVERHTTP_NO_UPDATE)
    {
      result_nl = "[update] Geen update aanwezig";
    }
    if (result == UPDATEOVERHTTP_NO_INTERNET)
    {
      result_nl = "[update] Geen connectie met de server aanwezig";
    }
    if (result == UPDATEOVERHTTP_OK)
    {
      result_nl = "[update] Update ok";
    }
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Connection", "keep-alive");
    server.sendHeader("Pragma", "no-cache");
    server.send(200, "text/html", result);
  }
  else
  {
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Connection", "keep-alive");
    server.sendHeader("Pragma", "no-cache");
    server.send(200, "text/html", result);
  }
}

void handleRestart() {
  uint8_t argumentCounter = 0;
  String result = "";
  String result_nl = "";

  if (server.method() == HTTP_POST)
  {
    argumentCounter = server.args();
    String name = "";
    for (uint8_t i=0; i< server.args(); i++){
      if (server.argName(i) == "name") {
        name = server.arg(i);
      }
    }
    // search name 
    if (name == "restart")
    {
      if (argumentCounter > 0)
      {
        updateSucceeded = true;
        result = "Restart completed";
        result_nl = "Restart compleet";
      }
    }
  }
  if (pSettings->getLanguage() == "NL")
  {
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Connection", "keep-alive");
    server.sendHeader("Pragma", "no-cache");
    server.send(200, "text/html", result_nl);
  }
  else
  {
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Connection", "keep-alive");
    server.sendHeader("Pragma", "no-cache");
    server.send(200, "text/html", result);
  }
}

void getMDNS() {
  //String firstFreeHostname = String(getFirstFreeHostName("model"));
  //String firstFreeHostname = findFirstFreeHostname();
  String hostname = getMDNSHostname();

  // used to answer a xhr call from the browser that is connected to the server
  String result = "";

  //result += firstFreeHostname;
  result += hostname;
  result += "<";
  result += pSettings->getRoleModel();
  result += ">";
  result += "\r\n";

  String allowServer = pSettings->getTargetServer();
  if ((pSettings->getTargetPort() != 80) && (pSettings->getTargetPort() != 443))
  {
    allowServer += ":";
    allowServer += String(pSettings->getTargetPort());
  }
      
  uint8_t argumentCounter = 0;
  argumentCounter = server.args();
  if (argumentCounter == 1) {
    for (uint8_t i=0; i< server.args(); i++){
      if (server.argName(i) == "name") {
        allowServer = server.arg(i);
        int8_t index = allowServer.lastIndexOf("/");
        if (index > -1) {
          if ((uint8_t)index == allowServer.length() -1) {
            allowServer = allowServer.substring(0, index);
          }
        }
      }
    }
  }

  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Connection", "keep-alive");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Access-Control-Allow-Origin", allowServer);
  server.send(200, "text/html", result);
}


void handleRoleModel() {
  //uint8_t argumentCounter = 0;
  String result = "";
  String result_nl = "";

  if (server.method() == HTTP_GET)
  {
    //argumentCounter = server.args();  // if argumentCounter > 0 then save
    String roleModel = "";
    String _speed = "";
    for (uint8_t i=0; i< server.args(); i++){
      if (server.argName(i) == "id") {
        if (server.arg(i).length() > 32) {
          roleModel = server.arg(i).substring(0, 32);
        }
        else{
          roleModel = server.arg(i);
        }
        show(false, pSettings->getRoleModel(), 0, 10);   
      }
      if (roleModel == INDEPENDENT)
      {
        if (server.argName(i) == "speed") {
          if (server.arg(i).length() < 6) { 
            _speed = server.arg(i);     // speed in revolutions per hour
            motorSpeedStepper = round(_speed.toInt() * stepsPerRevolution / 3600);
          }
        }
      }
    }
    if (pSettings->getRoleModel() != roleModel)
    {
      pSettings->setRoleModel(roleModel);
      result += "rolemodel is saved";
      result_nl += "rolmodel is opgeslagen";
    }
  }
  if (pSettings->getLanguage() == "NL")
  {
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Connection", "keep-alive");
    server.sendHeader("Pragma", "no-cache");
    server.send(200, "text/plain", result_nl);
  }
  else
  {
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Connection", "keep-alive");
    server.sendHeader("Pragma", "no-cache");
    server.send(200, "text/plain", result);
  }
}


void handleConnect() {
  uint8_t argumentCounter = 0;

  argumentCounter = server.args();
  if (argumentCounter < 4) {  // prevent to many arguments
    String targetServer = pSettings->getFactoryTargetServer();
    String targetPort = String(pSettings->getFactoryTargetPort());
    String targetPath = pSettings->getFactoryTargetPath();
    for (uint8_t i=0; i< server.args(); i++){
      if (server.argName(i) == "server") {
        targetServer = server.arg(i);
      }
      if (server.argName(i) == "port") {
        targetPort = server.arg(i);
      }
      if (server.argName(i) == "path") {
        targetPath = server.arg(i);
      }
    }

    pSettings->setTargetServer(targetServer);
    pSettings->setTargetPort(targetPort);
    pSettings->setTargetPath(targetPath);
    pSettings->saveTargetServerStuff();
  }
  handleInfo();
}

void handleMenu() {
  menu(server, pSettings, pWifiSettings, pSettings->getLanguage(), menuButton);
}

void handleLanguage() {
  uint8_t argumentCounter = 0;
  //String result = "";
  //String result_nl = "";

  if (server.method() == HTTP_POST)
  {
    argumentCounter = server.args();  // if argumentCounter > 0 then save
    String name = "";
    String language = "";
    for (uint8_t i=0; i< server.args(); i++){
      if (server.argName(i) == "name") {
        name = server.arg(i);
      }
      if (server.argName(i) == "language") {
        language = server.arg(i);
      }
      if (server.argName(i) == "menuButton") {
        String s = server.arg(i);
        uint8_t i, n;
        n = 0;
        for (i = 0; s[i] >= '0' && s[i] <= '9'; i++)
          n = 10 * n +(s[i] - '0');
        menuButton = n;
      }
    }
    // search name 
    if (name == "help")
    {
      if (argumentCounter > 0)
      {
        pSettings->setLanguage(language);
        menu(server, pSettings, pWifiSettings, pSettings->getLanguage(), menuButton);
      }
    }
  }
}

void handleNetworkSSID() {
  // creates a list of {ssid, including input field , dBm}
  // to prevent an error, the single quote will be replace here with a question mark
  //                      and restored within the handleWebServer  
  String result = "";
  int numberOfNetworks = WiFi.scanNetworks();
  for(int i =0; i<numberOfNetworks; i++){ 
    String foundSSID = WiFi.SSID(i);
    foundSSID.replace("'", "?");
    if (i > 0) {
      result += ",";
    }
    result += "{ssid:";
    result += "'<span><input type=\"radio\" name=\"networkSSID\" onclick=\"selectNetworkSSID(this)\" value=\"";
    result += foundSSID;
    result += "\">";
    result += foundSSID;
    result += "</span>";
    result += "',";
    result += "dBm:'";
    result += WiFi.RSSI(i);
    result += "'}";
  }
  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Connection", "keep-alive");
  server.sendHeader("Pragma", "no-cache");
  server.send(200, "text/html", result);
}

void handleWifiConnect() {
  uint8_t argumentCounter = 0;
  String result = "";
  String result_nl = "";

  if (server.method() == HTTP_POST)
  {
    argumentCounter = server.args();  // if argumentCounter > 0 then save
    String name = "";
    String ssid = "";
    String password = "";
    String target = "";               // for action Erase
    for (uint8_t i=0; i< server.args(); i++){
      if (server.argName(i) == "name") {
        name = server.arg(i);
      }
      if (server.argName(i) == "ssid") {
        ssid = server.arg(i);
      }
      if (server.argName(i) == "password") {
        password = server.arg(i);
      }
      if (server.argName(i) == "target") {
        target = server.arg(i);
      }
    }
    // zoek name (is ap of network en dan de ssid en password)
    if (name == "ap")
    {
      pWifiSettings->setAccessPointSSID(ssid);
      pWifiSettings->setAccessPointPassword(password);
      if (argumentCounter > 0) {
        pWifiSettings->saveAuthorizationAccessPoint();
        result += "Access Point data has been saved\n";
        result_nl += "Access Point gegevens zijn opgeslagen\n";
      }
    }
    if (name == "network")
    {
      pWifiSettings->setNetworkSSID(ssid);
      pWifiSettings->setNetworkPassword(password);
      if (argumentCounter > 0) {
        pWifiSettings->saveAuthorizationNetwork();
        result += "Network connection data has been saved\n";
        result_nl += "Netwerk verbindingsgegevens zijn opgeslagen\n";
      }
    }
    if (name == "erase")
    {
      if (argumentCounter > 0) {
        if (target == "eraseAPData")
        {
          pWifiSettings->eraseAccessPointSettings();
          result += "Access Point data has been erased\n";
          result_nl += "Access Point gegevens zijn gewist\n";
        }
        if (target == "eraseNetworkData")
        {
          pWifiSettings->eraseNetworkSettings();
          result += "Network connection data has been erased\n";
          result_nl += "Netwerk verbindingsgegevens zijn gewist\n";
        }
        if (target == "eraseWiFiData")
        {
          pWifiSettings->eraseWiFiSettings();
          result += "Access Point data and Network connection data has been erased\n";
          result_nl += "Access Point gegevens en Netwerk verbindingsgegevens zijn gewist\n";
        }
      }
    }
  }
  if (pSettings->getLanguage() == "NL")
  {
    server.send(200, "text/plain", result_nl);
  }
  else
  {
    server.send(200, "text/plain", result);
  }
  Serial.println(result);
}

void handleSpinSettings()
{
  uint8_t argumentCounter = 0;
  String result = "";
  String result_nl = "";

  if (server.method() == HTTP_POST)
  {
    // extract the settings-data and take action
    argumentCounter = server.args();  // if argumentCounter > 0 then save Settings
    String _name = "";
    String _spinMode = "";
    String _roleModelSpeed = "";
    String _roleModelCode = "";
    for (uint8_t i=0; i< server.args(); i++){
      if (server.argName(i) == "name") {
        _name = server.arg(i);
      }
      if (server.argName(i) == "spinMode") {
        _spinMode = server.arg(i);
      }
      if (server.argName(i) == "roleModelSpeed") {
        _roleModelSpeed = server.arg(i);
      }
      if (server.argName(i) == "roleModelCode") {
        _roleModelCode = server.arg(i);
      }
    }
    if (_name == "spin")
    {
      if (_spinMode == INDEPENDENT) {
        pSettings->setRoleModel(INDEPENDENT);
        if (_roleModelSpeed.toInt() != pSettings->getRoleModelRPM())
        {
          pSettings->setRoleModelRPM((uint8_t)_roleModelSpeed.toInt());
          pSettings->saveRoleModelRPM();
        }
        if (_roleModelSpeed.toInt() * stepsPerRevolution / 60 < maxSpeed) {
          motorSpeedStepper = round(_roleModelSpeed.toInt() * stepsPerRevolution / 60);
        }
        else {
          motorSpeedStepper = maxSpeed;
        }
      }
      if (_spinMode == "connected")
      {
        pSettings->setRoleModel(_roleModelCode); 
        if (WiFi.getMode() == WIFI_MODE_AP)
        {
          motorSpeedStepper = 0;
        }
      }
      if (_spinMode == "stop")
      {
        pSettings->setRoleModel("None");
        motorSpeedStepper = 0;
      }
    }
    if (argumentCounter > 0) {
      pSettings->saveRoleModelSetting();
      result += "Data has been saved\n";
      result_nl += "Gegevens zijn opgeslagen\n";
    }
  }
  if (pSettings->getLanguage() == "NL")
  {
    server.send(200, "text/plain", result_nl);
  }
  else
  {
    server.send(200, "text/plain", result);
  }
  Serial.println(result);
}

void smoothAcceleration() {
    if (motorSpeedStepper - previousMotorSpeedStepper > 20)
    {
      motorSpeedStepper = previousMotorSpeedStepper + 20;
      previousMotorSpeedStepper = motorSpeedStepper;
    }
    if (previousMotorSpeedStepper - motorSpeedStepper > 20)
      {
      motorSpeedStepper = previousMotorSpeedStepper - 20;
      previousMotorSpeedStepper = motorSpeedStepper;
    }
}

String getValueFromJSON(String key, String responseData)
{
  int16_t keyIndex = responseData.indexOf(key);
  if (keyIndex > -1)
  {
    int16_t start = responseData.indexOf(":", keyIndex);
    int16_t end = responseData.indexOf(",", start);
    if (end == -1) {
      end = responseData.indexOf("}", start);
    }
    String value = responseData.substring(start + 1, end);
    value.trim();
    value.replace("\"", "");
    return value;
  }
  return "";
}

void processServerData(String responseData) {
  /* data should come in JSON format */
  //Serial.println(responseData);
  String proposedUUID = getValueFromJSON("pKey", responseData);
  if ((proposedUUID != "") && (pSettings->getDeviceKey() != proposedUUID))
  {
    pSettings->setDeviceKey(proposedUUID);
    pSettings->saveDeviceKey(); // save to EEPROM
  }

String requestInterval = getValueFromJSON("t", responseData);
  if ((requestInterval != ""))
  {
    pSettings->setRequestInterval(requestInterval);
  }

  String pushFirmwareVersion = getValueFromJSON("pFv", responseData);
  if (pushFirmwareVersion != "")
  {  
    detectUpdateFlag = true;
  }

  String requestForInfo = getValueFromJSON("i", responseData);
  if (requestForInfo != "")
  {
    detectInfoRequest = true;
  }
  String rph = getValueFromJSON("rph", responseData);
  
  String myStepsPerRevolution = getValueFromJSON("spr", responseData);
  String myMaxSpeed = getValueFromJSON("ms", responseData);
  String myDirection = getValueFromJSON("d", responseData);
  String myMotorInterfaceType = getValueFromJSON("mit", responseData);

  if ((myStepsPerRevolution != "") && 
      (myMaxSpeed != "") && 
      ((myDirection == "-1") || (myDirection == "1")) && 
      (myMotorInterfaceType != ""))
  {
    if ((myStepsPerRevolution != String(pSettings->getStepsPerRevolution())) ||
        (myMaxSpeed != String(pSettings->getMaxSpeed())) ||
        (myDirection != String(pSettings->getDirection())) ||
        (myMotorInterfaceType != String(pSettings->getMotorInterfaceType())) )
        {
          // something has changed, so save and restart
          pSettings->setStepsPerRevolution((uint16_t)myStepsPerRevolution.toInt());
          pSettings->setMaxSpeed((uint16_t)myMaxSpeed.toInt());
          pSettings->setDirection((int8_t)myDirection.toInt());
          pSettings->setMotorInterfaceType((uint8_t)myMotorInterfaceType.toInt());

          pSettings->saveMotorSettings();
          ESP.restart();
        }
  }

  if (rph != "")
  {
    uint16_t speedValue = (uint16_t)rph.toInt();
    // setSpeed is per second, rph is per hour, so divide by 3600
    if (speedValue * stepsPerRevolution / 3600 < maxSpeed) {
       motorSpeedStepper = round(speedValue * stepsPerRevolution / 3600);
    }
    else {
      motorSpeedStepper = maxSpeed;
    }
  }
}

void toggleWiFi()
{
  pSettings->beginAsAccessPoint(!  pSettings->beginAsAccessPoint());  // toggle
  pSettings->saveStartAsAccessPoint();                                // save boolean startAsAccessPoint
  if (pSettings->beginAsAccessPoint() == true)
  {
    setupWiFiAsAccessPoint();           // local network as access point
  }
  else
  {
    setupWiFiAsStation();    // part of local network as station
  }
}

void initWire()
{
  //pinMode(I2C_SCL, PULLUP);
  //pinMode(I2C_SDA, PULLUP);
  Wire.begin(I2C_SDA, I2C_SCL);
  while (!Serial);   // waiting on serial monitor
  Serial.println("\nI2C scanner");

  byte error, address;
  int nDevices = 0;

  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found on 0x");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.println("unknown error");
    }
  }
  if (nDevices == 0)
  {
    Serial.println("No devices found");
    hasI2C = false;
  }
  else{
    hasI2C = true;
  }


}

void initOled()
{
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000); // Pause for 1 second

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(0, 0, SSD1306_WHITE);
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  //delay(1000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

}

void initHardware()
{
  Serial.begin(115200);

  pinMode(STATION_LED, OUTPUT);
  pinMode(ACCESSPOINT_LED, OUTPUT);

  pinMode(BUTTON, INPUT_PULLUP);

  myStepper.setMaxSpeed(maxSpeed);
  myStepper.setSpeed(0);

}

void initServer()
{
  server.close();
  // start webserver

  // bootstrap-like menu
  server.on("/", handleMenu);

  // handles notFound
  server.onNotFound(handleSpin);

  // interactive pages
  server.on("/spin/", handleSpin);
  server.on("/wifi/", handleWiFi);
  server.on("/info/", handleInfo);

  // handles input from interactive pages
  server.on("/networkssid/", handleNetworkSSID);
  server.on("/wifiConnect/", handleWifiConnect);
  server.on("/spinSettings/", handleSpinSettings);
  server.on("/language/", handleLanguage);
  server.on("/update/", handleVersion);
  server.on("/restart/", handleRestart);

  // data handler
  server.on("/data.sse/", handleSse);

  server.on("/setRoleModel/", handleRoleModel);
  // url-commands, not used in normal circumstances
  server.on("/ap/", switchToAccessPoint);
  server.on("/network/", switchToNetwork);
  server.on("/eraseSettings/", eraseSettings);
  server.on("/initSettings/", initSettings);
  server.on("/getSettings/", getSettings);
  server.on("/saveSettings/", saveSettings);
  server.on("/reset/", resetWiFiManagerToFactoryDefaults);

  // for selecting a target server when developing
  // arguments: server, port, path, default are the factorySettings
  // /connect/ without arguments connects to the production server, 
  //           port and path
  server.on("/connect/", handleConnect);

  // handles a check if this url is available
  // remove this when clients are availabe
  server.on("/_mdns/", getMDNS);

  server.begin();
  Serial.println("HTTP server started");
}

void checkSpinValue()
/* only at startup */
{
  if (pSettings->getRoleModel() == INDEPENDENT)
  {
    motorSpeedStepper = round(pSettings->getRoleModelRPM() * stepsPerRevolution / 60);
    if (motorSpeedStepper > maxSpeed)
    {
      motorSpeedStepper = maxSpeed;
    }
  }
  show(false, pSettings->getRoleModel(), 0, 10);

}

void setup()
{
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  /* It seems to help preventing ESPerror messages with mode(3,6) when
  using a delay */
  initHardware();
  //init_nvs_security();
  delay(pSettings->WAIT_PERIOD);
  printf("initHardware via printf ready");
  Serial.println("initHardware ready");
  initWire();
  if (hasI2C)
  {
    initOled();
  }

  printf("boot Settings\n");
  delay(pSettings->WAIT_PERIOD);
  pSettings->bootSettings();
  printf("booted Settings\n");
  delay(pSettings->WAIT_PERIOD);
  printf("boot WifiSettings\n");
  pWifiSettings->bootWiFi();
  printf("booted WiFiSettings\n");
  delay(pSettings->WAIT_PERIOD);

   // see https://forum.arduino.cc/index.php?topic=121654.0 voor circuit brownout
  //delay(pSettings->WAIT_PERIOD);
  // use EITHER setupWiFi OR setupWiFiManager

  // get saved setting from Memory
  eepromStartModeAP = pSettings->beginAsAccessPoint();

  delay(pSettings->WAIT_PERIOD);
  
  if ((pSettings->beginAsAccessPoint() == true) || (pWifiSettings->getNetworkSSID() == ""))
  {
    setupWiFiAsAccessPoint();        // local network as access point
  }
  else
  {
    setupWiFiAsStation();            // part of local network as station
  }

  delay(pSettings->WAIT_PERIOD);
  // first search for domain-name

  startmDNS();
  // end domain name server check

  delay(pSettings->WAIT_PERIOD);

  initServer();

  checkSpinValue();

  // for asyncrequest
  lastSendMillis = millis();    

  delay(pSettings->WAIT_PERIOD);

  buttonInterruptOn();
}

void loop()
{
  // update should be run on every loop

  if (detectUpdateFlag == true)
  {
    String result = updateFirmware("latest");
    detectUpdateFlag = false;
  }

  if (updateSucceeded == true)
  {
    ESP.restart();
  }

  if (detectButtonFlag == true)
  {
    toggleWiFi();   // only toggle between AP and STA by using the button and set corresponding Setting
    detectButtonFlag = false;
    // set eepromStartMode to the correct value, needed for independent spinning in ap-mode
    // saving in EEPROM
    if (pSettings->beginAsAccessPoint() == true)
    {
      eepromStartModeAP = true; 
    }
    else
    {
      eepromStartModeAP = false;
    }
    pSettings->saveStartAsAccessPoint();
  }

  // For ESP WebServer
  server.handleClient();
 
  // For handleHTTPClient
  if (WiFi.getMode() == WIFI_MODE_STA)
  {
    // send data to target server using ESP HTTPClient 
    if (millis() - lastSendMillis > pSettings->getSendPeriod())
    {

      if ((aRequest.readyState() == 0) || (aRequest.readyState() == 4)) {
          sendDataToTarget(&aRequest, wifiClient, pSettings, pWifiSettings, String(WiFi.macAddress()), detectInfoRequest);
          detectInfoRequest = false;    // reset value so no info will be sent again
      }


      lastSendMillis = millis();
    }

    String response = getAsyncResponse(&aRequest);
    if (response != "") 
    {
      processServerData(response);
    }
  }

  // For automatic Reset after loosing WiFi connection in STA mode
  if ((WiFi.getMode() == WIFI_MODE_AP) && (eepromStartModeAP == false))
  {
    if (millis() > lastStartMillis + NO_STA_MAX_MILLIS)
    {
      lastStartMillis = millis();
      setupWiFiAsStation();  // try to start WiFi again
    }
  }

  // Stepper motor
  if (motorSpeedStepper > 0) {

    myStepper.setSpeed(motorSpeedStepper * direction);
  }
  else
  {
    myStepper.setSpeed(0);
  }
  myStepper.runSpeed();

  // delay(1000);
  // digitalWrite(ACCESSPOINT_LED,HIGH);
  // delay(1000);
  // digitalWrite(ACCESSPOINT_LED,LOW);
   

}
