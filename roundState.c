#include <stdio.h>
#include <stdbool.h>
#include "include/roundState.h"
#include "include/raymath.h"
#include "include/hex.h"
#include "include/roundState.h"
#include "roundState.h"

Axial playerSpawnPoints[MAX_PLAYERS] = {
	{ 1, 1 },
	{ FIELD_H - 2, FIELD_W - 2 },
	{ 1, 1 },
	{ 1, 1 },
};

// Bomb detonation prototypes

static bool sharpBombDetonate(Vector2 position, float radius, float damage, Cell playfield[FIELD_H][FIELD_W], Player players[MAX_PLAYERS]);
static bool mineTryDetonate(Vector2 position, float radius, float damage, Cell playfield[FIELD_H][FIELD_W], Player players[MAX_PLAYERS]);
static void bombUpdateNothing(Bomb* bomb, const RoundState* roundState);
static bool explode(Vector2 position, float radius, float damage, Cell playfield[FIELD_H][FIELD_W], Player players[MAX_PLAYERS]);

// Properties

WeaponProperties weaponProperties[MAX_CELL_TYPES] = {
    [BOMB] = {
        .startingFuse = 2.0f,
        .damage = 150,
        .radius = 6,
        .price = 1,
		.detonationFunc = explode,
        .updateFunc = bombUpdateNothing,
        },            
    [MINE] = {
        .startingFuse = 5.0f,
        .damage = 200,
        .radius = 10,
        .price = 2,
		.detonationFunc = mineTryDetonate,
        .updateFunc = bombUpdateNothing,
        },             
    [SHARP_BOMB] = {
        .startingFuse = 3.0f,
        .damage = 500,
        .radius = 40,
        .price = 3,
		.detonationFunc = sharpBombDetonate,
        .updateFunc = bombUpdateNothing,
        },     
};

CellProperties cellProperties[MAX_CELL_TYPES] = {
	[AIR] = {
		.resistance = 0.0f,
		.solid = false,
	},
	[DIRT] = {
		.resistance = 1.0f,
		.solid = true,
	},
	[STONE] = {
		.resistance = 2.0f,
		.solid = true,
	},
	[TREASURE] = {
		.resistance = 0.5f,
		.solid = true,
	},
	[WALL] = {
		.resistance = 999999.0f,
		.solid = true,
	},
};

// Local functions

static Vector2 vec2FromAngle(float angle);
static float lerpAngle( float a, float b, float weight );
static int clampInt(int value, int min, int max);
static bool isPointInSolidCell(Vector2 point, Cell playfield[FIELD_H][FIELD_W]);
static void damagePlayer(Player *player, int damage);
static void borderPlayfield(Cell playfield[FIELD_H][FIELD_W]);
static void initPlayfield(Cell playfield[FIELD_H][FIELD_W]);
static void clearInventory(Player* player);
static void initPlayers(Player players[MAX_PLAYERS]);
static void damageCell(int row, int col, int damage, Cell playfield[FIELD_H][FIELD_W]);
static void damageCellAtPos(Vector2 pos, int damage, Cell playfield[FIELD_H][FIELD_W]);
static void initBombs(Bomb bombsList[MAX_BOMBS]);
static void spawnBomb(WeaponType wepType, Vector2 pos, Bomb bombsList[MAX_BOMBS]);
static void updateBombs(Bomb bombsList[MAX_BOMBS], const RoundState* roundState, Cell playfield[FIELD_H][FIELD_W], Player players[MAX_PLAYERS]);
static int getNumInventorySlotsUsed(Player* player);
static void updatePlayer(RoundState* state, int playerNum, const PlayerInputState* pInput);
static void updatePlayers(RoundState* state, const InputState* input);
static bool gameOverCondition(RoundState* state);

// Function definitions

void initRoundState(RoundState *state)
{
    state->roundTime = 30.0f;
    state->roundOver = false;
    initPlayfield(state->playfield);
    initPlayers(state->players);
    initBombs(state->bombs);
}

void updateRoundState(RoundState* state, const InputState* input)
{
    if (gameOverCondition(state))
    {
        state->roundOver = true;
    }
    // Press R to skip round
    if (IsKeyPressed(KEY_R))
    {
        state->roundOver = true;
    }
    updatePlayers(state, input);
    updateBombs(state->bombs, state, state->playfield, state->players);
}

// Returns true if the item was given, false if there was no room
bool giveItem(Player* player, WeaponType type, int amount)
{
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (player->inventory[i].type == type)
        {
            player->inventory[i].quantity += amount;
            return true;
        }
    }
    // If we get here, we didn't find the item in the inventory
    // So we need to add it
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        // Found an empty slot
        if (player->inventory[i].type == MAX_WEAPON_TYPE)
        {
            player->inventory[i].type = type;
            player->inventory[i].quantity = amount;
            return true;
        }
    }
    // No room
    return false;
}

WeaponProperties getWeaponProperties(WeaponType type)
{
    return weaponProperties[type];
}

CellProperties getCellProperties(CellType type)
{
	return cellProperties[type];
}

int getNumAlivePlayers(const Player players[MAX_PLAYERS])
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

const char* getWeaponName(WeaponType type)
{
    switch(type)
    {
        case BOMB:
            return "Bomb";
            break;
        case SHARP_BOMB:
            return "Sharp Bomb";
            break;
        case MINE:
            return "Mine";
            break;
        default:
            return "Unknown";
            break;
    }
}

static Vector2 vec2FromAngle(float angle) //Hopefully returns a vec2 length 1 based off the angle??
{
    Vector2 out = (Vector2){ cosf(angle), sinf(angle) };
    return out;
}

static int clampInt(int value, int min, int max)
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

static bool isPointInSolidCell(Vector2 point, Cell playfield[FIELD_H][FIELD_W])
{
    // Get the cell coordinates of the point
    int col, row;
    Axial pos = toCellCoords(point);
    col = pos.q;
    row = pos.r;
    
    // Check if the cell is solid
    return playfield[row][col].type != AIR;
}

static void damagePlayer(Player *player, int damage)
{
	if (!player->active)
	{
		return;
	}
	player->health -= damage;
	if (player->health <= 0)
	{
		player->active = false;
	}
}

static bool explode(Vector2 position, float radius, float damage, Cell playfield[FIELD_H][FIELD_W], Player players[MAX_PLAYERS])
{
    // Get the cell coordinates of the bomb
    Axial bombCell = toCellCoords(position);
    for (int col = (int)(bombCell.q) - radius; col <= (int)(bombCell.q) + radius; col++)
    {
        for (int row = (int)(bombCell.r) - radius; row <= (int)(bombCell.r) + radius; row++)
        {
            if (col >= 0 && col < FIELD_W && row >= 0 && row < FIELD_H)
            {
				Axial thisCell = (Axial){.q = col, .r = row};
                int cellDamage = damage - axialDistance(thisCell, bombCell) * 25;
                damageCell(row, col, cellDamage, playfield);
				// If there are any players in this cell, damage them too
				for (int i = 0; i < MAX_PLAYERS; i++)
				{
					if (axialDistance(toCellCoords(players[i].position), thisCell) <= 0)
					{
						damagePlayer(&players[i], cellDamage);
					}
				}
            }
        }
    }
    return true;
}

static bool sharpBombDetonate(Vector2 position, float radius, float damage, Cell playfield[FIELD_H][FIELD_W], Player players[MAX_PLAYERS])
{
    // Cause a series of small explosions in a line above and below, to the left and to the right
    // the reach of the line of explosions is determined by the radius
    for (int i = 0; i < radius; i++)
    {
        explode((Vector2){.x = position.x + i * CELL_H_SPACING, .y = position.y}, 1.0f / (float)(i / 2 + 1), damage / (i / 2 + 1), playfield, players);
        explode((Vector2){.x = position.x - i * CELL_H_SPACING, .y = position.y}, 1.0f / (float)(i / 2 + 1), damage / (i / 2 + 1), playfield, players);
        explode((Vector2){.x = position.x, .y = position.y + i * CELL_V_SPACING}, 1.0f / (float)(i / 2 + 1), damage / (i / 2 + 1), playfield, players);
        explode((Vector2){.x = position.x, .y = position.y - i * CELL_V_SPACING}, 1.0f / (float)(i / 2 + 1), damage / (i / 2 + 1), playfield, players);
    }
    return true;
}

static bool mineTryDetonate(Vector2 position, float radius, float damage, Cell playfield[FIELD_H][FIELD_W], Player players[MAX_PLAYERS])
{
    // See if any players are nearby. If so, explode. Otherwise, return false.
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (Vector2Distance(players[i].position, position) < 1)
        {
            explode(position, radius, damage, playfield, players);
            return true;
        }
    }
    return false;
}

static void bombUpdateNothing(Bomb* bomb, const RoundState* roundState)
{
    return;
}

static void borderPlayfield(Cell playfield[FIELD_H][FIELD_W])
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

static void initPlayfield(Cell playfield[FIELD_H][FIELD_W]){
    // Fill whole field
    for (int col = 0; col < FIELD_W; col++)
    {
        for (int row = 0; row < FIELD_H; row++)
        {
			Cell* cell = &playfield[row][col];
			// Make the type random dirt or stone
			if (GetRandomValue(0, 1) == 0)
			{
				cell->type = DIRT;
			}
			else
			{
				cell->type = STONE;
			}
            cell->health = 100;
        }
    }

    borderPlayfield(playfield);
    
    for (int i = 1; i < 5; i++)
    {
        playfield[1][i].type = AIR;
        playfield[i][1].type = AIR;
    }
}

static void clearInventory(Player* player)
{
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        player->inventory[i].type = MAX_WEAPON_TYPE;
        player->inventory[i].quantity = 0;
    }
}

static void initPlayers(Player players[MAX_PLAYERS])
{
    const Color playerColors[MAX_PLAYERS] = { RED, BLUE, GREEN, YELLOW };
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        players[i].health = 100;
        players[i].score = 0;
        players[i].active = false;
        players[i].color = playerColors[i];
        clearInventory(&players[i]);
        players[i].activeSlot = 0;
        players[i].position = toWorldCoords(playerSpawnPoints[i]);
        players[i].velocity = (Vector2) { 0.0f, 0.0f };
        players[i].defSpeed = 200.0f;
        players[i].defFriction = 0.7f;

        players[i].targetSpeed = players[i].defSpeed;
        players[i].friction = players[i].defFriction;
    }

    players[0].active = true;
    players[1].active = true;
}

static void damageCell(int row, int col, int damage, Cell playfield[FIELD_H][FIELD_W])
{
	Cell* cell = &playfield[row][col];
	if (damage < 0
		|| row < 0
		|| row >= FIELD_H
		|| col < 0
		|| col >= FIELD_W
	)
    {
        return;
    }
	CellProperties cellProps = getCellProperties(cell->type);
	if (!cellProps.solid)
	{
		return;
	}
	int appliedDamage = (int)((float)damage / cellProps.resistance);
    cell->health -= appliedDamage;
    if (cell->health <= 0)
    {
        cell->type = AIR;
    }
}

static void damageCellAtPos(Vector2 pos, int damage, Cell playfield[FIELD_H][FIELD_W])
{
    Axial cell = toCellCoords(pos);
    damageCell(cell.r, cell.q, damage, playfield);
}

static void initBombs(Bomb bombsList[MAX_BOMBS])
{
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        bombsList[i].active = false;
        bombsList[i].fuseTimer = 0;
        bombsList[i].position = (Vector2){0, 0};
    }
}

static void spawnBomb(WeaponType wepType, Vector2 pos, Bomb bombsList[MAX_BOMBS])
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

static void updateBombs(Bomb bombsList[MAX_BOMBS], const RoundState* roundState, Cell playfield[FIELD_H][FIELD_W], Player players[MAX_PLAYERS])
{
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        Bomb* bomb = &bombsList[i];
        if (bomb->active)
        {
            WeaponProperties props = getWeaponProperties(bomb->type);
            props.updateFunc(bomb, roundState);
            bomb->fuseTimer -= GetFrameTime();
            if (bomb->fuseTimer <= 0)
            {
                bomb->fuseTimer = 0.0f;
                if (props.detonationFunc(bomb->position, props.radius, props.damage, playfield, players))
                {
                    bomb->active = false;
                }
            }
        }
    }
}

static int getNumInventorySlotsUsed(Player* player)
{
    int numUsed = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (player->inventory[i].quantity > 0)
        {
            numUsed++;
        }
    }
    return numUsed;
}

static void updatePlayer(RoundState* state, int playerNum, const PlayerInputState* pInput)
{
    playerNum = clampInt(playerNum, 0, MAX_PLAYERS - 1);
    Player* player = &state->players[playerNum];

    // Movement control
    Vector2 playerPos = player->position;
    player->velocity = Vector2Add(player->velocity, Vector2Scale(pInput->direction, player->defSpeed * GetFrameTime()));
    player->velocity = Vector2Scale(player->velocity, player->friction);

    Vector2 desiredPosition = Vector2Add(playerPos, Vector2Scale(player->velocity, GetFrameTime()));
    Vector2 destination = desiredPosition;

    // Check if the player (point) is trying to move into a solid cell (rectanlge)
    if (isPointInSolidCell(desiredPosition, state->playfield))
    {
        player->velocity = (Vector2) { 0.0f, 0.0f };
    }

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
    const int miningSpeed = 600; // Health per second
    damageCell(row, col, (int)((float)miningSpeed * GetFrameTime()), state->playfield);
    // Attacking
    if (pInput->attackPressed)
    {
        WeaponSlot* slot = &player->inventory[player->activeSlot];
        if (slot->quantity > 0 && slot->type < MAX_WEAPON_TYPE)
        {
            slot->quantity--;
            printf("Using a %d! (%d left)\n", slot->type, slot->quantity);
            spawnBomb(slot->type, player->position, state->bombs);
        }
    }
    // Switching weapons
    if (pInput->wepSelectPressed)
    {
        // Find next available slot
        for (int i = 0; i < INVENTORY_SIZE; i++)
        {
            int nextSlot = (player->activeSlot + i + 1) % INVENTORY_SIZE;
            if (player->inventory[nextSlot].quantity > 0 && player->inventory[nextSlot].type < MAX_WEAPON_TYPE)
            {
                player->activeSlot = nextSlot;
                break;
            }
        }
    }
}

static void updatePlayers(RoundState* state, const InputState* input)
{
	// todo: make this work when some players are inactive
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
		if (state->players[i].active)
		{
        	updatePlayer(state, i, &input->player[i]);
		}
    }
}

// Returns true if it's game over!
static bool gameOverCondition(RoundState* state)
{
    int numPlayers = getNumAlivePlayers(state->players);
    return numPlayers <= 1; // uh, if there's 1 guy left, he is the winner
}
