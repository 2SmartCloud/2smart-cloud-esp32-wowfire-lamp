
#pragma once
#include <ESPAsyncWebServer.h>
#include <Update.h>

#include "device/device.h"

extern const char *http_username;

extern String ssid_name;
extern String ssid_password;
extern String person_mail;
extern String person_id;
extern String token;
extern String host;
extern String broker_port;
extern String web_auth_password;
extern String device_id;
extern String product_id;
extern const char *firmware_name;

class WebServer {
 public:
    explicit WebServer(Device *device);

    void Init();

 private:
    const uint8_t kPort_ = 80;
    const uint16_t kResponseDelay_ = 500;  // delay before reboot

    void SetupWebServer();

    void OnFirmwareUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
                          size_t len, bool final);
    void OnRequestWithAuth(AsyncWebServerRequest *request, ArRequestHandlerFunction onRequest);

    String FillPlaceholders(const String &var);

    AsyncWebServer *server_ = nullptr;
    Device *device_ = nullptr;
};
