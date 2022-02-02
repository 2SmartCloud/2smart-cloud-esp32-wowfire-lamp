#pragma once

#include <Arduino.h>
#include <EncButton.h>
#include <FastLED.h>

#include <map>
#include <string>

#include "homie.h"

#define DATA_PIN 18
#define TEXT_MAX_LENGTH 50

#define RED_FIRE_MODE 0
#define BLUE_FIRE_MODE 1
#define GREEN_FIRE_MODE 2

const TProgmemRGBPalette16 RedFire_p FL_PROGMEM = {
    0x000000,
    0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000,
    0xFF3300, 0xFF6600, 0xFF9900, 0xFFCC00, 0xFFFF00,
    0xFFFF33, 0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF
};

const TProgmemRGBPalette16 BlueFire_p FL_PROGMEM = {
    0x000000,
    0x000A33, 0x001247, 0x001B6B, 0x00248E, 0x002DB2,
    0x0036D6, 0x255AF8, 0x2381FC, 0x00F3FF, 0x67F6FD,
    0x84FFF9, 0x9BFDF8, 0xAFF9F5, 0xD9FFFD, 0xFFFFFF
};

const TProgmemRGBPalette16 GreenFire_p FL_PROGMEM = {
    0x000000,
    0x011A03, 0x183A01, 0x264E0B, 0x25490A, 0x3B7912,
    0x459113, 0x57B517, 0x6BBF32, 0x7CC64A, 0x95D56B,
    0xAADF87, 0xC2F0A3, 0xCBF3B1, 0xDBF1CD, 0xFFFFFF
};

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
        char text_[TEXT_MAX_LENGTH];
    } LsSettings;

    enum LsNewState { NO_CHANGES, NEW_COLOR, NEW_BRIGHTNESS, NEW_MODE, NEW_TEXT };
    enum LedStripStates {
        RAINBOW,
        COLOR,
        DISCO,
        FIRE,
        PARTS,
        KONFETTI,
        HAMELEON,
        MATRIX,
        DNA,
        TEXT,
        ICE_FIRE,
        FOREST_FIRE
    };

    void TurnOffLs();
    void Rainbow();
    void Disco();
    void ChangeColor();
    void Parts();
    void Fire(byte scale, int len, int mode);
    void Konfeti();
    void Hameleon();
    void Matrix();
    void DNAroutine();
    void TextRunning();
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
    String kDefaultText_ = "2Smart";

    byte scale_ = 100;  // scale (0.. 255)
    uint8_t hue = 10;
    uint8_t deltaHue = 0;

    bool new_data_for_save_ = false;

    bool loading = true;

    CRGB* leds_ptr_ = nullptr;

    uint8_t new_ls_state_ = 0;
    uint8_t counter_ = 0;
    uint32_t last_update_time_ = 0;

    int16_t offset = width_;
    uint32_t scrollTimer = 0LL;

    std::map<uint8_t, String> modes_ = {
        {RAINBOW, "rainbow"},
        {COLOR, "color"},
        {DISCO, "disco"},
        {FIRE, "fire"},
        {PARTS, "parts"},
        {KONFETTI, "konfetti"},
        {HAMELEON, "hameleon"},
        {MATRIX, "matrix"},
        {DNA, "DNA"},
        {TEXT, "text"},
        {ICE_FIRE, "ice fire"},
        {FOREST_FIRE, "forest fire"}
    };

    EncButton<EB_TICK, 19> button_;

    LsSettings ls = {
        true, FIRE, kDefaultBrigthness_, kDefaultColorR_, kDefaultColorG_, kDefaultColorB_, kDefaultLedsQuantity_};

    // ====================================================================================================================

    bool fillString(const char* text, CRGB letterColor, boolean itsText);

    void drawLetter(uint8_t letter, int8_t offset, CRGB letterColor);
    uint8_t getFont(uint8_t asciiCode, uint8_t row);

    int getLength();

    byte getScale();

    void WuPixel(uint32_t x, uint32_t y, CRGB* col);

    void setPix(int x, int y, CRGB color);
    void setLED(int x, CRGB color);

    uint16_t getPix(int x, int y);
    uint32_t getPixColor(int x, int y);
};
