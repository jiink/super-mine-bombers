void borderPlayfield(Cell playfield[FIELD_W][FIELD_H])
{
    for (int i = 0; i < FIELD_W; i++)
    {
        playfield[i][0].type = WALL;
        playfield[i][FIELD_H].type = WALL;
    }
    
    for (int i = 0; i < FIELD_H; i++)
    {
        playfield[0][i].type = WALL;
        playfield[FIELD_W][i].type = WALL;
    }
}

void initPlayfield(Cell playfield[FIELD_W][FIELD_H]){
    borderPlayfield();
    for (int col = 0; col < FIELD_W; col++)
    {
        for (int row = 0; row < FIELD_H; row++)
        {
            // todo
        }
    }
}

void initGameState(GameState* state)
{
    initPlayfield(state->playfield);
}