#ifndef ROUNDSTATE_H
#define ROUNDSTATE_H

#include <stdbool.h>
#include "common.h"
#include "raylib.h"
#include "inputState.h"

#define INVENTORY_SIZE 8
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

typedef enum {
    BOMB,
    MINE,
    SHARP_BOMB,
    MAX_WEAPON_TYPE
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
    Color color;
    int score;
    WeaponSlot inventory[INVENTORY_SIZE];
    int activeSlot;
    Vector2 position;
    Vector2 velocity;
    Vector2 lastVelocity;
    float defSpeed; //How fast you walk by default
    float targetSpeed; //How fast you want to walk
    float defFriction;
    float friction;
} Player;

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
    bool roundOver;
} RoundState;

typedef struct {
    float startingFuse;
    int damage;
    float radius;
    int price;
	bool (*detonationFunc)(Vector2, float, float, Cell[FIELD_H][FIELD_W], Player[MAX_PLAYERS]); // Should return true if the bomb sucessfully detonated. False if it did not (e.g. if a mine did not find a reason to explode)
    void (*updateFunc)(Bomb*, const RoundState*);
} WeaponProperties;

int getNumAlivePlayers(Player players[MAX_PLAYERS]);
void initRoundState(RoundState* state);
void updateRoundState(RoundState* state, InputState* input);
bool giveItem(Player* player, WeaponType type, int amount);
WeaponProperties getWeaponProperties(WeaponType type);
const char* getWeaponName(WeaponType type);
CellProperties getCellProperties(CellType type);

#endif  // ROUNDSTATE_H
