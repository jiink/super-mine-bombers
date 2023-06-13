
#include "raylib.h"

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Super Mine Bombers");

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            DrawRectangle(100, 100, 100, 100, RED);

        EndDrawing();
        
    }

    CloseWindow();

    return 0;
}