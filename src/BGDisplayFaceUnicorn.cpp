#include "BGDisplayFaceUnicorn.h"
#include "BGDisplayManager.h"
#include "globals.h"

namespace {
int16_t scrollX = -10;
unsigned long lastStepMs = 0;
unsigned long pauseStartMs = 0;
bool isPaused = false;

uint16_t rgbColor(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
}

uint16_t hsvToRgb(uint8_t hue) {
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
    return rgbColor(r, g, b);
}
} // namespace

bool BGDisplayFaceUnicorn::needsFrequentRefresh() const {
    return true;
}

unsigned long BGDisplayFaceUnicorn::getFrequentRefreshIntervalMs() const {
    return 40;
}

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

    DisplayManager.drawPixel(x + 1, y + 4, rgbColor(255, 105, 180), false);
    DisplayManager.drawPixel(x + 0, y + 4, rgbColor(255, 215, 0), false);
    DisplayManager.drawPixel(x + 1, y + 5, rgbColor(0, 255, 200), false);

    uint16_t hoof = rgbColor(255, 105, 180);
    if (frame == 0) {
        DisplayManager.drawPixel(x + 2, y + 6, COLOR_WHITE, false);
        DisplayManager.drawPixel(x + 1, y + 7, hoof, false);
        DisplayManager.drawPixel(x + 5, y + 6, COLOR_WHITE, false);
        DisplayManager.drawPixel(x + 6, y + 7, hoof, false);
    } else {
        DisplayManager.drawPixel(x + 3, y + 6, COLOR_WHITE, false);
        DisplayManager.drawPixel(x + 2, y + 7, hoof, false);
        DisplayManager.drawPixel(x + 4, y + 6, COLOR_WHITE, false);
        DisplayManager.drawPixel(x + 5, y + 7, hoof, false);
    }
}

void BGDisplayFaceUnicorn::drawTrail(int16_t startX, int16_t endX) const {
    if (endX < 0 || startX > 31) return;
    int16_t x0 = max((int16_t)0, startX);
    int16_t x1 = min((int16_t)31, endX);

    for (int16_t x = x0; x <= x1; x++) {
        DisplayManager.drawPixel(x, 2, rgbColor(255, 50, 120), false);
        DisplayManager.drawPixel(x, 3, rgbColor(255, 180, 0), false);
        DisplayManager.drawPixel(x, 4, rgbColor(0, 230, 120), false);
        DisplayManager.drawPixel(x, 5, rgbColor(120, 50, 255), false);
        if ((x + scrollX) % 3 == 0) {
            DisplayManager.drawPixel(x, 1, COLOR_WHITE, false);
        }
    }
}

void BGDisplayFaceUnicorn::showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    unsigned long now = millis();
    uint8_t legFrame = (now / 130) % 2;

    if (isPaused) {
        if (now - pauseStartMs > 3500) {
            isPaused = false;
        }
    } else if (now - lastStepMs > 40) {
        scrollX++;
        if (scrollX - 21 == 4) {
            isPaused = true;
            pauseStartMs = now;
        }
        if (scrollX > (32 + 55)) {
            scrollX = -10;
        }
        lastStepMs = now;
    }

    DisplayManager.clearMatrix(false);

    auto lastReading = readings.back();
    String valStr = getPrintableReading(lastReading.sgv);
    int16_t unicornX = scrollX;
    int16_t bgX = unicornX - 21;
    int16_t arrowX = bgX + DisplayManager.getTextWidth(valStr.c_str(), 2) + 2;

    drawTrail(unicornX - 8, unicornX - 1);
    drawUnicorn(unicornX, 0, legFrame);

    DisplayManager.setFont(FONT_TYPE::MEDIUM);
    uint8_t hue = (now / 20) % 255;
    for (size_t i = 0; i < valStr.length(); i++) {
        char buf[2] = {valStr[i], '\0'};
        uint16_t color = dataIsOld ? (uint16_t)COLOR_GRAY : hsvToRgb(hue + (i * 35));
        DisplayManager.setTextColor(color);
        DisplayManager.printText(bgX, 6, buf, TEXT_ALIGNMENT::LEFT, 2, false);
        bgX += DisplayManager.getTextWidth(buf, 2);
    }

    showTrendArrow(lastReading, arrowX, 1, dataIsOld, false, false);
    DisplayManager.update();
}
