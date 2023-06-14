#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>
#include "raylib.h"
#include "inputState.h"

#define MAX_PLAYERS 4
#define FIELD_W 64
#define FIELD_H 64
#define MAX_WEAPONS 5
#define MAX_BOMBS 32
#define CELL_SCALE 1.0f // How wide and tall is a cell in float units?
#define PLAYER_SCALE 0.75f // Width and height of the player collision in float units

// Enum to represent the type of a cell
typedef enum {
    AIR,
    DIRT,
    STONE,
    TREASURE,
    WALL,
    NUM_CELL_TYPES
} CellType;

// Struct to represent a cell in the playfield
typedef struct {
    CellType type;
    int health;
} Cell;

// Enum to represent weapon types
typedef enum {
    BOMB,
    MINE,
    SHARP_BOMB
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
    WeaponSlot inventory[MAX_WEAPONS];
} Player;

// Struct to represent a bomb entity
typedef struct {
    bool active;
    float positionX;
    float positionY;
    int fuseTimer;
    void (*detonationFunc)();
} Bomb;

// Struct to represent the game state
typedef struct {
    Cell playfield[FIELD_W][FIELD_H];
    Player players[MAX_PLAYERS];
    Bomb bombs[MAX_BOMBS]; // MAX_BOMBS is the maximum number of bombs allowed
    float roundTime;
} GameState;


void initGameState(GameState* state);
void updateGameState(GameState* state, InputState* input);
void borderPlayfield(Cell playfield[FIELD_H][FIELD_W]);
void initPlayfield(Cell playfield[FIELD_H][FIELD_W]);
void initPlayers(Player players[MAX_PLAYERS]);

#endif  // GAMESTATE_H
