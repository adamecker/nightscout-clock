#include "BGDisplayFaceUnicorn.h"
#include "BGSourceManager.h"
#include "globals.h"

BGDisplayFaceUnicorn::BGDisplayFaceUnicorn(DisplayManager& displayManager)
    : BGDisplayFace(displayManager) {
}

void BGDisplayFaceUnicorn::drawUnicorn(int16_t x, int16_t y, uint8_t frame) {
    _displayManager.drawPixel(x + 6, y + 0, CRGB(255, 215, 0));
    _displayManager.drawPixel(x + 5, y + 1, CRGB(255, 215, 0));

    _displayManager.drawPixel(x + 4, y + 1, CRGB(255, 20, 147));
    _displayManager.drawPixel(x + 3, y + 2, CRGB(255, 105, 180));
    _displayManager.drawPixel(x + 2, y + 3, CRGB(186, 85, 211));

    _displayManager.drawPixel(x + 4, y + 2, CRGB::White);
    _displayManager.drawPixel(x + 6, y + 2, CRGB::White);
    _displayManager.drawPixel(x + 6, y + 3, CRGB(255, 182, 193));
    _displayManager.drawPixel(x + 5, y + 2, CRGB(0, 191, 255));

    _displayManager.drawPixel(x + 3, y + 3, CRGB::White);
    _displayManager.drawPixel(x + 4, y + 3, CRGB::White);
    _displayManager.drawPixel(x + 5, y + 3, CRGB::White);
    for (int8_t bx = 2; bx <= 5; bx++) {
        _displayManager.drawPixel(x + bx, y + 4, CRGB::White);
        _displayManager.drawPixel(x + bx, y + 5, CRGB::White);
    }

    _displayManager.drawPixel(x + 1, y + 4, CRGB(255, 105, 180));
    _displayManager.drawPixel(x + 0, y + 4, CRGB(255, 215, 0));
    _displayManager.drawPixel(x + 1, y + 5, CRGB(0, 255, 200));

    CRGB hoof = CRGB(255, 105, 180);
    if (frame == 0) {
        _displayManager.drawPixel(x + 2, y + 6, CRGB::White);
        _displayManager.drawPixel(x + 1, y + 7, hoof);
        _displayManager.drawPixel(x + 5, y + 6, CRGB::White);
        _displayManager.drawPixel(x + 6, y + 7, hoof);
    } else {
        _displayManager.drawPixel(x + 3, y + 6, CRGB::White);
        _displayManager.drawPixel(x + 2, y + 7, hoof);
        _displayManager.drawPixel(x + 4, y + 6, CRGB::White);
        _displayManager.drawPixel(x + 5, y + 7, hoof);
    }
}

void BGDisplayFaceUnicorn::drawTrail(int16_t startX, int16_t endX) {
    if (endX < 0 || startX > 31) return;
    int16_t x0 = max((int16_t)0, startX);
    int16_t x1 = min((int16_t)31, endX);

    for (int16_t x = x0; x <= x1; x++) {
        _displayManager.drawPixel(x, 2, CRGB(255, 50, 120));
        _displayManager.drawPixel(x, 3, CRGB(255, 180, 0));
        _displayManager.drawPixel(x, 4, CRGB(0, 230, 120));
        _displayManager.drawPixel(x, 5, CRGB(120, 50, 255));
        if ((x + _scrollX) % 3 == 0) {
            _displayManager.drawPixel(x, 1, CRGB::White);
        }
    }
}

void BGDisplayFaceUnicorn::drawRainbowString(int16_t x, int16_t y, const String& text) {
    int16_t curX = x;
    for (size_t i = 0; i < text.length(); i++) {
        uint8_t h = _hue + (i * 30);
        _displayManager.drawChar(curX, y, text[i], CHSV(h, 220, 255));
        curX += 4;
    }
}

void BGDisplayFaceUnicorn::update() {
    uint32_t now = millis();
    if (now - _lastLegToggle > 130) {
        _legFrame = !_legFrame;
        _hue += 6;
        _lastLegToggle = now;
    }

    if (_isPaused) {
        if (now - _pauseStart > 3500) {
            _isPaused = false;
        }
        return;
    }

    if (now - _lastStep > 40) {
        _scrollX++;
        if (_scrollX - 21 == 4) {
            _isPaused = true;
            _pauseStart = now;
        }
        if (_scrollX > (32 + 55)) {
            _scrollX = -10;
        }
        _lastStep = now;
    }
}

void BGDisplayFaceUnicorn::render() {
    _displayManager.clear();
    BGReading r = bgSourceManager.getLatestReading();
    String valStr = String(r.value);
    String deltaStr = (r.delta >= 0 ? "+" : "") + String(r.delta);

    int16_t unicornX = _scrollX;
    int16_t bgX = unicornX - 21;
    int16_t arrowX = bgX + (valStr.length() * 4) + 1;
    int16_t deltaX = arrowX + 6;

    bool headingToTarget = (r.value > 180 && r.delta < 0) || (r.value < 70 && r.delta > 0) || (r.value >= 70 && r.value <= 180);
    CRGB deltaColor = headingToTarget ? CRGB::SpringGreen : CRGB(255, 140, 0);

    drawTrail(unicornX - 8, unicornX - 1);
    drawUnicorn(unicornX, 0, _legFrame);
    drawRainbowString(bgX, 1, valStr);
    _displayManager.drawTrendArrow(arrowX, 1, r.trend, CHSV(_hue + 160, 240, 255));
    _displayManager.drawString(deltaX, 1, deltaStr, deltaColor);
}
