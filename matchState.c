#include <stdio.h>
#include "matchState.h"

// Returns true if item was successfully bought. False if there was no room.
bool buyItem(ShoppingCart* shoppingCart, WeaponType type, int quantity)
{
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (shoppingCart->orders[i].type == type)
        {
            shoppingCart->orders[i].quantity += quantity;
            return true;
        }
    }
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (shoppingCart->orders[i].type == MAX_WEAPON_TYPE)
        {
            shoppingCart->orders[i].type = type;
            shoppingCart->orders[i].quantity = quantity;
            return true;
        }
    }
    return false;
}

void clearShoppingCarts(ShoppingCart shoppingCarts[MAX_PLAYERS])
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for (int j = 0; j < INVENTORY_SIZE; j++)
        {
            shoppingCarts[i].orders[j].type = MAX_WEAPON_TYPE;
            shoppingCarts[i].orders[j].quantity = 0;
        }
    }
}

void initMatchState(MatchState *matchState)
{
    matchState->roundNumber = 0;
    clearShoppingCarts(matchState->shoppingCarts);
    buyItem(&matchState->shoppingCarts[0], BOMB, 1);
    buyItem(&matchState->shoppingCarts[1], MINE, 1);
    initRoundState(&matchState->roundState);
    // Add bought items to the players' inventories
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for (int j = 0; j < INVENTORY_SIZE; j++)
        {
            if (matchState->shoppingCarts[i].orders[j].type != MAX_WEAPON_TYPE)
            {
                //matchState->roundState.players[i].inventory[j].type = matchState->shoppingCarts[i].orders[j].type;
                //matchState->roundState.players[i].inventory[j].quantity = matchState->shoppingCarts[i].orders[j].quantity;
                printf("Giving %d to player %d\n", matchState->shoppingCarts[i].orders[j].type, i);
                if (!giveItem(&matchState->roundState.players[i], matchState->shoppingCarts[i].orders[j].type, matchState->shoppingCarts[i].orders[j].quantity))
                {
                    printf("Player %d could not buy %d\n", i, matchState->shoppingCarts[i].orders[j].type);
                }
            }
        }
    }
}

void updateMatchState(MatchState *matchState, InputState *inputState)
{
    if (matchState->roundState.roundOver)
    {
        matchState->roundNumber++;
        printf("Starting round %d!\n", matchState->roundNumber);
        initRoundState(&matchState->roundState);
    }
    else
    {
        updateRoundState(&matchState->roundState, inputState);
    }
}
