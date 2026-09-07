#pragma once

#include "BGDisplayFace.h"

class BGDisplayFaceUnicorn : public BGDisplayFace {
public:
    explicit BGDisplayFaceUnicorn(DisplayManager& displayManager);
    void update() override;
    void render() override;

private:
    int16_t _scrollX = -10;
    uint32_t _lastStep = 0;
    uint32_t _lastLegToggle = 0;
    uint32_t _pauseStart = 0;
    uint8_t _legFrame = 0;
    uint8_t _hue = 0;
    bool _isPaused = false;

    void drawUnicorn(int16_t x, int16_t y, uint8_t frame);
    void drawTrail(int16_t startX, int16_t endX);
    void drawRainbowString(int16_t x, int16_t y, const String& text);
};
