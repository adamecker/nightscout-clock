#include "BGDisplayFaceCritter.h"

#include "BGDisplayManager.h"
#include "globals.h"

namespace {

// 12x8 sprites, one palette index per pixel (row-major). 0 = transparent,
// 1 = body (follows glucose level), 2 = accent, 3 = eye.

const uint8_t spritePoop[12 * 8] PROGMEM = {
    0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
};

const uint8_t spriteCat[12 * 8] PROGMEM = {
    0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
    0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 2, 2, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
};

const uint8_t spriteDog[12 * 8] PROGMEM = {
    0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
    0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 2, 2, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
};

const uint8_t spriteFrog[12 * 8] PROGMEM = {
    0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
    0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 3, 1, 1, 1, 1, 1, 1, 3, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 2, 2, 2, 2, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
};

const uint8_t spritePanda[12 * 8] PROGMEM = {
    0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 0,
    0, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 0,
    0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0,
    0, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 0,
    0, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 2, 2, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
};

const uint8_t spritePenguin[12 * 8] PROGMEM = {
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 2, 1, 1, 1, 1, 2, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 0,
    0, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
};

const uint8_t spriteOwl[12 * 8] PROGMEM = {
    0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 0,
    0, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 0,
    0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
};

const uint8_t spriteFox[12 * 8] PROGMEM = {
    0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
    0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 2, 2, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
};

const uint8_t spriteBear[12 * 8] PROGMEM = {
    0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 2, 2, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
};

const uint8_t spriteBunny[12 * 8] PROGMEM = {
    0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 3, 1, 1, 1, 1, 3, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
};

const uint8_t spritePig[12 * 8] PROGMEM = {
    0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 2, 2, 2, 2, 1, 1, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
};

// Natural body colors (RGB565) for each critter.
const uint16_t bodyNatural[] = {
    0x8A22,  // POOP brown
    0xFC00,  // CAT orange
    0xFD20,  // DOG tan
    0x07E0,  // FROG green
    0xFFFF,  // PANDA white
    0x0000,  // PENGUIN black
    0x8A22,  // OWL brown
    0xFC00,  // FOX orange
    0x8A22,  // BEAR brown
    0xFFFF,  // BUNNY white
    0xFC9F,  // PIG pink
};

// Accent colors (constant across levels).
const uint16_t accentColor[] = {
    0x71A0,  // POOP dark brown
    0xF81F,  // CAT pink nose
    0x8A22,  // DOG brown snout
    0x87F0,  // FROG light green smile
    0x0000,  // PANDA black ears/patches
    0xFFFF,  // PENGUIN white belly/eyes
    0xFD20,  // OWL tan beak
    0xFFFF,  // FOX white snout
    0xFD20,  // BEAR tan snout
    0xF81F,  // BUNNY pink nose
    0xF81F,  // PIG dark pink snout
};

// Eye colors (constant).
const uint16_t eyeColor[] = {
    0x0000,  // POOP
    0x0000,  // CAT
    0x0000,  // DOG
    0x0000,  // FROG
    0x0000,  // PANDA
    0xFFFF,  // PENGUIN (white eyes on black body)
    0x0000,  // OWL
    0x0000,  // FOX
    0x0000,  // BEAR
    0x0000,  // BUNNY
    0x0000,  // PIG
};

}  // namespace

BGDisplayFaceCritter::BGDisplayFaceCritter(CritterId id) : critterId(id) {}

const uint8_t* BGDisplayFaceCritter::getSprite() const {
    switch (critterId) {
        case CritterId::POOP: return spritePoop;
        case CritterId::CAT: return spriteCat;
        case CritterId::DOG: return spriteDog;
        case CritterId::FROG: return spriteFrog;
        case CritterId::PANDA: return spritePanda;
        case CritterId::PENGUIN: return spritePenguin;
        case CritterId::OWL: return spriteOwl;
        case CritterId::FOX: return spriteFox;
        case CritterId::BEAR: return spriteBear;
        case CritterId::BUNNY: return spriteBunny;
        case CritterId::PIG: return spritePig;
        default: return spritePoop;
    }
}

const uint16_t* BGDisplayFaceCritter::getPalette(BG_LEVEL level, bool dataIsOld) const {
    // Static to avoid stack allocation on each frame. RAM-based (not PROGMEM)
    // because the body color is computed dynamically from the glucose level.
    static uint16_t palette[4];
    uint8_t idx = static_cast<uint8_t>(critterId);

    uint16_t body;
    uint16_t accent = accentColor[idx];
    if (dataIsOld) {
        body = getDataOldColor();
        accent = getDataOldColor();
    } else {
        switch (level) {
            case BG_LEVEL::URGENT_LOW:
            case BG_LEVEL::URGENT_HIGH:
                body = 0xF800;  // red
                break;
            case BG_LEVEL::WARNING_LOW:
            case BG_LEVEL::WARNING_HIGH:
                body = 0xFFE0;  // yellow
                break;
            case BG_LEVEL::NORMAL:
            case BG_LEVEL::INVALID:
            default:
                body = bodyNatural[idx];
                break;
        }
    }

    palette[0] = 0x0000;  // transparent (unused)
    palette[1] = body;
    palette[2] = accent;
    palette[3] = eyeColor[idx];
    return palette;
}

void BGDisplayFaceCritter::drawSprite(const uint8_t* sprite, const uint16_t* palette) const {
    // drawIndexedSprite requires a PROGMEM palette; our palette is computed
    // in RAM, so draw manually.
    for (int16_t row = 0; row < 8; row++) {
        for (int16_t col = 0; col < 12; col++) {
            uint8_t paletteIndex = pgm_read_byte(&sprite[row * 12 + col]);
            if (paletteIndex == 0) {
                continue;  // transparent
            }
            DisplayManager.drawPixel(col, row, palette[paletteIndex], false);
        }
    }
}

void BGDisplayFaceCritter::showReadings(
    const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    auto lastReading = readings.back();
    auto bgLevel = bgDisplayManager.getGlucoseIntervals().getBGLevel(lastReading.sgv);

    drawSprite(getSprite(), getPalette(bgLevel, dataIsOld));
    showReading(lastReading, MATRIX_WIDTH - 1, 6, TEXT_ALIGNMENT::RIGHT, FONT_TYPE::MEDIUM, dataIsOld);
    drawTimerBlocks(lastReading, 16, 16, 7);
}

void BGDisplayFaceCritter::showNoData() const {
    DisplayManager.clearMatrix();
    // Show the critter in stale gray with no reading.
    drawSprite(getSprite(), getPalette(BG_LEVEL::INVALID, true));
    String noData = "---";
    if (SettingsManager.settings.bg_units == BG_UNIT::MMOLL) {
        noData = "--.-";
    }
    DisplayManager.setTextColor(getDataOldColor());
    DisplayManager.printText(33, 6, noData.c_str(), TEXT_ALIGNMENT::RIGHT, 2);
}
