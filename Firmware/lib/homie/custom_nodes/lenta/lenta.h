#pragma once

#include <Arduino.h>
#include <EncButton.h>
#include <FastLED.h>

#include <map>
#include <string>

#include "node/node.h"

#define DATA_PIN 18

class Lenta : public Node {
 public:
    Lenta(const char* name, const char* id, Device* device);

    bool Init(Homie* homie);

    void HandleCurrentState();

    String GetModes();

 private:
    typedef struct {
        bool state_;    // on/off
        uint8_t mode_;  // rainbow/disco/color ...
        uint8_t brightness_;
        uint8_t red_;
        uint8_t green_;
        uint8_t blue_;
        uint16_t quantity_;
    } LsSettings;

    enum LsNewState { NO_CHANGES, NEW_COLOR, NEW_BRIGHTNESS, NEW_MODE };
    enum LedStripStates { RAINBOW, COLOR, DISCO, FIRE, PARTS, KONFETTI, HAMELEON };

    LsSettings ls = {1, 0, 0, 0, 0, 0, 1};

    void TurnOffLs();
    void Rainbow();
    void Disco();
    void ChangeColor();
    void Parts();
    void Fire(byte scale, int len);
    void Konfeti();
    void Hameleon();

    void ExtractColor(String color_string);

    bool SaveLentaSettings();
    bool LoadLentaSettings();

    const uint16_t kMax_ma_ = 1500;  // max mA, for library

    const uint8_t kDefaultBrigthness_ = 10;
    const uint8_t kDefaultColor_ = 80;
    const uint16_t kDefaultLedsQuantity_ = 300;         // 5m led strip
    const uint16_t kSaveLentaSettingsTime_ = 5 * 1000;  // 5s

    const uint8_t length_ = 16;
    const uint8_t width_ = 16;
    byte scale_ = 100;  // масштаб (0.. 255)

    bool new_data_for_save_ = false;

    bool loading = true;

    CRGB* leds_ptr_ = nullptr;

    uint8_t new_ls_state_ = 0;
    uint8_t counter_ = 0;
    uint32_t last_update_time_ = 0;

    std::map<uint8_t, String> modes_ = {{RAINBOW, "rainbow"},  {COLOR, "color"}, {DISCO, "disco"},
                                        {FIRE, "fire"},        {PARTS, "parts"}, {KONFETTI, "konfetti"},
                                        {HAMELEON, "hameleon"}};

    EncButton<EB_TICK, 13> button_;

    // ====================================================================================================================

    int getLength();

    byte getScale();

    void setPix(int x, int y, CRGB color);
    void setLED(int x, CRGB color);

    // получить номер пикселя в ленте по координатам
    uint16_t getPix(int x, int y);
};
