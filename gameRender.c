#include "include/gameRender.h"
#include <stdio.h>

void initGameRender()
{
	initCellColorLookup();
}

void initCellColorLookup()
{
    cellColorLookup[AIR] = ColorAlpha(RED, 0.5);
    cellColorLookup[DIRT] = BROWN;
    cellColorLookup[STONE] = BLACK;
    cellColorLookup[TREASURE] = YELLOW;
    cellColorLookup[WALL] = BLUE;
}

void drawPlayfield(Cell playfield[FIELD_H][FIELD_W])
{
    for (int col = 0; col < FIELD_W; col++)
    {
        for (int row = 0; row < FIELD_H; row++)
        {
            CellType thisCell = playfield[row][col].type;
            if (thisCell == AIR) continue;
            const int size = 10;
            const int padding = 1;
            Vector2 pos = { col * (size + padding), row * (size + padding) };
            Vector2 tileSize = { size, size };
            Color color = cellColorLookup[thisCell];
            DrawRectangleV(pos, tileSize, color);
        }
    }
}

void drawGameState(GameState *state)
{
	drawPlayfield(state->playfield);
}