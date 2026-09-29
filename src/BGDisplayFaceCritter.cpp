#include "BGDisplayFaceCritter.h"

#include "BGDisplayManager.h"
#include "globals.h"

namespace {

// 12x8 sprites, one palette index per pixel (row-major). 0 = transparent,
// 1 = body (follows glucose level), 2 = accent, 3 = eye/detail.

// --- Original keepers ---
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

// --- New animals ---
const uint8_t spriteNarwhal[12 * 8] PROGMEM = {
    0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 2, 1, 1, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 3, 1, 1, 1, 3, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 2, 2, 2, 1, 1, 0, 0, 0,
};
const uint8_t spriteWhale[12 * 8] PROGMEM = {
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
};
const uint8_t spriteOctopus[12 * 8] PROGMEM = {
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 3, 3, 1, 1, 3, 3, 1, 1, 0,
    0, 1, 1, 3, 3, 1, 1, 3, 3, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0,
    0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0,
};
const uint8_t spriteTurtle[12 * 8] PROGMEM = {
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 2, 2, 2, 1, 1, 0, 0, 0,
    0, 1, 1, 1, 2, 2, 2, 1, 1, 1, 0, 0,
    0, 1, 1, 3, 1, 1, 1, 3, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
const uint8_t spriteMonkey[12 * 8] PROGMEM = {
    0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
    0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 1, 1, 2, 2, 2, 2, 1, 1, 0, 0,
    0, 0, 0, 1, 2, 2, 2, 2, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
};

// --- Mario characters ---
const uint8_t spriteMario[12 * 8] PROGMEM = {
    0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 1, 1, 3, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,
    0, 0, 2, 3, 2, 2, 3, 2, 0, 0, 0, 0,
    0, 0, 2, 2, 3, 3, 2, 2, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
};
const uint8_t spriteLuigi[12 * 8] PROGMEM = {
    0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 1, 1, 3, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,
    0, 0, 2, 3, 2, 2, 3, 2, 0, 0, 0, 0,
    0, 0, 2, 2, 3, 3, 2, 2, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
};
const uint8_t spritePeach[12 * 8] PROGMEM = {
    0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,
    0, 0, 2, 3, 2, 2, 3, 2, 0, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
};
const uint8_t spriteToad[12 * 8] PROGMEM = {
    0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 2, 1, 1, 2, 1, 1, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,
    0, 0, 2, 3, 2, 2, 3, 2, 0, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
const uint8_t spriteYoshi[12 * 8] PROGMEM = {
    0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 3, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// --- Frozen ---
const uint8_t spriteElsa[12 * 8] PROGMEM = {
    0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,
    0, 0, 2, 3, 3, 3, 3, 2, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
};
const uint8_t spriteAnna[12 * 8] PROGMEM = {
    0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,
    0, 0, 2, 3, 3, 3, 3, 2, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
};
const uint8_t spriteOlaf[12 * 8] PROGMEM = {
    0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 1, 3, 1, 1, 3, 1, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
};

// --- Halloween / Fall ---
const uint8_t spritePumpkin[12 * 8] PROGMEM = {
    0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 3, 1, 1, 3, 3, 1, 1, 3, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 3, 3, 3, 3, 3, 3, 1, 1, 0,
    0, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
};
const uint8_t spriteGhost[12 * 8] PROGMEM = {
    0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 3, 1, 1, 1, 1, 3, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 3, 3, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0,
};
const uint8_t spriteBat[12 * 8] PROGMEM = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 3, 1, 1, 1, 1, 3, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0, 0,
    0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
};
const uint8_t spriteWitch[12 * 8] PROGMEM = {
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,
    0, 0, 2, 3, 2, 2, 3, 2, 0, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
};
const uint8_t spriteTurkey[12 * 8] PROGMEM = {
    0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 1, 3, 1, 1, 3, 1, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
};

// --- Other cute ---
const uint8_t spriteMermaid[12 * 8] PROGMEM = {
    0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,
    0, 0, 2, 3, 3, 3, 3, 2, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
const uint8_t spriteDinosaur[12 * 8] PROGMEM = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
const uint8_t spriteButterfly[12 * 8] PROGMEM = {
    0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
    0, 1, 1, 1, 0, 2, 0, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 2, 1, 1, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 0, 2, 0, 1, 1, 0, 0, 0,
    0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
const uint8_t spriteBee[12 * 8] PROGMEM = {
    0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 3, 1, 1, 3, 1, 1, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 1, 2, 1, 2, 1, 2, 1, 1, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// Natural body colors (RGB565). Index matches CritterId order.
const uint16_t bodyNatural[] = {
    0x8A22,  // POOP brown
    0xFC00,  // CAT orange
    0xFD20,  // DOG tan
    0x07E0,  // FROG green
    0x8A22,  // OWL brown
    0xFC00,  // FOX orange
    0xFFFF,  // BUNNY white
    0x5AEB,  // NARWHAL blue-gray
    0x001F,  // WHALE blue
    0x801F,  // OCTOPUS purple
    0x07E0,  // TURTLE green
    0x8A22,  // MONKEY brown
    0xF800,  // MARIO red
    0x07E0,  // LUIGI green
    0xFC9F,  // PEACH pink
    0xF800,  // TOAD red
    0x07E0,  // YOSHI green
    0x001F,  // ELSA blue
    0x07E0,  // ANNA green
    0xFFFF,  // OLAF white
    0xFC00,  // PUMPKIN orange
    0xFFFF,  // GHOST white
    0x801F,  // BAT purple
    0x801F,  // WITCH purple
    0x8A22,  // TURKEY brown
    0x07FF,  // MERMAID teal
    0x07E0,  // DINOSAUR green
    0xFC9F,  // BUTTERFLY pink
    0xFFE0,  // BEE yellow
};

// Accent colors (constant across levels).
const uint16_t accentColor[] = {
    0x71A0,  // POOP dark brown
    0xF81F,  // CAT pink nose
    0x8A22,  // DOG brown snout
    0x87F0,  // FROG light green smile
    0xFD20,  // OWL tan beak
    0xFFFF,  // FOX white snout
    0xF81F,  // BUNNY pink nose
    0xFFFF,  // NARWHAL white tusk/belly
    0xFFFF,  // WHALE white belly
    0xF81F,  // OCTOPUS pink
    0x8A22,  // TURTLE brown shell
    0xFD20,  // MONKEY tan muzzle
    0xFD20,  // MARIO skin
    0xFD20,  // LUIGI skin
    0xFFE0,  // PEACH blonde hair
    0xFFFF,  // TOAD cream spots/face
    0xFFFF,  // YOSHI white belly
    0xFFE0,  // ELSA blonde hair
    0x8A22,  // ANNA auburn hair
    0xFC00,  // OLAF orange nose/buttons
    0x07E0,  // PUMPKIN green stem
    0xFFFF,  // GHOST white
    0x0000,  // BAT black
    0x07E0,  // WITCH green skin
    0xFC00,  // TURKEY orange beak
    0xF800,  // MERMAID red hair
    0xFFE0,  // DINOSAUR yellow belly
    0x0000,  // BUTTERFLY black body
    0x0000,  // BEE black stripes
};

// Eye/detail colors (constant).
const uint16_t eyeColor[] = {
    0x0000,  // POOP
    0x0000,  // CAT
    0x0000,  // DOG
    0x0000,  // FROG
    0x0000,  // OWL
    0x0000,  // FOX
    0x0000,  // BUNNY
    0x0000,  // NARWHAL
    0x0000,  // WHALE
    0x0000,  // OCTOPUS
    0x0000,  // TURTLE
    0x0000,  // MONKEY
    0x0000,  // MARIO
    0x0000,  // LUIGI
    0x0000,  // PEACH
    0x0000,  // TOAD
    0x0000,  // YOSHI
    0x0000,  // ELSA
    0x0000,  // ANNA
    0x0000,  // OLAF
    0x0000,  // PUMPKIN
    0x0000,  // GHOST
    0xF800,  // BAT red eyes
    0x0000,  // WITCH
    0x0000,  // TURKEY
    0x0000,  // MERMAID
    0x0000,  // DINOSAUR
    0x0000,  // BUTTERFLY
    0x0000,  // BEE
};

}  // namespace

BGDisplayFaceCritter::BGDisplayFaceCritter(CritterId id) : critterId(id) {}

const uint8_t* BGDisplayFaceCritter::getSprite() const {
    switch (critterId) {
        case CritterId::POOP: return spritePoop;
        case CritterId::CAT: return spriteCat;
        case CritterId::DOG: return spriteDog;
        case CritterId::FROG: return spriteFrog;
        case CritterId::OWL: return spriteOwl;
        case CritterId::FOX: return spriteFox;
        case CritterId::BUNNY: return spriteBunny;
        case CritterId::NARWHAL: return spriteNarwhal;
        case CritterId::WHALE: return spriteWhale;
        case CritterId::OCTOPUS: return spriteOctopus;
        case CritterId::TURTLE: return spriteTurtle;
        case CritterId::MONKEY: return spriteMonkey;
        case CritterId::MARIO: return spriteMario;
        case CritterId::LUIGI: return spriteLuigi;
        case CritterId::PEACH: return spritePeach;
        case CritterId::TOAD: return spriteToad;
        case CritterId::YOSHI: return spriteYoshi;
        case CritterId::ELSA: return spriteElsa;
        case CritterId::ANNA: return spriteAnna;
        case CritterId::OLAF: return spriteOlaf;
        case CritterId::PUMPKIN: return spritePumpkin;
        case CritterId::GHOST: return spriteGhost;
        case CritterId::BAT: return spriteBat;
        case CritterId::WITCH: return spriteWitch;
        case CritterId::TURKEY: return spriteTurkey;
        case CritterId::MERMAID: return spriteMermaid;
        case CritterId::DINOSAUR: return spriteDinosaur;
        case CritterId::BUTTERFLY: return spriteButterfly;
        case CritterId::BEE: return spriteBee;
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
