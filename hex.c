#include <math.h>
#include "include/hex.h"
#include "include/common.h"

Cube cubeRound(Cube frac)
{
	float q = round(frac.q);
	float r = round(frac.r);
	float s = round(frac.s);

	float qDiff = fabs(q - frac.q);
	float rDiff = fabs(r - frac.r);
	float sDiff = fabs(s - frac.s);

	if (qDiff > rDiff && qDiff > sDiff)
	{
		q = -r - s;
	}
	else if (rDiff > sDiff)
	{
		r = -q - s;
	}
	else
	{
		s = -q - r;
	}

	return (Cube){q, r, s};
}

Axial cubeToAxial(Cube cube)
{
	float q = cube.q;
	float r = cube.r;
	return (Axial){ q, r };
}

Cube axialToCube(Axial hex)
{
	float q = hex.q;
	float r = hex.r;
	float s = -q - r;
	return (Cube){ q, r, s };
}

Axial axialRound(Axial hex)
{
	return cubeToAxial(cubeRound(axialToCube(hex)));
}

float clampf(float d, float min, float max) {
  const float t = d < min ? min : d;
  return t > max ? max : t;
}

Axial toCellCoords(Vector2 point)
{
	const float size = CELL_H_SPACING * (2.0f / 3.0f);
	float q = point.x * (2.0f/3.0f) / size;
	float r = (-point.x / 3.0f + SQRT_3/3.0f * point.y) / size;
	q = clampf(q, 0, FIELD_W - 1);
	r = clampf(r, 0, FIELD_H - 1);
	return axialRound((Axial){ q, r });
	//printf("Point (%f, %f) is in cell (%d, %d)\n", point.x, point.y, *col, *row);
}
