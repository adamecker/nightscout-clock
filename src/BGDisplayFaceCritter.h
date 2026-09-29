#ifndef BGDISPLAYFACECRITTER_H
#define BGDISPLAYFACECRITTER_H

#include "BGDisplayFaceTextBase.h"
#include "BGDisplayFaceWithAge.h"

// Cute character faces. Each critter is a 12x8 sprite; the body color
// follows the glucose level (natural/yellow/red) or goes gray when stale.
enum class CritterId : uint8_t {
    POOP = 0,
    CAT,
    DOG,
    FROG,
    OWL,
    FOX,
    BUNNY,
    // Animals
    NARWHAL,
    WHALE,
    OCTOPUS,
    TURTLE,
    MONKEY,
    // Mario
    MARIO,
    LUIGI,
    PEACH,
    TOAD,
    YOSHI,
    // Frozen
    ELSA,
    ANNA,
    OLAF,
    // Halloween / Fall
    PUMPKIN,
    GHOST,
    BAT,
    WITCH,
    TURKEY,
    // Other cute
    MERMAID,
    DINOSAUR,
    BUTTERFLY,
    BEE,
    COUNT
};

class BGDisplayFaceCritter : public BGDisplayFaceTextBase, public BGDisplayFaceWithAge {
public:
    explicit BGDisplayFaceCritter(CritterId id);
    void showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld = false) const override;
    void showNoData() const override;

private:
    CritterId critterId;
    const uint8_t* getSprite() const;
    const uint16_t* getPalette(BG_LEVEL level, bool dataIsOld) const;
    void drawSprite(const uint8_t* sprite, const uint16_t* palette) const;
};

#endif  // BGDISPLAYFACECRITTER_H
