#include "BGDisplayFaceSmileyPlusStats.h"
#include "BGDisplayManager.h"
#include "globals.h"

namespace {
// 7x7 outline faces: circle rows 0-1/5-6, eyes row 2, mouth row 4 (happy/neutral)
// or row 5 (sad, pulled down). Mouth: smile corners / straight bar / low corners.
const uint8_t PROGMEM MINI_HAPPY[] = { 0b00111000, 0b01000100, 0b10101010, 0b10000010, 0b11000110, 0b01000100, 0b00111000 };
const uint8_t PROGMEM MINI_NEUTRAL[] = { 0b00111000, 0b01000100, 0b10101010, 0b10000010, 0b10111010, 0b01000100, 0b00111000 };
const uint8_t PROGMEM MINI_SAD[] = { 0b00111000, 0b01000100, 0b10101010, 0b10000010, 0b10000010, 0b11000110, 0b00111000 };
}

bool BGDisplayFaceSmileyPlusStats::needsFrequentRefresh() const { return true; }

unsigned long BGDisplayFaceSmileyPlusStats::getFrequentRefreshIntervalMs() const { return 100; }

void BGDisplayFaceSmileyPlusStats::showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    auto lastReading = readings.back();
    auto bgLevel = bgDisplayManager.getGlucoseIntervals().getBGLevel(lastReading.sgv);
    bool isFallingFast = (lastReading.trend == BG_TREND::DOUBLE_DOWN || lastReading.trend == BG_TREND::SINGLE_DOWN);

    const uint8_t* moodBmp = MINI_HAPPY;
    uint16_t statusColor = COLOR_GREEN;
    if (bgLevel == BG_LEVEL::URGENT_LOW || bgLevel == BG_LEVEL::WARNING_LOW) {
        statusColor = COLOR_RED;
        moodBmp = MINI_SAD;
    } else if (bgLevel == BG_LEVEL::URGENT_HIGH || bgLevel == BG_LEVEL::WARNING_HIGH || isFallingFast) {
        statusColor = COLOR_YELLOW;
        moodBmp = MINI_NEUTRAL;
    }
    if (dataIsOld) statusColor = getDataOldColor();

    DisplayManager.drawBitmap(0, 0, moodBmp, 7, 7, statusColor, false);
    showReading(lastReading, 8, 6, TEXT_ALIGNMENT::LEFT, FONT_TYPE::MEDIUM, dataIsOld, false);

    bool showDelta = (millis() / 2500) % 2 == 1;
    if (showDelta && readings.size() >= 2) {
        auto it = readings.rbegin();
        it++;
        int d = lastReading.sgv - it->sgv;
        String dStr = (d >= 0 ? "+" : "") + getPrintableReading(d);
        // sgv is stored in mg/dL internally regardless of display units.
        bool towards = (lastReading.sgv > 180 && d < 0) || (lastReading.sgv < 70 && d > 0)
            || (lastReading.sgv >= 70 && lastReading.sgv <= 180);
        uint16_t dCol = dataIsOld ? getDataOldColor() : (towards ? (uint16_t)COLOR_GREEN : (uint16_t)COLOR_YELLOW);
        DisplayManager.setFont(FONT_TYPE::SMALL);
        DisplayManager.setTextColor(dCol);
        DisplayManager.printText(31, 6, dStr.c_str(), TEXT_ALIGNMENT::RIGHT, 1, false);
    } else {
        showTrendArrow(lastReading, MATRIX_WIDTH - 5, 1, dataIsOld, false, false);
    }
    drawTimerBlocks(lastReading, MATRIX_WIDTH, 0, 7);
}
