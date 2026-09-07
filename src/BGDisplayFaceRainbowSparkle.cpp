#include "BGDisplayFaceRainbowSparkle.h"
#include "BGSourceManager.h"
#include "globals.h"

static const uint8_t PROGMEM SPARKLE_HAPPY_A[8] = {
    0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C
};
static const uint8_t PROGMEM SPARKLE_HAPPY_B[8] = {
    0x3C, 0x42, 0xA5, 0x81, 0x81, 0xBD, 0x42, 0x3C
};
static const uint8_t PROGMEM SPARKLE_SHOCKED[8] = {
    0x3C, 0x42, 0xA5, 0xA5, 0x81, 0x99, 0x99, 0x3C
};
static const uint8_t PROGMEM SPARKLE_DIZZY[8] = {
    0x3C, 0x42, 0x99, 0x24, 0x81, 0x5A, 0x42, 0x3C
};

BGDisplayFaceRainbowSparkle::BGDisplayFaceRainbowSparkle(DisplayManager& displayManager)
    : BGDisplayFace(displayManager) {
}

void BGDisplayFaceRainbowSparkle::update() {
    uint32_t now = millis();
    if (now - _lastTick > 300) {
        _frame = !_frame;
        _hueOffset += 14;
        _lastTick = now;
    }
}

void BGDisplayFaceRainbowSparkle::drawRainbowText(int16_t startX, int16_t startY, const String& text) {
    int curX = startX;
    for (size_t i = 0; i < text.length(); i++) {
        uint8_t charHue = _hueOffset + (i * 32);
        _displayManager.drawChar(curX, startY, text[i], CHSV(charHue, 220, 255));
        curX += 4;
    }
}

void BGDisplayFaceRainbowSparkle::render() {
    _displayManager.clear();
    BGReading r = bgSourceManager.getLatestReading();
    String valStr = String(r.value);

    bool isLow = (r.value < 70 && r.value > 0);
    bool isHigh = (r.value > 180);
    bool isFallingFast = (r.delta <= -3 || r.trend == TREND_DOUBLE_DOWN || r.trend == TREND_SINGLE_DOWN);

    if (isFallingFast) {
        _displayManager.drawBitmap(0, 0, SPARKLE_SHOCKED, 8, 8, CRGB::Gold);
    } else if (isHigh) {
        _displayManager.drawBitmap(0, 0, SPARKLE_DIZZY, 8, 8, CRGB::Magenta);
    } else if (isLow) {
        _displayManager.drawBitmap(0, 0, SPARKLE_SHOCKED, 8, 8, CRGB::Red);
    } else {
        _displayManager.drawBitmap(0, 0, _frame ? SPARKLE_HAPPY_A : SPARKLE_HAPPY_B, 8, 8, CRGB::LimeGreen);
    }
    drawRainbowText(10, 1, valStr);
    _displayManager.drawTrendArrow(25, 1, r.trend, CHSV(_hueOffset + 120, 240, 255));
}
