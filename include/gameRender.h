#ifndef GAMERENDER_H
#define GAMERENDER_H

#include "raylib.h"
#include "gameState.h"
#include "inputState.h"

Color cellColorLookup[NUM_CELL_TYPES];

void initGameRender(int screenWidth, int screenHeight);
void initCellColorLookup();
void drawGameState(GameState *gameState, InputState *input);
void drawPlayfield(Cell playfield[FIELD_H][FIELD_W]);
void drawPlayers(Player players[MAX_PLAYERS]);
void drawPlayer(Player *player);

#endif // GAMERENDER_H