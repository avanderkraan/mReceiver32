#include "handlemDNS.h"

String firstFreeHostName = "";

const char * getFirstFreeHostName(const char * hostName)
{
    int index = 0;
    int nrOfServices = MDNS.queryService("http", "tcp");
    String indexedHostName = String(hostName);

    for (int i = 0; i < nrOfServices; i++)
    {
        indexedHostName = String(hostName) + "-" + String(i);
        IPAddress ipHost = MDNS.queryHost(indexedHostName);
        //Serial.println(ipHost.toString());
        if (ipHost)
        {
            index+=1;
        }
    }
    firstFreeHostName = String(hostName);
    if (index > 0)
    {
        firstFreeHostName = String(hostName) + "-" + String(index);
    }
    return firstFreeHostName.c_str();
}

bool startmDNS()
{
    bool result = false;
    if (!MDNS.begin(getFirstFreeHostName("model")))
    {
        Serial.println("Error setting up mDNS Responder");
        while(1) {
            delay(1000);
        }
    }
    Serial.println("MDNS responder started");
    //result = (MDNS.addService("http", "tcp", 80) && _setStationHostname());
    return result;
}

bool _setStationHostname()
{
    bool result = false;
    if (firstFreeHostName != "") {
        result = WiFi.hostname(firstFreeHostName);
    }
    if (result)
    {
        Serial.println("SetStationHostname: Station hostname is set to " + firstFreeHostName);
    }
    return false;
}

