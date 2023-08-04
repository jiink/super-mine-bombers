#ifndef MATCHSTATE_H
#define MATCHSTATE_H

#include "roundState.h"

typedef struct {
    WeaponSlot orders[INVENTORY_SIZE];
} ShoppingCart;

typedef struct {
    ShoppingCart shoppingCarts[MAX_PLAYERS]; // you can buy stuff between each round
    int wallets[MAX_PLAYERS];
    RoundState roundState;
    int roundNumber;
} MatchState;

void initMatchState(MatchState* matchState);
void updateMatchState(MatchState* matchState, const InputState* inputState);

#endif // MATCHSTATE_H