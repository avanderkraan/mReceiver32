#include "handlemDNS.h"

//String firstFreeHostname = "";

MDNSResponder myMDNSResponder;

void startmDNS() {
    String hostname = "model";
    esp_err_t err = mdns_init();
    if (err)
    {
        printf("MDNS failed: %d\n", err);
        return;
    }
    printf("MDNS started\n");
    hostname = findFirstFreeHostname();
    setHostname(hostname);
    addService(hostname);
}

void addService(String hostname){
    uint16_t port = 80;
    //String instanceName = findFirstFreeHostname();
    //myMDNSResponder.setInstanceName(instanceName);
    mdns_service_add(hostname.c_str(), "_http", "_tcp", port, NULL, 0);
    //mdns_service_instance_name_set("_http", "_tcp", instanceName.c_str());
}

String getMDNSHostname() {
    // not working as expected yet
    // TODO: make this work
    return WiFi.getHostname();
}


bool setHostname(String hostname)
{
    if (hostname) 
    {
        uint16_t index = 0;
        esp_err_t err = mdns_hostname_set(hostname.c_str());
        String indexedHostname = "";
        if (err != ESP_OK)
        {
            return false;
        }

        err = mdns_instance_name_set(hostname.c_str());
        if (err != ESP_OK)
        {
            return false;
        }


        if (WiFi.getMode() == WIFI_MODE_STA)
        {
            WiFi.setHostname(hostname.c_str());
            printf("SetStationHostname: Station hostname is set to %s\n", hostname.c_str());
        }
        if (WiFi.getMode() == WIFI_MODE_AP)
        {
            WiFi.softAPsetHostname(hostname.c_str());
            printf("SetSoftAPHostname: AP hostname is set to %s\n", hostname.c_str());
        }

    }
    else
    {
        return false;
    }

    return true;
}

String findFirstFreeHostname() {
    String hostname = "model";
    uint32_t timeout = 1500;
    String indexedHostname = hostname;
    int index = 0;
    
    int nrOfServices = myMDNSResponder.queryService("http", "tcp");

    IPAddress ipHost = myMDNSResponder.queryHost(indexedHostname);
    if (ipHost)
    {
        //printf("iphost model  true\n");
    }

    for (int i = 0; i < nrOfServices; i++)
    {
        if (myMDNSResponder.hostname(i))
        {
            index+=1;
        }
    }
    indexedHostname = String(hostname);
    if (index > 1)
    {
        indexedHostname = String(hostname) + "-" + String(index);
    }
    return indexedHostname;
}
