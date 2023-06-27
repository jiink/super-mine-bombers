// hex stuff reference: https://www.redblobgames.com/grids/hexagons/

#include <stdio.h>
#include <math.h>
#include "include/gameRender.h"
#include "include/raymath.h"
#include "include/hex.h"

Camera2D camera = { 0 };

Color cellColorLookup[MAX_CELL_TYPES] = {
	[AIR] = (Color) { 255, 0, 0, 100 },
	[DIRT] = BROWN,
	[STONE] = GRAY,
	[TREASURE] = YELLOW,
	[WALL] = ORANGE,
};

void initGameRender(int screenWidth, int screenHeight)
{
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 20.0f;
}

void drawPlayfield(Cell playfield[FIELD_H][FIELD_W])
{
    for (int col = 0; col < FIELD_W; col++)
    {
        for (int row = 0; row < FIELD_H; row++)
        {
            const Vector2 cell_pos = toPixelCoords((Axial){ col, row });
                        
            CellType thisCell = playfield[row][col].type;
            if (thisCell == AIR)
                continue;

            Color color = ColorBrightness(cellColorLookup[thisCell], -(100 - playfield[row][col].health) / 100.0f);
            DrawPoly(cell_pos, 6, CELL_SCALE * (2.0/3.0) * (SQRT_3/2.0), 30.0, color);
        }
    }
}

void drawPlayer(Player* player)
{
	if (!player->active) return;
	const float diameter = 0.15f;
	Vector2 drawPos = {
		player->position.x,
		player->position.y
	};
	DrawCircleV(drawPos, diameter, player->color);
	// Draw health ring
	DrawRing(drawPos, diameter + 0.1f, diameter + 0.2f, 0, player->health * 1.80f, 30, player->color);
}

void drawPlayers(Player players[MAX_PLAYERS])
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        drawPlayer(&players[i]);
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

// Find the distance that lies between the 2 players who are the farthest apart
float getPlayersGreatestDistance(Player players[MAX_PLAYERS])
{
    int numPlayers = getNumPlayers(players);
    float greatestDistance = 1.0f;
    if (numPlayers < 2)
    {
        return greatestDistance;
    }
    // Check every combination
    for (int i = 0; i < numPlayers; i++)
    {
        for (int j = 0; j < numPlayers; j++)
        {
            float dist = Vector2Length(Vector2Subtract(players[i].position, players[j].position));
            if (dist > greatestDistance)
            {
                greatestDistance = dist;
            }
        }
    }
    return greatestDistance;
}

Vector2 getPlayersMidpoint(Player players[MAX_PLAYERS])
{
    Vector2 accumulation = { 0.0f, 0.0f };
    int numPlayers = getNumPlayers(players);
	if (numPlayers == 0) return (Vector2){ 0.0f, 0.0f };
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
		if (!players[i].active) continue;
        accumulation = Vector2Add(accumulation, players[i].position);
    }
    return (Vector2) {
        accumulation.x / (float)numPlayers,
        accumulation.y / (float)numPlayers
    };
}

void updateCamera(Camera2D *cam, Player players[MAX_PLAYERS], float smoothness)
{
    // Get camera target 🎯
    Vector2 targetPosition = getPlayersMidpoint(players);
    float camLerpFac = 0.05;
    // 📷 Smoothly interpolate camera's position to the camera target
    cam->target = Vector2Lerp(cam->target, targetPosition, camLerpFac);

    // Determine 🔍 zoom needed to have all players 👯‍♂️👯‍♀️ in view
    float zoomOffset = 1.0f;
    float zoomMultiplier = 0.75f;
	float minZoom = 16.0f;
	float maxZoom = 50.0f;
	float zoomTarget = zoomMultiplier * (GetScreenHeight() / getPlayersGreatestDistance(players));
	if (zoomTarget < minZoom) zoomTarget = minZoom;
	if (zoomTarget > maxZoom) zoomTarget = maxZoom;
    
    // Camera zoom --> target zoom, smooothly🌊 😸
	cam->zoom = Lerp(cam->zoom, zoomTarget, 0.1f);
}

void drawGameState(GameState *state, InputState *input)
{
	cellColorLookup[WALL] = ColorFromHSV((float)GetTime() * 10.0f, 0.5f, 0.5f);

    BeginDrawing();
        BeginMode2D(camera);

            ClearBackground(BLACK);
                    
            drawPlayfield(state->playfield);
            drawPlayers(state->players);
            drawBombs(state->bombs);

            updateCamera(&camera, state->players, 0.1);
            //camera.zoom += ((float)GetMouseWheelMove() * 3.0f);

        EndMode2D();

        // UI
        // draw fps
		//DrawText(TextFormat("%d", GetFPS()), 10, 10, 20, GREEN);
		// draw camera zoom
		DrawText(TextFormat("%f", camera.zoom), 10, 30, 60, BLUE);
    EndDrawing();
}