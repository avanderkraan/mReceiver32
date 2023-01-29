#include "handleWebServer.h"

// for converting String to char define a char buffer
char textBuffer[64];
// define text
const char *k_title[] = {"Mill Model", "Molen Model"};
const char *k_wiFiMode[] = {"WiFi mode:", "WiFi modus:"};
const char *k_language[] = {"Language", "Taal"};
const char *k_mill[] = {"Mill", "Molen"};

const char *k_accessPoint[] = {"Access Point", "Access Point"};
const char *k_or[] = {"or", "of"};
const char *k_for[] = {"(url for ", "(url voor "};
const char *k_networkStation[] = {"Network Station", "Network Station"};
const char *k_orLocalIP[] = {"or via a local IP address, last known is:", "of via een lokaal IP adres, laatst bekende adres is:"};
const char *k_hrefNetworkAddress[] = {"(<a href='http://${text1}/' target='_blank'>${text2}</a>)", \
                                      "(<a href='http://${text1}/' target='_blank'>${text2}</a>)"};
const char *k_version[] = {"Version:", "Versie:"};
const char *k_spinHeader[] = {"Settings for the mill model:", "Instellingen voor het molen model"};
const char *k_spinDescription[] = {"This model can spin by itself and also be connected to a real mill", "Dit model kan zelf draaien maar kan ook gekoppeld zijn aan een echte molen"};
const char *k_independent[] = {"Independent", "Onafhankelijk"};
const char *k_connectToMill[] = {"Connect to a mill", "Gekoppeld aan een molen"};
const char *k_stop[] = {"Stop", "Stop"};
const char *k_enterSpeed[] = {" Enter a speed (0-${text1} revolutions per minute)", \
                              " Geeft een snelheid op (0-${text1} toeren per minuut)"};
const char *k_giveSpeedMessage[] = {"Invalid number (0 - ${text2}), changed to ${text3}", \
                                   "Ongeldig nummer (0 - ${text2}), gewijzigd in ${text3}"};
const char *k_millCode[] = {"Enter a millcode", "Geef een molencode"};
const char *k_findMillList[] = {"Search in a list on ", "Zoek in een lijst op "};
const char *k_lang[] = {"en", "nl"};
const char *k_code[] = {" Code", " Code"};
const char *k_maxCodeChars[] = {"Max 32 characters", "Max 32 karakters"};
const char *k_confirmationWait[] = {"After 'Save' wait for confirmation", "Na 'Save' even wachten op bevestiging"};

const char *k_connectHeader[] = {"WiFi settings for the mill model", "WiFi instellingen voor het molen model"};
const char *k_chooseAccessPoint[] = {"Choose AccessPoint to change the default password", "Kies AccessPoint om het standaard wachtwoord te wijzigen"};
const char *k_chooseNetwork[] = {"Choose Network to connect the mill model to your own WiFi", "Kies Netwerk om het molen model te koppelen aan je WiFi"};
const char *k_choiceAccessPoint[] = {"Access Point", "Access Point"};
const char *k_choiceNetwork[] = {"Network Station", "Netwerk Station"};
const char *k_choiceEraseWiFi[] = {"Go to Erase WiFi-data menu", "Ga naar Verwijder WiFi-gegevens menu"};
const char *k_accessToAccessPoint[] = {" Clients can get access to this Access Point using the SSID and password entered below", \
                                        " Apparaten kunnen toegang krijgen tot dit Access Point met het hieronder ingevulde SSID en wachtwoord"};
const char *k_emptyPassword[] = {" An empty password will result in an unencrypted, open Access Point", \
                                  " Een niet-ingevuld wachtwoord geeft een onveilige, open Access Point"};
const char *k_ssid[] = {" SSID: ", " SSID "};
const char *k_password[] = {" Password: ", " Wachtwoord: "};
const char *k_checkURIComponent[] = {"onkeyup=\"checkURIComponent(this, 'apPasswordMessage', 'Invalid password character');\">", \
                                      "onkeyup=\"checkURIComponent(this, 'apPasswordMessage', 'Ongeldig wachtwoord karakter');\">"};
const char *k_connectWiFiList[] = {"Connect to one of the WiFi networks below", "Maak verbinding met een van de WiFi netwerken hieronder"};
const char *k_connectWiFiListWait[] = {"Please wait for a list or a message to show here", \
                                        "Wacht even tot er een lijst of een melding verschijnt"};
const char *k_giveNetWorkPassword[] = {"Give the password for the selected WiFi network: ", \
                                        "Geef het wachtwoord voor het geselecteerde WiFi netwerk: "};
const char *k_passwordPlaceholder[] = {"password", "wachtwoord"};
const char *k_messageInvalidPassword[] = {"Invalid password character", "Ongeldig wachtwoord karakter"};
const char *k_warning[] = {"!!!Warning!!!", "!!!Opgelet!!!"};
const char *k_clickErase[] = {"A click on an Erase button will immediately erase saved data! Your current connection will stay alive", \
                                "Een klik op de 'Erase' knop zal onmiddellijk de opgeslagen gegevens verwijderen! De huidige verbinding blijft bestaan"};
const char *k_cancelErase[] = {"Do NOT Erase and ", "Verwijder NIETS en "};
const char *k_eraseAccessPoint[] = {" Erase Access Point data, will result in an unencrypted, open Access Point", \
                                    " Verwijderen van Access Point resulteert in een onveilig, open Access Point"};
const char *k_eraseNetwork[] = {" Erase Network data", " Verwijder Netwerk gegevens"};
const char *k_eraseWiFiData[] = {" Erase Access Point AND Network data", " Verwijder Access Point EN Netwerk gegevens"};


const char *k_infoHeader[] = {"Info about the mill model:", "Informatie over het molen model"};
const char *k_infoMessageNotSent[] = {"Passwords and IP addresses are not sent to the server", \
                                      "Wachtwoorden en IP adressen worden niet naar de server gestuurd"};
const char *k_infoMessageSent[] = {"With an update request the information below in bold will be sent to ${text1}", \
                                   "Bij een update aanvraag wordt de dikgedrukte informatie die je hieronder ziet opgestuurd naar ${text1}"};
const char *k_ipAddress[] = {"IP address: ${text1}", "IP adres: ${text1}"};
const char *k_macAddress[] = {"MAC address: ${text1}", "MAC adres: ${text1}"};
const char *k_firmwareVersion[] = {"Firmware version: ${text1}", "Firmware versie: ${text1}"};
const char *k_motorSettings[] = {"Motor settings", "Motor instellingen"};
const char *k_stepsPerRevolution[] = {"- Steps per revolution: ${text1}", "- Stappen per omwenteling: ${text1}"};
const char *k_maxSpeed[] = {"- Maximum speed: ${text1}", "- Maximum snelheid: ${text1}"};
const char *k_direction[] = {"- Direction (1 or -1): ${text1}", "- Richting (1 of -1): ${text1}"};
const char *k_interfaceType[] = {"- Interface type: ${text1}", "- Interface type: ${text1}"};
const char *k_accessPointSSID[] = {"Access Point SSID: ${text1}", "Access Point SSID: ${text1}"};
const char *k_networkSSID[] = {"Network SSID: ${text1}", "Netwerk SSID: ${text1}"};
const char *k_serverName[] = {"Server name: ${text1}", "Server name: ${text1}"};
const char *k_serverPort[] = {"Server port: ${text1}", "Server poort: ${text1}"};
const char *k_serverPath[] = {"Server path: ${text1}", "Server pad: ${text1}"};

void menu(WebServer &server, Settings * pSettings, WiFiSettings * pWifiSettings, String language, uint8_t menuButton)
{
  String logo = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAABhGlDQ1BJQ0MgcHJvZmlsZQAAKJF9kT1Iw0AcxV9TpVUqClYQcchQnSyIijhqFYpQodQKrTqYXPoFTRqSFBdHwbXg4Mdi1cHFWVcHV0EQ/ABxdHJSdJES/5cUWsR4cNyPd/ced+8AoV5mqtkxDqiaZaTiMTGTXRUDrwhiAF3oQ0Bipj6XTCbgOb7u4ePrXZRneZ/7c/QoOZMBPpF4lumGRbxBPL1p6Zz3icOsKCnE58RjBl2Q+JHrsstvnAsOCzwzbKRT88RhYrHQxnIbs6KhEk8RRxRVo3wh47LCeYuzWq6y5j35C0M5bWWZ6zSHEccilpCECBlVlFCGhSitGikmUrQf8/APOf4kuWRylcDIsYAKVEiOH/wPfndr5icn3KRQDOh8se2PESCwCzRqtv19bNuNE8D/DFxpLX+lDsx8kl5raZEjoHcbuLhuafIecLkDDD7pkiE5kp+mkM8D72f0TVmg/xboXnN7a+7j9AFIU1eJG+DgEBgtUPa6x7uD7b39e6bZ3w8H+HJ89etzkgAAAAZiS0dEAP8A/wD/oL2nkwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAd0SU1FB+YIFQkQMU+zl7cAAAGwSURBVFjD7ZU9SwNBEIafRFS00vyBEFQEW0FEEARrOwUtk17RUrEwaewtbRQEf4GVjWhhaQI2ooEIdpr4UaSTnM0sLMveJZvsBosMbHH79T4zOzMHAxvYP7Ux4BB4BHIe7isAWy4HzoEq0AJee4QoAA3gGZjo9FAGqHiAyIt4HYiAdZfDGaAsEFEXEEq8IedL3YRvUiLx4ghhel7qJYFcIbyKmxDtcqLgI+xJEGUDImsRr4cQN6ujBuwAaU38O7S4WR1n2pt/AkvAG1DsR6dUOXErHu/JfI4+2r6Iq5Htl/ARMAvcGAAViUxQK4nYNfBlAPwIRCa0eF2SLjJGS0o0SCSUuGoyJxaAYBC65w2p+TWL+K80p0jathcIUzwv8wcxEZjzCWGGPa+tncraA3AsoY+ARekFPUMUYzxXpkpwG0gBH/K9qTUkBVGVzulUHXfGm5umSnAXGAHeLf8ABaEnZscQCwJgEx/V3rwJPGnfl8ZeHeLC9SmmYubnYxIwAu4t+3PAqs+esJEA0HS9LN0FwEzC2rjkRBCAIWAYmG6zb1n2pXyEW7/kKiH0trHSicAffrbMN6nraY8AAAAASUVORK5CYII=";
  uint8_t locale = EN;
  if (language == "NL")
  {
    locale = NL;
  }
  String result = "<!DOCTYPE HTML>\r\n<html>\r\n";
  result += "<head>\r\n";
  result += "<meta charset=\"utf-8\">\r\n";
  result += "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\r\n";
  result += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\r\n";
  result += "<!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->\r\n";
  result += "<link rel='icon' type='image/png' href='";
  result += logo;
  result += "'>\r\n";
  result += "<title>" + getText(locale, k_title) + "</title>\r\n";
  result += "<style>\r\n";
  result += ".title {\r\n";
  result += "  font-size:1.5em;\r\n";
  result += "}\r\n";
  result += ".small {\r\n";
  result += "  font-size:0.5em;\r\n";
  result += "}\r\n";
  result += ".menu-header {\r\n";
  result += "  font-size:1.2em;\r\n";
  result += "  font-weight:700;\r\n";
  result += "}\r\n";
  result += ".nav {\r\n";
  result += "  list-style:none;\r\n";
  result += "}\r\n";
  result += ".nav-pills>li+li {\r\n";
  result += "  margin-left:2px;\r\n";
  result += "}\r\n";
  result += ".nav-pills>li {\r\n";
  result += "  float:left;\r\n";
  result += "}\r\n";
  result += ".nav-pills>li.active>a, .nav-pills>li.active>a:focus, .nav-pills>li.active>a:hover {\r\n";
  result += "  color: #fff;\r\n";
  result += "  background-color: #337ab7;\r\n";
  result += "}\r\n";
  result += ".nav-pills>li>a {\r\n";
  result += "  border-top-left-radius:4px;\r\n";
  result += "  border-top-right-radius:4px;\r\n";
  result += "  border-bottom-left-radius:4px;\r\n";
  result += "  border-bottom-right-radius:4px;\r\n";
  result += "}\r\n";
  result += ".nav>li {\r\n";
  result += "  position:relative;\r\n";
  result += "  display:block;\r\n";
  result += "}\r\n";
  result += ".nav>li>a {\r\n";
  result += "  position:relative;\r\n";
  result += "  display:block;\r\n";
  result += "  padding-top:10px;\r\n";
  result += "  padding-right:15px;\r\n";
  result += "  padding-bottom:10px;\r\n";
  result += "  padding-left:15px;\r\n";
  result += "}\r\n";
  result += "a {\r\n";
  result += "  text-decoration:none;\r\n";
  result += "  cursor:pointer\r\n";
  result += "}\r\n";
  result += "/* Smartphones ----------- */\r\n";
  result += "@media only screen and (max-width: 760px) {\r\n";
  result += ".nav>li>a {\r\n";
  result += "  position:relative;\r\n";
  result += "  display:block;\r\n";
  result += "  padding-top:8px;\r\n";
  result += "  padding-right:10px;\r\n";
  result += "  padding-bottom:8px;\r\n";
  result += "  padding-left:10px;\r\n";
  result += "}\r\n";
  result += "</style>\r\n";
  result += "</head>\r\n";
  result += "<body>\r\n";
  result += "<div class='header'>\r\n";
  result += "<span>\r\n";
  result += "<a href='/'><img src='";
  result += logo;
  result += "' width='30'></a>\r\n";
  result += "<span class=\"small\">© 2022 MAMI</span>\r\n";
  result += "</span>\r\n";
  result += "<span class='title'>" + getText(locale, k_title) + "</span>\r\n";
  result += "</div>\r\n";
  result += "<div class='menu-button'>\r\n";

  result += "  <ul class='nav nav-pills' id='pills-tab'>\r\n";
  result += "    <li class='nav-item'>\r\n";
  result += "      <a class='nav-link' id='pills-spin-tab' href='#pills-spin'>" +  getText(locale, k_mill) + "</a>\r\n";
  result += "    </li>\r\n";
  result += "    <li class='nav-item'>\r\n";
  result += "      <a class='nav-link' id='pills-wifi-tab' href='#pills-wifi'>Connect</a>\r\n";
  result += "    </li>\r\n";
  result += "    <li class='nav-item'>\r\n";
  result += "      <a class='nav-link' id='pills-info-tab' href='#pills-info'>Info</a>\r\n";
  result += "    </li>\r\n";
  result += "  </ul>\r\n";
  result += "</div>\r\n";
  result += "<br><br><hr>\r\n";
  // start tab-content
  result += "<div class='tab-content' id='pills-tabContent'>\r\n";

  // start spin-content
  result += "  <div class='tab-pane' id='pills-spin'>\r\n";
  result += "<span class=\"menu-header\">\r\n";
  result += getText(locale, k_spinHeader);
  result += "</span><br><br><br>\r\n";
  result += "<div id=\"spinSetting\" onclick=\"clearMessage();\">\r\n";
  result += getText(locale, k_spinDescription) + "\r\n";
  result += "    <br>\r\n";
  result += "<input type=\"radio\" name=\"spin\" onclick=\"displaySpinSetting()\" value=\"independent\" ";
  result += (pSettings->getRoleModel() == "independent")?"checked":"";
  result += "> \r\n";
  result += getText(locale, k_independent);
  result += "<br>\r\n";
  result += "<input type=\"radio\" name=\"spin\" onclick=\"displaySpinSetting()\" value=\"connected\" ";
  result += ((pSettings->getRoleModel() != "independent") && (pSettings->getRoleModel() != "None"))?"checked":"";
  result += "> \r\n";
  result += getText(locale, k_connectToMill);
  result += "<br>\r\n";
  result += "<input type=\"radio\" name=\"spin\" onclick=\"displaySpinSetting()\" value=\"stop\" ";
  result += "> \r\n";
  result += getText(locale, k_stop);
  result += "<br>\r\n";
  result += "<br>\r\n";

  result += "<div id=\"spinSettingSpeed\">\r\n";
  String(pSettings->getMaxRoleModelRPM()).toCharArray(textBuffer, 64);
  const char *r_enterSpeed[] = {textBuffer};
  result += getText(locale, k_enterSpeed, r_enterSpeed, 1);
  result += "<br>\r\n";
  String(pSettings->getMaxRoleModelRPM()).toCharArray(textBuffer, 64);
  const char *r_giveSpeedMessage[] = {textBuffer, textBuffer};
  result += " Speed <input type=\"text\" name=\"roleModelSpeed\" min=\"0\" max=\"";
  result += pSettings->getMaxRoleModelRPM();
  result += "\" maxlength=\"2\" size=\"3\" onkeyup=\"checkNumber(this, 'speedMessage','";
  result += getText(locale, k_giveSpeedMessage, r_giveSpeedMessage, 2);
  result += "');\" placeholder=\"0\" value=\"";
  result += pSettings->getRoleModelRPM();
  result += "\">";
  result += "<div id=\"speedMessage\"></div>\r\n";
  result += "</div>\r\n";
  result += "<div id=\"spinSettingCode\">\r\n";
  result += getText(locale, k_millCode);
  result += "<br>\r\n";
  result += getText(locale, k_findMillList);
  if (WiFi.getMode() == WIFI_MODE_STA)
  {
    result += "<a href=\"";
    result += pSettings->getTargetServer();
    if ((pSettings->getTargetPort() != 80) && (pSettings->getTargetPort() != 443))
    {
      result += ":";
      result += String(pSettings->getTargetPort());
    }
    result += "/codes/?lang=en";
    result += "\" target=\"_blank\">\r\n";
  }
  result += pSettings->getTargetServer();
  if ((pSettings->getTargetPort() != 80) && (pSettings->getTargetPort() != 443))
  {
    result += ":";
    result += String(pSettings->getTargetPort());
  }
  result += "/codes/?lang=";
  result += getText(locale, k_lang);
  if (WiFi.getMode() == WIFI_MODE_STA)
  {
    result += "</a>\r\n";
  } 
  result += "<br>\r\n";
  result += getText(locale, k_code);
  result += " <input type=\"text\" name=\"roleModelCode\" maxlength=\"32\" size=\"33\" placeholder=\"01234\" value=\"";
  if (pSettings->getRoleModel() != "independent")
  {
    result += pSettings->getRoleModel();
  }
  result += "\"> ";
  result += getText(locale, k_maxCodeChars);
  result += "</div>\r\n";

  result += "</div>\r\n";

  result += "<br><br>\r\n";
  result += getText(locale, k_confirmationWait);
  result += "<br>\r\n";
  result += "<input id=\"spinButton\" type=\"button\" name=\"spinButton\" value=\"Save\" onclick=\"saveSpinSetting()\">\r\n";
  result += "<input type=\"button\" name=\"spinCancelButton\" value=\"Cancel\" onclick=\"cancelSettings()\">\r\n";

  result += "\r\n";
  result += "<br>\r\n";
  result += "  </div>\r\n";
  // end spin-content
  // start connect-content
  result += "  <div class='tab-pane' id='pills-wifi'>\r\n";
  result += "<span class=\"menu-header\">\r\n";
  result += getText(locale, k_connectHeader);
  result += "</span><br><br><br>\r\n";
  result += getText(locale, k_chooseAccessPoint);
  result += "<br>\r\n";
  result += getText(locale, k_chooseNetwork);
  result += "<br><br>\r\n";
  result += "<input type=\"radio\" name=\"wifi\" onclick=\"displayWiFiMode()\" value=\"ap\">";
  result += getText(locale, k_choiceAccessPoint);
  result += "<br>\r\n";
  result += "<input type=\"radio\" name=\"wifi\" onclick=\"displayWiFiMode()\" value=\"network\">";
  result += getText(locale, k_choiceNetwork);
  result += "<br>\r\n";
  result += "<input type=\"radio\" name=\"wifi\" onclick=\"displayWiFiMode()\" value=\"erase\">";
  result += getText(locale, k_choiceEraseWiFi);
  result += "<br>\r\n";
  result += "<br>\r\n";
  result += "<div id=\"ap\">\r\n";
  result += getText(locale, k_accessToAccessPoint);
  result += "  <br>\r\n";
  result += getText(locale, k_emptyPassword);
  result += "  <br>\r\n";
  result += getText(locale, k_ssid);
  if ((pWifiSettings->getAccessPointSSID() == "") || (pWifiSettings->getAccessPointSSID() == "ESP-") || (WiFi.softAPSSID().startsWith("ESP_")))
  {
    result += "ESP-" + WiFi.softAPmacAddress();
  }
  else
  {
    result += pWifiSettings->getAccessPointSSID();
  }
  result += "  <br>\r\n";
  result += getText(locale, k_password);
  result += "  <input type=\"password\" name=\"password\" maxlength=\"32\" size=\"33\" placeholder=\"";
  if (pWifiSettings->getAccessPointPassword() == "")
  {
    result += "administrator";
  }
  else
  {
    result += pWifiSettings->getAccessPointPassword();
  }
  result += "\" value=\"";
  result += pWifiSettings->getAccessPointPassword();
  result += "\" \r\n";
  result += getText(locale, k_checkURIComponent);
  result += " <span id=\"apPasswordMessage\"></span>\r\n";
  result += "  <br><br>\r\n";
  result += getText(locale, k_confirmationWait);
  result += "  <br>\r\n";
  result += "  <input id=\"apButton\" type=\"button\" name=\"apButton\" value=\"Save\" onclick=\"saveAP(this)\">\r\n";
  result += "  <input type=\"button\" name=\"apCancelButton\" value=\"Cancel\" onclick=\"cancelWiFi()\">\r\n";
  result += "</div>\r\n";
  result += "\r\n";
  result += "<div id=\"network\">\r\n";
  result += getText(locale, k_connectWiFiList);
  result += "  <br><br>\r\n";
  result += "  <span id=\"ssidList\">";
  result += getText(locale, k_connectWiFiListWait);
  result += "</span>\r\n";
  result += "  <br><br>\r\n";
  result += "  <span id=\"selectedWiFiPassword\">";
  result += getText(locale, k_giveNetWorkPassword);
  result += "<span id=\"selectedWiFi\"></span>\r\n";
  result += "     <br>\r\n";
  result += getText(locale, k_password);
  result += "<input type=\"password\" name=\"password\" maxlength=\"32\" size=\"33\" placeholder=\"";
  result += getText(locale, k_passwordPlaceholder);
  result += "\" value=\"";
  result += "\" onkeyup=\"checkURIComponent(this, 'networkPasswordMessage', '";
  result += "');\">\r\n";
  result += "     <br><br>\r\n";
  result += "  </span>\r\n";
  result += getText(locale, k_confirmationWait);
  result += "  <br>\r\n";
  result += "   <input id=\"networkButton\" type=\"button\" name=\"networkButton\" value=\"Save\" onclick=\"saveNetwork(this)\">\r\n";
  result += "   <input type=\"button\" name=\"networkCancelButton\" value=\"Cancel\" onclick=\"cancelWiFi()\">\r\n";
  result += "  <span id=\"networkPasswordMessage\"></span>\r\n";
  result += "</div>\r\n";
  result += "<div id=\"erase\">\r\n";
  result += getText(locale, k_warning);
  result += "  <br><br>\r\n";
  result += "  <br><br>\r\n";
  result += getText(locale, k_cancelErase);  
  result += "<input type=\"button\" name=\"networkButton\" value=\"Cancel\" onclick=\"cancelWiFi()\">\r\n";
  result += "  <br><br>\r\n";
  result += "  <input id=\"eraseAPData\" type=\"button\" name=\"eraseAPDataButton\" value=\"Erase\" onclick=\"eraseWiFiData(this)\">";
  result += getText(locale, k_eraseAccessPoint);
  result += "  <br>\r\n";
  result += "  <input id=\"eraseNetworkData\" type=\"button\" name=\"eraseNetworkDataButton\" value=\"Erase\" onclick=\"eraseWiFiData(this)\">";
  result += getText(locale, k_eraseNetwork);
  result += "  <br>\r\n";
  result += "  <input id=\"eraseWiFiData\" type=\"button\" name=\"eraseWiFiDataButton\" value=\"Erase\" onclick=\"eraseWiFiData(this)\">";
  result += getText(locale, k_eraseWiFiData);
  result += "</div>\r\n";
  result += "  </div>\r\n";
  // end connect-content
  // start info-content
  result += "  <div class='tab-pane' id='pills-info'>\r\n";
  result += "<span class=\"menu-header\">\r\n";
  result += getText(locale, k_infoHeader);
  result += "</span><br><br><br>\r\n";
  result += getText(locale, k_wiFiMode) + " ";
  if (pSettings->beginAsAccessPoint() == true)
  {
    result += getText(locale, k_accessPoint);
  }
  else
  {
    result += getText(locale, k_networkStation);
  }
  result += "<br>\r\n";
  result += getText(locale, k_for);
  result += getText(locale, k_accessPoint);
  result += ": <a href='http://model.local/' target='_blank'>model.local</a> ";
  result += getText(locale, k_or) + "\r\n";
  result += " <a href='http://192.168.4.1/' target='_blank'>http://192.168.4.1</a>)";
  pSettings->getLastNetworkIP().toCharArray(textBuffer, 64);
  const char *r_hrefNetworkAddress[] = {textBuffer, textBuffer};
  result += "<br>\r\n";
  result += getText(locale, k_for);
  result += getText(locale, k_networkStation);
  result += ": <a href='http://model.local/' target='_blank'>model.local</a> ";
  result += getText(locale, k_orLocalIP);
  result += getText(locale, k_hrefNetworkAddress, r_hrefNetworkAddress, 2);

  result += "<br><br>\r\n";

  result += "<input id='restartButton' type='button' onclick='restart()' value='Restart'<br>\r\n";
  result += "<br><br>\r\n";

  result += getText(locale, k_version);
  result += " <span id='version'></span>\r\n";
  result += " <input id='updateFirmwareButton' type='button' onclick='updateFirmware()' value='Update Firmware'<br>\r\n";
  result += " <div id=\"updateFirmwareMessage\"></div>\r\n";
  result += "<br>\r\n";




  String myIP = "";
  String startStrong = "<strong>";
  String endStrong = "</strong>";
  if (WiFi.getMode() == WIFI_MODE_AP)
  {
    myIP = WiFi.softAPIP().toString();
    startStrong = "";
    endStrong = "";
  }
  if (WiFi.getMode() == WIFI_MODE_STA)
  {
    myIP = WiFi.localIP().toString();
    pSettings->getTargetServer().toCharArray(textBuffer, 64);
    const char *r_infoMessageSent[] = {textBuffer};
    result += getText(locale, k_infoMessageSent, r_infoMessageSent, 1);
    result += "<br>\r\n";
    result += getText(locale, k_infoMessageNotSent);
    result += "<br>\r\n";
  }
  result += "\r\n<br>";
  myIP.toCharArray(textBuffer, 64);
  const char *r_ipAddress[] = {textBuffer};
  result += getText(locale, k_ipAddress, r_ipAddress, 1);
  result += "\r\n<br>";
  WiFi.macAddress().toCharArray(textBuffer, 64);
  const char *r_macAddress[] = {textBuffer};
  result += getText(locale, k_macAddress, r_macAddress, 1);
  result += "\r\n<br>";
  result += startStrong;
  pSettings->getFirmwareVersion().toCharArray(textBuffer, 64);
  const char *r_firmwareVersion[] = {textBuffer};
  result += getText(locale, k_firmwareVersion, r_firmwareVersion, 1);
  result += endStrong;
  result += "\r\n<br>";
  result += getText(locale, k_motorSettings);
  result += "\r\n<br>";
  result += startStrong;
  String(pSettings->getStepsPerRevolution()).toCharArray(textBuffer, 64);
  const char *r_stepsPerRevolution[] = {textBuffer};
  result += getText(locale, k_stepsPerRevolution, r_stepsPerRevolution, 1);
  result += "\r\n<br>";
  String(pSettings->getMaxSpeed()).toCharArray(textBuffer, 64);
  const char *r_maxSpeed[] = {textBuffer};
  result += getText(locale, k_maxSpeed, r_maxSpeed, 1);
  result += "\r\n<br>";
  String(pSettings->getDirection()).toCharArray(textBuffer, 64);
  const char *r_direction[] = {textBuffer};
  result += getText(locale, k_direction, r_direction, 1);
  result += "\r\n<br>";
  String(pSettings->getMotorInterfaceType()).toCharArray(textBuffer, 64);
  const char *r_interfaceType[] = {textBuffer};
  result += getText(locale, k_interfaceType, r_interfaceType, 1);
  result += endStrong;
  result += "\r\n<br>";
  pWifiSettings->readAccessPointSSID().toCharArray(textBuffer, 64);
  const char *r_accessPointSSID[] = {textBuffer};
  result += getText(locale, k_accessPointSSID, r_accessPointSSID, 1);
  result += "\r\n<br>";
  pWifiSettings->readNetworkSSID().toCharArray(textBuffer, 64);
  const char *r_networkSSID[] = {textBuffer};
  result += getText(locale, k_networkSSID, r_networkSSID, 1);
  result += "\r\n<br>";
  pSettings->getTargetServer().toCharArray(textBuffer, 64);
  const char *r_serverName[] = {textBuffer};
  result += getText(locale, k_serverName, r_serverName, 1);
  result += "\r\n<br>";
  String(pSettings->getTargetPort()).toCharArray(textBuffer, 64);
  const char *r_serverPort[] = {textBuffer};
  result += getText(locale, k_serverPort, r_serverPort, 1);
  result += "\r\n<br>";
  pSettings->getTargetPath().toCharArray(textBuffer, 64);
  const char *r_serverPath[] = {textBuffer};
  result += getText(locale, k_serverPath, r_serverPath, 1);

  result += "  </div>\r\n";
  // end info-content
  result += "</div>\r\n";
  // end tab-content
  result += "<div id=\"sendMessage\"></div>\r\n";


  result += "<br><hr>\r\n";
  result += "<select id='languagechoice' onchange='selectLanguage(this);'>\r\n";
  result += "  <option value='EN' id='EN'>English</option>\r\n";
  result += "  <option value='NL' id='NL'>Nederlands</option>\r\n";
  result += "</select>\r\n";

  // start General JavaScript
  result += "<script>\r\n";
  result += "  function showMenu(ev) {\r\n";
  result += "    ev.preventDefault();\r\n";
  result += "    var hash = location.hash.replace('#','');\r\n";
  result += "    if(hash != ''){\r\n";
  result += "      location.hash = '';\r\n";
  result += "    };\r\n";
  result += "    el = ev.target;\r\n";
  result += "    // off\r\n";
  result += "    document.getElementById('sendMessage').innerHTML='';\r\n";
  result += "    navItems = document.querySelectorAll('.nav-item');\r\n";
  result += "    navItems.forEach(function(navItem){\r\n";
  result += "      navItem.classList.remove('active');\r\n";
  result += "      targetPane = navItem.children[0].getAttribute('id').replace('-tab','');\r\n";
  result += "      document.getElementById(targetPane).style.display='none';\r\n";
  result += "    });\r\n";
  result += "    // on\r\n";
  result += "    el.parentNode.classList.add('active');\r\n";
  result += "    selectedMenuButtonId = el.getAttribute('id');\r\n";
  result += "    targetPane = el.getAttribute('id').replace('-tab','');\r\n";
  result += "    document.getElementById(targetPane).style.display='block';\r\n";
  result += "  };\r\n";
  result += "buttonElements = document.querySelectorAll('.nav-item');\r\n";
  result += "buttonElements.forEach(el=>el.addEventListener('click', event=>{\r\n";
  result += "  showMenu(event);\r\n";
  result += "}));\r\n";
  result += "document.getElementById('" + language + "').setAttribute('selected', 'selected');\r\n";
/* buttonAnchors is used to go to the selected html area */
  result += "const buttonAnchorArray = [\"pills-spin-tab\", \"pills-wifi-tab\", \"pills-info-tab\"];\r\n";

  result += "var selectedMenuButtonId = buttonAnchorArray[" + String(menuButton) + "];\r\n";
  result += "document.getElementById(selectedMenuButtonId).click();\r\n";
  result += "</script>\r\n";
  // end General JavaScript

  // start Home JavaScript
  result += "<script>\r\n";
  result += "  function restart() {\r\n";
  result += "    document.getElementById(\"restartButton\").disabled = true;\r\n";
  result += "    document.getElementById(\"updateFirmwareButton\").disabled = true;\r\n";
  result += "    document.getElementById(\"updateFirmwareMessage\").innerHTML = \"Please refresh this page after about 1 minute\";\r\n";
  result += "    var params = \"name=restart\";\r\n";
  result += "    sendUpdateFirmware(params, \"/restart/\");\r\n";
  result += "  };\r\n";
  result += "</script>\r\n";

  result += "<script>\r\n";
  result += "  document.getElementById(\"version\").innerHTML = \"";
  result += pSettings->getFirmwareVersion();
  result += "\";\r\n";
  result += "  function updateFirmware() {\r\n";
  result += "    document.getElementById(\"restartButton\").disabled = true;\r\n";
  result += "    document.getElementById(\"updateFirmwareButton\").disabled = true;\r\n";
  result += "    document.getElementById(\"updateFirmwareMessage\").innerHTML = \"Please refresh this page after about 1 minute\";\r\n";
  result += "    var params = \"name=update\";\r\n";
  result += "    sendUpdateFirmware(params, \"/update/\");\r\n";
  result += "  };\r\n";
  result += "</script>\r\n";
  result += "<script>\r\n";
  result += "  function selectLanguage(component) {\r\n";
  result += "    var selectedMenuButtonNumber = buttonAnchorArray.indexOf(selectedMenuButtonId);\r\n";
  result += "    if (selectedMenuButtonNumber < 0) {\r\n";
  result += "      selectedMenuButtonNumber = 0;\r\n";
  result += "    }\r\n";
  result += "    var params = \"name=help\" + \"&language=\" + component.value + \"&menuButton=\" + selectedMenuButtonNumber;\r\n";
  result += "    document.getElementById(\"NL\").removeAttribute('selected');\r\n";
  result += "    document.getElementById(\"EN\").removeAttribute('selected');\r\n";
  result += "    sendDataHome(params, \"/language/\");\r\n";
  result += "  }\r\n";
  result += "  function sendDataHome(data, path) {\r\n";
  result += "    var xhr = new XMLHttpRequest();   // new HttpRequest instance\r\n";
  result += "    xhr.open(\"POST\", path);\r\n";
  result += "    xhr.setRequestHeader(\"Content-Type\", \"application/x-www-form-urlencoded; charset=UTF-8\");\r\n";
  result += "    //xhr.setRequestHeader(\"Content-Type\", \"application/json\");\r\n";
  result += "   document.getElementById(\"sendMessage\").innerHTML = \"Please wait\";\r\n";
  result += "    xhr.onreadystatechange = function() { // Call a function when the state changes\r\n";
  result += "     var myResponseText = \"\";\r\n";
  result += "      if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {\r\n";
  result += "        window.location.reload();\r\n";
  result += "     }\r\n";
  result += "      if (this.readyState === XMLHttpRequest.DONE && this.status !== 200) {\r\n";
  result += "       myResponseText = this.statusText || \"\";\r\n";
  result += "      }\r\n";
  result += "      document.getElementById(\"sendMessage\").innerHTML = myResponseText;\r\n";
  result += "    }\r\n";
  result += "    xhr.send(data);\r\n";
  result += "  }\r\n";

  result += "  function sendUpdateFirmware(data, path) {\r\n";
  result += "    var xhr = new XMLHttpRequest();   // new HttpRequest instance\r\n";
  result += "    xhr.open(\"POST\", path);\r\n";
  result += "    xhr.setRequestHeader(\"Content-Type\", \"application/x-www-form-urlencoded; charset=UTF-8\");\r\n";
  result += "   document.getElementById(\"sendMessage\").innerHTML = \"Please wait\";\r\n";
  result += "    xhr.onreadystatechange = function() { // Call a function when the state changes\r\n";
  result += "     var myResponseText = \"\";\r\n";
  result += "      if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {\r\n";
  result += "        window.location.reload();\r\n";
  result += "     }\r\n";
  result += "      if (this.readyState === XMLHttpRequest.DONE && this.status !== 200) {\r\n";
  result += "       myResponseText = this.statusText || \"\";\r\n";
  result += "      }\r\n";
  result += "      document.getElementById(\"sendMessage\").innerHTML = myResponseText;\r\n";
  result += "    }\r\n";
  result += "    xhr.send(data);\r\n";
  result += "  }\r\n";

  result += "</script>\r\n";
  // end Home JavaScript

  // start Spin JavaScript
  result += "<script>\r\n";

  result += "function clearMessage() {\r\n";
  result += "  document.getElementById(\"sendMessage\").innerHTML = \"\";\r\n";
  result += "}\r\n";

  result += "  function checkNumber(component, messageId, message) {\r\n";
  result += "  //var validCharacterString = \"0123456789-.\";\r\n";
  result += "  var valid = false;\r\n";
  result += "    if ((component.value >= Number(component.getAttribute(\"min\"))) && (component.value <= Number(component.getAttribute(\"max\")))) {\r\n";
  result += "        valid = true;\r\n";
  result += "    }\r\n";
  result += "    if (valid) {\r\n";
  result += "      document.getElementById(messageId).innerHTML = \"\";\r\n";
  result += "    }\r\n";
  result += "    else {\r\n";
  result += "      document.getElementById(messageId).innerHTML = message;\r\n";
  result += "      component.value = component.getAttribute(\"max\");\r\n";
  result += "    }\r\n";
  result += "    return valid;\r\n";
  result += "  }\r\n";
  result += "\r\n";

  result += "  function displaySpinSetting() {\r\n";
  result += "    var spinMode = \"\"\r\n";
  result += "    var ele = document.getElementsByName(\"spin\") || [];\r\n";
  result += "    for(i = 0; i < ele.length; i++) {\r\n";
  result += "      if(ele[i].checked) {\r\n";
  result += "        spinMode = ele[i].value;\r\n";
  result += "      }\r\n";
  result += "    }\r\n";
  result += "    if (spinMode == \"independent\") {\r\n";
  result += "        document.getElementById(\"spinSettingSpeed\").style.display=\"block\";\r\n";
  result += "        document.getElementById(\"spinSettingCode\").style.display=\"none\";\r\n";
  result += "    }\r\n";
  result += "    else {\r\n";
  result += "        document.getElementById(\"spinSettingSpeed\").style.display=\"none\";\r\n";
  result += "        document.getElementById(\"spinSettingCode\").style.display=\"block\";\r\n";
  result += "    }\r\n";
  result += "    if (spinMode == \"stop\") {\r\n";
  result += "        document.getElementById(\"spinSettingSpeed\").style.display=\"none\";\r\n";
  result += "        document.getElementById(\"spinSettingCode\").style.display=\"none\";\r\n";
  result += "    };\r\n";
  result += "}\r\n";

  result += "  function cancelSettings() {\r\n";
  result += "    window.location.reload();\r\n";
  result += "  }\r\n";
  result += "  function sendDataSpin(data) {\r\n";
  result += "    var xhr = new XMLHttpRequest();   // new HttpRequest instance\r\n";
  result += "    xhr.open(\"POST\", \"/spinSettings/\");\r\n";
  result += "    xhr.setRequestHeader(\"Content-Type\", \"application/x-www-form-urlencoded; charset=UTF-8\");\r\n";
  result += "      document.getElementById(\"sendMessage\").innerHTML = \"Please wait\";\r\n";
  result += "      xhr.onreadystatechange = function() { // Call a function when the state changes\r\n";
  result += "        var myResponseText = \"\";\r\n";
  result += "        if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {\r\n";
  result += "          myResponseText = this.responseText || \"\";\r\n";
  result += "        }\r\n";
  result += "        if (this.readyState === XMLHttpRequest.DONE && this.status !== 200) {\r\n";
  result += "          myResponseText = this.statusText || \"\";\r\n";
  result += "        }\r\n";
  result += "        document.getElementById(\"sendMessage\").innerHTML = myResponseText;\r\n";
  result += "      }\r\n";
  result += "    xhr.send(data);\r\n";
  result += "  }\r\n";
  result += "\r\n";
  result += "  function saveSpinSetting() {\r\n";
  result += "    var ele = document.getElementsByName(\"spin\");\r\n";
  result += "    var spinmode = \"\";\r\n";
  result += "    for(i = 0; i < ele.length; i++) {\r\n";
  result += "      if(ele[i].checked) {\r\n";
  result += "        spinMode = ele[i].value;\r\n";
  result += "      }\r\n";
  result += "    }\r\n";
  result += "    var roleModelSpeed = document.getElementsByName(\"roleModelSpeed\")[0].value || \"\";\r\n";
  result += "    var roleModelCode = document.getElementsByName(\"roleModelCode\")[0].value || \"\";\r\n";
  result += "    var params = \"name=spin\" + \"&spinMode=\" + spinMode + \"&roleModelSpeed=\" + roleModelSpeed + \"&roleModelCode=\" + roleModelCode;\r\n";
  result += "    sendDataSpin(params);\r\n";
  result += "  }\r\n";
  result += "displaySpinSetting();\r\n";
  result += "</script>\r\n";
  // end Spin Javascript

  // start Connect JavaScript
  result += "<script>\r\n";
  result += "function checkURIComponent(component, messageId, message) {\r\n";
  result += "  var invalidCharacterArray = [\" \"];\r\n";
  result += "  var buttonNodeId = component.parentNode.id;\r\n";
  result += "  if (buttonNodeId == \"selectedWiFiPassword\") {\r\n";
  result += "    buttonNodeId = component.parentNode.parentNode.id;\r\n";
  result += "  };\r\n";
  result += "  var buttonId = buttonNodeId + \"Button\";\r\n";
  result += "   var valid = true;\r\n";
  result += "   for (var c = 0; c < invalidCharacterArray.length; c++) {\r\n";
  result += "    if (component.value.indexOf(invalidCharacterArray[c]) > -1) {\r\n";
  result += "      valid = false;\r\n";
  result += "      break;\r\n";
  result += "    };\r\n";
  result += "  };\r\n";
  result += "  if (valid) {\r\n";
  result += "    document.getElementById(messageId).innerHTML = \"\";\r\n";
  result += "  }\r\n";
  result += "  else {\r\n";
  result += "    document.getElementById(messageId).innerHTML = message;\r\n";
  result += "  }\r\n";
//  result += "  document.getElementById(buttonId).disabled = !valid;\r\n";
  result += "   return valid;\r\n";
  result += "}\r\n";
  result += "  function cancelWiFi() {\r\n";
  result += "    window.location.reload();\r\n";
  result += "  }\r\n";
  result += "  function sendDataConnect(data) {\r\n";
  result += "    var xhr = new XMLHttpRequest();   // new HttpRequest instance\r\n";
  result += "    xhr.open(\"POST\", \"/wifiConnect/\");\r\n";
  result += "    xhr.setRequestHeader(\"Content-Type\", \"application/x-www-form-urlencoded; charset=UTF-8\");\r\n";
  result += "    document.getElementById(\"sendMessage\").innerHTML = \"Please wait\";\r\n";
  result += "    xhr.onreadystatechange = function() { // Call a function when the state changes\r\n";
  result += "     var myResponseText = \"\";\r\n";
  result += "      if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {\r\n";
  result += "       myResponseText = this.responseText || \"\";\r\n";
  result += "     }\r\n";
  result += "      if (this.readyState === XMLHttpRequest.DONE && this.status !== 200) {\r\n";
  result += "       myResponseText = this.statusText || \"\";\r\n";
  result += "      }\r\n";
  result += "      document.getElementById(\"sendMessage\").innerHTML = myResponseText;\r\n";
  result += "    }\r\n";
  result += "    xhr.send(data);\r\n";
  result += "  }\r\n";
  result += "\r\n";
  result += "  function saveAP(content) {\r\n";
  result += "    var children = content.parentNode.childNodes;\r\n";
  result += "    var ssid = \"\";\r\n";
  result += "    var password = \"\";\r\n";
  result += "    for (var i = 0; i < children.length; i++) {\r\n";
  result += "      if (children[i].name == \"ssid\") {\r\n";
  result += "        ssid = children[i].value || \"\";\r\n";
  result += "      }\r\n";
  result += "      if (children[i].name == \"password\") {\r\n";
  result += "        password = children[i].value || \"\";\r\n";
  result += "      }\r\n";
  result += "    }\r\n";
  result += "    var params = \"name=ap\" + \"&ssid=\" + encodeURIComponent(ssid) + \"&password=\" + encodeURIComponent(password);\r\n";
  result += "    sendDataConnect(params);\r\n";
  result += "  }\r\n";
  result += "\r\n";
  result += "  function saveNetwork(content) {\r\n";
  result += "    var children = document.getElementById(\"selectedWiFiPassword\").childNodes;\r\n";
  result += "    var ssid = document.getElementById(\"selectedWiFi\").innerHTML;\r\n";
  result += "    var password = \"\";\r\n";
  result += "    for (var i = 0; i < children.length; i++) {\r\n";
  result += "      if (children[i].name == \"password\") {\r\n";
  result += "        password = children[i].value || \"\";\r\n";
  result += "      }\r\n";
  result += "    }\r\n";
  result += "    var params = \"name=network\" + \"&ssid=\" + encodeURIComponent(ssid) + \"&password=\" + encodeURIComponent(password);\r\n";
  result += "    sendDataConnect(params);\r\n";
  result += "  }\r\n";
  result += "  function eraseWiFiData(content) {\r\n";
  result += "  var params = \"name=erase\" + \"&target=\" + content.id;\r\n";
  result += "      sendDataConnect(params);\r\n";
  result += "  }\r\n";
  result += "</script>\r\n";
  result += "\r\n";
  result += "<script>\r\n";

  result += "function showNetworkList(networks) {\r\n";
  result += "  var showNetworkList = [];\r\n";
  result += "  var showNetwork = \"\";\r\n";
  result += "  var networkList = (new Function(\"return [\" + networks + \"];\")());\r\n";
  result += "  networkList.sort((a,b) => (a.dBm > b.dBm) ? 1: (a.dBm === b.dBm) ? ((a.ssid > b.ssid) ? 1: -1) : -1);\r\n";
  result += "  networkList = networkList.filter(function(e) { return e.dBm > -80});\r\n";
  result += "  sortedList = [...networkList].reverse();\r\n";
  result += "  for (var key in sortedList) {\r\n";
  result += "    var value = sortedList[key];\r\n";
  result += "    showNetworkList.push(Object.values(value)[0]);\r\n";
  result += "  }\r\n";
  result += "  var uniqueShowNetworkList = [...new Set(showNetworkList)];\r\n";
  result += "  for (var key in uniqueShowNetworkList) {\r\n";
  result += "    showNetwork += \"<div>\" + uniqueShowNetworkList[key].replaceAll(\"?\",\"'\") + \"</div>\";\r\n";
  result += "  }\r\n";
  result += "  if (showNetwork == \"\") {\r\n";
  result += "    showNetwork = \"No WiFi signal available\";\r\n";
  result += "  }\r\n";
  result += "  else {\r\n";
  result += "    document.getElementById(\"selectedWiFiPassword\").style.display=\"block\";\r\n";
  result += "  }\r\n";
  result += "  document.getElementById(\"ssidList\").innerHTML = showNetwork;\r\n";
  result += "}\r\n"; 
  result += "function selectNetworkSSID(content) {\r\n";
  result += "  var ssid = \"\";\r\n";
  result += "  if (content.name == \"networkSSID\") {\r\n";
  result += "    ssid = content.value;\r\n";
  result += "  }\r\n";
  result += "  if (ssid != \"\"){\r\n";
  result += "    document.getElementById(\"selectedWiFi\").innerHTML = ssid;\r\n";
  result += "    document.getElementById(\"networkButton\").disabled = false;\r\n";
  result += "  }\r\n";
  result += "}\r\n";
  result += "function loadWiFiNetworkList() {\r\n";
  result += "  var xhttp = new XMLHttpRequest();\r\n";
  result += "  xhttp.onreadystatechange = function() {\r\n";
  result += "    if (this.readyState == 4 && this.status == 200) {\r\n";
  result += "      showNetworkList(this.responseText);\r\n";
  result += "    }\r\n";
  result += "  };\r\n";
  result += "  xhttp.open(\"GET\", \"/networkssid/\", true);\r\n";
  result += "  xhttp.send();\r\n";
  result += "}\r\n";
  result += "</script>\r\n";
  result += "\r\n";
  result += "<script>\r\n";
  result += "function displayWiFiMode() {\r\n";
  result += "var ele = document.getElementsByName('wifi');\r\n";
  result += "\r\n";
  result += "  for(i = 0; i < ele.length; i++) {\r\n";
  result += "    if(ele[i].checked) {\r\n";
  result += "       document.getElementById(ele[i].value).style.display=\"block\";\r\n";
  result += "       if (ele[i].value == 'network') {\r\n";
  result += "        document.getElementById(\"networkButton\").disabled = true;\r\n";
  result += "        document.getElementById(\"selectedWiFi\").innerHTML = \"\";\r\n";
  result += "        document.getElementById(\"selectedWiFiPassword\").style.display=\"none\";\r\n";
  result += "         loadWiFiNetworkList();\r\n";
  result += "       }\r\n";
  result += "     }\r\n";
  result += "     else {\r\n";
  result += "       document.getElementById(ele[i].value).style.display=\"none\";\r\n";
  result += "     }\r\n";
  result += "  }\r\n";
  result += "}\r\n";
  result += "displayWiFiMode();\r\n";
  result += "</script>\r\n";
  // end Connect JavaScript

  result += "\r\n</body>\r\n</html>\r\n";
  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Connection", "keep-alive");
  server.sendHeader("Pragma", "no-cache");
  server.send(200, "text/html", result);
}

///////////////// sse is language independent ///////////////////////////
void sse(WebServer &server, Settings * pSettings, uint32_t revolutions, uint32_t viewPulsesPerMinute)
{
  String result = "retry: ";
  result += String(pSettings->SSE_RETRY);
  result += "\r\n";
  result += "data: ";
  result += "{";
  result += "\"revolutions\":";
  result += String(revolutions);
  result += ",";
  result += "\"viewPulsesPerMinute\":";
  result += String(viewPulsesPerMinute);
  result += "}\r\n\r\n";
  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Connection", "keep-alive");
  server.sendHeader("Pragma", "no-cache");
  server.send(200, "text/event-stream", result);

}
