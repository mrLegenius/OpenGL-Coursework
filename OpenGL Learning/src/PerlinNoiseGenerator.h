#pragma once
constexpr auto maxPrimeIndex = 10;

class PerlinNoiseGenerator
{
private:
    int numX = 512,
        numY = 512,
        numOctaves = 7;
    float persistence = 0.5f;

    int primeIndex = 0;

    int primes[maxPrimeIndex][3] = {
        { 995615039, 600173719, 701464987 },
        { 831731269, 162318869, 136250887 },
        { 174329291, 946737083, 245679977 },
        { 362489573, 795918041, 350777237 },
        { 457025711, 880830799, 909678923 },
        { 787070341, 177340217, 593320781 },
        { 405493717, 291031019, 391950901 },
        { 458904767, 676625681, 424452397 },
        { 531736441, 939683957, 810651871 },
        { 997169939, 842027887, 423882827 }
    };

    float Noise(int i, int x, int y);

    float SmoothedNoise(int i, int x, int y);

    float Interpolate(float a, float b, float x);

    float InterpolatedNoise(int i, float x, float y);
public:
    PerlinNoiseGenerator(unsigned int width = 512, unsigned int height = 512, unsigned int octaves = 7, float persistence = 0.5f, unsigned long long seed = 0);
    float ValueNoise_2D(float x, float y);
};

