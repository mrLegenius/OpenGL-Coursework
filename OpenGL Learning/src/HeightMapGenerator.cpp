#include "HeightMapGenerator.h"
using namespace noise;

HeightMapGenerator::HeightMapGenerator(int resolution) : resolution(resolution)
{
	perlinNoise.SetFrequency(1.0f / resolution);
	heightMapBuilder.SetSourceModule(perlinNoise);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(resolution, resolution);
}

utils::NoiseMap& HeightMapGenerator::GetHeightMap(int x, int z)
{
	double left = ((double)resolution - 1) * x;
	double right = left + resolution;
	double bottom = ((double)resolution - 1) * z;
	double up = bottom + resolution;
	heightMapBuilder.SetBounds(left, right, bottom, up);
	heightMapBuilder.Build();
	return heightMap;
}
