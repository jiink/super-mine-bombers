#ifndef MATCHSTATE_H
#define MATCHSTATE_H

#include "roundState.h"

typedef struct {
    RoundState roundState;
    int roundNumber;
} MatchState;

void initMatchState(MatchState* matchState);
void updateMatchState(MatchState* matchState, InputState* inputState);

#endif // MATCHSTATE_H