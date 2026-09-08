#include "BGDisplayFaceBigTextRainbow.h"
#include "globals.h"

namespace {
constexpr unsigned long RAINBOW_REFRESH_INTERVAL_MS = 30;
uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) { return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3); }
uint16_t hsvToRgb565(uint8_t hue) {
    uint8_t region = hue / 43, rem = (hue - (region * 43)) * 6, q = 255 - rem, t = rem;
    switch (region) {
        case 0: return rgb565(255, t, 0);
        case 1: return rgb565(q, 255, 0);
        case 2: return rgb565(0, 255, t);
        case 3: return rgb565(0, q, 255);
        case 4: return rgb565(t, 0, 255);
        default: return rgb565(255, 0, q);
    }
}
}
void BGDisplayFaceBigTextRainbow::showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    auto lastReading = readings.back();
    showAnimatedReading(lastReading, dataIsOld);
    showTrendArrow(lastReading, MATRIX_WIDTH - 5, 1, dataIsOld, false, false);
}
bool BGDisplayFaceBigTextRainbow::needsFrequentRefresh() const { return true; }
unsigned long BGDisplayFaceBigTextRainbow::getFrequentRefreshIntervalMs() const { return RAINBOW_REFRESH_INTERVAL_MS; }
void BGDisplayFaceBigTextRainbow::showAnimatedReading(const GlucoseReading& reading, bool dataIsOld) const {
    String str = getPrintableReading(reading.sgv);
    uint8_t hueOffset = (millis() / 8) % 255;
    int16_t x = 0;
    DisplayManager.setFont(FONT_TYPE::LARGE);
    for (size_t i = 0; i < str.length(); i++) {
        char buf[2] = {str[i], '\0'};
        uint16_t col = dataIsOld ? (uint16_t)COLOR_GRAY : hsvToRgb565(hueOffset + (i * 45));
        DisplayManager.setTextColor(col);
        DisplayManager.printText(x, 7, buf, TEXT_ALIGNMENT::LEFT, 2, false);
        x += DisplayManager.getTextWidth(buf, 2);
    }
}
