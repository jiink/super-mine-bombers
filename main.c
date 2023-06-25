#include <stdbool.h>
#include <stdio.h>
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

	SetWindowState(FLAG_VSYNC_HINT);
    while (!WindowShouldClose())
    {
        updateInputState(&inputState, &bindings);
        updateGameState(&state, &inputState);
        drawGameState(&state, &inputState);

		// If R is pressed, restart by initializing the game state again
		if (IsKeyPressed(KEY_R))
		{
			initGameState(&state);
		}
    }

    CloseWindow();

    return 0;
}