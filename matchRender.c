#include "matchRender.h"
#include "roundState.h"
#include <stdio.h>

#define MUSIC_ENABLE false

Music shopMusic;
bool shopMusicPause = true;
Music fightMusic;
bool fightMusicPause = true;

static void drawBuyingPhase(const MatchState* matchState)
{
    // Draw the background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    // Draw the players
    for (int i = 0; i < matchState->numPlayers; i++)
    {
        DrawRectangle(i * GetScreenWidth() / matchState->numPlayers, 0, GetScreenWidth() / matchState->numPlayers, GetScreenHeight(), ColorBrightness(playerColors[i], -0.5f));
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
            DrawText("I'm ready!", i * GetScreenWidth() / matchState->numPlayers + 10, GetScreenHeight() - 30, 20, WHITE);
        }
        else
        {
            // Draw more aggressive ready prompt if everyone else is ready but not this person
            bool onlyOneNotReady = true;
            for (int j = 0; j < matchState->numPlayers; j++)
            {
                if (j != i && !matchState->shopperStates[j].ready)
                {
                    onlyOneNotReady = false;
                }
            }
            if (onlyOneNotReady)
            {
                if ((int)(GetTime() * 8) % 2 == 0)
                {
                    DrawText("PRESS X READY UP!!!", i * GetScreenWidth() / matchState->numPlayers + 10, GetScreenHeight() - 30, 20, WHITE);
                }
            }
            else
            {
                DrawText("Press X to ready.", i * GetScreenWidth() / matchState->numPlayers + 10, GetScreenHeight() - 30, 20, WHITE);
            }
        }
    
        // Draw the chosen weapon with its price
        DrawText(TextFormat("Chosen weapon: %s ($%d)", getWeaponName(shopper->chosenWeapon), getWeaponProperties(shopper->chosenWeapon).price), i * GetScreenWidth() / matchState->numPlayers + 10, GetScreenHeight() - 60, 20, WHITE);
    
    }
    
}

void initMatchRender()
{
    shopMusic = LoadMusicStream("assets/music/mine_bombers.ogg");
    fightMusic = LoadMusicStream("assets/music/grenade_swing.ogg");
    PlayMusicStream(shopMusic);
    //printf(">>>>>>>>>>>>>>>>>>>> initMatchRender\n");
    //PauseMusicStream(shopMusic);
    PlayMusicStream(fightMusic);
    PauseMusicStream(fightMusic);
}

void drawMatchState(const MatchState* matchState, const InputState* input)
{
    if (MUSIC_ENABLE)
    {
        UpdateMusicStream(shopMusic);
        UpdateMusicStream(fightMusic);
    }
    if (matchState->startShopMusic)
    {
        PauseMusicStream(fightMusic);
        ResumeMusicStream(shopMusic);
    }
    if (matchState->startFightMusic)
    {
        PauseMusicStream(shopMusic);
        ResumeMusicStream(fightMusic);
    }
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