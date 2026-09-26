#ifndef BGDISPLAYFACEDIAGNOSTICS_H
#define BGDISPLAYFACEDIAGNOSTICS_H

#include "BGDisplayFaceTextBase.h"

class BGDisplayFaceDiagnostics : public BGDisplayFaceTextBase {
public:
    void showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld = false) const override;
    void showNoData() const override;
    bool needsFrequentRefresh() const override;
    unsigned long getFrequentRefreshIntervalMs() const override;
    void onActivate() const override;

private:
    void showDiagnosticsTicker(const std::list<GlucoseReading>& readings, bool dataIsOld) const;
    void updateDiagnosticText(const std::list<GlucoseReading>& readings, bool dataIsOld) const;
};

#endif
