#include "wifi_ap.h"

WifiAp::WifiAp() {
    pinMode(kLedStatus_, OUTPUT);
    digitalWrite(kLedStatus_, LOW);
}

void WifiAp::Start(String ssid_name) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid_name.c_str());
    IPAddress local_ip(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
}

void WifiAp::Blink() {
    if (millis() - last_blink_AP_ > kBlinkingTimeAPMode_) {
        Serial.print("/");
        digitalWrite(kLedStatus_, !digitalRead(kLedStatus_));
        last_blink_AP_ = millis();
    }
}
