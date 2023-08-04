#include <stdio.h>
#include "matchState.h"

// Local functions
static void fillWallets(int wallets[MAX_PLAYERS], int amount);
static bool buyItem(ShoppingCart* shoppingCart, int* wallet, WeaponType type, int quantity);
static void clearShoppingCarts(ShoppingCart shoppingCarts[MAX_PLAYERS]);

void initMatchState(MatchState *matchState)
{
    matchState->roundNumber = 0;
    clearShoppingCarts(matchState->shoppingCarts);
    fillWallets(matchState->wallets, 50);
    buyItem(&matchState->shoppingCarts[0], &matchState->wallets[0], BOMB, 15);
    buyItem(&matchState->shoppingCarts[0], &matchState->wallets[0], MINE, 5);
    buyItem(&matchState->shoppingCarts[0], &matchState->wallets[0], SHARP_BOMB, 6);
    buyItem(&matchState->shoppingCarts[1], &matchState->wallets[1], BOMB, 15);
    buyItem(&matchState->shoppingCarts[1], &matchState->wallets[1], MINE, 5);
    buyItem(&matchState->shoppingCarts[1], &matchState->wallets[1], SHARP_BOMB, 6);
    initRoundState(&matchState->roundState);
    // Add bought items to the players' inventories
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for (int j = 0; j < INVENTORY_SIZE; j++)
        {
            if (matchState->shoppingCarts[i].orders[j].type != MAX_WEAPON_TYPE)
            {
                if (!giveItem(&matchState->roundState.players[i], matchState->shoppingCarts[i].orders[j].type, matchState->shoppingCarts[i].orders[j].quantity))
                {
                    printf("Player %d could not buy %d\n", i, matchState->shoppingCarts[i].orders[j].type);
                }
            }
        }
    }
}

void updateMatchState(MatchState *matchState, const InputState *inputState)
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

static void fillWallets(int wallets[MAX_PLAYERS], int amount)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        wallets[i] = amount;
    }
}

// Returns true if item was successfully bought. False if there was no room.
static bool buyItem(ShoppingCart* shoppingCart, int* wallet, WeaponType type, int quantity)
{
    if (quantity <= 0       // you want nothing
    || type < 0 || type >= MAX_WEAPON_TYPE  // you want something we don't have
    || shoppingCart == NULL // you don't have a shopping cart
    || wallet == NULL       // you don't even have a wallet
    || *wallet <= 0         // you have no money
    || *wallet < getWeaponProperties(type).price * quantity) // you can't afford it
    {
        printf("Get out of my store!\n");
        return false;
    }
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (shoppingCart->orders[i].type == type)
        {
            shoppingCart->orders[i].quantity += quantity;
            *wallet -= quantity;
            return true;
        }
    }
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (shoppingCart->orders[i].type == MAX_WEAPON_TYPE)
        {
            shoppingCart->orders[i].type = type;
            shoppingCart->orders[i].quantity = quantity;
            *wallet -= quantity;
            return true;
        }
    }
    return false;
}

static void clearShoppingCarts(ShoppingCart shoppingCarts[MAX_PLAYERS])
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
