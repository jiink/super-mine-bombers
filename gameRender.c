// hex stuff reference: https://www.redblobgames.com/grids/hexagons/

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
	DrawRing(drawPos, diameter + 0.1f, diameter + 0.2f, 0, player->health * 3.60f, 30, ColorAlpha(player->color, 0.5f));
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
    float greatestDistance = 4.0f;
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
    for (int i = 0; i < numPlayers; i++)
    {
        accumulation = Vector2Add(accumulation, players[i].position);
    }
    return (Vector2) {
        accumulation.x / (float)numPlayers,
        accumulation.y / (float)numPlayers
    };
}

void updateCamera(Camera2D *cam, Player players[MAX_PLAYERS], float smoothness)
{
    // Calculate the difference between the current position and the target position
    Vector2 targetPosition = getPlayersMidpoint(players);
    Vector2 positionDiff = Vector2Subtract(targetPosition, cam->target);

    // Calculate the new position with interpolation
    Vector2 newPosition = Vector2Add(cam->target, Vector2Scale(positionDiff, smoothness));

    // Update the camera position
    cam->offset = Vector2Subtract(cam->offset, Vector2Subtract(newPosition, cam->target));
    cam->target = newPosition;

    // Zoom to show all players
    float zoomOffset = 50.0f;
    float zoomMultiplier = 10.0f;
    cam->zoom = zoomOffset - logf(getPlayersGreatestDistance(players)) * zoomMultiplier;
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
            //camera.zoom += ((float)GetMouseWheelMove() * 3.0f);

            if (camera.zoom > 50.0f) camera.zoom = 50.0f;
            else if (camera.zoom < 0.5f) camera.zoom = 0.5f;

        EndMode2D();

        // UI
        //char inputReport[128];
        //sprintf(inputReport, "Direction: %f, %f\nAttack: %d", input->direction.x, input->direction.y, input->attack);
        //DrawText(inputReport, 10, 10, 20, GRAY);
    EndDrawing();
}