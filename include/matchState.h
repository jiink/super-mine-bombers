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
    ShoppingCart shoppingCart;
    int wallet;
} ShopperState;

typedef struct {
    RoundState roundState;
    Phase phase;
    int roundNumber;
    ShopperState shopperStates[MAX_PLAYERS];
    int numPlayers;
    float roundOverTimer;
    bool startShopMusic;
    bool startFightMusic;
} MatchState;

void initMatchState(MatchState* matchState);
void updateMatchState(MatchState* matchState, const InputState* inputState);

#endif // MATCHSTATE_H