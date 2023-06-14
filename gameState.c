#include <stdio.h>
#include "include/gameState.h"

void borderPlayfield(Cell playfield[FIELD_H][FIELD_W])
{
    for (int i = 0; i < FIELD_W; i++)
    {
        playfield[i][0].type = WALL;
        playfield[i][FIELD_H - 1].type = WALL;
    }
    
    for (int i = 0; i < FIELD_H; i++)
    {
        playfield[0][i].type = WALL;
        playfield[FIELD_W - 1][i].type = WALL;
    }
}

void initPlayfield(Cell playfield[FIELD_H][FIELD_W]){
    // Fill with air
    for (int col = 0; col < FIELD_W; col++)
    {
        for (int row = 0; row < FIELD_H; row++)
        {
            playfield[row][col].type = AIR;
            playfield[row][col].health = 100;
        }
    }

    borderPlayfield(playfield);
    // for (int col = 0; col < FIELD_W; col++)
    // {
    //     for (int row = 0; row < FIELD_H; row++)
    //     {
    //         // todo
    //     }
    // }
}

void initGameState(GameState* state)
{
    initPlayfield(state->playfield);
}