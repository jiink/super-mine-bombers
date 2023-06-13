#include "include/gameRender.h"

void initCellColorLookup()
{
    cellColorLookup[AIR] = ColorAlpha(RED, 0.5);
    cellColorLookup[DIRT] = BROWN;
    cellColorLookup[STONE] = BLACK;
    cellColorLookup[TREASURE] = YELLOW;
    cellColorLookup[WALL] = BLUE;
}

void drawPlayfield(Cell playfield[FIELD_W][FIELD_H])
{
    for (int col = 0; col < FIELD_W; col++)
    {
        for (int row = 0; row < FIELD_H; row++)
        {
            printf("%02X", playfield[col][row].type);
            // CellType thisCell = playfield[row][col].type;
            // if (thisCell == AIR) continue;
            // const int size = 10;
            // const int padding = 1;
            // Vector2 pos = { col * (size + padding), row * (size + padding) };
            // Vector2 tileSize = { size, size };
            // Color color = cellColorLookup[thisCell];
            // DrawRectangleV(pos, tileSize, color);
        }
        printf("\n");
    }
    //DrawCircle(100, 100, 50, BLUE);
}