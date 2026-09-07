#pragma once

#include "BGDisplayFace.h"

class BGDisplayFaceDiagnostics : public BGDisplayFace {
public:
    explicit BGDisplayFaceDiagnostics(DisplayManager& displayManager);
    void update() override;
    void render() override;

private:
    int16_t _scrollX = 32;
    uint32_t _lastScrollMs = 0;
    uint32_t _lastRefreshMs = 0;
    String _text;
    CRGB _textColor = CRGB::DeepSkyBlue;

    void refreshDiagnosticText();
};
