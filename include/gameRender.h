#ifndef GAMERENDER_H
#define GAMERENDER_H

#include "raylib.h"
#include "gameState.h"

Color cellColorLookup[NUM_CELL_TYPES];

void initCellColorLookup();
void drawPlayfield(Cell playfield[FIELD_W]);

#endif // GAMERENDER_H