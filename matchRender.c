#include "matchRender.h"

void drawMatchState(MatchState* matchState, InputState* input)
{
    BeginDrawing();
        drawRoundState(&matchState->roundState, input);
        // Draw the round number in the middle
        DrawText(TextFormat("Round %d", matchState->roundNumber), GetScreenWidth() / 2 - 50, 10, 20, GREEN);
    EndDrawing();
}