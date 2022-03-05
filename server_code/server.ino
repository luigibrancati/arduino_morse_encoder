#include "server_utils.h"

void setup(){
    Serial.begin(115200);
    // Setup static IP and hostname
    Serial.println("Configuring WIFI");
    WiFi.mode(WIFI_STA);
    if (!WiFi.config(serverIP, serverGateway, serverSubnet, serverPrimaryDNS, serverSecondaryDNS)) {
        Serial.println("STA Failed to configure");
    }
    Serial.println("Hostname set to "+serverHostname);
    WiFi.setHostname(serverHostname.c_str());
    connectToWiFi();
    setupRouting();
}

void loop(){
    server.handleClient();
    delay(500);
}