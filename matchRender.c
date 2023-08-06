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
        const ShopperState* shopper = &matchState->shopperStates[i];

        // Draw the wallets
        DrawText(TextFormat("Wallet: %d", shopper->wallet), i * GetScreenWidth() / matchState->numPlayers + 10, 10, 20, WHITE);
    
        // Draw the shopping carts
    
        for (int j = 0; j < INVENTORY_SIZE; j++)
        {
            if (shopper->shoppingCart.orders[j].type != MAX_WEAPON_TYPE)
            {
                DrawText(TextFormat("%d %s", shopper->shoppingCart.orders[j].quantity, getWeaponName(shopper->shoppingCart.orders[j].type)), i * GetScreenWidth() / matchState->numPlayers + 10, 40 + j * 20, 20, WHITE);
            }
        }
    
        // Draw the ready states
    
        if (shopper->ready)
        {
            DrawText("Ready", i * GetScreenWidth() / matchState->numPlayers + 10, GetScreenHeight() - 30, 20, WHITE);
        }
        else
        {
            DrawText("Not ready", i * GetScreenWidth() / matchState->numPlayers + 10, GetScreenHeight() - 30, 20, WHITE);
        }
    
        // Draw the chosen weapons
    
        DrawText(TextFormat("Chosen weapon: %s", getWeaponName(shopper->chosenWeapon)), i * GetScreenWidth() / matchState->numPlayers + 10, GetScreenHeight() - 60, 20, WHITE);
    
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
            default:
                break;
        }
        // Draw the round number in the middle
        DrawText(TextFormat("Round %d", matchState->roundNumber), GetScreenWidth() / 2 - 50, 10, 20, GREEN);
    EndDrawing();
}