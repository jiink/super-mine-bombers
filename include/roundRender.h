#ifndef ROUNDRENDER_H
#define ROUNDRENDER_H

#include "raylib.h"
#include "roundState.h"
#include "inputState.h"

void initCamera(int screenWidth, int screenHeight);
void drawRoundState(RoundState *roundState, InputState *input);

#endif // ROUNDRENDER_H