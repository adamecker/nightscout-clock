#include "BGDisplayFaceSmileyPlusStats.h"
#include "BGSourceManager.h"
#include "globals.h"

static const uint8_t PROGMEM MINI_HAPPY[7] = {
    0x1C, 0x22, 0x49, 0x41, 0x49, 0x22, 0x1C
};
static const uint8_t PROGMEM MINI_NEUTRAL[7] = {
    0x1C, 0x22, 0x49, 0x41, 0x41, 0x22, 0x1C
};
static const uint8_t PROGMEM MINI_SAD[7] = {
    0x1C, 0x22, 0x49, 0x41, 0x55, 0x22, 0x1C
};

BGDisplayFaceSmileyPlusStats::BGDisplayFaceSmileyPlusStats(DisplayManager& displayManager)
    : BGDisplayFace(displayManager) {
}

void BGDisplayFaceSmileyPlusStats::update() {
    uint32_t now = millis();
    if (now - _lastBlink > 700) {
        _staleBlink = !_staleBlink;
        _lastBlink = now;
    }
}

void BGDisplayFaceSmileyPlusStats::render() {
    _displayManager.clear();
    BGReading r = bgSourceManager.getLatestReading();
    uint32_t ageMin = (r.timestampMs > 0) ? (millis() - r.timestampMs) / 60000 : 0;

    bool isLow = (r.value < 70 && r.value > 0);
    bool isHigh = (r.value > 180);
    bool isFallingFast = (r.delta <= -3 || r.trend == TREND_DOUBLE_DOWN || r.trend == TREND_SINGLE_DOWN);
    bool headingToTarget = (r.value > 180 && r.delta < 0) || (r.value < 70 && r.delta > 0) || (!isLow && !isHigh);

    CRGB statusColor = CRGB::Green;
    const uint8_t* moodBmp = MINI_HAPPY;

    if (isLow) {
        statusColor = CRGB::Red;
        moodBmp = MINI_SAD;
    } else if (isHigh) {
        statusColor = CRGB::Gold;
        moodBmp = MINI_NEUTRAL;
    } else if (isFallingFast) {
        statusColor = CRGB::OrangeRed;
        moodBmp = MINI_NEUTRAL;
    }

    _displayManager.drawBitmap(0, 0, moodBmp, 7, 7, statusColor);

    CRGB numColor = (ageMin >= 15) ? CRGB(70, 70, 70) : CRGB::White;
    _displayManager.drawString(8, 1, String(r.value), numColor);

    String deltaStr = (r.delta >= 0 ? "+" : "") + String(r.delta);
    CRGB deltaColor = headingToTarget ? CRGB::Green : CRGB::Orange;
    _displayManager.drawString(20, 1, deltaStr, deltaColor);

    _displayManager.drawTrendArrow(28, 1, r.trend, statusColor);

    if (ageMin >= 10 && _staleBlink) {
        _displayManager.drawPixel(31, 7, CRGB::Red);
    }
}
