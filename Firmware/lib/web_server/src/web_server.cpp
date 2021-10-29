#include "web_server.h"

#include <ArduinoJson.h>

#include "file_system.h"
#include "gpio.h"
#include "utils.h"

WebServer::WebServer(Device *device) {
    device_ = device;
    server_ = new AsyncWebServer(kPort_);
}

void WebServer::Init() { SetupWebServer(); }

String WebServer::FillPlaceholders(const String &var) {
    Serial.println(var);
    if (var == "LOGIN") {
        return person_mail;
    }
    if (var == "TOKEN") {
        return token;
    }
    if (var == "HOSTNAME") {
        return host;
    }
    if (var == "BRPORT") {
        return broker_port;
    }
    if (var == "PRODUCTID") {
        return product_id;
    }
    if (var == "DEVICEID") {
        return device_id;
    }

    if (var == "FIRMWARE") {
        return firmware_name;
    }
    return String();
}

void WebServer::OnRequestWithAuth(AsyncWebServerRequest *request, ArRequestHandlerFunction onRequest) {
    if (!request->authenticate(http_username, web_auth_password.c_str())) return request->requestAuthentication();

    onRequest(request);
}

void WebServer::SetupWebServer() {
    server_->on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/index.html", String(), false,
                          [this](const String &var) { return FillPlaceholders(var); });
        });
    });

    server_->on("/index.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/index.html", String(), false,
                          [this](const String &var) { return FillPlaceholders(var); });
        });
    });

    server_->on("/header.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/header.html", String(), false,
                          [this](const String &var) { return FillPlaceholders(var); });
        });
    });

    server_->on("/wifi.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/wifi.html", String(), false,
                          [this](const String &var) { return FillPlaceholders(var); });
        });
    });

    server_->on("/settings.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/settings.html", String(), false,
                          [this](const String &var) { return FillPlaceholders(var); });
        });
    });

    server_->on("/system.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/system.html", String(), false,
                          [this](const String &var) { return FillPlaceholders(var); });
        });
    });

    server_->on("/static/favicon.png", HTTP_GET,
                [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/static/favicon.png", "image/png"); });

    server_->on("/static/logo.svg", HTTP_GET,
                [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/static/logo.svg", "image/svg+xml"); });

    server_->on("/styles.css", HTTP_GET,
                [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/styles.css", "text/css"); });

    server_->on("/healthcheck", HTTP_GET,
                [](AsyncWebServerRequest *request) { request->send(200, "text/html", "OK"); });

    server_->on("/reboot", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
            request->send(200, "text/plain", "OK");
            delay(kResponseDelay_);
            ESP.restart();
        });
    });

    server_->on("/resetdefault", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
            if (!EraseFlash()) {
                request->send(500, "text/plain", "Server error");
                return;
            }
            request->send(200, "text/plain", "OK");
            delay(kResponseDelay_);
            ESP.restart();
        });
    });

    server_->on("/newauthpass", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
            if (!request->hasParam("newpass")) {
                request->send(400);
                return;
            }

            web_auth_password = request->getParam("newpass")->value();
            if (!SaveConfig()) {
                request->send(500, "text/plain", "Server error");
                return;
            }

            request->send(200, "text/plain", "OK");
            delay(kResponseDelay_);
            ESP.restart();
        });
    });

    server_->on("/setwifi", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
            if (!request->hasParam("ssid") || !request->hasParam("pass")) {
                request->send(400);
                return;
            }

            ssid_name = request->getParam("ssid")->value();
            ssid_password = request->getParam("pass")->value();

            if (!SaveConfig()) {
                request->send(500, "text/plain", "Server error");
                return;
            }
            request->send(200, "text/plain", "OK");
            delay(kResponseDelay_);
            ESP.restart();
        });
    });

    server_->on("/scan", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
            DynamicJsonDocument doc(1024);
            int n = WiFi.scanComplete();
            if (n == WIFI_SCAN_FAILED) {
                WiFi.scanNetworks(true);
            } else if (n) {
                for (int i = 0; i < n; ++i) {
                    doc[WiFi.SSID(i)] = String(WiFi.encryptionType(i));
                }
                WiFi.scanDelete();
                if (WiFi.scanComplete() == WIFI_SCAN_FAILED) {
                    WiFi.scanNetworks(true);
                }
            }
            String response;
            serializeJson(doc, response);
            request->send(200, "application/json", response);
        });
    });

    server_->on("/connectedwifi", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [](AsyncWebServerRequest *request) {
            request->send(200, "text/plain", WiFi.status() == WL_CONNECTED ? ssid_name : "NULL");
        });
    });

    server_->on("/setcredentials", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
            if (!request->hasParam("mail") || !request->hasParam("token") || !request->hasParam("hostname") ||
                !request->hasParam("brokerPort") || !request->hasParam("product_id") ||
                !request->hasParam("device_id")) {
                request->send(400, "text/plain", "Incorrect data");
                return;
            }

            person_mail = request->getParam("mail")->value();
            token = request->getParam("token")->value();
            host = request->getParam("hostname")->value();
            broker_port = request->getParam("brokerPort")->value();
            product_id = request->getParam("product_id")->value();
            device_id = request->getParam("device_id")->value();
            person_id = Sha256(person_mail);

            Serial.println(person_mail);
            Serial.println(person_id);
            Serial.println(token);
            Serial.println(host);
            Serial.println(broker_port);
            Serial.println(product_id);
            Serial.println(device_id);
            if (!SaveConfig()) {
                request->send(500, "text/plain", "Server error");
                return;
            }

            request->send(200, "text/plain", "OK");
            delay(kResponseDelay_);
            ESP.restart();
        });
    });

    server_->on("/pair", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (!request->hasParam("ssid") || !request->hasParam("psk") || !request->hasParam("wsp") ||
            !request->hasParam("token") || !request->hasParam("host") || !request->hasParam("brport")) {
            request->send(400, "text/plain", "Incorrect data");
            return;
        }

        ssid_name = request->getParam("ssid")->value();
        ssid_password = request->getParam("psk")->value();
        person_mail = request->getParam("wsp")->value();
        token = request->getParam("token")->value();
        host = request->getParam("host")->value();
        broker_port = request->getParam("brport")->value();

        String devId = WiFi.macAddress();
        devId.toLowerCase();
        devId.replace(":", "-");
        device_id = devId;
        person_id = Sha256(person_mail);
        Serial.println("WebServer update:");
        Serial.println("SSID_Name = " + ssid_name);
        Serial.println("SSID_Password = " + ssid_password);
        Serial.println("person_mail = " + person_mail);
        Serial.println("person_id = " + person_id);
        Serial.println("token = " + token);
        Serial.println("host = " + host);
        Serial.println("brport = " + broker_port);
        Serial.println("device_id = " + device_id);
        if (ssid_name == "") {
            request->send(400, "text/plain", "Wifi name is NULL");
            return;
        }
        if (!SaveConfig()) {
            request->send(500, "text/plain", "Server error");
            return;
        }

        request->send(200, "text/plain", "OK");
        delay(kResponseDelay_);
        ESP.restart();
    });

    server_->on("/update", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
            if (!request->hasParam("output") || !request->hasParam("state")) {
                request->send(400);
                return;
            }
            String output;
            String state;
            output = request->getParam("output")->value();
            state = request->getParam("state")->value();
            if (output == "relay1") {
                Node *node = device_->GetNode("lenta");
                Property *property = node->GetProperty("state");
                property->SetValue(state == "1" ? "true" : "false");
                request->send(200, "text/plain", "OK");
            } else {
                request->send(400);
            }
        });
    });

    server_->on("/settings", HTTP_GET, [this](AsyncWebServerRequest *request) {
        OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
            StaticJsonDocument<256> doc;
            Node *node = device_->GetNode("lenta");
            Property *property = node->GetProperty("state");
            String led_state = property->GetValue();
            doc["data"]["relay1"] = led_state == "true" ? "1" : "0";
            String response;
            serializeJson(doc, response);
            request->send(200, "application/json", response);
        });
    });

    server_->on(
        "/firmware/upload", HTTP_POST,
        [this](AsyncWebServerRequest *request) {
            OnRequestWithAuth(request, [this](AsyncWebServerRequest *request) {
                request->send((Update.hasError()) ? 500 : 200);
                delay(500);
                ESP.restart();
            });
        },
        [this](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len,
               bool final) { OnFirmwareUpload(request, filename, index, data, len, final); });

    server_->onNotFound([](AsyncWebServerRequest *request) { request->send(404); });

    server_->begin();
}

void WebServer::OnFirmwareUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
                                 size_t len, bool final) {
    if (!index) {
        uint32_t freeSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if (!Update.begin(freeSpace)) {
            Update.printError(Serial);
        }
    }
    if (!Update.hasError()) {
        if (Update.write(data, len) != len) {
            Update.printError(Serial);
        }
    }

    if (final) {
        if (!Update.end(true)) {
            Update.printError(Serial);
        } else {
            Serial.println("Update complete");
        }
    }
}
