#include <stdio.h>
#include <math.h>
#include "include/gameRender.h"
#include "include/raymath.h"

Camera2D camera = { 0 };

void initGameRender(int screenWidth, int screenHeight)
{
	initCellColorLookup();
	camera.target = (Vector2){ 0, 0 };
	camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 20.0f;
}

void initCellColorLookup()
{
    cellColorLookup[AIR] = ColorAlpha(RED, 0.5);
    cellColorLookup[DIRT] = BROWN;
    cellColorLookup[STONE] = BLACK;
    cellColorLookup[TREASURE] = YELLOW;
    cellColorLookup[WALL] = BLUE;
}

void drawPlayfield(Cell playfield[FIELD_H][FIELD_W])
{
    for (int col = 0; col < FIELD_W; col++)
    {
        for (int row = 0; row < FIELD_H; row++)
        {
			const Vector2 cell_pos = {
				col * CELL_H_SPACING * CELL_SCALE,
				row * CELL_V_SPACING * CELL_SCALE + (0.5 * CELL_V_SPACING * col * CELL_SCALE)
			};
			
			// Draw a dot here
			const Rectangle dot = { cell_pos.x, cell_pos.y, 0.1, 0.1 };
			DrawRectangleRec(dot, BLACK);
            
			CellType thisCell = playfield[row][col].type;
            if (thisCell == AIR)
				continue;

            Color color = ColorBrightness(cellColorLookup[thisCell], -(100 - playfield[row][col].health) / 100.0f);
            DrawPoly(cell_pos, 6, CELL_SCALE * (2.0/3.0) * (SQRT_3/2.0), 30.0, color);
        }
    }
}

void drawPlayers(Player players[MAX_PLAYERS])
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (!players[i].active) continue;
		//const Rectangle hitboxVis = { players[i].position.x - PLAYER_SCALE / 2.0f, players[i].position.y - PLAYER_SCALE / 2.0f, PLAYER_SCALE, PLAYER_SCALE };
		//DrawRectangleRec(hitboxVis, PINK);
		const float diameter = 0.35f;
		Vector2 drawPos = {
			players[i].position.x - CELL_H_SPACING / 2.0f,
			players[i].position.y - CELL_V_SPACING / 2.0f
		};
		DrawCircleV(drawPos, diameter, players[i].color);
	}
}

void drawBombs(Bomb bombs[MAX_BOMBS])
{
	for (int i = 0; i < MAX_BOMBS; i++)
	{
		if (!bombs[i].active) continue;
		float diameter = sinf(bombs[i].fuseTimer * 30.0f) * 0.1f + 0.2f;
		DrawCircleV(bombs[i].position, diameter + 0.1f, WHITE);
		DrawCircleV(bombs[i].position, diameter, MAGENTA);
	}
}

void updateCamera(Camera2D *cam, Player players[MAX_PLAYERS], float smoothness)
{
	// Calculate the difference between the current position and the target position
	Vector2 targetPosition = players[0].position;
	Vector2 positionDiff = Vector2Subtract(targetPosition, cam->target);

	// Calculate the new position with interpolation
	Vector2 newPosition = Vector2Add(cam->target, Vector2Scale(positionDiff, smoothness));

	// Update the camera position
	cam->offset = Vector2Subtract(cam->offset, Vector2Subtract(newPosition, cam->target));
	cam->target = newPosition;
}


void drawGameState(GameState *state, InputState *input)
{
	BeginDrawing();
		BeginMode2D(camera);

			ClearBackground(RAYWHITE);
					
			drawPlayfield(state->playfield);
			drawPlayers(state->players);
			drawBombs(state->bombs);

			updateCamera(&camera, state->players, 0.1);
			camera.zoom += ((float)GetMouseWheelMove() * 3.0f);

			if (camera.zoom > 50.0f) camera.zoom = 50.0f;
			else if (camera.zoom < 1.0f) camera.zoom = 1.0f;

		EndMode2D();

		// UI
		char inputReport[128];
		sprintf(inputReport, "Direction: %f, %f\nAttack: %d", input->direction.x, input->direction.y, input->attack);
		DrawText(inputReport, 10, 10, 20, GRAY);
	EndDrawing();
}