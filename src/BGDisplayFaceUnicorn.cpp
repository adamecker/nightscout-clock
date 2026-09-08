#include "BGDisplayFaceUnicorn.h"
#include "BGDisplayManager.h"
#include "globals.h"

namespace {
int16_t uScrollX = -12;
unsigned long uLastStepMs = 0, uPauseStartMs = 0;
bool uIsPaused = false;

uint16_t rgbColor(uint8_t r, uint8_t g, uint8_t b) { return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3); }
uint16_t hsvToRgb(uint8_t hue) {
    uint8_t region = hue / 43, rem = (hue - (region * 43)) * 6, q = 255 - rem, t = rem;
    switch (region) {
        case 0: return rgbColor(255, t, 0);
        case 1: return rgbColor(q, 255, 0);
        case 2: return rgbColor(0, 255, t);
        case 3: return rgbColor(0, q, 255);
        case 4: return rgbColor(t, 0, 255);
        default: return rgbColor(255, 0, q);
    }
}
}

void BGDisplayFaceUnicorn::onActivate() const { uScrollX = -12; uLastStepMs = 0; uPauseStartMs = 0; uIsPaused = false; }
bool BGDisplayFaceUnicorn::needsFrequentRefresh() const { return true; }
unsigned long BGDisplayFaceUnicorn::getFrequentRefreshIntervalMs() const { return 35; }

void BGDisplayFaceUnicorn::drawUnicorn(int16_t x, int16_t y, uint8_t frame) const {
    DisplayManager.drawPixel(x + 6, y + 0, rgbColor(255, 215, 0), false);
    DisplayManager.drawPixel(x + 5, y + 1, rgbColor(255, 215, 0), false);
    DisplayManager.drawPixel(x + 4, y + 1, rgbColor(255, 20, 147), false);
    DisplayManager.drawPixel(x + 3, y + 2, rgbColor(255, 105, 180), false);
    DisplayManager.drawPixel(x + 4, y + 2, COLOR_WHITE, false);
    DisplayManager.drawPixel(x + 6, y + 2, COLOR_WHITE, false);
    DisplayManager.drawPixel(x + 6, y + 3, rgbColor(255, 182, 193), false);
    DisplayManager.drawPixel(x + 5, y + 2, rgbColor(0, 191, 255), false);
    DisplayManager.drawPixel(x + 3, y + 3, COLOR_WHITE, false);
    DisplayManager.drawPixel(x + 4, y + 3, COLOR_WHITE, false);
    DisplayManager.drawPixel(x + 5, y + 3, COLOR_WHITE, false);
    for (int8_t bx = 2; bx <= 5; bx++) {
        DisplayManager.drawPixel(x + bx, y + 4, COLOR_WHITE, false);
        DisplayManager.drawPixel(x + bx, y + 5, COLOR_WHITE, false);
    }
    uint16_t hoof = rgbColor(255, 105, 180);
    if (frame == 0) {
        DisplayManager.drawPixel(x + 2, y + 6, COLOR_WHITE, false); DisplayManager.drawPixel(x + 1, y + 7, hoof, false);
        DisplayManager.drawPixel(x + 5, y + 6, COLOR_WHITE, false); DisplayManager.drawPixel(x + 6, y + 7, hoof, false);
    } else {
        DisplayManager.drawPixel(x + 3, y + 6, COLOR_WHITE, false); DisplayManager.drawPixel(x + 2, y + 7, hoof, false);
        DisplayManager.drawPixel(x + 4, y + 6, COLOR_WHITE, false); DisplayManager.drawPixel(x + 5, y + 7, hoof, false);
    }
}

void BGDisplayFaceUnicorn::drawNyanRainbow(int16_t startX, int16_t endX, uint8_t waveTick) const {
    if (endX < 0 || startX > 31) return;
    int16_t x0 = max((int16_t)0, startX), x1 = min((int16_t)31, endX);
    const uint16_t nyan[5] = { rgbColor(255, 0, 55), rgbColor(255, 140, 0), rgbColor(255, 235, 0), rgbColor(0, 255, 60), rgbColor(160, 40, 255) };
    for (int16_t x = x0; x <= x1; x++) {
        int seg = ((x / 2) + waveTick) % 2;
        int yBase = (seg == 0) ? 1 : 2;
        for (int b = 0; b < 5; b++) DisplayManager.drawPixel(x, yBase + b, nyan[b], false);
        if ((x + waveTick) % 5 == 0) DisplayManager.drawPixel(x, (seg == 0 ? 7 : 0), COLOR_WHITE, false);
    }
}

void BGDisplayFaceUnicorn::showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    unsigned long now = millis();
    uint8_t leg = (now / 110) % 2, wave = (now / 110) % 4;
    auto lastReading = readings.back();
    String valStr = getPrintableReading(lastReading.sgv);
    int valWidth = DisplayManager.getTextWidth(valStr.c_str(), 2);

    if (uIsPaused) {
        if (now - uPauseStartMs > 3500) uIsPaused = false;
    } else if (now - uLastStepMs > 35) {
        uScrollX++;
        if (uScrollX == 24) { uIsPaused = true; uPauseStartMs = now; }
        if (uScrollX > (32 + valWidth + 18)) uScrollX = -10;
        uLastStepMs = now;
    }

    int16_t ux = uScrollX, tEnd = ux + 1, tStart = ux - 10, bgX = tStart - valWidth - 2, arrX = bgX + valWidth + 1;
    drawNyanRainbow(tStart, tEnd, wave);
    drawUnicorn(ux, 0, leg);

    DisplayManager.setFont(FONT_TYPE::MEDIUM);
    uint8_t hue = (now / 15) % 255;
    int16_t curX = bgX;
    for (size_t i = 0; i < valStr.length(); i++) {
        char buf[2] = {valStr[i], '\0'};
        uint16_t col = dataIsOld ? (uint16_t)COLOR_GRAY : hsvToRgb(hue + (i * 35));
        DisplayManager.setTextColor(col);
        DisplayManager.printText(curX, 6, buf, TEXT_ALIGNMENT::LEFT, 2, false);
        curX += DisplayManager.getTextWidth(buf, 2);
    }
    showTrendArrow(lastReading, arrX, 1, dataIsOld, false, false);
}
