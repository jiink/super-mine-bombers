#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>
#include "common.h"
#include "raylib.h"
#include "inputState.h"

#define INVENTORY_SIZE 5
#define MAX_BOMBS 32

// Enum to represent the type of a cell
typedef enum {
    AIR,
    DIRT,
    STONE,
    TREASURE,
    WALL,
    MAX_CELL_TYPES
} CellType;

// Struct to represent a cell in the playfield
typedef struct {
    CellType type;
    char health;
} Cell;

typedef struct {
	float resistance;
	bool solid;
} CellProperties;

// Enum to represent weapon types
typedef enum {
    BOMB,
    MINE,
    SHARP_BOMB,
    MAX_WEAPONS
} WeaponType;

// Struct to represent a weapon in the inventory
typedef struct {
    WeaponType type;
    int quantity;
} WeaponSlot;

// Struct to represent a player
typedef struct {
    bool active;
    int health;
    Vector2 position;
    Color color;
    int score;
    WeaponSlot inventory[INVENTORY_SIZE];
    int activeSlot;
} Player;

typedef struct {
    float startingFuse;
    int damage;
    float radius;
	void (*detonationFunc)(Vector2, float, float, Cell[FIELD_H][FIELD_W], Player[MAX_PLAYERS]);
} WeaponProperties;

// Struct to represent a bomb entity
typedef struct {
    bool active;
    WeaponType type;
    Vector2 position;
    float fuseTimer;
} Bomb;

// Struct to represent the game state
typedef struct {
    Cell playfield[FIELD_W][FIELD_H];
    Player players[MAX_PLAYERS];
    Bomb bombs[MAX_BOMBS]; // MAX_BOMBS is the maximum number of bombs allowed
    float roundTime;
} GameState;

int getNumAlivePlayers(Player players[MAX_PLAYERS]);
void initGameState(GameState* state);
void updateGameState(GameState* state, InputState* input);
void borderPlayfield(Cell playfield[FIELD_H][FIELD_W]);
void initPlayfield(Cell playfield[FIELD_H][FIELD_W]);
void initPlayers(Player players[MAX_PLAYERS]);
void damagePlayer(Player* player, int damage);
void damageCell(int row, int col, int damage, Cell playfield[FIELD_H][FIELD_W]);
void damageCellAtPos(Vector2 pos, int damage, Cell playfield[FIELD_H][FIELD_W]);
void explode(Vector2 position, float radius, float damage, Cell playfield[FIELD_H][FIELD_W], Player players[MAX_PLAYERS]);
WeaponProperties getWeaponProperties(WeaponType type);
CellProperties getCellProperties(CellType type);

#endif  // GAMESTATE_H
