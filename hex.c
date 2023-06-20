#include <math.h>
#include "include/hex.h"

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
