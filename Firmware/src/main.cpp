#include "main.h"

#include "file_system/src/file_system.h"
#include "gpio.h"
#include "homie.h"
#include "lenta.h"
#include "web_server.h"
#include "wifi_ap/src/wifi_ap.h"
#include "wifi_client/src/wifi_client.h"

MqttClient *mqtt_client = new MqttClient();
Homie homie(mqtt_client);
Notifier notifier(mqtt_client);
Device device(&homie);
WebServer web_server(&device);
NtpTimeClient *time_client = new NtpTimeClient();

WifiClient wifi_client;

void setup() {
    Serial.begin(115200);
    setGpios();
    if (!InitFiles() || !LoadConfig()) {
        delay(5000);
        ESP.restart();
    }

    String mac = WiFi.macAddress().c_str();
    if (device_id.length() <= 1) {
        String bufferMacAddr = WiFi.macAddress();
        bufferMacAddr.toLowerCase();
        bufferMacAddr.replace(":", "-");
        device_id = bufferMacAddr;
    }
    String ip_addr = WiFi.localIP().toString();

    // ---------------------------------------------- Homie convention init
    AutoUpdateFw *firmware = new AutoUpdateFw("Firmware", "firmware", &device);                   // (name, id, device)
    Notifications *notifications = new Notifications("Notifications", "notifications", &device);  // (name, id, device)

    Property *update_status = new Property("update status", "updatestate", firmware, SENSOR, false, false, "string");
    Property *update_button = new Property("update button", "update", firmware, SENSOR, true, false, "boolean");
    Property *update_time = new Property("update time", "updatetime", firmware, SENSOR, true, true, "string");
    Property *auto_update = new Property("autoUpdate", "autoupdate", firmware, SENSOR, true, true, "boolean");
    Property *fw_version = new Property("version", "version", firmware, SENSOR, false, true, "integer");
    // ------------- notification`s properties
    Property *system_notification =
        new Property("System Notifications", "system", notifications, SENSOR, true, true, "boolean");
    Property *update_notification =
        new Property("Update Notifications", "update", notifications, SENSOR, true, true, "boolean");

    DeviceData device_data{device_name, device_version, product_id.c_str(), ip_addr.c_str(), "esp32",
                           mac.c_str(), "ready",        device_id.c_str()};
    notifier.SetUserHash(person_id);

    device.SetCredentials(device_data);
    device.SetNotifier(&notifier);

    Property *dev_ip = new Property("ipw", "ipw", &device, TELEMETRY, false, true, "string");
    device.AddProperty(dev_ip);

    firmware->AddProperty(fw_version);
    firmware->AddProperty(update_status);
    firmware->AddProperty(update_button);
    firmware->AddProperty(update_time);
    firmware->AddProperty(auto_update);
    firmware->SetTimeClient(time_client);
    device.AddNode(firmware);

    notifications->AddProperty(system_notification);
    notifications->AddProperty(update_notification);
    device.AddNode(notifications);

    /* -------------------- Start init your nodes and properties --------------------*/

    Lenta *lenta = new Lenta("Lenta", "lenta", &device);  // (name, id, device)
    Property *lenta_status = new Property("state", "state", lenta, SENSOR, true, true, "boolean");
    Property *lenta_mode = new Property("mode", "mode", lenta, SENSOR, true, true, "enum", lenta->GetModes());
    Property *lenta_color = new Property("color", "color", lenta, SENSOR, true, true, "color", "rgb");
    Property *lenta_brightness =
        new Property("brightness", "brightness", lenta, SENSOR, true, true, "integer", "0:100");
    Property *lenta_text = new Property("text", "text", lenta, SENSOR, true, true, "string", "#");

    lenta->AddProperty(lenta_status);
    lenta->AddProperty(lenta_mode);
    lenta->AddProperty(lenta_color);
    lenta->AddProperty(lenta_brightness);
    lenta->AddProperty(lenta_text);
    device.AddNode(lenta);

    /* -------------------- End init your nodes and properties --------------------*/

    homie.SetDevice(&device);
    device.Init();

    WifiAp wifiAP;
    if (ssid_name == "Wifi_Name" || ssid_name == "") {
        wifiAP.Start(device_name);
        web_server.Init();
    }
    while (ssid_name == "Wifi_Name" || ssid_name == "") {
        // Handling buttons and offline logic
        device.HandleCurrentState();
        wifiAP.Blink();
    }
    wifi_client.SetCredentials(ssid_name, ssid_password);
    while (!wifi_client.Connect()) {
        // Handling buttons and offline logic
        device.HandleCurrentState();
        if (erase_flag) {
            EraseFlash();
        }
    }
    time_client->Init();
    web_server.Init();

    ip_addr = WiFi.localIP().toString();
    Serial.print("IP: ");
    Serial.println(ip_addr);
    dev_ip->SetValue(ip_addr);

    while (!homie.Init(person_id, host, broker_port, token, HandleMessage)) {
        device.HandleCurrentState();
    }

    // ---------------------------------------------- Homie convention end
}

void loop() {
    wifi_client.Connect();

    homie.HandleCurrentState();  // mqttLoop();

    if (erase_flag) {
        EraseFlash();
    }
}

void HandleMessage(char *topic, byte *payload, unsigned int length) {
    Serial.println("mess hendled");
    homie.HandleMessage(String(topic), payload, length);
}
