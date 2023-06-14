#include <stdio.h>
#include "include/gameRender.h"
#include "include/raymath.h"

Camera2D camera = { 0 };

void initGameRender(int screenWidth, int screenHeight)
{
	initCellColorLookup();
	camera.target = (Vector2){ 0, 0 };
	camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 10.0f;
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
			// Draw a dot here
			const Rectangle dot = { col, row, 0.1, 0.1 };
			DrawRectangleRec(dot, BLACK);
            
			CellType thisCell = playfield[row][col].type;
            if (thisCell == AIR) continue;
            const float size = 1.0f;
            const float padding = 0.0f;
            Vector2 pos = { col * (size + padding), row * (size + padding) };
            Vector2 tileSize = { size, size };
			Rectangle cellRect = { pos.x, pos.y, tileSize.x, tileSize.y };
            Color color = cellColorLookup[thisCell];
            DrawRectangleRec(cellRect, color);
        }
    }
}

void drawPlayers(Player players[MAX_PLAYERS])
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (!players[i].active) continue;
		const float diameter = 0.35f;
		DrawCircleV(players[i].position, diameter, players[i].color);
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


void drawGameState(GameState *state)
{
	BeginDrawing();
		BeginMode2D(camera);

			ClearBackground(RAYWHITE);
					
			drawPlayfield(state->playfield);
			drawPlayers(state->players);

			updateCamera(&camera, state->players, 0.1);
			camera.zoom += ((float)GetMouseWheelMove() * 3.0f);

			if (camera.zoom > 50.0f) camera.zoom = 50.0f;
			else if (camera.zoom < 1.0f) camera.zoom = 1.0f;

		EndMode2D();
	EndDrawing();
}