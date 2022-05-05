#pragma once
#include <Arduino.h>

#include "file_system/src/file_system.h"
#include "gpio.h"
#include "homie.h"
#include "lenta.h"
#include "web_server.h"
#include "wifi_ap/src/wifi_ap.h"
#include "wifi_client/src/wifi_client.h"

#include "custom_nodes/reset_button/rst_button.h"
#include "custom_sensors/UpdateTime.h"
#include "custom_sensors/wifisignal/WifiSignal.h"
// ----------------------------------------------------------HTTP-----------
String ssid_name = "Wifi_Name";  // WiFi name
String ssid_password = "";       // WiFi password
String ap_password = "";         // initial access point password, 8-63 symbols
                                 // |NOTE| if left empty, AP will start open, with no auth
String person_mail = "";
String person_id = "";
String token = "";
String host = "cloud.2smart.com";
String broker_port = "1883";
String web_auth_password = "";
const char* http_username = "admin";
// -------------------------------------------------------Production settings
String device_id = "";  // DeviceID/ MAC:adress
String product_id = "1635262012594842";
// -------------------------------------------------------MQTT variables

const char* device_name = "2Smart Lamp";
const char* device_version = "5";
const char *firmware_name = product_id.c_str();

void HandleMessage(char* topic, byte* payload, unsigned int length);
