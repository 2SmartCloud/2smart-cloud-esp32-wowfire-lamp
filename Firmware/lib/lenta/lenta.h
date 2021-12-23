#pragma once

#include <Arduino.h>
#include <EncButton.h>
#include <FastLED.h>

#include <map>
#include <string>

#include "homie.h"

#define DATA_PIN 18

class Lenta : public Node {
 public:
    Lenta(const char* name, const char* id, Device* device);

    bool Init(Homie* homie);

    void HandleCurrentState();

    String GetModes();

    void PublishMode(uint8_t mode_num);

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
    enum LedStripStates { RAINBOW, COLOR, DISCO, FIRE, PARTS, KONFETTI, HAMELEON, MATRIX, DNA };

    void TurnOffLs();
    void Rainbow();
    void Disco();
    void ChangeColor();
    void Parts();
    void Fire(byte scale, int len);
    void Konfeti();
    void Hameleon();
    void Matrix();
    void DNAroutine();

    void ExtractColor(String color_string);

    bool SaveLentaSettings();
    bool LoadLentaSettings();

    const uint16_t kMax_ma_ = 1300;  // max mA, for library

    const uint8_t length_ = 16;
    const uint8_t width_ = 16;

    const uint8_t kDefaultBrigthness_ = 10;
    const uint8_t kDefaultColorR_ = 50;
    const uint8_t kDefaultColorG_ = 200;
    const uint8_t kDefaultColorB_ = 200;
    const uint16_t kDefaultLedsQuantity_ = length_ * width_;
    const uint16_t kSaveLentaSettingsTime_ = 5 * 1000;  // 5s

    byte scale_ = 100;  // масштаб (0.. 255)
    uint8_t hue = 10;
    uint8_t deltaHue = 0;

    bool new_data_for_save_ = false;

    bool loading = true;

    CRGB* leds_ptr_ = nullptr;

    uint8_t new_ls_state_ = 0;
    uint8_t counter_ = 0;
    uint32_t last_update_time_ = 0;

    std::map<uint8_t, String> modes_ = {{RAINBOW, "rainbow"},   {COLOR, "color"},   {DISCO, "disco"},
                                        {FIRE, "fire"},         {PARTS, "parts"},   {KONFETTI, "konfetti"},
                                        {HAMELEON, "hameleon"}, {MATRIX, "matrix"}, {DNA, "DNA"}};

    EncButton<EB_TICK, 19> button_;

    LsSettings ls = {
        true, FIRE, kDefaultBrigthness_, kDefaultColorR_, kDefaultColorG_, kDefaultColorB_, kDefaultLedsQuantity_};

    // ====================================================================================================================

    int getLength();

    byte getScale();

    void WuPixel(uint32_t x, uint32_t y, CRGB* col);

    void setPix(int x, int y, CRGB color);
    void setLED(int x, CRGB color);

    // получить номер пикселя в ленте по координатам
    uint16_t getPix(int x, int y);
    uint32_t getPixColor(int x, int y);
};
