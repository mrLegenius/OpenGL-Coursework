#include "PerlinNoiseGenerator.h"
#include <cmath>

PerlinNoiseGenerator::PerlinNoiseGenerator(unsigned int width, unsigned int height, unsigned int octaves, float persistence, unsigned long long seed)
    : numX(width)
    , numY(height)
    , numOctaves(octaves)
    , persistence(persistence) 
{
    srand(seed);
    primeIndex = rand() % maxPrimeIndex;
}

float PerlinNoiseGenerator::ValueNoise_2D(float x, float y) {
    float total = 0,
        frequency = pow(2, numOctaves),
        amplitude = 1;
    for (int i = 0; i < numOctaves; ++i) {
        frequency /= 2;
        amplitude *= persistence;
        total += InterpolatedNoise((primeIndex + i) % maxPrimeIndex,
            x / frequency, y / frequency) * amplitude;
    }
    return total / frequency;
}

float PerlinNoiseGenerator::InterpolatedNoise(int i, float x, float y) {
    int integer_X = x;
    float fractional_X = x - integer_X;
    int integer_Y = y;
    float fractional_Y = y - integer_Y;

    float v1 = SmoothedNoise(i, integer_X, integer_Y),
        v2 = SmoothedNoise(i, integer_X + 1, integer_Y),
        v3 = SmoothedNoise(i, integer_X, integer_Y + 1),
        v4 = SmoothedNoise(i, integer_X + 1, integer_Y + 1),
        i1 = Interpolate(v1, v2, fractional_X),
        i2 = Interpolate(v3, v4, fractional_X);
    return Interpolate(i1, i2, fractional_Y);
}

float PerlinNoiseGenerator::SmoothedNoise(int i, int x, int y) {
    float corners = (Noise(i, x - 1, y - 1) + Noise(i, x + 1, y - 1) +
        Noise(i, x - 1, y + 1) + Noise(i, x + 1, y + 1)) / 16,
        sides = (Noise(i, x - 1, y) + Noise(i, x + 1, y) + Noise(i, x, y - 1) +
            Noise(i, x, y + 1)) / 8,
        center = Noise(i, x, y) / 4;
    return corners + sides + center;
}

float PerlinNoiseGenerator::Interpolate(float a, float b, float x) {  // cosine interpolation
    float ft = x * 3.1415927,
        f = (1 - cos(ft)) * 0.5;
    return  a * (1 - f) + b * f;
}

float PerlinNoiseGenerator::Noise(int i, int x, int y) {
    int n = x + y * 57;
    n = (n << 13) ^ n;
    int a = primes[i][0], b = primes[i][1], c = primes[i][2];
    int t = (n * (n * n * a + b) + c) & 0x7fffffff;
    return 1.0 - (float)(t) / 1073741824.0f;
}