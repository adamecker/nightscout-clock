#include "BGDisplayFaceDiagnostics.h"
#include "globals.h"
#include <WiFi.h>

namespace {
int16_t scrollX = 32;
unsigned long lastScrollMs = 0;
unsigned long lastRefreshMs = 0;
String cachedText = "";
uint16_t textColor = COLOR_CYAN;

void updateDiagnosticText(const std::list<GlucoseReading>& readings) {
    int rssi = WiFi.RSSI();
    String wifiStr = WiFi.isConnected() ? (String(rssi) + "dBm") : "OFFLINE";

    int ageMin = 0;
    int sgvVal = 0;
    if (!readings.empty()) {
        ageMin = readings.back().getSecondsAgo() / 60;
        sgvVal = readings.back().sgv;
    }
    
    uint32_t freeHeap = ESP.getFreeHeap() / 1024;

    cachedText = "WIFI: " + wifiStr +
                 " | VAL: " + String(sgvVal) +
                 " | AGE: " + String(ageMin) + "m" +
                 " | BAT: " + String(BATTERY_PERCENT) + "%" +
                 " | RAM: " + String(freeHeap) + "KB ";

    if (!WiFi.isConnected() || ageMin > 15) {
        textColor = COLOR_RED;
    } else {
        textColor = COLOR_CYAN;
    }
}
} // namespace

void BGDisplayFaceDiagnostics::showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    showDiagnosticsTicker(readings);
}

void BGDisplayFaceDiagnostics::showNoData() const {
    std::list<GlucoseReading> empty;
    showDiagnosticsTicker(empty);
}

bool BGDisplayFaceDiagnostics::needsFrequentRefresh() const {
    return true;
}

unsigned long BGDisplayFaceDiagnostics::getFrequentRefreshIntervalMs() const {
    return 40;
}

void BGDisplayFaceDiagnostics::showDiagnosticsTicker(const std::list<GlucoseReading>& readings) const {
    unsigned long now = millis();
    if (now - lastRefreshMs > 5000 || cachedText.length() == 0) {
        updateDiagnosticText(readings);
        lastRefreshMs = now;
    }

    if (now - lastScrollMs > 40) {
        scrollX--;
        int textWidth = DisplayManager.getTextWidth(cachedText.c_str(), 2);
        if (scrollX < -textWidth) {
            scrollX = 32;
        }
        lastScrollMs = now;
    }

    DisplayManager.clearMatrix(false);
    DisplayManager.setFont(FONT_TYPE::SMALL);
    DisplayManager.setTextColor(textColor);
    DisplayManager.printText(scrollX, 6, cachedText.c_str(), TEXT_ALIGNMENT::LEFT, 2, false);
    DisplayManager.update();
}
