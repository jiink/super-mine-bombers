#include <stdio.h>
#include <math.h>
#include "matchState.h"

#define ROUND_OVER_WAIT_TIME 3.0
#define NUM_PLAYERS 3

// Local functions
static void fillAllWallets(ShopperState shopperStates[MAX_PLAYERS], int amount);
static void fillWallet(int* wallet, int amount);
static bool buyItem(ShoppingCart* shoppingCart, int* wallet, WeaponType type, int quantity);
static void checkoutAll(ShopperState shopperStates[MAX_PLAYERS], Player players[MAX_PLAYERS]);
static void clearShoppingCart(ShoppingCart* shoppingCart);

// Add bought items to the players' inventories
static void checkoutAll(ShopperState shopperStates[MAX_PLAYERS], Player players[MAX_PLAYERS])
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for (int j = 0; j < INVENTORY_SIZE; j++)
        {
            ShoppingCart* cart = &shopperStates[i].shoppingCart;
            if (cart->orders[j].type != MAX_WEAPON_TYPE)
            {
                if (!giveItem(&players[i], cart->orders[j].type, cart->orders[j].quantity))
                {
                    printf("Player %d could not buy %d\n", i, cart->orders[j].type);
                }
            }
        }
    }
}

void initShopperStates(ShopperState shopperStates[MAX_PLAYERS], bool clearCarts)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        shopperStates[i].ready = false;
        shopperStates[i].chosenWeapon = BOMB;
        if (clearCarts)
        {
            clearShoppingCart(&shopperStates[i].shoppingCart);
        }
    }
}

void initMatchState(MatchState *matchState)
{
    matchState->phase = BUYING;
    matchState->roundNumber = 0;
    matchState->numPlayers = NUM_PLAYERS;
    matchState->roundOverTimer = 0.0;
    matchState->startShopMusic = false;
    matchState->startFightMusic = false;
    initShopperStates(matchState->shopperStates, true);
    fillAllWallets(matchState->shopperStates, 20);

    // Automatic buying for the first round to quicken debugging
    for (int i = 0; i < matchState->numPlayers; i++)
    {
        // buyItem(&matchState->shopperStates[i].shoppingCart, &matchState->shopperStates[i].wallet, BOMB, 10);
        matchState->shopperStates[i].ready = true;
    }
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

static void getPointersToWallets(const ShopperState shoppers[MAX_PLAYERS], int* pointersToWallets[MAX_PLAYERS])
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        pointersToWallets[i] = &shoppers[i].wallet;
    }
}

void updateBuyingPhase(MatchState* matchState, const InputState* inputState)
{
    for (int i = 0; i < matchState->numPlayers; i++)
    {
        ShopperState* shopperState = &matchState->shopperStates[i];
        if (inputState->player[i].rightPressed)
        {
            shopperState->chosenWeapon++;
            if (shopperState->chosenWeapon > MAX_WEAPON_TYPE - 1)
            {
                shopperState->chosenWeapon = 1;
            }
        }
        if (inputState->player[i].leftPressed)
        {
            shopperState->chosenWeapon--;
            if (shopperState->chosenWeapon < 1)
            {
                shopperState->chosenWeapon = MAX_WEAPON_TYPE - 1;
            }
        }
        if (inputState->player[i].wepSelectPressed)
        {
            shopperState->ready = !shopperState->ready;
        }
        if (inputState->player[i].attackPressed)
        {
            if (buyItem(&shopperState->shoppingCart, &shopperState->wallet, shopperState->chosenWeapon, 1))
            {
                printf("Player %d bought %d\n", i, shopperState->chosenWeapon);
            }
            else
            {
                printf("Player %d could not buy %d\n", i, shopperState->chosenWeapon);
            }
        }
    }
    if (allShoppersAreReady(matchState->shopperStates, matchState->numPlayers))
    {
        matchState->phase = FIGHTING;
        matchState->startFightMusic = true;
        int* pointersToWallets[MAX_PLAYERS];
        getPointersToWallets(matchState->shopperStates, pointersToWallets);
        initRoundState(&matchState->roundState, matchState->numPlayers, pointersToWallets);
        checkoutAll(matchState->shopperStates, matchState->roundState.players);
    }
}

static double TIMER_diff(double a, double b)
{
    return fabs(a - b);
}

void updateFightingPhase(MatchState* matchState, const InputState* inputState)
{
    if (matchState->roundState.roundOver)
    {
        if (TIMER_diff(matchState->roundOverTimer, GetTime()) > ROUND_OVER_WAIT_TIME)
        {
            matchState->roundNumber++;
            printf("Starting round %d!\n", matchState->roundNumber);
            matchState->phase = BUYING;
            matchState->startShopMusic = true;
            initShopperStates(matchState->shopperStates, false);
        }
    }
    else
    {
        updateRoundState(&matchState->roundState, inputState);
        if (matchState->roundState.roundOver)
        {
            matchState->roundOverTimer = GetTime();
            printf("Now waiting a bit\n");
        }
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
    matchState->startShopMusic = false;
    matchState->startFightMusic = false;
    switch (matchState->phase)
    {
        case BUYING:
            //letPlayersJoin(matchState, inputState);
            updateBuyingPhase(matchState, inputState);
            break;
        case FIGHTING:
            updateFightingPhase(matchState, inputState);
            break;
        default:
            break;
    }
}

static void fillWallet(int* wallet, int amount)
{
    if (wallet != NULL)
    {
        *wallet = amount;
    }
}

static void fillAllWallets(ShopperState shopperStates[MAX_PLAYERS], int amount)
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        fillWallet(&shopperStates[i].wallet, amount);
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
            *wallet -= quantity * getWeaponProperties(type).price;
            return true;
        }
    }
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (shoppingCart->orders[i].type == MAX_WEAPON_TYPE)
        {
            shoppingCart->orders[i].type = type;
            shoppingCart->orders[i].quantity = quantity;
            *wallet -= quantity * getWeaponProperties(type).price;
            return true;
        }
    }
    return false;
}

static void clearShoppingCart(ShoppingCart* shoppingCart)
{
    for (int j = 0; j < INVENTORY_SIZE; j++)
    {
        shoppingCart->orders[j].type = MAX_WEAPON_TYPE;
        shoppingCart->orders[j].quantity = 0;
    }
}
