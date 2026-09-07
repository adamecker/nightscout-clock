#include "BGDisplayFaceRainbowSparkle.h"
#include "BGDisplayManager.h"
#include "globals.h"

namespace {
const uint8_t PROGMEM SPARKLE_HAPPY_A[] = {
    0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10100101,
    0b10011001,
    0b01000010,
    0b00111100
};

const uint8_t PROGMEM SPARKLE_HAPPY_B[] = {
    0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10000001,
    0b10111101,
    0b01000010,
    0b00111100
};

const uint8_t PROGMEM SPARKLE_SHOCKED[] = {
    0b00111100,
    0b01000010,
    0b10100101,
    0b10100101,
    0b10000001,
    0b10011001,
    0b10011001,
    0b00111100
};

const uint8_t PROGMEM SPARKLE_DIZZY[] = {
    0b00111100,
    0b01000010,
    0b10011001,
    0b00100100,
    0b10000001,
    0b01011010,
    0b01000010,
    0b00111100
};

uint16_t hsvToRgb565(uint8_t hue) {
    uint8_t region = hue / 43;
    uint8_t remainder = (hue - (region * 43)) * 6;
    uint8_t q = 255 - remainder;
    uint8_t t = remainder;
    uint8_t r = 0, g = 0, b = 0;
    switch (region) {
        case 0: r = 255; g = t;   b = 0;   break;
        case 1: r = q;   g = 255; b = 0;   break;
        case 2: r = 0;   g = 255; b = t;   break;
        case 3: r = 0;   g = q;   b = 255; break;
        case 4: r = t;   g = 0;   b = 255; break;
        default: r = 255; g = 0;   b = q;   break;
    }
    return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
}
} // namespace

void BGDisplayFaceRainbowSparkle::showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    auto lastReading = readings.back();
    showAnimatedReading(lastReading, dataIsOld);
    showTrendArrow(lastReading, MATRIX_WIDTH - 5, 1, dataIsOld, false, false);
    DisplayManager.update();
}

bool BGDisplayFaceRainbowSparkle::needsFrequentRefresh() const {
    return true;
}

unsigned long BGDisplayFaceRainbowSparkle::getFrequentRefreshIntervalMs() const {
    return 60;
}

void BGDisplayFaceRainbowSparkle::showAnimatedReading(const GlucoseReading& reading, bool dataIsOld) const {
    DisplayManager.clearMatrix(false);

    auto bgLevel = bgDisplayManager.getGlucoseIntervals().getBGLevel(reading.sgv);
    bool isFallingFast = (reading.trend == BG_TREND::DOUBLE_DOWN || reading.trend == BG_TREND::SINGLE_DOWN);

    const uint8_t* faceBmp = (millis() / 300 % 2 == 0) ? SPARKLE_HAPPY_A : SPARKLE_HAPPY_B;
    uint16_t faceColor = COLOR_GREEN;

    if (isFallingFast) {
        faceBmp = SPARKLE_SHOCKED;
        faceColor = COLOR_YELLOW;
    } else if (bgLevel == BG_LEVEL::URGENT_HIGH || bgLevel == BG_LEVEL::WARNING_HIGH) {
        faceBmp = SPARKLE_DIZZY;
        faceColor = 0xF81F;
    } else if (bgLevel == BG_LEVEL::URGENT_LOW || bgLevel == BG_LEVEL::WARNING_LOW) {
        faceBmp = SPARKLE_SHOCKED;
        faceColor = COLOR_RED;
    }

    if (dataIsOld) {
        faceColor = COLOR_GRAY;
    }

    DisplayManager.drawBitmap(0, 0, faceBmp, 8, 8, faceColor, false);

    String readingToDisplay = getPrintableReading(reading.sgv);
    uint8_t hueOffset = (millis() / 25) % 255;
    uint8_t textLen = readingToDisplay.length();
    int16_t x = 9;

    DisplayManager.setFont(FONT_TYPE::MEDIUM);
    for (uint8_t i = 0; i < textLen; i++) {
        char buf[2] = {readingToDisplay[i], '\0'};
        uint8_t charHue = hueOffset + (i * 35);
        uint16_t color = dataIsOld ? (uint16_t)COLOR_GRAY : hsvToRgb565(charHue);
        DisplayManager.setTextColor(color);
        DisplayManager.printText(x, 6, buf, TEXT_ALIGNMENT::LEFT, 2, false);
        x += DisplayManager.getTextWidth(buf, 2);
    }
}
