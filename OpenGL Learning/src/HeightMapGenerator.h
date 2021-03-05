#pragma once
#include <noise/noise.h>
#include <libnoiseutils/noiseutils.h>

class HeightMapGenerator
{
public:
	HeightMapGenerator(int resolution);
	noise::utils::NoiseMap& GetHeightMap(int x, int z);
	inline int GetResolution() { return resolution; }
private:
	int resolution;

	module::Perlin perlinNoise;

	noise::utils::NoiseMap heightMap;
	noise::utils::NoiseMapBuilderPlane heightMapBuilder;
};

