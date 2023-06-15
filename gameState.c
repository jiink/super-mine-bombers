#include <stdio.h>
#include <stdbool.h>
#include "include/gameState.h"
#include "include/raymath.h"

typedef struct Cube
{
	float q;
	float r;
	float s;
} Cube;

typedef struct Axial
{
	float q;
	float r;
} Axial;

Cube cubeRound(Cube frac)
{
	float q = round(frac.q);
	float r = round(frac.r);
	float s = round(frac.s);

	float qDiff = fabs(q - frac.q);
	float rDiff = fabs(r - frac.r);
	float sDiff = fabs(s - frac.s);

	if (qDiff > rDiff && qDiff > sDiff)
	{
		q = -r - s;
	}
	else if (rDiff > sDiff)
	{
		r = -q - s;
	}
	else
	{
		s = -q - r;
	}

	return (Cube){q, r, s};
}

Axial cubeToAxial(Cube cube)
{
	float q = cube.q;
	float r = cube.r;
	return (Axial){ q, r };
}

Cube axialToCube(Axial hex)
{
	float q = hex.q;
	float r = hex.r;
	float s = -q - r;
	return (Cube){ q, r, s };
}

Axial axialRound(Axial hex)
{
	return cubeToAxial(cubeRound(axialToCube(hex)));
}

int clamp(int value, int min, int max)
{
	if (value < min)
	{
		return min;
	}
	else if (value > max)
	{
		return max;
	}
	else
	{
		return value;
	}
}

float clampf(float value, float min, float max)
{
	if (value < min)
	{
		return min;
	}
	else if (value > max)
	{
		return max;
	}
	else
	{
		return value;
	}
}

Axial toCellCoords(Vector2 point)
{
	const float size = CELL_H_SPACING * (2.0f / 3.0f);
	float q = point.x * (2.0f/3.0f) / size;
	float r = (-point.x / 3.0f + SQRT_3/3.0f * point.y) / size;
	q = clampf(q, 0, FIELD_W - 1);
	r = clampf(r, 0, FIELD_H - 1);
	return axialRound((Axial){ q, r });
	//printf("Point (%f, %f) is in cell (%d, %d)\n", point.x, point.y, *col, *row);
}

bool isPointInSolidCell(Vector2 point, Cell playfield[FIELD_H][FIELD_W])
{
    // Get the cell coordinates of the point
    int col, row;
	Axial pos = toCellCoords(point);
	col = pos.q;
	row = pos.r;
    
    // Check if the cell is solid
    return playfield[row][col].type != AIR;
}

void DetonateBomb(Vector2 position, float strength, Cell playfield[FIELD_H][FIELD_W])
{
    int bombCellX = (int)position.x;
    int bombCellY = (int)position.y;
    for (int col = bombCellX - strength; col <= bombCellX + strength; col++)
    {
        for (int row = bombCellY - strength; row <= bombCellY + strength; row++)
        {
            if (col >= 0 && col < FIELD_W && row >= 0 && row < FIELD_H)
            {
                if (isPointInSolidCell((Vector2){col, row}, playfield))
                {
                    playfield[row][col].type = AIR;
                }
            }
        }
    }
}

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
		players[i].position.x = 31.5;
		players[i].position.y = 31.5;
		players[i].health = 100;
		players[i].score = 0;
		players[i].active = false;
		players[i].color = playerColors[i];
        players[i].inventory[0] = (WeaponSlot) { .type = BOMB, .quantity = 5 };
        players[i].activeSlot = 0;
	}

	players[0].active = true;
}

void initBombs(Bomb bombsList[MAX_BOMBS])
{
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        bombsList[i].active = false;
        bombsList[i].fuseTimer = 0;
        bombsList[i].position = (Vector2){0, 0};
        bombsList[i].detonationFunc = DetonateBomb;
    }
}

void initGameState(GameState* state)
{
    initPlayfield(state->playfield);
	initPlayers(state->players);
    initBombs(state->bombs);
}

void SpawnBomb(WeaponType wepType, Vector2 pos, Bomb bombsList[MAX_BOMBS])
{
    // Find an inactive bomb
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        if (!bombsList[i].active)
        {
            // Activate it
            bombsList[i].active = true;
            bombsList[i].position = pos;
            bombsList[i].fuseTimer = 3;
            bombsList[i].detonationFunc = &DetonateBomb;
            break;
        }
    }
}

void updateBombs(Bomb bombsList[MAX_BOMBS], Cell playfield[FIELD_H][FIELD_W])
{
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        if (bombsList[i].active)
        {
            bombsList[i].fuseTimer -= GetFrameTime();
            if (bombsList[i].fuseTimer <= 0)
            {
                bombsList[i].detonationFunc(bombsList[i].position, 3, playfield);
                bombsList[i].active = false;
            }
        }
    }
}

void updateGameState(GameState* state, InputState* input)
{
	// Player control (WASD)
	const float speed = 10.0f;
    Vector2 playerPos = state->players[0].position;
    Vector2 desiredPosition = Vector2Add(playerPos, Vector2Scale(input->direction, speed * GetFrameTime()));
	Vector2 destination = desiredPosition;

    // Check if the player (point) is trying to move into a solid cell (rectanlge)
    if (isPointInSolidCell((Vector2){desiredPosition.x, playerPos.y}, state->playfield))
    {
        // If so, don't move horizontally
        destination.x = playerPos.x;
    }
    if (isPointInSolidCell((Vector2){playerPos.x, desiredPosition.y}, state->playfield))
    {
        // If so, don't move vertically
        destination.y = playerPos.y;
    }

    state->players[0].position = destination;

    // If you push against a solid cell you start mining it
	Axial pos = toCellCoords(desiredPosition);
	int col = pos.q;
    int row = pos.r;
    const int miningSpeed = 10;
    if (state->playfield[row][col].type != AIR)
    {
        state->playfield[row][col].health -= miningSpeed;
        if (state->playfield[row][col].health <= 0)
        {
            state->playfield[row][col].type = AIR;
        }
    }

    if (input->attackPressed)
    {
        WeaponSlot* slot = &state->players[0].inventory[state->players[0].activeSlot];
        if (slot->quantity > 0)
        {
            slot->quantity--;
            printf("Using a %d! (%d left)\n", slot->type, slot->quantity);
            SpawnBomb(slot->type, state->players[0].position, state->bombs);
        }
        
    }

    updateBombs(state->bombs, state->playfield);
}