#include "lenta.h"

#define FOR_i(x, y) for (int i = (x); i < (y); i++)

#include "file_system/src/file_system.h"
#include "fonts.h"
#include "timerMillis.h"

timerMillis effTmr(100, true);

Lenta::Lenta(const char* name, const char* id, Device* device) : Node(name, id, device) {
    if (!ReadSettings("/lentaconf.txt", reinterpret_cast<byte*>(&ls), sizeof(ls))) {
        ls.brightness_ = kDefaultBrigthness_;
        ls.state_ = true;
        ls.mode_ = FIRE;
        ls.red_ = kDefaultColorR_;
        ls.green_ = kDefaultColorG_;
        ls.blue_ = kDefaultColorB_;
        ls.quantity_ = kDefaultLedsQuantity_;
        kDefaultText_.toCharArray(ls.text_, kDefaultText_.length() + 1);
    }

    leds_ptr_ = new CRGB[ls.quantity_];
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds_ptr_, ls.quantity_).setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, kMax_ma_);
    new_ls_state_ = NO_CHANGES;
    FastLED.setBrightness(ls.brightness_ * 2);
    if (ls.mode_ == COLOR) new_ls_state_ = NEW_MODE;
    button_.resetState();

    random16_set_seed(millis());
}

bool Lenta::Init(Homie* homie) {  // initialize toggles for notification
    bool status = true;
    if (!Node::Init(homie)) status = false;

    LoadLentaSettings();
    HandleCurrentState();

    return status;
}

String validString(String value) {
    const char* text = value.c_str();
    String result = "";
    uint8_t i = 0;
    while (text[i] != '\0') {
        if (i == TEXT_MAX_LENGTH - 1) {  // max text length
            break;
        }

        if ((uint8_t)text[i] < 127) {  // supported ASCII symbols
            result += text[i];
        }

        i++;
    }

    return String(result);
}

void Lenta::HandleCurrentState() {
    button_.tick();
    if (button_.isPress()) {
        if (++ls.mode_ > modes_.size() - 1) ls.mode_ = 0;

        String state_in_string = modes_.find(ls.mode_)->second;
        properties_.find("mode")->second->SetValue(state_in_string);
        new_ls_state_ = NEW_MODE;
    }
    if (button_.isHolded()) {
        ls.state_ = !ls.state_;
        String state_in_string = ls.state_ ? "true" : "false";
        properties_.find("state")->second->SetValue(state_in_string);
        new_ls_state_ = NEW_MODE;
    }

    if (properties_.find("state")->second->HasNewValue()) {
        ls.state_ = properties_.find("state")->second->GetValue() == "true";
        new_ls_state_ = NEW_MODE;
        properties_.find("state")->second->SetHasNewValue(false);
    }
    if (properties_.find("mode")->second->HasNewValue()) {
        String mode_buffer = properties_.find("mode")->second->GetValue();
        for (auto it = modes_.begin(); it != modes_.end(); ++it) {
            if (it->second == mode_buffer) {
                ls.mode_ = it->first;
            }
        }
        new_ls_state_ = NEW_MODE;
        properties_.find("mode")->second->SetHasNewValue(false);
        loading = true;
    }
    if (properties_.find("color")->second->HasNewValue()) {
        ExtractColor(properties_.find("color")->second->GetValue());

        new_ls_state_ = NEW_COLOR;
        properties_.find("color")->second->SetHasNewValue(false);
    }

    if (properties_.find("brightness")->second->HasNewValue()) {
        ls.brightness_ = properties_.find("brightness")->second->GetValue().toInt();
        new_ls_state_ = NEW_BRIGHTNESS;
        properties_.find("brightness")->second->SetHasNewValue(false);
    }

    if (properties_.find("text")->second->HasNewValue()) {
        String text_buffer_ = validString(properties_.find("text")->second->GetValue());
        text_buffer_.toCharArray(ls.text_, text_buffer_.length() + 1);

        properties_.find("text")->second->SetValue(ls.text_);  // update value after validation
        properties_.find("text")->second->SetHasNewValue(false);
        new_ls_state_ = NEW_TEXT;
    }

    switch (new_ls_state_) {
        case NEW_BRIGHTNESS:
            FastLED.setBrightness(ls.brightness_ * 2);
            FastLED.show();
            break;
        default:
            break;
    }

    if (ls.state_) {
        switch (ls.mode_) {
            case COLOR:
                if (new_ls_state_ == NO_CHANGES) break;
                ChangeColor();
                break;
            case RAINBOW:
                Rainbow();
                break;
            case DISCO:
                Disco();
                break;
            case FIRE:
                Fire(scale_, length_, RED_FIRE_MODE);
                break;
            case PARTS:
                Parts();
                break;
            case KONFETTI:
                Konfeti();
                break;
            case HAMELEON:
                Hameleon();
                break;
            case MATRIX:
                Matrix();
                break;
            case DNA:
                DNAroutine();
                break;
            case TEXT:
                TextRunning();
                break;
            case ICE_FIRE:
                Fire(scale_, length_, BLUE_FIRE_MODE);
                break;
            case FOREST_FIRE:
                Fire(scale_, length_, GREEN_FIRE_MODE);
                break;
        }
    } else if (new_ls_state_) {
        TurnOffLs();
    }
    if (new_ls_state_ != NO_CHANGES) {
        last_update_time_ = millis();
        new_ls_state_ = NO_CHANGES;
        new_data_for_save_ = true;
    }
    if (millis() - last_update_time_ > kSaveLentaSettingsTime_ && new_data_for_save_) {
        SaveLentaSettings();
        new_data_for_save_ = false;
    }

    button_.resetState();
}

void Lenta::TurnOffLs() {
    FastLED.clear();
    FastLED.show();
}

void Lenta::Rainbow() {
    if (leds_ptr_ == nullptr) return;
    for (int i = 0; i < ls.quantity_; i++) {
        leds_ptr_[i] = CHSV(counter_ + i * 2, 255, 255);  // CHSV(hue, saturation, value)
    }
    counter_++;  // 0 to 255 (byte data type)
    FastLED.show();
    delay(10);  // moving speed
}

void Lenta::Hameleon() {
    if (leds_ptr_ == nullptr) return;
    for (int i = 0; i < ls.quantity_; i++) {
        leds_ptr_[i] = CHSV(counter_, 255, 255);  // CHSV(hue, saturation, value)
    }
    counter_++;  // 0 to 255 (byte data type)
    FastLED.show();
    delay(10);  // moving speed
}

void Lenta::ExtractColor(String color_string) {
    ls.red_ = color_string.substring(0, color_string.indexOf(',')).toInt();
    ls.green_ = color_string.substring(color_string.indexOf(',') + 1, color_string.lastIndexOf(',')).toInt();
    ls.blue_ = color_string.substring(color_string.lastIndexOf(',') + 1).toInt();
}

void Lenta::ChangeColor() {
    Serial.println("in change color");
    for (int led = 0; led < ls.quantity_; led++) {
        leds_ptr_[led] = CRGB(ls.red_, ls.green_, ls.blue_);
        delay(1);
    }
    FastLED.show();
}

void Lenta::Disco() {
    uint8_t random_delay = random(5, 100);
    uint8_t random_bool = random8(20);
    for (int i = 0; i < ls.quantity_; i++) {
        leds_ptr_[i] = (random_bool < 5) ? CRGB::White : CRGB::Black;
    }
    LEDS.show();
    delay(random_delay);
}

String Lenta::GetModes() {
    String ModeFormatValue = "";
    for (auto it = modes_.begin(); it != modes_.end(); ++it) {
        ModeFormatValue += it->second;
        ModeFormatValue += ",";
    }
    return ModeFormatValue.substring(0, ModeFormatValue.length() - 1);
}

bool Lenta::LoadLentaSettings() {
    String state_in_string = ls.state_ ? "true" : "false";

    properties_.find("state")->second->SetValue(state_in_string);
    properties_.find("state")->second->SetHasNewValue(false);

    state_in_string = modes_.find(ls.mode_)->second;
    properties_.find("mode")->second->SetValue(state_in_string);
    properties_.find("mode")->second->SetHasNewValue(false);

    properties_.find("brightness")->second->SetValue(String(ls.brightness_));
    properties_.find("brightness")->second->SetHasNewValue(false);

    char message_buffer[12];  // length of RGB mess

    snprintf(message_buffer, sizeof(message_buffer), "%d,%d,%d", ls.red_, ls.green_, ls.blue_);
    properties_.find("color")->second->SetValue(message_buffer);
    properties_.find("color")->second->SetHasNewValue(false);

    properties_.find("text")->second->SetValue(String(ls.text_));
    properties_.find("text")->second->SetHasNewValue(false);

    return true;
}

void Lenta::PublishMode(uint8_t mode_num) {
    if (mode_num > modes_.size() - 1) return;
    ls.mode_ = mode_num;
    String state_in_string = modes_.find(ls.mode_)->second;
    properties_.find("mode")->second->SetValue(state_in_string);
}

bool Lenta::SaveLentaSettings() { return WriteSettings("/lentaconf.txt", reinterpret_cast<byte*>(&ls), sizeof(ls)); }

void Lenta::Parts() {
    if (!effTmr.isReady()) return;
    uint32_t nowWeekMs = random16();
    uint16_t speed = 100;

    FOR_i(0, kDefaultLedsQuantity_) leds_ptr_[i].fadeToBlackBy(70);
    {
        uint16_t rndVal = 0;
        byte amount = (getScale() >> 3) + 1;
        FOR_i(0, amount) {
            rndVal = rndVal * 2053 + 13849;  // random2053
            int homeX = inoise16(i * 100000000ul + (nowWeekMs << 3) * speed / 255);
            homeX = map(homeX, 15000, 50000, 0, length_);
            int offsX = inoise8(i * 2500 + (nowWeekMs >> 1) * speed / 255) - 128;
            offsX = length_ / 2 * offsX / 128;
            int thisX = homeX + offsX;

            int homeY = inoise16(i * 100000000ul + 2000000000ul + (nowWeekMs << 3) * speed / 255);
            homeY = map(homeY, 15000, 50000, 0, width_);
            int offsY = inoise8(i * 2500 + 30000 + (nowWeekMs >> 1) * speed / 255) - 128;
            offsY = length_ / 2 * offsY / 128;
            int thisY = homeY + offsY;
            setPix(thisX, thisY, CRGB(ls.red_, ls.green_, ls.blue_));
        }
    }
    FastLED.show();
}

void Lenta::Konfeti() {
    if (!effTmr.isReady()) return;
    uint16_t speed = 100;

    byte amount = (getScale() >> 3) + 1;
    FOR_i(0, amount) {
        int x = random(0, kDefaultLedsQuantity_);
        if (leds_ptr_[x] == CRGB(0, 0, 0)) leds_ptr_[x] = CHSV(random8(255), 255, 255);
    }
    FOR_i(0, kDefaultLedsQuantity_) {
        if (leds_ptr_[i].r >= 10 || leds_ptr_[i].g >= 10 || leds_ptr_[i].b >= 10)
            leds_ptr_[i].fadeToBlackBy(speed / 2 + 1);
        else
            leds_ptr_[i] = 0;
    }
    FastLED.show();
}

byte Lenta::getScale() { return scale_; }

void Lenta::setPix(int x, int y, CRGB color) {
    if (y >= 0 && y < length_ && x >= 0 && x < width_) leds_ptr_[getPix(x, y)] = color;
}

void Lenta::setLED(int x, CRGB color) {
    if (x >= 0 && x < length_) leds_ptr_[x] = color;
}

// get pixel by coords
uint16_t Lenta::getPix(int x, int y) {
    int matrixW;
    matrixW = width_;
    int thisX, thisY;

    thisX = x;
    thisY = y;

    if (!(thisY & 1))
        return (thisY * matrixW + thisX);  // even line
    else
        return (thisY * matrixW + matrixW - thisX - 1);  // odd line
}

uint32_t Lenta::getPixColor(int x, int y) {
    int thisPix = getPix(x, y);
    if (thisPix < 0 || thisPix >= kDefaultLedsQuantity_) return 0;
    return (((uint32_t)leds_ptr_[thisPix].r << 16) | ((int64_t)leds_ptr_[thisPix].g << 8) |
            (int64_t)leds_ptr_[thisPix].b);
}

CRGB GetFirePalette(int mode, uint8_t index, uint8_t brightness) {
    switch (mode) {
        case BLUE_FIRE_MODE:
            return ColorFromPalette(BlueFire_p, index, brightness);
        case GREEN_FIRE_MODE:
            return ColorFromPalette(GreenFire_p, index, brightness);
        default:
            return ColorFromPalette(RedFire_p, index, brightness);
    }
}

void Lenta::Fire(byte scale, int len, int mode) {
    if (!effTmr.isReady()) return;

    static uint8_t deltaValue;
    static uint8_t deltaHue;
    static uint8_t step;
    static uint8_t shiftHue[50];
    static float trackingObjectPosX[100];
    static float trackingObjectPosY[100];
    static uint16_t ff_y, ff_z;

    if (loading) {
        loading = false;
        deltaValue = map(scale, 0, 255, 8, 168);
        deltaHue =
            map(deltaValue, 8U, 168U, 8U, 84U);  // the height of the flames should not decrease as quickly as the width
        step = map(255U - deltaValue, 87U, 247U, 4U, 32U);  // probability of displacement of the spark along the x-axis
        for (uint8_t j = 0; j < length_; j++) {
            shiftHue[j] = (length_ - 1 - j) * 255 / (length_ - 1);  // init colorfade table
        }

        for (uint8_t i = 0; i < width_ / 8; i++) {
            trackingObjectPosY[i] = random8(length_);
            trackingObjectPosX[i] = random8(width_);
        }
    }

    for (uint16_t i = 0; i < width_; i++) {
        for (uint16_t j = 0; j < len; j++) {
            leds_ptr_[getPix(i, len - 1U - j)] = GetFirePalette(
                mode,
                qsub8(inoise8(i * deltaValue, (j + ff_y + random8(2)) * deltaHue, ff_z), shiftHue[j]),
                255U);
        }
    }

    // insert sparks from a separate array
    for (uint8_t i = 0; i < width_ / 8; i++) {
        if (trackingObjectPosY[i] > 3U) {
            leds_ptr_[getPix(trackingObjectPosX[i], trackingObjectPosY[i])] =
                leds_ptr_[getPix(trackingObjectPosX[i], 3U)];
            leds_ptr_[getPix(trackingObjectPosX[i], trackingObjectPosY[i])].fadeToBlackBy(trackingObjectPosY[i] * 2U);
        }
        trackingObjectPosY[i]++;
        if (trackingObjectPosY[i] >= len) {
            trackingObjectPosY[i] = random8(4U);
            trackingObjectPosX[i] = random8(width_);
        }
        if (!random8(step))
            trackingObjectPosX[i] = (width_ + (uint8_t)trackingObjectPosX[i] + 1U - random8(3U)) % width_;
    }
    ff_y++;
    if (ff_y & 0x01) ff_z++;
    LEDS.show();
}

#define COLOR1 0x00aa00  // max brightness   170
#define COLOR2 0x00ff00  // initial color    255
#define COLOR3 0x006e00  // the pixel is almost gone 110
#define COLOR4 0x003c00  // the pixel is almost gone 60
#define COLOR5 0x003200  // decay slower 50
#define COLOR6 0x00a000  // drastically reduce brightness 160

void Lenta::Matrix() {  // ------------- Matrix ---------------
    if (!effTmr.isReady()) return;

    const uint8_t density = 20;
    for (uint8_t x = 0U; x < length_; x++) {
        for (uint8_t y = 0U; y < width_; y++) {
            uint32_t thisColor = getPixColor(x, y);        // pixel color
            uint32_t upperColor = getPixColor(x, y + 1U);  // pixel color above
            if (upperColor >= COLOR1 && random(7 * width_) != 0U) {
                setPix(x, y, upperColor);
            } else if (thisColor == 0U &&
                       random(density) == 0U) {
                setPix(x, y, COLOR2);
            } else if (thisColor <= COLOR3) {
                if (thisColor >= COLOR4) {
                    setPix(x, y, COLOR5);
                } else {
                    if (thisColor != 0U) setPix(x, y, 0U);
                }
            } else {
                if (thisColor >= COLOR1)
                    setPix(x, y, 0x00a100);
                else
                    setPix(x, y, thisColor - 0x002800);
            }
        }
    }
    LEDS.show();
}

void Lenta::DNAroutine() {
    uint8_t step = 15;

    if (loading) {
        loading = false;
        deltaHue = hue > 50U;
        if (deltaHue) hue = 101U - hue;
        hue = 255U - map(51U - hue, 1U, 50U, 0, 255U);
    }

    double freq = 3000;
    float mn = 255.0 / 13.8;

    fadeToBlackBy(leds_ptr_, kDefaultLedsQuantity_, step * 2);
    uint16_t ms = millis();

    if (deltaHue) {
        for (uint8_t i = 0; i < width_; i++) {
            uint32_t x = beatsin16(step, 0, (length_ - 1) * 256, 0, i * freq);
            uint32_t y = i * 256;

            CRGB col = CHSV(ms / 29 + i * 255 / (width_ - 1), 255, qadd8(hue, beatsin8(step, 60, 255U, 0, i * mn)));

            WuPixel(y, x, &col);
        }
    } else {
        for (uint8_t i = 0; i < length_; i++) {
            uint32_t x = beatsin16(step, 0, (width_ - 1) * 256, 0, i * freq);
            uint32_t y = i * 256;

            CRGB col = CHSV(ms / 29 + i * 255 / (length_ - 1), 255, qadd8(hue, beatsin8(step, 60, 255U, 0, i * mn)));
            WuPixel(x, y, &col);
        }
    }

    blur2d(leds_ptr_, width_, length_, 16);
    LEDS.show();
}

void Lenta::WuPixel(uint32_t x, uint32_t y, CRGB* col) {  // awesome WuPixel procedure by reddit u/sutaburosu
    // extract the fractional parts and derive their inverses
    uint8_t xx = x & 0xff, yy = y & 0xff, ix = 255 - xx, iy = 255 - yy;
// calculate the intensities for each affected pixel
#define WU_WEIGHT(a, b) ((uint8_t)(((a) * (b) + (a) + (b)) >> 8))
    uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy), WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
    // multiply the intensities by the colour, and saturating-add them to the pixels
    for (uint8_t i = 0; i < 4; i++) {
        uint16_t xy = (x >> 8) + (i & 1) + (((y >> 8) + ((i >> 1) & 1)) * 16);
        if (xy < kDefaultLedsQuantity_) {
            leds_ptr_[xy].r = qadd8(leds_ptr_[xy].r, col->r * wu[i] >> 8);
            leds_ptr_[xy].g = qadd8(leds_ptr_[xy].g, col->g * wu[i] >> 8);
            leds_ptr_[xy].b = qadd8(leds_ptr_[xy].b, col->b * wu[i] >> 8);
        }
    }
}

uint16_t XY(uint8_t x, uint8_t y) { return (y * 16 + x); }

void Lenta::TextRunning() {
    fillString(ls.text_, CRGB(ls.red_, ls.green_, ls.blue_), true);
}

#define TEXT_DIRECTION (1U)  // 1 - horizontal, 0 - vertically
#define MIRR_V (0U)          // display text vertically (0 / 1)
#define MIRR_H (0U)          // display text horizontally (0 / 1)

#define TEXT_HEIGHT (4U)  // pixel offset from bottom
#define LET_WIDTH (5U)    // symbol width (contstant = 5)
#define LET_HEIGHT (7U)   // symbol height (contstant = 7)
#define SPACE (1U)        // indent between symbols
#define LETTER_COLOR (CRGB::White)  // initial color

bool Lenta::fillString(const char* text, CRGB letterColor, boolean itsText) {
    if (!text || !strlen(text)) {
        FastLED.clear();
        FastLED.show();
        return true;
    }
    if (loading && !itsText) {
        offset = width_;  // start from rigth side
        loading = false;
    }

    if (millis() - scrollTimer >= 300 /* modes[EFF_TEXT].Speed */) {
        scrollTimer = millis();
        FastLED.clear();
        uint8_t i = 0, j = 0;
        while (text[i] != '\0') {
            if ((uint8_t)text[i] > 191) {  // cyrillic symbols
                i++;
            } else {
                drawLetter(text[i], offset + j * (LET_WIDTH + SPACE), letterColor);
                i++;
                j++;
            }
        }

        offset--;
        if (offset < (int16_t)(-j * (LET_WIDTH + SPACE))) {  // end of string
            offset = width_ + 3;
            return true;
        }
        FastLED.show();
    }

    return false;
}

void Lenta::drawLetter(uint8_t letter, int8_t offset, CRGB letterColor) {
    uint8_t start_pos = 0, finish_pos = LET_WIDTH;

    if (offset < (int8_t)-LET_WIDTH || offset > (int8_t)width_) {
        return;
    }
    if (offset < 0) {
        start_pos = (uint8_t)-offset;
    }
    if (offset > (int8_t)(width_ - LET_WIDTH)) {
        finish_pos = (uint8_t)(width_ - offset);
    }
    for (uint8_t i = start_pos; i < finish_pos; i++) {
        uint8_t thisByte;
        if (MIRR_V) {
            thisByte = getFont(letter, (uint8_t)(LET_WIDTH - 1 - i));
        } else {
            thisByte = getFont(letter, i);
        }

        for (uint8_t j = 0; j < LET_HEIGHT; j++) {
            bool thisBit = MIRR_H ? thisByte & (1 << j) : thisByte & (1 << (LET_HEIGHT - 1 - j));

            // draw colomn (i - horizontal, j - vertical)
            if (TEXT_DIRECTION) {
                if (thisBit) {
                    leds_ptr_[getPix(offset + i, TEXT_HEIGHT + j)] = letterColor;
                } else {
                    setPix(offset + i, TEXT_HEIGHT + j, 0x000000);
                }
            } else {
                if (thisBit) {
                    leds_ptr_[getPix(i, offset + TEXT_HEIGHT + j)] = letterColor;
                } else {
                    setPix(i, offset + TEXT_HEIGHT + j, 0x000000);
                }
            }
        }
    }
}

uint8_t Lenta::getFont(uint8_t asciiCode,
                       uint8_t row) {
    asciiCode = asciiCode - '0' + 16;

    if (asciiCode <= 90) {
        return pgm_read_byte(&fontHEX[asciiCode][row]);
    } else if (asciiCode >= 112 && asciiCode <= 159) {
        return pgm_read_byte(&fontHEX[asciiCode - 17][row]);
    } else if (asciiCode >= 96 && asciiCode <= 111) {
        return pgm_read_byte(&fontHEX[asciiCode + 47][row]);
    }

    return 0;
}
