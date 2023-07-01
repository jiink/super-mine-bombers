#include "matchState.h"

void initMatchState(MatchState *matchState)
{
    matchState->roundNumber = 0;
    initRoundState(&matchState->roundState);
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
