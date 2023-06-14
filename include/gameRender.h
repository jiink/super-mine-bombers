#ifndef GAMERENDER_H
#define GAMERENDER_H

#include "raylib.h"
#include "gameState.h"

Color cellColorLookup[NUM_CELL_TYPES];

void initGameRender();
void initCellColorLookup();
void drawGameState(GameState *gameState);
void drawPlayfield(Cell playfield[FIELD_H][FIELD_W]);

#endif // GAMERENDER_H