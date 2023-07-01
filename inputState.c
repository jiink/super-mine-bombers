#include "include/inputState.h"
#include "include/raymath.h"
#include "include/roundState.h"

void initPlayerInputState(PlayerInputState *pInput)
{
    pInput->direction.x = 0;
    pInput->direction.y = 0;
    pInput->attack = false;
    pInput->attackPressed = false;
    pInput->wepSelectPressed = false;
}

void initPlayerBindings(PlayerBindings* pBindings, int playerNum)
{
    switch (playerNum)
    {
    case 0:
        pBindings->bindings[UP].key = KEY_W;
        pBindings->bindings[DOWN].key = KEY_S;
        pBindings->bindings[LEFT].key = KEY_A;
        pBindings->bindings[RIGHT].key = KEY_D;
        pBindings->bindings[ATTACK].key = KEY_E;
        pBindings->bindings[WEP_SELECT].key = KEY_Q;
        break;
    case 1:
        pBindings->bindings[UP].key = KEY_I;
        pBindings->bindings[DOWN].key = KEY_K;
        pBindings->bindings[LEFT].key = KEY_J;
        pBindings->bindings[RIGHT].key = KEY_L;
        pBindings->bindings[ATTACK].key = KEY_O;
        pBindings->bindings[WEP_SELECT].key = KEY_U;
        break;
    default:
        pBindings->bindings[UP].key = KEY_NULL;
        pBindings->bindings[DOWN].key = KEY_NULL;
        pBindings->bindings[LEFT].key = KEY_NULL;
        pBindings->bindings[RIGHT].key = KEY_NULL;
        pBindings->bindings[ATTACK].key = KEY_NULL;
        pBindings->bindings[WEP_SELECT].key = KEY_NULL;
        break;
    }
}

void initInputState(InputState* input, Bindings* bindings)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        PlayerInputState* pInput = &input->player[i];
        PlayerBindings* pBindings = &bindings->player[i];
        initPlayerInputState(pInput);
        initPlayerBindings(pBindings, i);
    }
}

void updatePlayerInputState(PlayerInputState* pInput, PlayerBindings* pBindings, int gamepadNum)
{
    initPlayerInputState(pInput);

    // Movement
    if (IsKeyDown(pBindings->bindings[UP].key))
    {
        pInput->direction.y -= 1;
    }
    if (IsKeyDown(pBindings->bindings[DOWN].key))
    {
        pInput->direction.y += 1;
    }
    if (IsKeyDown(pBindings->bindings[LEFT].key))
    {
        pInput->direction.x -= 1;
    }
    if (IsKeyDown(pBindings->bindings[RIGHT].key))
    {
        pInput->direction.x += 1;
    }
    if (IsGamepadAvailable(gamepadNum))
    {
        Vector2 stickRawInput = {
            GetGamepadAxisMovement(gamepadNum, GAMEPAD_AXIS_LEFT_X),
            GetGamepadAxisMovement(gamepadNum, GAMEPAD_AXIS_LEFT_Y)
            };
        if (Vector2Length(stickRawInput) > DEADZONE)
        {
            pInput->direction.x += GetGamepadAxisMovement(gamepadNum, GAMEPAD_AXIS_LEFT_X);
            pInput->direction.y += GetGamepadAxisMovement(gamepadNum, GAMEPAD_AXIS_LEFT_Y);
        }
    }
    pInput->direction = Vector2Normalize(pInput->direction);

    // Buttons
    if (IsKeyDown(pBindings->bindings[ATTACK].key))
    {
        pInput->attack = true;
    }
    if (IsKeyPressed(pBindings->bindings[ATTACK].key))
    {
        pInput->attackPressed = true;
    }
    if (IsGamepadAvailable(gamepadNum))
    {
        pInput->attack = pInput->attack || IsGamepadButtonDown(gamepadNum, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
        pInput->attackPressed = pInput->attackPressed || IsGamepadButtonPressed(gamepadNum, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    }
    if (IsKeyPressed(pBindings->bindings[WEP_SELECT].key))
    {
        pInput->wepSelectPressed = true;
    }
}

void updateInputState(InputState *input, Bindings* bindings)
{
    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        PlayerInputState* pInput = &input->player[i];
        PlayerBindings* pBindings = &bindings->player[i];
        updatePlayerInputState(pInput, pBindings, i);
    }
}
