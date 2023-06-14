#include <stdio.h>
#include <stdbool.h>
#include "include/gameState.h"
#include "include/raymath.h"

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
    // Fill whole field
    for (int col = 0; col < FIELD_W; col++)
    {
        for (int row = 0; row < FIELD_H; row++)
        {
            playfield[row][col].type = DIRT;
            playfield[row][col].health = 100;
        }
    }

    borderPlayfield(playfield);
    
    for (int i = 1; i < 5; i++)
    {
        playfield[1][i].type = AIR;
        playfield[i][1].type = AIR;
    }
}

void initPlayers(Player players[MAX_PLAYERS])
{
	const Color playerColors[MAX_PLAYERS] = { RED, BLUE, GREEN, YELLOW };
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		players[i].position.x = 1.5;
		players[i].position.y = 1.5;
		players[i].health = 100;
		players[i].score = 0;
		players[i].active = false;
		players[i].color = playerColors[i];
	}

	players[0].active = true;
}

void initGameState(GameState* state)
{
    initPlayfield(state->playfield);
	initPlayers(state->players);
}

static bool isPointInSolidCell(Vector2 point, Cell playfield[FIELD_H][FIELD_W])
{
    // Get the cell coordinates of the point
    int col = (int)point.x;
    int row = (int)point.y;
    
    // Check if the cell is solid
    return playfield[row][col].type != AIR;
}

void updateGameState(GameState* state, InputState* input)
{
	// Player control (WASD)
	const float speed = 10.0f;
    Vector2 playerPos = state->players[0].position;
    Vector2 desiredPosition = Vector2Add(playerPos, Vector2Scale(input->direction, speed * GetFrameTime()));
	
    // Check if the player (point) is trying to move into a solid cell (rectanlge)
    if (isPointInSolidCell((Vector2){desiredPosition.x, playerPos.y}, state->playfield))
    {
        // If so, don't move horizontally
        desiredPosition.x = playerPos.x;
    }
    if (isPointInSolidCell((Vector2){playerPos.x, desiredPosition.y}, state->playfield))
    {
        // If so, don't move vertically
        desiredPosition.y = playerPos.y;
    }

    state->players[0].position = desiredPosition;


}