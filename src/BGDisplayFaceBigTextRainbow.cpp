#include "BGDisplayFaceBigTextRainbow.h"
#include "globals.h"

namespace {
constexpr unsigned long RAINBOW_REFRESH_INTERVAL_MS = 30;
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
        uint16_t col = dataIsOld ? getDataOldColor() : DisplayManager.hsvToRgb565(hueOffset + (i * 45));
        DisplayManager.setTextColor(col);
        DisplayManager.printText(x, 7, buf, TEXT_ALIGNMENT::LEFT, 2, false);
        x += DisplayManager.getTextWidth(buf, 2);
    }
}
