#ifndef MATCHSTATE_H
#define MATCHSTATE_H

#include "roundState.h"

typedef struct {
    WeaponSlot orders[INVENTORY_SIZE];
} ShoppingCart;

typedef struct {
    ShoppingCart shoppingCarts[MAX_PLAYERS]; // you can buy stuff between each round
    RoundState roundState;
    int roundNumber;
} MatchState;

void initMatchState(MatchState* matchState);
void updateMatchState(MatchState* matchState, InputState* inputState);

#endif // MATCHSTATE_H