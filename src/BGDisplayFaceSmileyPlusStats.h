#pragma once

#include "BGDisplayFace.h"

class BGDisplayFaceSmileyPlusStats : public BGDisplayFace {
public:
    explicit BGDisplayFaceSmileyPlusStats(DisplayManager& displayManager);
    void update() override;
    void render() override;

private:
    uint8_t _staleBlink = 0;
    uint32_t _lastBlink = 0;
};
