
#include <stdbool.h>
#include "include/raylib.h"
#include "gameState.h"
#include "gameRender.h"


int main(void)
{
    GameState state;
    initGameState(state);
    
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Super Mine Bombers");

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            DrawRectangle(100, 100, 100, 100, RED);
            
            //drawPlayfield(state.playfield);

        EndDrawing();
        
    }

    CloseWindow();

    return 0;
}