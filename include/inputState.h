#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include <stdbool.h>
#include "common.h"
#include "raylib.h"

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	ATTACK,
	NUM_ACTIONS
} BindingAction;

typedef struct {
	BindingAction action;
	int key;
} KeyBind;

typedef struct {
	KeyBind bindings[NUM_ACTIONS];
} PlayerBindings;

typedef struct {
	PlayerBindings player[MAX_PLAYERS];
} Bindings;

typedef struct {
    Vector2 direction;
    bool attack;
    bool attackPressed;
} PlayerInputState;

typedef struct {
	PlayerInputState player[MAX_PLAYERS];
} InputState;

void initInputState(InputState* inputState, Bindings* bindings);
void updateInputState(InputState* inputState, Bindings* bindings);

#endif // INPUTSTATE_H