#include <stdbool.h>
#include <stdio.h>
#include "include/raylib.h"
#include "include/roundState.h"
#include "include/matchState.h"
#include "include/matchRender.h"
#include "include/inputState.h"

#define ENABLE_MSAA false

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    MatchState matchState;
    InputState inputState;
    Bindings bindings;

    initMatchState(&matchState);
    initInputState(&inputState, &bindings);

    if (ENABLE_MSAA)
    {
        SetConfigFlags(FLAG_MSAA_4X_HINT);
    }
    InitWindow(screenWidth, screenHeight, "Super Mine Bombers");
	SetWindowState(FLAG_VSYNC_HINT);
    
    initRoundRender(screenWidth, screenHeight);
    while (!WindowShouldClose())
    {
        updateInputState(&inputState, &bindings);
        updateMatchState(&matchState, &inputState);
        drawMatchState(&matchState, &inputState);
    }

    CloseWindow();

    return 0;
}