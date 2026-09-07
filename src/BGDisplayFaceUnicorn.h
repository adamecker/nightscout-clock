#ifndef BGDISPLAYFACEUNICORN_H
#define BGDISPLAYFACEUNICORN_H

#include "BGDisplayFaceTextBase.h"

class BGDisplayFaceUnicorn : public BGDisplayFaceTextBase {
public:
    void showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld = false) const override;
    bool needsFrequentRefresh() const override;
    unsigned long getFrequentRefreshIntervalMs() const override;

private:
    void drawUnicorn(int16_t x, int16_t y, uint8_t frame) const;
    void drawTrail(int16_t startX, int16_t endX) const;
};

#endif
