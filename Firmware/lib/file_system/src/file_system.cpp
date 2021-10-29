#include "file_system.h"

bool InitFiles() {
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return false;
    }

    File root = SPIFFS.open("/");
    File file = root.openNextFile();

    while (file) {
        file = root.openNextFile();
    }
    return true;
}

bool LoadConfig() {
    if (ssid_name != "Wifi_Name" && ssid_name != "") {
        return false;
    }

    UserData user_data = {"", "", "", "", "", "", "", "", "", ""};
    ReadSettings("/config.txt", reinterpret_cast<byte *>(&user_data), sizeof(user_data));
    if (strlen(user_data.ssid_name) == 0) {
        EraseFlash();
    }

    ssid_name = user_data.ssid_name;
    ssid_password = user_data.ssid_password;
    person_mail = user_data.person_mail;
    person_id = user_data.person_id;
    token = user_data.token;
    host = user_data.host;
    broker_port = user_data.broker_port;
    device_id = user_data.device_id;
    product_id = user_data.product_id;
    web_auth_password = user_data.web_auth_password;

    return true;
}

bool SaveConfig() {
    UserData user_data = {"", "", "", "", "", "", "", "", "", ""};
    ssid_name.toCharArray(user_data.ssid_name, ssid_name.length() + 1);
    ssid_password.toCharArray(user_data.ssid_password, ssid_password.length() + 1);
    person_mail.toCharArray(user_data.person_mail, person_mail.length() + 1);
    person_id.toCharArray(user_data.person_id, person_id.length() + 1);
    token.toCharArray(user_data.token, token.length() + 1);
    host.toCharArray(user_data.host, host.length() + 1);
    broker_port.toCharArray(user_data.broker_port, broker_port.length() + 1);
    device_id.toCharArray(user_data.device_id, device_id.length() + 1);
    product_id.toCharArray(user_data.product_id, product_id.length() + 1);
    web_auth_password.toCharArray(user_data.web_auth_password, web_auth_password.length() + 1);

    return WriteSettings("/config.txt", reinterpret_cast<byte *>(&user_data), sizeof(user_data));
}

bool EraseFlash() {
    Serial.println("Create default config file");
    UserData user_data = {"Wifi_Name", "", "", "", "", "", "", "", "", "admin"};
    product_id.toCharArray(user_data.product_id, product_id.length() + 1);

    if (WriteSettings("/config.txt", reinterpret_cast<byte *>(&user_data), sizeof(user_data))) ESP.restart();
    return false;
}

bool WriteSettings(const char *file_path, uint8_t *data, size_t size_of_data) {
    File configFile = SPIFFS.open(file_path, FILE_WRITE);
    if (!configFile) {
        Serial.printf("Failed to open %s for writing\r\n", file_path);
        return false;
    }
    configFile.write(data, size_of_data);
    configFile.close();
    Serial.printf("File %s saved\r\n", file_path);
    return true;
}

bool ReadSettings(const char *file_path, uint8_t *data, size_t size_of_data) {
    File configFile = SPIFFS.open(file_path, FILE_READ);
    if (!configFile) {
        Serial.printf("Failed to open %s for reading\r\n", file_path);
        return false;
    }
    size_t size = configFile.size();
    if (size > 1024) {
        Serial.printf("file %s is too large \r\n", file_path);
        return false;
    }

    configFile.read(data, size_of_data);
    configFile.close();

    return true;
}
