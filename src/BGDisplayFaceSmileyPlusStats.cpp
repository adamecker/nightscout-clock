#include "BGDisplayFaceSmileyPlusStats.h"
#include "BGDisplayManager.h"
#include "globals.h"

namespace {
const uint8_t PROGMEM MINI_HAPPY[] = {
    0b00111000,
    0b01000100,
    0b10101010,
    0b10000010,
    0b10101010,
    0b01000100,
    0b00111000
};

const uint8_t PROGMEM MINI_NEUTRAL[] = {
    0b00111000,
    0b01000100,
    0b10101010,
    0b10000010,
    0b10000010,
    0b01000100,
    0b00111000
};

const uint8_t PROGMEM MINI_SAD[] = {
    0b00111000,
    0b01000100,
    0b10101010,
    0b10000010,
    0b10101010,
    0b01000100,
    0b00111000
};
} // namespace

void BGDisplayFaceSmileyPlusStats::showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    auto lastReading = readings.back();
    auto bgLevel = bgDisplayManager.getGlucoseIntervals().getBGLevel(lastReading.sgv);
    bool isFallingFast = (lastReading.trend == BG_TREND::DOUBLE_DOWN || lastReading.trend == BG_TREND::SINGLE_DOWN);

    const uint8_t* moodBmp = MINI_HAPPY;
    uint16_t statusColor = COLOR_GREEN;

    if (bgLevel == BG_LEVEL::URGENT_LOW || bgLevel == BG_LEVEL::WARNING_LOW) {
        statusColor = COLOR_RED;
        moodBmp = MINI_SAD;
    } else if (bgLevel == BG_LEVEL::URGENT_HIGH || bgLevel == BG_LEVEL::WARNING_HIGH) {
        statusColor = COLOR_YELLOW;
        moodBmp = MINI_NEUTRAL;
    } else if (isFallingFast) {
        statusColor = COLOR_YELLOW;
        moodBmp = MINI_NEUTRAL;
    }

    if (dataIsOld) {
        statusColor = COLOR_GRAY;
    }

    DisplayManager.clearMatrix(false);
    DisplayManager.drawBitmap(0, 0, moodBmp, 7, 7, statusColor, false);
    showReading(lastReading, 8, 6, TEXT_ALIGNMENT::LEFT, FONT_TYPE::MEDIUM, dataIsOld, false);

    // Delta calculation
    if (readings.size() >= 2) {
        auto it = readings.rbegin();
        it++;
        int deltaVal = lastReading.sgv - it->sgv;
        String deltaStr = (deltaVal >= 0 ? "+" : "") + String(deltaVal);
        bool headingToTarget = (lastReading.sgv > 180 && deltaVal < 0) || (lastReading.sgv < 70 && deltaVal > 0) || (lastReading.sgv >= 70 && lastReading.sgv <= 180);
        uint16_t deltaColor = dataIsOld ? (uint16_t)COLOR_GRAY : (headingToTarget ? (uint16_t)COLOR_GREEN : (uint16_t)COLOR_YELLOW);

        DisplayManager.setFont(FONT_TYPE::SMALL);
        DisplayManager.setTextColor(deltaColor);
        DisplayManager.printText(24, 6, deltaStr.c_str(), TEXT_ALIGNMENT::RIGHT, 1, false);
    }

    showTrendArrow(lastReading, MATRIX_WIDTH - 5, 1, dataIsOld, false, false);
    drawTimerBlocks(lastReading, MATRIX_WIDTH, 0, 7);
    DisplayManager.update();
}
