#pragma once

#include "BGDisplayFace.h"

class BGDisplayFaceRainbowSparkle : public BGDisplayFace {
public:
    explicit BGDisplayFaceRainbowSparkle(DisplayManager& displayManager);
    void update() override;
    void render() override;

private:
    uint8_t _hueOffset = 0;
    uint8_t _frame = 0;
    uint32_t _lastTick = 0;
    void drawRainbowText(int16_t startX, int16_t startY, const String& text);
};
