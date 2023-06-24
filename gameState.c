#include <stdio.h>
#include <stdbool.h>
#include "include/gameState.h"
#include "include/raymath.h"
#include "include/hex.h"
#include "include/gameState.h"
#include "gameState.h"

WeaponProperties weaponProperties[MAX_WEAPONS] = {
    [BOMB] = {
        .startingFuse = 2.0f,
        .damage = 150,
        .radius = 6,
		.detonationFunc = explode,
        },            
    [MINE] = {
        .startingFuse = 5.0f,
        .damage = 5,
        .radius = 5,
		.detonationFunc = explode,
        },             
    [SHARP_BOMB] = {
        .startingFuse = 5.0f,
        .damage = 200,
        .radius = 20,
		.detonationFunc = explode,
        },     
};

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

void explode(Vector2 position, float radius, float damage, Cell playfield[FIELD_H][FIELD_W])
{
    // Get the cell coordinates of the bomb
    Axial bombCell = toCellCoords(position);
    for (int col = (int)(bombCell.q) - radius; col <= (int)(bombCell.q) + radius; col++)
    {
        for (int row = (int)(bombCell.r) - radius; row <= (int)(bombCell.r) + radius; row++)
        {
            if (col >= 0 && col < FIELD_W && row >= 0 && row < FIELD_H)
            {
                int cellDamage = damage - axialDistance((Axial){.q = col, .r = row}, bombCell) * 25;
                damageCell(row, col, cellDamage, playfield);
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
        players[i].inventory[0] = (WeaponSlot) { .type = BOMB, .quantity = 20 };
        players[i].activeSlot = 0;
    }

    players[0].active = true;
    players[1].active = true;
}

void damageCell(int row, int col, int damage, Cell playfield[FIELD_H][FIELD_W])
{
    if (damage < 0)
    {
        return;
    }
    playfield[row][col].health -= damage;
    if (playfield[row][col].health <= 0)
    {
        playfield[row][col].type = AIR;
    }
}

void damageCellAtPos(Vector2 pos, int damage, Cell playfield[FIELD_H][FIELD_W])
{
    Axial cell = toCellCoords(pos);
    damageCell(cell.r, cell.q, damage, playfield);
}

WeaponProperties getWeaponProperties(WeaponType type)
{
    return weaponProperties[type];
}

void initBombs(Bomb bombsList[MAX_BOMBS])
{
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        bombsList[i].active = false;
        bombsList[i].fuseTimer = 0;
        bombsList[i].position = (Vector2){0, 0};
    }
}

int getNumPlayers(Player players[MAX_PLAYERS])
{
    int numPlayers = 0;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (players[i].active)
        {
            numPlayers++;
        }
    }
    return numPlayers;
}

void initGameState(GameState *state)
{
    initPlayfield(state->playfield);
    initPlayers(state->players);
    initBombs(state->bombs);
}

void spawnBomb(WeaponType wepType, Vector2 pos, Bomb bombsList[MAX_BOMBS])
{
    // Find an inactive bomb
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        if (!bombsList[i].active)
        {
            // Activate it
            bombsList[i].active = true;
            bombsList[i].position = pos;
            bombsList[i].fuseTimer = getWeaponProperties(wepType).startingFuse;
            bombsList[i].type = wepType;
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
                WeaponProperties props = getWeaponProperties(bombsList[i].type);
                props.detonationFunc(bombsList[i].position, props.radius, props.damage, playfield);
                bombsList[i].active = false;
            }
        }
    }
}

void updatePlayer(GameState* state, int playerNum, PlayerInputState* pInput)
{
    playerNum = clamp(playerNum, 0, MAX_PLAYERS - 1);
    Player* player = &state->players[playerNum];
    // Movement control
    const float speed = 10.0f;
    Vector2 playerPos = player->position;
    Vector2 desiredPosition = Vector2Add(playerPos, Vector2Scale(pInput->direction, speed * GetFrameTime()));
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
    player->position = destination;

    // If you push against a solid cell you start mining it
    Axial pos = toCellCoords(desiredPosition);
    int col = pos.q;
    int row = pos.r;
    const int miningSpeed = 10;
    if (state->playfield[row][col].type != AIR)
    {
        damageCell(row, col, miningSpeed, state->playfield);
    }
    // Attacking
    if (pInput->attackPressed)
    {
        WeaponSlot* slot = &player->inventory[player->activeSlot];
        if (slot->quantity > 0)
        {
            slot->quantity--;
            printf("Using a %d! (%d left)\n", slot->type, slot->quantity);
            spawnBomb(slot->type, player->position, state->bombs);
        }
        
    }
}

void updatePlayers(GameState* state, InputState* input)
{
    for (int i = 0; i < getNumPlayers(state->players); i++)
    {
        updatePlayer(state, i, &input->player[i]);
    }
}

void updateGameState(GameState* state, InputState* input)
{
    updatePlayers(state, input);
    updateBombs(state->bombs, state->playfield);
}