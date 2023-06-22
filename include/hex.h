#ifndef HEX_H
#define HEX_H

#include "raylib.h"

typedef struct Cube
{
	float q;
	float r;
	float s;
} Cube;

typedef struct Axial
{
	float q;
	float r;
} Axial;

Cube cubeRound(Cube frac);
Axial cubeToAxial(Cube cube);
Cube axialToCube(Axial hex);
Axial axialRound(Axial hex);
Axial toCellCoords(Vector2 point);
Cube cubeSubtract(Cube a, Cube b);
float cubeDistance(Cube a, Cube b);
float axialDistance(Axial a, Axial b);

#endif