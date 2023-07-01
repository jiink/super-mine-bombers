#include <stdbool.h>
#include <stdio.h>
#include "include/raylib.h"
#include "include/roundState.h"
#include "include/gameRender.h"
#include "include/inputState.h"

#define ENABLE_MSAA false

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    RoundState state;
    InputState inputState;
    Bindings bindings;

    initRoundState(&state);
    initInputState(&inputState, &bindings);
    initGameRender(screenWidth, screenHeight);

    if (ENABLE_MSAA)
    {
        SetConfigFlags(FLAG_MSAA_4X_HINT);
    }
    InitWindow(screenWidth, screenHeight, "Super Mine Bombers");
	SetWindowState(FLAG_VSYNC_HINT);
    while (!WindowShouldClose())
    {
        updateInputState(&inputState, &bindings);
        updateRoundState(&state, &inputState);
        drawRoundState(&state, &inputState);

		// If R is pressed, restart by initializing the game state again
		if (IsKeyPressed(KEY_R))
		{
			initRoundState(&state);
		}
    }

    CloseWindow();

    return 0;
}