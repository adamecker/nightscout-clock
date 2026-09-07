#ifndef BGDISPLAYFACESMILEYPLUSSTATS_H
#define BGDISPLAYFACESMILEYPLUSSTATS_H

#include "BGDisplayFaceTextBase.h"
#include "BGDisplayFaceWithAge.h"

class BGDisplayFaceSmileyPlusStats : public BGDisplayFaceTextBase, public BGDisplayFaceWithAge {
public:
    void showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld = false) const override;
};

#endif
