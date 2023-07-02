#ifndef ROUNDRENDER_H
#define ROUNDRENDER_H

#include "raylib.h"
#include "roundState.h"
#include "inputState.h"

void initGameRender(int screenWidth, int screenHeight);
void initCellColorLookup();
void drawRoundState(RoundState *roundState, InputState *input);
void drawPlayfield(Cell playfield[FIELD_H][FIELD_W]);
void drawPlayers(Player players[MAX_PLAYERS]);
void drawPlayer(Player *player);

#endif // ROUNDRENDER_H