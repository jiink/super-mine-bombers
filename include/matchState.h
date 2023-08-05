#ifndef MATCHSTATE_H
#define MATCHSTATE_H

#include "roundState.h"

typedef enum {
    BUYING,
    FIGHTING,
    NUM_PHASES
} Phase;

typedef struct {
    WeaponSlot orders[INVENTORY_SIZE];
} ShoppingCart;

typedef struct {
    int chosenWeapon;
    bool ready;
} ShopperState;

typedef struct {
    ShoppingCart shoppingCarts[MAX_PLAYERS]; // you can buy stuff between each round
    int wallets[MAX_PLAYERS];
    RoundState roundState;
    Phase phase;
    int roundNumber;
    ShopperState shopperStates[MAX_PLAYERS];
    int numPlayers;
} MatchState;

void initMatchState(MatchState* matchState);
void updateMatchState(MatchState* matchState, const InputState* inputState);

#endif // MATCHSTATE_H