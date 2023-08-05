#include "matchRender.h"
#include "roundState.h"

static void drawBuyingPhase(const MatchState* matchState)
{
    // Draw the background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    // Draw the players
    for (int i = 0; i < matchState->numPlayers; i++)
    {
        DrawRectangle(i * GetScreenWidth() / matchState->numPlayers, 0, GetScreenWidth() / matchState->numPlayers, GetScreenHeight(), playerColors[i]);
    }
    // Draw the wallets
    for (int i = 0; i < matchState->numPlayers; i++)
    {
        DrawText(TextFormat("Wallet: %d", matchState->wallets[i]), i * GetScreenWidth() / matchState->numPlayers + 10, 10, 20, WHITE);
    }
    // Draw the shopping carts
    for (int i = 0; i < matchState->numPlayers; i++)
    {
        for (int j = 0; j < INVENTORY_SIZE; j++)
        {
            if (matchState->shoppingCarts[i].orders[j].type != MAX_WEAPON_TYPE)
            {
                DrawText(TextFormat("%d %s", matchState->shoppingCarts[i].orders[j].quantity, getWeaponName(matchState->shoppingCarts[i].orders[j].type)), i * GetScreenWidth() / matchState->numPlayers + 10, 40 + j * 20, 20, WHITE);
            }
        }
    }
    // Draw the ready states
    for (int i = 0; i < matchState->numPlayers; i++)
    {
        if (matchState->shopperStates[i].ready)
        {
            DrawText("Ready", i * GetScreenWidth() / matchState->numPlayers + 10, GetScreenHeight() - 30, 20, WHITE);
        }
        else
        {
            DrawText("Not ready", i * GetScreenWidth() / matchState->numPlayers + 10, GetScreenHeight() - 30, 20, WHITE);
        }
    }
    // Draw the chosen weapons
    for (int i = 0; i < matchState->numPlayers; i++)
    {
        DrawText(TextFormat("Chosen weapon: %s", getWeaponName(matchState->shopperStates[i].chosenWeapon)), i * GetScreenWidth() / matchState->numPlayers + 10, GetScreenHeight() - 60, 20, WHITE);
    }
}

void drawMatchState(const MatchState* matchState, const InputState* input)
{
    BeginDrawing();
        switch (matchState->phase)
        {
            case BUYING:
                drawBuyingPhase(matchState);
                break;
            case FIGHTING:
                drawRoundState(&matchState->roundState, input);
                break;
        }
        // Draw the round number in the middle
        DrawText(TextFormat("Round %d", matchState->roundNumber), GetScreenWidth() / 2 - 50, 10, 20, GREEN);
    EndDrawing();
}