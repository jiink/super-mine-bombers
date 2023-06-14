#include "include/inputState.h"
#include "include/raymath.h"

void initInputState(InputState *inputState)
{
    inputState->direction.x = 0;
    inputState->direction.y = 0;
    inputState->attack = false;
}

void updateInputState(InputState *inputState)
{
    inputState->direction.x = 0;
    inputState->direction.y = 0;
    inputState->attack = false;

    if (IsKeyDown(KEY_W))
    {
        inputState->direction.y = -1;
    }
    if (IsKeyDown(KEY_S))
    {
        inputState->direction.y = 1;
    }
    if (IsKeyDown(KEY_A))
    {
        inputState->direction.x = -1;
    }
    if (IsKeyDown(KEY_D))
    {
        inputState->direction.x = 1;
    }
    
    // Normalize the direction
    inputState->direction = Vector2Normalize(inputState->direction);

    if (IsKeyDown(KEY_SPACE))
    {
        inputState->attack = true;
    }

    
}
