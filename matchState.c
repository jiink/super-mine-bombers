#include <stdio.h>
#include "matchState.h"

// Local functions
static void fillWallets(int wallets[MAX_PLAYERS], int amount);
static bool buyItem(ShoppingCart* shoppingCart, int* wallet, WeaponType type, int quantity);
static void clearShoppingCarts(ShoppingCart shoppingCarts[MAX_PLAYERS]);
static void checkoutAll(ShoppingCart shoppingCarts[MAX_PLAYERS], Player players[MAX_PLAYERS]);

// Add bought items to the players' inventories
static void checkoutAll(ShoppingCart shoppingCarts[MAX_PLAYERS], Player players[MAX_PLAYERS])
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for (int j = 0; j < INVENTORY_SIZE; j++)
        {
            if (shoppingCarts[i].orders[j].type != MAX_WEAPON_TYPE)
            {
                if (!giveItem(&players[i], shoppingCarts[i].orders[j].type, shoppingCarts[i].orders[j].quantity))
                {
                    printf("Player %d could not buy %d\n", i, shoppingCarts[i].orders[j].type);
                }
            }
        }
    }
}

void initShopperStates(ShopperState shopperStates[MAX_PLAYERS])
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        shopperStates[i].ready = false;
        shopperStates[i].chosenWeapon = 0;
    }
}

void initMatchState(MatchState *matchState)
{
    matchState->phase = BUYING;
    matchState->roundNumber = 0;
    matchState->numPlayers = 4;
    initShopperStates(matchState->shopperStates);
    clearShoppingCarts(matchState->shoppingCarts);
    fillWallets(matchState->wallets, 50);
}

static bool allShoppersAreReady(const ShopperState shoppers[MAX_PLAYERS], int numPlayers)
{
    for (int i = 0; i < numPlayers; i++)
    {
        if (!shoppers[i].ready)
        {
            return false;
        }
    }
    return true;
}

void updateBuyingPhase(MatchState* matchState, const InputState* inputState)
{
    for (int i = 0; i < matchState->numPlayers; i++)
    {
        ShopperState* shopperState = &matchState->shopperStates[i];
        if (inputState->player[i].wepSelectPressed)
        {
            shopperState->chosenWeapon++;
            if (shopperState->chosenWeapon > MAX_WEAPON_TYPE)
            {
                shopperState->chosenWeapon = 0;
            }
        }
        if (inputState->player[i].attackPressed)
        {
            if (shopperState->chosenWeapon == MAX_WEAPON_TYPE)
            {
                shopperState->ready = !shopperState->ready;
            }
            else
            {
                if (buyItem(&matchState->shoppingCarts[i], &matchState->wallets[i], shopperState->chosenWeapon, 1))
                {
                    printf("Player %d bought %d\n", i, shopperState->chosenWeapon);
                }
                else
                {
                    printf("Player %d could not buy %d\n", i, shopperState->chosenWeapon);
                }
            }
        }
    }
    if (allShoppersAreReady(matchState->shopperStates, matchState->numPlayers))
    {
        matchState->phase = FIGHTING;
        initRoundState(&matchState->roundState, matchState->numPlayers);
        checkoutAll(matchState->shoppingCarts, matchState->roundState.players);
    }
}

void updateFightingPhase(MatchState* matchState, const InputState* inputState)
{
    if (matchState->roundState.roundOver)
    {
        matchState->roundNumber++;
        printf("Starting round %d!\n", matchState->roundNumber);
        matchState->phase = BUYING;
        initShopperStates(matchState->shopperStates);
        clearShoppingCarts(matchState->shoppingCarts);
    }
    else
    {
        updateRoundState(&matchState->roundState, inputState);
    }
}

// If a new player presses a button, add them to the game
void letPlayersJoin(MatchState* matchState, const InputState* inputState)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        // if (inputState->player[i].attackPressed && )
        // {
        //     matchState->numPlayers++;
        //     matchState->shopperStates[i].ready = false;
        //     matchState->shopperStates[i].chosenWeapon = 0;
        //     matchState->wallets[i] = 50;
        //     printf("Player %d joined the game!\n", i);
        // }
    }
}

void updateMatchState(MatchState* matchState, const InputState* inputState)
{
    switch (matchState->phase)
    {
        case BUYING:
            //letPlayersJoin(matchState, inputState);
            updateBuyingPhase(matchState, inputState);
            break;
        case FIGHTING:
            updateFightingPhase(matchState, inputState);
            break;
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
