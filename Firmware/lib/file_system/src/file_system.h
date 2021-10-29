#pragma once
#include <SPIFFS.h>  // For FS & FileConfig

typedef struct {
    char ssid_name[33];      // WiFi name
    char ssid_password[65];  // WiFi password
    char person_mail[65];
    char person_id[65];
    char token[11];
    char host[65];
    char broker_port[6];
    char device_id[32];
    char product_id[32];
    char web_auth_password[32];
} UserData;

extern String ssid_name;
extern String ssid_password;
extern String person_mail;
extern String person_id;
extern String token;
extern String host;
extern String broker_port;
extern String device_id;
extern String product_id;
extern String web_auth_password;

bool InitFiles();
bool LoadConfig();
bool SaveConfig();
bool EraseFlash();
bool WriteSettings(const char *file_path, uint8_t *data, size_t size_of_data);
bool ReadSettings(const char *file_path, uint8_t *data, size_t size_of_data);
