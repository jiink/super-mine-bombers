#include <stdio.h>
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

void updateGameState(GameState* state, InputState* input)
{
	// Player control (WASD)
	const float speed = 10.0f;
	state->players[0].position = Vector2Add(state->players[0].position, Vector2Scale(input->direction, speed * GetFrameTime()));
    // if (IsKeyDown(KEY_W)) state->players[0].position.y -= speed * GetFrameTime();
	// if (IsKeyDown(KEY_S)) state->players[0].position.y += speed * GetFrameTime();
	// if (IsKeyDown(KEY_A)) state->players[0].position.x -= speed * GetFrameTime();
	// if (IsKeyDown(KEY_D)) state->players[0].position.x += speed * GetFrameTime();

}