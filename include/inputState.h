#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include <stdbool.h>
#include "raylib.h"

typedef struct {
    Vector2 direction;
    bool attack;
} InputState;

void initInputState(InputState* inputState);
void updateInputState(InputState* inputState);

#endif // INPUTSTATE_H