#pragma once

#include <Arduino.h>

#include <map>
#include <string>

#include "node/node.h"
#include "ntp_time_client/ntp_time_client.h"

class AutoUpdateFw : public Node {
 public:
    AutoUpdateFw(const char* name, const char* id, Device* device);

    bool Init(Homie* homie);

    void SetTimeClient(TimeClient* time_client);

    void HandleCurrentState();

 private:
    enum fwStates { FW_CHECK, FW_ACTUAL, FW_UPDATING, FW_UPDATED, FW_FAILED };

    typedef struct {
        bool autoUpdate_;
        uint8_t version_;
        uint8_t hours_;
        uint8_t minutes_;
    } FwSettings;

    const uint16_t kLoopDelay_ = 10 * 1000;      // sec
    const uint16_t kFirmwareDelay_ = 50 * 1000;  // sec || max 65k, then change to uint32_t

    const char* kUrlFirmwareVersionPath_ = "/firmwares/v1/products/";  // then ad "product_id" and "firmware-version"
    const char* kUrlFirmwareFilePath_ = "/firmwares/v1/";              // then ad "product_id".bin

    TimeClient* time_client_ = nullptr;

    void CheckFirmware(uint8_t firmware_ver);
    uint8_t CheckFirmwareVersion();
    void UpdateFirmware();

    bool LoadFwSettings();
    bool SaveFwSettings();

    void SetUpdateTime(String time);

    bool force_update_ = false;
    bool new_fw_settings_ = false;
    bool fw_updated_ = false;

    uint32_t period_loop = millis();
    uint32_t last_millis_firmware_ = 0;

    std::map<uint8_t, std::string> fwStates_ = {{FW_CHECK, "Checking for updates"},
                                                {FW_ACTUAL, "Firmware is up to date"},
                                                {FW_UPDATING, "Updating"},
                                                {FW_UPDATED, "Updated successfully"},
                                                {FW_FAILED, "Update failed"}};

    FwSettings fw_settings = {1, 0, 0, 0};
};
