#include "matchRender.h"

void drawMatchState(const MatchState* matchState, const InputState* input)
{
    BeginDrawing();
        drawRoundState(&matchState->roundState, input);
        // Draw the round number in the middle
        DrawText(TextFormat("Round %d", matchState->roundNumber), GetScreenWidth() / 2 - 50, 10, 20, GREEN);
    EndDrawing();
}