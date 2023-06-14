
#include <stdbool.h>
#include "include/raylib.h"
#include "gameState.h"
#include "gameRender.h"


int main(void)
{
	const int screenWidth = 1280;
    const int screenHeight = 720;

    GameState state;
    initGameState(&state);

	initGameRender(screenWidth, screenHeight);
    
    InitWindow(screenWidth, screenHeight, "Super Mine Bombers");

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
		updateGameState(&state);
        
		drawGameState(&state);
    }

    CloseWindow();

    return 0;
}