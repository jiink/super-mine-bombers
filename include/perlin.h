#ifndef PERLIN_H
#define PERLIN_H

#define GRID_SIZE 256
#define PERSISTENCE 0.5
#define OCTAVES 2

float perlin2d(float x, float y, float freq, int depth);
void setSeed(int seed);

#endif // PERLIN_H