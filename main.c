#include <stdbool.h>
#include "include/raylib.h"
#include "include/gameState.h"
#include "include/gameRender.h"
#include "include/inputState.h"

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    GameState state;
    InputState inputState;
	Bindings bindings;

    initGameState(&state);
    initInputState(&inputState, &bindings);
    initGameRender(screenWidth, screenHeight);
    
    InitWindow(screenWidth, screenHeight, "Super Mine Bombers");

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        updateInputState(&inputState, &bindings);
        updateGameState(&state, &inputState);
        drawGameState(&state, &inputState);
    }

    CloseWindow();

    return 0;
}