#ifndef ROUNDRENDER_H
#define ROUNDRENDER_H

#include "raylib.h"
#include "roundState.h"
#include "inputState.h"

void initRoundRender(int screenWidth, int screenHeight);
void drawRoundState(const RoundState* roundState, const InputState* input);

#endif // ROUNDRENDER_H