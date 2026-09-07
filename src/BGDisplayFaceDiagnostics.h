#ifndef BGDISPLAYFACEDIAGNOSTICS_H
#define BGDISPLAYFACEDIAGNOSTICS_H

#include "BGDisplayFace.h"

class BGDisplayFaceDiagnostics : public BGDisplayFace {
public:
    void showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld = false) const override;
    void showNoData() const override;
    bool needsFrequentRefresh() const override;
    unsigned long getFrequentRefreshIntervalMs() const override;

private:
    void showDiagnosticsTicker(const std::list<GlucoseReading>& readings) const;
};

#endif
