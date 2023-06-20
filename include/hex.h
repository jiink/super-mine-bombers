#ifndef HEX_H
#define HEX_H

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

#endif