#include "BGDisplayFaceDiagnostics.h"
#include "BGSourceManager.h"
#include "PeripheryManager.h"
#include "globals.h"
#include <WiFi.h>

BGDisplayFaceDiagnostics::BGDisplayFaceDiagnostics(DisplayManager& displayManager)
    : BGDisplayFace(displayManager) {
    refreshDiagnosticText();
}

void BGDisplayFaceDiagnostics::refreshDiagnosticText() {
    int rssi = WiFi.RSSI();
    String wifiStr = WiFi.isConnected() ? (String(rssi) + "dBm") : "OFFLINE";

    BGReading latest = bgSourceManager.getLatestReading();
    uint32_t ageMin = (latest.timestampMs > 0) ? (millis() - latest.timestampMs) / 60000 : 0;
    float batV = peripheryManager.getBatteryVoltage();
    uint32_t freeHeap = ESP.getFreeHeap() / 1024;

    _text = "WIFI: " + wifiStr +
            " | VAL: " + String(latest.value) +
            " | AGE: " + String(ageMin) + "m" +
            " | BAT: " + String(batV, 2) + "V" +
            " | RAM: " + String(freeHeap) + "KB ";

    if (!WiFi.isConnected() || ageMin > 15) {
        _textColor = CRGB::OrangeRed;
    } else {
        _textColor = CRGB::DeepSkyBlue;
    }
}

void BGDisplayFaceDiagnostics::update() {
    uint32_t now = millis();
    if (now - _lastRefreshMs > 5000 || _text.length() == 0) {
        refreshDiagnosticText();
        _lastRefreshMs = now;
    }

    if (now - _lastScrollMs > 40) {
        _scrollX--;
        int16_t totalWidth = (int16_t)(_text.length() * 6);
        if (_scrollX < -totalWidth) {
            _scrollX = 32;
        }
        _lastScrollMs = now;
    }
}

void BGDisplayFaceDiagnostics::render() {
    _displayManager.clear();
    _displayManager.drawString(_scrollX, 1, _text, _textColor);
}
