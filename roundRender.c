// hex stuff reference: https://www.redblobgames.com/grids/hexagons/

#include <stdio.h>
#include <math.h>
#include "include/roundRender.h"
#include "include/raymath.h"
#include "include/hex.h"
#include "include/vitmap.h"
#include "include/rlgl.h"

Camera2D camera = { 0 };

Vitmap* characterSprite;

Color cellColorLookup[MAX_CELL_TYPES] = {
	[AIR] = (Color) { 255, 0, 0, 100 },
	[DIRT] = BROWN,
	[STONE] = GRAY,
	[TREASURE] = YELLOW,
	[WALL] = ORANGE,
};

// Local functions

static Vector2 worldToDrawCoords(Vector2 worldCoords);
static void drawHexagon(Vector2 center, Color color);
static void drawPlayfield(const Cell playfield[FIELD_H][FIELD_W]);
static void drawPlayer(const Player* player);
static void drawPlayers(const Player players[MAX_PLAYERS]);
static void drawBombs(const Bomb bombs[MAX_BOMBS]);
static float getPlayersGreatestDistance(const Player players[MAX_PLAYERS]);
static Vector2 getPlayersMidpoint(const Player players[MAX_PLAYERS]);
static void initCamera(int screenWidth, int screenHeight);
static void updateCamera(Camera2D *cam, const Player players[MAX_PLAYERS], float smoothness);

// Function definitions

void initRoundRender(int screenWidth, int screenHeight)
{
    rlDisableBackfaceCulling(); // TODO: Turn clockwise triangles into counter-clockwise ones during vitmap shape baking so we don't have to do this
    initCamera(screenWidth, screenHeight);
    characterSprite = loadAndBakeVitmap("assets/cubil.vmp");
}

void drawRoundState(const RoundState *state, const InputState *input)
{
	cellColorLookup[WALL] = ColorFromHSV((float)GetTime() * 10.0f, 0.5f, 0.5f);
    BeginMode2D(camera);

        ClearBackground(WHITE);
                
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
    //DrawText(TextFormat("%f", camera.zoom), 10, 30, 60, BLUE);
    // Draw each player's selected weapon and quantity
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        const Player* player = &state->players[i];
        if (!player->active) continue;
        Vector2 drawPos = { 10.0f, 10.0f + 20.0f * i };
        DrawText(TextFormat("%s: %d", getWeaponName(player->inventory[player->activeSlot].type), player->inventory[player->activeSlot].quantity), drawPos.x, drawPos.y, 20, state->players[i].color);
    }
    // Draw player 1's:
    //currentSpeed
    // targetSpeed
    // topSpeed
    // acceleration
    // deceleration
    DrawText(TextFormat("Speed: %f", state->players[0].velocity.y), 10, 100, 20, state->players[0].color);
    // DrawText(TextFormat("Target: %f", state->players[0].speed), 10, 120, 20, state->players[0].color);
    DrawText(TextFormat("Top: %f", state->players[0].defSpeed), 10, 140, 20, state->players[0].color);
    // DrawText(TextFormat("Accel: %f", state->players[0].rotSpeed), 10, 160, 20, state->players[0].color);
    // DrawText(TextFormat("Decel: %f", state->players[0].inputDirInterp), 10, 180, 20, state->players[0].color);
    
}

// Everything is a little squished
// normal hex -> our hex is multiplying its height by (4 / 3sqrt3)
static Vector2 worldToDrawCoords(Vector2 worldCoords)
{
    const float heightMult = 4.0f / (3.0f * SQRT_3);
    return (Vector2) {
        worldCoords.x,
        worldCoords.y * heightMult
    };
}

static void drawHexagon(Vector2 center, Color color)
{
    const float size = 0.626f; // Seams appear at 0.625f
    const float height = 4.0f/3.0f * size;
    const float THIRD = 1.0f / 3.0f;
    Vector2 verts[8] = {
        // Center for triangle fan
        (Vector2){ center.x, center.y },
        // Verts along hexagon counter-clockwise so it shows up
        (Vector2){ center.x + size, center.y },
        (Vector2){ center.x + THIRD * size, center.y - height * 0.5f },
        (Vector2){ center.x - THIRD * size, center.y - height * 0.5f },
        (Vector2){ center.x - size, center.y },
        (Vector2){ center.x - THIRD * size, center.y + height * 0.5f },
        (Vector2){ center.x + THIRD * size, center.y + height * 0.5f },
        // Return to first vert of hexagon for triangle fan to be complete
        (Vector2){ center.x + size, center.y },
    };
    DrawTriangleFan(verts, 8, color);
}

static void drawPlayfield(const Cell playfield[FIELD_H][FIELD_W])
{
    for (int col = 0; col < FIELD_W; col++)
    {
        for (int row = 0; row < FIELD_H; row++)
        {
            const Vector2 cell_pos = worldToDrawCoords(toWorldCoords((Axial){ col, row }));
                        
            CellType thisCell = playfield[row][col].type;
            if (thisCell == AIR)
                continue;

            Color color = ColorBrightness(cellColorLookup[thisCell], -(100 - playfield[row][col].health) / 100.0f);
            //DrawPoly(cell_pos, 6, CELL_SCALE * (2.0/3.0) * (SQRT_3/2.0) * 0.1, 30.0, color);
            drawHexagon(cell_pos, color);
        }
    }
}

static void drawPlayer(const Player* player)
{
	if (!player->active) return;
	const float diameter = 0.15f;
	Vector2 drawPos = worldToDrawCoords(player->position);
	//DrawCircleV(drawPos, diameter, player->color);
	// Draw health ring
	DrawRing(drawPos, diameter + 0.1f, diameter + 0.2f, 0, player->health * 1.80f, 30, player->color);
    // Draw sprite
    drawVitmap(characterSprite,
        Vector2Add(drawPos, (Vector2) {-0.35f, -0.75f}),
        (Vector2) { 0.05, 0.05 },
        0.0f);
}

static void drawPlayers(const Player players[MAX_PLAYERS])
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        drawPlayer(&players[i]);
    }
}

static void drawBombs(const Bomb bombs[MAX_BOMBS])
{
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        if (!bombs[i].active) continue;
        float diameter = sinf(bombs[i].fuseTimer * 30.0f) * 0.1f + 0.2f;
        Vector2 drawCoords = worldToDrawCoords(bombs[i].position);
        DrawCircleV(drawCoords, diameter + 0.1f, WHITE);
        switch (bombs[i].type)
        {
            case BOMB:
                DrawCircleV(drawCoords, diameter, RED);
                break;
            case MINE:
                DrawCircleV(drawCoords, diameter, BLUE);
                break;
            case SHARP_BOMB:
                DrawCircleV(drawCoords, diameter, GREEN);
                break;
            default:
                break;
        }
        // Draw fuse timer ring
        float relativeTimeLeft = bombs[i].fuseTimer / getWeaponProperties(bombs[i].type).startingFuse;
        DrawRing(drawCoords, diameter + 0.3f, diameter + 0.4f, 0, relativeTimeLeft * 360.0f, 30, WHITE);
        DrawRing(drawCoords, diameter + 0.2f, diameter + 0.3f, 0, relativeTimeLeft * 360.0f, 30, BLACK);
    }
}

// Find the distance that lies between the 2 players who are the farthest apart
static float getPlayersGreatestDistance(const Player players[MAX_PLAYERS])
{
    int numPlayers = getNumAlivePlayers(players);
    float greatestDistance = 0.1f;
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

static Vector2 getPlayersMidpoint(const Player players[MAX_PLAYERS])
{
    Vector2 accumulation = { 0.0f, 0.0f };
    int numPlayers = getNumAlivePlayers(players);
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

static void initCamera(int screenWidth, int screenHeight)
{
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 20.0f;
}

static void updateCamera(Camera2D *cam, const Player players[MAX_PLAYERS], float smoothness)
{
    // Get camera target 🎯
    Vector2 targetPosition = worldToDrawCoords( getPlayersMidpoint(players));
    float camLerpFac = 0.05;
    // 📷 Smoothly interpolate camera's position to the camera target
    cam->target = Vector2Lerp(cam->target, targetPosition, camLerpFac);

    // Determine 🔍 zoom needed to have all players 👯‍♂️👯‍♀️ in view
    float zoomMultiplier = 1.0f;
	float minZoom = 7.0f;
	float maxZoom = 100.0f;
	float zoomTarget = zoomMultiplier * (GetScreenHeight() / getPlayersGreatestDistance(players));
	if (zoomTarget < minZoom) zoomTarget = minZoom;
	if (zoomTarget > maxZoom) zoomTarget = maxZoom;
    
    // Camera zoom --> target zoom, smooothly🌊 😸
	cam->zoom = Lerp(cam->zoom, zoomTarget, 0.1f);
}

