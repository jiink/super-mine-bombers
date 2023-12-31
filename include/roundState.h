#ifndef ROUNDSTATE_H
#define ROUNDSTATE_H

#include <stdbool.h>
#include "common.h"
#include "raylib.h"
#include "inputState.h"

#define INVENTORY_SIZE 8
#define MAX_BOMBS 128

// Enum to represent the type of a cell
typedef enum {
    AIR,
    DIRT,
    STONE,
    TREASURE,
    SUPER_TREASURE,
    METAL,
    WALL,
    MAX_CELL_TYPES
} CellType;

// Struct to represent a cell in the playfield
typedef struct {
    CellType type;
    float health;
} Cell;

typedef struct {
	float resistance;
	bool solid;
} CellProperties;

typedef enum {
    NONE,
    BOMB,
    MINE,
    SHARP_BOMB,
    ROLLER,
    GRENADE,
    NUKE,
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
    int playerNum;
    int score;
    int* wallet;
    WeaponSlot inventory[INVENTORY_SIZE];
    int activeSlot;
    Vector2 facingDirection;
    Vector2 position;
    Vector2 velocity;
    Vector2 lastVelocity;
    float defSpeed; //How fast you walk by default
    float targetSpeed; //How fast you want to walk
    float defFriction;
    float friction;
    WeaponType heldBomb;
    bool playDeploySound;
    bool isWinner;
    bool isInvincible;
} Player;

// Struct to represent a bomb entity
typedef struct {
    bool active;
    WeaponType type;
    Vector2 position;
    float height;
    float heightVelocity;
    float gravity;
    float fuseTimer;
    Player* owner;
    Vector2 velocity;
    bool playExplosionSound;
} Bomb;

// Struct to represent the game state
typedef struct {
    Cell playfield[FIELD_H][FIELD_W];
    Player players[MAX_PLAYERS];
    Bomb bombs[MAX_BOMBS]; // MAX_BOMBS is the maximum number of bombs allowed
    float roundTime;
    bool roundOver;
    bool suddenDeath;
    bool playSuddenDeathSound;
} RoundState;

typedef struct {
    float startingFuse;
    int damage;
    float radius;
    int price;
	bool (*detonationFunc)(Vector2, float, float, Cell[FIELD_H][FIELD_W], RoundState*); // Should return true if the bomb sucessfully detonated. False if it did not (e.g. if a mine did not find a reason to explode)
    void (*updateFunc)(Bomb*, const RoundState*);
    char name[64];
} WeaponProperties;

extern Color playerColors[MAX_PLAYERS];

int getNumAlivePlayers(const Player players[MAX_PLAYERS]);
void initRoundState(RoundState* state, int numPlayers, int* wallets[MAX_PLAYERS]);
void updateRoundState(RoundState* state, const InputState* input);
bool giveItem(Player* player, WeaponType type, int amount);
WeaponProperties getWeaponProperties(WeaponType type);
const char* getWeaponName(WeaponType type);
CellProperties getCellProperties(CellType type);

#endif  // ROUNDSTATE_H
