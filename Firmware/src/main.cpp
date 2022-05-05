#include "main.h"

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

    /* -------------------- Start init your nodes and properties --------------------*/

    Lenta *lenta = new Lenta("Lenta", "lenta", &device);  // (name, id, device)
    Property *lenta_status = new Property("state", "state", lenta, SENSOR, true, true, "boolean");
    Property *lenta_mode = new Property("mode", "mode", lenta, SENSOR, true, true, "enum", lenta->GetModes());
    Property *lenta_color = new Property("color", "color", lenta, SENSOR, true, true, "color", "rgb");
    Property *lenta_brightness =
        new Property("brightness", "brightness", lenta, SENSOR, true, true, "integer", "0:100");
    Property *lenta_text = new Property("text", "text", lenta, SENSOR, true, true, "string", "#");

    /* -------------------- End init your nodes and properties --------------------*/

    // ---------------------------------------------- Homie convention init
    AutoUpdateFw *firmware = new AutoUpdateFw("Firmware", "firmware", &device);                   // (name, id,device)
    Notifications *notifications = new Notifications("Notifications", "notifications", &device);  // (name,id, device)
    RstButton *rstbutton = new RstButton("ResetButton", "rstbutton", &device);                   // (name, id,device)

    Property *update_status = new Property("update status", "updatestate", firmware, SENSOR, false, false, "string");
    Property *update_button = new Property("update button", "update", firmware, SENSOR, true, false, "boolean");

    UpdateTime *update_time = new UpdateTime("update time", "updatetime", firmware, SENSOR, true, true, "string");

    Property *auto_update = new Property("autoUpdate", "autoupdate", firmware, SENSOR, true, true, "boolean");
    Property *fw_version = new Property("version", "version", firmware, SENSOR, false, true, "integer");
    Property *staging_option = new Property("Staging", "staging", firmware, SENSOR, true, true, "boolean");
    Property *reset_button = new Property("Reset button", "resetbutton", rstbutton, SENSOR, true, false, "boolean");
    Property *reset_state = new Property("Reset state", "resetstate", rstbutton, SENSOR, false, true,
    "integer", "", "%");
    // ------------- notification`s properties
    Property *system_notification =
        new Property("System Notifications", "system", notifications, SENSOR, true, true, "boolean");
    Property *update_notification =
        new Property("Update Notifications", "update", notifications, SENSOR, true, true, "boolean");

    WifiSignal *wifisignal = new WifiSignal("WiFi Signal", WIFI_SIGNAL, &device,
    TELEMETRY, false, true, "integer");

    DeviceData device_data{device_name, device_version, product_id.c_str(), ip_addr.c_str(), "esp32",
                           mac.c_str(), "ready",        device_id.c_str()};
    notifier.SetUserHash(person_id);

    device.SetCredentials(device_data);
    device.SetNotifier(&notifier);

    Property *dev_ip = new Property("ipw", "ipw", &device, TELEMETRY, false, true, "string");

    firmware->SetTimeClient(time_client);

    homie.SetDevice(&device);

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

    while (!homie.Init(person_id, host, broker_port, token, HandleMessage)) {
        device.HandleCurrentState();
    }

    dev_ip->SetValue(ip_addr);
    // ---------------------------------------------- Homie convention ends
}

void loop() {
    wifi_client.Connect();

    if (wifi_client.isConnected()) {
        homie.HandleCurrentState();
    } else {  // standalone mode
        device.HandleCurrentState();
    }

    if (erase_flag) {
        EraseFlash();
    }
}

Device *GetDevice() { return &device; }

void HandleMessage(char *topic, byte *payload, unsigned int length) {
    homie.HandleMessage(String(topic), payload, length);
}
